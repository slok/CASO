#include "TcpListener.h"
#include "Dsm.h"
#include "colors.h"
#include "ticTacToeUtil.h"




#define BUFFER_SIZE 1024

// we know that global variables are bad idea, but only will be one of each and will be used in some methods





void usage() {
	cout << "Usage: ProposalDsmClient <port> <dns>" << endl;
	exit(1);
}


int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

	// Hacer lookup dsm.deusto.es 
    TicTacToeUtil ttt("127.0.0.1", atoi(argv[1]), argv[2]);
    PracticaCaso::DsmDriver * driver = ttt.getDriver();
    PracticaCaso::DsmData data;
    
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
            //1 - get basic variables(turn...)
            turn = ttt.getTurnFromServer();
            win = ttt.getWinFromServer();
            ttt.getBoardFromServer();
            system("clear"); //*nix
            //system("cls"); //windows
            ttt.drawMatrix(); 
    
            //2 - check if the game has finished(win, or not)
            
            if( win == -1)
            {
                //3 - check if is our turn, if not, wait
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
                    //4 - for the position
                    cout << CYAN_BOLD << "SELECT X:" << COL_RESET;
                    getline(cin, aux);
                    x = atoi(aux.c_str());
                    cout << CYAN_BOLD << "SELECT y:" << COL_RESET;
                    getline(cin, aux);
                    y = atoi(aux.c_str());
                    x-=1; y-=1;
                    
                    //5 - update in the server
                    //set position
                    ttt.board2D[x][y] = player - 1;
                    ttt.setBoardToServer();
                    //set referees turn
                    ttt.setTurnToServer(0);
                    //this is to destroy the notification in the stack that we make when we put the refereees turn
                    ttt.getDriver()->dsm_wait("turn");
                }
            //6 - wait for the turn
            ttt.getDriver()->dsm_wait("turn");
            }
            else
            {
                cout << YELLOW_BOLD <<"GAME OVER!!!" << COL_RESET << endl;
                exitLoop = true;
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
