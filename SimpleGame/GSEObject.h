#pragma once

#include "GSEGlobals.h"

class GSEObject
{
public:
	GSEObject();
	~GSEObject();

	void Update(float elapsedTimeInSec, GSEUpdateParams* param);

	void SetPosition(float x, float y, float depth);
	void SetRelPosition(float x, float y, float depth);
	void SetSize(float sx, float sy);
	void SetVel(float x, float y);
	void SetAcc(float x, float y);
	void SetMass(float x);
	void SetType(GSEObjectType type);
	void SetState(GSEObjectState state);
	void SetParentID(int parentID);
	void SetLifeTime(float lifeTime);
	void SetLife(float life);
	void SetApplyPhysics(bool bPhy);
	void SetCoolTime(float coolTime);
	void ResetRemainingCoolTime();
	void SetStickToParent(bool bStick);

	void GetPosition(float* x, float* y, float* depth);
	void GetRelPosition(float* x, float* y, float* depth);
	void GetSize(float* sx, float* sy);
	void GetVel(float* x, float* y);
	void GetAcc(float* x, float* y);
	void GetMass(float* x);
	void GetType(GSEObjectType* type);
	void GetState(GSEObjectState* state);
	int GetParentID();
	float GetLife();
	float GetLifeTime();
	bool GetApplyPhysics();
	float GetRemainingCoolTime();
	bool GetStickToParent();
	void SetTextureID(int id);
	int GetTextureID();

	bool IsAnimation();

	int GetAnimationState() const;
	void SetAnimationState(int AnimationState);

	void GetAnimationFrame(int* AnimationFrame, float* AnimationFrameTime, float* AnimationFrameSpeed);
	void SetAnimationFrame(int AnimationFrame, float AnimationFrameTime, float AnimationFrameSpeed);

	void SetAnimationFrameCnt(int idle, int run, int attack, int die);
    int GetAnimationFrameCnt();

	void SetAnimationTextureID(int idle, int run, int attack, int die);
	int GetAnimationTextureID();

private:
	float m_PositionX, m_PositionY;
	float m_RelPositionX, m_RelPositionY;
	float m_Depth;
	float m_RelDepth;
	float m_SizeX, m_SizeY;
	float m_VelX, m_VelY;
	float m_AccX, m_AccY;
	float m_Mass;
	int m_Parent; //hero id, npc id
	float m_LifeTime;
	float m_Life;
	bool m_ApplyPhysics;
	float m_CoolTime;
	float m_RemainingCoolTime;
	bool m_StickToParent;
	int m_TextureID;

	bool m_bAnimation;
	int m_AnimationFrame;
	float m_AnimationFrameTime;
	float m_AnimationFrameSpeed;
	int m_AnimationState;
	int m_AnimationFrameCnt[4];

	int m_AnimationTextureID[4];

	GSEObjectState m_State;
	GSEObjectType m_Type;
};

