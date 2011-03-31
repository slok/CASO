#ifndef _TICTACTOEUTIL_H
#define _TICTACTOEUTIL_H

#include <iostream>
#include "colors.h"

#define CROSS 1
#define CIRCLE 0
#define BLANK -1

void initializeBoard(int board[][3]);
void drawMatrix(int board[][3]);
bool checkBoardComplete(int board[][3]);
int checkBoardState(int board[][3]);
void convert1DTo2D(int *orig, int out[][3]);
void convert2DTo1D(int orig[][3], int *out);

#endif
