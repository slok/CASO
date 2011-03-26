#include "NameServer.h"
#include "AESUtil.h"

void usage() {
	cout << "Usage: ProposalDnsClient.bin <server-port> <domain-name-to-resolve>" << endl;
	exit(1);
}

vector<string> extractCommandAndArgs(string message)
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
    vector<string> commandVec;
    
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
	string dnsName, ipAddressAndPort, msg, ip, aux;
    int port, length;
    vector<string> commandVec;
    bool exit = false, badCommand = true;
    
   
    
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
        while(!exit)
        { 
            //reset bad command boolean
            badCommand = true;
            
            while(badCommand) //if the command is bad then loop until is a good constructed command
            {
                //insert command
                cout << "Insert your command: ";
                getline(cin, msg);
                
                //copy the string to have the original msg intact, the strtoken brokens
                strcpy((char *)aux.c_str(),(char *)msg.c_str());
                //split command
                commandVec = extractCommandAndArgs(aux);
                
                if ((commandVec.size() >= 2) || (commandVec[0].find("quit") == 0))
                    badCommand = false;
                else if(commandVec[0].find("help") == 0) //prompt help
                {
                    cout << "######################" << endl;
                    cout << "use: command <arg>" << endl;
                    cout << "commands:" << endl;
                    cout << "     *help" << endl;
                    cout << "     *user <username>:" << endl;
                    cout << "     *pass <password>:" << endl;
                    cout << "     *echo <anything>:" << endl;
                    cout << "     *quit" << endl;
                    cout << "######################" << endl;
                    
                }
            }
            
            //if is the command then ecript
            if (commandVec[0].find("pass") == 0)
            {    
                //encrypt password 
                msg = encryptMsg(commandVec[1]);
                msg = "pass " + msg;
            }else if(commandVec[0].find("quit") == 0) //we want to quit, we send to the server and wait response to close connection
                exit = true;
             
            //send and wait response
            client->send(msg);
            msg = client->receive();
            
            cout << msg << endl;
            
            if(msg.find("[QUIT]") == 0)
                exit = true;
        }
	}
    //finish
    client->close();
    delete client;
    
}


