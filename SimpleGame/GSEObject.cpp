#include "stdafx.h"
#include "GSEObject.h"

#define GSE_NULL_VAL -1000000;

GSEObject::GSEObject()
{
	m_PositionX		= GSE_NULL_VAL;
	m_PositionY		= GSE_NULL_VAL;
	m_Depth			= GSE_NULL_VAL;
	m_SizeX			= GSE_NULL_VAL;
	m_SizeY			= GSE_NULL_VAL;
}

GSEObject::~GSEObject()
{
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
