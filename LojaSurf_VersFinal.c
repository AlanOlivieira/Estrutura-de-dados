#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct PRODUTO {
    char *codigo;
    char *tipo;
    char *descricao;
    float preco;
    struct PRODUTO *ant;
    struct PRODUTO *prox;
} PRODUTO;

typedef struct Cod_caixa {
    char *codigo;
    struct Cod_caixa* prox;
} Cod_caixa;

typedef struct Caixa_estoque{
    int cod_caixa_num;
    Cod_caixa* cods_produtos;
    int quantidadeatt;
    struct Caixa_estoque* prox;
} Caixa_estoque;   

typedef struct Pilha {
    Caixa_estoque * topo;
    int tam;
}Pilha;

typedef struct Pedido {
    char *codigo;
    char *tipo;
    char *descricao;
    float preco;
    char *NomeCliente;
    char *cpf;
    char *cep;
    char *rua;
    char *numero;
    char *complemento;
    struct Pedido *prox;
} Pedido;

Pedido * inicio_fila = NULL;
Pedido * fim_fila = NULL;

PRODUTO * inicio = NULL;
PRODUTO * fim = NULL;
Pilha *pilha_parafinas = NULL;
Pilha *pilha_leash = NULL;
Pilha *pilha_quilhas = NULL;
Pilha *pilha_decks = NULL;

int tam = 0;

void add_produto (char *codigo, char *tipo, char *descricao, float preco) {
    PRODUTO * novo = malloc(sizeof(PRODUTO));
    novo->codigo = codigo;
    novo->tipo = tipo;
    novo->descricao = descricao;
    novo->preco = preco;
    novo->ant = NULL;
    novo->prox = NULL;
    if (inicio == NULL) {
        inicio = novo;
        fim = novo;
    } else if (preco < inicio->preco) {
        novo->prox = inicio;
        inicio->ant = novo;
        inicio = novo;
    } else {
        PRODUTO* aux = inicio;
        while (aux->prox != NULL && aux->prox->preco <= preco) {
            aux = aux->prox;
        }
        novo->prox = aux->prox;
        novo->ant = aux;
        if (aux->prox != NULL) {
            aux->prox->ant = novo;
        } else {
            fim = novo; 
        }
        aux->prox = novo;
    }
    tam++;
}

Pilha* construtor_de_pilha () {
    Pilha *p = malloc(sizeof(Pilha));
    p->topo = NULL;
    p->tam = 0;
    return p;
}

void imprimir_lista_encadeada() {
    PRODUTO* p = inicio;
    if (p == NULL) {
        printf("Lista encadeada vazia!\n");
        return;
    }
    printf("\n***** LISTA DUPLAMENTE ENCADEADS DE PRODUTOS *****\n");
    while (p != NULL) {
        printf("Codigo: %s ; Tipo: %s ; Descricao: %s ; Preco: R$ %.2f\n", p->codigo, p->tipo, p->descricao, p->preco );
        p = p->prox;
        printf("__________________________________________________________________________\n");
    }
}

void add_produtos_na_categoria(Pilha *pilha, char *cod_produto, int quantidade_recebida_produtos, int capacidade_maxima_produtos) {
    for (int i = 0; i < quantidade_recebida_produtos; i++) {
        if (pilha->topo == NULL || pilha->topo->quantidadeatt >= capacidade_maxima_produtos) {
            Caixa_estoque *nova_caixa = malloc(sizeof(Caixa_estoque));
            nova_caixa->cod_caixa_num = pilha->tam + 1;
            nova_caixa->cods_produtos = NULL;
            nova_caixa->quantidadeatt = 0;
            nova_caixa->prox = pilha->topo;
            pilha->topo = nova_caixa;
            pilha->tam++;
        }
        Cod_caixa *novo_cod = malloc(sizeof(Cod_caixa));
        novo_cod->codigo = cod_produto;
        novo_cod->prox = pilha->topo->cods_produtos;
        pilha->topo->cods_produtos = novo_cod;
        pilha->topo->quantidadeatt++;
    }
}

void imprimir_pilha(Pilha *p, const char *nome_categoria) {
    printf("IMPRIMINDO PILHA DA: %s\n", nome_categoria);
    if (p->tam == 0) {
        printf("A pilha está vazia!\n");
        return;
    }
    Caixa_estoque *caixa;
    for (caixa = p->topo; caixa != NULL; caixa = caixa->prox) {
        printf("Caixa %d\n", caixa->cod_caixa_num);
        printf("Quantidade de produtos no estoque: %d\n", caixa->quantidadeatt);
        Cod_caixa *produto;
        int count = 1;
        for (produto = caixa->cods_produtos; produto != NULL; produto = produto->prox) {
            printf("  Produto %d - Codigo do produto: %s\n", count, produto->codigo);
            count++;
        }
    }
}

void ver_produtos_por_categoria(const char *categoria) {
    PRODUTO *p = inicio;
    int encontrados = 0;
    printf("\n**** PRODUTOS DA CATEGORIA: %s ********\n", categoria);
    while (p != NULL) {
        if (strcmp(p->tipo, categoria) == 0) {
            printf("Codigo: %s , Tipo: %s , Descrição: %s , Preco: R$ %.2f\n",
                   p->codigo, p->tipo, p->descricao, p->preco);
            encontrados++;
        }
        p = p->prox;
    }
    if (encontrados == 0) {
        printf("Nenhum produto encontrado pra essa categoria.\n");
    }
    printf("___________________________________________________\n");
}

void ver_produtos_por_limite_preco(float preco_min, float preco_max) {
    PRODUTO *p = inicio;
    int encontrados = 0;
    printf("\n**** PRODUTOS COM PRECO ENTRE RS%.2f E R$%.2f ****\n", preco_min, preco_max);
    while (p != NULL) {
        if (p->preco >= preco_min && p->preco <= preco_max) {
            printf("Codigo: %s , Tipo: %s , Descricao: %s , Preco: R$ %.2f\n",
                   p->codigo, p->tipo, p->descricao, p->preco);
            encontrados++;
        }
        p = p->prox;
    }
    if (encontrados == 0) {
        printf("Nenhum produto encontrado nesse limite de preco.\n");
    }
    printf("_____________________________________________________\n");
}

PRODUTO* remover_da_lista_pelo_codigo(char *codigo) {
    PRODUTO *p = inicio;
    while (p != NULL && strcmp(p->codigo, codigo) != 0) {
        p = p->prox;
    }
    if (p == NULL) {
        printf("Produto com esse codigo %s nao foi encontrado na lista.\n", codigo);
        return NULL;
    }
    if (p == inicio && p == fim) {
        inicio = fim = NULL;
    } else if (p == inicio) {
        inicio = p->prox;
        if (inicio != NULL) inicio->ant = NULL;
    } else if (p == fim) {
        fim = p->ant;
        if (fim != NULL) fim->prox = NULL;
    } else {
        p->ant->prox = p->prox;
        p->prox->ant = p->ant;
    }
    tam--;
    return p;
}

int remover_codigo_em_caixas(Pilha *origem, Pilha *aux, const char *codigo) {
    int encontrado = 0;
    while (origem->topo != NULL) {
        Caixa_estoque *cx = origem->topo;
        Cod_caixa *prev = NULL, *cur = cx->cods_produtos;
        while (cur != NULL) {
            if (strcmp(cur->codigo, codigo) == 0) {
                if (prev) prev->prox = cur->prox; else cx->cods_produtos = cur->prox;
                free(cur);
                cx->quantidadeatt--;
                encontrado = 1;
                break;
            }
            prev = cur;
            cur = cur->prox;
        }
        if (encontrado) {
            if (cx->quantidadeatt == 0) {
                origem->topo = cx->prox;
                free(cx);
                origem->tam--;
            }
            break;
        }
        origem->topo = cx->prox;
        cx->prox = aux->topo;
        aux->topo = cx;
        origem->tam--;
        aux->tam++;
    }
    return encontrado;
}

void remover_da_pilha_de_estoque(char *codigo, char *tipo) {
    Pilha *pilha_categoria = NULL;
    if (strcmp(tipo, "Parafina") == 0) pilha_categoria = pilha_parafinas;
    else if (strcmp(tipo, "Leash") == 0) pilha_categoria = pilha_leash;
    else if (strcmp(tipo, "Quilhas") == 0) pilha_categoria = pilha_quilhas;
    else if (strcmp(tipo, "Deck") == 0) pilha_categoria = pilha_decks;
    else { printf("Tipo de produto desconhecido!\n"); return; }
    if (!pilha_categoria) { printf("Pilha da categoria nao inicializada!\n"); return; }
    Pilha *aux = construtor_de_pilha();
    int ok = remover_codigo_em_caixas(pilha_categoria, aux, codigo);
    while (aux->topo != NULL) {
        Caixa_estoque *cx = aux->topo;
        aux->topo = cx->prox;
        cx->prox = pilha_categoria->topo;
        pilha_categoria->topo = cx;
        aux->tam--;
        pilha_categoria->tam++;
    }
    free(aux);
    if (!ok) {
        printf("Codigo %s nao encontrado no estoque da categoria %s.\n", codigo, tipo);
    }
}

void comprar_produto(char *codigo) {
    PRODUTO *produto = remover_da_lista_pelo_codigo(codigo);
    if (produto == NULL) return;
    remover_da_pilha_de_estoque(produto->codigo, produto->tipo);
    printf("\n *PRODUTO VENDIDO* \n");
    printf("Codigo: %s\nTipo: %s\nDescricao: %s\nPreco: R$ %.2f\n",
           produto->codigo, produto->tipo, produto->descricao, produto->preco);
    free(produto);
}

void adicionar_pedido_na_fila(const char *codigo, const char *tipo, const char *descricao,
                              float preco, const char *nome, const char *cpf,
                              const char *cep, const char *rua, const char *numero,
                              const char *complemento) {
    Pedido *novo = malloc(sizeof(Pedido));
    novo->codigo = (char*)codigo;
    novo->tipo = (char*)tipo;
    novo->descricao = (char*)descricao;
    novo->preco = preco;
    novo->NomeCliente = (char*)nome;
    novo->cpf = (char*)cpf;
    novo->cep = (char*)cep;
    novo->rua = (char*)rua;
    novo->numero = (char*)numero;
    novo->complemento = (char*)complemento;
    novo->prox = NULL;
    if (inicio_fila == NULL) inicio_fila = fim_fila = novo;
    else { fim_fila->prox = novo; fim_fila = novo; }
}

void fazer_a_entrega() {
    if (inicio_fila == NULL) {
        printf("nao tem nenhum pedido pra entrega.\n");
        return;
    }
    Pedido *entrega = inicio_fila;
    inicio_fila = entrega->prox;
    if (inicio_fila == NULL) fim_fila = NULL;
    printf("\n****Produto saindo para entrega*******\n");
    printf("Codigo: %s\ntipo: %s\ndeacricao: %s\npreco: Rs %.2f\n", 
            entrega->codigo, entrega->tipo, entrega->descricao, entrega->preco);
    printf("cliente: %s\ncpf: %s\nCEP: %s\nrua: %s\nnumero: %s\ncomplemento: %s\n",
            entrega->NomeCliente, entrega->cpf, entrega->cep,
            entrega->rua, entrega->numero, entrega->complemento);
    free(entrega);
}

int main(){
    for(int i=1;i<=100;i++){
        char *tipo;
        float preco;
        if(i%4==0){tipo="Parafina";preco=80;}
        else if(i%4==1){tipo="Leash";preco=90;}
        else if(i%4==2){tipo="Deck";preco=150;}
        else{tipo="Quilhas";preco=100;}
        char *codigo=malloc(32); sprintf(codigo,"%s%02d",tipo,i);
        add_produto(codigo,tipo,"Descricao do produto",preco);
    }

    pilha_parafinas = construtor_de_pilha();
    pilha_leash = construtor_de_pilha();
    pilha_quilhas = construtor_de_pilha();
    pilha_decks = construtor_de_pilha();

    add_produtos_na_categoria(pilha_parafinas, "Parafina01", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina02", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina03", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina04", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina05", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina06", 50, 50);
    add_produtos_na_categoria(pilha_parafinas, "Parafina07", 50, 50);

    add_produtos_na_categoria(pilha_leash, "Leash01", 25, 25);
    add_produtos_na_categoria(pilha_leash, "Leash02", 25, 25);
    add_produtos_na_categoria(pilha_leash, "Leash03", 25, 25);
    add_produtos_na_categoria(pilha_leash, "Leash04", 25, 25);
    add_produtos_na_categoria(pilha_leash, "Leash05", 25, 25);

    add_produtos_na_categoria(pilha_quilhas, "Quilhas01", 10, 10);
    add_produtos_na_categoria(pilha_quilhas, "Quilhas02", 10, 10);
    add_produtos_na_categoria(pilha_quilhas, "Quilhas03", 10, 10);
    add_produtos_na_categoria(pilha_quilhas, "Quilhas04", 10, 10);

    add_produtos_na_categoria(pilha_decks, "Deck01", 5, 5);
    add_produtos_na_categoria(pilha_decks, "Deck02", 5, 5);
    add_produtos_na_categoria(pilha_decks, "Deck03", 5, 5);
    add_produtos_na_categoria(pilha_decks, "Deck04", 5, 5);
    add_produtos_na_categoria(pilha_decks, "Deck05", 5, 5);

    printf("*Exibindo produtos na faixa de preco:* \n");
    ver_produtos_por_limite_preco(0.0, 79.99); 
    ver_produtos_por_limite_preco(100.0, 200.00);

    printf("____________________________________________________");
    printf("\n *ANTES DA COMPRA:* \n");
    imprimir_pilha(pilha_parafinas, "PARAFINAS");
    imprimir_pilha(pilha_leash, "LEASH");
    imprimir_pilha(pilha_quilhas, "QUILHAS");
    imprimir_pilha(pilha_decks, "DECKS");
    imprimir_lista_encadeada ();

    printf("\n*REALIZANDO COMPRA DOS PRODUTOS*\n");
    comprar_produto("Parafina01");
    comprar_produto("Parafina02");
    comprar_produto("Parafina03");
    comprar_produto("Parafina04");
    comprar_produto("Parafina05");
    comprar_produto("Parafina06");
    comprar_produto("Leash01");
    comprar_produto("Leash02");
    comprar_produto("Leash03");
    comprar_produto("Leash04");
    comprar_produto("Leash05");
    comprar_produto("Deck01");
    comprar_produto("Deck02");
    comprar_produto("Deck03");
    comprar_produto("Deck04");
    comprar_produto("Deck05");
    comprar_produto("Quilhas01");
    comprar_produto("Quilhas02");
    comprar_produto("Quilhas03");
    comprar_produto("Quilhas04");
    comprar_produto("Parafina07");

    char *clientes[10] = {"Alan", "Marcia", "Levi", "Laura", "vitoria", "Gabriel", "Jose", "Carioca", "Eduarda", "Maju"};
    char *cpfs[10] = {"11111111111", "22222222222", "33333333333", "44444444444", "55555555555","66666666666", "77777777777", "88888888888", "99999999999", "12345678912"};
    char *codigos[] = {"Parafina02", "Parafina03", "Parafina04", "Parafina05", "Parafina06","Leash01", "Leash02", "Leash03", "Deck01", "Quilhas01"};
    char *tipos[] = {"Parafina", "Parafina", "Parafina", "Parafina", "Parafina","Leash", "Leash", "Leash", "Deck", "Quilhas"};
    float precos[] = {80, 80, 80, 80, 80, 90, 90, 90, 150, 100};
    for (int i = 0; i < 10; i++) {
        remover_da_pilha_de_estoque(codigos[i], tipos[i]);
        adicionar_pedido_na_fila(codigos[i], tipos[i], "Descricao do produto", precos[i],
                         clientes[i], cpfs[i], "629000470", "Rua Ufc russas", "123", "apto");
    }

    printf("\n****DEPOS DA COMPRA:***\n");
    for (int i = 0; i < 10; i++) fazer_a_entrega();

    imprimir_pilha(pilha_parafinas, "PARAFINAS");
    imprimir_pilha(pilha_decks, "DECKS");
    imprimir_pilha(pilha_leash, "LEASH");
    imprimir_pilha(pilha_quilhas, "QUILHAS");
    imprimir_lista_encadeada();

    return 0;
}
