#include "object.h"
#include "collision.h"

// ObjectQueue (linked list)
void init_objectQueue(ObjectQueue* object_queue) {
	object_queue->head = NULL;
	object_queue->size = 0;
}

void stack_objectQueue(ObjectQueue* object_queue, Object* object) { //add to the tail
	Object* last_object = object_queue->head;

	if (last_object == NULL) {//add first element
		object->next_object = NULL;
		object_queue->head = object;
		object_queue->size = 1;
	}
	else {
		while (1) {
			if (last_object->next_object == NULL) {
				object->next_object = NULL;
				last_object->next_object = object;
				object_queue->size += 1;
				break;
			}
			else {
				last_object = last_object->next_object;
			}
		}
	}
}

void delete_objectQueue(ObjectQueue* object_queue, Object* object) {

	// 0 -> 1 -> 2 -> 3 -> NULL
	// 0 -> 2 -> 3 -> NULL
	// cursor -> player -> object1 -> object2 -> ..

	int index = 0;
	Object* ptr = object_queue->head;
	while (ptr != object) {
		index += 1;
		ptr = ptr->next_object;
		if (ptr == NULL) { return; }
	}
	
	Object* target = object_queue->head;
	Object* prev_target = target;

	if (index == 0) { // headdb
		object_queue->head = object_queue->head->next_object;
		object_queue->size -= 1;
		//free head
		destroy_object(object_queue->head);
	}
	else {
		for (int i = 0; i < index; i++) {
			prev_target = target;
			target = target->next_object;
		}
		
		if (index == object_queue->size - 1) {
			prev_target->next_object = NULL;
		}
		else {
			prev_target->next_object = target->next_object;
		}
		object_queue->size -= 1;

		//free target
		destroy_object(target);
	}
}

//OBJECT_BUTTON
void init_button(Object* button, int type) {
	switch (type) {

	case BUTTON_HOME:
		break;
	case BUTTON_SINGLE_PLAYER:
		button->object_type = OBJECT_BUTTON;
		button->type = BUTTON_SINGLE_PLAYER;
		button->x = 700;
		button->y = 80;
		button->width = 400;
		button->height = 133;
		button->state = 0;
		button->img_index = 0;
		button->hide = 0;
		button->draw_direction = RIGHT;
		button->img[0] = al_load_bitmap("./resources/img/button/button1_1_400x133.png");
		button->img[1] = al_load_bitmap("./resources/img/button/button1_2_400x133.png");
		if (!(button->img[0])) { al_show_native_message_box(NULL, "warning", "'button1_1_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(button->img[1])) { al_show_native_message_box(NULL, "warning", "'button1_2_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case BUTTON_TWO_PLAYERS:
		button->object_type = OBJECT_BUTTON;
		button->type = BUTTON_TWO_PLAYERS;
		button->x = 700;
		button->y = 293;
		button->width = 400;
		button->height = 133;
		button->state = 0;
		button->img_index = 0;
		button->hide = 0;
		button->draw_direction = RIGHT;
		button->img[0] = al_load_bitmap("./resources/img/button/button1_1_400x133.png");
		button->img[1] = al_load_bitmap("./resources/img/button/button1_2_400x133.png");
		if (!(button->img[0])) { al_show_native_message_box(NULL, "warning", "'button1_1_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(button->img[1])) { al_show_native_message_box(NULL, "warning", "'button1_2_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case BUTTON_EXIT:
		button->object_type = OBJECT_BUTTON;
		button->type = BUTTON_EXIT;
		button->x = 700;
		button->y = 506;
		button->width = 400;
		button->height = 133;
		button->state = 0;
		button->img_index = 0;
		button->hide = 0;
		button->draw_direction = RIGHT;
		button->img[0] = al_load_bitmap("./resources/img/button/button1_1_400x133.png");
		button->img[1] = al_load_bitmap("./resources/img/button/button1_2_400x133.png");
		if (!(button->img[0])) { al_show_native_message_box(NULL, "warning", "'button1_1_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(button->img[1])) { al_show_native_message_box(NULL, "warning", "'button1_2_400x133.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	}

}

//OBJECT_CURSOR
void init_cursor(Object* cursor) {
	cursor->object_type = OBJECT_CURSOR;
	cursor->x = SCREENWIDTH/2;
	cursor->y = SCREENHEIGHT/2;
	cursor->type = CURSOR_NORMAL_MODE;
	cursor->state = 0;
	cursor->img_index = 0;
	cursor->hide = 0;
	cursor->draw_direction = RIGHT;
	cursor->img[0] = al_load_bitmap("./resources/img/cursor/cursor_blue_32x32.png");
	cursor->img[1] = al_load_bitmap("./resources/img/cursor/cursor_yellow_32x32.png");
	cursor->img[2] = al_load_bitmap("./resources/img/cursor/bomb_32x32.png");
	cursor->img[4] = al_load_bitmap("./resources/img/platform/platform_wood_32x192.png");
	cursor->img[5] = al_load_bitmap("./resources/img/platform/platform_frozen_32x128.png");
	cursor->img[6] = al_load_bitmap("./resources/img/platform/platform_spiky_4_32x64.png");
	cursor->img[7] = al_load_bitmap("./resources/img/bow/bow_1_64x64.png");
	cursor->img[8] = al_load_bitmap("./resources/img/fireball_launcher/fireball_launcher_1_32x32.png");
	cursor->img[9] = al_load_bitmap("./resources/img/spiky_ball/spiky_ball_32x32.png");
	cursor->img[10] = al_load_bitmap("./resources/img/coin/coin_1_32x32.png");
	cursor->img[11] = al_load_bitmap("./resources/img/arrow/arrow_8x32.png");
	cursor->img[12] = al_load_bitmap("./resources/img/fireball/fireball_1_16x16.png");




	if (!(cursor->img[0])) { al_show_native_message_box(NULL, "warning", "'cursor_blue_32x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	if (!(cursor->img[1])) {al_show_native_message_box(NULL, "warning", "'cursor_yellow_32x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);}
}

//OBJECT_CHARACTER
void init_character(Object* character, int type, char* character_name, float x, float y) { //type 0~xx
	character->object_type = OBJECT_CHARACTER;
	character->type = type;
	character->width = 24;
	character->height = 56;
	character->x = x - character->width;
	character->y = y - character->height;
	character->vx = 0;
	character->vy = 0;
	character->state = CHARACTER_IDLE;
	character->direction = RIGHT;
	character->draw_direction = RIGHT;
	character->img_index = 0;
	character->hide = 0;
	character->character_score = 0;
	character->character_name = character_name;
	character->collision_object_type = OBJECT_NONE;

	if (type == CHARACTER_01) { // appearance type 0
		//idle
		character->img[0] = al_load_bitmap("./resources/img/character/character1/character1_idle1_24x56.png");
		character->img[1] = al_load_bitmap("./resources/img/character/character1/character1_idle1_24x56.png");
		character->img[2] = al_load_bitmap("./resources/img/character/character1/character1_idle2_24x56.png");
		character->img[3] = al_load_bitmap("./resources/img/character/character1/character1_idle2_24x56.png");
		//walk
		character->img[4] = al_load_bitmap("./resources/img/character/character1/character1_walk1_24x56.png");
		character->img[5] = al_load_bitmap("./resources/img/character/character1/character1_walk2_24x56.png");
		character->img[6] = al_load_bitmap("./resources/img/character/character1/character1_walk3_24x56.png");
		character->img[7] = al_load_bitmap("./resources/img/character/character1/character1_walk4_24x56.png");
		//jump
		character->img[8] = al_load_bitmap("./resources/img/character/character1/character1_jump1_24x56.png");
		character->img[9] = al_load_bitmap("./resources/img/character/character1/character1_jump2_24x56.png");
		character->img[10] = al_load_bitmap("./resources/img/character/character1/character1_jump3_24x56.png");
		character->img[11] = al_load_bitmap("./resources/img/character/character1/character1_jump4_24x56.png");
		//dead
	}
	else if (type == CHARACTER_02) { // appearance type 0
		//idle
		character->img[0] = al_load_bitmap("./resources/img/character/character2/character2_idle1_24x56.png");
		character->img[1] = al_load_bitmap("./resources/img/character/character2/character2_idle1_24x56.png");
		character->img[2] = al_load_bitmap("./resources/img/character/character2/character2_idle2_24x56.png");
		character->img[3] = al_load_bitmap("./resources/img/character/character2/character2_idle2_24x56.png");
		//walk
		character->img[4] = al_load_bitmap("./resources/img/character/character2/character2_walk1_24x56.png");
		character->img[5] = al_load_bitmap("./resources/img/character/character2/character2_walk2_24x56.png");
		character->img[6] = al_load_bitmap("./resources/img/character/character2/character2_walk3_24x56.png");
		character->img[7] = al_load_bitmap("./resources/img/character/character2/character2_walk4_24x56.png");
		//jump
		character->img[8] = al_load_bitmap("./resources/img/character/character2/character2_jump1_24x56.png");
		character->img[9] = al_load_bitmap("./resources/img/character/character2/character2_jump2_24x56.png");
		character->img[10] = al_load_bitmap("./resources/img/character/character2/character2_jump3_24x56.png");
		character->img[11] = al_load_bitmap("./resources/img/character/character2/character2_jump4_24x56.png");
		//dead
	}

	for (int i = 0; i < 12; i++) {
		if (!(character->img[i])) { al_show_native_message_box(NULL, "warning", "character image file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}	
}

void move_all_object(ObjectQueue* object_queue) { //move character with keyboard input when timer ticks
	//check keyboard input
	al_get_keyboard_state(&keyboard_state);// key_board_state is defined in global.c

	Object* object = object_queue->head;
	
	while (object != NULL) {
		switch (object->object_type){

		case OBJECT_CHARACTER:
			if (object->state == CHARACTER_GOAL) {
				object->vx = 0;
				object->vy = 0;
			}

			if (object->state == CHARACTER_IDLE || object->state == CHARACTER_WALK) {
				if (object->collision_object_type != OBJECT_PLATFORM_FROZEN) {
					object->vx = 0; // stop when on the ground
					object->state = CHARACTER_IDLE;
				}
			}

			if (object->type == CHARACTER_01 && object->hide == 0) {
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_W)) {
					if (object->state == CHARACTER_IDLE || object->state == CHARACTER_WALK) { // can jump
						object->vy -= JUMP / FPS_CHARACTER_MOVE;
						al_play_sample(sound_jump, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_S)) {
					//character->vy = MOVESPEED/FPS_CHARACTER_MOVE;
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_A)) {
					object->vx = -MOVESPEED / FPS_CHARACTER_MOVE;
					object->draw_direction = LEFT;
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_D)) {
					object->vx = MOVESPEED / FPS_CHARACTER_MOVE;
					object->draw_direction = RIGHT;
				}
			}

			if (object->type == CHARACTER_02 && object->hide == 0) {
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_UP)) {
					if (object->state == CHARACTER_IDLE || object->state == CHARACTER_WALK) { // can jump
						object->vy -= JUMP / FPS_CHARACTER_MOVE;
						al_play_sample(sound_jump, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
					}
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_DOWN)) {
					//character->vy = MOVESPEED/FPS_CHARACTER_MOVE;
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_LEFT)) {
					object->vx = -MOVESPEED / FPS_CHARACTER_MOVE;
					object->draw_direction = LEFT;
				}
				if (al_key_down(&keyboard_state, ALLEGRO_KEY_RIGHT)) {
					object->vx = MOVESPEED / FPS_CHARACTER_MOVE;
					object->draw_direction = RIGHT;
				}
			}


			// gravity
			if (object->state == CHARACTER_AIR && object->vy < 1.5*MOVESPEED/FPS_CHARACTER_MOVE) { // terminal speed
				object->vy += GRAVITY / FPS_CHARACTER_MOVE;
			}

			object->x += object->vx;
			object->y += object->vy;	

			// check whether the character goes outside the window
			if (object->x < 0 || object->x > SCREENWIDTH - object->width) {
				object->x = max(0, min(object->x + object->width, SCREENWIDTH) - object->width);
				object->vx = 0;
			}
			else if (object->y < 0 || object->y > SCREENHEIGHT - object->height) {
				object->y = max(0, min(object->y + object->height, SCREENHEIGHT) - object->height);
				object->vy = 0;
			}
			break;
		
		case OBJECT_FIREBALL:
		case OBJECT_ARROW:
			object->x += object->vx;
			object->y += object->vy;
			break;

		default:
			break;
		}
		object = object->next_object;
		
	}

	
	
}

int check_collision_character(Object* character, Object* object) { // ray bounding box collision
	//float elapsed_time, float* contact_point[2], float* t_hit_nea
	float elapsed_time = 1.0;
	float contact_point[2] = { 0,0 };
	float t_hit_near = 0;

	int collision_state = ray_bounding_box_collision_detection(*character, *object, elapsed_time, &contact_point[2], &t_hit_near);

	if (collision_state && t_hit_near < 1) {

		character->collision_object_type = object->object_type;

		// if collide with coin and other objects
		if (character->collision_object_type == OBJECT_COIN || character->collision_object_type == OBJECT_ARROW || character->collision_object_type == OBJECT_FIREBALL) {
			object->hide = 1; // hide the object
		}

		else {

			if (collision_state == COLLISION_TOP) {
				//printf("TOP\n");
				//character->vy -= (1 - t_hit_near) * fabs(character->vy);
				character->y -= (1 - t_hit_near) * fabs(character->vy);
				character->vy = min(0, character->vy);
			}
			else if (collision_state == COLLISION_BOTTOM) {
				//printf("BOTTOM\n");
				//character->vy += (1 - t_hit_near) * fabs(character->vy);
				character->y += (1 - t_hit_near) * fabs(character->vy);
				character->vy = max(0, character->vy);
			}
			else if (collision_state == COLLISION_LEFT) {
				//printf("LEFT\n");
				//character->vx -= (1 - t_hit_near) * fabs(character->vx);
				character->x -= (1 - t_hit_near) * fabs(character->vx);
				character->vx = min(0, character->vx);
			}
			else if (collision_state == COLLISION_RIGHT) {
				//printf("RIGHT\n");
				//character->vx += (1 - t_hit_near) * fabs(character->vx);
				character->x += (1 - t_hit_near) * fabs(character->vx);
				character->vx = max(0, character->vx);
			}
		}

	}
	else { //ray_bbcd() sometimes will not return COLLISION_NONE even there is no collision. (but line of sight has obstacle)
		//printf("NONE\n");

		int contact_state = contact_detection(*character, *object);


		if (contact_state != CONTACT_NONE) { // it is contact with object
			character->collision_object_type = object->object_type;
			if (contact_state == CONTACT_TOP) { collision_state = COLLISION_TOP; }
			if (contact_state == CONTACT_BOTTOM) { collision_state = COLLISION_BOTTOM; }
			if (contact_state == CONTACT_LEFT) { collision_state = COLLISION_LEFT; }
			if (contact_state == CONTACT_RIGHT) { collision_state = COLLISION_RIGHT; }
		}

		else {
			collision_state = COLLISION_NONE;
		}

	}

	//character->x += character->vx;
	//character->y += character->vy;

	return collision_state;
}

void check_collsion_gamemap(ObjectQueue object_queue) {

	Object* object = object_queue.head;
	
	while (object != NULL) {

		if (object->object_type == OBJECT_CHARACTER && object->hide == 0) {
			int isCollision = 0;
			int isOnTheGround = 0;
			int isDead = 0;

			Object* object2 = object_queue.head;
			while (object2 != NULL) {
				if (object2->object_type != OBJECT_CHARACTER && object2->object_type != OBJECT_CURSOR && object2->hide == 0) { //prevent collsion check
					int collision_state = check_collision_character(object, object2);
					if (collision_state == COLLISION_TOP) {
						if (object2->object_type == OBJECT_PLATFORM_SPIKY && object2->state == PLATFORM_ON) {
							isDead = 1;
						}
						else {
							isOnTheGround = 1; // if there is one object that character stay on it, then isOnTheGround.
						}
					} 
					if (collision_state != COLLISION_NONE) { isCollision = 1; } // the obejct is colliding/contacting with at least one object
				}
				object2 = object2->next_object;
			}

			// change character state
			if (isDead) {
				object->state = CHARACTER_DEAD;
				object->hide = 1;
				al_play_sample(sound_die, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
			}

			else if (isOnTheGround) {
				if (object->vx == 0) { object->state = CHARACTER_IDLE; }
				else { object->state = CHARACTER_WALK;}
			}
			else {
				object->state = CHARACTER_AIR;
			}

			if (isCollision == 0) { // no collide with any object
				object->collision_object_type = OBJECT_NONE;
			}

		}
		else {
			object->collision_object_type = OBJECT_NONE;
		}
		object = object->next_object;
	}
		
}


// OBJECT_PLATFORM
void init_platform(Object* platform, float x, float y, int object_type) {

	platform->x = x;
	platform->y = y;
	
	platform->state = PLATFORM_OFF;
	platform->height = 0;
	platform->hide = 0;
	platform->direction = RIGHT;
	platform->draw_direction = RIGHT;
	
	switch (object_type){
	case OBJECT_PLATFORM_WOOD:
		platform->object_type = OBJECT_PLATFORM_WOOD;
		platform->width = 192;
		platform->height = 32;
		platform->vx = 0;
		platform->vy = 0;
		platform->img_index = 0;
		platform->img[0] = al_load_bitmap("./resources/img/platform/platform_wood_32x192.png");
		if (!(platform->img[0])) { al_show_native_message_box(NULL, "warning", "'platform_wood_32x192.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_PLATFORM_FROZEN:
		platform->object_type = OBJECT_PLATFORM_FROZEN;
		platform->width = 128;
		platform->height = 32;
		platform->vx = 0;
		platform->vy = 0;
		platform->img_index = 0;
		platform->img[0] = al_load_bitmap("./resources/img/platform/platform_frozen_32x128.png");
		if (!(platform->img[0])) { al_show_native_message_box(NULL, "warning", "'platform_frozen_32x128.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_PLATFORM_SPIKY:
		platform->object_type = OBJECT_PLATFORM_SPIKY;
		platform->width = 64;
		platform->height = 32;
		platform->vx = 0;
		platform->vy = 0;
		platform->img_index = 0;
		platform->img[0] = al_load_bitmap("./resources/img/platform/platform_spiky_1_32x64.png");
		platform->img[1] = al_load_bitmap("./resources/img/platform/platform_spiky_2_32x64.png");
		platform->img[2] = al_load_bitmap("./resources/img/platform/platform_spiky_3_32x64.png");
		platform->img[3] = al_load_bitmap("./resources/img/platform/platform_spiky_4_32x64.png");
		for (int i = 0; i < 4; i++) {
			if (!(platform->img[i])) { al_show_native_message_box(NULL, "warning", "'platform_spiky_32x64.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		}
		break;
	}

}

// OBJECT_GOAL
void init_goal(Object* goal, float x, float y) {
	goal->object_type = OBJECT_GOAL;
	goal->x = x;
	goal->y = y;
	goal->width = 32;
	goal->height = 64;
	goal->vx = 0;
	goal->vy = 0;
	goal->state = 0;
	goal->hide = 0;
	goal->direction = RIGHT;
	goal->draw_direction = RIGHT;
	goal->img_index = 0;
	goal->img[0] = al_load_bitmap("./resources/img/goal/goal_1_32x64.png");
	goal->img[1] = al_load_bitmap("./resources/img/goal/goal_2_32x64.png");
	goal->img[2] = al_load_bitmap("./resources/img/goal/goal_3_32x64.png");
	goal->img[3] = al_load_bitmap("./resources/img/goal/goal_4_32x64.png");

	for (int i = 0; i < 4; i++) {
		if (!(goal->img[i])) { al_show_native_message_box(NULL, "warning", "goal image file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}
}

// OBJECT_SPIKY_BALL
void init_spiky_ball(Object* spiky_ball, float x, float y) {
	spiky_ball->object_type = OBJECT_SPIKY_BALL;
	spiky_ball->width = 32;
	spiky_ball->height = 32;
	spiky_ball->x = x;
	spiky_ball->y = y;
	spiky_ball->vx = 0;
	spiky_ball->vy = 0;
	spiky_ball->state = 0;
	spiky_ball->hide = 0;
	spiky_ball->direction = RIGHT;
	spiky_ball->draw_direction = RIGHT;
	spiky_ball->img_index = 0;

	spiky_ball->img[0] = al_load_bitmap("./resources/img/spiky_ball/spiky_ball_32x32.png");
	if (!(spiky_ball->img[0])) { al_show_native_message_box(NULL, "warning", "'spiky_ball_32x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
}

// OBJECT_COIN
void init_coin(Object* coin, float x, float y) {
	coin->object_type = OBJECT_COIN;
	coin->width = 32;
	coin->height = 32;
	coin->x = x;
	coin->y = y;
	coin->vx = 0;
	coin->vy = 0;
	coin->state = 0;
	coin->hide = 0;
	coin->direction = RIGHT;
	coin->draw_direction = RIGHT;
	coin->img_index = 0;

	coin->img[0] = al_load_bitmap("./resources/img/coin/coin_1_32x32.png");
	coin->img[1] = al_load_bitmap("./resources/img/coin/coin_1_32x32.png");
	coin->img[2] = al_load_bitmap("./resources/img/coin/coin_2_32x32.png");
	coin->img[3] = al_load_bitmap("./resources/img/coin/coin_2_32x32.png");
	for (int i = 0; i < 4; i++) {
		if (!(coin->img[i])) { al_show_native_message_box(NULL, "warning", "'coin_32x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}
}

// OBJECT_ARROW_LAUNCHER

void init_arrow_launcher(Object* bow, float x, float y) {
	bow->object_type = OBJECT_ARROW_LAUNCHER;
	bow->width = 64;
	bow->height = 64;
	bow->x = x;
	bow->y = y;
	bow->vx = x;
	bow->vy = y;
	bow->state = BOW_IDLE;
	bow->hide = 0;
	bow->direction = RIGHT;
	bow->draw_direction = RIGHT;
	bow->img_index = 0;

	bow->img[0] = al_load_bitmap("./resources/img/bow/bow_1_64x64.png");
	bow->img[1] = al_load_bitmap("./resources/img/bow/bow_2_64x64.png");
	bow->img[2] = al_load_bitmap("./resources/img/bow/bow_3_64x64.png");
	bow->img[3] = al_load_bitmap("./resources/img/bow/bow_4_64x64.png");
	for (int i = 0; i < 4; i++) {
		if (!(bow->img[i])) { al_show_native_message_box(NULL, "warning", "'bow_64x64.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}
}

// OBJECT_ARROW

void init_arrow(Object* arrow, float x, float y, int direction) {
	arrow->object_type = OBJECT_ARROW;
	arrow->width = 8;
	arrow->height = 32;
	arrow->x = x;
	arrow->y = y;
	arrow->state = 0;
	arrow->hide = 0;
	arrow->direction = direction;
	arrow->draw_direction = RIGHT;
	arrow->img_index = 0;

	switch (direction) {
	case UP:
		arrow->vx = 0;
		arrow->vy = -ARROW_SPEED/ FPS_CHARACTER_MOVE;
		break;
	case DOWN:
		arrow->vx = 0;
		arrow->vy = ARROW_SPEED/ FPS_CHARACTER_MOVE;
		break;
	case LEFT:
		arrow->vx = -ARROW_SPEED/ FPS_CHARACTER_MOVE;
		arrow->vy = 0;
		break;
	case RIGHT:
		arrow->vx = ARROW_SPEED/ FPS_CHARACTER_MOVE;
		arrow->vy = 0;
		break;
	default:
		break;
	}
	arrow->img[0] = al_load_bitmap("./resources/img/arrow/arrow_8x32.png");
	if (!(arrow->img[0])) { al_show_native_message_box(NULL, "warning", "'arrow_8x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
}

// OBJECT_FIREBALL_LAUNCHER

void init_fireball_launcher(Object* launcher, float x, float y) {
	launcher->object_type = OBJECT_FIREBALL_LAUNCHER;
	launcher->width = 32;
	launcher->height = 32;
	launcher->x = x;
	launcher->y = y;
	launcher->vx = 0;
	launcher->vy = 0;
	launcher->state = 0;
	launcher->hide = 0;
	launcher->direction = RIGHT;
	launcher->draw_direction = RIGHT;
	launcher->img_index = 0;

	launcher->img[0] = al_load_bitmap("./resources/img/fireball_launcher/fireball_launcher_1_32x32.png");
	launcher->img[1] = al_load_bitmap("./resources/img/fireball_launcher/fireball_launcher_2_32x32.png");
	for (int i = 0; i < 2; i++) {
		if (!(launcher->img[i])) { al_show_native_message_box(NULL, "warning", "'fireball_launcher_32x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}
}

// OBJECT_FIREBALL

void init_fireball(Object* fireball, float x, float y) {
	fireball->object_type = OBJECT_FIREBALL;
	fireball->width = 16;
	fireball->height = 16;
	fireball->x = x;
	fireball->y = y;
	fireball->vx = randf(-1,1) * FIREBALL_SPEED / FPS_CHARACTER_MOVE;
	fireball->vy = randf(-1,1) * FIREBALL_SPEED / FPS_CHARACTER_MOVE;
	fireball->state = 0;
	fireball->hide = 0;
	fireball->direction = RIGHT;
	fireball->draw_direction = RIGHT;
	fireball->img_index = 0;

	fireball->img[0] = al_load_bitmap("./resources/img/fireball/fireball_1_16x16.png");
	fireball->img[1] = al_load_bitmap("./resources/img/fireball/fireball_2_16x16.png");
	for (int i = 0; i < 2; i++) {
		if (!(fireball->img[i])) { al_show_native_message_box(NULL, "warning", "'fireball_16x16.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
	}
}


// animation

void update_animation(Object* object) {
	switch (object->object_type) {
	case OBJECT_CHARACTER:
		if (object->state == CHARACTER_IDLE) {
			if (!(0 <= object->img_index && object->img_index < 4) || object->img_index == 3) { //triggered when index out of range
				object->img_index = 0; //start from 0
			}
			else {
				object->img_index += 1;
			}
		}
		else if (object->state == CHARACTER_WALK) {
			if (!(4 <= object->img_index && object->img_index < 8) || object->img_index == 7) {
				object->img_index = 4; //start from 4
			}
			else {
				object->img_index += 1;
			}
		}
		else if (object->state == CHARACTER_AIR) {
			if (!(0 <= object->img_index && object->img_index < 4) || object->img_index == 3) {
				object->img_index = 0; //start from 0
			}
			else {
				object->img_index += 1;
			}
		}
		else if (object->state == CHARACTER_JUMP) {
			if (!(8 <= object->img_index && object->img_index < 12) || object->img_index == 11) {
				object->img_index = 8; //start from 8
			}
			else {
				object->img_index += 1;
			}
		}
		else if (object->state == CHARACTER_DEAD) {
			if (!(0 <= object->img_index && object->img_index < 4) || object->img_index == 3) {
				object->img_index = 0; //start from 12
			}
			else {
				object->img_index += 1;
			}
		}
		break;


	case OBJECT_ARROW_LAUNCHER:
		if (object->state == BOW_IDLE) {
			object->img_index = 0;
		}
		else if (object->state == BOW_READY_TO_LAUNCH) {
			if (object->img_index == 3) {
				object->state = BOW_LAUNCH;
				object->img_index = 0;
			}
			else {
				object->img_index += 1;
			}
		}
		break;
	
	case OBJECT_FIREBALL:
	case OBJECT_FIREBALL_LAUNCHER:
		if (!(0 <= object->img_index && object->img_index < 2) || object->img_index == 1) {
			object->img_index = 0; //start from 0
		}
		else {
			object->img_index += 1;
		}
		break;

	case OBJECT_PLATFORM_SPIKY:
		if (object->state == PLATFORM_OFF) {
			if (object->img_index != 0) {
				object->img_index -= 1;
			}
		}
		else if (object->state == PLATFORM_ON) {
			if (object->img_index != 3) {
				object->img_index += 1;
			}
		}
		break;



	case OBJECT_COIN:
	case OBJECT_GOAL:
		if (!(0 <= object->img_index && object->img_index < 4) || object->img_index == 3) {
			object->img_index = 0; //start from 0
		}
		else {
			object->img_index += 1;
		}
		break;




	default:
		break;
	}

}


void update_all_animation(ObjectQueue* object_queue) {
	Object* object = object_queue->head;

	while (object != NULL) {
		update_animation(object);
		object = object->next_object;
	}
}




// draw
void draw_object(Object object) { // draw one object
	if (object.hide == 0) {
		if (object.draw_direction == RIGHT) { // default image is facing to the right side.
			if (object.img_index == 1) {
			}
			al_draw_bitmap(object.img[object.img_index], object.x, object.y, 0);
		}
		else if (object.draw_direction == LEFT) {
			al_draw_bitmap(object.img[object.img_index], object.x, object.y, ALLEGRO_FLIP_HORIZONTAL);
		}
	}
}

void draw_all_object(ObjectQueue object_queue) { // draw all objects in ObjectQueue
	Object* object= object_queue.head;
	while (1) {
		draw_object(*object);
		if (object->next_object == NULL) {
			break;
		}
		else {
			object = object->next_object;
		}
	}
}


// destroy
void destroy_object(Object* object) { // destroy one object
	switch (object->object_type) {

	case OBJECT_BUTTON:
		break;
	case OBJECT_CURSOR:
		al_destroy_bitmap(object->img[0]);
		al_destroy_bitmap(object->img[1]);
		al_destroy_bitmap(object->img[2]);
		al_destroy_bitmap(object->img[4]);
		al_destroy_bitmap(object->img[5]);
		break;

	case OBJECT_CHARACTER:
		for (int i = 0; i < 16; i++) {
			al_destroy_bitmap(object->img[i]);
		}
		break;

	case OBJECT_GOAL:
		for (int i = 0; i < 4; i++) {
			al_destroy_bitmap(object->img[i]);
		}
		break;

	case OBJECT_PLATFORM_WOOD:
	case OBJECT_PLATFORM_FROZEN:
	case OBJECT_ENVIRONMENT:
		al_destroy_bitmap(object->img[0]);
		break;
	}
}


void destroy_all_object(ObjectQueue* object_queue) { // destroy all objects in ObjectQueue
	Object* object = object_queue->head;

	while (object != NULL) {
		destroy_object(object);
		object = object->next_object;
	}
}