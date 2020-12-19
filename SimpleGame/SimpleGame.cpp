/*
Copyright 2020 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "GSEGame.h"
#include "GSETitle.h"

GSEGame* g_game = NULL;
GSETitle* g_title = NULL;
GSEInputs g_inputs;

int g_GameState = TITLE_STATE;
 
bool g_focused = true;

int g_prevTimeInMillisecond = 0;

void RenderScene(int temp)
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currentTime-g_prevTimeInMillisecond;
	g_prevTimeInMillisecond = currentTime;
	float elapsedTimeInSec = (float)elapsedTime / 1000.f;

	GSEInputs tempInputs;
	memcpy(&tempInputs, &g_inputs, sizeof(GSEInputs));

	if (g_focused)
	{
		if (g_GameState == TITLE_STATE) {
			g_title->Update(elapsedTimeInSec, &tempInputs);
			g_title->RenderScene();
		}
		else if (g_GameState == GAMING_STATE) {
			//g_game->Update(elapsedTimeInSec, &tempInputs);
			//g_game->RenderScene();
		}
		else if (g_GameState == ENDING_STATE) {

		}
	}

	glutSwapBuffers(); //double buffering, front->back, back->front

	glutTimerFunc(16, RenderScene, 16);
}

void Display(void)
{
	g_focused = true;
}

void Reshape(int width, int height)
{
	g_focused = false;
}

void Idle(void)
{
}

void MouseInput(int button, int state, int x, int y)
{
}

void KeyDownInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' | 'W':
		g_inputs.KEY_W = true;
		break;
	case 'a' | 'A':
		g_inputs.KEY_A = true;
		break;
	case 's' | 'S':
		g_inputs.KEY_S = true;
		break;
	case 'd' | 'D':
		g_inputs.KEY_D = true;
		break;
	}
}

void KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w' | 'W':
		g_inputs.KEY_W = false;
		break;
	case 'a' | 'A':
		g_inputs.KEY_A = false;
		break;
	case 's' | 'S':
		g_inputs.KEY_S = false;
		break;
	case 'd' | 'D':
		g_inputs.KEY_D = false;
		break;
	}
}

void SpecialKeyDownInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_inputs.ARROW_UP = true;
		break;
	case GLUT_KEY_DOWN:
		g_inputs.ARROW_DOWN = true;
		break;
	case GLUT_KEY_LEFT:
		g_inputs.ARROW_LEFT = true;
		break;
	case GLUT_KEY_RIGHT:
		g_inputs.ARROW_RIGHT = true;
		break;
	}
}

void SpecialKeyUpInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_inputs.ARROW_UP = false;
		break;
	case GLUT_KEY_DOWN:
		g_inputs.ARROW_DOWN = false;
		break;
	case GLUT_KEY_LEFT:
		g_inputs.ARROW_LEFT = false;
		break;
	case GLUT_KEY_RIGHT:
		g_inputs.ARROW_RIGHT = false;
		break;
	}
}

int main(int argc, char **argv)
{
	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(GSE_WINDOWSIZE_X, GSE_WINDOWSIZE_Y);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	// g_game = new GSEGame();
	g_title = new GSETitle();

	//Init g_inputs
	memset(&g_inputs, 0, sizeof(GSEInputs));

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyDownInput);
	glutKeyboardUpFunc(KeyUpInput);
	glutMouseFunc(MouseInput);
	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	g_prevTimeInMillisecond = glutGet(GLUT_ELAPSED_TIME);

	glutTimerFunc(16, RenderScene, 16);

	glutMainLoop();

	delete g_title;
	//delete g_game;

    return 0;
}

