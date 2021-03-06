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
const float TANK_SHOT_SPEED = 10;
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
	float x_vec, y_vec, x_shot_vec, y_shot_vec;

	int is_shooting;
	int life;
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

	t->shot_speed = -TANK_SHOT_SPEED;
	t->speed = 0;
	t->angular_speed = 0;

	t->angle = M_PI/2;
	t->x_vec = cos(t->angle);
	t->y_vec = sin(t->angle);

	t->x_shot_vec = cos(t->angle);
	t->y_shot_vec = sin(t->angle);

	t->is_shooting = 0;
	t->life = 0;
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

void Rotate(Point *P, float angular_speed) {
	float x = P->x, y = P->y;

	P->x = ((x * cos(angular_speed)) - (y * sin(angular_speed)));
	P->y = ((y * cos(angular_speed)) + (x * sin(angular_speed)));
}

void rotateTank(Tank *t) {
	if (t->angular_speed != 0) {
		Rotate(&t->A, t->angular_speed);
		Rotate(&t->B, t->angular_speed);
		Rotate(&t->C, t->angular_speed);

		t->angle += t->angular_speed;
		t->x_vec = cos(t->angle);
		t->y_vec = sin(t->angle);
	}
}

void updateTank(Tank *t) {
	rotateTank(t);

	t->center.y += t->speed * t->y_vec;
	t->center.x += t->speed * t->x_vec;
}

void updateShot(Tank *t) {
	if (t->is_shooting) {
		t->shot.y += t->shot_speed * t->y_shot_vec;
		t->shot.x += t->shot_speed * t->x_shot_vec;
	} else {
		Rotate(&t->shot, t->angular_speed);
	}
}

void resetShotPosition(Tank *t) {
	t->is_shooting = 0;
	t->shot.x = t->A.x;
	t->shot.y = t->A.y;
}

void shotOutOfScreen(Tank *t) {
	if (
		t->shot.x >= SCREEN_W + RADIUS_SHOT
		|| t->shot.y >= SCREEN_H + RADIUS_SHOT
		|| t->shot.x <= 0 - RADIUS_SHOT
		|| t->shot.y <= 0 - RADIUS_SHOT
	) {
		resetShotPosition(t);
	}
}

void collisionTankScreen(Tank *t) {
	if (t->center.x >= SCREEN_W - RADIUS_FORCE_FIELD) {
		t->center.x = SCREEN_W - RADIUS_FORCE_FIELD;
	} else if (t->center.x <= 0 + RADIUS_FORCE_FIELD) {
		t->center.x = 0 + RADIUS_FORCE_FIELD;
	} 
	
	if (t->center.y <= 0 + RADIUS_FORCE_FIELD) {
		t->center.y = 0 + RADIUS_FORCE_FIELD;
	} else if (t->center.y >= SCREEN_H - RADIUS_FORCE_FIELD) {
		t->center.y = SCREEN_H - RADIUS_FORCE_FIELD;
	} 
}

void collisionBetweenTanks(Tank *t1, Tank *t2) {
	float distance_x = t1->center.x - t2->center.x;
	float distance_y = t1->center.y - t2->center.y;
	float sum_radius = RADIUS_FORCE_FIELD * 2;
	float distanceBetweenCircles = sqrt(distance_x * distance_x + distance_y * distance_y);

	if (distanceBetweenCircles <= sum_radius) {
		float distanceToMove = sum_radius - distanceBetweenCircles;

		if (t1->speed < 0) {
			t1->center.x += distanceToMove * t1->x_vec;
			t1->center.y += distanceToMove * t1->y_vec;
		} else if (t1->speed > 0) {
			t1->center.x -= distanceToMove * t1->x_vec;
			t1->center.y -= distanceToMove * t1->y_vec;
		}

		if (t2->speed < 0) {
			t2->center.x += distanceToMove * t2->x_vec;
			t2->center.y += distanceToMove * t2->y_vec;
		} else if (t2->speed > 0){
			t2->center.x -= distanceToMove * t2->x_vec;
			t2->center.y -= distanceToMove * t2->y_vec;
		}
	}
}

int collisionShotTank(Point c1, Point c2) {
	float distance_x = c1.x - c2.x;
	float distance_y = c1.y - c2.y;
	float sum_radius = RADIUS_FORCE_FIELD + RADIUS_SHOT;
	float distanceBetweenCircles = sqrt(distance_x * distance_x + distance_y * distance_y);

	if (distanceBetweenCircles <= sum_radius) return 1;
	return 0;
}

void collisionTankShot(Tank *t1, Tank *t2) {
	int shot_collide = 0;
	if (t1->is_shooting) {

		shot_collide = collisionShotTank(t1->shot, t2->center);
		if (shot_collide) {
			t2->life -= 1;
			resetShotPosition(t1);
		}
	} 
	
	if (t2->is_shooting) {
		shot_collide = collisionShotTank(t2->shot, t1->center);
		if (shot_collide) {
			t1->life -= 1;
			resetShotPosition(t2);
		}
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
					if (!tank_1.is_shooting) {
						tank_1.is_shooting = 1;
						tank_1.x_shot_vec = tank_1.x_vec;
						tank_1.y_shot_vec = tank_1.y_vec;
						tank_1.shot.x += tank_1.center.x;
						tank_1.shot.y += tank_1.center.y;
					}
					break;
				case ALLEGRO_KEY_ENTER:
					if (!tank_2.is_shooting) {
						tank_2.is_shooting = 1;
						tank_2.x_shot_vec = tank_2.x_vec;
						tank_2.y_shot_vec = tank_2.y_vec;
						tank_2.shot.x += tank_2.center.x;
						tank_2.shot.y += tank_2.center.y;
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