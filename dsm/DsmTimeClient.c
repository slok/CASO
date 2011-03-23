extern "C" {
	#include <sys/time.h>
}

#include "TcpListener.h"
#include "Dsm.h"

#define BUFFER_SIZE 1024

void usage() {
	cout << "Usage: DsmTimeClient <port> <dns>" << endl;
	exit(1);
}

int main(int argc, char** argv) {

	if (argc != 3) {
		usage();
	}

	// Hacer lookup dsm.deusto.es 
	PracticaCaso::DsmDriver * driver = new PracticaCaso::DsmDriver("127.0.0.1", atoi(argv[1]), argv[2]);
    PracticaCaso::DsmData data;
    cout << "[OK] Node created with " << driver->get_nid() << " nid"<< endl;
    
    struct timeval first, second, lapsed;
    struct timezone tzp;
    gettimeofday (&first, &tzp);
    
    while(1)
    {        
        gettimeofday (&second, &tzp);
        
        if (first.tv_usec > second.tv_usec) {
            second.tv_usec += 1000000;
            second.tv_sec--;
        }
        
        lapsed.tv_sec = second.tv_sec - first.tv_sec;
        
        if (lapsed.tv_sec > 1)
        {
            gettimeofday (&first, &tzp);
            gettimeofday (&second, &tzp);
            if (first.tv_usec > second.tv_usec) {
                second.tv_usec += 1000000;
                second.tv_sec--;
            }
            //cout << "ha pasado un segundo" << endl;
            try 
            {
                data = driver->dsm_get("GLOBAL_TIMESTAMP"); 
                cout << "Current time: " << (char*)data.addr << endl;
            } catch (DsmException dsme) 
            {
                cerr << "ERROR in dsm_get(\"GLOBAL_TIMESTAMP\") - waiting" << dsme << endl;
                driver->dsm_wait("GLOBAL_TIMESTAMP");
            }
        }
    }
    
}
