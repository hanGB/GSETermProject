#include "stdafx.h"
#include "GSEObject.h"
#include "math.h"
#include <iostream>


GSEObject::GSEObject()
{
	m_PositionX = NON_DATA;
	m_PositionY = NON_DATA;
	m_RelPositionX = NON_DATA;
	m_RelPositionY = NON_DATA;
	m_Depth = NON_DATA;
	m_SizeX = NON_DATA;
	m_SizeY = NON_DATA;
	m_AccX = NON_DATA;
	m_AccY = NON_DATA;
	m_VelX = NON_DATA;
	m_VelY = NON_DATA;
	m_Mass = NON_DATA;

	m_Parent = -1;
	m_LifeTime = 0.f;
	m_Life = 0.f; //health point, HP
	m_ApplyPhysics = false;
	m_CoolTime = 0.3f;
	m_RemainingCoolTime = m_CoolTime;
	m_StickToParent = false;
	m_TextureID = -1;

	m_State = GSEObjectState::STATE_FALLING;
	m_Type = GSEObjectType::TYPE_ENEMY;

	m_bAnimation = false;
	m_AnimationFrame = 0;
	m_AnimationFrameTime = 0;
	m_AnimationState = ANIMATION_IDLE;

	m_dir = 1;
}

GSEObject::~GSEObject()
{

}

void GSEObject::SetTextureID(int id)
{
	m_TextureID = id;
}

int GSEObject::GetTextureID()
{
	return m_TextureID;
}

bool GSEObject::IsAnimation()
{
	return m_bAnimation;
}

int GSEObject::GetAnimationState() const
{
	return m_AnimationState;
}

void GSEObject::SetAnimationState(int AnimationState)
{
	m_AnimationState = AnimationState;
}

void GSEObject::GetAnimationFrame(int* AnimationFrame, float* AnimationFrameTime, float* AnimationFrameSpeed)
{
	*AnimationFrame = m_AnimationFrame;
	*AnimationFrameTime = m_AnimationFrameTime;
	*AnimationFrameSpeed = m_AnimationFrameSpeed[m_AnimationState];
}

void GSEObject::SetAnimationFrame(int AnimationFrame, float AnimationFrameTime)
{
	m_AnimationFrame = AnimationFrame;
	m_AnimationFrameTime = AnimationFrameTime;
}

void GSEObject::SetAnimationFrameSpeed(float idle, float run, float attack, float die)
{
	m_AnimationFrameSpeed[ANIMATION_IDLE] = idle;
	m_AnimationFrameSpeed[ANIMATION_RUN] = run;
	m_AnimationFrameSpeed[ANIMATION_ATTACK] = attack;
	m_AnimationFrameSpeed[ANIMATION_DIE] = die;
}


void GSEObject::SetAnimationFrameCnt(int idle, int run, int attack, int die)
{
	m_bAnimation = true;

	m_AnimationFrameCnt[ANIMATION_IDLE] = idle;
	m_AnimationFrameCnt[ANIMATION_RUN] = run;
	m_AnimationFrameCnt[ANIMATION_ATTACK] = attack;
	m_AnimationFrameCnt[ANIMATION_DIE] = die;
}

int GSEObject::GetAnimationFrameCnt()
{
	return m_AnimationFrameCnt[m_AnimationState];
}

void GSEObject::SetAnimationTextureID(int idle, int run, int attack, int die)
{
	m_AnimationTextureID[ANIMATION_IDLE] = idle;
	m_AnimationTextureID[ANIMATION_RUN] = run;
	m_AnimationTextureID[ANIMATION_ATTACK] = attack;
	m_AnimationTextureID[ANIMATION_DIE] = die;
}

int GSEObject::GetAnimationTextureID()
{
	return m_AnimationTextureID[m_AnimationState];
}

void GSEObject::SetEnemyType(int type)
{
	m_enemyType = type;
}

int GSEObject::GetEnemyType() const
{
	return m_enemyType;
}

void GSEObject::SetDir(int dir)
{
	m_dir = dir;
}

int GSEObject::GetDir()
{
	return m_dir;
}

void GSEObject::Update(float elapsedTimeInSec, GSEUpdateParams* param)
{
	m_RemainingCoolTime -= elapsedTimeInSec;
	m_LifeTime -= elapsedTimeInSec;

	if (m_Type != GSEObjectType::TYPE_ENEMY && m_Type != GSEObjectType::TYPE_HERO && m_Type != GSEObjectType::TYPE_BULLET)
		return;

	if (!m_ApplyPhysics)
		return;

	if (m_StickToParent)
		return;

	float t = elapsedTimeInSec;
	float tt = elapsedTimeInSec * elapsedTimeInSec;

	float accX = 0; 
	float accY = 0;

	//Calc X axis
	//friction 
	if ((fabs(m_VelX) > 0.f && m_State == GSEObjectState::STATE_GROUND) || 
		(fabs(m_VelX) > 0.f && m_Type == GSEObjectType::TYPE_BULLET))
	{
		//calce temporary
		accX = param->forceX / m_Mass;

		//sum with object'acc
		accX += m_AccX;

		//calc friction
		accX += m_VelX / fabs(m_VelX) * 0.7f * (-GSE_GRAVITY);

		//check wrong friction direction
		float tempVelX = m_VelX + accX * t;

		if (m_VelX * tempVelX < 0.f)
		{
			m_VelX = 0.f;
		}
		else
		{
			m_VelX = tempVelX;
		}

		//update velocity
		m_VelY = 0.f;
	}
	else if(m_State == GSEObjectState::STATE_GROUND || m_Type == GSEObjectType::TYPE_BULLET)
	{
		//calce temporary
		accX = param->forceX / m_Mass;

		m_VelX = m_VelX + accX * t;
	}

	//Calc Y axis
	if (m_State == GSEObjectState::STATE_GROUND || m_Type == GSEObjectType::TYPE_BULLET)
	{
		accY += m_AccY;

		//Jump!
		accY += param->forceY / m_Mass;
	}
	else if (m_State == GSEObjectState::STATE_FALLING)
	{
		accY -= GSE_GRAVITY;
	}

	m_VelY = m_VelY + accY * t;

	//update position
	if (m_VelX > 0)
		m_dir = 1;
	else if (m_VelX < 0)
		m_dir = -1;

	if (fabs(m_VelX) > PLAYER_MAXIUM_SPEED - 1)
		m_AnimationFrameSpeed[ANIMATION_RUN] = 3;
	else if (fabs(m_VelX) > PLAYER_MAXIUM_SPEED - 2.5)
		m_AnimationFrameSpeed[ANIMATION_RUN] = 2;
	else if (fabs(m_VelX) > PLAYER_MAXIUM_SPEED - 4)
		m_AnimationFrameSpeed[ANIMATION_RUN] = 1;
	else 
		m_AnimationFrameSpeed[ANIMATION_RUN] = 0.5;

	m_PositionX = m_PositionX + m_VelX * t + 0.5f * accX * tt;

	m_PositionY = m_PositionY + m_VelY * t + 0.5f * accY * tt;
}

void GSEObject::SetPosition(float x, float y, float depth)
{
	m_PositionX = x;
	m_PositionY = y;
	m_Depth = depth;
}

void GSEObject::SetRelPosition(float x, float y, float depth)
{
	m_RelPositionX = x;
	m_RelPositionY = y;
	m_RelDepth = depth;
}

void GSEObject::GetPosition(float* x, float* y, float* depth)
{
	*x = m_PositionX;
	*y = m_PositionY;
	*depth = m_Depth;
}

void GSEObject::GetRelPosition(float* x, float* y, float* depth)
{
	*x = m_RelPositionX;
	*y = m_RelPositionY;
	*depth = m_RelDepth;
}

void GSEObject::SetSize(float sx, float sy)
{
	m_SizeX = sx;
	m_SizeY = sy;
}

void GSEObject::GetSize(float* sx, float* sy)
{
	*sx = m_SizeX;
	*sy = m_SizeY;
}

void GSEObject::SetVel(float x, float y)
{
	m_VelX = x;
	m_VelY = y;
}

void GSEObject::SetAcc(float x, float y)
{
	m_AccX = x;
	m_AccY = y;
}

void GSEObject::SetMass(float x)
{
	m_Mass = x;
}

void GSEObject::SetType(GSEObjectType type)
{
	m_Type = type;
}

void GSEObject::SetState(GSEObjectState state)
{
	m_State = state;
}

void GSEObject::SetParentID(int parentID)
{
	m_Parent = parentID;
}

void GSEObject::SetLifeTime(float lifeTime)
{
	m_LifeTime = lifeTime;
}

void GSEObject::SetLife(float life)
{
	m_Life = life;
}

void GSEObject::SetApplyPhysics(bool bPhy)
{
	m_ApplyPhysics = bPhy;
}

void GSEObject::SetCoolTime(float coolTime)
{
	m_CoolTime = coolTime;
}

void GSEObject::ResetRemainingCoolTime()
{
	m_RemainingCoolTime = m_CoolTime;
}

void GSEObject::SetStickToParent(bool bStick)
{
	if (m_Parent < 0)
	{
		std::cout << "This object has no parent" << std::endl;
		return;
	}

	if (m_StickToParent && !bStick)
	{
		m_PositionX = m_PositionX + m_RelPositionX;
		m_PositionY = m_PositionY + m_RelPositionY;
	}

	m_StickToParent = bStick;
}

void GSEObject::GetVel(float* x, float* y)
{
	*x = m_VelX;
	*y = m_VelY;
}

void GSEObject::GetAcc(float* x, float* y)
{
	*x = m_AccX;
	*y = m_AccY;
}

void GSEObject::GetMass(float* x)
{
	*x = m_Mass;
}

void GSEObject::GetType(GSEObjectType* type)
{
	*type = m_Type;
}

void GSEObject::GetState(GSEObjectState* state)
{
	*state = m_State;
}

int GSEObject::GetParentID()
{
	return m_Parent;
}

float GSEObject::GetLife()
{
	return 0.0f;
}

float GSEObject::GetLifeTime()
{
	return m_LifeTime;
}

bool GSEObject::GetApplyPhysics()
{
	return m_ApplyPhysics;
}

float GSEObject::GetRemainingCoolTime()
{
	return m_RemainingCoolTime;
}

bool GSEObject::GetStickToParent()
{
	return m_StickToParent;
}
