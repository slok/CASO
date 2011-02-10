# Makefile
# author: dipina@eside.deusto.es
# g++ -shared  -o TcpListener.o libprueba.so
CC=g++ -fPIC
LDFLAGS=-shared -lm -lpthread 

main: libesidesocket.so libesidethread.so libesidesh.so  

libesidesocket.so: TcpListener.o
	$(CC) $(LDFLAGS) TcpListener.o -o libesidesocket.so

libesidethread.so: Thread.o
	$(CC) $(LDFLAGS) Thread.o -o libesidethread.so

libesidesh.so: MySh.o popen.o
	$(CC) $(LDFLAGS) MySh.o popen.o -o libesidesh.so

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

