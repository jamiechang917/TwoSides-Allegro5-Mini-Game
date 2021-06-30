#pragma once
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define max(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)

enum mode {
	MODE_HOME = 0,
	MODE_SINGLE_PLAYER = 1,
	MODE_TWO_PLAYERS = 2,

	MODE_EXIT = 99,
	MODE_DEBUG = 100,
};

enum direction {
	RIGHT = 0,
	LEFT = 1,
	UP = 2,
	DOWN = 3,
};

enum object_type {
	OBJECT_NONE = 0,
	OBJECT_CURSOR = 1,
	OBJECT_BUTTON = 2,
	OBJECT_CHARACTER = 3,
	OBJECT_PLATFORM_WOOD = 4,
	OBJECT_PLATFORM_FROZEN,
	OBJECT_PLATFORM_SPIKY,
	OBJECT_ARROW_LAUNCHER,
	OBJECT_FIREBALL_LAUNCHER,
	OBJECT_SPIKY_BALL,
	OBJECT_COIN,
	//OBJECT_TELEPORT_DOOR,
	//OBJECT_SAW_ARM,
	OBJECT_BOMB,
	OBJECT_ARROW,
	OBJECT_FIREBALL,
	OBJECT_GOAL,

	OBJECT_ENVIRONMENT = 100,
};

enum button_type {
	BUTTON_HOME = 0,
	BUTTON_SINGLE_PLAYER = 1,
	BUTTON_TWO_PLAYERS = 2,
	BUTTON_EXIT = 3,
};

enum cursor_type {
	CURSOR_NORMAL_MODE = 1,
	CURSOR_PLACE_MODE = 2,
	CURSOR_DELETE_MODE = 3,
};

enum character_type {
	CHARACTER_01 = 1,
	CHARACTER_02 = 2,
	CHARACTER_03 = 3,
	CHARACTER_04 = 4,
};

enum map_type {
	MAP_CITY = 0,
	MAP_SKY = 1,
};

enum collision_state {
	COLLISION_NONE = 0,
	COLLISION_INSIDE = 1,
	COLLISION_TOP = 2,
	COLLISION_BOTTOM = 3,
	COLLISION_LEFT = 4,
	COLLISION_RIGHT = 5,
};

enum contact_state {
	CONTACT_NONE = 0,
	CONTACT_TOP = 1,
	CONTACT_BOTTOM = 2,
	CONTACT_LEFT = 3,
	CONTACT_RIGHT = 4
};

enum character_state {
	CHARACTER_IDLE = 0,
	CHARACTER_WALK = 1,
	CHARACTER_JUMP = 2,
	CHARACTER_AIR  = 3,
	CHARACTER_DEAD = 4,
	CHARACTER_GOAL = 5,
};

enum bow_state {
	BOW_IDLE = 0,
	BOW_READY_TO_LAUNCH = 1,
	BOW_LAUNCH = 2,
};

enum fireball_launcher_state {
	FIREBALL_LAUNCHER_IDLE = 0,
	FIREBALL_LAUNCHER_LAUNCH = 1,
};

enum platform_state {
	PLATFORM_OFF = 0,
	PLATFORM_ON = 1,
};



extern const char GAME_NAME[];
extern const char VERSION[];

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;
extern const float FPS;
extern const float FPS_ANIMATION;
extern const float FPS_CHARACTER_MOVE;

extern const float MOVESPEED;
extern const float GRAVITY;
extern const float JUMP;
extern const float WIN_SCORE;

extern const float LAUNCH_ARROW_TIME;
extern const float ARROW_SPEED;
extern const float FIREBALL_SPEED;


extern ALLEGRO_DISPLAY* display;
extern ALLEGRO_EVENT_QUEUE* queue;
extern ALLEGRO_KEYBOARD_STATE keyboard_state;
extern ALLEGRO_TIMER* timer_fps;
extern ALLEGRO_SAMPLE* sound_jump;
extern ALLEGRO_SAMPLE* sound_die;
extern ALLEGRO_SAMPLE* sound_coin;

float randf(float, float);
int randi(int, int);