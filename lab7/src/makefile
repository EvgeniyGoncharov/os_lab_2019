CC=gcc
CFLAGS=-I.

all: tcpclient udpclient tcpserver udpserver

tcpclient: tcpclient.c
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)

udpclient: udpclient.c
	$(CC) -o udpclient udpclient.c $(CFLAGS)

tcpserver: tcpserver.c
	$(CC) -o tcpserver tcpserver.c $(CFLAGS) $(LDFLAGS)

udpserver: udpserver.c
	$(CC) -o udpserver udpserver.c $(CFLAGS) $(LDFLAGS)


clean:
	rm -f tcpclient udpclient
