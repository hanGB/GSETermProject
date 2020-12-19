#pragma once
#include "GSEGame.h"

class GSEBattle : public GSEGame {

	bool m_bNextState;

	int m_HeroTexture;
	int m_SwordManTexture;
	int m_GunManTexture;
	int m_BrickTexture;
	int m_BackGroundTexture;

	int m_SwordSound; 
	int m_TriggerSound;
	int m_BackGroundSound;

	int m_HeroID;

public:
	GSEBattle();
	~GSEBattle();

	void Update(float elapsedTimeInSec, GSEInputs* inputs);
	void RenderScene();

	bool IsNextState() { return m_bNextState; }
};