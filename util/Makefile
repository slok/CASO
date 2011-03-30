# Makefile
# author: dipina@eside.deusto.es
# g++ -shared  -o TcpListener.o libprueba.so
CC=g++ -fPIC

LDFLAGS=-shared -lm -lpthread 


main: libesidesocket.so libesidethread.so libesidesh.so  libesidesqlite.so libcasoaes.so libcasotictactoe.so insertPasswordDB.bin getPasswordsDB.bin
#SQLiteMap.bin

getPasswordsDB.bin: getPasswordsDB.cpp
	g++ -L./ -lesidesqlite -lcasoaes getPasswordsDB.cpp -o getPasswordsDB.bin

insertPasswordDB.bin: insertPasswordDB.cpp
	g++ -L./ -lesidesqlite -lcasoaes insertPasswordDB.cpp -o insertPasswordDB.bin

libesidesocket.so: TcpListener.o
	$(CC) $(LDFLAGS) TcpListener.o -o libesidesocket.so

libesidethread.so: Thread.o
	$(CC) $(LDFLAGS) Thread.o -o libesidethread.so

libesidesh.so: MySh.o popen.o
	$(CC) $(LDFLAGS) MySh.o popen.o -o libesidesh.so

libcasoaes.so  :  AESUtil.o
	$(CC) $(LDFLAGS) AESUtil.o -lcrypto -o  libcasoaes.so
	
libcasotictactoe.so : ticTacToeUtil.o
	$(CC) $(LDFLAGS) ticTacToeUtil.o -o  libcasotictactoe.so

ticTacToeUtil.o: ticTacToeUtil.cpp ticTacToeUtil.h colors.h
	$(CC) -c ticTacToeUtil.cpp

AESUtil.o: AESUtil.cpp AESUtil.h types.h
	$(CC) -c AESUtil.cpp
	
libesidesqlite.so :  SQLiteMap.o
	$(CC) $(LDFLAGS) SQLiteMap.o -lsqlite3 -ldl -o  libesidesqlite.so

SQLiteMap.o: SQLiteMap.cc SQLiteMap.h
	$(CC) -c SQLiteMap.cc

#SQLiteMap.bin: SQLiteMap.cc SQLiteMap.h
#	g++ -lsqlite3 -ldl SQLiteMap.cc -o SQLiteMap.bin

Thread.o: Thread.cc Thread.h
	$(CC) -c Thread.cc

TcpListener.o: TcpListener.cc TcpListener.h
	$(CC) -c TcpListener.cc
	
MySh.o: MySh.cc MySh.h
	$(CC) -c MySh.cc
	
popen.o: popen.cc popen.h
	$(CC) -c popen.cc
	
clean:
	rm *.o
	rm *.so
	rm *.bin
	rm *.dll

