#include "../includes/tank.h"

static ALLEGRO_COLOR generate_random_color() {
    return al_map_rgb(rand() % 255, rand() % 255, rand() % 255);
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
	t->points = 0;
}

static void Rotate(Point *P, float angular_speed) {
	float x = P->x, y = P->y;

	P->x = ((x * cos(angular_speed)) - (y * sin(angular_speed)));
	P->y = ((y * cos(angular_speed)) + (x * sin(angular_speed)));
}

static void rotateTank(Tank *t) {
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

void tankShot(Tank *t, ALLEGRO_SAMPLE *sound) {
    if (!t->is_shooting) {
        t->is_shooting = 1;
        t->x_shot_vec = t->x_vec;
        t->y_shot_vec = t->y_vec;
        t->shot.x += t->center.x;
        t->shot.y += t->center.y;

		al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
    }
}