#include "stdafx.h"
#include "GSEEnding.h"

GSEEnding::GSEEnding()
{
	m_bNextState = false;

	m_KZTexture = getRenderer()->GenPngTexture("./resource/image/ending/KZTexture.png");
	m_MSTexture = getRenderer()->GenPngTexture("./resource/image/ending/MSTexture.png");
	m_MusicTexture = getRenderer()->GenPngTexture("./resource/image/ending/musicTexture.png");

	m_BackgroundSound = getSound()->CreateBGSound("./resource/sound/bg/endingSound.mp3");

	getSound()->PlayBGSound(m_BackgroundSound, true, 1.0f);

	cameraY = 0;
}

GSEEnding::~GSEEnding()
{
	getSound()->StopBGSound(m_BackgroundSound);
	getSound()->DeleteBGSound(m_BackgroundSound);

	getRenderer()->DeleteTexture(m_KZTexture);
	getRenderer()->DeleteTexture(m_MSTexture);
	getRenderer()->DeleteTexture(m_MusicTexture);
}

void GSEEnding::Update(float elapsedTimeInSec, GSEInputs* inputs)
{
	cameraY -=  100 * elapsedTimeInSec;
	getRenderer()->SetCameraPos(0, cameraY);
}

void GSEEnding::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
