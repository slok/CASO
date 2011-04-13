/*
Copyright (C) 2011 Iraide Diaz (Sharem)
Copyright (C) 2011 Xabier Larrakoetxea (slok)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
