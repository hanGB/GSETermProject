#include "stdafx.h"
#include "GSEGame.h"

GSEGame::GSEGame()
{
	// initialize renderer
	m_renderer	= new Renderer(500, 500);
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		m_Objects[i] = NULL;
	}

	// Test Objects
	for (int i = 0; i < 1000; ++i) {
		float randX		= ((float)rand() / (float)RAND_MAX) * 500.f - 250.f;
		float randY		= ((float)rand() / (float)RAND_MAX) * 500.f - 250.f;
		float depth		= 0.f;
		float randSX	= ((float)rand() / (float)RAND_MAX) * 10.f;
		float randSY	= ((float)rand() / (float)RAND_MAX) * 10.f;
		float randVelX	= ((float)rand() / (float)RAND_MAX) * 3.f;
		float randVelY	= ((float)rand() / (float)RAND_MAX) * 3.f;
		float accX		= 0.f; 
		float accY		= 0.f;
		float mass		= 1.f;

		AddObject(randX, randY, depth, randSX, randSY, randVelX, randVelY, accX, accY, mass);
	}
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

				m_renderer->DrawSolidRect(x, y, depth, sx, 1, 0, 1, 1);
		}
	}
}

void GSEGame::Update(float elapsedTimeInSec)
{
	// Update All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (m_Objects[i] != NULL) {
			m_Objects[i]->Update(elapsedTimeInSec);
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
