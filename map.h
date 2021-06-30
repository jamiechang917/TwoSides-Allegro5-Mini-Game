#pragma once
#include "global.h"
#include "object.h"

typedef struct {
	int map_style;
	int map_object_len;
	ALLEGRO_BITMAP* background;
	Object map_object[20];
	float start_location[2]; // The initial location (x,y) of the character.
	float goal_location[2]; // The location(x, y) of the goal.
	//int restricted_area[n][4]; (x1,x2,y1,y2) form a zone
} Map;

void init_map(Map*, int);
