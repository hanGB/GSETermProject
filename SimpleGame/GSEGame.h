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

	void RenderScene();
	int AddObject(float x, float y, float depth,
		float sx, float sy,
		float velX, float velY,
		float accX, float accY,
		float mass);
	void DeleteObject(int index);
	void Update(float elapsedTimeInSec, GSEInputs* inputs);

private:
	bool AABBCollision(GSEObject* a, GSEObject* b);
	bool ProcessCollision(GSEObject* a, GSEObject* b);
	void AdjustPosition(GSEObject* a, GSEObject* b);
	void DoGarbageCollect();

	Renderer* m_renderer = NULL;
	Sound* m_Sound = NULL;
	GSEObject* m_Objects[GSE_MAX_OBJECTS];
	int m_HeroID = -1;

	int m_HeroTexture = -1;
	int m_BrickTexture = -1;
	int m_SpriteTexture = -1;
	int m_BGTexture = -1;

	int m_BGSound = -1;
	int m_SwordSound = -1;

	int m_snowParticle = -1;
	int m_snowTexture = -1;
};

