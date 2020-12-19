#pragma once

#define GLUT_KEY_ENTER 13

#define GSE_MAX_OBJECTS 1000
#define GSE_GRAVITY 9.8f

#define GSE_WINDOWSIZE_X 1280
#define GSE_WINDOWSIZE_Y 720

#define TITLE_STATE 0
#define GAMING_STATE 1
#define ENDING_STATE 2

typedef struct GSEInputs
{
	bool ARROW_UP;
	bool ARROW_DOWN;
	bool ARROW_LEFT;
	bool ARROW_RIGHT;

	bool KEY_W;
	bool KEY_A;
	bool KEY_S;
	bool KEY_D;

	bool KEY_ENTER;
};

typedef struct GSEUpdateParams
{
	float forceX;
	float forceY;
};

enum GSEObjectType {
	TYPE_HERO,
	TYPE_MOVABLE,
	TYPE_FIXED,
	TYPE_BULLET,
	TYPE_SWORD,
};

enum GSEObjectState {
	STATE_GROUND,
	STATE_FALLING,
	STATE_FLY,
	STATE_SWIMMING,
};