#ifndef CONFIG_H
#define CONFIG_H

#define FPS 60  

#define SCREEN_W 960
#define SCREEN_H 540

#define THETA M_PI/4

#define RADIUS_FORCE_FIELD 30
#define RADIUS_SHOT 6.0

#define TANK_SPEED 3.5
#define TANK_SHOT_SPEED 12
#define TANK_ANGULAR_SPEED M_PI/90

typedef struct Point {
	float x;
	float y;
} Point;

#endif