// Made by Jamie Chang 2021/06/10-2021/06/27 ver 0.1
#include "global.h"
#include "object.h"
#include "collision.h"
#include "mode_home.h"
#include "mode_single_player.h"
#include "mode_two_players.h"
#include "mode_debug.h"


char TITLE[100];
int MODE = MODE_HOME;

//variables

void game_initiatize() {
	// set random seed
	srand(time(0));
	//windows title
	strcat_s(TITLE, sizeof(TITLE), GAME_NAME);
	strcat_s(TITLE, sizeof(TITLE), " ");
	strcat_s(TITLE, sizeof(TITLE), VERSION);

	//init
	al_init();
	al_init_primitives_addon();
	al_init_native_dialog_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_install_keyboard();
	al_install_mouse();

	//display
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);
	display = al_create_display(SCREENWIDTH, SCREENHEIGHT);
	al_set_window_title(display, &TITLE);
	al_set_window_position(display, 0, 0);


	//timer // remove
	timer_fps = al_create_timer(1.0 / FPS); //global
	
	//cursor
	al_hide_mouse_cursor(display);

	//sound
	al_reserve_samples(3);
	sound_jump = al_load_sample("./resources/sound/jump.wav");
	sound_die = al_load_sample("./resources/sound/die.wav");
	sound_coin = al_load_sample("./resources/sound/coin.wav");
}

void game_start() {
	al_start_timer(timer_fps);

	bool running = true;
	printf("Running the game...\n");

	while (running) {

		switch (MODE) {
		case MODE_EXIT:
			running = false;
			break;
		case MODE_HOME:
			MODE_HOME_init();
			MODE = MODE_HOME_run();
			MODE_HOME_destroy();
			break;
		case MODE_SINGLE_PLAYER:
			MODE_SINGLE_PLAYER_init();
			MODE = MODE_SINGLE_PLAYER_run();
			MODE_SINGLE_PLAYER_destroy();
			break;
		case MODE_TWO_PLAYERS:
			MODE_TWO_PLAYERS_init();
			MODE = MODE_TWO_PLAYERS_run();
			MODE_TWO_PLAYERS_destroy();
			break;
		case MODE_DEBUG:
			MODE_DEBUG_init();
			MODE = MODE_DEBUG_run();
			MODE_DEBUG_destroy();
			break;
		}
		
	}
}

void game_destroy() {
	al_destroy_display(display);
	al_destroy_timer(timer_fps);
	al_destroy_sample(sound_jump);
	al_destroy_sample(sound_die);
	al_destroy_sample(sound_coin);
	
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_audio();
}



int main()
{

	game_initiatize(); printf("Initializing the game...\n");
	game_start();
	game_destroy(); printf("Game stopped\n");
	

	return 0;
}




/*

1.
2.
3.

problem
mode_home.c 裡面的 Object* buttons[3]，陣列裡要存指針才能在更改img_index時連動影響ObjectQueue裡的按鈕物件
要改character jump 部分的動畫 在 update func (jump 部分圖片繪製有問題)

每次切回主畫面RAM都會增加
JUMP圖片要重畫
部分函式的參數ObjectQueue*可以替換成ObjectQueue
timer_launch_arror 改成 timer_launch
*/