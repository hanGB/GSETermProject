#include "stdafx.h"
#include "GSEBattle.h"

GSEBattle::GSEBattle()
{
	m_bNextState = false;
	m_NowMap = 0;
	
	m_SwordSound = getSound()->CreateShortSound("./resource/sound/short/swordSound.wav");
	m_TriggerSound = getSound()->CreateShortSound("./resource/sound/short/triggerSound.wav");

	m_HeroTexture = getRenderer()->GenPngTexture("./resource/image/battle/hero.png"); //재사용 가능!!
	m_SwordManTexture;
	m_GunManTexture;

	m_RailRoadMapTexture = getRenderer()->GenPngTexture("./resource/image/battle/mapRailLoad.png");
	m_FireMapTexture = getRenderer()->GenPngTexture("./resource/image/battle/mapFire.png");

	//Create Hero
	m_HeroID = AddObject(0, 0, 0, 0.8, 1.6, 0, 0, 0, 0, 50);
	getObject(m_HeroID)->SetType(GSEObjectType::TYPE_HERO);
	getObject(m_HeroID)->SetApplyPhysics(true);
	getObject(m_HeroID)->SetLife(100000000.f);
	getObject(m_HeroID)->SetLifeTime(100000000.f);
	getObject(m_HeroID)->SetTextureID(m_HeroTexture);

	int floor = AddObject(-1.25, -2.5, 0, 2, 0.3, 0, 0, 0, 0, 10000);
	getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(floor)->SetApplyPhysics(true);
	getObject(floor)->SetLife(100000000.f);
	getObject(floor)->SetLifeTime(100000000.f);

	floor = AddObject(+1.25, 0, 0, 1, 0.3, 0, 0, 0, 0, 10000);
	getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(floor)->SetApplyPhysics(true);
	getObject(floor)->SetLife(100000000.f);
	getObject(floor)->SetLifeTime(100000000.f);

	floor = AddObject(0, -3.6, 0, 67, 0.3, 0, 0, 0, 0, 10000);
	getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(floor)->SetApplyPhysics(true);
	getObject(floor)->SetLife(100000000.f);
	getObject(floor)->SetLifeTime(100000000.f);

	m_BackGroundSound = getSound()->CreateBGSound("./resource/sound/bg/battleSound.mp3");
	
	// 임시 무음
	//getSound()->PlayBGSound(m_BackGroundSound, true, 0.5f);
}

GSEBattle::~GSEBattle()
{

}

void GSEBattle::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	//do garbage collecting
	DoGarbageCollect();

	GSEUpdateParams othersParam;
	GSEUpdateParams heroParam;
	memset(&othersParam, 0, sizeof(GSEUpdateParams));
	memset(&heroParam, 0, sizeof(GSEUpdateParams));

	//calc force
	float forceAmount = 400.f;
	if (inputs->KEY_W)
	{
		heroParam.forceY += 20 * forceAmount;
	}
	if (inputs->KEY_A)
	{
		heroParam.forceX -= forceAmount;
	}
	if (inputs->KEY_S)
	{
		heroParam.forceY -= forceAmount;
	}
	if (inputs->KEY_D)
	{
		heroParam.forceX += forceAmount;
	}

	//sword
	float swordPosX = 0.f;
	float swordPosY = 0.f;

	if (inputs->ARROW_LEFT) swordPosX += -1.f;
	if (inputs->ARROW_RIGHT) swordPosX += 1.f;
	if (inputs->ARROW_DOWN) swordPosY += -1.f;
	if (inputs->ARROW_UP) swordPosY += 1.f;
	float swordDirSize = sqrtf(swordPosX * swordPosX + swordPosY * swordPosY);
	if (swordDirSize > 0.f)
	{
		float norDirX = swordPosX / swordDirSize;
		float norDirY = swordPosY / swordDirSize;

		float aX, aY, asX, asY;
		float bX, bY, bsX, bsY;
		float temp;

		getObject(m_HeroID)->GetPosition(&aX, &aY, &temp);
		getObject(m_HeroID)->GetSize(&asX, &asY);
		if (getObject(m_HeroID)->GetRemainingCoolTime() < 0.f)
		{
			int swordID = AddObject(0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			getObject(swordID)->SetParentID(m_HeroID);
			getObject(swordID)->SetRelPosition(norDirX, norDirY, 0.f);
			getObject(swordID)->SetStickToParent(true);
			getObject(swordID)->SetLife(100.f);
			getObject(swordID)->SetLifeTime(0.3f); //0.3 초 후 자동 삭제.
			getObject(m_HeroID)->ResetRemainingCoolTime();

			getSound()->PlayShortSound(m_SwordSound, false, 1.f);
		}
	}

	//Processing collision
	bool isCollide[GSE_MAX_OBJECTS];
	memset(isCollide, 0, sizeof(bool) * GSE_MAX_OBJECTS);
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		for (int j = i + 1; j < GSE_MAX_OBJECTS; j++)
		{
			if (getObject(i) != NULL && getObject(j) != NULL
				&&
				getObject(i)->GetApplyPhysics() && getObject(j)->GetApplyPhysics())
			{
				bool collide = ProcessCollision(getObject(i), getObject(j));
				if (collide)
				{
					isCollide[i] = true;
					isCollide[j] = true;
				}
			}
		}
	}

	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (getObject(i) != NULL)
		{
			if (!isCollide[i])
				getObject(i)->SetState(GSEObjectState::STATE_FALLING);
		}
	}

	//Update All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (getObject(i) != NULL)
		{
			if (i == m_HeroID)
			{
				getObject(i)->Update(elapsedTimeInSec, &heroParam);
			}
			else
			{
				if (getObject(i)->GetStickToParent())
				{
					float posX, posY, depth;
					float relPosX, relPosY, relDepth;
					int parentID = getObject(i)->GetParentID();
					getObject(parentID)->GetPosition(&posX, &posY, &depth);
					getObject(i)->GetRelPosition(&relPosX, &relPosY, &relDepth);
					getObject(i)->SetPosition(posX + relPosX, posY + relPosY, depth + relDepth);
					getObject(i)->Update(elapsedTimeInSec, &othersParam);
				}
				else
				{
					getObject(i)->Update(elapsedTimeInSec, &othersParam);
				}
			}
		}
	}
	float x, y, z;
	getObject(m_HeroID)->GetPosition(&x, &y, &z);
	getRenderer()->SetCameraPos(x * 100.f, y * 100.f);
}

void GSEBattle::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Draw background
	if (m_NowMap == RAILROAD_MAP) {
		getRenderer()->DrawGround(0, 0, 0, 6721, -720, 1, 1, 1, 1, 1, m_RailRoadMapTexture);
	}
	else if (m_NowMap == FIRE_MAP) {
		getRenderer()->DrawGround(0, 0, 0, 4661, -720, 1, 1, 1, 1, 1, m_FireMapTexture);
	}
	

	//Draw All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (getObject(i) != NULL)
		{
			float x, y, depth;
			getObject(i)->GetPosition(&x, &y, &depth);
			float sx, sy;
			getObject(i)->GetSize(&sx, &sy);

			int textureID = getObject(i)->GetTextureID();

			//meter to pixel
			x = x * 100.f;
			y = y * 100.f;
			sx = sx * 100.f;
			sy = sy * 100.f;

			GSEObjectType type;
			getObject(i)->GetType(&type);

			if (textureID < 0)
			{
				getRenderer()->DrawSolidRect(x, y, depth, sx, sy, 0.f, 1, 0, 1, 1);
			}
			else if (type == TYPE_FIXED) {
				getRenderer()->DrawTextureRect(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID);
			}
			else
			{
				getRenderer()->DrawTextureRectAnim(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID,
					28,
					12,
					5,
					0);
			}
		}
	}
}
