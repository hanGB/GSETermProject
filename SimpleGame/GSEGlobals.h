#pragma once

#define GLUT_KEY_ENTER 13

#define GSE_MAX_OBJECTS 1000
#define GSE_GRAVITY 9.8f

#define GSE_WINDOWSIZE_X 1280
#define GSE_WINDOWSIZE_Y 720

#define TITLE_STATE 0
#define BATTLE_STATE 1
#define ENDING_STATE 2

#define RAILROAD_MAP 0
#define FIRE_MAP 1

#define PLAYER_MAXIUM_SPEED 5

#define ANIMATION_IDLE 0
#define ANIMATION_RUN 1
#define ANIMATION_ATTACK 2
#define ANIMATION_DIE 3

#define NON_DATA -1000000

#define ENEMY_SOLDIER 0
#define ENEMY_SOLDIER_WITH_GUN 1

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

	bool KEY_SPACE;

	bool KEY_ENTER;
};

typedef struct GSEUpdateParams
{
	float forceX;
	float forceY;
};

enum GSEObjectType {
	TYPE_HERO,
	TYPE_ENEMY,
	TYPE_FIXED,
	TYPE_BULLET,
	TYPE_SWORD,
	TYPE_WALL
};

enum GSEObjectState {
	STATE_GROUND,
	STATE_FALLING,
	STATE_FLY,
	STATE_SWIMMING,
};