#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

#define THREAD_COUNT 4

struct FactorialArgs {
    uint64_t begin;
    uint64_t end;
    uint64_t mod;
};

struct ThreadArgs {
    uint64_t start;
    uint64_t end;
    uint64_t mod;
    uint64_t result;
};


void *ThreadFactorial(void *args) {
    struct ThreadArgs *targs = (struct ThreadArgs *)args;
    targs->result = 1;
    for (uint64_t i = targs->start; i <= targs->end; ++i) {
        targs->result = MultModulo(targs->result, i, targs->mod);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int port = -1;

    while (true) {
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"port", required_argument, 0, 0},
                                          {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 0: {
                switch (option_index) {
                    case 0:
                        port = atoi(optarg);
                        break;
                    default:
                        printf("Index %d is out of options\n", option_index);
                }
            } break;

            case '?':
                printf("Unknown argument\n");
                break;
            default:
                fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    }

    if (port == -1) {
        fprintf(stderr, "Using: %s --port 20001\n", argv[0]);
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        fprintf(stderr, "Can not create server socket!");
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons((uint16_t)port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

    int err = bind(server_fd, (struct sockaddr *)&server, sizeof(server));
    if (err < 0) {
        fprintf(stderr, "Can not bind to socket!");
        return 1;
    }

    err = listen(server_fd, 128);
    if (err < 0) {
        fprintf(stderr, "Could not listen on socket\n");
        return 1;
    }

    printf("Server listening at %d\n", port);

    while (true) {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);

        if (client_fd < 0) {
            fprintf(stderr, "Could not establish new connection\n");
            continue;
        }

        int read_size;
        struct FactorialArgs *args = (struct FactorialArgs *)malloc(sizeof(struct FactorialArgs));

        read_size = recv(client_fd, args, sizeof(*args), 0);
        if (read_size < 0) {
            perror("recv");
            close(client_fd);
            continue;
        }

        printf("Received task: begin=%llu, end=%llu, mod=%llu\n", args->begin, args->end, args->mod);

        pthread_t threads[THREAD_COUNT];
        struct ThreadArgs targs[THREAD_COUNT];

        uint64_t range_length = (args->end - args->begin + 1) / THREAD_COUNT;
        for (int i = 0; i < THREAD_COUNT; ++i) {
            targs[i].start = args->begin + i * range_length;
            targs[i].end = i == THREAD_COUNT - 1 ? args->end : targs[i].start + range_length - 1;
            targs[i].mod = args->mod;
            pthread_create(&threads[i], NULL, ThreadFactorial, &targs[i]);
        }

        uint64_t result = 1;
        for (int i = 0; i < THREAD_COUNT; ++i) {
            pthread_join(threads[i], NULL);
            result = MultModulo(result, targs[i].result, args->mod);
        }

        // Отправляем клиенту результат
        if (send(client_fd, &result, sizeof(result), 0) < 0) {
            fprintf(stderr, "Can't send data to client\n");
        }

        close(client_fd);
    }

    return 0;
}
