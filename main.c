#include <stdio.h>
#include <stdlib.h>


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

int main(){

    printf("1 = Novo Jogo\n");
    printf("2 = Configurar\n");
    printf("3 = Carregar ultimo Jogo\n");
    printf("4 = Sair\n");

    int n;
    int **seu_campo = NULL;
    int **campo_visao_adversario = NULL;

    scanf("%d", &n);

    switch(n){

        case 1:

            printf("Novo Jogo\n");

            seu_campo = AlocarMatriz(TAMANHO);
            campo_visao_adversario = AlocarMatriz(TAMANHO);

            if(campo_visao_adversario != NULL){
                for(int i = 0; i < TAMANHO; i++){
                    for(int j = 0; j < TAMANHO; j++){
                        campo_visao_adversario[i][j] = AGUADESCONHECIDA;
                    }
                }
            }

            if(seu_campo != NULL && campo_visao_adversario != NULL){

                printf("\nMatrizes alocadas com sucesso!\n");

                mostrarmatriz(seu_campo, TAMANHO, "SEU CAMPO");
                mostrarmatriz(campo_visao_adversario, TAMANHO, "CAMPO DE VISAO DO ADVERSARIO");

                limpar_campo(seu_campo, TAMANHO);
                limpar_campo(campo_visao_adversario, TAMANHO);
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