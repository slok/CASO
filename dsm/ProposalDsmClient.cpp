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

void setAgain(TicTacToeUtil ttt, bool again)
{
    //store turn for the first one
    try {
        ttt.getDriver()->dsm_put("again", (void *)&again, sizeof(again));
    } catch (DsmException dsme) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"again\", again, " << sizeof(again) << ")): " << dsme << COL_RESET << endl;
        ttt.getDriver()->dsm_free("again");
        exit(1);
    }
}
void leaveGame(TicTacToeUtil ttt)
{
    int numPlayers;
    bool playersGet = false;
	PracticaCaso::DsmData data;
    
    //get the player
    cout << RED_BOLD << "leaving the game" << COL_RESET << endl;
    while (!playersGet) {
		try {
			cout << GREEN_BOLD << "[GETTING NUMBER OF PLAYERS: " << ttt.getDriver()->get_nid() << " ]"<< COL_RESET << endl;
            data = ttt.getDriver()->dsm_get("numPlayers");
            numPlayers = *((int *)data.addr);
			playersGet = true;
		} catch (DsmException dsme) {
			cerr << RED_BOLD << "ERROR: Referee not connected, waiting..." << dsme << COL_RESET<< endl;
			ttt.getDriver()->dsm_wait("numPlayers");
		}
	}
    
    //set the player number
    numPlayers--;
    
    try 
    {
        cout << GREEN_BOLD << "[DECREMENTING NUMBER OF PLAYERS: " << ttt.getDriver()->get_nid() << " ]"<< COL_RESET << endl;
        ttt.getDriver()->dsm_put("numPlayers", (void *)&numPlayers, sizeof(numPlayers));
    } catch (...) {
        cerr << RED_BOLD << "ERROR: dsm_put(\"numPlayers\", (void *)&numPlayers, sizeof(numPlayers))" << COL_RESET << endl;
        exit(1);
    }
    
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
    numPlayers = ttt.getNumPlayersFromServer();
    
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
                    bool ask = true; 
                    while(ask)
                    {
                        cout << CYAN_BOLD << "SELECT X:" << COL_RESET;
                        getline(cin, aux);
                        x = atoi(aux.c_str());
                        cout << CYAN_BOLD << "SELECT y:" << COL_RESET;
                        getline(cin, aux);
                        y = atoi(aux.c_str());
                        x-=1; y-=1;
                        
                        //player -1 because we have player1 = 1 and player2 = 2 and we want circle = 0 and cross = 1
                        //if the assignation returns false then, there was an error
                        if(ttt.setPositionInBoard(player-1,x ,y ))
                            ask = false;
                        else
                            cout << RED_BOLD << "THAT POSITION WASN'T EMPTY OR DOESN'T EXIST, TRY AGAIN PLEASE..." << COL_RESET << endl;
                    }
                    //5 - update in the server
                    
                    ttt.setBoardToServer();
                    //set referees turn
                    ttt.setTurnToServer(0);
                    //this is to destroy the notification in the stack that we make when we put the refereees turn
                    ttt.getDriver()->dsm_wait("turn");
                }
            //6 - wait for the turn
            ttt.getDriver()->dsm_wait("turn");
            }
            else //check the other cases that are after finished the game
            {
                switch(win)
                {
                    case -1:
                    {
                         cout << YELLOW_BOLD << "NO BODY HAS WON THE GAME :(" << COL_RESET << endl;
                        break;
                    }
                    case 1:
                    {
                        cout << YELLOW_BOLD << "PLAYER 1 HAS WON" << COL_RESET << endl;
                        break;
                    }
                    case 2:
                    {
                        cout << YELLOW_BOLD <<"PLAYER 2 HAS WON" << COL_RESET << endl;
                        break;
                    }
                }
                cout << YELLOW_BOLD <<"GAME OVER!!!" << COL_RESET << endl;
                
                bool ask=true;
                string cont;
                while(ask)
                {
                    cout << MAGENTA_BOLD <<"DO YOU WANT TO CONTINUE PLAYING? (y/n): " << COL_RESET << endl;
                    getline(cin, cont);
                    
                    //if one of the players already has say that don't want to continue, we can't continue too
                    if( (!ttt.getAgainFromServer()) && (cont.find("y") == 0) )
                    {
                        cout << RED_BOLD << "Sorry, the other player didn't want to play" << COL_RESET <<endl;
                        cont = "n";
                    }
                       
                    if(cont.find("y") == 0)
                    {
                        ttt.setAgainToServer(true);
                        ask = false;
                        
                        //only wait if we have to continue playing
                        ttt.getDriver()->dsm_wait("turn");
                        
                        //stop a little, the threads are very fast :S 
                        cout << GREEN_BOLD << "[WAITING 2 SECONDS, PLEASE BE PATIENT...]" << COL_RESET << endl;
                        sleep(2);
                        
                        //now we have to check if the other player has ansered no or yes (if he has gone then, we get out too)
                        exitLoop  = (ttt.getNumPlayersFromServer() == 1 )?true:false;
                        
                        
                    }
                    else if(cont.find("n") == 0)
                    {
                        ttt.setAgainToServer(false);
                        ask = false;
                        exitLoop  = true;
                    }
                }
                //We have set in the turn in the previous cycle
                /*
                //if the turn has been set by the other player, we don't have to put
                if(ttt.getTurnFromServer() != 0)
                {
                    //give turn to server
                    ttt.setTurnToServer(0);
                    //destroy our set notification
                    ttt.getDriver()->dsm_wait("turn");
                }*/
            }
        }
    }
    else
        cout << RED_BOLD << "ERROR: Two players connected..." << COL_RESET<< endl;
    
    cout << GREEN_BOLD << "[SLEEPING FOR A SECOND BEFORE FINISHING...]" << COL_RESET << endl;
    //if we don't sleep, referee and client colapse between them
	sleep(1);
	leaveGame(ttt);
	
    delete driver;
}
