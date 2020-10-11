#pragma once
class GSEObject
{
public:
	GSEObject();
	~GSEObject();

	void SetPosition(float x, float y, float depth);
	void SetSize(float sx, float sy);
	void GetPosition(float* x, float* y, float* depth);
	void GetSize(float* sx, float* sy);

private:
	float m_PositionX, m_PositionY;
	float m_Depth;
	float m_SizeX, m_SizeY;
};

