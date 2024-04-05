#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
unsigned long long result = 1; // Глобальная переменная для хранения результата факториала

struct FactorialArgs {
    int start;
    int end;
    int mod;
};

void *calculateFactorial(void *args) {
    struct FactorialArgs *fargs = (struct FactorialArgs *)args;
    int mod = fargs->mod;
    unsigned long long localResult = 1;

    for (int i = fargs->start; i <= fargs->end; i++) {
        localResult = (localResult * i) % mod;
    }

    pthread_mutex_lock(&mutex);
    result = (result * localResult) % mod;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 7) {
        printf("Usage: %s -k <number> --pnum=<number> --mod=<number>\n", argv[0]);
        return 1;
    }

    int k, pnum, mod;
    sscanf(argv[2], "%d", &k);
    sscanf(argv[4], "%d", &pnum);
    sscanf(argv[6], "%d", &mod);

    pthread_t threads[pnum];
    struct FactorialArgs args[pnum];

    int chunk_size = k / pnum; // Размер части факториала для каждого потока
    int extra = k % pnum; // Остаток от деления для последнего потока

    int start = 1;
    for (int i = 0; i < pnum; i++) {
        args[i].start = start;
        args[i].end = start + chunk_size - 1 + (i < extra ? 1 : 0); // Добавляем 1 к размеру для последнего потока
        args[i].mod = mod;
        start = args[i].end + 1;

        pthread_create(&threads[i], NULL, calculateFactorial, &args[i]);
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Factorial of %d (mod %d) = %llu\n", k, mod, result);

    pthread_mutex_destroy(&mutex);
    return 0;
}
