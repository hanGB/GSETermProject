#pragma once
#include "GSEGame.h"

class GSETitle : public GSEGame {
	float m_time;
	float m_grassAnimationTime;
	int m_grassAnimationFrame;

	int m_TitleGrassTexture;
	int m_TitleLogoTexture;
	int m_TitlePressKeyTexture;

	int m_grassID;
	int m_pressKeyID;
	
	int m_CameraY;

public:
	GSETitle();
	~GSETitle();

	void Update(float elapsedTimeInSec, GSEInputs* inputs);
	void RenderScene();
};