#include "TcpListener.h"
#include "Dsm.h"
#include "colors.h"
#include "ticTacToeUtil.h"




#define BUFFER_SIZE 1024

// we know that global variables are bad idea, but only will be one of each and will be used in some methods
PracticaCaso::DsmDriver *driver;
PracticaCaso::DsmData data;

void getBoard(int board2D[][3])
{
    int *board1D;
    //get the board
    cout << GREEN_BOLD << "[GETTING 3X3 TIC TAC TOE BOARD: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool boardGet = false;
    while (!boardGet) {
        try {
            data = driver->dsm_get("board");
            board1D = ((int *)data.addr);
            convert1DTo2D(board1D, board2D);
            boardGet = true;
        } catch (DsmException dsme) {
            cerr << RED_BOLD << "ERROR in dsm_get(\"board\") - waiting for other process to initialize it: " << dsme << COL_RESET<< endl;
            driver->dsm_wait("board");
        }
    }
}

void setBoard(int board2D[][3])
{
    int *board1D;
    try 
    {
        //is need to transform, because the server stores only an array
        convert2DTo1D(board2D, board1D);
        driver->dsm_put("board", (void *)board1D, sizeof(board1D)); 
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"board\", board, " << sizeof(board1D) << ")): " << dsme << COL_RESET << endl;
        driver->dsm_free("board");
        exit(1);
    }
}

int getTurn()
{
    int turn;
    //get the board
    cout << GREEN_BOLD << "[GETTING TURN: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool turnGet = false;
    while (!turnGet) {
        try {
            data = driver->dsm_get("turn");
            turn = *((int *)data.addr);
            turnGet = true;
        } catch (DsmException dsme) {
            cout << RED_BOLD << "WAITING FOR TURN "<< COL_RESET<< endl;
            driver->dsm_wait("turn");
        }
    }
    return turn;
}
void setTurn(int turn)
{
    //referee's turn
    turn = 0;
    try {
        driver->dsm_put("turn", (void *)&turn, sizeof(turn)); 
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"turn\", turn, " << sizeof(turn) << ")): " << dsme << COL_RESET << endl;
        driver->dsm_free("board");
        exit(1);
    }
}
int getWin()
{
    int win;
    //get the board
    cout << GREEN_BOLD << "[GETTING TURN: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    bool winGet = false;
    while (!winGet) {
        try {
            data = driver->dsm_get("win");
            win = *((int *)data.addr);
            winGet = true;
        } catch (DsmException dsme) {
            cout << RED_BOLD << "WAITING FOR WIN VARIABLE "<< COL_RESET<< endl;
            driver->dsm_wait("win");
        }
    }
    return win;
}

void usage() {
	cout << "Usage: ProposalDsmClient <port> <dns>" << endl;
	exit(1);
}


int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

	// Hacer lookup dsm.deusto.es 
    driver = new PracticaCaso::DsmDriver("127.0.0.1", atoi(argv[1]), argv[2]);
	//PracticaCaso::DsmData data;
    int board2D[3][3];
    int *board1D;
    string name, aux;
    int numPlayers, turn, player, win, x, y;
    bool exitLoop = false;
    
    //get number of players and check referee connected
    bool playersGet = false;
	while (!playersGet) {
		try {
			cout << GREEN_BOLD << "[GETTING NUMBER OF PLAYERS: " << driver->get_nid() << " ]"<< COL_RESET << endl;
            data = driver->dsm_get("numPlayers");
			playersGet = true;
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: Referee not connected, waiting..." << dsme << COL_RESET<< endl;
			driver->dsm_wait("numPlayers");
		}
	}
    
    numPlayers = *((int *)data.addr);
    
    //check number of players
    if(numPlayers < 2)
    {
        numPlayers +=1;
        
        //select the turn (1 or 2)
        player = numPlayers;
        cout << CYAN << "YOU ARE THE PLAYER: " <<  player << COL_RESET <<endl;
        
        try {
            cout << GREEN_BOLD << "[INCREMENT NUMBER OF PLAYERS: " << driver->get_nid() << " ]"<< COL_RESET << endl;
			driver->dsm_put("numPlayers", (void *)&numPlayers, sizeof(numPlayers));
		} catch (...) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"numPlayers\", (void *)&numPlayers, sizeof(numPlayers))" << COL_RESET << endl;
			exit(1);
		}
        
        //select name
        //cout << CYAN << "Select Player name: " << COL_RESET << endl;
        //getline(cin, name);
        
        //gameloop
        while(!exitLoop)
        {
            
           
            getBoard(board2D);
            system("clear"); //*nix
            //system("cls"); //windows
            drawMatrix(board2D);
            turn = getTurn();
            win = getWin();
            
            if( win == -1)
            {
                if( turn == 0)
                {
                    cout << RED_BOLD << "IS REFEREES TURN" << COL_RESET << endl;
                    cout << RED_BOLD << "WAITING FOR TURN "<< COL_RESET<< endl;
                    driver->dsm_wait("turn");
                }
                else if( turn != player)
                {
                    cout << RED_BOLD << "IS THE OTHER PLAYERS TURN" << COL_RESET << endl;
                    cout << RED_BOLD << "WAITING FOR TURN "<< COL_RESET<< endl;
                    driver->dsm_wait("turn");
                }
                else //our turn
                {
                    //ask for the position
                    cout << CYAN_BOLD << "SELECT X:" << endl;
                    getline(cin, aux);
                    x = atoi(aux.c_str());
                    cout << CYAN_BOLD << "SELECT y:" << endl;
                    getline(cin, aux);
                    y = atoi(aux.c_str());
                    x-=1; y-=1;
                    
                    //set position
                    board2D[x][y] = player - 1;
                    //drawMatrix(board2D);
                    setBoard(board2D);
                    
                }
            }
            else
            {
                //game has finished(have we win??)
            }
        }
    }
    else
        cout << RED_BOLD << "ERROR: Two players connected..." << COL_RESET<< endl;
    
	cout << GREEN_BOLD << "[SLEEPING FOR A SECOND BEFORE FINISHING...]" << COL_RESET << endl;
	sleep(1);
	delete driver;
}
