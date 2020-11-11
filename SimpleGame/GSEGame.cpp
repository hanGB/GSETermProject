#include "stdafx.h"
#include "GSEGame.h"

GSEGame::GSEGame()
{
	// initialize renderer
	m_renderer	= new Renderer(500, 500);
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		m_Objects[i] = NULL;
	}

	// Create Hero
	m_HeroID = AddObject(0, 0, 0, 1, 1, 0, 0, 0, 0, 20);
	m_Objects[m_HeroID]->SetType(GSEObjectType::TYPE_HERO);

	int floor = AddObject(-1.25, -2.5, 0, 2, 0.3, 0, 0, 0, 0, 10000);
	m_Objects[floor]->SetType(GSEObjectType::TYPE_FIXED);
	floor = AddObject(+1.25, 0, 0, 1, 0.3, 0, 0, 0, 0, 10000);
	m_Objects[floor]->SetType(GSEObjectType::TYPE_FIXED);
}

GSEGame::~GSEGame()
{
	// delete renderer
	delete		m_renderer;
	delete[]	m_Objects;
}

void GSEGame::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	// Draw All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (m_Objects[i] != NULL) {
			float x, y, depth;
			m_Objects[i]->GetPosition(&x, &y, &depth);

			float sx, sy;
			m_Objects[i]->GetSize(&sx, &sy);

			// meter to pixel
			x = x * 100.f;
			y = y * 100.f;
			sx = sx * 100.f;
			sy = sy * 100.f;

			m_renderer->DrawSolidRect(x, y, depth, sx, sy, 1, 0, 1, 1);
		}
	}
}

void GSEGame::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	GSEUpdateParams othersParam;
	GSEUpdateParams heroParam;
	memset(&othersParam, 0, sizeof(GSEUpdateParams));
	memset(&heroParam, 0, sizeof(GSEUpdateParams));

	// calc force
	float forceAmount = 400.f;
	if (inputs->KEY_W)
	{
		heroParam.forceY += forceAmount;
	}
	if (inputs->KEY_A)
	{
		heroParam.forceX += -forceAmount;
	}
	if (inputs->KEY_S)
	{
		heroParam.forceY += -forceAmount;
	}
	if (inputs->KEY_D)
	{
		heroParam.forceX += forceAmount;
	}

	// Processing collision
	for (int i = 0; i < GSE_MAX_OBJECTS; i++) {
		for (int j = i + 1; j < GSE_MAX_OBJECTS; j++) {
			if (m_Objects[i] != NULL && m_Objects[j] != NULL) {
				ProcessCollision(m_Objects[i], m_Objects[j]);
			}
		}
	}

	// Update All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (m_Objects[i] != NULL) {
			if (i == m_HeroID)
			{
				m_Objects[i]->Update(elapsedTimeInSec, &heroParam);
			}
			else
			{
				m_Objects[i]->Update(elapsedTimeInSec, &othersParam);
			}
			
		}
	}
}

bool GSEGame::AABBCollsion(GSEObject* a, GSEObject* b)
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

	if (aMinX > bMaxX) return false;
	if (aMaxX < bMinX) return false;
	if (aMinY > bMaxY) return false;
	if (aMaxY < bMinY) return false;

	AdjustPosition(a, b);
	return true;
}

void GSEGame::ProcessCollision(GSEObject* a, GSEObject* b)
{
	if (AABBCollsion(a, b)) 
	{
		GSEObjectType aType, bType;
		a->GetType(&aType);
		b->GetType(&bType);
		if (aType == GSEObjectType::TYPE_FIXED || bType == GSEObjectType::TYPE_FIXED)
		{
			a->SetState(GSEObjectState::STATE_GROUND);
			b->SetState(GSEObjectState::STATE_GROUND);
		}
	}
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
		else {
			aY = aY - (aMaxY - bMinY);

			a->SetPosition(aX, aY, 0.f);

			float vx, vy;

			a->GetVel(&vx, &vy);
			a->SetVel(vx, 0.f);
		}
	}
	else if ((bType == GSEObjectType::TYPE_MOVABLE || bType == GSEObjectType::TYPE_HERO)
		&&
		aType == GSEObjectType::TYPE_FIXED)
	{
		if (bMaxY > aMaxY)
		{
			bY = bY + (aMaxY - bMinY);

			b->SetPosition(bX, bY, 0.f);

			float vx, vy;

			b->GetVel(&vx, &vy);
			b->SetVel(vx, 0.f);
		}
		else {
			bY = bY - (bMaxY - aMinY);

			b->SetPosition(bX, bY, 0.f);

			float vx, vy;

			b->GetVel(&vx, &vy);
			b->SetVel(vx, 0.f);
		}
	}
}


int GSEGame::AddObject(float x, float y, float depth,
	float sx, float sy,
	float velX, float velY,
	float accX, float accY,
	float mass)
{
	// find empty slot
	int index = -1;
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (m_Objects[i] == NULL) {
			index = i;
			break;
		}
	}
	if (index < 0) {
		std::cout << "No empty object slot.." << std::endl;
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
	if (m_Objects[index] != NULL) {
		delete m_Objects[index];
		m_Objects[index] = NULL;
	}
	else {
		std::cout << "Try to delete NULL Object : " << index << std::endl;
	}
}
