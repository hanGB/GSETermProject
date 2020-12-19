#pragma once
#include "GSEGame.h"

class GSETitle : public GSEGame {
	float m_time;
	float m_grassAnimationTime;
	int m_grassAnimationFrame;

	int m_TitleGrassTexture;
	int m_TitleLogoTexture;
	int m_TitlePressKeyTexture;

	int m_RainParticleTexture;
	int m_RainParticle;

	int m_RainSound;
	int m_SwordSound;

	int m_grassID;
	int m_pressKeyID;
	
	int m_CameraY;

	bool m_bNextState;

public:
	GSETitle();
	~GSETitle();

	void Update(float elapsedTimeInSec, GSEInputs* inputs);
	void RenderScene();

	bool IsNextState() { return m_bNextState; }
};