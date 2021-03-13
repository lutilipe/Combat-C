#include "../include/score.h"

int * getHistoryFromFile(char *fname) {
    FILE *file;
	static int score[2] = {0, 0};

    if ((file = fopen(fname, "r"))) {
		fscanf(file, "%i %i", &score[0], &score[1]);
        fclose(file);
    }

	return score;
}

int * writeHistoryFile(char *fname, int winner) {
	int *score = getHistoryFromFile(fname);
	FILE *file;
	score[winner - 1] += 1;
	file = fopen(fname, "w");
	fprintf(file, "%i %i", score[0], score[1]);
	fclose(file);
	return score;
}

int gameWinner(Tank t1, Tank t2) {
	if (t1.points >= 5) {
		return 1;
	} else if (t2.points >= 5) {
		return 2;
	}

	return 0;
}