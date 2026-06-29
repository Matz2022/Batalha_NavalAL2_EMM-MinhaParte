#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AGUA 0
#define AGUADESCONHECIDA 7
#define TAMANHO 8
#define NAVIO 1
#define NAVIOATINGIDO 2
#define TAMANHO_NAVIO 2  
#define QTD_NAVIOS 4 

typedef struct {
    int linha[TAMANHO_NAVIO];
    int coluna[TAMANHO_NAVIO];
    int partesAtingidas;
    int afundado;
} Navio;

int verificaSobreposicao(int **campo, int tamanho, int linha, int coluna) {
    if (linha < 0 || linha >= tamanho || coluna < 0 || coluna >= tamanho)
        return 1;

    return campo[linha][coluna] != AGUA;
}

void limpar_campo(int **campo, int tamanho){
    if(campo != NULL){
        for(int i = 0; i < tamanho; i++){
            free(campo[i]);
        }
        free(campo);
    }
}

int **AlocarMatriz(int tam){
    int **matriz = (int **)malloc(tam * sizeof(int *));

    if(matriz == NULL){
        printf("ERRO\n");
        return NULL;
    }

    for(int i = 0; i < tam; i++){
        matriz[i] = (int *)malloc(tam * sizeof(int));

        if(matriz[i] == NULL){
            for(int j = 0; j < i; j++){
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }

        for(int j = 0; j < tam; j++){
            matriz[i][j] = AGUA;
        }
    }

    return matriz;
}

void mostrarmatriz(int **matriz, int tam, const char *titulo){
    printf("\n%s\n", titulo);

    printf("  ");
    for(int j = 0; j < tam; j++){
        printf("%d ", j);
    }
    printf("\n");

    for(int i = 0; i < tam; i++){
        printf("%d ", i);
        for(int j = 0; j < tam; j++){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void posicionarNaviosAleatorio(int **campo, Navio *navios, int tamanho, int qtd_navios) {
    for (int i = 0; i < qtd_navios; i++) {
        int posicionado = 0;

        while (!posicionado) {
            int orientacao = rand() % 2;

            int lin1 = rand() % tamanho;
            int col1 = rand() % tamanho;

            int lin2 = lin1 + (orientacao == 1 ? 1 : 0);
            int col2 = col1 + (orientacao == 0 ? 1 : 0);

            if (lin2 < tamanho && col2 < tamanho) {
                if (!verificaSobreposicao(campo, tamanho, lin1, col1) &&
                    !verificaSobreposicao(campo, tamanho, lin2, col2)) {

                    navios[i].linha[0] = lin1;
                    navios[i].coluna[0] = col1;
                    navios[i].linha[1] = lin2;
                    navios[i].coluna[1] = col2;
                    navios[i].partesAtingidas = 0;
                    navios[i].afundado = 0;

                    campo[lin1][col1] = NAVIO;
                    campo[lin2][col2] = NAVIO;

                    posicionado = 1;
                }
            }
        }
    }
}

int main(){
    srand((unsigned int)time(NULL));

    printf("1 = Novo Jogo\n");
    printf("2 = Configurar\n");
    printf("3 = Carregar ultimo Jogo\n");
    printf("4 = Sair\n");

    int n;
    int **seu_campo = NULL;
    int **campo_visao_adversario = NULL;
    int **campo_computador = NULL;
    int **visao_computador = NULL;

    scanf("%d", &n);

    switch(n){

        case 1:
            printf("Novo Jogo\n");

            seu_campo = AlocarMatriz(TAMANHO);
            campo_visao_adversario = AlocarMatriz(TAMANHO);
            campo_computador = AlocarMatriz(TAMANHO);
            visao_computador = AlocarMatriz(TAMANHO);

            Navio navios_jogador[QTD_NAVIOS];
            Navio navios_computador[QTD_NAVIOS];
            
            if(campo_visao_adversario != NULL && visao_computador != NULL){
                for(int i = 0; i < TAMANHO; i++){
                    for(int j = 0; j < TAMANHO; j++){
                        campo_visao_adversario[i][j] = AGUADESCONHECIDA;
                        visao_computador[i][j] = AGUADESCONHECIDA;
                    }
                }
            }

            if(seu_campo != NULL && campo_visao_adversario != NULL && campo_computador != NULL && visao_computador != NULL){
                
                posicionarNaviosAleatorio(seu_campo, navios_jogador, TAMANHO, QTD_NAVIOS);
                posicionarNaviosAleatorio(campo_computador, navios_computador, TAMANHO, QTD_NAVIOS);

                printf("\nMatrizes alocadas e navios de ambos posicionados com sucesso!\n");

                mostrarmatriz(seu_campo, TAMANHO, "SEU CAMPO (NAVIOS = 1)");
                mostrarmatriz(campo_visao_adversario, TAMANHO, "CAMPO DO ADVERSARIO (ESCONDIDO)");

                limpar_campo(seu_campo, TAMANHO);
                limpar_campo(campo_visao_adversario, TAMANHO);
                limpar_campo(campo_computador, TAMANHO);
                limpar_campo(visao_computador, TAMANHO);
            }
            break;

        case 2:
            printf("Configurar\n");
            break;

        case 3:
            printf("Carregar ultimo jogo\n");
            break;

        case 4:
            printf("Sair\n");
            break;

        default:
            printf("Nenhuma opcao encontrada.\n");
            break;
    }

    return 0;
}