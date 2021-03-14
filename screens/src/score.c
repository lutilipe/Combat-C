#include "../includes/score.h"
#include <stdlib.h>

static void *getHistoryFromFile(char *fname, int *score) {
    FILE *file;

    if ((file = fopen(fname, "r"))) {
		fscanf(file, "%i %i", &score[0], &score[1]);
        fclose(file);
    }
}

static void *writeHistoryFile(char *fname, int winner, int *score) {
	getHistoryFromFile(fname, score);
	FILE *file;
	score[winner - 1] += 1;
	file = fopen(fname, "w");
	fprintf(file, "%i %i", score[0], score[1]);
	fclose(file);
}

void renderScore(
	Tank t1, 
	Tank t2, 
	int winner, 
	ALLEGRO_SAMPLE *win, 
	ALLEGRO_FONT *score_font,
	ALLEGRO_EVENT_QUEUE *queue
) {
	ALLEGRO_TIMEOUT timeout;
   	ALLEGRO_EVENT event;

	int score[2] = {0, 0};
	writeHistoryFile("./historico.txt", winner, score);
	al_play_sample(win, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	al_init_timeout(&timeout, 5);
	while (al_wait_for_event_until(queue, &event, &timeout)) {
		drawScoreScreen(t1, t2, score, score_font);
		al_flip_display();
	}
}