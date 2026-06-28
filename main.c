#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AGUA 0
#define AGUADESCONHECIDA 7
#define TAMANHO 8
#define NAVIO 1
#define NAVIOATINGIDO 2

void limpar_campo(int **campo, int tamanho){
    if(campo != NULL){
        for(int i = 0; i < tamanho; i++){
            free(campo[i]);
        }
        free(campo);
    }
}
void mostrarCampo(int **matriz, int tam) {
    for(int i = 0; i < tam; i++) {
        for(int j = 0; j < tam; j++) {

            if(matriz[i][j] == AGUA)
                printf("~ ");
            else if(matriz[i][j] == NAVIO)
                printf("O ");
            else if(matriz[i][j] == AGUADESCONHECIDA)
                printf("7 ");
            else if(matriz[i][j] == NAVIOATINGIDO)
                printf("X ");
        }
        printf("\n");
    }
}
void posicionarBarcos(int **campo, int tam) {

    int linha, coluna, direcao;

    for (int i = 0; i < 4; i++) {

        while (1) {

            linha = rand() % tam;
            coluna = rand() % tam;
            direcao = rand() % 2; 

            if (direcao == 0) {
                if (coluna < tam - 1 &&
                    campo[linha][coluna] == AGUA &&
                    campo[linha][coluna + 1] == AGUA) {

                    campo[linha][coluna] = NAVIO;
                    campo[linha][coluna + 1] = NAVIO;
                    break;
                }
            } else {
                if (linha < tam - 1 &&
                    campo[linha][coluna] == AGUA &&
                    campo[linha + 1][coluna] == AGUA) {

                    campo[linha][coluna] = NAVIO;
                    campo[linha + 1][coluna] = NAVIO;
                    break;
                }
            }
        }
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

void atirarJogador(int **campo, int **visao, int tam) {
    int linha, coluna;

    printf("Digite linha e coluna: ");
    scanf("%d %d", &linha, &coluna);

    if (campo[linha][coluna] == NAVIO) {
        printf("Acertou!\n");
        campo[linha][coluna] = NAVIOATINGIDO;
        visao[linha][coluna] = NAVIOATINGIDO;
    } else {
        printf("Errou!\n");
        visao[linha][coluna] = AGUA;
    }
}
void ataqueComputador(int **campo, int **visao, int tam) {
    int linha, coluna;

    while (1) {
        linha = rand() % tam;
        coluna = rand() % tam;

        if (visao[linha][coluna] == AGUADESCONHECIDA) {
            printf("\nComputador atirou em: %d %d\n", linha, coluna);

            if (campo[linha][coluna] == NAVIO) {
                printf("Computador acertou!\n");
                campo[linha][coluna] = NAVIOATINGIDO;
                visao[linha][coluna] = NAVIOATINGIDO;
            } else {
                printf("Computador errou!\n");
                visao[linha][coluna] = AGUA;
            }
            break;
        }
    }
}
int main() {
    int n;

    srand(time(NULL));

    printf("1 = Novo Jogo\n");
    printf("2 = Configurar\n");
    printf("3 = Carregar ultimo Jogo\n");
    printf("4 = Sair\n");

    scanf("%d", &n);

    int **seu_campo = NULL;
    int **campo_inimigo = NULL;
    int **sua_visao = NULL;
    int **visao_computador = NULL;

    switch(n){

        case 1:
            printf("Novo Jogo\n");

            seu_campo = AlocarMatriz(TAMANHO);
            campo_inimigo = AlocarMatriz(TAMANHO);
            sua_visao = AlocarMatriz(TAMANHO);
            visao_computador = AlocarMatriz(TAMANHO);

            posicionarBarcos(seu_campo, TAMANHO);
            posicionarBarcos(campo_inimigo, TAMANHO);

            for(int i = 0; i < TAMANHO; i++){
                for(int j = 0; j < TAMANHO; j++){
                    sua_visao[i][j] = AGUADESCONHECIDA;
                    visao_computador[i][j] = AGUADESCONHECIDA;
                }
            }

            if(seu_campo && campo_inimigo && sua_visao && visao_computador){

                printf("\nSEU CAMPO:\n");
                mostrarCampo(seu_campo, TAMANHO);

                for(int turno = 0; turno < 5; turno++) {

                    printf("\n=== SUA VEZ ===\n");
                    mostrarCampo(sua_visao, TAMANHO);
                    atirarJogador(campo_inimigo, sua_visao, TAMANHO);

                    printf("\n=== VEZ DO COMPUTADOR ===\n");
                    ataqueComputador(seu_campo, visao_computador, TAMANHO);

                    printf("\nSEU CAMPO ATUAL:\n");
                    mostrarCampo(seu_campo, TAMANHO);
                }

                limpar_campo(seu_campo, TAMANHO);
                limpar_campo(campo_inimigo, TAMANHO);
                limpar_campo(sua_visao, TAMANHO);
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