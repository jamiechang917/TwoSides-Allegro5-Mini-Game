#include "mode_debug.h"
#include "map.h"

// debug mode

//local variables
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_KEYBOARD_STATE keyboard_state;

//fonts
static ALLEGRO_FONT* font_Neutrons = NULL;

static ObjectQueue game_objects;
static Object cursor;

//temp
static Object player;

//map
static Map map_city;

static ALLEGRO_TIMER* timer_animation = NULL;
static ALLEGRO_TIMER* timer_character_move = NULL;

//debug
static int show_bounding_box = 0;

static void place_object(ObjectQueue*, Object);
static void delete_object(ObjectQueue*, Object);
static void next_round_scene(int, Map);
static void check_character_event(Object*);

void MODE_DEBUG_init() {
	//fonts
	font_Neutrons = al_load_font("./resources/font/Neutrons.ttf", 30, NULL);

	//cursor
	init_cursor(&cursor);

	//map 
	init_map(&map_city, MAP_CITY);

	//load ObjectQueue
	init_objectQueue(&game_objects);
	// temp
	init_character(&player, CHARACTER_01, NULL, map_city.start_location[0], map_city.start_location[1]);
	stack_objectQueue(&game_objects, &cursor);
	stack_objectQueue(&game_objects, &player);

	for (int i = 0; i < map_city.map_object_len; i++) {
		stack_objectQueue(&game_objects, &map_city.map_object[i]);
	}

	//timer
	timer_animation = al_create_timer(1.0 / FPS_ANIMATION);
	timer_character_move = al_create_timer(1.0 / FPS_CHARACTER_MOVE);

	//event queue
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer_fps));
	al_register_event_source(queue, al_get_timer_event_source(timer_animation));
	al_register_event_source(queue, al_get_timer_event_source(timer_character_move));

}
int MODE_DEBUG_run() {
	//start timer
	al_start_timer(timer_animation);
	al_start_timer(timer_character_move);

	bool running = true;
	printf("Running (MODE_DEBUG)...\n");

	ALLEGRO_EVENT event;
	ALLEGRO_KEYBOARD_STATE keyboard_state;

	while (running) {
		al_wait_for_event(queue, &event);
		al_get_keyboard_state(&keyboard_state);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // exit by closing the window
			running = false;
			return MODE_EXIT;
		}

		// cursor
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
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
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

		if (al_key_down(&keyboard_state, ALLEGRO_KEY_H)) {
			running = false;
			return MODE_HOME;
		}


		// keyboard
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (al_key_down(&keyboard_state, ALLEGRO_KEY_M)) { //debug
				show_bounding_box = 1 - show_bounding_box;
			}
			if (al_key_down(&keyboard_state, ALLEGRO_KEY_R)) {
				destroy_object(&player);
				init_character(&player, CHARACTER_01, NULL, map_city.start_location[0], map_city.start_location[1]);
			}

			if (al_key_down(&keyboard_state, ALLEGRO_KEY_P)) {
				if (cursor.type != CURSOR_PLACE_MODE) {
					cursor.type = CURSOR_PLACE_MODE;
					cursor.cursor_place_object_type = OBJECT_PLATFORM_WOOD;
					cursor.img_index = cursor.cursor_place_object_type;
				}
				else { // back to normal mode
					if (cursor.cursor_place_object_type < OBJECT_PLATFORM_FROZEN) {
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

			if (al_key_down(&keyboard_state, ALLEGRO_KEY_O)) {
				al_stop_timer(timer_animation);
				al_stop_timer(timer_character_move);
				next_round_scene(1, map_city);
				al_start_timer(timer_animation);
				al_start_timer(timer_character_move);
			}
			
			
		}



		if (event.type == ALLEGRO_EVENT_TIMER) { // refresh every 1/FPS seconds.

			if (event.timer.source == timer_fps) { // regular timer
				//background 
				al_draw_bitmap(map_city.background, 0, 0, NULL);

				draw_all_object(game_objects);
				draw_object(player);
				draw_object(cursor);

				if (show_bounding_box) {
					Object* object = game_objects.head;
					while (object != NULL) {
						al_draw_rectangle(object->x, object->y, object->x + object->width, object->y + object->height, al_map_rgb(255, 0, 0), 5);
						object = object->next_object;
					}
				}

				al_draw_text(font_Neutrons, al_map_rgb(255, 0, 0), SCREENWIDTH/2, 10, ALLEGRO_ALIGN_CENTRE, "Debug Mode");
				al_flip_display();
			}

			else if (event.timer.source == timer_character_move) {
				move_all_object(&game_objects);
				check_collsion_gamemap(game_objects);
				check_character_event(&player);
			}

			else if (event.timer.source == timer_animation) {
				update_all_animation(&game_objects);
			}
		}
	}
	al_destroy_event_queue(&event);

}
void MODE_DEBUG_destroy() {
	//stop timer
	al_stop_timer(timer_animation);
	al_stop_timer(timer_character_move);
	//destroy timer
	al_destroy_timer(timer_animation);
	al_destroy_timer(timer_character_move);

	//destroy bitmap
	destroy_all_object(&game_objects);

	//destroy fonts
	al_destroy_font(font_Neutrons);

}


// =========================================================

// local functions

void place_object(ObjectQueue* object_queue, Object cursor) {
	Object* object = malloc(sizeof(Object));
	switch (cursor.cursor_place_object_type) {
		case OBJECT_PLATFORM_WOOD:
			init_platform(object, cursor.x, cursor.y, OBJECT_PLATFORM_WOOD);
			break;
		case OBJECT_PLATFORM_FROZEN:
			init_platform(object, cursor.x, cursor.y, OBJECT_PLATFORM_FROZEN);
			break;
	}
	stack_objectQueue(object_queue, object);
}

void delete_object(ObjectQueue* object_queue, Object cursor) {
	Object* object = object_queue->head;
	
	while (object != NULL) {
		if (object->object_type != OBJECT_CURSOR && 
			object->object_type != OBJECT_BUTTON && 
			object->object_type != OBJECT_CHARACTER &&
			object->object_type != OBJECT_GOAL &&
			object->object_type != OBJECT_ENVIRONMENT) {
			int isCollide = bounding_box_collision_detection(cursor, *object);
			if (isCollide == COLLISION_INSIDE) {
				delete_objectQueue(object_queue, object); // automatic destroy the bitmap
			}
		}
		object = object->next_object;
	}
}

void check_character_event(Object* character) { // dead, goal, or other event
	switch (character->collision_object_type)
	{
		case OBJECT_GOAL:
			character->state = CHARACTER_GOAL;
			character->hide = 1;
			break;

		default:
			break;
	}

}

void next_round_scene(int round, Map map) {
	al_draw_bitmap(map.background, 0, 0, NULL);
	ALLEGRO_FONT*  font = al_load_font("./resources/font/pirulen.ttf", 60, NULL);
	char text[20] = "";
	snprintf(text, 20, "Round %d", round);
	al_draw_text(font, al_map_rgb(255, 255, 255), SCREENWIDTH / 2, SCREENHEIGHT/2, ALLEGRO_ALIGN_CENTRE, text);
	al_flip_display();
	al_rest(3);
	al_destroy_font(font);
}