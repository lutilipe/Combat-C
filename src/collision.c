#include "collision.h"

static void resetShotPosition(Tank *t) {
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

static int collisionShotTank(Point c1, Point c2) {
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