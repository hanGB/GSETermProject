#include "stdafx.h"
#include "GSEObject.h"
#include "GSEGlobals.h"


GSEObject::GSEObject()
{
	m_PositionX		= GSE_NULL_VAL;
	m_PositionY		= GSE_NULL_VAL;
	m_Depth			= GSE_NULL_VAL;
	m_SizeX			= GSE_NULL_VAL;
	m_SizeY			= GSE_NULL_VAL;
	m_AccX			= GSE_NULL_VAL;
	m_AccY			= GSE_NULL_VAL;
	m_VelX			= GSE_NULL_VAL;
	m_VelY			= GSE_NULL_VAL;
	m_Mass			= GSE_NULL_VAL;
}

GSEObject::~GSEObject()
{
}

void GSEObject::Update(float elapsedTimeInSec, GSEUpdateParams* param)
{
	float t = elapsedTimeInSec;
	float tt = elapsedTimeInSec * elapsedTimeInSec;

	// calce temporary
	float accX = param->forceX / m_Mass;
	float accY = param->forceY / m_Mass;

	// sum with objec'acc
	accX += m_AccX;
	accY += m_AccY;

	// update position
	m_PositionX = m_PositionX + m_VelX * t + 0.5f * accX * tt;
	m_PositionY = m_PositionY + m_VelY * t + 0.5f * accY * tt;

	// update velocity
	m_VelX = m_VelX + accX * t;
	m_VelY = m_VelY + accY * t;
}

void GSEObject::SetPosition(float x, float y, float depth)
{
	m_PositionX		= x;
	m_PositionY		= y;
	m_Depth			= depth;
}

void GSEObject::SetSize(float sx, float sy)
{
	m_SizeX			= sx;
	m_SizeY			= sy;
}

void GSEObject::SetVel(float x, float y)
{
	m_VelX			= x;
	m_VelY			= y;
}

void GSEObject::SetAcc(float x, float y)
{
	m_AccX			= x;
	m_AccY			= y;
}

void GSEObject::SetMass(float x)
{
	m_Mass			= x;
}

void GSEObject::GetPosition(float* x, float* y, float* depth)
{
	*x				= m_PositionX;
	*y				= m_PositionY;
	*depth			= m_Depth;
}

void GSEObject::GetSize(float* sx, float* sy)
{
	*sx				= m_SizeX;
	*sy				= m_SizeY;
}

void GSEObject::GetVel(float* x, float* y)
{
	*x				= m_VelX;
	*y				= m_VelY;
}

void GSEObject::GetAcc(float* x, float* y)
{
	*x				= m_AccX;
	*y				= m_AccY;
}

void GSEObject::GetMass(float* x)
{
	*x				= m_Mass;
}
