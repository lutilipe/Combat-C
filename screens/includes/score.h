#include <stdio.h>
#include <stdlib.h>

#include "./screens.h"

#ifndef SCORE_H
#define SCORE_H

void renderScore(
    Tank t1, 
    Tank t2, 
    int winner, 
    ALLEGRO_SAMPLE *win, 
    ALLEGRO_FONT *score_font
);

#endif