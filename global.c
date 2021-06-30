#include "global.h"

const char GAME_NAME[] = "TwoSides";
const char VERSION[] = "0.1";

const int SCREENWIDTH = 1280;
const int SCREENHEIGHT = 720;
const float FPS = 75.0;
const float FPS_ANIMATION = 10.0;
const float FPS_CHARACTER_MOVE = 360.0; // prevent movespeed affected by fps.

const float MOVESPEED = 300.0; // how many pixel character can walk in one second
const float GRAVITY = 8.0;
const float JUMP = 900.0;
const float WIN_SCORE = 10; // The score needs to be obtained to win the game.

const float LAUNCH_ARROW_TIME = 3; // the interval time between every arrow launch.
const float ARROW_SPEED = 100;
const float FIREBALL_SPEED = 300;

ALLEGRO_DISPLAY* display = NULL;
ALLEGRO_EVENT_QUEUE* queue = NULL; //remove
ALLEGRO_KEYBOARD_STATE keyboard_state; // remove
ALLEGRO_TIMER* timer_fps = NULL;
ALLEGRO_SAMPLE* sound_jump = NULL;
ALLEGRO_SAMPLE* sound_die = NULL;
ALLEGRO_SAMPLE* sound_coin = NULL;

float randf(float low, float high) {
    return (float) (rand() / (float)(RAND_MAX)) * fabs(low - high) + low;
}

int randi(int low, int high) {
    return low + rand() / (RAND_MAX / (high - low + 1) + 1);
}