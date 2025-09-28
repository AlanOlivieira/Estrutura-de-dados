#include <stdio.h>
#include <stdlib.h>

typedef struct NO{
    int chave;
    struct NO *dir;
    struct NO *esq;
}NO;

NO *raiz = NULL;
int tam = 0;

NO * buscar(NO* aux, int chave){
    if(aux == NULL){
        return NULL;
    }else if(chave == aux->chave){
        return aux;
    }else if(chave < aux->chave){
        if(aux->esq != NULL){
            return buscar(aux->esq, chave);
        }else{
            return aux;
        }
    }else{
        if(aux->dir != NULL){
            return buscar(aux->dir, chave);
        }else{
            return aux;
        }
    }
}

void add(int chave){
    NO * novo = malloc(sizeof(NO));
    novo->chave = chave;
    novo->esq = NULL;
    novo->dir = NULL;
    NO * aux = buscar(raiz, chave);
    if(aux == NULL){
        raiz = novo;
    }else if(aux->chave == chave){
        printf("Adição nao permitida. Chave duplicada\n");
        free(novo);
    }else{
        if(chave < aux->chave){
            aux->esq = novo;
        }else{
            aux->dir = novo;
        }
    }
}

void imprimir_in(NO * aux){
    if(aux != NULL){
        imprimir_in(aux->esq);
        printf("%d\n", aux->chave);
        imprimir_in(aux->dir);
    }
}

NO * remover(NO * aux, int chave){
    if (aux == NULL) {
        return NULL;
    }if (chave < aux->chave) {
        aux->esq = remover(aux->esq, chave);
    } else if (chave > aux->chave) {
        aux->dir = remover(aux->dir, chave);
    } else {
        NO *aux1;
        if(aux->dir == NULL && aux->esq == NULL){
            free(aux);
            return NULL;
      } else if (aux->dir == NULL && aux->esq != NULL){
        aux1 = aux->esq;
        free(aux);
        return aux1;
      } else if (aux->esq == NULL && aux->dir != NULL){
        aux1 = aux->dir;
        free(aux);
        return aux1;
}       else {
        NO *aux2 = aux->esq;
        while (aux2->dir != NULL) {
        aux2 = aux2->dir;
}
        aux->chave = aux2->chave;
        aux->esq = remover(aux->esq, aux2->chave);
}
}
return aux;
}
int main() {
    add(52);
    add(30);
    add(12);
    add(25);
    add(58);
    raiz = remover(raiz, 58);
    imprimir_in(raiz);
    return 0;
}