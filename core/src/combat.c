#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "../../modules/includes/obstacle.h"
#include "../../modules/includes/tank.h"
#include "../../modules/includes/collision.h"
#include "../../modules/includes/drawer.h"

#include "../../screens/includes/score.h"
#include "../../screens/includes/game.h"
 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	ALLEGRO_SAMPLE *shotSample = NULL;
	ALLEGRO_SAMPLE *shot_hit_block_sample = NULL;
	ALLEGRO_SAMPLE *shot_hit_tank_sample = NULL;
	ALLEGRO_SAMPLE *win_sample = NULL;

	int screen = GAME_SCREEN;

	srand(time(NULL));
   
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	if(!al_install_audio()) {
		fprintf(stderr, "failed to install audio!\n");
		return -1;
	}

	if(!al_init_acodec_addon()) {
		fprintf(stderr, "failed to init acodec addon!\n");
		return -1;
	}

	al_init_font_addon();

	al_reserve_samples(10);

	shotSample = al_load_sample("./assets/sounds/shot.wav");
	shot_hit_block_sample = al_load_sample("./assets/sounds/hit_block.wav");
	shot_hit_tank_sample = al_load_sample("./assets/sounds/hit_tank.wav");
	win_sample = al_load_sample("./assets/sounds/win.wav");

	al_set_window_position(display, 100, 60);

	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
    ALLEGRO_FONT *arcade_32 = al_load_font("./assets/fonts/8-BIT WONDER.ttf", 32, 1);   
	if(arcade_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	int SCREEN_X, SCREEN_Y;
	al_get_window_position(display, &SCREEN_X, &SCREEN_Y);
	al_set_window_position(display, SCREEN_X, SCREEN_Y);
   
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());	


	al_start_timer(timer);

	Tank tank_1;
	Tank tank_2;

	createTank(&tank_1, TANK_1_INIT_POS_X, TANK_1_INIT_POS_Y);
	createTank(&tank_2, TANK_2_INIT_POS_X, TANK_2_INIT_POS_Y);

	Obstacle obstacles[2 * NUMBER_OF_OBSTACLES];

	createObstacles(obstacles);

	int playing = 1;
	int winner = 0;

	while(playing) {
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			gameWinner(tank_1, tank_2, &winner, &screen);
			switch (screen) {
				case GAME_SCREEN: {
					renderGame(
						&tank_1,
						&tank_2,
						obstacles,
						arcade_32,
						shot_hit_block_sample,
						shot_hit_tank_sample
					);
					break;
				}
				case SCORE_SCREEN: {
					renderScore(
						tank_1,
						tank_2,
						winner,
						win_sample,
						arcade_32,
						event_queue
					);
					restartGame(&tank_1, &tank_2, obstacles, &winner, &screen);
					break;
				}
				default:
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN && screen == GAME_SCREEN) {
			switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_W:
					tank_1.speed -= TANK_SPEED;
					break;
				case ALLEGRO_KEY_S:
					tank_1.speed += TANK_SPEED;
					break;
				case ALLEGRO_KEY_D:
					tank_1.angular_speed += TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_A:
					tank_1.angular_speed -= TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_UP:
					tank_2.speed -= TANK_SPEED;
					break;
				case ALLEGRO_KEY_DOWN:
					tank_2.speed += TANK_SPEED;
					break;
				case ALLEGRO_KEY_LEFT:
					tank_2.angular_speed -= TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_RIGHT:
					tank_2.angular_speed += TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_Q:
					tankShot(&tank_1, shotSample);
					break;
				case ALLEGRO_KEY_ENTER:
					tankShot(&tank_2, shotSample);
					break;
				default:
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP && screen == GAME_SCREEN) {
			switch (ev.keyboard.keycode) {
				case ALLEGRO_KEY_W:
					tank_1.speed += TANK_SPEED;
					break;
				case ALLEGRO_KEY_S:
					tank_1.speed -= TANK_SPEED;
					break;
				case ALLEGRO_KEY_D:
					tank_1.angular_speed -= TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_A:
					tank_1.angular_speed += TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_UP:
					tank_2.speed += TANK_SPEED;
					break;
				case ALLEGRO_KEY_DOWN:
					tank_2.speed -= TANK_SPEED;
					break;
				case ALLEGRO_KEY_LEFT:
					tank_2.angular_speed += TANK_ANGULAR_SPEED;
					break;
				case ALLEGRO_KEY_RIGHT:
					tank_2.angular_speed -= TANK_ANGULAR_SPEED;
					break;
				default:
					break;
			}
		}

	}
     
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_sample(shotSample);
	al_destroy_sample(shot_hit_block_sample);
	al_destroy_sample(shot_hit_tank_sample);
	al_destroy_sample(win_sample);

	return 0;
}