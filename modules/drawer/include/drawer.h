#include <allegro5/allegro_font.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "../../obstacle/include/obstacle.h"
#include "../../tank/include/tank.h"
#include "../../collision/include/collision.h"

#ifndef DRAWER_H
#define DRAWER_H

void drawScenario();
void drawTank(Tank t);
void drawShot(Tank t);
void drawPoints(int point, ALLEGRO_COLOR color, ALLEGRO_FONT *font, float x, float y);
void drawScoreScreen(Tank t1, Tank t2, int *score, ALLEGRO_FONT *font);
void drawObstacle(Obstacle o);

#endif