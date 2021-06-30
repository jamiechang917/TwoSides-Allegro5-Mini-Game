#include "collision.h"

int bounding_box_collision_detection(Object a, Object b) {	// a collide to b
	/*                 |-->x
	* _______________  |
	* | A           |  y
	* |        _____|______     
	* |	      |     |     |
	* |_______|_____|     |
	*		  |			B |
	*         |___________|
	*/
	float a_x1 = a.x, a_x2 = a.x + a.width, a_y1 = a.y, a_y2 = a.y + a.height;
	float b_x1 = b.x, b_x2 = b.x + b.width, b_y1 = b.y, b_y2 = b.y + b.height;
	float delta_x, delta_y;
	if (a_x1 <= b_x2 &&
		a_x2 >= b_x1 &&
		a_y1 <= b_y2 &&
		a_y2 >= b_y1) { // collision detected!
		
		if ((b_y1 < a_y2 && a_y2 < b_y2) && a_y1 < b_y1) { // Top
			if ((a_x1 <= b_x1 && b_x2 <= a_x2) || (b_x1 <= a_x1 && a_x2 <= b_x2)) {
				return COLLISION_TOP;
			}
			else if (a_x1 < b_x1) { // Left
				delta_x = fabs(a_x2 - b_x1);
				delta_y = fabs(a_y2 - b_y1);
				if (delta_x > delta_y) { return COLLISION_TOP; }
				else { return COLLISION_LEFT; }
			}
			else if (a_x2 > b_x2) { // Right
				delta_x = fabs(a_x1 - b_x2);
				delta_y = fabs(a_y2 - b_y1);
				if (delta_x > delta_y) { return COLLISION_TOP; }
				else { return COLLISION_RIGHT; }
			}
			else {
				return COLLISION_INSIDE;
			}
		}

		else if ((b_y1 <= a_y1 && a_y1 <= b_y2) && a_y2 > b_y2) { // Top
			if ((a_x1 < b_x1 && b_x2 < a_x2) || (b_x1 < a_x1 && a_x2 < b_x2)) {
				return COLLISION_BOTTOM;
			}
			else if (a_x1 < b_x1) { // Left
				delta_x = fabs(a_x2 - b_x1);
				delta_y = fabs(a_y1 - b_y2);
				if (delta_x > delta_y) { return COLLISION_BOTTOM; }
				else { return COLLISION_LEFT; }
			}
			else if (a_x2 > b_x2) { // Right
				delta_x = fabs(a_x1 - b_x2);
				delta_y = fabs(a_y1 - b_y2);
				if (delta_x > delta_y) { return COLLISION_BOTTOM; }
				else { return COLLISION_RIGHT; }
			}
			else {
				return COLLISION_INSIDE;
			}
		}

		else if ((b_x1 < a_x2 && a_x2 < b_x2) && a_x1 < b_x1) { // Left
			return COLLISION_LEFT;
		}
		else if ((b_x1 < a_x1 && a_x1 < b_x2) && a_x2 > b_x2) { // Right
			return COLLISION_RIGHT;
		}
		else {
			return COLLISION_INSIDE;
		}

	}
	else {
		return COLLISION_NONE;
	}	
}


int ray_box_intersection(float ray_origin[2], float ray_dir[2], float box_pos[2], float box_size[2], float* contact_point[2], float* t_hit_near) {
	/*
	*  * ray origin
	*   \ 
	*box \_____ near intersection.
	*pos |\    |
	*    | \   |
	*    |  \  |
	*    |   \ |
	*    |____\|
	*          \ box pos + box size
	*           \ far intersection.
	*            \ 
	*             * ray origin + ray direction
	*/


	
	float t_near[2], t_far[2]; // near time and far time for x, y axis
	t_near[0] = (box_pos[0] - ray_origin[0]) / ray_dir[0];
	t_near[1] = (box_pos[1] - ray_origin[1]) / ray_dir[1];
	t_far[0] = (box_pos[0] + box_size[0] - ray_origin[0]) / ray_dir[0];
	t_far[1] = (box_pos[1] + box_size[1] - ray_origin[1]) / ray_dir[1];
	
	if (t_near[0] > t_far[0]) {
		float tmp = t_near[0];
		t_near[0] = t_far[0];
		t_far[0] = tmp;
	}

	if (t_near[1] > t_far[1]) {
		float tmp = t_near[1];
		t_near[1] = t_far[1];
		t_far[1] = tmp;
	}

	if (t_near[0] > t_far[1] || t_near[1] > t_far[0]) {
		return COLLISION_NONE;
	}

	*t_hit_near = max(t_near[0], t_near[1]);
	float t_hit_far = min(t_far[0], t_far[1]);

	if (t_hit_far < 0) {
		return COLLISION_NONE;
	}

	if (t_near[0] > t_near[1]) {
		if (ray_dir[0] < 0) {
			return COLLISION_RIGHT;
		}
		else {
			return COLLISION_LEFT;
		}
	}

	else if (t_near[0] < t_near[1]) {
		if (ray_dir[1] < 0) {
			return COLLISION_BOTTOM;
		}
		else {
			return COLLISION_TOP;
		}
	}
}

int ray_bounding_box_collision_detection(Object a, Object b, float elapsed_time, float* contact_point[2], float* t_hit_near) { // a collide to b
	
	if (a.vx == 0 && a.vy == 0) {
		return COLLISION_NONE;
	}

	float ray_origin[2] = { a.x + a.width / 2, a.y + a.height / 2 };
	float ray_dir[2] = { a.vx*elapsed_time, a.vy*elapsed_time }; // the end point of the ray will be affected by input elaspedtime.
	float box_pos[2] = { b.x - a.width / 2, b.y - a.height / 2 };
	float box_size[2] = { b.width + a.width, b.height + a.height };

	return ray_box_intersection(ray_origin, ray_dir, box_pos, box_size, contact_point, t_hit_near);
}

int contact_detection(Object a, Object b) { // two objects contact with each other
	float a_x1 = round(a.x), a_x2 = round(a.x + a.width), a_y1 = round(a.y), a_y2 = round(a.y + a.height);
	float b_x1 = round(b.x), b_x2 = round(b.x + b.width), b_y1 = round(b.y), b_y2 = round(b.y + b.height);
	if (a_x1 <= b_x2 &&
		a_x2 >= b_x1 &&
		a_y1 <= b_y2 &&
		a_y2 >= b_y1) {

		if (a_y2 == b_y1) { return CONTACT_TOP; }
		else if (a_y1 == b_y2) { return CONTACT_BOTTOM;}
		else if (a_x2 == b_x1) { return CONTACT_LEFT; }
		else if (a_x1 == b_x2) { return CONTACT_RIGHT; }
	}
	return CONTACT_NONE;
}