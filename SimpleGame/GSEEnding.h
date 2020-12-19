#pragma once
#include "GSEGame.h"

class GSEEnding : public GSEGame{
	bool m_bNextState;

	int m_KZTexture ;
	int m_MSTexture;
	int m_MusicTexture;
	int m_SignTexture;
	int m_KPUTexture;
	int m_tyfpTexture;
	int m_finTexture;

	int m_BackgroundSound;

	int cameraY;
	float m_time;

public:
		GSEEnding();
		~GSEEnding();

		void Update(float elapsedTimeInSec, GSEInputs* inputs);
		void RenderScene();

		bool IsNextState() { return m_bNextState; }
};