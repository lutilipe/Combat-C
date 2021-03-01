#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const float FPS = 60;  

const int SCREEN_W = 960;
const int SCREEN_H = 540;

const float THETA = M_PI/4;
const float RADIUS_FORCE_FIELD = 30;
const float RADIUS_SHOT = 6.0;

const float TANK_SPEED = 2.5;
const float TANK_SHOT_SPEED = 7.5;
const float TANK_ANGULAR_SPEED = M_PI/90;

typedef struct Point {
	float x;
	float y;
} Point;


typedef struct Tank {
	Point center;
	Point A, B, C;
	Point shot;

	ALLEGRO_COLOR color;

	float speed;
	float angular_speed;
	float shot_speed;
	float angle;
	float x_vec, y_vec, x_shot_vec, y_shot_vec, a, b;
} Tank;

ALLEGRO_COLOR generate_random_color() {
    return al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
}

void createTank(Tank *t, float startX, float startY) {
	t->center.x = startX;
	t->center.y = startY;
	t->color = generate_random_color();

	t->A.x = 0;
	t->A.y = -RADIUS_FORCE_FIELD;

	t->shot.x = 0;
	t->shot.y = -RADIUS_FORCE_FIELD;

	float alpha = M_PI / 2 - THETA;
	float h = RADIUS_FORCE_FIELD * sin(alpha);
	float w = RADIUS_FORCE_FIELD * sin(THETA);

	t->B.x = -w;
	t->B.y = h;

	t->C.x = w;
	t->C.y = h;

	t->a = 0;
	t->b = 0;

	t->shot_speed = -TANK_SHOT_SPEED;
	t->speed = 0;
	t->angular_speed = 0;

	t->angle = M_PI/2;
	t->x_vec = cos(t->angle);
	t->y_vec = sin(t->angle);

	t->x_shot_vec = cos(t->angle);
	t->y_shot_vec = sin(t->angle);
}

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

void drawShot(Tank t, int tank_shot) {
	float x = tank_shot
		? t.shot.x + t.a
		: t.A.x + t.center.x;

	float y = tank_shot
		? t.shot.y + t.b
		: t.A.y + t.center.y;

	al_draw_filled_circle(
		x, 
		y, 
		RADIUS_SHOT, 
		t.color
	);
}

void Rotate(Point *P, float angular_speed) {
	float x = P->x, y = P->y;

	P->x = ((x * cos(angular_speed)) - (y * sin(angular_speed)));
	P->y = ((y * cos(angular_speed)) + (x * sin(angular_speed)));
}

void rotateTank(Tank *t, int tank_shot) {
	if (t->angular_speed != 0) {
		Rotate(&t->A, t->angular_speed);
		Rotate(&t->B, t->angular_speed);
		Rotate(&t->C, t->angular_speed);
		if (!tank_shot) {
			Rotate(&t->shot, t->angular_speed);
		}

		t->angle += t->angular_speed;
		t->x_vec = cos(t->angle);
		t->y_vec = sin(t->angle);
	}
}

void updateTank(Tank *t, int tank_shot) {
	rotateTank(t, tank_shot);

	t->center.y += t->speed * t->y_vec;
	t->center.x += t->speed * t->x_vec;
}

void tankShot(Tank *t, int tank_shot) {
	if (tank_shot) {
		t->shot.y += t->shot_speed * t->y_shot_vec;
		t->shot.x += t->shot_speed * t->x_shot_vec;
	}
}

void shotOutOfScreen(Tank *t, int *tank_shot) {
	if (
		t->shot.x > SCREEN_W + RADIUS_SHOT
		|| t->shot.y > SCREEN_H + RADIUS_SHOT
		|| t->shot.x < -SCREEN_W - RADIUS_SHOT
		|| t->shot.y < -SCREEN_H - RADIUS_SHOT
	) {
		*tank_shot = 0;
		t->shot.x = t->A.x;
		t->shot.y = t->A.y;
	}
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
	
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
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
	int tank1_shot = 0;
	int tank2_shot = 0;

	while(playing) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if(ev.type == ALLEGRO_EVENT_TIMER) {
			drawScenario();

			updateTank(&tank_1, tank1_shot);
			updateTank(&tank_2, tank2_shot);

			tankShot(&tank_1, tank1_shot);
			tankShot(&tank_2, tank2_shot);

			shotOutOfScreen(&tank_1, &tank1_shot);
			shotOutOfScreen(&tank_2, &tank2_shot);

			drawTank(tank_1);
			drawTank(tank_2);

			drawShot(tank_1, tank1_shot);
			drawShot(tank_2, tank2_shot);

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
					if (!tank1_shot) {
						tank1_shot = 1;
						tank_1.x_shot_vec = tank_1.x_vec;
						tank_1.y_shot_vec = tank_1.y_vec;
						tank_1.a = tank_1.center.x;
						tank_1.b = tank_1.center.y;
					}
					break;
				case ALLEGRO_KEY_ENTER:
					if (!tank2_shot) {
						tank2_shot = 1;
						tank_2.x_shot_vec = tank_2.x_vec;
						tank_2.y_shot_vec = tank_2.y_vec;
						tank_2.a = tank_2.center.x;
						tank_2.b = tank_2.center.y;
					}
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