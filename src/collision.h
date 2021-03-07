#include "tank.h"

#ifndef COLLISION_H
#define COLLISION_H

void shotOutOfScreen(Tank *t);
void collisionTankScreen(Tank *t);
void collisionBetweenTanks(Tank *t1, Tank *t2);
void collisionTankShot(Tank *t1, Tank *t2);

#endif