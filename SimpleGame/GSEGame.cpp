#include "stdafx.h"
#include "GSEGame.h"
#include "math.h"

GSEGame::GSEGame()
{
	//Renderer initialize
	m_renderer = new Renderer(GSE_WINDOWSIZE_X, GSE_WINDOWSIZE_Y);
	m_Sound = new Sound();

	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		m_Objects[i] = NULL;
	}

	m_HeroTexture = m_renderer->GenPngTexture("a.png"); //재사용 가능!!
	m_BrickTexture = m_renderer->GenPngTexture("brick.png"); //재사용 가능!!
	m_SpriteTexture = m_renderer->GenPngTexture("spriteSheet.png"); //재사용 가능!!
	m_BGTexture = m_renderer->GenPngTexture("background.png");

	m_snowTexture = m_renderer->GenPngTexture("particle.png");

	m_snowParticle = m_renderer->CreateParticleObject(10000,
		-640, -360, 640, 360,
		10, 10, 10, 10,
		-10, -10, -5, -5);

	//Create Hero
	m_HeroID = AddObject(0, 0, 0, 1, 1, 0, 0, 0, 0, 20);
	m_Objects[m_HeroID]->SetType(GSEObjectType::TYPE_HERO); 
	m_Objects[m_HeroID]->SetApplyPhysics(true);
	m_Objects[m_HeroID]->SetLife(100000000.f);
	m_Objects[m_HeroID]->SetLifeTime(100000000.f);
	m_Objects[m_HeroID]->SetTextureID(m_SpriteTexture);

	int floor = AddObject(-1.25, -2.5, 0, 2, 0.3, 0, 0, 0, 0, 10000);
	m_Objects[floor]->SetType(GSEObjectType::TYPE_FIXED);
	m_Objects[floor]->SetApplyPhysics(true);
	m_Objects[floor]->SetLife(100000000.f);
	m_Objects[floor]->SetLifeTime(100000000.f);
	m_Objects[floor]->SetTextureID(m_BrickTexture);

	floor = AddObject(+1.25, 0, 0, 1, 0.3, 0, 0, 0, 0, 10000);
	m_Objects[floor]->SetType(GSEObjectType::TYPE_FIXED);
	m_Objects[floor]->SetApplyPhysics(true);
	m_Objects[floor]->SetLife(100000000.f);
	m_Objects[floor]->SetLifeTime(100000000.f);
	m_Objects[floor]->SetTextureID(m_BrickTexture);

	m_BGSound = m_Sound->CreateBGSound("bgSound.mp3");
	m_Sound->PlayBGSound(m_BGSound, true, 1.0f);

	m_SwordSound = m_Sound->CreateShortSound("swordSound.wav");
	
}

GSEGame::~GSEGame()
{
	//Renderer delete
}

void GSEGame::Update(float elapsedTimeInSec, GSEInputs* inputs)
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
		heroParam.forceY += 20*forceAmount;
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

		m_Objects[m_HeroID]->GetPosition(&aX, &aY, &temp);
		m_Objects[m_HeroID]->GetSize(&asX, &asY);

		if (m_Objects[m_HeroID]->GetRemainingCoolTime() < 0.f)
		{
			int swordID = AddObject(0.f, 0.f, 0.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			m_Objects[swordID]->SetParentID(m_HeroID);
			m_Objects[swordID]->SetRelPosition(norDirX, norDirY, 0.f);
			m_Objects[swordID]->SetStickToParent(true);
			m_Objects[swordID]->SetLife(100.f);
			m_Objects[swordID]->SetLifeTime(0.3f); //0.3 초 후 자동 삭제.
			m_Objects[m_HeroID]->ResetRemainingCoolTime();

			m_Sound->PlayShortSound(m_SwordSound, false, 1.f);
		}
	}

	//Processing collision
	bool isCollide[GSE_MAX_OBJECTS];
	memset(isCollide, 0, sizeof(bool) * GSE_MAX_OBJECTS);
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		for (int j = i + 1; j < GSE_MAX_OBJECTS; j++)
		{
			if (m_Objects[i] != NULL && m_Objects[j] != NULL
				&&
				m_Objects[i]->GetApplyPhysics() && m_Objects[j]->GetApplyPhysics())
			{
				bool collide = ProcessCollision(m_Objects[i], m_Objects[j]);
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
		if (m_Objects[i] != NULL)
		{
			if (!isCollide[i])
				m_Objects[i]->SetState(GSEObjectState::STATE_FALLING);
		}
	}

	//Update All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{
			if (i == m_HeroID)
			{
				m_Objects[i]->Update(elapsedTimeInSec, &heroParam);
			}
			else
			{
				if (m_Objects[i]->GetStickToParent())
				{
					float posX, posY, depth;
					float relPosX, relPosY, relDepth;
					int parentID = m_Objects[i]->GetParentID();
					m_Objects[parentID]->GetPosition(&posX, &posY, &depth);
					m_Objects[i]->GetRelPosition(&relPosX, &relPosY, &relDepth);
					m_Objects[i]->SetPosition(posX + relPosX, posY + relPosY, depth + relDepth);
					m_Objects[i]->Update(elapsedTimeInSec, &othersParam);
				}
				else
				{
					m_Objects[i]->Update(elapsedTimeInSec, &othersParam);
				}
			}
		}
	}

	float x, y, z;
	m_Objects[m_HeroID]->GetPosition(&x, &y, &z);
	m_renderer->SetCameraPos(x * 100.f, y * 100.f);
}

bool GSEGame::ProcessCollision(GSEObject* a, GSEObject* b)
{
	GSEObjectType aType, bType;
	a->GetType(&aType);
	b->GetType(&bType);

	bool isCollide = AABBCollision(a, b);
	if (isCollide)
	{
		//do something
		if (aType == GSEObjectType::TYPE_FIXED || bType == GSEObjectType::TYPE_FIXED)
		{
			a->SetState(GSEObjectState::STATE_GROUND);
			b->SetState(GSEObjectState::STATE_GROUND);
		}
	}
	return isCollide;
}

bool GSEGame::AABBCollision(GSEObject* a, GSEObject* b)
{
	GSEObjectType aType;
	GSEObjectType bType;

	float aMinX, aMaxX, aMinY, aMaxY;
	float bMinX, bMaxX, bMinY, bMaxY;
	float aX, aY, asX, asY;
	float bX, bY, bsX, bsY;
	float temp;

	a->GetType(&aType);
	b->GetType(&bType);

	a->GetPosition(&aX, &aY, &temp);
	a->GetSize(&asX, &asY);
	b->GetPosition(&bX, &bY, &temp);
	b->GetSize(&bsX, &bsY);

	aMinX = aX - asX / 2.f;
	aMaxX = aX + asX / 2.f;
	aMinY = aY - asY / 2.f;
	aMaxY = aY + asY / 2.f;
	bMinX = bX - bsX / 2.f;
	bMaxX = bX + bsX / 2.f;
	bMinY = bY - bsY / 2.f;
	bMaxY = bY + bsY / 2.f;

	if (aMinX > bMaxX) // || fabs(aMinX-bMaxX)<FLT_EPSILON
	{
		return false;
	}
	if (aMaxX < bMinX)
	{
		return false;
	}
	if (aMinY > bMaxY)
	{
		return false;
	}
	if (aMaxY < bMinY)
	{
		return false;
	}

	AdjustPosition(a, b);
	return true;
}

void GSEGame::AdjustPosition(GSEObject* a, GSEObject* b)
{
	GSEObjectType aType;
	GSEObjectType bType;

	float aMinX, aMaxX, aMinY, aMaxY;
	float bMinX, bMaxX, bMinY, bMaxY;
	float aX, aY, asX, asY;
	float bX, bY, bsX, bsY;
	float temp;

	a->GetType(&aType);
	b->GetType(&bType);

	a->GetPosition(&aX, &aY, &temp);
	a->GetSize(&asX, &asY);
	b->GetPosition(&bX, &bY, &temp);
	b->GetSize(&bsX, &bsY);

	aMinX = aX - asX / 2.f;
	aMaxX = aX + asX / 2.f;
	aMinY = aY - asY / 2.f;
	aMaxY = aY + asY / 2.f;
	bMinX = bX - bsX / 2.f;
	bMaxX = bX + bsX / 2.f;
	bMinY = bY - bsY / 2.f;
	bMaxY = bY + bsY / 2.f;

	if ((aType == GSEObjectType::TYPE_MOVABLE || aType == GSEObjectType::TYPE_HERO)
		&&
		bType == GSEObjectType::TYPE_FIXED)
	{
		if (aMaxY > bMaxY)
		{
			aY = aY + (bMaxY - aMinY);

			a->SetPosition(aX, aY, 0.f);

			float vx, vy;
			a->GetVel(&vx, &vy);
			a->SetVel(vx, 0.f);
		}
		else
		{
			aY = aY - (aMaxY - bMinY);

			a->SetPosition(aX, aY, 0.f);

			float vx, vy;
			a->GetVel(&vx, &vy);
			a->SetVel(vx, 0.f);
		}
	}
	else if (
		(bType == GSEObjectType::TYPE_MOVABLE || bType == GSEObjectType::TYPE_HERO)
		&&
		(aType == GSEObjectType::TYPE_FIXED)
		)
	{
		if (!(bMaxY > aMaxY && bMinY < aMinY))
		{
			if (bMaxY > aMaxY)
			{
				bY = bY + (aMaxY - bMinY);

				b->SetPosition(bX, bY, 0.f);
				float vx, vy;
				b->GetVel(&vx, &vy);
				b->SetVel(vx, 0.f);
			}
			else
			{
				bY = bY - (bMaxY - aMinY);

				b->SetPosition(bX, bY, 0.f);
				float vx, vy;
				b->GetVel(&vx, &vy);
				b->SetVel(vx, 0.f);
			}
		}
	}
}

void GSEGame::DoGarbageCollect()
{
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{
			float life = m_Objects[i]->GetLife();
			float lifeTime = m_Objects[i]->GetLifeTime();
			if (life < 0.f || lifeTime < 0.f)
			{
				DeleteObject(i);
			}
		}
	}
}

int temp = 0;
float tempTime = 0.0f;

void GSEGame::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// Renderer Test
	//m_renderer->DrawSolidRect(0, 0, 0, 100, 1, 0, 1, 1);
	// Draw background
	m_renderer->DrawGround(0, 0, 0, 3840, 2160, 1, 1, 1, 1, 1, m_BGTexture);

	//Draw All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (m_Objects[i] != NULL)
		{
			float x, y, depth;
			m_Objects[i]->GetPosition(&x, &y, &depth);
			float sx, sy;
			m_Objects[i]->GetSize(&sx, &sy);

			int textureID = m_Objects[i]->GetTextureID();

			//meter to pixel
			x = x * 100.f;
			y = y * 100.f;
			sx = sx * 100.f;
			sy = sy * 100.f;

			GSEObjectType type;
			m_Objects[i]->GetType(&type);

			if (textureID < 0)
			{
				m_renderer->DrawSolidRect(x, y, depth, sx, sy, 0.f, 1, 0, 1, 1);
			}
			else if(type == TYPE_FIXED) {
				m_renderer->DrawTextureRect(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID);
			}
			else
			{
				/*m_renderer->DrawTextureRect(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID);*/
				m_renderer->DrawTextureRectAnim(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID,
					12,
					12,
					temp,
					0);
				temp++;
				temp = temp % 12;
			}

		}
	}

	m_renderer->DrawParticle(m_snowParticle, 0, 0, 0, 
		1.f, 
		1, 1, 1, 1,
		0, 0, m_snowTexture, 1.f, tempTime, 0);

	tempTime += 0.016;
}

int GSEGame::AddObject(float x, float y, float depth, 
	float sx, float sy,
	float velX, float velY,
	float accX, float accY,
	float mass)
{
	//find empty slot
	int index = -1;
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (m_Objects[i] == NULL)
		{
			index = i;
			break;
		}
	}

	if (index < 0)
	{
		std::cout << "No empty object slot.. " << std::endl;
		return -1;
	}

	m_Objects[index] = new GSEObject();
	m_Objects[index]->SetPosition(x, y, depth);
	m_Objects[index]->SetSize(sx, sy);
	m_Objects[index]->SetVel(velX, velY);
	m_Objects[index]->SetAcc(accX, accY);
	m_Objects[index]->SetMass(mass);

	return index;
}

void GSEGame::DeleteObject(int index)
{
	if (m_Objects[index] != NULL)
	{
		delete m_Objects[index];
		m_Objects[index] = NULL;
	}
	else
	{
		std::cout << "Try to delete NULL object : " << index << std::endl;
	}
}
