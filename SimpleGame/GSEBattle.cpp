#include "stdafx.h"
#include "GSEBattle.h"

GSEBattle::GSEBattle()
{
	m_bReadyToPlay = false;

	m_bNextState = false;
	m_NowMap = 0;
	
	// 히어로 텍스쳐
	m_HeroIdleTexture = getRenderer()->GenPngTexture("./resource/image/battle/heroIdleSprite.png");
	m_HeroRunTexture = getRenderer()->GenPngTexture("./resource/image/battle/heroRunSprite.png"); 
	m_HeroAttackTexture = getRenderer()->GenPngTexture("./resource/image/battle/heroAttackSprite.png");
	m_HeroDieTexture = getRenderer()->GenPngTexture("./resource/image/battle/heroDieSprite.png");

	// 적 군인 텍스쳐
	m_SoldierIdleTexture = getRenderer()->GenPngTexture("./resource/image/battle/soldierIdleSprite.png");
	m_SoldierRunTexture = getRenderer()->GenPngTexture("./resource/image/battle/soldierRunSprite.png");
	m_SoldierDieTexture = getRenderer()->GenPngTexture("./resource/image/battle/soldierDieSprite.png");
	m_SoldierAttackTexture = getRenderer()->GenPngTexture("./resource/image/battle/soldierAttackSprite.png");
	m_SoldierGunAttackTexture = getRenderer()->GenPngTexture("./resource/image/battle/soldierGunAttackSprite.png");

	m_BulletTexture = getRenderer()->GenPngTexture("./resource/image/battle/bullet.png");

	// 배경 텍스쳐
	m_RailRoadMapTexture = getRenderer()->GenPngTexture("./resource/image/battle/mapRailLoad.png");
	m_FireMapTexture = getRenderer()->GenPngTexture("./resource/image/battle/mapFire.png");

	// 효과 사운드
	m_SwordSound = getSound()->CreateShortSound("./resource/sound/short/swordSound.wav");
	m_TriggerSound = getSound()->CreateShortSound("./resource/sound/short/triggerSound.wav");

	// 배경 사운드
	m_BackGroundSound = getSound()->CreateBGSound("./resource/sound/bg/battleSound.mp3");
	
	// 임시 무음
	getSound()->PlayBGSound(m_BackGroundSound, true, 0.3f);
}

GSEBattle::~GSEBattle()
{
	getSound()->StopBGSound(m_BackGroundSound);
	getSound()->DeleteBGSound(m_BackGroundSound);
	getSound()->DeleteShortSound(m_SwordSound);
	getSound()->DeleteShortSound(m_TriggerSound);

	getRenderer()->DeleteTexture(m_HeroIdleTexture);
	getRenderer()->DeleteTexture(m_HeroRunTexture);
	getRenderer()->DeleteTexture(m_HeroAttackTexture);
	getRenderer()->DeleteTexture(m_HeroDieTexture);

	getRenderer()->DeleteTexture(m_SoldierIdleTexture);
	getRenderer()->DeleteTexture(m_SoldierRunTexture);
	getRenderer()->DeleteTexture(m_SoldierDieTexture);
	getRenderer()->DeleteTexture(m_SoldierAttackTexture);
	getRenderer()->DeleteTexture(m_SoldierGunAttackTexture);

	getRenderer()->DeleteTexture(m_BulletTexture);

	getRenderer()->DeleteTexture(m_RailRoadMapTexture);
	getRenderer()->DeleteTexture(m_FireMapTexture);
}

void GSEBattle::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	// space를 누를 시 시간이 느리게 감
	if (inputs->KEY_SPACE)
		elapsedTimeInSec = elapsedTimeInSec / 3;

	
	// 테스트 용 enter시 맵 전환
	/*
	if (inputs->KEY_ENTER) {
		getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);

		m_NowMap++;
		m_NowMap = m_NowMap %2;
	}
	*/

	if (!m_bReadyToPlay) {
		MakeStage(m_NowMap);
		m_bReadyToPlay = true;
	}

	//do garbage collecting
	DoGarbageCollect();

	GSEUpdateParams othersParam;
	GSEUpdateParams heroParam;
	memset(&othersParam, 0, sizeof(GSEUpdateParams));
	memset(&heroParam, 0, sizeof(GSEUpdateParams));

	//calc force
	float forceAmountX = 800.f;
	float forceAmountY = 225.f;

	if (inputs->KEY_W)
	{
		heroParam.forceY += 50 * forceAmountY;
		
	}
	if (inputs->KEY_A)
	{
		float vx, vy;
		getObject(m_HeroID)->GetVel(&vx, &vy);
		if (vx > -PLAYER_MAXIUM_SPEED)
			heroParam.forceX -= forceAmountX;
	}
	if (inputs->KEY_S)
	{
		heroParam.forceY -= forceAmountY;
	}
	if (inputs->KEY_D)
	{
		float vx, vy;
		getObject(m_HeroID)->GetVel(&vx, &vy);
		if (vx < PLAYER_MAXIUM_SPEED)
			heroParam.forceX += forceAmountX;
	}

	//sword
	float swordPosX = 0.f;
	float swordPosY = 0.f;

	if (inputs->ARROW_LEFT) {
		getObject(m_HeroID)->SetDir(-1);
		swordPosX += -1.0f;
	}
	if (inputs->ARROW_RIGHT) {
		getObject(m_HeroID)->SetDir(+1);
		swordPosX += 1.0f;
	}

	float swordDirSize = sqrtf(swordPosX * swordPosX + swordPosY * swordPosY);
	if (swordDirSize > 0.f)
	{
		float norDirX = swordPosX / swordDirSize / 3;
		float norDirY =( swordPosY -0.2) / swordDirSize;

		float aX, aY, asX, asY;
		float bX, bY, bsX, bsY;
		float temp;

		getObject(m_HeroID)->GetPosition(&aX, &aY, &temp);
		getObject(m_HeroID)->GetSize(&asX, &asY);
		if (getObject(m_HeroID)->GetRemainingCoolTime() < 0.f)
		{
			if (getObject(m_HeroID)->GetAnimationState() != ANIMATION_DIE) {
				int swordID = AddObject(0.f, 0.f, 0.f, 0.5f, 1.0f, 0.f, 0.f, 0.f, 0.f, 1.f);
				getObject(swordID)->SetParentID(m_HeroID);
				getObject(swordID)->SetRelPosition(norDirX, norDirY, 0.f);
				getObject(swordID)->SetApplyPhysics(true);
				getObject(swordID)->SetStickToParent(true);
				getObject(swordID)->SetLife(100.f);
				getObject(swordID)->SetType(TYPE_SWORD);

				getObject(swordID)->SetLifeTime(0.3f); //0.3 초 후 자동 삭제.
				getObject(m_HeroID)->ResetRemainingCoolTime();

				getSound()->PlayShortSound(m_SwordSound, false, 1.f);

				getObject(m_HeroID)->SetAnimationState(ANIMATION_ATTACK);
				getObject(m_HeroID)->SetAnimationFrame(0, 0.f);
			}
		}
	}

	SoldierBehave(elapsedTimeInSec);

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
			// Animation Frame Change
			if (getObject(i)->IsAnimation())
			{
				int aniState;
				float vx, vy;

				getObject(i)->GetVel(&vx, &vy);

				aniState = getObject(i)->GetAnimationState();

				int frame;
				float frameTime;
				float frameSpeed;

				getObject(i)->GetAnimationFrame(&frame, &frameTime, &frameSpeed);

				if (aniState == ANIMATION_IDLE || aniState == ANIMATION_ATTACK)
					if (vx != 0.f || vy != 0.f) {
						if (aniState == ANIMATION_ATTACK) {
							if (frame + 1 >= getObject(i)->GetAnimationFrameCnt()) {
								getObject(i)->SetAnimationFrame(0, 0.f);
								aniState = ANIMATION_RUN;
							}
						}
						else {
							getObject(i)->SetAnimationFrame(0, 0.f);
							aniState = ANIMATION_RUN;
						}
					}

				if (aniState == ANIMATION_RUN || aniState == ANIMATION_ATTACK)
					if (vx == 0.f && vy == 0.f) {
						if (aniState == ANIMATION_ATTACK) {
							if (frame + 1 >= getObject(i)->GetAnimationFrameCnt()) {
								getObject(i)->SetAnimationFrame(0, 0.f);
								aniState = ANIMATION_IDLE;
							}
						}
						else {
							getObject(i)->SetAnimationFrame(0, 0.f);
							aniState = ANIMATION_IDLE;
						}
					}

				getObject(i)->SetAnimationState(aniState);

				// 사망 애니메이션이 끝나면 삭제
				if (getObject(i)->GetAnimationState() == ANIMATION_DIE) {
					if (frame + 1 >= getObject(i)->GetAnimationFrameCnt()) {
						if (i == m_HeroID) {
							m_bReadyToPlay = false;
						}
						getObject(i)->SetLifeTime(0.0f);
					}
				}
		
				GSEObjectState objectState;
				getObject(i)->GetState(&objectState);

				// 애니메이션 프래임 계산
				if (aniState == ANIMATION_RUN) {
					if (objectState == STATE_GROUND) {
						frameTime = frameTime + frameSpeed * getObject(i)->GetAnimationFrameCnt() * elapsedTimeInSec;
						frame = (int)frameTime % getObject(i)->GetAnimationFrameCnt();
					}
				}
				else {
					frameTime = frameTime + frameSpeed * getObject(i)->GetAnimationFrameCnt() * elapsedTimeInSec;
					frame = (int)frameTime % getObject(i)->GetAnimationFrameCnt();
				}

				getObject(i)->SetAnimationFrame(frame, frameTime);
			}
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
					memset(&othersParam, 0, sizeof(GSEUpdateParams));

					GSEObjectType type;
					getObject(i)->GetType(&type);
					if (type == TYPE_BULLET) {
						othersParam.forceX = getObject(i)->GetDir() * 100;

					}
					getObject(i)->Update(elapsedTimeInSec, &othersParam);
				}
			}
		}
	}

	float x, y, z;
	getObject(m_HeroID)->GetPosition(&x, &y, &z);

	// 플레이어 위치 출력
	// std::cout << "x: " << x * 100 << " , y: " << y * 100 << std::endl;

	x = x * 100.f;
	y = y * 100.f;

	if (m_NowMap == RAILROAD_MAP) {
		if (x < -3500) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (x > 3500) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (y < -600) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (y > 600) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);

		if (3300 < x && x < 3330) {
			if (138.5 < y && y < 147.5) {
				m_bReadyToPlay = false;
				m_NowMap++;
			}
		}
	}
	else if (m_NowMap == FIRE_MAP) {
		if (x < -2400) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (x > 2400) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (y < -600) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);
		if (y > 600) getObject(m_HeroID)->SetAnimationState(ANIMATION_DIE);

		if (2270 < x && x < 2300) {
			if (-170 < y && y < -160) {
				m_bNextState = true;
			}
		}
	}

	getRenderer()->SetCameraPos(x, y);
	
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

			if (type == TYPE_FIXED || type == TYPE_WALL || type == TYPE_SWORD)
			{
				// 충돌체 체크용
				//getRenderer()->DrawSolidRect(x, y, depth, sx, sy, 0.f, 1, 0, 1, 1);
			}
			else
			{
				if (getObject(i)->IsAnimation()) {
					// 충돌체 체크용
					//getRenderer()->DrawSolidRect(x, y, depth, getObject(i)->GetDir() * sx, sy, 0.f, 1, 0, 1, 1);
					
					int frame;
					float frametime;
					float framespeed;
					getObject(i)->GetAnimationFrame(&frame, &frametime, &framespeed);

					getRenderer()->DrawTextureRectAnim(
						x, y, depth,
						getObject(i)->GetDir() * sx, sy, 1.f,
						1.f, 1.f, 1.f, 1.f,
						getObject(i)->GetAnimationTextureID(),
						getObject(i)->GetAnimationFrameCnt(), 1,
						frame, 0);
				}
				else {
					// 충돌체 체크용
					//getRenderer()->DrawSolidRect(x, y, depth, getObject(i)->GetDir() * sx, sy, 0.f, 1, 0, 1, 1);

					getRenderer()->DrawTextureRect(
						x, y, depth,
						getObject(i)->GetDir() * sx, -sy, 1.f,
						1.f, 1.f, 1.f, 1.f,
						textureID
					);
				}
			}
		}
	}
}

void GSEBattle::MakeStage(int map)
{
	for (int i = 0; i < GSE_MAX_OBJECTS; i++) {
		if (getObject(i) != NULL) {
			DeleteObject(i);
		}
	}

	if (map == RAILROAD_MAP) 
	{
		//Create Hero
		m_HeroID = AddObject(-30, 3, 0, 0.8, 1.65, 0, 0, 0, 0, 50);
		getObject(m_HeroID)->SetType(GSEObjectType::TYPE_HERO);
		getObject(m_HeroID)->SetApplyPhysics(true);
		getObject(m_HeroID)->SetLife(100.f);
		getObject(m_HeroID)->SetLifeTime(100000000.f);
		getObject(m_HeroID)->SetAnimationTextureID(
			m_HeroIdleTexture, m_HeroRunTexture, m_HeroAttackTexture, m_HeroDieTexture);
		getObject(m_HeroID)->SetAnimationFrameCnt(14, 8, 13, 5);
		getObject(m_HeroID)->SetAnimationFrame(0, 0); 
		getObject(m_HeroID)->SetAnimationFrameSpeed(0.5, 3, 1, 1);

		//Create Soldier
		int soldier = AddObject(0, 3, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(-14.66, 0.90, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(-2.72, -1.4, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(3.72, -1.4, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(23.72, -1.4, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		//Create Soldier with gun attack
		soldier = AddObject(10, 3, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierGunAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 5, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 0.5, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER_WITH_GUN);

		//Create Floor
		int floor = AddObject(0, -2.5, 0, 67, 0.5, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-23.5, 0, 0, 20, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(33, 0.5, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(32, 0, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(31, -0.5, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(30, -1, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(29, -1.5, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(28, -2, 0, 1, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		//Create Wall
		int wall = AddObject(33.8, 0, 0, 0.5, 7.2, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);

		wall = AddObject(-33.8, 0, 0, 0.5, 7.2, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);
	}
	else if (map == FIRE_MAP) 
	{
		//Create Hero
		m_HeroID = AddObject(-21, 0, 0, 0.8, 1.65, 0, 0, 0, 0, 50);
		getObject(m_HeroID)->SetType(GSEObjectType::TYPE_HERO);
		getObject(m_HeroID)->SetApplyPhysics(true);
		getObject(m_HeroID)->SetLife(100.f);
		getObject(m_HeroID)->SetLifeTime(100000000.f);
		getObject(m_HeroID)->SetAnimationTextureID(
			m_HeroIdleTexture, m_HeroRunTexture, m_HeroAttackTexture, m_HeroDieTexture);
		getObject(m_HeroID)->SetAnimationFrameCnt(14, 8, 13, 5);
		getObject(m_HeroID)->SetAnimationFrame(0, 0);
		getObject(m_HeroID)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		
		// Create Soldier
		int soldier = AddObject(-12.54, -1.5, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(-10.54, -1.5, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(5.13, 2.59, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(9.13, 2.59, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		soldier = AddObject(9.52, -0.3, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 7, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 1, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER);

		// Create Soldier with gun attack
		soldier = AddObject(0.83, 5.2, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierGunAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 5, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 0.5, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER_WITH_GUN);

		soldier = AddObject(17.98, -1.59, 0, 0.8, 1.75, 0, 0, 0, 0, 70);
		getObject(soldier)->SetType(GSEObjectType::TYPE_ENEMY);
		getObject(soldier)->SetApplyPhysics(true);
		getObject(soldier)->SetLife(100.f);
		getObject(soldier)->SetLifeTime(100000000.f);
		getObject(soldier)->SetAnimationTextureID(
			m_SoldierIdleTexture, m_SoldierRunTexture, m_SoldierGunAttackTexture, m_SoldierDieTexture);
		getObject(soldier)->SetAnimationFrameCnt(5, 12, 5, 7);
		getObject(soldier)->SetAnimationFrame(0, 0);
		getObject(soldier)->SetAnimationFrameSpeed(0.5, 3, 0.5, 1);
		getObject(soldier)->SetDir(-1);
		getObject(soldier)->SetEnemyType(ENEMY_SOLDIER_WITH_GUN);

		// Create Floor
		int floor = AddObject(0, -2.7, 0, 47, 0.5, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(2.1, -0.9, 0, 13.8, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(5.9, 1.59, 0, 5.9, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-3.5, 1.59, 0, 2, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(9.75, -1.3, 0, 1.55, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-3.5, 1.59, 0, 2, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-7.5, -2.3, 0, 1.5, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-6.4, -2.1, 0, 0.5, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-5.9, -1.8, 0, 0.5, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-5.4, -1.5, 0, 0.5, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		floor = AddObject(-4.9, -1.2, 0, 0.5, 0.25, 0, 0, 0, 0, 10000);
		getObject(floor)->SetType(GSEObjectType::TYPE_FIXED);
		getObject(floor)->SetApplyPhysics(true);
		getObject(floor)->SetLife(100000000.f);
		getObject(floor)->SetLifeTime(100000000.f);

		//Create Wall
		int wall = AddObject(23.5, 0, 0, 0.5, 7.2, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);

		wall = AddObject(-23.5, 0, 0, 0.5, 7.2, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);

		wall = AddObject(8.7, -1.6, 0, 0.5, 1.6, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);

		wall = AddObject(10.2, -1.9, 0, 0.5, 0.9, 0, 0, 0, 0, 10000);
		getObject(wall)->SetType(GSEObjectType::TYPE_WALL);
		getObject(wall)->SetApplyPhysics(true);
		getObject(wall)->SetLife(100000000.f);
		getObject(wall)->SetLifeTime(100000000.f);
	}
}

void GSEBattle::SoldierBehave(float elapsedTimeInSec)
{
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (getObject(i) != NULL) {
			GSEObjectType type;
			getObject(i)->GetType(&type);

			if (type == TYPE_ENEMY) {
				float x, y, z;
				getObject(i)->GetPosition(&x, &y, &z);

				float heroX, heroY, heroZ;
				getObject(m_HeroID)->GetPosition(&heroX, &heroY, &heroZ);

				if (getObject(i)->GetEnemyType() == ENEMY_SOLDIER) {
					if (getObject(i)->GetAnimationState() == ANIMATION_IDLE ||
						getObject(i)->GetAnimationState() == ANIMATION_RUN) {
						if (fabs(heroY - y) < 5 && fabs(heroX - x) < 10) {
							// 가까우면 공격
							if (fabs(heroY - y) < 0.5 && fabs(heroX - x) < 0.5) {
								float swordPosX = 0.f;
								float swordPosY = 0.f;

								if (heroX - x > 0) {
									getObject(i)->SetDir(1);
									swordPosX += 1.0f;

								}
								else {
									getObject(i)->SetDir(-1);
									swordPosX += -1.0f;
								}

								float swordDirSize = sqrtf(swordPosX * swordPosX + swordPosY * swordPosY);
								if (swordDirSize > 0.f)
								{
									float norDirX = swordPosX / swordDirSize / 3;
									float norDirY = swordPosY / swordDirSize;

									if (getObject(i)->GetRemainingCoolTime() < 0.f)
									{
										if (getObject(i)->GetAnimationState() != ANIMATION_DIE) {
											int swordID = AddObject(0.f, 0.f, 0.f, 0.5f, 1.0f, 0.f, 0.f, 0.f, 0.f, 1.f);
											getObject(swordID)->SetParentID(i);
											getObject(swordID)->SetRelPosition(norDirX, norDirY, 0.f);
											getObject(swordID)->SetApplyPhysics(true);
											getObject(swordID)->SetStickToParent(true);
											getObject(swordID)->SetLife(100.f);
											getObject(swordID)->SetType(TYPE_SWORD);

											getObject(swordID)->SetLifeTime(0.3f); //0.3 초 후 자동 삭제.
											getObject(i)->ResetRemainingCoolTime();

											getSound()->PlayShortSound(m_SwordSound, false, 1.f);

											getObject(i)->SetAnimationState(ANIMATION_ATTACK);
											getObject(i)->SetAnimationFrame(0, 0.f);
										}
									}
								}
							}
							// 주인공(히어로)에게 이동
							else {
								GSEUpdateParams othersParam;
								memset(&othersParam, 0, sizeof(GSEUpdateParams));

								float forceAmountX = 2000.f;
								float forceAmountY = 225.f;

								if (heroX - x > 0) {
									float vx, vy;
									getObject(i)->GetVel(&vx, &vy);
									if (vx > -PLAYER_MAXIUM_SPEED)
										othersParam.forceX += forceAmountX;
								}
								else {
									float vx, vy;
									getObject(i)->GetVel(&vx, &vy);
									if (vx > -PLAYER_MAXIUM_SPEED)
										othersParam.forceX -= forceAmountX;
								}

								if ((heroY - y > 2))
									othersParam.forceY += 70 * forceAmountY;
								getObject(i)->Update(elapsedTimeInSec, &othersParam);
							}
						}
					}
				}
				else if (getObject(i)->GetEnemyType() == ENEMY_SOLDIER_WITH_GUN) {
					if (getObject(i)->GetAnimationState() == ANIMATION_IDLE ||
						getObject(i)->GetAnimationState() == ANIMATION_RUN) {
						if (fabs(heroY - y) < 5 && fabs(heroX - x) < 10) {
							// 가까우면 공격
							if (fabs(heroY - y) < 3 && fabs(heroX - x) < 5) {
								float bulletPosX = 0.f;
								float bulletPosY = 0.f;

								if (heroX - x > 0) {
									getObject(i)->SetDir(1);
									bulletPosX += 1.0f;

								}
								else {
									getObject(i)->SetDir(-1);
									bulletPosX += -1.0f;
								}

								float swordDirSize = sqrtf(bulletPosX * bulletPosX + bulletPosY * bulletPosY);
								if (swordDirSize > 0.f)
								{
									float norDirX = bulletPosX / swordDirSize / 2;
									float norDirY = bulletPosY / swordDirSize * 1.5;

									if (getObject(i)->GetRemainingCoolTime() < 0.f)
									{
										if (getObject(i)->GetAnimationState() != ANIMATION_DIE) {
											int bulletID = AddObject(x + norDirX, y + norDirY, z, 0.2f, 0.1f, 0.f, 0.f, 0.f, 0.f, 1.f);
											getObject(bulletID)->SetParentID(i);
											getObject(bulletID)->SetApplyPhysics(true);
											getObject(bulletID)->SetStickToParent(false);
											getObject(bulletID)->SetLife(100.f);
											getObject(bulletID)->SetType(TYPE_BULLET);
											getObject(bulletID)->SetTextureID(m_BulletTexture);

											if (bulletPosX > 0) {
												getObject(bulletID)->SetDir(1);
											}
											else {
												getObject(bulletID)->SetDir(-1);
											}

											getObject(bulletID)->SetLifeTime(5.0f); // 5초 후 자동 삭제.
											getObject(i)->ResetRemainingCoolTime();

											getSound()->PlayShortSound(m_TriggerSound, false, 0.5f);

											getObject(i)->SetAnimationState(ANIMATION_ATTACK);
											getObject(i)->SetAnimationFrame(0, 0.f);
										}
									}
								}
							}
							// 주인공(히어로)에게 이동
							else {
								GSEUpdateParams othersParam;
								memset(&othersParam, 0, sizeof(GSEUpdateParams));

								float forceAmountX = 900.f;
								float forceAmountY = 225.f;

								if (heroX - x > 0) {
									float vx, vy;
									getObject(i)->GetVel(&vx, &vy);
									if (vx > -PLAYER_MAXIUM_SPEED)
										othersParam.forceX += forceAmountX;
								}
								else {
									float vx, vy;
									getObject(i)->GetVel(&vx, &vy);
									if (vx > -PLAYER_MAXIUM_SPEED)
										othersParam.forceX -= forceAmountX;
								}

								if ((heroY - y > 2))
									othersParam.forceY += 70 * forceAmountY;
								getObject(i)->Update(elapsedTimeInSec, &othersParam);
							}
						}
					}
				}
			}
		}
	}
}

