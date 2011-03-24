#include "NameServer.h"

void usage() {
	cout << "Usage: ProposalDnsClient.bin <server-port> <domain-name-to-resolve>" << endl;
	exit(1);
}

int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

    //////////////NameServer Part/////////////////////
	PracticaCaso::TcpClient *client = new PracticaCaso::TcpClient();
	string dnsName, ipAddressAndPort, user, pass, msg, ip;
    int port, tries = 3, state = 0; //0 = not logged, 1 = login accepted(not the pass yet), 2 = logged, 3 = close
    
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
                    cin >> user;
                    msg = "USER " + user;
                    
                    //go to the 1 state, there we will check the user accepted response
                    state = 1;
                    
                    break;
                }
                case 1:
                {
                    if(msg.find("[USER OK]") == 0)
                    {
                        cout << "[USER ACCEPTED]" << endl;
                        //reset the tries
                        tries = 3;
                        cout << "Insert your password: ";
                        cin >> pass;
                        msg = pass; //this has to be encrypted
                        
                        //go to the 2 state, there we will check the password  accepted response
                        state = 2;
                        
                    }else
                    {
                        cout << "[USER NOT ACCEPTED...]" << endl;
                        cout << "[REMAINING TRIES: "<< tries-1 << " ]"<< endl;
                        
                        //go to the user state again and rest the used try
                        state = 0; 
                        tries--;
                    }
                    break;
                }
                case 2:
                {
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


