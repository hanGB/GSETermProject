#pragma once
#include "GSEGame.h"

class GSEEnding : public GSEGame{
	bool m_bNextState;

	int m_KZTexture ;
	int m_MSTexture;
	int m_MusicTexture;

	int m_BackgroundSound;

	int cameraY;

public:
		GSEEnding();
		~GSEEnding();

		void Update(float elapsedTimeInSec, GSEInputs* inputs);
		void RenderScene();

		bool IsNextState() { return m_bNextState; }
};