#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "../../core/includes/config.h"

#ifndef TANK_H
#define TANK_H

#define TANK_1_INIT_POS_X RADIUS_FORCE_FIELD
#define TANK_1_INIT_POS_Y SCREEN_H / 2

#define TANK_2_INIT_POS_X SCREEN_W - RADIUS_FORCE_FIELD
#define TANK_2_INIT_POS_Y SCREEN_H / 2

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
	int points;
} Tank;

void createTank(Tank *t, float startX, float startY);
void rotateTank(Tank *t);
void updateShot(Tank *t);
void updateTank(Tank *t);
void tankShot(Tank *t, ALLEGRO_SAMPLE *sound);

#endif
