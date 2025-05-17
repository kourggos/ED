#include <stdio.h>
#include <stdlib.h>
#define M 7
#define VALOR_MAX 999999999

struct registroNat {
    int info, vazio;
};typedef struct registroNat tRegNat;


int achaMenorNat(tRegNat lista[], int tam) {
    int menorIndice = -1, menorValor = VALOR_MAX;

    for (int i=0; i<tam; i++) {
        if (lista[i].info < menorValor && !lista[i].vazio) {
            menorIndice = i;
            menorValor = lista[i].info;
        }
    }
    return menorIndice;
}

void particoesSelecaoNatural(FILE *arqReg, int m) {
    tRegNat lista[M], reserva[M], aux;
    FILE *arqPart;
    int i, contPart=0, contNumReg=0, contRes=0, firstInPart=1, ultimoColocado, indiceRegistro;
    char nomeArq[20];

    // cria a lista e o reservatório
    for (i=0; i<m; i++) {
        lista[i].info = 0;
        lista[i].vazio = 1;
    }
    for (i=0; i<m; i++) {
        reserva[i].info = 0;
        reserva[i].vazio = 1;
    }
    // le M registros do arquivo para a memoria
    contNumReg=0;
    while (contNumReg<m && !feof(arqReg)) {
        fscanf(arqReg, "%d", &lista[contNumReg].info);
        lista[contNumReg].vazio = 0;
        contNumReg++;
    } 

    do {
        sprintf(nomeArq, "particoes/particao%d.txt", contPart);
        arqPart = fopen(nomeArq, "w");

        
        do {
            indiceRegistro = achaMenorNat(lista, m);
            if (indiceRegistro != -1) {
                if (!firstInPart){
                    fprintf(arqPart, "\n", lista[indiceRegistro].info);
                }
                fprintf(arqPart, "%d", lista[indiceRegistro].info);
                firstInPart = 0;
            }
            else break;
            ultimoColocado = lista[indiceRegistro].info;
            if (!feof(arqReg) && contRes<m){
                fscanf(arqReg, "%d", &lista[indiceRegistro].info);
                while (lista[indiceRegistro].info < ultimoColocado && contRes < m) {
                    reserva[contRes] = lista[indiceRegistro];
                    reserva[contRes].vazio = 0;
                    contRes++;
                    if (!feof(arqReg) && contRes < m){
                        fscanf(arqReg, "%d", &lista[indiceRegistro].info);
                    } else {
                        lista[indiceRegistro].vazio = 1;
                        break;
                    }
                }
            } else {
                lista[indiceRegistro].vazio = 1;
            }
        } while (indiceRegistro != -1);
        fclose(arqPart);
        contNumReg = 0;
        for (i=0; i<m; i++) {
            aux = lista[i];
            lista[i] = reserva[i];
            if (!lista[i].vazio) contNumReg++;
            reserva[i] = aux;
            reserva[i].info = 0;
            reserva[i].vazio = 1;
        }
        contRes = 0;
        contPart++;
        firstInPart = 1;

    } while (contNumReg != 0);
}

int main (void) {
    int n;
    FILE *arqReg;
    arqReg = fopen("registrosINT.txt", "a");
    if (arqReg != NULL) {
        n = ftell(arqReg);
        fclose(arqReg);
    }
    arqReg = fopen("registrosINT.txt", "r");
    if (arqReg != NULL) {
        particoesSelecaoNatural(arqReg, M);
        fclose(arqReg);
    }
    return 0;
}