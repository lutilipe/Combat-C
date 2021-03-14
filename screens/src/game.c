#include "../includes/game.h"

void restartGame(Tank *t1, Tank *t2, int *winner, int *screen) {
    *winner = 0;
    *screen = GAME_SCREEN;

	createTank(t1, TANK_1_INIT_POS_X, TANK_1_INIT_POS_Y);
	createTank(t2, TANK_2_INIT_POS_X, TANK_2_INIT_POS_Y);
}

void gameWinner(Tank t1, Tank t2, int *winner, int *screen) {
	if (t1.points >= 5) {
		*winner = 1;
        *screen = SCORE_SCREEN;
	} else if (t2.points >= 5) {
		*winner = 2;
		*screen = SCORE_SCREEN;
	} else {
        *winner = 0;
    }
}

void renderGame(
    Tank *t1, 
    Tank *t2, 
    Obstacle o1,
    ALLEGRO_FONT *points_font, 
    ALLEGRO_SAMPLE *shot_hit_block, 
    ALLEGRO_SAMPLE *shot_hit_tank
) {
    drawScenario();
    drawObstacle(o1);

    drawPoints(t1->points, t1->color, points_font, 100, 30);
    drawPoints(t2->points, t2->color, points_font, SCREEN_W - 100, 30);

    updateTank(t1);
    updateShot(t1);

    updateTank(t2);
    updateShot(t2);

    collisionBetweenTanks(t1, t2);

    collisionTankObstacle(t1, o1);
    collisionTankObstacle(t2, o1);

    collisionShotObstacle(t1, o1, shot_hit_block);
    collisionShotObstacle(t2, o1, shot_hit_block);

    shotOutOfScreen(t1);
    shotOutOfScreen(t2);

    collisionTankScreen(t1);
    collisionTankScreen(t2);

    collisionTankShot(t1, t2, shot_hit_tank);

    drawTank(*t1);
    drawShot(*t1);

    drawTank(*t2);
    drawShot(*t2);

    al_flip_display();
}