#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define NUM_REGISTROS 10000

int main (void) {
    FILE *arq;
    int r;
    srand(time(NULL));
    arq = fopen("arquivo_desorganizado.txt", "w");
    for (int i=0; i<NUM_REGISTROS; i++) {
        r = rand();
        fprintf(arq, "%d", r);
        if (i != NUM_REGISTROS-1) {
            fprintf(arq, "\n");
        }
    }
}