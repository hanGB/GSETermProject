#include "stdafx.h"
#include "GSETitle.h"
#include <iostream>

GSETitle::GSETitle()
{
	m_time = 0;
	m_grassAnimation = 0;

	m_TitleLogoTexture = getRenderer()->GenPngTexture("./resource/image/title/titleLogo.png");
	m_TitleGrassTexture = getRenderer()->GenPngTexture("./resource/image/title/titleGrass.png");
	m_TitlePressKeyTexture = getRenderer()->GenPngTexture("./resource/image/title/titlePressKey.png");

	m_grassID = AddObject(0, 0, 1, 649, 350, 0, 0, 0, 0, 10000);
	getObject(m_grassID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(m_grassID)->SetApplyPhysics(true);
	getObject(m_grassID)->SetLife(100000000.f);
	getObject(m_grassID)->SetLifeTime(100000000.f);
	getObject(m_grassID)->SetTextureID(m_TitleGrassTexture);

	m_pressKeyID = AddObject(0, 0, 1, 161, 18, 0, 0, 0, 0, 10000);
	getObject(m_pressKeyID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(m_pressKeyID)->SetApplyPhysics(true);
	getObject(m_pressKeyID)->SetLife(100000000.f);
	getObject(m_pressKeyID)->SetLifeTime(100000000.f);
	getObject(m_pressKeyID)->SetTextureID(m_TitlePressKeyTexture);
}

GSETitle::~GSETitle()
{
}

void GSETitle::Update(float elapsedTimeInSec, GSEInputs* inputs)
{

}

void GSETitle::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Draw background
	getRenderer()->DrawGround(0, 0, 0, 645, -728, 1, 1, 1, 1, 1, m_TitleLogoTexture);

	//Draw All Objects
	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (getObject(i) != NULL)
		{
			float x, y, depth;
			getObject(i)->GetPosition(&x, &y, &depth);
			float sx, sy;
			getObject(i)->GetSize(&sx, &sy);

			int textureID = getObject(i)->GetTextureID();

			//meter to pixel
			x = x * 100.f;
			y = y * 100.f;
			sx = sx;
			sy = sy;

			if (i == m_pressKeyID) {
				getRenderer()->DrawTextureRect(
					x, y, depth,
					sx, -sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID);
			}
			else if (i == m_grassID)
			{
				getRenderer()->DrawTextureRectAnim(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID,
					3, 4, m_grassAnimation,
					0);
			}

		}
	}
}
