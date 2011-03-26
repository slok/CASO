// ProposalDnsServerThread.h
// author: dipina@eside.deusto.es
#ifndef __PROPOSALDNSSERVERTHREAD_H
#define __PROPOSALDNSSERVERTHREAD_H

#include "Thread.h"
#include "TcpListener.h"
#include "AESUtil.h"
#include "SQLiteMap.h" 
#include <sstream> 

namespace PracticaCaso
{
	class ProposalDnsServerThread: public Thread {
		private:
			TcpClient* client;
			void run();
            vector<string> extractCommandAndArgs(string message);
		public:
			ProposalDnsServerThread(TcpClient* c): client(c) {}
			~ProposalDnsServerThread();
	};
};
#endif
