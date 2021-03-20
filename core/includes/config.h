#ifndef CONFIG_H
#define CONFIG_H

#define FPS 60  

#define SCREEN_W 1270
#define SCREEN_H 720

#define THETA M_PI/4

#define RADIUS_FORCE_FIELD 30
#define RADIUS_SHOT 6.0

#define TANK_SPEED 4
#define TANK_SHOT_SPEED 15
#define TANK_ANGULAR_SPEED M_PI/80

#define NUMBER_OF_OBSTACLES 4

typedef struct Point {
	float x;
	float y;
} Point;

#endif