#pragma once
#include "global.h"

typedef struct Object Object;

struct Object{ // universal properties (properties for drawing, collision detection.)
	// general properties
	int object_type; // see object_type defined in global.h
	float x, y;
	float width, height; // bounding box
	float vx, vy; // velocity
	int type; // could be used for different types of object appearance.
	int state;
	int hide; // temporary hide the element
	int direction;
	int draw_direction; // default is RIGHT, if this is set to LEFT, it will flip the bitmap horizontally.
	int img_index; // Current index of the img[]. Draw the bitmap according to this index.
	ALLEGRO_BITMAP* img[50]; // all images will store in this array.


	// linked list option
	Object* next_object;

	// OBJECT_CURSOR option
	int cursor_place_object_type;

	// OBJECT_CHARACTER option
	char* character_name;
	int character_score;
	int collision_object_type; // collide with which type of the object
};


typedef struct { // linked list for Object
	Object* head;
	int size;
} ObjectQueue;




void init_objectQueue(ObjectQueue*);
void stack_objectQueue(ObjectQueue*, Object*);
void delete_objectQueue(ObjectQueue*, Object*); // delete object with input index

void init_button(Object*, int);

void init_cursor(Object*);

void init_character(Object*, int, char*, float, float);
void move_all_object(Object*);
int  check_collision_character(Object*, Object*);
void check_collsion_gamemap(ObjectQueue);

void init_platform(Object*, float, float, int);
void init_goal(Object*, float, float);
void init_spiky_ball(Object*, float, float);
void init_coin(Object*, float, float);
void init_arrow_launcher(Object*, float, float);
void init_arrow(Object*, float, float, int);
void init_fireball_launcher(Object*, float, float);
void init_fireball(Object*, float, float);


void update_animation(Object*);
void update_all_animation(ObjectQueue*);

void draw_object(Object);
void draw_all_object(ObjectQueue);

void destroy_object(Object*);
void destroy_all_object(ObjectQueue*);