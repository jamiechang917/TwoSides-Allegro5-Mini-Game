#include "mode_two_players.h"
#include "map.h"

// two players mode

//local variables
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_KEYBOARD_STATE keyboard_state;

static ObjectQueue game_objects;
static Object cursor;

static Object player1, player2;

//map
static Map map;

static ALLEGRO_TIMER* timer_animation = NULL;
static ALLEGRO_TIMER* timer_character_move = NULL;
static ALLEGRO_TIMER* timer_arrow_launcher = NULL;

//debug
static int show_bounding_box = 0;

static void start_all_timer();
static void stop_all_timer();
static void destroy_all_timer();
static void place_object(ObjectQueue*, Object);
static int  delete_object(ObjectQueue*, Object);
static void check_all_event(ObjectQueue*);
static void reset_all_object(ObjectQueue, Map);
static void win_scene(Map, int);
static void next_round_scene(int, Map, Object, Object, ObjectQueue*);
static void random_object(Object*, float, float);


void MODE_TWO_PLAYERS_init() {
	//cursor
	init_cursor(&cursor);

	//map 
	int map_type = randi(0, 1);
	init_map(&map, map_type);

	//load ObjectQueue
	init_objectQueue(&game_objects);
	
	init_character(&player1, CHARACTER_01, "Player1", map.start_location[0], map.start_location[1] + 56);
	init_character(&player2, CHARACTER_02, "Player2", map.start_location[0] - 5, map.start_location[1] + 56);
	stack_objectQueue(&game_objects, &cursor);
	stack_objectQueue(&game_objects, &player1);
	stack_objectQueue(&game_objects, &player2);


	for (int i = 0; i < map.map_object_len; i++) {
		stack_objectQueue(&game_objects, &map.map_object[i]);
	}


	//timer
	timer_animation = al_create_timer(1.0 / FPS_ANIMATION);
	timer_character_move = al_create_timer(1.0 / FPS_CHARACTER_MOVE);
	timer_arrow_launcher = al_create_timer(LAUNCH_ARROW_TIME);

	//event queue
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer_fps));
	al_register_event_source(queue, al_get_timer_event_source(timer_animation));
	al_register_event_source(queue, al_get_timer_event_source(timer_character_move));
	al_register_event_source(queue, al_get_timer_event_source(timer_arrow_launcher));

}

int MODE_TWO_PLAYERS_run() {
	int round = 1;
	bool running = true;
	printf("Running (MODE_TWO_PLAYERS)...\n");


	next_round_scene(round, map, player1, player2, &game_objects);
	round++;

	//start timer
	start_all_timer();

	ALLEGRO_EVENT event;
	ALLEGRO_KEYBOARD_STATE keyboard_state;

	while (running) {
		al_wait_for_event(queue, &event);
		al_get_keyboard_state(&keyboard_state);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // exit by closing the window
			running = false;
			return MODE_EXIT;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_AXES) { // move mouse
			al_set_mouse_xy(display, min(max(0, event.mouse.x), SCREENWIDTH), min(max(0, event.mouse.y), SCREENHEIGHT));
			cursor.x = event.mouse.x;
			cursor.y = event.mouse.y;
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			switch (cursor.type) {
			case CURSOR_NORMAL_MODE:
				cursor.img_index = 0;
				break;
			case CURSOR_PLACE_MODE:
				cursor.img_index = cursor.cursor_place_object_type;
				break;
			}
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // check
			switch (cursor.type) {
			case CURSOR_NORMAL_MODE:
				cursor.img_index = 1;
				break;
			case CURSOR_PLACE_MODE:
				place_object(&game_objects, cursor);
				break;
			case CURSOR_DELETE_MODE:
				delete_object(&game_objects, cursor);
				break;

			}
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (al_key_down(&keyboard_state, ALLEGRO_KEY_H)) {
				running = false;
				return MODE_HOME;
			}

			if (al_key_down(&keyboard_state, ALLEGRO_KEY_M)) { //debug
				show_bounding_box = 1 - show_bounding_box;

			}

			if (al_key_down(&keyboard_state, ALLEGRO_KEY_P)) {
				if (cursor.type != CURSOR_PLACE_MODE) {
					cursor.type = CURSOR_PLACE_MODE;
					cursor.cursor_place_object_type = OBJECT_PLATFORM_WOOD;
					cursor.img_index = cursor.cursor_place_object_type;
				}
				else { // back to normal mode
					if (cursor.cursor_place_object_type < OBJECT_COIN) {
						cursor.cursor_place_object_type += 1;
						cursor.img_index = cursor.cursor_place_object_type;
					}
					else {
						cursor.type = CURSOR_NORMAL_MODE;
						cursor.img_index = 0;
					}
				}

			}

			if (al_key_down(&keyboard_state, ALLEGRO_KEY_L)) {
				if (cursor.type != CURSOR_DELETE_MODE) {
					cursor.type = CURSOR_DELETE_MODE;
					cursor.img_index = 2;

				}
				else {
					cursor.type = CURSOR_NORMAL_MODE;
					cursor.img_index = 0;
				}
			}
		}


		if (event.type == ALLEGRO_EVENT_TIMER) { // refresh every 1/FPS seconds.

			if (event.timer.source == timer_fps) { // regular timer
				//background 
				al_draw_bitmap(map.background, 0, 0, NULL);

				draw_all_object(game_objects);
				draw_object(player1);
				draw_object(player2);
				draw_object(cursor);

				if (show_bounding_box) { // debug
					Object* object = game_objects.head;
					while (object != NULL) {
						al_draw_rectangle(object->x, object->y, object->x + object->width, object->y + object->height, al_map_rgb(255, 0, 0), 5);
						object = object->next_object;
					}
				}

				al_flip_display();
			}

			else if (event.timer.source == timer_character_move) {
				move_all_object(&game_objects);
				check_collsion_gamemap(game_objects);
				check_all_event(&game_objects);
				if ((player1.state == CHARACTER_GOAL || player1.state == CHARACTER_DEAD) && (player2.state == CHARACTER_GOAL || player2.state == CHARACTER_DEAD)) {
					stop_all_timer();
					reset_all_object(game_objects, map);
					if (player1.character_score >= WIN_SCORE || player2.character_score >= WIN_SCORE) {
						if (player1.character_score > player2.character_score) { win_scene(map, 1); }
						else if (player2.character_score > player1.character_score) { win_scene(map, 2); }
						else if (player2.character_score == player1.character_score) { win_scene(map, 0); }
						running = false;
						return MODE_HOME;
					}

					next_round_scene(round, map, player1, player2, &game_objects);
					round++;
					start_all_timer();
				}



			}

			else if (event.timer.source == timer_arrow_launcher) {
				Object* object = game_objects.head;
				while (object != NULL) {
					if (object->object_type == OBJECT_ARROW_LAUNCHER) {
						object->state = BOW_READY_TO_LAUNCH;
					}
					if (object->object_type == OBJECT_PLATFORM_SPIKY) {
						if (object->state == PLATFORM_OFF) { object->state = PLATFORM_ON; }
						else { object->state = PLATFORM_OFF; }
					}
					if (object->object_type == OBJECT_FIREBALL_LAUNCHER) {
						object->state = FIREBALL_LAUNCHER_LAUNCH;
					}

					object = object->next_object;
				}
			}


			else if (event.timer.source == timer_animation) {
				update_all_animation(&game_objects);
			}
		}
	}
	al_destroy_event_queue(&event);
}


void MODE_TWO_PLAYERS_destroy() {
	//stop timer
	stop_all_timer();
	//destroy timer
	destroy_all_timer();
	//destroy bitmap
	destroy_all_object(&game_objects);
}

// =========================================================

// local functions

void start_all_timer() {
	al_start_timer(timer_animation);
	al_start_timer(timer_character_move);
	al_start_timer(timer_arrow_launcher);
}

void stop_all_timer() {
	al_stop_timer(timer_animation);
	al_stop_timer(timer_character_move);
	al_stop_timer(timer_arrow_launcher);
}

void destroy_all_timer() {
	al_destroy_timer(timer_animation);
	al_destroy_timer(timer_character_move);
	al_destroy_timer(timer_arrow_launcher);
}


void place_object(ObjectQueue* object_queue, Object cursor) {
	Object* object = malloc(sizeof(Object));
	switch (cursor.cursor_place_object_type) {
	case OBJECT_PLATFORM_WOOD:
		init_platform(object, cursor.x, cursor.y, OBJECT_PLATFORM_WOOD);
		break;
	case OBJECT_PLATFORM_FROZEN:
		init_platform(object, cursor.x, cursor.y, OBJECT_PLATFORM_FROZEN);
		break;
	case OBJECT_PLATFORM_SPIKY:
		init_platform(object, cursor.x, cursor.y, OBJECT_PLATFORM_SPIKY);
		break;
	case OBJECT_ARROW_LAUNCHER:
		init_arrow_launcher(object, cursor.x, cursor.y);
		break;
	case OBJECT_ARROW:
		init_arrow(object, cursor.x, cursor.y, UP);
		break;
	case OBJECT_FIREBALL_LAUNCHER:
		init_fireball_launcher(object, cursor.x, cursor.y);
		break;
	case OBJECT_FIREBALL:
		init_fireball(object, cursor.x, cursor.y);
		break;
	case OBJECT_SPIKY_BALL:
		init_spiky_ball(object, cursor.x, cursor.y);
		break;
	case OBJECT_COIN:
		init_coin(object, cursor.x, cursor.y);
		break;
	}
	stack_objectQueue(object_queue, object);
}

int delete_object(ObjectQueue* object_queue, Object cursor) {
	Object* object = object_queue->head;
	int isDelete = 0;
	while (object != NULL) {
		if (object->object_type != OBJECT_CURSOR &&
			object->object_type != OBJECT_BUTTON &&
			object->object_type != OBJECT_CHARACTER &&
			object->object_type != OBJECT_GOAL &&
			object->object_type != OBJECT_ENVIRONMENT) {
			int isCollide = bounding_box_collision_detection(cursor, *object);
			if (isCollide == COLLISION_INSIDE) {
				delete_objectQueue(object_queue, object); // automatic destroy the bitmap
				isDelete = 1;
			}
		}
		object = object->next_object;
	}
	return isDelete;
}

void check_all_event(ObjectQueue* object_queue) { // dead, goal, or other event
	Object* object = object_queue->head;
	
	while (object != NULL) {
		switch (object->object_type) {

		case OBJECT_CHARACTER:
			switch (object->collision_object_type) { // collide with which object
			case OBJECT_GOAL:
				object->state = CHARACTER_GOAL;
				object->hide = 1;
				object->character_score++;
				break;

			case OBJECT_ARROW:
			case OBJECT_FIREBALL:
			case OBJECT_SPIKY_BALL:
				object->state = CHARACTER_DEAD;
				object->hide = 1;
				al_play_sample(sound_die, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
				break;

			case OBJECT_COIN:
				object->character_score++;
				al_play_sample(sound_coin, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
				break;

			default:
				break;
			}

			// fall into the ground
			if (object->y + object->height >= SCREENHEIGHT && object->hide == 0) {
				object->state = CHARACTER_DEAD;
				object->hide = 1;
				al_play_sample(sound_die, 0.8f, 0, 1.0f, ALLEGRO_PLAYMODE_ONCE, NULL);
			}
			break;

		case OBJECT_FIREBALL:
		case OBJECT_ARROW:
			// delete arrow outside the screen
			if (object->x<0 || object->x > SCREENWIDTH || object->y < 0 || object->y > SCREENWIDTH) {
				Object* arrow = object;
				object = object->next_object;
				delete_objectQueue(object_queue, arrow);
				continue;
			}
			break;

		case OBJECT_ARROW_LAUNCHER:
			// launch the arrow, add arrow object into linked list
			if (object->state == BOW_LAUNCH) {
				Object* arrow = (Object*)malloc(sizeof(Object));
				init_arrow(arrow, object->x + object->width / 2 - 4, object->y, UP);
				stack_objectQueue(object_queue, arrow);
				object->state = BOW_IDLE;
			}
			break;

		case OBJECT_FIREBALL_LAUNCHER:
			// launch the fireball, add arrow object into linked list
			if (object->state == FIREBALL_LAUNCHER_LAUNCH) {
				Object* fireball = (Object*)malloc(sizeof(Object));
				init_fireball(fireball, object->x + 8, object->y + 8);
				stack_objectQueue(object_queue, fireball);
				object->state = FIREBALL_LAUNCHER_IDLE;
			}
			break;

		default:
			break;
		}
		object = object->next_object;

	}
}

void win_scene(Map map, int state) {
	al_draw_bitmap(map.background, 0, 0, NULL);
	ALLEGRO_FONT* font = al_load_font("./resources/font/Prototype.ttf", 60, NULL);
	char text[20] = "";
	if (state == 0) { 
		snprintf(text, 20, "Tie");
		al_draw_text(font, al_map_rgb(255, 195, 60), SCREENWIDTH / 2, SCREENHEIGHT / 2, ALLEGRO_ALIGN_CENTRE, text);
	}
	else {
		snprintf(text, 20, "WINNER");
		al_draw_text(font, al_map_rgb(255, 195, 60), SCREENWIDTH / 2, SCREENHEIGHT / 2 - 100, ALLEGRO_ALIGN_CENTRE, text);
		if (state == 1) { snprintf(text, 20, "Player 1"); }
		else if (state == 2) { snprintf(text, 20, "Player 2"); }
		al_draw_text(font, al_map_rgb(255, 195, 60), SCREENWIDTH / 2, SCREENHEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTRE, text);
	}
	al_flip_display();
	al_rest(3);
	al_destroy_font(font);
}

void next_round_scene(int round, Map map, Object player1, Object player2, ObjectQueue* object_queue) { // enter next round
	al_draw_bitmap(map.background, 0, 0, NULL);
	ALLEGRO_FONT* font = al_load_font("./resources/font/pirulen.ttf", 60, NULL);
	ALLEGRO_FONT* font2 = al_load_font("./resources/font/Prototype.ttf", 40, NULL);
	char text[20] = "";
	snprintf(text, 20, "Round %d", round);
	al_draw_text(font, al_map_rgb(255, 195, 60), SCREENWIDTH / 2, SCREENHEIGHT / 2 - 20, ALLEGRO_ALIGN_CENTRE, text);
	snprintf(text, 20, "Player1  %d/%.0f", player1.character_score, WIN_SCORE);
	al_draw_text(font2, al_map_rgb(255, 255, 255), SCREENWIDTH / 2, SCREENHEIGHT / 2 + 100, ALLEGRO_ALIGN_CENTRE, text);
	snprintf(text, 20, "Player2  %d/%.0f", player2.character_score, WIN_SCORE);
	al_draw_text(font2, al_map_rgb(255, 255, 255), SCREENWIDTH / 2, SCREENHEIGHT / 2 + 150, ALLEGRO_ALIGN_CENTRE, text);
	al_flip_display();
	al_rest(3);
	
	// place the object by players

	ALLEGRO_EVENT event;
	cursor.type = CURSOR_NORMAL_MODE;

	for (int i = 0; i < 2; i++) { // 2 players

		Object object1, object2, object3, object4;
		random_object(&object1, 240, 40); 
		random_object(&object2, 740, 40);
		random_object(&object3, 240, 380);
		random_object(&object4, 740, 380);

		int choosed_object = 0;
		while (!choosed_object) {
			al_wait_for_event(queue, &event);

			if (event.type == ALLEGRO_EVENT_MOUSE_AXES) { // move mouse
				al_set_mouse_xy(display, min(max(0, event.mouse.x), SCREENWIDTH), min(max(0, event.mouse.y), SCREENHEIGHT));
				cursor.x = event.mouse.x;
				cursor.y = event.mouse.y;
			}
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				switch (cursor.type) {
				case CURSOR_NORMAL_MODE:
					cursor.img_index = 0;
					break;
				//case CURSOR_PLACE_MODE:
				//	break;
				}
			}
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				switch (cursor.type) {
				case CURSOR_NORMAL_MODE:
					cursor.img_index = 1;
					if (240 <= cursor.x && cursor.x <= 540 && 40 <= cursor.y && cursor.y <= 340) { choosed_object = object1.object_type;}
					else if (740 <= cursor.x && cursor.x <= 1040 && 40 <= cursor.y && cursor.y <= 340) { choosed_object = object2.object_type; }
					else if (240 <= cursor.x && cursor.x <= 540 && 380 <= cursor.y && cursor.y <= 680) { choosed_object = object3.object_type; }
					else if (740 <= cursor.x && cursor.x <= 1040 && 380 <= cursor.y && cursor.y <= 680) { choosed_object = object4.object_type; }
					break;
				//case CURSOR_PLACE_MODE:
				//	break;
				//case CURSOR_DELETE_MODE:
				//	break;

				}
			}
			if (event.type == ALLEGRO_EVENT_TIMER) { // refresh every 1/FPS seconds.
				if (event.timer.source == timer_fps) { // regular timer
					if (i == 0) { snprintf(text, 20, "Player1"); }
					else if (i == 1) { snprintf(text, 20, "Player2"); }
					al_draw_bitmap(map.background, 0, 0, NULL);
					draw_object(object1);
					draw_object(object2);
					draw_object(object3);
					draw_object(object4);
					al_draw_text(font2, al_map_rgb(255, 195, 60), SCREENWIDTH / 2, 10, ALLEGRO_ALIGN_CENTRE, text);
					draw_object(cursor);
					al_flip_display();
				}
			}
		}

		int isPlace = 0;
		cursor.cursor_place_object_type = choosed_object;
		
		if (choosed_object == OBJECT_BOMB) { cursor.type = CURSOR_DELETE_MODE; }
		else { cursor.type = CURSOR_PLACE_MODE; }
		while (!isPlace) {
			al_wait_for_event(queue, &event);
			if (event.type == ALLEGRO_EVENT_MOUSE_AXES) { // move mouse
				al_set_mouse_xy(display, min(max(0, event.mouse.x), SCREENWIDTH), min(max(0, event.mouse.y), SCREENHEIGHT));
				cursor.x = event.mouse.x;
				cursor.y = event.mouse.y;
			}
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
				switch (cursor.type) {
				case CURSOR_NORMAL_MODE:
					cursor.img_index = 0;
					break;
				case CURSOR_PLACE_MODE:
					cursor.img_index = cursor.cursor_place_object_type;
					break;
				case CURSOR_DELETE_MODE:
					cursor.img_index = 2;
					break;
				}
			}
			if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
				switch (cursor.type) {
				case CURSOR_NORMAL_MODE:
					break;
				case CURSOR_PLACE_MODE:
					place_object(object_queue, cursor);
					isPlace = 1;
					cursor.type = CURSOR_NORMAL_MODE;
					break;
				case CURSOR_DELETE_MODE:
					if (delete_object(object_queue, cursor)) {
						isPlace = 1;
						cursor.type = CURSOR_NORMAL_MODE;
					}
					break;

				}
			}
			if (event.type == ALLEGRO_EVENT_TIMER) { // refresh every 1/FPS seconds.
				if (event.timer.source == timer_fps) { // regular timer
					al_draw_bitmap(map.background, 0, 0, NULL);
					draw_all_object(*object_queue);
					draw_object(player1);
					draw_object(player2);
					draw_object(cursor);
					if (show_bounding_box) { // debug
						Object* object = game_objects.head;
						while (object != NULL) {
							al_draw_rectangle(object->x, object->y, object->x + object->width, object->y + object->height, al_map_rgb(255, 0, 0), 5);
							object = object->next_object;
						}
					}
					al_flip_display();
				}
			}
		}
	}
	al_destroy_font(font);
	al_destroy_font(font2);
	//al_destroy_event_queue(&event);

}

void reset_all_object(ObjectQueue object_queue, Map map) {
	Object* object = object_queue.head;
	while (object != NULL) {
		switch (object->object_type) {
		case OBJECT_CURSOR:
			cursor.type = CURSOR_NORMAL_MODE;
			object->img_index = 0;
			break;

		case OBJECT_CHARACTER:
			object->x = map.start_location[0];
			object->y = map.start_location[1];
			object->vx = 0;
			object->vy = 0;
			object->hide = 0;
			break;
		case OBJECT_COIN:
			object->hide = 0;
			break;


		case OBJECT_FIREBALL:
		case OBJECT_ARROW:
			if (true) {
			}
			Object* arrow = object;
			object = object->next_object;
			delete_objectQueue(&object_queue, arrow);
			continue;
			break;

		case OBJECT_ARROW_LAUNCHER:
			object->state = BOW_IDLE;
			object->img_index = 0;
			break;

		case OBJECT_FIREBALL_LAUNCHER:
			object->state = FIREBALL_LAUNCHER_IDLE;
			object->img_index = 0;
			break;

		case OBJECT_PLATFORM_SPIKY:
			object->state = PLATFORM_OFF;
			object->img_index = 0;
			break;

		default:
			break;
		}

		object = object->next_object;
	}
}

void random_object(Object* object, float x, float y) {
	int random_object_type = randi(OBJECT_PLATFORM_WOOD, OBJECT_BOMB);
	object->object_type = random_object_type;
	object->img_index = 0;
	object->draw_direction = RIGHT;
	object->hide = 0;
	object->x = x;
	object->y = y;
	object->width = 300;
	object->height = 300;

	switch (random_object_type) {
	case OBJECT_PLATFORM_WOOD:
		object->img[0] = al_load_bitmap("./resources/img/icon/platform_wood.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'platform_wood.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_PLATFORM_FROZEN:
		object->img[0] = al_load_bitmap("./resources/img/icon/platform_forzen.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'platform_forzen.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_PLATFORM_SPIKY:
		object->img[0] = al_load_bitmap("./resources/img/icon/platform_spiky.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'platform_spiky.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_ARROW_LAUNCHER:
		object->img[0] = al_load_bitmap("./resources/img/icon/arrow_launcher.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'arrow_launcher.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_FIREBALL_LAUNCHER:
		object->img[0] = al_load_bitmap("./resources/img/icon/fireball_launcher.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'fireball_launcher.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_SPIKY_BALL:
		object->img[0] = al_load_bitmap("./resources/img/icon/spiky_ball.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'spiky_ball.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_COIN:
		object->img[0] = al_load_bitmap("./resources/img/icon/coin.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'coin.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case OBJECT_BOMB:
		object->img[0] = al_load_bitmap("./resources/img/icon/bomb.png");
		if (!(object->img[0])) { al_show_native_message_box(NULL, "warning", "'bomb.png' file is missing!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	}
}