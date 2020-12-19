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
}

GSEGame::~GSEGame()
{
	//Renderer delete
	delete m_renderer;
	delete m_Sound;
}

Renderer* GSEGame::getRenderer()
{
	return m_renderer;
}

Sound* GSEGame::getSound()
{
	return m_Sound;
}

GSEObject* GSEGame::getObject(int index)
{
	return m_Objects[index];
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
		(bType == GSEObjectType::TYPE_FIXED || bType == GSEObjectType::TYPE_FIXED_UP))
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
			if (bType != GSEObjectType::TYPE_FIXED_UP) {
				aY = aY - (aMaxY - bMinY);

				a->SetPosition(aX, aY, 0.f);

				float vx, vy;
				a->GetVel(&vx, &vy);
				a->SetVel(vx, 0.f);
			}
		}
	}
	else if (
		(bType == GSEObjectType::TYPE_MOVABLE || bType == GSEObjectType::TYPE_HERO)
		&&
		(aType == GSEObjectType::TYPE_FIXED || aType == GSEObjectType::TYPE_FIXED_UP)
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
				if (aType != GSEObjectType::TYPE_FIXED_UP) {
					bY = bY - (bMaxY - aMinY);

					b->SetPosition(bX, bY, 0.f);
					float vx, vy;
					b->GetVel(&vx, &vy);
					b->SetVel(vx, 0.f);
				}
			}
		}
	}

	// 벽(Wall)과 충돌 처리
	else if ((aType == GSEObjectType::TYPE_MOVABLE || aType == GSEObjectType::TYPE_HERO)
		&&
		bType == GSEObjectType::TYPE_WALL)
	{
		if (aMaxX > bMaxX)
		{
			aX = aX + (bMaxX - aMinX);

			a->SetPosition(aX, aY, 0.f);

			float vx, vy;
			a->GetVel(&vx, &vy);
			a->SetVel(0.f, vy);
		}
		else
		{
			aX = aX - (aMaxX - bMinX);

			a->SetPosition(aX, aY, 0.f);

			float vx, vy;
			a->GetVel(&vx, &vy);
			a->SetVel(0.f, vy);
		}
	}
	else if (
		(bType == GSEObjectType::TYPE_MOVABLE || bType == GSEObjectType::TYPE_HERO)
		&&
		(aType == GSEObjectType::TYPE_WALL)
		)
	{
		if (!(bMaxX > aMaxX && bMinX < aMinX))
		{
			if (bMaxX > aMaxX)
			{
				bX = bX + (aMaxX - bMinX);

				b->SetPosition(bX, bY, 0.f);
				float vx, vy;
				b->GetVel(&vx, &vy);
				b->SetVel(0.f, vy);
			}
			else
			{
				bX = bX - (bMaxX - aMinX);

				b->SetPosition(bX, bY, 0.f);
				float vx, vy;
				b->GetVel(&vx, &vy);
				b->SetVel(0.f, vy);
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
