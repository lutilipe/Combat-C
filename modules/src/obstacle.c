#include "../includes/obstacle.h"
#include <stdio.h>

const int MARGIN_OF_SECURITY = RADIUS_FORCE_FIELD * 2 + 10;
const int MAX_X_POSITION = SCREEN_W / 2;

static int randomInt(int max) {
    return rand() % max;
}

static float randomX() {
    return MARGIN_OF_SECURITY + randomInt(MAX_X_POSITION - MARGIN_OF_SECURITY + 1);
}

static float randomY() {
    return MARGIN_OF_SECURITY + randomInt(SCREEN_H - 2*MARGIN_OF_SECURITY + 1);
}

static float randomWidth(float x) {
    float randWidth = MIN_WIDTH + randomInt(MAX_WIDTH - MIN_WIDTH + 1);
    if (x + randWidth >= MAX_X_POSITION) {
        return MAX_X_POSITION;
    }
    return x + randWidth;
}

static float randomHeight(float y) {
    float randHeight = MIN_HEIGHT + randomInt(MAX_HEIGHT - MIN_HEIGHT + 1);
    if (y + randHeight >= (SCREEN_H - MARGIN_OF_SECURITY)) {
        return SCREEN_H - MARGIN_OF_SECURITY;
    }
    return y + randHeight;
}

static void generateRandomPositions(Obstacle *o) {
    o->upper_left.x = randomX();
    o->upper_left.y = randomY();
    o->lower_right.x = randomWidth(o->upper_left.x);
    o->lower_right.y = randomHeight(o->upper_left.y);
}

static void createMirroredObstacle(Obstacle o, Obstacle *mirror) {
    mirror->upper_left.x = SCREEN_W - o.upper_left.x;
    mirror->upper_left.y = o.upper_left.y;
    mirror->lower_right.x = SCREEN_W - o.lower_right.x;
    mirror->lower_right.y = o.lower_right.y;
}

void createObstacles(Obstacle *obstacles) {
    int i = 0;
    for (i = 0; i < NUMBER_OF_OBSTACLES * 2; i+=2) {
        Obstacle o1, o2;
        generateRandomPositions(&o1);
        createMirroredObstacle(o1, &o2);
        obstacles[i] = o1;
        obstacles[i + 1] = o2;
    }
}