#include "stdafx.h"
#include "GSETitle.h"
#include <Windows.h>

GSETitle::GSETitle()
{
	m_bNextState = false;

	m_time = 1;
	m_grassAnimationTime = 0;
	m_grassAnimationFrame = 0;
	m_CameraY = 0;

	m_TitleLogoTexture = getRenderer()->GenPngTexture("./resource/image/title/titleLogo.png");
	m_TitleGrassTexture = getRenderer()->GenPngTexture("./resource/image/title/titleGrass.png");
	m_TitlePressKeyTexture = getRenderer()->GenPngTexture("./resource/image/title/titlePressKey.png");

	m_RainParticleTexture = getRenderer()->GenPngTexture("./resource/image/title/rainParticle.png");

	m_RainParticle = getRenderer()->CreateParticleObject(100, -1280, -720, 1280, 720, 100, 100, 300, 300, 0, -100, 0, -100);

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

	m_RainSound = getSound()->CreateBGSound("./resource/sound/bg/rainSound.wav");
	getSound()->PlayBGSound(m_RainSound, true, 1.f);
}

GSETitle::~GSETitle()
{
	getSound()->StopBGSound(m_RainSound);
	getSound()->DeleteBGSound(m_RainSound);

	getRenderer()->DeleteTexture(m_TitleGrassTexture);
	getRenderer()->DeleteTexture(m_TitleLogoTexture);
	getRenderer()->DeleteTexture(m_TitlePressKeyTexture);
}

void GSETitle::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	if (m_CameraY > -350) {
		getRenderer()->SetCameraPos(0, m_CameraY);
		m_CameraY -= 200 * elapsedTimeInSec;
	}
	else {
		m_time += elapsedTimeInSec;

		if (!m_bNextState) {
			if (inputs->KEY_ENTER) {
				m_bNextState = true;
			}
		}
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

	getRenderer()->DrawParticle(m_RainParticle, 
		0, 0, 0, 
		1.f,
		1, 1, 1, 1, 
		0, -9.8 * m_time,
		m_RainParticleTexture, 
		1.f, m_time, 0.f);
}
