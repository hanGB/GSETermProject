#pragma once
#include "Renderer.h"
#include "GSEObject.h"

#define GSE_MAX_OBJECTS 1000

class GSEGame
{
public:
	GSEGame();
	~GSEGame();
	
	void RenderScene();
	int AddObject(float x, float y, float depth, float sx, float sy);
	void DeleteObject(int index);

private:
	Renderer* m_renderer = NULL;
	GSEObject* m_Objects[GSE_MAX_OBJECTS];
};

