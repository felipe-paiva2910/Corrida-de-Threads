#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "corrida.h"

pthread_mutex_t mutex;
pthread_barrier_t barreira;

int usar_barreira = 0;

void imprimirProgresso(int progresso, int id) {
    int barra = progresso * 100 / DISTANCIA;

    printf("\033[%d;0H", id);

    printf("Cavalo %d: [", id);

    for (int i = 0; i < barra; i++) printf("#");
    for (int i = barra; i < 100; i++) printf(" ");

    printf("] %3d%%", progresso);

    fflush(stdout);
}

void *corrida(void *arg) {
    Cavalo *cavalo = (Cavalo *)arg;
    unsigned int seed = time(NULL) ^ cavalo->id;
    struct timespec inicio, fim;
    
    if (usar_barreira) {
        pthread_barrier_wait(&barreira);
    }

    clock_gettime(CLOCK_MONOTONIC, &inicio);

    while (cavalo->progresso < DISTANCIA) {
        int passo = rand_r(&seed) % 10 + 1;

        cavalo->progresso += passo;

        if (cavalo->progresso > DISTANCIA)
            cavalo->progresso = DISTANCIA;

        pthread_mutex_lock(&mutex);
        imprimirProgresso(cavalo->progresso, cavalo->id);
        pthread_mutex_unlock(&mutex);
        usleep((rand_r(&seed) % 300 + 100) * 1000);
    }

    clock_gettime(CLOCK_MONOTONIC, &fim);

    cavalo->tempo = (fim.tv_sec - inicio.tv_sec) +
                    (fim.tv_nsec - inicio.tv_nsec) / 1e9;

    return NULL;
}

void criar(Cavalo cavalos[]) {
    pthread_t threads[NUM_CAVALOS];

    pthread_mutex_init(&mutex, NULL);

    if (usar_barreira) {
        pthread_barrier_init(&barreira, NULL, NUM_CAVALOS);
    }

    for (int i = 0; i < NUM_CAVALOS; i++) {
        cavalos[i].id = i + 1;
        cavalos[i].progresso = 0;
        cavalos[i].tempo = 0;

        pthread_create(&threads[i], NULL, corrida, &cavalos[i]);
    }

    for (int i = 0; i < NUM_CAVALOS; i++) {
        pthread_join(threads[i], NULL);
    }

    if (usar_barreira) {
        pthread_barrier_destroy(&barreira);
    }

    pthread_mutex_destroy(&mutex);
}

void resultado(Cavalo cavalos[]) {
    for (int i = 0; i < NUM_CAVALOS - 1; i++) {
        for (int j = i + 1; j < NUM_CAVALOS; j++) {
            if (cavalos[i].tempo > cavalos[j].tempo) {
                Cavalo temp = cavalos[i];
                cavalos[i] = cavalos[j];
                cavalos[j] = temp;
            }
        }
    }
    printf("\033[%d;0H", NUM_CAVALOS + 2);
    printf("\nRanking final:\n");
    for (int i = 0; i < NUM_CAVALOS; i++) {
        printf("%d lugar: Cavalo %d (%.2f s)\n",
               i + 1, cavalos[i].id, cavalos[i].tempo);
    }
}