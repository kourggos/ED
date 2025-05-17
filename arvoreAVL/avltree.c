#include <stdio.h>
#include <stdlib.h>

struct No {
    int info, altura;
    struct No *esq, *dir;
};typedef struct No tNo;

int maior(int a, int b) {
    if (a > b) return a;
    else return b;
}

int calculaAltura(tNo *no) {
    if (no == NULL) return 0;
    return no->altura;
}

int calculaBalanceamento(tNo *no) {
    return calculaAltura(no->dir) - calculaAltura(no->esq);
}

tNo* rotacaoEsquerda (tNo *no) {
    tNo *aux = no->dir;

    no->dir = aux->esq;
    aux->esq = no;

    no->altura = maior(calculaAltura(no->esq), calculaAltura(no->dir))+1;
    aux->altura = maior(calculaAltura(aux->esq), calculaAltura(aux->dir))+1;

    return aux;
}
/*
tNo* rotacaoDuplaEsquerda (tNo *no) {
    tNo *aux;

    aux = no->dir->esq;

    no->dir->esq = aux->dir;
    aux->dir = no->dir;
    no->dir = aux->esq;
    aux->esq = no;
    aux->altura = maior(calculaAltura(aux->esq), calculaAltura(aux->dir))+1;
    aux->dir->altura = maior(calculaAltura(aux->dir->esq), calculaAltura(aux->dir->dir))+1;
    no->altura = maior(calculaAltura(no->esq), calculaAltura(no->dir))+1;
    return aux;
}*/
  
tNo* rotacaoDireita (tNo *no) {
    tNo *aux = no->esq;

    no->esq = aux->dir;
    aux->dir = no;

    no->altura = maior(calculaAltura(no->esq), calculaAltura(no->dir))+1;
    aux->altura = maior(calculaAltura(aux->esq), calculaAltura(aux->dir))+1;
    
    return aux;
}
/*
tNo* rotacaoDuplaDireita (tNo *no) {
    tNo *aux;

    aux = no->esq->dir;
    
    no->esq->dir = aux->esq;
    aux->esq = no->esq;
    no->esq = aux->dir;
    aux->dir = no;
    aux->altura = maior(calculaAltura(aux->esq), calculaAltura(aux->dir))+1;;
    aux->esq->altura = maior(calculaAltura(aux->esq->esq), calculaAltura(aux->esq->dir))+1;;
    no->altura = maior(calculaAltura(no->esq), calculaAltura(no->dir))+1;;
    return aux;
}*/

tNo* removeMaiorElementoArvore (tNo *raiz) {
    if (raiz->dir != NULL) {
        removeMaiorElementoArvore(raiz->dir);
    } else {
        
    }

}

tNo* insereNoAVL(tNo *raiz, int info) {
    tNo *noInserir;
    int fb;

    if (raiz == NULL) {
        noInserir = (tNo*)malloc(sizeof(tNo));
        noInserir->dir = NULL;
        noInserir->esq = NULL;
        noInserir->info = info;
        noInserir->altura = 1;
        return noInserir;
    }


    if (raiz->info > info) {
        raiz->esq = insereNoAVL(raiz->esq, info);
    } else {
        raiz->dir = insereNoAVL(raiz->dir, info);
    }

    raiz->altura = maior(calculaAltura(raiz->esq), calculaAltura(raiz->dir))+1;

    fb = calculaBalanceamento(raiz);
    if (fb == 2) {
        if (calculaBalanceamento(raiz->dir) >= 0){
            raiz = rotacaoEsquerda(raiz);
        } else {
            raiz->dir = rotacaoDireita(raiz->dir);
            raiz = rotacaoEsquerda(raiz);
            //raiz = rotacaoDuplaEsquerda(raiz);
        }
    } else if (fb == -2) {
        if (calculaBalanceamento(raiz->esq) <= -0) {
            raiz = rotacaoDireita(raiz);
        } else {
            raiz->esq = rotacaoEsquerda(raiz->esq);
            raiz = rotacaoDireita(raiz);
            //raiz = rotacaoDuplaDireita(raiz);
        }
    }

    return raiz;
}

tNo* achaMaiorNaArvore(tNo *raiz) {
    if (raiz->dir == NULL) {
        return raiz;
    } return achaMaiorNaArvore(raiz->dir);
}

tNo* removeAVL (tNo *raiz, int info) {
    tNo *sucessor;
    int fb;

    if (raiz == NULL) return NULL;

    if (raiz->info > info) {
        raiz->esq = removeAVL(raiz->esq, info);
    } else if (raiz->info < info){
        raiz->dir = removeAVL(raiz->dir, info);
    } else {
        /* se for folha ou nao tiver filho a esquerda, retorna o filho da direita (se for folha, será NULL) */
        if (raiz->esq == NULL) {
            sucessor = raiz->dir;
            free(raiz);
            return sucessor;

        } else if (raiz->dir == NULL) { /*se nao tiver filho a direita, retorna filho a esquerda */
            sucessor = raiz->esq;
            free(raiz);
            return sucessor;
        } else { /*se tiver os dois filhos, tem que achar um sucessor, copiar o valor para o no atual e remover o sucessor*/
            sucessor = achaMaiorNaArvore(raiz->esq);
            raiz->info = sucessor->info;
            raiz->esq = removeAVL(sucessor, sucessor->info);
        }
    }


    raiz->altura = maior(calculaAltura(raiz->esq), calculaAltura(raiz->dir))+1;

    fb = calculaBalanceamento(raiz);
    if (fb == 2) {
        if (calculaBalanceamento(raiz->dir) >= 0){
            raiz = rotacaoEsquerda(raiz);
        } else {
            raiz->dir = rotacaoDireita(raiz->dir);
            raiz = rotacaoEsquerda(raiz);
            //raiz = rotacaoDuplaEsquerda(raiz);
        }
    } else if (fb == -2) {
        if (calculaBalanceamento(raiz->esq) <= 0) {
            raiz = rotacaoDireita(raiz);
        } else {
            raiz->esq = rotacaoEsquerda(raiz->esq);
            raiz = rotacaoDireita(raiz);
            //raiz = rotacaoDuplaDireita(raiz);
        }
    }

    return raiz;
}

void printaAVLOrdemAltura(tNo *raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->info);
        printaAVLOrdemAltura(raiz->esq);
        printaAVLOrdemAltura(raiz->dir);
    }
}

void printaAVLOrdemCrescente(tNo *raiz) {
    if (raiz != NULL) {
        printaAVLOrdemCrescente(raiz->esq);
        printf("%d ", raiz->info);
        printaAVLOrdemCrescente(raiz->dir);
    }
}

int main(void) {
    tNo *raiz = NULL;

    
    raiz = insereNoAVL(raiz, 10);
    raiz = insereNoAVL(raiz, 20);
    raiz = insereNoAVL(raiz, 30);
    raiz = insereNoAVL(raiz, 40);
    raiz = insereNoAVL(raiz, 50);
    raiz = insereNoAVL(raiz, 25);

    printaAVLOrdemAltura(raiz);

    raiz = removeAVL(raiz, 50);
    raiz = removeAVL(raiz, 40);
    raiz = removeAVL(raiz, 10);

    printf("\n");
    printaAVLOrdemAltura(raiz);

    return 0;
}