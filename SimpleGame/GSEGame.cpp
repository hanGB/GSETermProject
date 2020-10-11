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
		float randX = ((float)rand() / (float)RAND_MAX) * 500.f - 250.f;
		float randY = ((float)rand() / (float)RAND_MAX) * 500.f - 250.f;
		float depth = 0.f;
		float randSX = ((float)rand() / (float)RAND_MAX) * 10.f;
		float randSY = ((float)rand() / (float)RAND_MAX) * 10.f;
		AddObject(randX, randY, depth, randSX, randSY);
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

int GSEGame::AddObject(float x, float y, float depth, float sx, float sy)
{
	// find empty slot
	int index = -1;
	for (int i = 0; i < GSE_MAX_OBJECTS; ++i) {
		if (m_Objects[i] == NULL) {
			index = i;
			break;
		}
	}

	m_Objects[index] = new GSEObject();
	m_Objects[index]->SetPosition(x, y, depth);
	m_Objects[index]->SetSize(sx, sy);

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
