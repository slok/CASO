
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
        uint8_t *encryptedMsg, *decryptedMsg;
        uint8_t *keyClient = (uint8_t *)"0123456789ABCDEF0123"; //The decrypt key in the client and the DB stored passwords, are different, by this way is "impossible to compare both"
        uint8_t *keyDB = (uint8_t *)"01234567899876543210";
        AESUtil aesCryptDB(keyDB, salt);
        AESUtil aesCryptClient(keyClient, salt);
        
        //check if the first command of the user is quit(if not, this will be checked every loop)
        if(commandVec[0].find("quit") == 0) 
        { 
            //user wants to quit, so we close connection and send confirmation message to the client
            state = 3;
            (this->client)->send("[QUIT]");
        }
        
        while(state < 3)
        {
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
                        clientDecryptedPass = aesCryptClient.decrypt((uint8_t *)commandVec[1].c_str(), &len);
                        
                        //decrypt the BD pass
                        pass = SQLiteMap->get(user);
                        len = pass.size();
                        dbDecryptedPass = aesCryptDB.decrypt((uint8_t *)pass.c_str(), &len);
                        
                        //compare both results
                        if (strncmp((char *)clientDecryptedPass, (char *)dbDecryptedPass, pass.size()+1) != 0)
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
                    cout << "[COMMAND MESSAGE RECEIVED]" << endl;
                    
                    if(commandVec[0].find("echo") == 0) 
                    {
                        cout << "[SOMETHING  MESSAGE RECEIVED]" << endl;
                        msg = "<echo>";
                        for (int i=1; i < commandVec.size(); i++)
                        {
                            msg = msg + " " + commandVec[i];
                        }   
                        msg +=" </echo>";
                    }else 
                    if(commandVec.size() == 3) //if isn't echo or the commands  don't have 2 parameters the command error(for now...)
                    {
                        if(commandVec[0].find("sum") == 0)
                        {
                            int aux = atoi(commandVec[1].c_str()) + atoi(commandVec[2].c_str());
                            intStrAux.str("");
                            intStrAux << commandVec[1] << " + " << commandVec[2] << " = " << aux;
                            msg = ""+ intStrAux.str();
                            
                        }else if(commandVec[0].find("rest") == 0)
                        {
                            int aux = atoi(commandVec[1].c_str()) - atoi(commandVec[2].c_str());
                            intStrAux.str("");
                            intStrAux << commandVec[1] << " - " << commandVec[2] << " = " << aux;
                            msg = ""+ intStrAux.str();
                        }else if(commandVec[0].find("multiply") == 0)
                        {
                            int aux = atoi(commandVec[1].c_str()) * atoi(commandVec[2].c_str());
                            intStrAux.str("");
                            intStrAux << commandVec[1] << " x " << commandVec[2] << " = " << aux;
                            msg = ""+ intStrAux.str();
                        }else if(commandVec[0].find("divide") == 0)
                        {
                            int aux = atoi(commandVec[1].c_str()) / atoi(commandVec[2].c_str());
                            intStrAux.str("");
                            intStrAux << commandVec[1] << " / " << commandVec[2] << " = " << aux;
                            msg = ""+ intStrAux.str();
                        }else if(commandVec[0].find("encrypt") == 0)
                        {
                            //create again the original message
                            string origStr = commandVec[2];
                            for(int i=3; i < commandVec.size(); i++) //the first is the command, seccond the key and third is added in the previous line to start the string
                            {
                                origStr = origStr + " " + commandVec[i];
                            }   
                            
                            //encrypt the message
                            AESUtil aesCryptMsg((uint8_t *)commandVec[1].c_str(), salt);
                            len = origStr.size();
                            encryptedMsg = aesCryptMsg.encrypt((uint8_t *)origStr.c_str(), &len);
                            
                            //check if is encrypted ok
                            len = ((string)(char *)encryptedMsg).size();
                            decryptedMsg = aesCryptMsg.decrypt(encryptedMsg, &len);
                            
                            //compare both results
                            len = ((string)(char *)decryptedMsg).size();
                            if(strncmp((char *)origStr.c_str(), (char *)decryptedMsg, len+1) == 0)
                            {
                                 //create the message
                                msg = "[Original message: " + origStr + " ]\n";
                                msg = msg + "[Encryptation key: " + commandVec[1] + " ]\n";
                                msg =  msg + "[Encrypted message(AES): " + (char *)encryptedMsg + " ]";
                                cout << "[ENCRYPTATION OK]";
                            }
                            else
                            {
                                cout << "[ENCRYPTATION FAILED]";
                                msg = "[ENCRYPTATION ERROR]";
                            }
                           
                        }
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

