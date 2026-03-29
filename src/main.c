#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "corrida.h"

extern int usar_barreira;

int main(int argc, char *argv[]) {
    printf("\033[2J");
    Cavalo cavalos[NUM_CAVALOS];

    setbuf(stdout, NULL);

    if (argc > 1) {
        usar_barreira = atoi(argv[1]);
    }

    criar(cavalos);
    resultado(cavalos);

    return 0;
}