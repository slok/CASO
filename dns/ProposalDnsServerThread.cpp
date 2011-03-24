
#include "ProposalDnsServerThread.h"
#include "SQLiteMap.h" 

namespace PracticaCaso
{
    ProposalDnsServerThread::~ProposalDnsServerThread() {
        delete this->client;
    }

    void ProposalDnsServerThread::run() 
    {
        string msg = (this->client)->receive();
        char seps[] = " ";
        string token; 
        PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("logins.db");
        int state = 0; //0 = not logged, 1 = login accepted(not the pass yet), 2 = logged, 3 = close 
        
        while(state < 3)
        {
            switch(state)
            {
                case 0: //not logged
                {
                    if(msg.find("USER") == 0) 
                    {
                        cout << "[USER  MESSAGE RECEIVED]" << endl;
                        token = strtok((char *)msg.c_str(), seps);
                        token = strtok(NULL, seps); //get the second part after " "(blank)
                        
                        //check if the user exists on the DB
                        if(SQLiteMap->get(token).size() > 0)
                        {    
                            state = 1;//now the client can enter the password
                            cout << "[USER  LOGIN OK]" << endl;
                            msg = "[USER OK]";
                        }
                        else
                        {
                            cout << "[USER  LOGIN ERROR]" << endl;
                            msg =  "[USER ERROR]";
                        }
                    }
                    break;
                }
                case 1: //login accepted (not the pass yet)
                {
                    cout << "[PASS  MESSAGE RECEIVED]" << endl;
                    msg = "[CLOSE]";
                    state = 3;
                    break;
                }
                case 2: //logged
                {
                    cout << "[SOMETHING  MESSAGE RECEIVED]" << endl;
                    msg = "[CLOSE]";
                    state = 3;
                    break;
                }
            }
            //send message and wait for response
            (this->client)->send(msg);
            //if we changed the state to exit/close, we don't have to wait for a response
            if(state != 3)
                msg = (this->client)->receive();

        }
        if (state == 3) // close connection
        {
            cout << "[CONNECTION CLOSED WITH SOCKET... DESTROYING THREAD...]" << endl;
            (this->client)->close();
        }
    }
}

