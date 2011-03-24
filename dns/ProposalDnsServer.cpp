#include "TcpListener.h"
#include "ProposalDnsServerThread.h"

extern "C" {
	#include </usr/include/signal.h>
}

// global variable
PracticaCaso::TcpListener * listener_pointer;

// function called when CTRL-C is pressed
void ctrl_c(int)
{
    cout << "CTRL-C was pressed..." << endl;
	listener_pointer->stop();
}

void usage() {
	cout << "Usage: ProposalDnsServer.bin [port]" << endl;
	exit(1);
}

int main(int argc, char** argv) {
	int port;
    if (argc > 2) {
		usage();
	}else if(argc == 1)
        port = 4321;
    else
        port = atoi(argv[1]);
    signal(SIGINT,ctrl_c);
	PracticaCaso::TcpListener listener(port);
	listener_pointer = &listener;
	cout << "TcpListener created: " << listener << endl;
	listener.start();

    while(1)
    {
        PracticaCaso::TcpClient* client = listener.acceptTcpClient();
        PracticaCaso::ProposalDnsServerThread* t = new PracticaCaso::ProposalDnsServerThread(client);
        t->start();
    }
    
	cout << "Finishing server ..." << endl;
	listener.stop();
}
