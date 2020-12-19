#include "stdafx.h"
#include "GSEEnding.h"

GSEEnding::GSEEnding()
{
	m_bNextState = false;

	cameraY = 200;
	m_time = 0;

	m_KZTexture = getRenderer()->GenPngTexture("./resource/image/ending/KZTexture.png");
	m_MSTexture = getRenderer()->GenPngTexture("./resource/image/ending/MSTexture.png");
	m_MusicTexture = getRenderer()->GenPngTexture("./resource/image/ending/musicTexture.png");
	m_SignTexture = getRenderer()->GenPngTexture("./resource/image/ending/signTexture.png");
	m_KPUTexture = getRenderer()->GenPngTexture("./resource/image/ending/kpuTexture.png");
	m_tyfpTexture = getRenderer()->GenPngTexture("./resource/image/ending/tyfpTexture.png");	
	m_finTexture = getRenderer()->GenPngTexture("./resource/image/ending/finTexture.png");

	int textID = AddObject(0, -100, 1, 345, 48, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_KZTexture);

	textID = AddObject(0, -250, 1, 345, 48, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_MSTexture);

	textID = AddObject(0, -400, 1, 641, 87, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_MusicTexture);

	textID = AddObject(0, -550, 1, 290, 42, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_SignTexture);

	textID = AddObject(0, -700, 1, 335, 44, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_KPUTexture);

	textID = AddObject(0, -900, 1, 649, 61, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_tyfpTexture);

	textID = AddObject(450, -1400, 1, 86, 54, 0, 0, 0, 0, 10000);
	getObject(textID)->SetType(GSEObjectType::TYPE_FIXED);
	getObject(textID)->SetApplyPhysics(true);
	getObject(textID)->SetLife(100000000.f);
	getObject(textID)->SetLifeTime(100000000.f);
	getObject(textID)->SetTextureID(m_finTexture);

	m_BackgroundSound = getSound()->CreateBGSound("./resource/sound/bg/endingSound.mp3");

	getSound()->PlayBGSound(m_BackgroundSound, true, 1.0f);
}

GSEEnding::~GSEEnding()
{
	getSound()->StopBGSound(m_BackgroundSound);
	getSound()->DeleteBGSound(m_BackgroundSound);

	getRenderer()->DeleteTexture(m_KZTexture);
	getRenderer()->DeleteTexture(m_MSTexture);
	getRenderer()->DeleteTexture(m_MusicTexture);
	getRenderer()->DeleteTexture(m_SignTexture);
	getRenderer()->DeleteTexture(m_KPUTexture);
	getRenderer()->DeleteTexture(m_tyfpTexture);
	getRenderer()->DeleteTexture(m_finTexture);
}

void GSEEnding::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	if (inputs->KEY_SPACE) {
		elapsedTimeInSec = elapsedTimeInSec * 2;
	}

	if (inputs->KEY_ENTER) {
		m_bNextState = true;
	}

	if (cameraY > -1150) {
		cameraY -= 100 * elapsedTimeInSec;
		getRenderer()->SetCameraPos(0, cameraY);
	}
	else {
		m_time += elapsedTimeInSec;
		if (m_time > 3)
			m_bNextState = true;
	}
	
}

void GSEEnding::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	for (int i = 0; i < GSE_MAX_OBJECTS; i++)
	{
		if (getObject(i) != NULL)
		{
			float x, y, depth;
			getObject(i)->GetPosition(&x, &y, &depth);
			float sx, sy;
			getObject(i)->GetSize(&sx, &sy);

			int textureID = getObject(i)->GetTextureID();

			getRenderer()->DrawTextureRect(
					x, y, depth,
					sx, -sy, 1.f,
					1.f, 1.f, 1.f, 1.f,
					textureID);

		}
	}
}
