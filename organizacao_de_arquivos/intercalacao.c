#include <stdio.h>
#include <stdlib.h>
#define F 4
#define HV 999999999
#define TAM_NOME 40
#define FILE_NAME "particoes/particao"

struct No {
    int vencedor;
    struct No *dir, *esq, *pontVencedor;
    FILE *arq;
};typedef struct No tNo;

struct noNome {
    char nomeArq[TAM_NOME];
    struct noNome *prox;
};typedef struct noNome tNome;

tNome* adicionaNoNaFila (tNome *finalFila, char nome[]) {
    tNome *aux;
    aux = (tNome*)malloc(sizeof(tNome));
    aux->prox = NULL;
    for(int i=0; i<TAM_NOME; i++)
        aux->nomeArq[i] = nome[i];
    finalFila->prox = aux;
    finalFila = aux;
    return finalFila;
}

tNome* removeNoDaFila (tNome *inicioFilaNomes, tNome *finalFilaNomes) {
    tNome *aux;
    aux = inicioFilaNomes->prox;
    if  (aux != NULL) {
        inicioFilaNomes->prox = aux->prox;
        free(aux);
    }
    if (inicioFilaNomes->prox == NULL) {
        return inicioFilaNomes;
    }
    return finalFilaNomes;
}

tNome* criaListaDeNomesDosArquivos(tNome *finalFilaNomes, int numPart) {
    char nomeArq[TAM_NOME];

    for (int i=0;i<numPart;i++) {
        sprintf(nomeArq, "%s%d.txt", FILE_NAME, i);
        finalFilaNomes = adicionaNoNaFila(finalFilaNomes, nomeArq);
    }
    return finalFilaNomes;
}

int abreArquivos(FILE *listaArq[], tNome *inicioFilaNomes, tNome **finalFilaNomes) {
    FILE *arqPart;
    tNome *aux;
    int cont=0;
    aux = inicioFilaNomes->prox;
    for (int i=0;i<F-1;i++) {
        if (aux != NULL) {
            arqPart = fopen(aux->nomeArq, "r");
            listaArq[i] = arqPart;
            cont++;
            *finalFilaNomes = removeNoDaFila(inicioFilaNomes, *finalFilaNomes);
            aux = inicioFilaNomes->prox;
        }
    }

    return cont;
}

tNo* criaNoDoArquivo(FILE *arqPart) {
    tNo *noCriado;

    noCriado = (tNo*)malloc(sizeof(tNo));
    noCriado->dir = NULL;
    noCriado->esq = NULL;
    noCriado->pontVencedor = NULL;
    fscanf(arqPart, "%d", &noCriado->vencedor);
    noCriado->arq = arqPart;
    
    return noCriado;
}

tNo* criaNo(tNo *dir, tNo *esq) {
    tNo *noCriado;
    
    noCriado = (tNo*)malloc(sizeof(tNo));
    noCriado->dir = dir;
    noCriado->esq = esq;
    if (esq->vencedor > dir->vencedor) {
        noCriado->vencedor = dir->vencedor;
        noCriado->arq = dir->arq;
        noCriado->pontVencedor = dir;
    } else {
        noCriado->vencedor = esq->vencedor;
        noCriado->arq = esq->arq;
        noCriado->pontVencedor = esq;
    }
    
    return noCriado;
}

void criaArvoreDeVencedores(FILE *listaArq[], tNo *lista[], int numPartNaLista) {
    tNo *noCriado;
    int ind=numPartNaLista, i;

    for (i=0; i<numPartNaLista; i++) {
        lista[i] = criaNoDoArquivo(listaArq[i]);
    }

    for (i=0; i<2*(numPartNaLista)-2; i+=2) {
        noCriado = (tNo*)malloc(sizeof(tNo));
        noCriado->dir = lista[i+1];
        noCriado->esq = lista[i];
        if (lista[i]->vencedor > lista[i+1]->vencedor) {
            noCriado->vencedor = lista[i+1]->vencedor;
            noCriado->arq = lista[i+1]->arq;
            noCriado->pontVencedor = lista[i+1];
        } else {
            noCriado->vencedor = lista[i]->vencedor;
            noCriado->arq = lista[i]->arq;
            noCriado->pontVencedor = lista[i];
        }
        lista[ind] = noCriado;
        ind++;
    }
}

void tiraDaArvore(tNo *noVencedor) {
    if (noVencedor->pontVencedor == NULL) {
        if (!feof(noVencedor->arq))
            fscanf(noVencedor->arq, "%d", &noVencedor->vencedor);
        else noVencedor->vencedor = HV;
    } else {
        tiraDaArvore(noVencedor->pontVencedor);
        if (noVencedor->esq->vencedor > noVencedor->dir->vencedor) {
            noVencedor->vencedor = noVencedor->dir->vencedor;
            noVencedor->arq = noVencedor->dir->arq;
            noVencedor->pontVencedor = noVencedor->dir;
        } else {
            noVencedor->vencedor = noVencedor->esq->vencedor;
            noVencedor->arq = noVencedor->esq->arq;
            noVencedor->pontVencedor = noVencedor->esq;
        }
    }
}

void intercalacaoPrincipal(int numPart) {
    FILE *listaArq[F-1], *arqEscrita;
    tNome *inicioFilaNomes, *finalFilaNomes;
    char nomeProxPart[TAM_NOME];
    tNo *noVencedor, *listaNos[2*(F-1)-1];
    int atual=numPart, numPartAtual, i;

    inicioFilaNomes = (tNome*)malloc(sizeof(tNome));
    inicioFilaNomes->prox = NULL;
    finalFilaNomes = inicioFilaNomes;

    finalFilaNomes = criaListaDeNomesDosArquivos(finalFilaNomes, numPart);

    do {
        numPartAtual = abreArquivos(listaArq, inicioFilaNomes, &finalFilaNomes);
        if (numPartAtual == 1) {
            sprintf(nomeProxPart, "%s%d.txt", FILE_NAME, atual-1);
            fclose(listaArq[0]);
            remove("destino.txt");
            if (rename(nomeProxPart, "destino.txt") != 0) {
                perror("Error renaming file");
                printf("Error code: %d\n", errno);
            }
        } else {
            criaArvoreDeVencedores(listaArq, listaNos, numPartAtual);

            sprintf(nomeProxPart, "%s%d.txt", FILE_NAME,atual);
            atual++;
            arqEscrita = fopen(nomeProxPart, "w");
            
            do {
                noVencedor = listaNos[2*(numPartAtual)-2];
                fprintf(arqEscrita, "%d \n", noVencedor->vencedor);
                tiraDaArvore(noVencedor);
            } while (noVencedor->vencedor != HV);

            for (i=0; i<numPartAtual; i++) {
                fclose(listaArq[i]);
            }

            finalFilaNomes = adicionaNoNaFila(finalFilaNomes, nomeProxPart);

            for (i=0; i<numPartAtual; i++) {
                fclose(listaArq[i]);
            }

            for (i=0; i<2*numPartAtual-1; i++) {
                free(listaNos[i]);
            }

            fclose(arqEscrita);
        }

    } while (numPartAtual != 1);
}

int main(void) {
    FILE *arqReg;
    int numPart;

    printf("Digite o número de partiçoes: ");
    scanf("%d", &numPart);
    intercalacaoPrincipal(numPart);

    return 0;
}
