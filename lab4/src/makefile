CC=gcc
CFLAGS=-I.

all: parallel_min_max clean process_memory

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max parallel_min_max.o find_min_max.o utils.o $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

process_memory: process_memory.c
	$(CC) -o process_memory process_memory.c

parallel_sum: parallel_sum.c utils.h utils.c
	$(CC) -pthread -L. -I. -o project parallel_sum.c utils.c -lsum

clean :
	rm utils.o find_min_max.o 