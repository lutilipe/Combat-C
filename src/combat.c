#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <time.h>
#include "tank.h"
#include "collision.h"

void drawScenario() {
	al_clear_to_color(al_map_rgb(255, 255, 255));
}

void drawTank(Tank t) {
	al_draw_circle(t.center.x, t.center.y, RADIUS_FORCE_FIELD, t.color, 1);

	al_draw_filled_triangle(
		t.A.x + t.center.x, 
		t.A.y + t.center.y, 
		t.B.x + t.center.x,
		t.B.y + t.center.y,
		t.C.x + t.center.x,
		t.C.y + t.center.y,
		t.color
	);
}

void drawShot(Tank t) {
	float x = t.is_shooting
		? t.shot.x
		: t.shot.x + t.center.x;

	float y = t.is_shooting
		? t.shot.y
		: t.shot.y + t.center.y;

	al_draw_filled_circle(
		x, 
		y, 
		RADIUS_SHOT, 
		t.color
	);
}
 
int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

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

	al_init_font_addon();

	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
    ALLEGRO_FONT *size_32 = al_load_font("./assets/fonts/arial.ttf", 32, 1);   
	if(size_32 == NULL) {
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
	
	int playing = 1;

	while(playing) {
		ALLEGRO_EVENT ev;

		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			drawScenario();

			updateTank(&tank_1);
			updateShot(&tank_1);

			updateTank(&tank_2);
			updateShot(&tank_2);

			collisionBetweenTanks(&tank_1, &tank_2);

			shotOutOfScreen(&tank_1);
			shotOutOfScreen(&tank_2);

			collisionTankScreen(&tank_1);
			collisionTankScreen(&tank_2);

			collisionTankShot(&tank_1, &tank_2);

			drawTank(tank_1);
			drawShot(tank_1);

			drawTank(tank_2);
			drawShot(tank_2);

			al_flip_display();
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
					tankShot(&tank_1);
					break;
				case ALLEGRO_KEY_ENTER:
					tankShot(&tank_2);
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
   
 
	return 0;
}