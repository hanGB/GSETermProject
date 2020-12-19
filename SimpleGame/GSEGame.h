#pragma once
#include "Renderer.h"
#include "GSEObject.h"
#include "GSEGlobals.h"
#include "Sound.h"

class GSEGame
{
public:
	GSEGame();
	~GSEGame();

	//void RenderScene();
	int AddObject(float x, float y, float depth,
		float sx, float sy,
		float velX, float velY,
		float accX, float accY,
		float mass);
	void DeleteObject(int index);

	virtual void Update(float elapsedTimeInSec, GSEInputs* inputs) = 0;
	virtual void RenderScene() = 0;

	Renderer* getRenderer();
	Sound* getSound();
	GSEObject* getObject(int index);

	void DoGarbageCollect();
	bool ProcessCollision(GSEObject* a, GSEObject* b);
	

private:
	bool AABBCollision(GSEObject* a, GSEObject* b);
	void AdjustPosition(GSEObject* a, GSEObject* b);

	Renderer* m_renderer = NULL;
	Sound* m_Sound = NULL;
	GSEObject* m_Objects[GSE_MAX_OBJECTS];
};

