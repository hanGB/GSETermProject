#pragma once
#include "GSEGame.h"

class GSEBattle : public GSEGame {

	bool m_bNextState;

	int m_NowMap;

	int m_HeroIdleTexture;
	int m_HeroRunTexture;
	int m_HeroDieTexture;
	int m_HeroAttackTexture;

	int m_SoldierIdleTexture;
	int m_SoldierRunTexture;
	int m_SoldierDieTexture;
	int m_SoldierAttackTexture;
	int m_SoldierGunAttackTexture;

	int m_BulletTexture;

	int m_RailRoadMapTexture;
	int m_FireMapTexture;
	int m_FireMapFrontTexture;

	int m_SwordSound; 
	int m_TriggerSound;
	int m_BackGroundSound;

	int m_HeroID;

	bool m_bReadyToPlay;

public:
	GSEBattle();
	~GSEBattle();

	void Update(float elapsedTimeInSec, GSEInputs* inputs);
	void RenderScene();

	bool IsNextState() { return m_bNextState; }
	
private:
	void MakeStage(int map);
};