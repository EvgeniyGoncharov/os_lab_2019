CC=gcc
CFLAGS=-I.
LDFLAGS=-pthread

all: client server

# Компиляция клиента
client: client.o multmodulo.o
	$(CC) -o client client.o multmodulo.o $(CFLAGS)

# Компиляция сервера
server: server.o multmodulo.o
	$(CC) -o server server.o multmodulo.o $(CFLAGS) $(LDFLAGS)

# Создание объектных файлов
client.o: client.c multmodulo.h
	$(CC) -c -o client.o client.c $(CFLAGS)

server.o: server.c multmodulo.h
	$(CC) -c -o server.o server.c $(CFLAGS)

multmodulo.o: multmodulo.c multmodulo.h
	$(CC) -c -o multmodulo.o multmodulo.c $(CFLAGS)

clean:
	rm -f client server *.o
