#include "TcpListener.h"
#include "Dsm.h"
#include "colors.h"
#include "ticTacToeUtil.h"

#define BUFFER_SIZE 1024

void usage() {
	cout << "Usage: ProposalDsmReferee <port> <dns>" << endl;
	exit(1);
}




int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

	// Hacer lookup dsm.deusto.es 
    PracticaCaso::DsmDriver * driver = new PracticaCaso::DsmDriver("127.0.0.1", atoi(argv[1]), argv[2]);
	PracticaCaso::DsmData data;
    int board2D[3][3];
    int board1D[9];
    int numPlayers, next=1, win = -1; // -1 = not finished, 0 = nobody, 1 = player1, 2 = player2
    int turn; //0 = referee, 1 = player1, 2 = player2 
    
	
    //////////////////create number players///////////////////////
    try 
    {
		driver->dsm_malloc("numPlayers", sizeof(numPlayers));
        //initialize
        numPlayers = 0;
        //set in server
        try {
			driver->dsm_put("numPlayers", (void *)&numPlayers, sizeof(numPlayers)); 
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"numPlayers\", numPlayers, " << sizeof(numPlayers) << ")): " << dsme << COL_RESET << endl;
			driver->dsm_free("numPlayers");
			exit(1);
		}
	} catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"numPlayers\", sizeof(" << sizeof(numPlayers) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
    /////////////////////create board/////////////////////////////
    cout << GREEN_BOLD << "[CREATING 3X3 TIC TAC TOE BOARD: " << driver->get_nid() << " ]"<< COL_RESET << endl;
    try 
    {
		driver->dsm_malloc("board", sizeof(board1D));
        //initialize
        for(int i=0; i < 3; i++)
        {
            for(int j=0; j< 3; j++)
            {
                board2D[i][j] = BLANK;
            }
        }
        //set in server
        try {
            //is need to transform, because the server stores only an array
            convert2DTo1D(board2D, board1D);
			driver->dsm_put("board", (void *)board1D, sizeof(board1D)); 
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"board\", board, " << sizeof(board1D) << ")): " << dsme << COL_RESET << endl;
			driver->dsm_free("board");
			exit(1);
		}
		
	} catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"Board\", sizeof(" << sizeof(board1D) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
	
    ///////////////////create turn/////////////////////////////
     try 
    {
		driver->dsm_malloc("turn", sizeof(turn));
        turn = 1;
        //store turn for the first one
        try {
			driver->dsm_put("turn", (void *)&turn, sizeof(turn));
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"turn\", turn, " << sizeof(turn) << ")): " << dsme << COL_RESET << endl;
			driver->dsm_free("turn");
			exit(1);
		}
        
    } catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"turn\", sizeof(" << sizeof(turn) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
    
    ////////////////create win/////////////////////////
    try
    {
		driver->dsm_malloc("win", sizeof(win));
        win = -1;
        //store win
        try {
			driver->dsm_put("win", (void *)&win, sizeof(win));
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"win\", win, " << sizeof(win) << ")): " << dsme << COL_RESET << endl;
			driver->dsm_free("turn");
			exit(1);
		}
        
    } catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"win\", sizeof(" << sizeof(win) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
    
    
    while(1)
    {
        
    }
    
    
    driver->dsm_free("numPlayers");
    driver->dsm_free("board");
    driver->dsm_free("turn");
    driver->dsm_free("win");
    cout << GREEN_BOLD << "[SLEEPING FOR A SECOND BEFORE FINISHING...]" << COL_RESET << endl;
	sleep(1);
	delete driver;
}

