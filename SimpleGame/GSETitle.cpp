#include "stdafx.h"
#include "GSETitle.h"
#include <iostream>

GSETitle::GSETitle()
{
	m_time = 1;
	m_grassAnimationTime = 0;
	m_grassAnimationFrame = 0;
	m_CameraY = 0;

	m_TitleLogoTexture = getRenderer()->GenPngTexture("./resource/image/title/titleLogo.png");
	m_TitleGrassTexture = getRenderer()->GenPngTexture("./resource/image/title/titleGrass.png");
	m_TitlePressKeyTexture = getRenderer()->GenPngTexture("./resource/image/title/titlePressKey.png");

	m_grassID = AddObject(0, -300, 1, 1280, 690, 0, 0, 0, 0, 10000);
	getObject(m_grassID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(m_grassID)->SetApplyPhysics(true);
	getObject(m_grassID)->SetLife(100000000.f);
	getObject(m_grassID)->SetLifeTime(100000000.f);
	getObject(m_grassID)->SetTextureID(m_TitleGrassTexture);

	m_pressKeyID = AddObject(0, -500, 1, 241, 27, 0, 0, 0, 0, 10000);
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
	if (m_CameraY > -350) {
		getRenderer()->SetCameraPos(0, m_CameraY);
		m_CameraY -= 200 * elapsedTimeInSec;
	}
	else {
		m_time += elapsedTimeInSec;
	}
	m_grassAnimationTime = m_grassAnimationTime + 0.2 * 12 * elapsedTimeInSec;
	m_grassAnimationFrame = (int)m_grassAnimationTime % 12;
}

void GSETitle::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Draw background
	getRenderer()->DrawGround(0, 0, 0, 1280, -1444, 1, 1, 1, 1, 1, m_TitleLogoTexture);

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

			if (i == m_pressKeyID) {
				if (m_CameraY <= -350) {
					if ((int)m_time % 2) {
						getRenderer()->DrawTextureRect(
							x, y, depth,
							sx, -sy, 1.f,
							1.f, 1.f, 1.f, 1.f,
							textureID);
					}
				}
			}
			else if (i == m_grassID)
			{
				getRenderer()->DrawTextureRectAnim(
					x, y, depth,
					sx, sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID,
					3, 4, (int)m_grassAnimationFrame / 3, (int)m_grassAnimationFrame % 3);
			}

		}
	}
}
