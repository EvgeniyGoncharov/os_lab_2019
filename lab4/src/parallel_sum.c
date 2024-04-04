#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>
#include <sys/time.h>
#include "utils.h"
#include "sum.h"

struct SumArgs {
    int *array;
    int begin;
    int end;
};

void *ThreadSum(void *args) {
    struct SumArgs *sum_args = (struct SumArgs *)args;
    int *sum = malloc(sizeof(int));
    *sum = Sum(sum_args);
    pthread_exit(sum);
}

int main(int argc, char **argv) {
    u_int32_t threads_num = 0;
    u_int32_t array_size = 0;
    u_int32_t seed = 0;
    pthread_t *threads = NULL;
    
    // Обработка аргументов командной строки
    while (1) {
        int current_optind = optind ? optind : 1;
        static struct option options[] = {{"seed", required_argument, 0, 0},
                                          {"array_size", required_argument, 0, 0},
                                          {"threads_num", required_argument, 0, 0},
                                          {0, 0, 0, 0}};
    
        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);
    
        if (c == -1) break;
        switch (option_index) {
            case 0:
                seed = atoi(optarg);
                if (seed <= 0) {
                    printf("seed should be a positive number\n");
                    return 1;
                }
                break;
            case 1:
                array_size = atoi(optarg);
                if (array_size <= 0) {
                    printf("array_size should be a positive number\n");
                    return 1;
                }
                break;
            case 2:
                threads_num = atoi(optarg);
                if (threads_num <= 0) {
                    printf("threads_num should be a positive number\n");
                    return 1;
                }
                break;
            default:
                printf("Index %d is out of options\n", option_index);
        }
    }
    
    // Проверка корректности введенных аргументов
    if (seed == 0 || array_size == 0 || threads_num == 0) {
        printf("Usage: %s --seed <num> --array_size <num> --threads_num <num>\n", argv[0]);
        return 1;
    }
    
    // Выделение памяти под массив потоков
    threads = malloc(sizeof(pthread_t) * threads_num);
    if (threads == NULL) {
        printf("Memory allocation error\n");
        return 1;
    }
    
    // Выделение памяти под массив для генерации
    int *array = malloc(sizeof(int) * array_size);
    if (array == NULL) {
        printf("Memory allocation error\n");
        free(threads); // Освобождаем ранее выделенную память
        return 1;
    }
    
    // Генерация массива
    GenerateArray(array, array_size, seed);
    
    // Измерение времени начала выполнения
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    
    // Вычисление суммы массива параллельно
    struct SumArgs *args = malloc(sizeof(struct SumArgs) * threads_num);
    if (args == NULL) {
        printf("Memory allocation error\n");
        free(threads); // Освобождаем ранее выделенную память
        free(array);   // Освобождаем ранее выделенную память
        return 1;
    }
    
    int step = array_size / threads_num;
    for (u_int32_t i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].begin = i * step;
        args[i].end = (i == threads_num - 1) ? array_size : (i + 1) * step;
        if (pthread_create(&threads[i], NULL, ThreadSum, &args[i])) {
            printf("Error: pthread_create failed!\n");
            free(threads); // Освобождаем ранее выделенную память
            free(array);   // Освобождаем ранее выделенную память
            free(args);    // Освобождаем ранее выделенную память
            return 1;
        }
    }
    
    // Суммируем результаты всех потоков
    int total_sum = 0;
    for (u_int32_t i = 0; i < threads_num; i++) {
        int *sum;
        pthread_join(threads[i], (void **)&sum);
        total_sum += *sum;
        free(sum); // Освобождаем память, выделенную для каждого потока
    }
    
    // Измерение времени окончания выполнения
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    
    // Вывод результатов и времени выполнения
    printf("\nTotal: %d\n", total_sum);
    printf("Elapsed time: %ld microseconds\n", ((end_time.tv_sec - start_time.tv_sec) * 1000000L) + end_time.tv_usec - start_time.tv_usec);
    
    // Освобождение выделенной памяти
    free(threads);
    free(array);
    free(args);
    
    return 0;
}
