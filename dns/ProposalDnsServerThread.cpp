
#include "ProposalDnsServerThread.h"


namespace PracticaCaso
{
    ProposalDnsServerThread::~ProposalDnsServerThread() {
        delete this->client;
    }
    vector<string> ProposalDnsServerThread::extractCommandAndArgs(string message)
    {
        char seps[] = " ";
        vector<string> vec;
        char *token;
        
        token = strtok((char *)message.c_str(), seps);
            
        while (token != NULL) 
        {
            vec.push_back((string)token); 
            token = strtok(NULL, seps);        
        }
        return vec;
    }
    void ProposalDnsServerThread::run() 
    {
        string msg = (this->client)->receive();
        vector<string> commandVec = extractCommandAndArgs(msg);// in the vector we will store the commands and arguments
        string user, pass; 
        PracticaCaso::SQLiteMap * SQLiteMap = new PracticaCaso::SQLiteMap("logins.db");
        int tries = 3, state = 0; //0 = not logged, 1 = login accepted(not the pass yet), 2 = logged, 3 = close 
        stringstream intStrAux;
        
        
        //neccesary variables for AES encryptation
        uint32_t salt[] = {12345, 54321}; //to salt the AES. mmmmmmm... tasty :D
        int len;
        uint8_t *clientDecryptedPass;
        uint8_t *dbDecryptedPass;
        uint8_t *key = (uint8_t *)"01234567899876543210";
        AESUtil aesCrypt(key, salt);
        
        //check if the first command of the user is quit(if not, this will be checked every loop)
        if(commandVec[0].find("quit") == 0) 
        { 
            //user wants to quit, so we close connection and send confirmation message to the client
            state = 3;
            (this->client)->send("[QUIT]");
        }
        
        while(state < 3)
        {
            cout << commandVec.size() << endl;
            switch(state)
            {
                case 0: //not logged
                {
                    if(commandVec[0].find("user") == 0) 
                    {
                        cout << "[USER MESSAGE RECEIVED]" << endl;
                    
                        //check if the user exists on the DB
                        if(SQLiteMap->get(commandVec[1]).size() > 0)
                        {    
                            user = commandVec[1];
                            state = 1;//now the client can enter the password
                            tries = 3;//reset the tries for the password
                            cout << "[USER  LOGIN OK]" << endl;
                            msg = "[USER OK]";
                        }
                        else
                        {
                            //rest one try;
                            tries --;
                            cout << "[USER  LOGIN ERROR]" << endl;
                            //clear the stringstream and use to convert integer to string
                            intStrAux.str("");
                            intStrAux << tries;
                            msg =  "[USER ERROR] tries remaining: " + intStrAux.str();
                        }
                    }
                    else
                        msg =  "[COMMAND ERROR]";
                    break;
                }
                case 1: //login accepted (not the pass yet)
                {
                    if(commandVec[0].find("pass") == 0) 
                    {
                        cout << "[PASS  MESSAGE RECEIVED]" << endl;
                       
                        //decrypt the pass message
                        len = commandVec[1].size();
                        clientDecryptedPass = aesCrypt.decrypt((uint8_t *)commandVec[1].c_str(), &len);
                        
                        //decrypt the BD pass
                        pass = SQLiteMap->get(user);
                        len = pass.size();
                        dbDecryptedPass = aesCrypt.decrypt((uint8_t *)pass.c_str(), &len);
                        
                        //compare both results
                        if (strncmp((char *)clientDecryptedPass, (char *)dbDecryptedPass, pass.size()+1))
                        {
                            tries--;
                            cout << "[AUTENTICATION FAILED]" << endl;
                            //clear the stringstream and use to convert integer to string
                            intStrAux.str("");
                            intStrAux << tries;
                            msg = "[PASS ERROR] tries remaining: "+ intStrAux.str();
                        }
                        else
                        {
                            cout << "[AUTENTICATION OK]" << endl;
                            msg = "[PASS OK]";
                            state = 2;
                            tries = 3;
                        }
                    }
                    else
                        msg =  "[COMMAND ERROR]";
                    break;
                }
                case 2: //logged
                {
                    if(commandVec[0].find("echo") == 0) 
                    {
                        cout << "[SOMETHING  MESSAGE RECEIVED]" << endl;
                        msg = "<echo> "+ msg +" </echo>";
                    }
                    else
                        msg =  "[COMMAND ERROR]";
                    break;
                }
            }
            
            //check the tries(3) if are 0 then close
            if(tries <= 0)
            {
                msg = "[QUIT][3 TRIES USED]";
                state = 3;
            }
            
            //send message and wait for response
            (this->client)->send(msg);
            
            //if we changed the state to exit/close, we don't have to wait for a response
            if(state != 3)
            {
                msg = (this->client)->receive();
                //get command of the message
                commandVec = extractCommandAndArgs(msg);
                
                if(commandVec[0].find("quit") == 0) 
                { 
                    //user wants to quit, so we close connection and send confirmation message to the client
                    state = 3;
                    (this->client)->send("[QUIT]");
                    
                }
            }
        }
        if (state == 3) // close connection
        {
            cout << "[CONNECTION CLOSED WITH SOCKET... DESTROYING THREAD...]" << endl;
            (this->client)->close();
        }
    }
}

