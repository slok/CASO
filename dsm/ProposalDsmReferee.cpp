#include "TcpListener.h"
#include "Dsm.h"
#include "colors.h"
#include "ticTacToeUtil.h"
#include "SQLiteMap.h"

#define BUFFER_SIZE 1024

void usage() {
	cout << "Usage: ProposalDsmReferee <port> <dns>" << endl;
	exit(1);
}

void createNumberOfplayers(TicTacToeUtil ttt, int numPlayers)
{
    try 
    {
		ttt.getDriver()->dsm_malloc("numPlayers", sizeof(numPlayers));
        //initialize
        numPlayers = 0;
        //set in server
        try {
			ttt.getDriver()->dsm_put("numPlayers", (void *)&numPlayers, sizeof(numPlayers)); 
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"numPlayers\", numPlayers, " << sizeof(numPlayers) << ")): " << dsme << COL_RESET << endl;
			ttt.getDriver()->dsm_free("numPlayers");
			exit(1);
		}
	} catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"numPlayers\", sizeof(" << sizeof(numPlayers) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
}

void createTurn(TicTacToeUtil ttt, int turn)
{
    try 
    {
		ttt.getDriver()->dsm_malloc("turn", sizeof(turn));
        //store turn for the first one
        try {
            ttt.getDriver()->dsm_put("turn", (void *)&turn, sizeof(turn));
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: dsm_put(\"turn\", turn, " << sizeof(turn) << ")): " << dsme << COL_RESET << endl;
			ttt.getDriver()->dsm_free("turn");
			exit(1);
		}
        
    } catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"turn\", sizeof(" << sizeof(turn) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}
}

void createWin(TicTacToeUtil ttt, int win)
{
    try
    {
        ttt.getDriver()->dsm_malloc("win", sizeof(win));
        win = -1;
        //store win
        try {
            ttt.getDriver()->dsm_put("win", (void *)&win, sizeof(win));
        } catch (DsmException dsme) {
            cerr << RED_BOLD << "ERROR: dsm_put(\"win\", win, " << sizeof(win) << ")): " << dsme << COL_RESET << endl;
            ttt.getDriver()->dsm_free("turn");
            exit(1);
        }
        
    } catch (DsmException dsme) {
        // There may be several processes doing a dsm_malloc, only the first one will succeed 
        cerr << RED_BOLD << "[ERROR in dsm_malloc(\"win\", sizeof(" << sizeof(win) << ")): " << dsme << " ]" << COL_RESET << endl;
        exit(1);
    }
}

void createAgain(TicTacToeUtil ttt)
{
    bool again=true;
    try 
    {
		ttt.getDriver()->dsm_malloc("again", sizeof(again));
        try {
            ttt.getDriver()->dsm_put("again", (void *)&again, sizeof(again));
            //counter the put
            ttt.getDriver()->dsm_wait("again");
        } catch (DsmException dsme) {
            cerr << RED_BOLD << "ERROR: dsm_put(\"again\", again, " << sizeof(again) << ")): " << dsme << COL_RESET << endl;
            ttt.getDriver()->dsm_free("again");
            exit(1);
        }
    } catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"again\", sizeof(" << sizeof(again) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}

}

void createPlayerNames(TicTacToeUtil ttt)
{
    char name[50];
    try 
    {
		ttt.getDriver()->dsm_malloc("playerName1", sizeof(name));
        ttt.getDriver()->dsm_malloc("playerName2", sizeof(name));
    } catch (DsmException dsme) {
		// There may be several processes doing a dsm_malloc, only the first one will succeed 
		cerr << RED_BOLD << "[ERROR in dsm_malloc(\"playerX\", sizeof(" << sizeof(name) << ")): " << dsme << " ]" << COL_RESET << endl;
		exit(1);
	}

}

//resets variables(win, board and turn)
void resetAll(TicTacToeUtil ttt)
{
    cout << RED_BOLD << "RESETING VALUES..." << COL_RESET<< endl;
    //reset variables
    int turn = 1, win= -1;
    bool again = true;
    ttt.initializeBoard();
    
    //set variables in server
    ttt.setAgainToServer(again);
    ttt.getDriver()->dsm_wait("again"); //counter the set again
    ttt.setWinToServer(win);
    ttt.setTurnToServer(turn);
    ttt.getDriver()->dsm_wait("turn"); //we have to counteract the set of turn notification
    ttt.setBoardToServer();
}

bool firstTime(TicTacToeUtil ttt, int p1, int p2)
{
    stringstream intStrAux;
    bool first = false;
    PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("scores.db");
    
    //reset for first time (new player)  and save it if not again
    if(!ttt.getAgainFromServer())
    {
        cout << CYAN_BOLD << p1 <<" | "<< p2 << COL_RESET << endl;
        
        //no negative points in database
        if(p1 < 0)
            p1 = 0;
        if(p2 < 0)
            p2 = 0;
            
        cout << CYAN_BOLD << "INSERTING SCORES IN DATABASE..." << COL_RESET << endl;
        intStrAux.str("");
        intStrAux << p1;
        SQLiteMap->set(ttt.getPlayerName1FromServer(), intStrAux.str());
        
        intStrAux.str("");
        intStrAux << p2;
        SQLiteMap->set(ttt.getPlayerName2FromServer(), intStrAux.str());
        first = true;
    }
    delete SQLiteMap;
    return first;
}

bool again(TicTacToeUtil ttt, int win, int p1, int p2)
{
    //returns if is again a first time of new roud of new players
    
    bool first= false;
    //the client at this moment is waiting for a turn, so to break the deadlock(interbloqueo) 
    //because we need that the client set the again. I other words, referee is waiting "again notification" and client is waiting "turn notification"
    ttt.setTurnToServer(0);
    
    /*we have to counteract the set of turn notification, but the players don't put the turn, 
    because they did ir already in the previous cycle, so we omit this wait, because in the main 
    loop of the referee we have 2 waits :D 
    ttt.getDriver()->dsm_wait("turn"); */
    
    ttt.setWinToServer(win);
    //we want both results if one is no, the other one cat play
    ttt.getDriver()->dsm_wait("again");
    
    //we have 2 options:
    //1 (the game could continue = reset always)
    ttt.getDriver()->dsm_wait("again");
    
    first = firstTime(ttt, p1, p2);
    resetAll(ttt);
    
    //2 the game will finish after the players colose connection, so only reset if we are going to play again
    /*if(ttt.getAgainFromServer())
    {
        ttt.getDriver()->dsm_wait("again");
        if(ttt.getAgainFromServer())
            resetAll(ttt);
    }*/
    
    return first;
}



int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

	// Hacer lookup dsm.deusto.es 
    // Hacer lookup dsm.deusto.es 
    TicTacToeUtil ttt("127.0.0.1", atoi(argv[1]), argv[2]);
    PracticaCaso::DsmDriver * driver = ttt.getDriver();
	PracticaCaso::DsmData data;
    PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("scores.db");

    int numPlayers, next=1, win = -1; // -1 = not finished, 0 = nobody, 1 = player1, 2 = player2
    int points1, points2, turn = 1; //0 = referee, 1 = player1, 2 = player2 
    bool first = true;
	
    //////////////////create number players///////////////////////
    createNumberOfplayers(ttt, numPlayers);
    /////////////////////create board/////////////////////////////
    ttt.allocBoardInServer();	
    ///////////////////create turn/////////////////////////////
    createTurn(ttt, turn);
    ////////////////create win/////////////////////////
    createWin(ttt, win);
    /////////////////create Again/////////////////////////
    createAgain(ttt);
    ////////////////create Players////////////////////////
    createPlayerNames(ttt);
    

    while(1)
    {
        
        //1 - wait for the turn
        cout << GREEN_BOLD <<"[WAITING FOR TURN...]" << COL_RESET << endl;
        
        //we have to wait 2 times, because the one that we put in the stack counts too... so we delete it
        ttt.getDriver()->dsm_wait("turn");
        ttt.getDriver()->dsm_wait("turn");
        
        //2 - get all the variables
        
        if(first) //if is the first time of the game with new players then...
        {
            
            cout << CYAN_BOLD << "GETTING SCORES IN DATABASE..." << COL_RESET << endl;
            string aux = SQLiteMap->get(ttt.getPlayerName1FromServer());
            
            cout << CYAN_BOLD << aux << COL_RESET << endl;
            
            if(aux.c_str() > 0)
                points1 = atoi(aux.c_str());
            else
                points1 = 0;
            
            aux = SQLiteMap->get(ttt.getPlayerName2FromServer());
            cout << CYAN_BOLD << aux << COL_RESET << endl;
            if(aux.c_str() > 0)
                points2 = atoi(aux.c_str());
            else
                points2 = 0;
            
             cout << CYAN_BOLD << points1 <<" | "<< points2 << COL_RESET << endl;
            
            first = false;
            
        }
        
        ttt.getBoardFromServer();
        turn = ttt.getTurnFromServer();
        
        //3 -check if the game is over or not (1 = not finished, 0 = nobody, 1 = player1, 2 = player2)
        sleep(0.5);
        win = ttt.checkBoardState();
        win++; //this return returns 0 and 1 and we want 1 and 2 if circle or croos have won
        
        switch(win)
        {
            case 0: //no winner
            {
                if(!ttt.checkBoardComplete())
                {
                    win = -1;
                    //4 - the next in the turn is...
                    next = next==2?1:2;
                    //5 - set the variables in the server
                    ttt.setBoardToServer();
                    ttt.setTurnToServer(next);
                    ttt.setWinToServer(win);
                }
                else
                {
                    win = 0;
                    first = again(ttt, win, points1, points2);
                    next = 1;
                }
                break;
            }
            case 1: //circle wins
            {
                points1++;
                points2--;
                first = again(ttt, win, points1, points2);
                next = 1;
                break;
            }
            case 2: //cross wins
            {
                points1--;
                points2++;
                first = again(ttt, win, points1, points2);
                next = 1;
                break;   
            }
            
        }
        
        cout << YELLOW << first << endl;
    }
    
    
    delete SQLiteMap;
    driver->dsm_free("numPlayers");
    driver->dsm_free("board");
    driver->dsm_free("turn");
    driver->dsm_free("win");
    driver->dsm_free("again");
    cout << GREEN_BOLD << "[SLEEPING FOR A SECOND BEFORE FINISHING...]" << COL_RESET << endl;
	sleep(1);
	delete driver;
}

