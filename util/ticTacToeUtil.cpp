
#include "ticTacToeUtil.h"


TicTacToeUtil::TicTacToeUtil(string ip, int port, string dns)
{
    this-> driver = new PracticaCaso::DsmDriver(ip, port, dns);
    this->board1D = new int[9];
    this->arraySize = 9;
    this->initializeBoard();

}
//returns 1 if the assignation was sucessful
bool TicTacToeUtil::setPositionInBoard(int player, int x, int y)
{
    bool result = false;
    if(this->board2D[x][y] == -1)
    {
        this->board2D[x][y] = player;
        result = true;
    }
    return result;
}

PracticaCaso::DsmDriver *TicTacToeUtil::getDriver()
{
    return this->driver;
}
int *TicTacToeUtil::getBoard1D()
{
    return this->board1D;
}

/*int **TicTacToeUtil::getboard2D()
{
    return this->board2D;
}*/

void TicTacToeUtil::initializeBoard()
{
    for(int i=0; i < 3; i++)
    {
        for(int j=0; j< 3; j++)
        {
            this->board2D[i][j] = BLANK;
        }
    }
}

void TicTacToeUtil::drawMatrix()
{
    /* aspect:
        -------------------
        |  O  |  O  |  0  |
        -------------------
        |  O  |  O  |  0  |
        -------------------
        |  O  |  O  |  0  |
        -------------------
    */
    for(int i=0; i < 3; i++)
    {
        std::cout <<"-------------------" << std::endl;

        for(int j=0; j< 3; j++)
        {
            switch(this->board2D[j][i])
            {
                case CROSS: std::cout << "|" << "  " << RED_BOLD << "X" << COL_RESET << "  "; break;
                case CIRCLE: std::cout << "|" << "  " << BLUE_BOLD<< "O" << COL_RESET << "  "; break;
                case BLANK: std::cout << "|" << "  " << " " << "  ";; break;
            }
        }
        std::cout << "|" << std::endl;
    }
    std::cout << "-------------------" << std::endl;
}

void TicTacToeUtil::drawMatrix1D()
{
    for(int i=0;i < 9; i++)
    {
        cout << "["<<this->board1D[i] << "] ";
    }
    cout << endl;
}

bool TicTacToeUtil::checkBoardComplete()
{
    bool complete = true;
    
    //check if the board is complete
    for(int i=0; i < 3; i++)
    {
        for(int j=0; j< 3; j++)
        {
            if(this->board2D[i][j] == BLANK)
            {
                complete = false;
                break;
            }
        }
    }
    
    return complete;
}

int TicTacToeUtil::checkBoardState()
{
    // 0 = nobody, 1 = player1, 2 = player2
    int win;
    
    if( (board2D[0][0] == CROSS && board2D[0][0] == board2D[0][1] && board2D[0][1] == board2D[0][2]) || (board2D[1][0] == CROSS && board2D[1][0] == board2D[1][1] && board2D[1][1] == board2D[1][2]) ||
        (board2D[2][0] == CROSS && board2D[2][0] == board2D[2][1] && board2D[2][1] == board2D[2][2]) || (board2D[1][0] == CROSS && board2D[0][0] == board2D[1][0] && board2D[1][0] == board2D[2][0]) ||
        (board2D[0][1] == CROSS && board2D[0][1] == board2D[1][1] && board2D[1][1] == board2D[2][1]) || (board2D[0][2] == CROSS && board2D[0][2] == board2D[1][2] && board2D[1][2] == board2D[2][2]) ||
        (board2D[0][0] == CROSS && board2D[0][0] == board2D[1][1] && board2D[1][1] == board2D[2][2]) || (board2D[2][0] == CROSS && board2D[2][0] == board2D[1][1] && board2D[1][1] == board2D[0][2]))
    {
        win = CROSS;
    }else
    if( (board2D[0][0] == CIRCLE && board2D[0][0] == board2D[0][1] && board2D[0][1] == board2D[0][2]) || (board2D[1][0] == CIRCLE && board2D[1][0] == board2D[1][1] && board2D[1][1] == board2D[1][2]) ||
        (board2D[2][0] == CIRCLE && board2D[2][0] == board2D[2][1] && board2D[2][1] == board2D[2][2]) || (board2D[1][0] == CIRCLE && board2D[0][0] == board2D[1][0] && board2D[1][0] == board2D[2][0]) ||
        (board2D[0][1] == CIRCLE && board2D[0][1] == board2D[1][1] && board2D[1][1] == board2D[2][1]) || (board2D[0][2] == CIRCLE && board2D[0][2] == board2D[1][2] && board2D[1][2] == board2D[2][2]) ||
        (board2D[0][0] == CIRCLE && board2D[0][0] == board2D[1][1] && board2D[1][1] == board2D[2][2]) || (board2D[2][0] == CIRCLE && board2D[2][0] == board2D[1][1] && board2D[1][1] == board2D[0][2]))
    {
        win = CIRCLE;
    }
    else
        win = BLANK;

    return win;

}

void TicTacToeUtil::convert1DTo2D()
{
    int x = 0;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            this->board2D[j][i] = this->board1D[x];
            x+=1;
        }
    }
}

void TicTacToeUtil::convert2DTo1D()
{
    int x = 0;

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            this->board1D[x] = this->board2D[j][i];
            x+=1;
        }
    }

}
void TicTacToeUtil::getBoardFromServer()
{
    PracticaCaso::DsmData data;
    //get the board
    cout << GREEN_BOLD << "[GETTING 3X3 TIC TAC TOE BOARD: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool boardGet = false;
    while (!boardGet) {
        try {
            data = this->driver->dsm_get("board");
            this->board1D = ((int *)data.addr);
            this->convert1DTo2D();
            boardGet = true;
        } catch (DsmException dsme) {
            cerr << RED_BOLD << "ERROR in dsm_get(\"board\") - waiting for other process to initialize it: " << dsme << COL_RESET<< endl;
            driver->dsm_wait("board");
        }
    }
}

void TicTacToeUtil::setBoardToServer()
{
    PracticaCaso::DsmData data;
    try 
    {
        //is need to transform, because the server stores only an array
        this->convert2DTo1D();
        this->driver->dsm_put("board", (void *)this->board1D, sizeof(int)* this->arraySize); 
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"board\", board, " << sizeof(int)*this->arraySize << ")): " << dsme << COL_RESET << endl;
        driver->dsm_free("board");
        exit(1);
    }
}

int TicTacToeUtil::getTurnFromServer()
{
    PracticaCaso::DsmData data;
    int turn;
    //get the board
    cout << GREEN_BOLD << "[GETTING TURN: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool turnGet = false;
    while (!turnGet) {
        try {
            data = this->driver->dsm_get("turn");
            turn = *((int *)data.addr);
            turnGet = true;
        } catch (DsmException dsme) {
            cout << RED_BOLD << "WAITING FOR TURN "<< COL_RESET<< endl;
            driver->dsm_wait("turn");
        }
    }
    return turn;
}

void TicTacToeUtil::setTurnToServer(int turn)
{
    //referee's turn
    //turn = 0;
    try {
        this->driver->dsm_put("turn", (void *)&turn, sizeof(turn)); 
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"turn\", turn, " << sizeof(turn) << ")): " << dsme << COL_RESET << endl;
        driver->dsm_free("board");
        exit(1);
    }
}

int TicTacToeUtil::getWinFromServer()
{
    PracticaCaso::DsmData data;
    int win;
    //get the board
    cout << GREEN_BOLD << "[GETTING TURN: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool winGet = false;
    while (!winGet) {
        try {
            data = this->driver->dsm_get("win");
            win = *((int *)data.addr);
            winGet = true;
        } catch (DsmException dsme) {
            cout << RED_BOLD << "WAITING FOR WIN VARIABLE "<< COL_RESET<< endl;
            driver->dsm_wait("win");
        }
    }
    return win;
}
void TicTacToeUtil::setWinToServer(int win)
{
    try {
        this->driver->dsm_put("win", (void *)&win, sizeof(win)); 
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"win\", win, " << sizeof(win) << ")): " << dsme << COL_RESET << endl;
        driver->dsm_free("board");
        exit(1);
    }
}
void TicTacToeUtil::allocBoardInServer()
{
    cout << GREEN_BOLD << "[CREATING 3X3 TIC TAC TOE BOARD: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    try 
    {
		this->driver->dsm_malloc("board", sizeof(int)* this->arraySize);
        
        //set in server
        this->setBoardToServer();
        
	} catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"Board\", sizeof(" << sizeof(int) * this->arraySize << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
}

void TicTacToeUtil::setAgainToServer(bool again)
{
    //store turn for the first one
    try {
        this->driver->dsm_put("again", (void *)&again, sizeof(again));
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"again\", again, " << sizeof(again) << ")): " << dsme << COL_RESET << endl;
        this->driver->dsm_free("again");
        exit(1);
    }
}
bool TicTacToeUtil::getAgainFromServer()
{
    PracticaCaso::DsmData data;
    bool again;
    //get the board
    cout << GREEN_BOLD << "[GETTING AGAIN: " << this->driver->get_nid() << " ]"<< COL_RESET << endl;
    bool againGet = false;
    while (!againGet) {
        try {
            data = this->driver->dsm_get("again");
            again = *((bool *)data.addr);
            againGet = true;
        } catch (DsmException dsme) {
            cout << RED_BOLD << "WAITING FOR AGAIN VARIABLE "<< COL_RESET<< endl;
            this->driver->dsm_wait("again");
        }
    }
    return again;
}
/*int main()
{

    int board[][3];
    int win;
    initializeBoard(board);
    board[0][0] = CROSS;
    board[1][1] = CROSS;
    board[2][2] = CROSS;

    board[0][1] = CIRCLE;
    board[0][2] = CIRCLE;
    board[2][1] = CIRCLE;

    std::cout << GREEN_BOLD << "Tic Tac Toe!!" << COL_RESET<< std::endl<< std::endl<< std::endl;
    drawMatrix(board);
    win = checkBoardState(board);
    std::cout << std::endl;

    switch(win)
    {
        case CROSS: std::cout << CYAN_BOLD<< "The winner is: " << RED_BOLD << "CROSS" << COL_RESET<< std::endl; break;
        case CIRCLE: std::cout << CYAN_BOLD << "The winner is: " <<BLUE_BOLD << "CIRCLE" << COL_RESET<< std::endl ;break;
        case BLANK: std::cout << CYAN_BOLD << "No winner :(" << COL_RESET<< std::endl; break;
    }


    return 0;
}*/
