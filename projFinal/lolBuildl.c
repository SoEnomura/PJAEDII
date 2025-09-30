#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX_RODADAS 8
#define MAX_BUILD 6

typedef struct item{
    int  id, valor;
    char nome[25];
    char tipo[10];
}Item;

typedef struct no {
    Item   dado;
    struct no *ant;
    struct no *prox;
}No;

typedef struct listaCDE{
    No *inicio;
    No *fim;
    int tam;
}ListaCDE;

typedef struct player{
    int id, win, loss;
    char nome[20];
}Player;

void cria(ListaCDE *l);
int insere(ListaCDE *l, Item dado);
//int insereOrdenado(ListaCDE *l, Item dado);
int retira(ListaCDE *l, Item dado);
int estaVazia(ListaCDE l);
Item getInicio(ListaCDE l);
Item getFim(ListaCDE l);
int getTamanho(ListaCDE l);
void mostra(ListaCDE, int sentido);

int gerarIdJogador();
void novoJogador(Player *p);
void carregaItens(ListaCDE *l, const char *filename);

int ganhaOuro(int valorBuild);
int valorBuild(ListaCDE *build);

int comprarItem(ListaCDE *loja, ListaCDE *build, int id, int *ouro);
int venderItem(ListaCDE *build, int id, int *ouro);

int carregaJogador(int id, Player *p);
void salvaJogador(Player p);

int main(){
    srand(time(NULL));

    ListaCDE loja, build;
    Item dado;
    Player invocador;

    int op;
    int ouro = 0, rodadas = 0;
    int escolhaId;
    int fimDeJogo = 0;
    int itemId;

    printf("Entrando no Hexgate para Summoners Rift...\n");

    printf("\nEs um novo invocador ou ja esteve aqui antes?\n");
    printf("[1] - Novo invocador\n");
    printf("[2] - Ja estive aqui\n");
    printf("Digite sua resposta: ");
    scanf("%d", &escolhaId);

    if (escolhaId == 2){
        printf("Digite o seu ID: ");
        scanf("%d", &invocador.id);
        if (!carregaJogador(invocador.id, &invocador)){
            printf("Nenhum invocador com esse ID! Criando um novo...\n");
            novoJogador(&invocador);
        }
    }
    else if (escolhaId == 1){
        novoJogador(&invocador);
    }
    else{
        printf("Escolha invalida!\n");
        printf("Criando novo ID para comecar o jogo...\n");
        novoJogador(&invocador);
    }

    printf("\nBem-vindo, invocador %s! Seu id eh %d. Voce tem %d vitoria(s) e %d derrota(s)!",
           invocador.nome, invocador.id, invocador.win, invocador.loss);
    printf("\n\nVoce tem %d rodadas para completar sua build de %d itens!", MAX_RODADAS, MAX_BUILD);
    printf("\nA cada vez que escolher ganhar mais ouro, uma rodada eh contada.");
    printf("\nOu seja, pense sabiamente em quais itens comprar e vender antes de selecionar ganhar ouro!");
    printf("\nRuneterra deseja-lhe boa sorte!\n");

    cria(&loja);
    cria(&build);

    carregaItens(&loja, "itens.txt");

    do{
        printf("\n===== Menu =====\n");
        printf("\n[1] - Ver todos os itens");
        printf("\n[2] - Ver sua build");
        printf("\n[3] - Ganhar ouro");
        printf("\n[4] - Ver ouro");
        printf("\n[5] - Comprar item");
        printf("\n[6] - Vender item");
        printf("\n[7] - Ver informacoes");
        printf("\n\n================\n");
        printf("Digite sua escolha: ");
        scanf("%d", &op);
        printf("\n");

        switch (op){
        case 1:
            mostra(loja, 1);
            break;
        case 2:
            mostra(build, 1);
            break;
        case 3:
            {
                int bonus = valorBuild(&build);
                int ganho = ganhaOuro(bonus);
                ouro += ganho;
                rodadas++;
                printf("Voce ganhou %d de ouro! Total: %d\n", ganho, ouro);
                printf("Rodadas usadas: %d/%d\n", rodadas, MAX_RODADAS);
                if (rodadas == MAX_RODADAS)
                    printf("Sua ultima chance, invocador!");
                if (rodadas > MAX_RODADAS){
                    invocador.loss++;
                    printf("\nVoce perdeu! Nao completou a build dentro das %d rodadas.\n", MAX_RODADAS);
                    printf("Vitorias: %d | Derrotas: %d\n", invocador.win, invocador.loss);
                    fimDeJogo = 1;
                }
            }
            break;
        case 4:
            printf("Ouro disponivel: %d\n", ouro);
            break;
        case 5:
            printf("Qual o ID do item que deseja? ");
            scanf("%d", &itemId);
            {
                int res;
                res = comprarItem(&loja, &build, itemId, &ouro);
                if (res == 1)
                    printf("Item comprado!\n");
                else if (res == 2)
                    printf("Voce precisa de mais ouro, invocador!\n");
                else if (res == 3)
                    printf("Ei, voce ja comprou esse!\n");
                else
                    printf("Item nao encontrado!\n");
            }
            if (getTamanho(build) >= MAX_BUILD) {
                invocador.win++;
                printf("\nParabens, invocador! Voce completou a build com sucesso.\n");
                printf("Vitorias: %d | Derrotas: %d\n", invocador.win, invocador.loss);
                fimDeJogo = 1;
            }
            break;
        case 6:
            printf("Digite o ID do item que deseja vender: ");
            scanf("%d", &itemId);
            {
                int res;
                res = venderItem(&build, itemId, &ouro);
                if (res){
                    printf("Item vendido! Agora voce tem %d de ouro\n", ouro);
                }
                else{
                    printf("Item nao encontrado na build!\n");
                }
            }
            break;
        case 7:
            printf("Mostrando informacoes do invocador...\n\n");
            printf("ID: %d\nNome: %s\nRodadas: %d/%d\nVitorias: %d\nDerrotas: %d\n",
                   invocador.id, invocador.nome, rodadas, MAX_RODADAS, invocador.win, invocador.loss);
            break;
        default:
            printf("\nNao escolheu nenhuma opcao\n");
        }
    }while(!fimDeJogo);
    salvaJogador(invocador);

    while (!estaVazia(loja)) {
        Item item = getInicio(loja);
        retira(&loja, item);
    }
    while (!estaVazia(build)) {
        Item item = getInicio(build);
        retira(&build, item);
    }

    return 0;
}

void cria(ListaCDE *l){
    l->inicio = NULL;
    l->fim = NULL;
    l->tam = 0;
}

int insere(ListaCDE *l, Item dado){
    No *aux = (No *) malloc(sizeof(No));
    if(aux == NULL)
        return 0;

    aux->dado = dado;
    l->tam++;

    if(l->inicio == NULL){
        l->inicio = aux;
        l->fim = aux;
        aux->ant = l->fim;
        aux->prox = l->inicio;
        return 1;
    }

    aux->ant = l->fim;
    aux->prox = l->inicio;
    l->fim->prox = aux;
    l->fim = aux;
    l->inicio->ant = l->fim;
    return 1;
}

int retira(ListaCDE *l, Item dado){
    No *aux;
    if(l->inicio == NULL){
        printf("Vazio\n");
        return 0;
    }

    if((dado.id == l->inicio->dado.id) && (l->inicio == l->fim)) {
        aux = l->inicio;
        l->inicio = NULL;
        l->fim = NULL;
        free(aux);
        l->tam--;
        return 1;
    }

    if(dado.id == l->inicio->dado.id) {
        aux = l->inicio;
        l->inicio = aux->prox;
        l->inicio->ant = l->fim;
        l->fim->prox = l->inicio;
        free(aux);
        l->tam--;
        return 1;
    }

    if(dado.id == l->fim->dado.id) {
        aux = l->fim;
        l->fim = aux->ant;
        l->fim->prox = l->inicio;
        l->inicio->ant = l->fim;
        free(aux);
        l->tam--;
        return 1;
    }

    aux = l->inicio->prox;
    while((dado.id != aux->dado.id) && (aux != l->fim))
        aux = aux->prox;

    if(aux == l->fim){
        printf("O elemento %d nao esta na lista.\n", dado.id);
        return 0;
    }

    aux->prox->ant = aux->ant;
    aux->ant->prox = aux->prox;
    free(aux);
    l->tam--;
    return 1;
}

int estaVazia(ListaCDE l){
    if(l.inicio == NULL)
        return 1;
    return 0;
}

Item getInicio(ListaCDE l){
    return l.inicio->dado;
}

Item getFim(ListaCDE l){
    return l.fim->dado;
}

int getTamanho(ListaCDE l){
    return l.tam;
}

void mostra(ListaCDE l, int sentido){
    No *aux;
    if(l.inicio == NULL)
        printf("Vazio!\n");
    else {
        if(sentido == 1){
            aux = l.inicio;
            do {
                printf("ID: %d", aux->dado.id);
                printf(" | %s", aux->dado.nome);
                printf(" | Valor: %d", aux->dado.valor);
                printf(" | Tipo: %s\n", aux->dado.tipo);
                aux = aux->prox;
            }while(aux != l.inicio);
        }
        else {
            printf("\nExibindo lista do fim ao inicio:\n\n");
            aux = l.fim;
            do {
                printf("ID: %d\n", aux->dado.id);
                printf("Nome: %s", aux->dado.nome);
                printf("Valor: %d", aux->dado.valor);
                printf("Tipo: %s\n\n", aux->dado.tipo);
                aux = aux->ant;
            } while(aux != l.fim);
        }
    }
}

int gerarIdJogador(){
    FILE *file = fopen("historico.txt", "r");
    if (file == NULL)
        return 1;

    int maxId = 0;
    Player temp;
    while (fscanf(file, "%d %s %d %d", &temp.id, temp.nome, &temp.win, &temp.loss) == 4) {
        if (temp.id > maxId)
            maxId = temp.id;
    }
    fclose(file);
    return maxId + 1;
}

void novoJogador(Player *p){
    printf("\nComo deseja ser chamado? ");
    scanf("%s", p->nome);
    p->id = gerarIdJogador();
    p->win = 0;
    p->loss = 0;
}

void carregaItens(ListaCDE *l, const char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s!\n", filename);
        return;
    }
    Item item;
    while (fscanf(file, "%d %s %d %s", &item.id, item.nome, &item.valor, item.tipo) == 4) {
        insere(l, item);
    }
    fclose(file);
}

int ganhaOuro(int valorBuild){
    int base = rand() % 201 + 100;
    return base + valorBuild / 2;
}

int valorBuild(ListaCDE *build){
    int total = 0;
    if (!build->inicio) return 0;

    No *aux = build->inicio;
    do{
        total += aux->dado.valor;
        aux = aux->prox;
    } while (aux != build->inicio);
    return total;
}

int comprarItem(ListaCDE *loja, ListaCDE *build, int id, int *ouro){
    if (!estaVazia(*build)) {
        No *build_aux = build->inicio;
        do {
            if (build_aux->dado.id == id) {
                return 3;
            }
            build_aux = build_aux->prox;
        } while (build_aux != build->inicio);
    }

    No *aux = loja->inicio;
    do {
        if (aux->dado.id == id){
            if (*ouro >= aux->dado.valor){
                *ouro -= aux->dado.valor;
                insere(build, aux->dado);
                return 1;
            }
            return 2;
        }
        aux = aux->prox;
    }while (aux != loja->inicio);
    return 0;
}

int venderItem(ListaCDE *build, int id, int *ouro) {
    if (estaVazia(*build))
        return 0;

    No *aux = build->inicio;
    do {
        if (aux->dado.id == id) {
            *ouro += aux->dado.valor / 2;
            retira(build, aux->dado);
            return 1;
        }
        aux = aux->prox;
    } while (aux != build->inicio);
    return 0;
}

int carregaJogador(int id, Player *p) {
    FILE *file = fopen("historico.txt", "r");
    if (file == NULL) return 0;

    int encontrado = 0;
    Player temp;
    while (fscanf(file, "%d %s %d %d", &temp.id, temp.nome, &temp.win, &temp.loss) == 4) {
        if (temp.id == id) {
            *p = temp;
            encontrado = 1;
        }
    }
    fclose(file);
    return encontrado;
}

void salvaJogador(Player p) {
    FILE *file = fopen("historico.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo historico.txt para escrita!\n");
        return;
    }
    fprintf(file, "\n%d %s %d %d", p.id, p.nome, p.win, p.loss);
    fclose(file);
}
