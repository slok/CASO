#ifndef _TICTACTOEUTIL_H
#define _TICTACTOEUTIL_H

#include <iostream>
#include "colors.h"
#include "TcpListener.h"
#include "Dsm.h"

#define CROSS 1
#define CIRCLE 0
#define BLANK -1
class TicTacToeUtil
{
    private:
    int *board1D;
    PracticaCaso::DsmDriver *driver;
    int arraySize; //this is for the sizeof
    
    public:
    int board2D[3][3]; //this is a bad thing, but we can't return multidimensional array

    PracticaCaso::DsmDriver *getDriver();
    int *getBoard1D();
    //int **getboard2D();
    TicTacToeUtil(string ip, int port, string dns);
    void initializeBoard();
    void drawMatrix();
    void drawMatrix1D();
    bool checkBoardComplete();
    int checkBoardState();
    void convert1DTo2D();
    void convert2DTo1D();
    void getBoardFromServer();
    void setBoardToServer();
    int getTurnFromServer();
    void setTurnToServer(int turn);
    int getWinFromServer();
    void setWinToServer(int win);
    void allocBoardInServer();
    
};
#endif
