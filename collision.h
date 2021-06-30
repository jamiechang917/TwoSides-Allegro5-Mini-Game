#pragma once
#include "global.h"
#include "object.h"

int bounding_box_collision_detection(Object, Object);
int ray_box_intersection(float[2], float[2], float[2], float[2], float* [2], float*);
int ray_bounding_box_collision_detection(Object, Object, float, float* [2], float*);
int contact_detection(Object, Object);