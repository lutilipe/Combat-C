#include <stdio.h>
#include <stdlib.h>

#include "./screens.h"

#ifndef GAME_H
#define GAME_H

void gameWinner(Tank t1, Tank t2, int *winner, int *screen);
void restartGame(Tank *t1, Tank *t2, int *winner, int *screen);
void renderGame(
    Tank *t1, 
    Tank *t2, 
    Obstacle o1,
    ALLEGRO_FONT *points_font, 
    ALLEGRO_SAMPLE *shot_hit_block, 
    ALLEGRO_SAMPLE *shot_hit_tank
);

#endif