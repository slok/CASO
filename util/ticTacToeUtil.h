/*
Copyright (C) 2011 Iraide Diaz (Sharem)
Copyright (C) 2011 Xabier Larrakoetxea (slok)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
    int board2D[3][3];
    PracticaCaso::DsmDriver *driver;
    int arraySize; //this is for the sizeof
    
    public:
    
    PracticaCaso::DsmDriver *getDriver();
    int *getBoard1D();
    //int **getboard2D();
    TicTacToeUtil(string ip, int port, string dns);
    bool setPositionInBoard(int player, int x, int y);
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
    void setAgainToServer(bool again);
    bool getAgainFromServer();
    int getNumPlayersFromServer();
    void setPlayerName1(string name);
    void setPlayerName2(string name);
    char *getPlayerName1FromServer();
    char *getPlayerName2FromServer();
    
};
#endif
