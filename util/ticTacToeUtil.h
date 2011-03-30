#include <iostream>
#include "colors.h"

#define CROSS 1
#define CIRCLE 0
#define BLANK -1

void initializeBoard(int board[3][3]);
void drawMatrix(int board[3][3]);
bool checkBoardComplete(int board[3][3]);
int checkBoardState(int board[3][3]);
