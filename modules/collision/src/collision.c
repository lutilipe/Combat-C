#include "../include/collision.h"

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
		} 
		
		if (t1->speed > 0) {
			t1->center.x -= distanceToMove * t1->x_vec;
			t1->center.y -= distanceToMove * t1->y_vec;
		}

		if (t2->speed < 0) {
			t2->center.x += distanceToMove * t2->x_vec;
			t2->center.y += distanceToMove * t2->y_vec;
		} 
		
		if (t2->speed > 0){
			t2->center.x -= distanceToMove * t2->x_vec;
			t2->center.y -= distanceToMove * t2->y_vec;
		}
	}
}

static int checkCollisionTankShot(Point c1, Point c2) {
	float distance_x = c1.x - c2.x;
	float distance_y = c1.y - c2.y;
	float sum_radius = RADIUS_FORCE_FIELD + RADIUS_SHOT;
	float distanceBetweenCircles = sqrt(distance_x * distance_x + distance_y * distance_y);

	if (distanceBetweenCircles <= sum_radius) return 1;
	return 0;
}

void collisionTankShot(Tank *t1, Tank *t2, ALLEGRO_SAMPLE *sound) {
	int shot_collide = 0;
	if (t1->is_shooting) {
		shot_collide = checkCollisionTankShot(t1->shot, t2->center);
		if (shot_collide) {
			t1->points += 1;
			al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			resetShotPosition(t1);
		}
	} 
	
	if (t2->is_shooting) {
		shot_collide = checkCollisionTankShot(t2->shot, t1->center);
		if (shot_collide) {
			t2->points += 1;
			al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			resetShotPosition(t2);
		}
	}
}

static float obstacleHeight(Obstacle o) {
    return fabs(o.lower_right.y - o.upper_left.y);
}

static float obstacleWidth(Obstacle o) {
    return fabs(o.upper_left.x - o.lower_right.x);
}

static Point obstacleCenter(Obstacle ret) {
    Point centro;
    centro.x = (ret.upper_left.x + ret.lower_right.x) / 2;
    centro.y = (ret.upper_left.y + ret.lower_right.y) / 2;
    return centro;
}

void collisionTankObstacle(Tank *t, Obstacle o) {
	int collide = 0, distance = 0;

    float retH = obstacleHeight(o);
    float retW = obstacleWidth(o);
    Point retCenter = obstacleCenter(o);

    float distX = fabs(t->center.x - retCenter.x);
    float distY = fabs(t->center.y - retCenter.y);

	float dx = distX - retW / 2;
    float dy = distY - retH / 2;
	float diagonalDistance = sqrt(dx*dx+dy*dy);

	if (distX > (retW / 2 + RADIUS_FORCE_FIELD) || distY > (retH / 2 + RADIUS_FORCE_FIELD)) collide = 0; 
    else if (distX <= (retW / 2)) collide = 1; 
	else if (distY <= (retH / 2)) collide = 2;
    else if (diagonalDistance <= RADIUS_FORCE_FIELD) collide = 3;

	float distanceToMove = 0;
	
	if (collide == 1) distanceToMove = (retH / 2) + RADIUS_FORCE_FIELD - distY;
	else if (collide == 2) distanceToMove = (retW / 2) + RADIUS_FORCE_FIELD - distX;
	else if (collide == 3) distanceToMove = RADIUS_FORCE_FIELD - diagonalDistance;

	if (t->speed < 0) {
		t->center.x += distanceToMove * t->x_vec;
		t->center.y += distanceToMove * t->y_vec;
	} 

	if (t->speed > 0) {
		t->center.x -= distanceToMove * t->x_vec;
		t->center.y -= distanceToMove * t->y_vec;
	}
}

static int checkCollisionShotObstacle(Point p, Obstacle o) {
    float retH = obstacleHeight(o);
    float retW = obstacleWidth(o);
    Point retCenter = obstacleCenter(o);

    float distX = fabs(p.x - retCenter.x);
    float distY = fabs(p.y - retCenter.y);

	float dx = distX - retW / 2;
    float dy = distY - retH / 2;
	float diagonalDistance = sqrt(dx*dx+dy*dy);

	if (distX > (retW / 2 + RADIUS_SHOT) 
		|| distY > (retH / 2 + RADIUS_SHOT)) return 0;

    if (distX <= (retW / 2)) return 1; 
	if (distY <= (retH / 2)) return 1;

    return diagonalDistance <= RADIUS_SHOT;
}

void collisionShotObstacle(Tank *t, Obstacle o, ALLEGRO_SAMPLE *sound) {
	int shot_collide = 0;
	if (t->is_shooting) {
		shot_collide = checkCollisionShotObstacle(t->shot, o);
		if (shot_collide) {
			al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			resetShotPosition(t);
		}
	} 
}
