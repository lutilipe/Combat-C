#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include "../../modules/includes/tank.h"
#include "../../modules/includes/collision.h"
#include "../../modules/includes/obstacle.h"
#include "../../modules/includes/drawer.h"

#include "../../core/includes/config.h"

#ifndef SCREENS_H
#define SCREENS_H

enum SCREEN { 
    GAME_SCREEN, 
    SCORE_SCREEN
};

#endif