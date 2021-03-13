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

#include "../../modules/obstacle/include/obstacle.h"
#include "../../modules/tank/include/tank.h"
#include "../../modules/collision/include/collision.h"
#include "../../modules/drawer/include/drawer.h"
#include "../include/score.h"
 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	ALLEGRO_SAMPLE *shotSample = NULL;
	ALLEGRO_SAMPLE *shotHitBlockSample = NULL;
	ALLEGRO_SAMPLE *shotHitTankSample = NULL;
	ALLEGRO_SAMPLE *winSample = NULL;

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
	shotHitBlockSample = al_load_sample("./assets/sounds/hit_block.wav");
	shotHitTankSample = al_load_sample("./assets/sounds/hit_tank.wav");
	winSample = al_load_sample("./assets/sounds/win.wav");

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
   
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());	


	al_start_timer(timer);

	Tank tank_1;
	Tank tank_2;

	createTank(&tank_1, 0 + RADIUS_FORCE_FIELD, SCREEN_H / 2);
	createTank(&tank_2, SCREEN_W - RADIUS_FORCE_FIELD, SCREEN_H / 2);

	Obstacle o1;

	o1.upper_left.x = 100;
	o1.upper_left.y = 120;
	o1.lower_right.x = 500;
	o1.lower_right.y = 240;
	
	int playing = 1;
	int winner = 0;

	while(playing) {
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			if (gameWinner(tank_1, tank_2)) {
				int *score, winner = gameWinner(tank_1, tank_2);
				score = writeHistoryFile("./historico.txt", winner);
				al_play_sample(winSample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
				time_t start = time(NULL);
				while (time(NULL) - start <= 5) {
					drawScoreScreen(tank_1, tank_2, score, arcade_32);
					al_flip_display();
				}
				playing = 0;

			} else {
				drawScenario();
				drawObstacle(o1);

				drawPoints(tank_1.points, tank_1.color, arcade_32, 100, 30);
				drawPoints(tank_2.points, tank_2.color, arcade_32, SCREEN_W - 100, 30);

				updateTank(&tank_1);
				updateShot(&tank_1);

				updateTank(&tank_2);
				updateShot(&tank_2);

				collisionBetweenTanks(&tank_1, &tank_2);

				collisionTankObstacle(&tank_1, o1);
				collisionTankObstacle(&tank_2, o1);

				collisionShotObstacle(&tank_1, o1, shotHitBlockSample);
				collisionShotObstacle(&tank_2, o1, shotHitBlockSample);

				shotOutOfScreen(&tank_1);
				shotOutOfScreen(&tank_2);

				collisionTankScreen(&tank_1);
				collisionTankScreen(&tank_2);

				collisionTankShot(&tank_1, &tank_2, shotHitTankSample);

				drawTank(tank_1);
				drawShot(tank_1);

				drawTank(tank_2);
				drawShot(tank_2);

				al_flip_display();
			}
		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
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
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
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
	al_destroy_sample(shotHitBlockSample);
	al_destroy_sample(shotHitTankSample);
	al_destroy_sample(winSample);

	return 0;
}