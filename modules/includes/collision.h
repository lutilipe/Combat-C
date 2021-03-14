#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "./obstacle.h"
#include "./tank.h"

#ifndef COLLISION_H
#define COLLISION_H

void shotOutOfScreen(Tank *t);
void collisionTankScreen(Tank *t);
void collisionBetweenTanks(Tank *t1, Tank *t2);
void collisionTankShot(Tank *t1, Tank *t2, ALLEGRO_SAMPLE *sound);
void collisionTankObstacle(Tank *t, Obstacle o);
void collisionShotObstacle(Tank *t, Obstacle o, ALLEGRO_SAMPLE *sound);

#endif