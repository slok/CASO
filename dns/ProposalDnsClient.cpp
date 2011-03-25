#include "NameServer.h"
#include "AESUtil.h"

void usage() {
	cout << "Usage: ProposalDnsClient.bin <server-port> <domain-name-to-resolve>" << endl;
	exit(1);
}

string encryptMsg(string textDec)
{
    //neccesary variables for AES encryptation
    uint32_t salt[] = {12345, 54321}; //to salt the AES. mmmmmmm... tasty :D
    uint8_t *auxPass = NULL;
    int len;
    uint8_t *cipherPass;
    uint8_t *key = (uint8_t *)"01234567899876543210";
    AESUtil aesCrypt(key, salt);
    string msg;
    
    //encrypt the message
    len = textDec.size();
    auxPass = (uint8_t *)textDec.c_str();
    cipherPass = aesCrypt.encrypt(auxPass, &len);
    //cast unsigned char to char and the to string :s, double casting yai!
    msg = (string)(char *)cipherPass;
    return msg;

}

int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

    //general variables
	PracticaCaso::TcpClient *client = new PracticaCaso::TcpClient();
	string dnsName, ipAddressAndPort, user, pass, msg, ip;
    int port, tries = 3, state = 0; //0 = not logged, 1 = login accepted(not the pass yet), 2 = logged, 3 = close
    
   
    
    //////////////NameServer Part/////////////////////
    client->connect("127.0.0.1", atoi(argv[1])); //connect with the name server
	
    // Lookup in NameServer(send the domain that we want)
	dnsName = argv[2];
	client->send(dnsName);   
	
    //receive the response
    ipAddressAndPort = client->receive();
    client->close();
    
    //connect to the proposal server
    ip = ipAddressAndPort.substr(0, ipAddressAndPort.find(":"));
    port = atoi((ipAddressAndPort.substr(ipAddressAndPort.find(":")+1)).c_str());
    client->connect(ip,port);
    
	if (ipAddressAndPort.find("ERROR") == 0) {
		cout << "The DNS name " << dnsName << " could not be resolved." << endl;
	} 
    else 
    {//////////////////Server Part/////////////////////
        while((state < 3) && (tries > 0))//if 3 tries are used then close
        { 
            switch(state)
            {
                case 0:
                {
                    cout << "Insert your Username(Login): ";
                    getline(cin, user);
                    msg = "USER " + user;
                    
                    //go to the 1 state, there we will check the user accepted response
                    state = 1;
                    
                    break;
                }
                case 1:
                {
                    if(msg.find("[USER ERROR]") == 0)
                    {
                        cout << "[USER NOT ACCEPTED...]" << endl;
                        cout << "[REMAINING TRIES: "<< tries-1 << " ]"<< endl;
                        
                        //go to the user state again and rest the used try
                        state = 0; 
                        tries--;
                        
                        
                        
                    }else //user accepted
                    {
                        //if is the first time in this state, we have to reset the tries, otherwise we come 
                        //from the state 2, so we have not to reset the tries
                        if(msg.find("[USER OK]") == 0)
                        {
                            cout << "[USER ACCEPTED]" << endl;
                            //reset the tries
                            tries = 3;
                        }
                        cout << "Insert your password: ";
                        getline(cin, pass);
                        //encrypt password 
                        msg = encryptMsg(pass);
                        msg = "PASS " + msg;
                        //go to the 2 state, there we will check the password  accepted response and come again or not
                        state = 2;
                        
                    }

                    break;
                }
                case 2:
                {
                    if(msg.find("[PASS ERROR]") == 0)
                    {
                        cout << "[USER AND PASS NOTAUTENTICATED]" << endl;
                        //reset the tries and go to pass state(not logged = 1)
                        state = 1; 
                        tries--;
                        
                    }
                    else
                    {
                        //if is the first time after loogged in, reset tries and prompt message
                        //if not, is the logged state.
                        if(msg.find("[PASS ERROR]") == 0)
                        {
                            cout << "[USER AND PASS AUTENTICATED]" << endl;
                            //reset the tries
                            tries = 3;
                        }
                        //start logged mode with echo server
                        cout << msg << endl;//echo the servers response
                        cout << "Insert your word to echo by the server: ";
                        getline(cin, msg);
                        
                    }
                    break;
                }
            }
            //send and wait response
            client->send(msg);
            msg = client->receive();
            
            //if exit message then close
            if(msg.find("[CLOSE]") == 0)
            {
                cout << "[CLOSE MESSAGE RECEIVED]" << endl;
                state = 3;
            }
        }
	}
    
    if(state == 3)
    {
        client->close();
        delete client;
    }
}


