#include "map.h"
// map stuff

void init_map(Map* map, int map_type) {
	switch (map_type) {
	case MAP_CITY:
		map->map_style = MAP_CITY;
		map->background = al_load_bitmap("./resources/img/background/city_1280x720.jpg");
		map->map_object_len = 5; // <------------------------------------------ important
		Object house1, house2, goal_city, platform1, platform2;
		house1.img[0] = al_load_bitmap("./resources/img/environment/house_1_192x320.png");
		house1.width = 192;
		house1.height = 320;
		house1.x = 0;
		house1.y = SCREENHEIGHT - house1.height;
		house1.object_type = OBJECT_ENVIRONMENT;
		house1.state = 0;
		house1.img_index = 0;
		house1.hide = 0;
		house1.draw_direction = RIGHT;
		map->map_object[0] = house1;
		house2.img[0] = al_load_bitmap("./resources/img/environment/house_2_192x400.png");
		house2.width = 192;
		house2.height = 400;
		house2.x = SCREENWIDTH - house2.width;
		house2.y = SCREENHEIGHT - house2.height;
		house2.object_type = OBJECT_ENVIRONMENT;
		house2.state = 0;
		house2.hide = 0;
		house2.img_index = 0;
		house2.draw_direction = RIGHT;
		map->map_object[1] = house2;
		map->start_location[0] = house1.width / 2;
		map->start_location[1] = house1.y -56;
		map->goal_location[0] = SCREENWIDTH - house2.width/2;
		map->goal_location[1] = house2.y - 64;

		init_goal(&goal_city, map->goal_location[0], map->goal_location[1]);
		map->map_object[2] = goal_city;

		init_platform(&platform1, 350, 350, OBJECT_PLATFORM_WOOD);
		init_platform(&platform2, 700, 300, OBJECT_PLATFORM_WOOD);
		map->map_object[3] = platform1;
		map->map_object[4] = platform2;

		if (!(map->background)) { al_show_native_message_box(NULL, "warning", "'city_1280x720.jpg' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(map->map_object[0].img[0])) { al_show_native_message_box(NULL, "warning", "'house_1_192x320.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(map->map_object[1].img[0])) { al_show_native_message_box(NULL, "warning", "'house_2_192x400.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		break;
	case MAP_SKY:
		map->map_style = MAP_SKY;
		map->background = al_load_bitmap("./resources/img/background/sky_1280x720.png");
		map->map_object_len = 5; // <------------------------------------------ important
		Object cloud1, cloud2, cloud3, platform_frozen_sky, goal_sky;
		cloud1.img[0] = al_load_bitmap("./resources/img/environment/cloud_192x32.png");
		cloud1.width = 192;
		cloud1.height = 32;
		cloud1.x = 0;
		cloud1.y = 300;
		cloud1.object_type = OBJECT_ENVIRONMENT;
		cloud1.state = 0;
		cloud1.img_index = 0;
		cloud1.hide = 0;
		cloud1.draw_direction = RIGHT;
		map->map_object[0] = cloud1;
		cloud2.img[0] = al_load_bitmap("./resources/img/environment/cloud_192x32.png");
		cloud2.width = 192;
		cloud2.height = 32;
		cloud2.x = SCREENWIDTH - cloud2.width;
		cloud2.y = 450;
		cloud2.object_type = OBJECT_ENVIRONMENT;
		cloud2.state = 0;
		cloud2.hide = 0;
		cloud2.img_index = 0;
		cloud2.draw_direction = RIGHT;
		map->map_object[1] = cloud2;
		map->start_location[0] = cloud1.width / 2;
		map->start_location[1] = cloud1.y -56;
		map->goal_location[0] = SCREENWIDTH - cloud2.width / 2;
		map->goal_location[1] = cloud2.y - 64;
		cloud3.img[0] = al_load_bitmap("./resources/img/environment/cloud_192x32.png");
		cloud3.width = 192;
		cloud3.height = 32;
		cloud3.x = 400;
		cloud3.y = 400;
		cloud3.object_type = OBJECT_ENVIRONMENT;
		cloud3.state = 0;
		cloud3.hide = 0;
		cloud3.img_index = 0;
		cloud3.draw_direction = RIGHT;
		map->map_object[2] = cloud3;

		
		init_goal(&goal_sky, map->goal_location[0], map->goal_location[1]);
		map->map_object[3] = goal_sky;

		init_platform(&platform_frozen_sky, 800, 450, OBJECT_PLATFORM_FROZEN);
		map->map_object[4] = platform_frozen_sky;



		if (!(map->background)) { al_show_native_message_box(NULL, "warning", "'sky_1280x720.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(map->map_object[0].img[0])) { al_show_native_message_box(NULL, "warning", "'cloud_192x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(map->map_object[1].img[0])) { al_show_native_message_box(NULL, "warning", "'cloud_192x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }
		if (!(map->map_object[2].img[0])) { al_show_native_message_box(NULL, "warning", "'cloud_192x32.png' is not found!", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR); }

		break;


	
	}
}
