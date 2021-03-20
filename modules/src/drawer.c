#include "../includes/drawer.h"

void drawScenario() {
	al_clear_to_color(al_map_rgb(255, 255, 255));
}

void drawTank(Tank t) {
	al_draw_circle(t.center.x, t.center.y, RADIUS_FORCE_FIELD, t.color, 1);

	al_draw_filled_triangle(
		t.A.x + t.center.x, 
		t.A.y + t.center.y, 
		t.B.x + t.center.x,
		t.B.y + t.center.y,
		t.C.x + t.center.x,
		t.C.y + t.center.y,
		t.color
	);
}

void drawShot(Tank t) {
	float x = t.is_shooting
		? t.shot.x
		: t.shot.x + t.center.x;

	float y = t.is_shooting
		? t.shot.y
		: t.shot.y + t.center.y;

	al_draw_filled_circle(
		x, 
		y, 
		RADIUS_SHOT, 
		t.color
	);
}

void drawPoints(int point, ALLEGRO_COLOR color, ALLEGRO_FONT *font, float x, float y) {
	al_draw_textf(font, color, x, y, ALLEGRO_ALIGN_CENTRE, "%i", point);
}

void drawScoreScreen(Tank t1, Tank t2, int *score, ALLEGRO_FONT *font) {
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H * 0.125, ALLEGRO_ALIGN_CENTRE, "%s", "RESULTADO");
	drawPoints(t1.points, t1.color, font, SCREEN_W * 0.42, SCREEN_H * 0.25);
	al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H * 0.25, ALLEGRO_ALIGN_CENTRE, "%s", "x");
	drawPoints(t2.points, t2.color, font, SCREEN_W * 0.58, SCREEN_H * 0.25);
	al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H * 0.5, ALLEGRO_ALIGN_CENTRE, "%s", "HISTORICO");
	drawPoints(score[0], t1.color, font, SCREEN_W * 0.42, SCREEN_H * 0.625);
	al_draw_textf(font, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H * 0.625, ALLEGRO_ALIGN_CENTRE, "%s", "x");
	drawPoints(score[1], t2.color, font, SCREEN_W * 0.58, SCREEN_H * 0.625);
}

void drawObstacle(Obstacle o) {
	al_draw_filled_rectangle(o.upper_left.x, o.upper_left.y, o.lower_right.x, o.lower_right.y, al_map_rgb(0, 0, 0));
}