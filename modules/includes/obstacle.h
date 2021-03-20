#include <stdlib.h>

#include "../../core/includes/config.h"

#ifndef OBSTACLE_H
#define OBSTACLE_H

#define MAX_WIDTH 200
#define MAX_HEIGHT 200

#define MIN_WIDTH 20
#define MIN_HEIGHT 20

typedef struct Obstacle {
	Point upper_left, lower_right; 
} Obstacle;

void createObstacles(Obstacle *obstacles);

#endif