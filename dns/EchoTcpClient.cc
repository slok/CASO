// EchoTcpClient.cc
// author: dipina@eside.deusto.es

#include "TcpListener.h"

int main(int argc, char** argv) {

	// MODIFICATION 2.3.3
    PracticaCaso::TcpClient * client = new PracticaCaso::TcpClient();
    
    if (argc != 3)
        cout << "Usage: EchoTcpClient <port> <ip adress>\n";
    else
    {
        client->connect(argv[2], atoi(argv[1]));
        client->send("echo.deusto.es");
        string response = "";
        string ip = "";
        int port = 0;
        
        response = client->receive();
        
        if(response.find("ERROR") != 0)
        {
            ip = response.substr(0, response.find(":"));
            port = atoi((response.substr(response.find(":")+1)).c_str());
            client->close();
            client->connect(ip,port);
            
            string msg = "Hello CASO students";
            client->send(msg);
            cout << "Message sent: " << msg << endl;
            msg = client->receive();
            cout << "Message received: " << msg << endl;
        }
        else //error
        {
            cout << "ERROR, no se encuentra\n";
        }
        
        client->close();
        delete client;
    }
// MODIFICATION 2.3.5
/*    for (int i=0; i<100; i++) {
    PracticaCaso::TcpClient * client = new PracticaCaso::TcpClient();
    cout << "Petición: " << i << endl;
    client->connect("127.0.0.1", 4321);
    string msg = "¡Hello CASO students!";
    client->send(msg);
    cout << "Message sent: " << msg << endl;
    msg = client->receive();
    cout << "Message received: " << msg << endl;
    client->close();	
    delete client;

    }
*/
}

