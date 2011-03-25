
#include "ProposalDnsServerThread.h"


namespace PracticaCaso
{
    ProposalDnsServerThread::~ProposalDnsServerThread() {
        delete this->client;
    }

    void ProposalDnsServerThread::run() 
    {
        string msg = (this->client)->receive();
        char seps[] = " ";
        string token, user, pass; 
        PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("logins.db");
        int state = 0; //0 = not logged, 1 = login accepted(not the pass yet), 2 = logged, 3 = close 
        
        //neccesary variables for AES encryptation
        uint32_t salt[] = {12345, 54321}; //to salt the AES. mmmmmmm... tasty :D
        int len;
        uint8_t *clientDecryptedPass;
        uint8_t *dbDecryptedPass;
        uint8_t *key = (uint8_t *)"01234567899876543210";
        AESUtil aesCrypt(key, salt);
        
        while(state < 3)
        {
            switch(state)
            {
                case 0: //not logged
                {
                    cout << "[USER  MESSAGE RECEIVED]" << endl;
                    if(msg.find("USER") == 0) 
                    {
                        token = strtok((char *)msg.c_str(), seps);
                        token = strtok(NULL, seps); //get the second part after " "(blank)
                        
                        //check if the user exists on the DB
                        if(SQLiteMap->get(token).size() > 0)
                        {    
                            user = token;
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
                    else
                        msg =  "[COMMAND ERROR]";
                    break;
                }
                case 1: //login accepted (not the pass yet)
                {
                    cout << "[PASS  MESSAGE RECEIVED]" << endl;
                   
                    token = strtok((char *)msg.c_str(), seps);
                    msg = strtok(NULL, seps); //get the second part after " "(blank) 
                    
                    //decrypt the pass message
                    len = msg.size();
                    clientDecryptedPass = aesCrypt.decrypt((uint8_t *)msg.c_str(), &len);
                    
                    //decrypt the BD pass
                    pass = SQLiteMap->get(user);
                    len = pass.size();
                    dbDecryptedPass = aesCrypt.decrypt((uint8_t *)pass.c_str(), &len);
                    
                    //compare both results
                    if (strncmp((char *)clientDecryptedPass, (char *)dbDecryptedPass, pass.size()+1))
                    {
                        cout << "[AUTENTICATION FAILED]" << endl;
                        msg = "[PASS ERROR]";
                        //msg = "[CLOSE]";
                        //state = 3;
                    }
                    else
                    {
                        cout << "[AUTENTICATION OK]" << endl;
                        msg = "[PASS OK]";
                        state = 2;
                    }
                    
                    break;
                }
                case 2: //logged
                {
                    cout << "[SOMETHING  MESSAGE RECEIVED]" << endl;
                    
                    //if the user wants to quit we have to go to state 3, if not, do a normal echo
                    if(msg.find("QUIT") == 0) 
                    { 
                        msg = "[CLOSE]";
                        state = 3;
                    }
                    else
                        msg = "<echo> "+ msg +" </echo>";
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

