#include "mode_home.h"
// home menu

// local variables
static ALLEGRO_EVENT_QUEUE* queue = NULL;
static ALLEGRO_KEYBOARD_STATE keyboard_state;

static ALLEGRO_FONT* font_BigSpace = NULL;


static ObjectQueue home_menu_objects;
static Object cursor;

static Object button_single_player;
static Object button_two_players;
static Object button_exit;


void MODE_HOME_init() {
	
	//fonts
	font_BigSpace = al_load_font("./resources/font/BigSpace.ttf", 60, NULL);
	if (!font_BigSpace) { al_show_native_message_box(display, "Warning", "'BigSpace.ttf' not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }

	//cursor
	init_cursor(&cursor);

	//button
	init_button(&button_single_player, BUTTON_SINGLE_PLAYER);
	init_button(&button_two_players, BUTTON_TWO_PLAYERS);
	init_button(&button_exit, BUTTON_EXIT);


	//load ObjectQueue
	init_objectQueue(&home_menu_objects);
	stack_objectQueue(&home_menu_objects, &button_single_player);
	stack_objectQueue(&home_menu_objects, &button_two_players);
	stack_objectQueue(&home_menu_objects, &button_exit);
	stack_objectQueue(&home_menu_objects, &cursor);


	//event_queue
	queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer_fps));
}

int MODE_HOME_run() {
	//start timer

	bool running = true;
	printf("Running (MODE_MENU)...\n");

	Object* buttons[3] = { &button_single_player, &button_two_players, &button_exit }; // store pointer so that we can change properties of buttons
	ALLEGRO_BITMAP* bg = al_load_bitmap("./resources/img/background/home_1280x720.jpg");

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
			al_set_mouse_xy(display, min(max(0,event.mouse.x),SCREENWIDTH), min(max(0, event.mouse.y), SCREENHEIGHT));
			
			cursor.x = event.mouse.x;
			cursor.y = event.mouse.y;

			for (int i = 0; i < 3; i++) {
				int isCollide = bounding_box_collision_detection(cursor, *buttons[i]);
				if (isCollide == COLLISION_INSIDE) {
					buttons[i]->img_index = 1;
				}
				else {
					buttons[i]->img_index = 0;
				}
			}
		}

		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			cursor.img_index = 0;
		}
		if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) { // check
			cursor.img_index = 1;

			for (int i = 0; i < 3; i++) {
				int isCollide = bounding_box_collision_detection(cursor, *buttons[i]);
				if (isCollide == COLLISION_INSIDE) {
					switch (buttons[i]->type) {
					case BUTTON_HOME:
						return MODE_HOME;
						break;
					case BUTTON_SINGLE_PLAYER:
						return MODE_SINGLE_PLAYER;
						break;
					case BUTTON_TWO_PLAYERS:
						return MODE_TWO_PLAYERS;
						break;
					case BUTTON_EXIT:
						return MODE_EXIT;
						break;
					}
				}
			}
		
		}

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (al_key_down(&keyboard_state, ALLEGRO_KEY_M)) { //debug
				return MODE_DEBUG;
			}
		}


		if (event.type == ALLEGRO_EVENT_TIMER) { // refresh every 1/FPS seconds.

			if (event.timer.source == timer_fps) {
				//background
				//al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_bitmap(bg, 0, 0, NULL);

				//draw buttons

				draw_all_object(home_menu_objects);

				//draw text
				al_draw_text(font_BigSpace, al_map_rgb(255, 255, 255), button_single_player.x+button_single_player.width/2, button_single_player.y + button_single_player.height / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Single Player");
				al_draw_text(font_BigSpace, al_map_rgb(255, 255, 255), button_two_players.x + button_two_players.width / 2, button_two_players.y + button_two_players.height / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Two Players");
				al_draw_text(font_BigSpace, al_map_rgb(255, 255, 255), button_exit.x + button_exit.width / 2, button_exit.y + button_exit.height / 2 - 30, ALLEGRO_ALIGN_CENTRE, "Exit");
				al_draw_text(font_BigSpace, al_map_rgb(255, 255, 255), 300, 300, ALLEGRO_ALIGN_CENTRE, "Two Sides");

				draw_object(cursor);
				
				al_flip_display();
			}
		}
	}
	al_destroy_bitmap(bg);
	al_destroy_event_queue(&event);
	

} 

void MODE_HOME_destroy() {
	destroy_all_object(&home_menu_objects);
	al_destroy_font(font_BigSpace);
}