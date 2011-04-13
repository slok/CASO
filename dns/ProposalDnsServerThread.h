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
