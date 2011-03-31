# Makefile
# author: dipina@eside.deusto.es
CC=g++ -fPIC
LDFLAGS=-L../util -lm -lpthread -lesidesocket -lesidethread
INCLUDE=-I../util

main: libesidedsm.so DsmServer DsmTest MatrixEvenAdder MatrixOddAdder DsmTimeServer DsmTimeClient ProposalDsmClient.bin ProposalDsmReferee.bin

libesidedsm.so: Dsm.o
	$(CC) -shared $(LDFLAGS) Dsm.o -o libesidedsm.so
	cp libesidedsm.so libesidedsm.dll
	cp ./libesidedsm* ../util
	
Dsm.o: Dsm.cc Dsm.h DsmTypes.h
	$(CC) $(INCLUDE) -c Dsm.cc
	
DsmServer: DsmServer.o
	$(CC)  DsmServer.o $(LDFLAGS) -o DsmServer

DsmTest: DsmTest.o
	$(CC) DsmTest.o $(LDFLAGS) -L. -lesidedsm -o DsmTest
	
MatrixEvenAdder: MatrixEvenAdder.o
	$(CC) MatrixEvenAdder.o $(LDFLAGS) -L. -lesidedsm -o MatrixEvenAdder
	
MatrixOddAdder: MatrixOddAdder.o
	$(CC) MatrixOddAdder.o $(LDFLAGS) -L. -lesidedsm -o MatrixOddAdder

DsmTimeServer: DsmTimeServer.o
	$(CC) DsmTimeServer.o $(LDFLAGS) -L. -lesidedsm -o DsmTimeServer
	
DsmTimeClient: DsmTimeClient.o
	$(CC) DsmTimeClient.o $(LDFLAGS) -L. -lesidedsm -o DsmTimeClient

ProposalDsmReferee.bin: ProposalDsmReferee.o
	$(CC) ProposalDsmReferee.o $(LDFLAGS) -L. -lesidedsm -lcasotictactoe -o ProposalDsmReferee

ProposalDsmClient.bin: ProposalDsmClient.o
	$(CC) ProposalDsmClient.o $(LDFLAGS) -L. -lesidedsm -lcasotictactoe -o ProposalDsmClient

DsmServer.o: DsmServer.cc DsmServer.h DsmTypes.h
	$(CC) $(INCLUDE) -c DsmServer.cc

DsmTest.o: DsmTest.cc
	$(CC) $(INCLUDE) -c DsmTest.cc

MatrixEvenAdder.o: MatrixEvenAdder.cc
	$(CC) $(INCLUDE) -c MatrixEvenAdder.cc
	
MatrixOddAdder.o: MatrixOddAdder.cc
	$(CC) $(INCLUDE) -c MatrixOddAdder.cc
	
DsmTimeServer.o: DsmTimeServer.c
	$(CC) $(INCLUDE) -c DsmTimeServer.c

DsmTimeClient.o: DsmTimeClient.c
	$(CC) $(INCLUDE) -c DsmTimeClient.c

ProposalDsmReferee.o: ProposalDsmReferee.cpp
	$(CC) $(INCLUDE) -c ProposalDsmReferee.cpp

ProposalDsmClient.o: ProposalDsmClient.cpp
	$(CC) $(INCLUDE) -c ProposalDsmClient.cpp

clean:
	rm *.o
	#rm *.dll
	rm DsmServer DsmTest MatrixEvenAdder MatrixOddAdder libesidedsm.so 
	rm *.bin
