# Makefile
# author: dipina@eside.deusto.es
CC=g++
LDFLAGS=-L../util -lm -lpthread -lesidesocket -lesidethread -lesidesqlite
INCLUDE=-I../util

main: EchoTcpListener EchoTcpClient NameServer NameClient ProposalDnsClient.bin ProposalDnsServer.bin

ProposalDnsServer.bin: ProposalDnsServerThread.o  ProposalDnsServer.o
	$(CC) ProposalDnsServerThread.o ProposalDnsServer.o  $(LDFLAGS) -lcasoaes -o ProposalDnsServer.bin

ProposalDnsClient.bin: ProposalDnsClient.o
	$(CC) ProposalDnsClient.o  $(LDFLAGS) -lcasoaes -o ProposalDnsClient.bin

EchoTcpListener: EchoServerThread.o EchoTcpListener.o
	$(CC) EchoServerThread.o EchoTcpListener.o $(LDFLAGS) -o EchoTcpListener

EchoTcpClient: EchoTcpClient.o
	$(CC) EchoTcpClient.o $(LDFLAGS) -o EchoTcpClient

NameServer: NameServer.o
	$(CC) NameServer.o $(LDFLAGS) -o NameServer

NameClient: NameClient.o
	$(CC) NameClient.o $(LDFLAGS) -o NameClient

ProposalDnsServerThread.o: ProposalDnsServerThread.cpp  ProposalDnsServerThread.h
	$(CC) $(INCLUDE) -c ProposalDnsServerThread.cpp

ProposalDnsServer.o: ProposalDnsServer.cpp 
	$(CC) $(INCLUDE) -c ProposalDnsServer.cpp

ProposalDnsClient.o: ProposalDnsClient.cpp 
	$(CC) $(INCLUDE) -c ProposalDnsClient.cpp
	
EchoServerThread.o: EchoServerThread.cc EchoServerThread.h
	$(CC) $(INCLUDE) -c EchoServerThread.cc

EchoTcpListener.o: EchoTcpListener.cc
	$(CC) $(INCLUDE) -c EchoTcpListener.cc

EchoTcpClient.o: EchoTcpClient.cc
	$(CC) $(INCLUDE) -c EchoTcpClient.cc

NameServer.o: NameServer.cc NameServer.h
	$(CC) $(INCLUDE) -c NameServer.cc

NameClient.o: NameClient.cc 
	$(CC) $(INCLUDE) -c NameClient.cc

clean:
	rm *.o
	rm EchoTcpListener EchoTcpClient NameServer NameClient
	rm *.exe
	rm *.bin
