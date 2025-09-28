// considerações finais: foi necessario mudar muita coisa em relação as etapas anteriores
// por conta de testes rasos que eu fiz nessas etapas, nessa ultima etapa deu varios erros
// (como a remoção de produtos) pois eu tinha definido apenas um codico para varios produtos
// que estavam no mesmo estoque.

#include <stdio.h>
#include <stdlib.h>
typedef struct PRODUTO {
    char *codigo;
    char *tipo;
    char *descricao;
    float preco;
    struct PRODUTO *ant;
    struct PRODUTO *prox;
} PRODUTO;

typedef struct Cod_caixa {
    char *codico;
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

// prototipos
int remover_codigo_em_caixas(Pilha *origem, Pilha *aux3, char *codigo);
void reempilhar(Pilha *origem, Pilha *aux4);
void comprar_produto(char *codigo);

PRODUTO * inicio = NULL;
PRODUTO * fim = NULL;
// nessa parte 2 foi necessario declarar de forma global para conseguir fazer a função de remover dentro da pilha
// como foi sugerido no laboratorio
Pilha *pilha_parafinas = NULL;
Pilha *pilha_leash = NULL;
Pilha *pilha_quilhas = NULL;
Pilha *pilha_decks = NULL;

int tam = 0;

void add_produto (char *codigo, char *tipo, char *descricao, float preco, int pos) {
    if(pos >= 0 && pos<=tam){
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
        novo_cod->codico = cod_produto;
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
    int i, j;
    for (caixa = p->topo, i = 0; i < p->tam && caixa != NULL; i++, caixa = caixa->prox) {
        printf("Caixa %d\n", caixa->cod_caixa_num);
        printf("Quantidade de produtos no estoque: %d\n", caixa->quantidadeatt);
        Cod_caixa *produto;
        int count = 1;
        for (produto = caixa->cods_produtos; produto != NULL; produto = produto->prox) {
            printf("  Produto %d - Codigo do produto: %s\n", count, produto->codico);
            count++;
        }

    }
}

// parte 2
int comparar_strings(char *str1, char *str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}
void ver_produtos_por_categoria(const char *categoria) {
    PRODUTO *p = inicio;
    int encontrados = 0;
    printf("\n**** PRODUTOS DA CATEGORIA: %s ********\n", categoria);
    while (p != NULL) {
        if (comparar_strings(p->tipo, categoria)) {
            printf("Codigo: %s , Tipo: %s , Descrição: %s , Preco: R$ %.2f\n",
                   p->codigo, p->tipo, p->descricao, p->preco);
            encontrados++;
        }
        p = p->prox;
    }
    if (encontrados == 0) {
        printf("Nenhum produto encontrado pra eessa categoria.\n");
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
    }printf("_____________________________________________________\n");
}

PRODUTO* remover_da_lista_pelo_codigo(char *codigo) {
    PRODUTO *p = inicio;

    while (p != NULL && !comparar_strings(p->codigo, codigo)) {
        p = p->prox;
    }
    if (p == NULL) {
        printf("Produto com esse codigo %s nao foi encontrado na lista.\n", codigo);
        return NULL;
}
    if (p == inicio && p == fim) {
        inicio = fim = NULL;
}   else if (p == inicio) {
        inicio = p->prox;
        if (inicio != NULL) inicio->ant = NULL;
}   else if (p == fim) {
        fim = p->ant;
        if (fim != NULL) fim->prox = NULL;
    } else {
        p->ant->prox = p->prox;
        p->prox->ant = p->ant;
}
    tam--;
    return p; // como foi explicado no lab!!!
}

void remover_da_pilha_de_estoque(char *codigo, char *tipo) {
    Pilha *pilha_categoria = NULL;

    if (comparar_strings(tipo, "Parafina")) pilha_categoria = pilha_parafinas;
    else if (comparar_strings(tipo, "Leash")) pilha_categoria = pilha_leash;
    else if (comparar_strings(tipo, "Quilhas")) pilha_categoria = pilha_quilhas;
    else if (comparar_strings(tipo, "Deck") || comparar_strings(tipo, "Decks")) pilha_categoria = pilha_decks;
    else {
        printf("Tipo de produto desconhecido!\n");
        return;
    }

    Pilha *aux2 = construtor_de_pilha();
    int encontrado = remover_codigo_em_caixas(pilha_categoria, aux2, codigo);
    if (encontrado) {
        reempilhar(pilha_categoria, aux2);
}
    while (aux2->topo != NULL) {
        Caixa_estoque *cx = aux2->topo;
        aux2->topo = cx->prox;
        cx->prox = pilha_categoria->topo;
        pilha_categoria->topo = cx;
        pilha_categoria->tam++;
}
    free(aux2);
}
int remover_codigo_em_caixas(Pilha *origem, Pilha *aux3, char *codigo) {
    int encontrado = 0;
    while (origem->topo != NULL && !encontrado) {
        Caixa_estoque *caixa = origem->topo;
        Cod_caixa *produto = caixa->cods_produtos;
        Cod_caixa *anterior = NULL;
            while (produto != NULL) {
            if (comparar_strings(produto->codico, codigo)) {
                if (anterior == NULL) {
                    caixa->cods_produtos = produto->prox;
            }   else {
                    anterior->prox = produto->prox;
                }
                free(produto);
                caixa->quantidadeatt--;
                encontrado = 1;
                break;
}
            anterior = produto;
            produto = produto->prox;
    }
        if (!encontrado) {
            origem->topo = caixa->prox;
            caixa->prox = aux3->topo;
            aux3->topo = caixa;
            aux3->tam++;
            origem->tam--;
    }
    }
    return encontrado;
}
void reempilhar(Pilha *origem, Pilha *aux4) {
    if (aux4->topo == NULL) return;
    Caixa_estoque *ultima = aux4->topo;
    if (ultima->quantidadeatt > 0) {
        Cod_caixa *mover = ultima->cods_produtos;
        ultima->cods_produtos = mover->prox;
        ultima->quantidadeatt--;
        Caixa_estoque *repor = origem->topo;

        Cod_caixa *novo_cod = malloc(sizeof(Cod_caixa));
        novo_cod->codico = mover->codico;
        novo_cod->prox = repor->cods_produtos;
        repor->cods_produtos = novo_cod;
        repor->quantidadeatt++;
        free(mover);
    }
    Caixa_estoque *cx1 = origem->topo;
    origem->topo = cx1->prox;
    cx1->prox = NULL;
    Caixa_estoque *cx2 = aux4->topo;
    aux4->topo = cx2->prox;
    cx2->prox = cx1;
    origem->topo = cx2;
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

void adicionar_pedido_na_fila(PRODUTO *produto, char *nome, char *cpf, char *cep, char *rua, char *numero, char *complemento) {
    Pedido *novo = malloc(sizeof(Pedido));
    novo->codigo = produto->codigo;
    novo->tipo = produto->tipo;
    novo->descricao = produto->descricao;
    novo->preco = produto->preco;

    novo-> NomeCliente = nome;
    novo->cpf = cpf;
    novo->cep = cep;
    novo->rua = rua;
    novo->numero = numero;
    novo->complemento = complemento;
    novo->prox = NULL;

    if (inicio_fila == NULL) {
        inicio_fila = fim_fila = novo;
}   else {
        fim_fila->prox = novo;
        fim_fila = novo;
}}
void fazer_a_entrega() {
    if (inicio_fila == NULL) {
        printf("nao tem nenhum pedido pra entrega.\n");
        return;
}Pedido *entrega = inicio_fila;
    inicio_fila = entrega->prox;
    if (inicio_fila == NULL) {
        fim_fila = NULL;
}
    printf("\n****Produto saindo para entrega*******\n");
    printf("Codigo: %s\ntipo: %s\ndeacricao: %s\npreco: Rs %.2f\n", 
            entrega->codigo, entrega->tipo, entrega->descricao, entrega->preco);
    printf("cliente: %s\ncpf: %s\nCEP: %s\nrua: %s\nnumero: %s\ncomplemento: %s\n",
            entrega->NomeCliente, entrega->cpf, entrega->cep,
            entrega->rua, entrega->numero, entrega->complemento);
    free(entrega);
}


int main(){
add_produto("Parafina01", "Parafina", "Descricao do produto", 80.0, 0);
add_produto("Parafina02", "Parafina", "Descricao do produto", 80.0, 1);
add_produto("Parafina03", "Parafina", "Descricao do produto", 80.0, 2);
add_produto("Parafina04", "Parafina", "Descricao do produto", 80.0, 3);
add_produto("Leash01", "Leash", "Descricao do produto", 90.0, 4);
add_produto("Leash02", "Leash", "Descricao do produto", 90.0, 5);
add_produto("Leash03", "Leash", "Descricao do produto", 90.0, 6);
add_produto("Deck01", "Deck", "Descricao do produto", 150.0, 7);
add_produto("Deck02", "Deck", "Descricao do produto", 150.0, 8);
add_produto("Deck03", "Deck", "Descricao do produto", 150.0, 9);
add_produto("Quilhas01", "Quilhas", "Descricao do produto", 100.0, 10);
add_produto("Quilhas02", "Quilhas", "Descricao do produto", 100.0, 11);
add_produto("Quilhas03", "Quilhas", "Descricao do produto", 100.0, 12);
add_produto("Quilhas04", "Quilhas", "Descricao do produto", 100.0, 13);
add_produto("Parafina05", "Parafina", "Descricao do produto", 80.0, 14);
add_produto("Leash04", "Leash", "Descricao do produto", 90.0, 15);
add_produto("Deck04", "Deck", "Descricao do produto", 150.0, 16);
add_produto("Parafina06", "Parafina", "Descricao do produto", 80.0, 17);
add_produto("Deck05", "Deck", "Descricao do produto", 150.0, 18);
add_produto("Leash05", "Leash", "Descricao do produto", 90.0, 19);

    pilha_parafinas = construtor_de_pilha();
    pilha_leash = construtor_de_pilha();
    pilha_quilhas = construtor_de_pilha();
    pilha_decks = construtor_de_pilha();

add_produtos_na_categoria(pilha_parafinas, "Parafina01", 5, 50);
add_produtos_na_categoria(pilha_parafinas, "Parafina02", 10, 50);
add_produtos_na_categoria(pilha_parafinas, "Parafina03", 10, 50);
add_produtos_na_categoria(pilha_parafinas, "Parafina04", 5, 50);
add_produtos_na_categoria(pilha_parafinas, "Parafina05", 10, 50);
add_produtos_na_categoria(pilha_parafinas, "Parafina06", 10, 50);
add_produtos_na_categoria(pilha_leash, "Leash01", 5, 25);
add_produtos_na_categoria(pilha_leash, "Leash02", 5, 25);
add_produtos_na_categoria(pilha_leash, "Leash03", 5, 25);
add_produtos_na_categoria(pilha_leash, "Leash04", 5, 25);
add_produtos_na_categoria(pilha_leash, "Leash05", 5, 25);
add_produtos_na_categoria(pilha_quilhas, "Quilhas01", 2, 10);
add_produtos_na_categoria(pilha_quilhas, "Quilhas02", 3, 10);
add_produtos_na_categoria(pilha_quilhas, "Quilhas03", 2, 10);
add_produtos_na_categoria(pilha_quilhas, "Quilhas04", 3, 10);
add_produtos_na_categoria(pilha_decks, "Deck01", 1, 5);
add_produtos_na_categoria(pilha_decks, "Deck02", 1, 5);
add_produtos_na_categoria(pilha_decks, "Deck03", 1, 5);
add_produtos_na_categoria(pilha_decks, "Deck04", 1, 5);
add_produtos_na_categoria(pilha_decks, "Deck05", 1, 5);

    
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

printf("\n*REALIZANDO COMPRA DOS PRODUTOD*\n");
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

char *clientes[10] = {"Alan", "Marcia", "Levi", "Laura", "vitoria", "Gabriel", "Jose", "Carioca", "Eduarda", "Maju"};
char *cpfs[10] = {"11111111111", "22222222222", "33333333333", "44444444444", "55555555555","66666666666", "77777777777", "88888888888", "99999999999", "12345678912"};
char *codigos[] = {"Parafina02", "Parafina03", "Parafina04", "Parafina05", "Parafina06","Leash01", "Leash02", "Leash03", "Deck01", "Quilhas01"};
char *tipos[] = {"Parafina", "Parafina", "Parafina", "Parafina", "Parafina","Leash", "Leash", "Leash", "Deck", "Quilhas"};
float precos[] = {80, 80, 80, 80, 80, 90, 90, 90, 150, 100};
for (int i = 0; i < 10; i++) {
    remover_da_pilha_de_estoque(codigos[i], tipos[i]);
    PRODUTO *p = malloc(sizeof(PRODUTO));
    p->codigo = codigos[i];
    p->tipo = tipos[i];
    p->descricao = "Descricao do produto";
    p->preco = precos[i];

    adicionar_pedido_na_fila(p, clientes[i], cpfs[i], "629000470", "Rua Ufc russas", "123", "apto");
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