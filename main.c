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
                printf("? ");
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

void atirar(int **campo, int **visao, int tam) {
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
int main(){
    printf("1 = Novo Jogo\n 2 = Configurar\n 3 = Carregar ultimo Jogo\n 4 = Sair\n");
			  
		int n;
		scanf("%d", &n);

		    switch (n){
			case 1:
			printf("Novo Jogo\n");
				break;
            
			case 2:
			printf("Configurar\n");
			    break;
			case 3:
			printf("Carregar ultimo jogo\n");
			    break;
			case 4:
			printf("Sair\n");
			
			default:
			printf("nenhuma opção encontrada\n");
				break;
			}
    	printf("teste Mateus 14/06");
		printf("teste2 Mateus 14/06");
	
    printf("1 = Novo Jogo\n");
    printf("2 = Configurar\n");
    printf("3 = Carregar ultimo Jogo\n");
    printf("4 = Sair\n");
	srand(time(NULL));
   
    int **seu_campo = NULL;
    int **campo_visao_adversario = NULL;

    scanf("%d", &n);

    switch(n){

        case 1:

            printf("Novo Jogo\n");

            seu_campo = AlocarMatriz(TAMANHO);
            campo_visao_adversario = AlocarMatriz(TAMANHO);

			posicionarBarcos(seu_campo, TAMANHO);

            if(campo_visao_adversario != NULL){
                for(int i = 0; i < TAMANHO; i++){
                    for(int j = 0; j < TAMANHO; j++){
                        campo_visao_adversario[i][j] = AGUADESCONHECIDA;
                    }
                }
            }

            if(seu_campo != NULL && campo_visao_adversario != NULL){

                printf("\nMatrizes alocadas com sucesso!\n");

                printf("\nSEU CAMPO:\n");
				mostrarCampo(seu_campo, TAMANHO);
                printf("\nCAMPO DE VISAO DO ADVERSARIO:\n");
				mostrarCampo(campo_visao_adversario, TAMANHO);
				
				for(int i = 0; i < 5; i++) {
   				mostrarCampo(campo_visao_adversario, TAMANHO);
  				atirar(seu_campo, campo_visao_adversario, TAMANHO);
				}
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