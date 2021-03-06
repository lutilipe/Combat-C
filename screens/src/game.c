#include "../includes/game.h"

void restartGame(Tank *t1, Tank *t2, Obstacle *obstacles, int *winner, int *screen) {
    *winner = 0;
    *screen = GAME_SCREEN;

	createTank(t1, TANK_1_INIT_POS_X, TANK_1_INIT_POS_Y);
	createTank(t2, TANK_2_INIT_POS_X, TANK_2_INIT_POS_Y);

    createObstacles(obstacles);
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
    Obstacle *obstacles,
    ALLEGRO_FONT *points_font, 
    ALLEGRO_SAMPLE *shot_hit_block, 
    ALLEGRO_SAMPLE *shot_hit_tank
) {
    int i = 0;
    drawScenario();

    updateTank(t1);
    updateShot(t1);

    updateTank(t2);
    updateShot(t2);

    for (i = 0; i < NUMBER_OF_OBSTACLES * 2; i++) {
        drawObstacle(obstacles[i]);
        
        collisionTankObstacle(t1, obstacles[i]);
        collisionTankObstacle(t2, obstacles[i]);

        collisionShotObstacle(t1, obstacles[i], shot_hit_block);
        collisionShotObstacle(t2, obstacles[i], shot_hit_block);
    }

    drawPoints(t1->points, t1->color, points_font, 100, 30);
    drawPoints(t2->points, t2->color, points_font, SCREEN_W - 100, 30);

    collisionBetweenTanks(t1, t2);

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