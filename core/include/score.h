#include <stdio.h>
#include <stdlib.h>

#include "../../tank/include/tank.h"

#ifndef SCORE_H
#define SCORE_H

int * getHistoryFromFile(char *fname);
int * writeHistoryFile(char *fname, int winner);
int gameWinner(Tank t1, Tank t2);

#endif