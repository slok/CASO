
#include "ticTacToeUtil.h"

void initializeBoard(int board[3][3])
{
    for(int i=0; i < 3; i++)
    {
        for(int j=0; j< 3; j++)
        {
            board[i][j] = BLANK;
        }
    }
}

void drawMatrix(int board[3][3])
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
            switch(board[j][i])
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

bool checkBoardComplete(int board[3][3])
{
    bool complete = false;
    
    //check if the board is complete
    for(int i=0; i < 3; i++)
    {
        for(int j=0; j< 3; j++)
        {
            if(board[i][j] == BLANK)
            {
                complete = true;
                break;
            }
        }
    }
    
    return complete;
}

int checkBoardState(int board[3][3])
{
    int win;
    
    if( (board[0][0] == CROSS && board[0][0] == board[0][1] && board[0][1] == board[0][2]) || (board[1][0] == CROSS && board[1][0] == board[1][1] && board[1][1] == board[1][2]) ||
        (board[2][0] == CROSS && board[2][0] == board[2][1] && board[2][1] == board[2][2]) || (board[1][0] == CROSS && board[0][0] == board[1][0] && board[1][0] == board[2][0]) ||
        (board[0][1] == CROSS && board[0][1] == board[1][1] && board[1][1] == board[2][1]) || (board[0][2] == CROSS && board[0][2] == board[1][2] && board[1][2] == board[2][2]) ||
        (board[0][0] == CROSS && board[0][0] == board[1][1] && board[1][1] == board[2][2]) || (board[2][0] == CROSS && board[2][0] == board[1][1] && board[1][1] == board[0][2]))
    {
        win = CROSS;
    }else
    if( (board[0][0] == CIRCLE && board[0][0] == board[0][1] && board[0][1] == board[0][2]) || (board[1][0] == CIRCLE && board[1][0] == board[1][1] && board[1][1] == board[1][2]) ||
        (board[2][0] == CIRCLE && board[2][0] == board[2][1] && board[2][1] == board[2][2]) || (board[1][0] == CIRCLE && board[0][0] == board[1][0] && board[1][0] == board[2][0]) ||
        (board[0][1] == CIRCLE && board[0][1] == board[1][1] && board[1][1] == board[2][1]) || (board[0][2] == CIRCLE && board[0][2] == board[1][2] && board[1][2] == board[2][2]) ||
        (board[0][0] == CIRCLE && board[0][0] == board[1][1] && board[1][1] == board[2][2]) || (board[2][0] == CIRCLE && board[2][0] == board[1][1] && board[1][1] == board[0][2]))
    {
        win = CIRCLE;
    }
    else
        win = BLANK;

    return win;

}

/*int main()
{

    int board[3][3];
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
