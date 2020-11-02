#pragma once
#include "GSEGlobals.h"

class GSEObject
{
public:
	GSEObject();
	~GSEObject();

	void Update(float elapsedTimeInSec, GSEUpdateParams* param);

	void SetPosition(float x, float y, float depth);
	void SetSize(float sx, float sy);
	void SetVel(float x, float y);
	void SetAcc(float x, float y);
	void SetMass(float x);

	void GetPosition(float* x, float* y, float* depth);
	void GetSize(float* sx, float* sy);
	void GetVel(float* x, float* y);
	void GetAcc(float* x, float* y);
	void GetMass(float* x);

private:
	float m_PositionX, m_PositionY;
	float m_Depth;
	float m_SizeX, m_SizeY;
	float m_VelX, m_VelY;
	float m_AccX, m_AccY;
	float m_Mass;
};

