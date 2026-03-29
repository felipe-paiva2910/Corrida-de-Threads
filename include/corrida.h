#ifndef CORRIDA_H
#define CORRIDA_H

#include <pthread.h>

#define NUM_CAVALOS 5
#define DISTANCIA 100

typedef struct {
    int id;
    int progresso;
    double tempo;
} Cavalo;

extern int usar_barreira;

void criar(Cavalo cavalos[]);
void resultado(Cavalo cavalos[]);

#endif