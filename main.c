#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AGUA 0
#define AGUADESCONHECIDA 7
#define TAMANHO 8
#define NAVIO 1
#define NAVIOATINGIDO 2
typedef struct {
    int tamanho;
    int quantidade_navios;
} Config;

int **AlocarMatriz(int tam){
    int **m = malloc(tam * sizeof(int*));
    for(int i = 0; i < tam; i++){
        m[i] = malloc(tam * sizeof(int));
        for(int j = 0; j < tam; j++) m[i][j] = AGUA;
    }
    return m;
}
void limpar_campo(int **m, int tam){
    for(int i = 0; i < tam; i++) free(m[i]);
    free(m);
}
void salvarConfig(Config cfg){
    FILE *f = fopen("config.bin","wb");
    if(f){
        fwrite(&cfg,sizeof(Config),1,f);
        fclose(f);
    }
}
Config carregarConfig(){
    Config cfg = {8,4};
    FILE *f = fopen("config.bin","rb");
    if(f){
        fread(&cfg,sizeof(Config),1,f);
        fclose(f);
    }
    return cfg;
}
void salvarJogo(int **a, int **b, int tam){
    FILE *f = fopen("save.bin","wb");
    fwrite(&tam,sizeof(int),1,f);

    for(int i=0;i<tam;i++) fwrite(a[i],sizeof(int),tam,f);
    for(int i=0;i<tam;i++) fwrite(b[i],sizeof(int),tam,f);

    fclose(f);
}
void carregarJogo(int ***a, int ***b, int *tam){
    FILE *f = fopen("save.bin","rb");
    if(!f){
        printf("Sem save!\n");
        return;
    }

    fread(tam,sizeof(int),1,f);
    *a = AlocarMatriz(*tam);
    *b = AlocarMatriz(*tam);

    for(int i=0;i<*tam;i++) fread((*a)[i],sizeof(int),*tam,f);
    for(int i=0;i<*tam;i++) fread((*b)[i],sizeof(int),*tam,f);

    fclose(f);
}
void mostrarCampo(int **m, int tam){
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            if(m[i][j]==AGUA) printf("~ ");
            else if(m[i][j]==NAVIO) printf("O ");
            else if(m[i][j]==NAVIOATINGIDO) printf("X ");
            else printf("7 ");
        }
        printf("\n");
    }
}
void posicionarBarcos(int **c, int tam, int qtd){
    for(int i=0;i<qtd;i++){
        while(1){
            int l=rand()%tam, col=rand()%tam, d=rand()%2;

            if(d==0 && col<tam-1 &&
               c[l][col]==AGUA && c[l][col+1]==AGUA){
                c[l][col]=c[l][col+1]=NAVIO;
                break;
            }
            if(d==1 && l<tam-1 &&
               c[l][col]==AGUA && c[l+1][col]==AGUA){
                c[l][col]=c[l+1][col]=NAVIO;
                break;
            }
        }
    }
}
int verificarVitoria(int **c,int tam){
    for(int i=0;i<tam;i++)
        for(int j=0;j<tam;j++)
            if(c[i][j]==NAVIO) return 0;
    return 1;
}
void turnoJogador(int **inimigo,int **visao,int tam){
    for(int i=0;i<3;i++){
        int pos;
        printf("Ataque %d (-1 sair): ",i+1);
        scanf("%d",&pos);

        if(pos==-1) exit(0);

        int l=pos/tam, c=pos%tam;

        if(l<0||l>=tam||c<0||c>=tam){
            printf("Invalido!\n"); i--; continue;
        }

        if(visao[l][c]!=AGUADESCONHECIDA){
            printf("Ja atacado!\n"); i--; continue;
        }

        if(inimigo[l][c]==NAVIO){
            printf("Impacto!\n");
            inimigo[l][c]=NAVIOATINGIDO;
            visao[l][c]=NAVIOATINGIDO;
        } else{
            printf("Agua!\n");
            visao[l][c]=AGUA;
        }
    }
}
void turnoComputador(int **seu,int **visao,int tam){
    int a=0;
    while(a<3){
        int l=rand()%tam,c=rand()%tam;

        if(visao[l][c]==AGUADESCONHECIDA){
            if(seu[l][c]==NAVIO){
                printf("Computador acertou!\n");
                seu[l][c]=NAVIOATINGIDO;
                visao[l][c]=NAVIOATINGIDO;
            } else{
                printf("Computador errou!\n");
                visao[l][c]=AGUA;
            }
            a++;
        }
    }
}
int main(){

    srand(time(NULL));
    Config cfg = carregarConfig();
    int op;

    while(1){

        printf("\n1 Novo\n2 Config\n3 Carregar\n4 Sair\n");
        scanf("%d",&op);

        if(op==1){
            int tam = cfg.tamanho;

            int **seu=AlocarMatriz(tam);
            int **ini=AlocarMatriz(tam);
            int **v1=AlocarMatriz(tam);
            int **v2=AlocarMatriz(tam);

            posicionarBarcos(seu,tam,cfg.quantidade_navios);
            posicionarBarcos(ini,tam,cfg.quantidade_navios);

            for(int i=0;i<tam;i++)
                for(int j=0;j<tam;j++)
                    v1[i][j]=v2[i][j]=AGUADESCONHECIDA;

            while(1){
                printf("\nSEU CAMPO\n");
                mostrarCampo(seu,tam);

                printf("\nSUA VISAO\n");
                mostrarCampo(v1,tam);

                turnoJogador(ini,v1,tam);
                if(verificarVitoria(ini,tam)){
                    printf("VOCE VENCEU\n"); break;
                }

                turnoComputador(seu,v2,tam);
                if(verificarVitoria(seu,tam)){
                    printf("VOCE PERDEU\n"); break;
                }

                salvarJogo(seu,ini,tam);
            }
            limpar_campo(seu,tam);
            limpar_campo(ini,tam);
            limpar_campo(v1,tam);
            limpar_campo(v2,tam);
        }
        else if(op==2){
            printf("Tamanho: ");
            scanf("%d",&cfg.tamanho);

            printf("Navios: ");
            scanf("%d",&cfg.quantidade_navios);

            salvarConfig(cfg);
        }
        else if(op==3){
            int **a,**b,t;
            carregarJogo(&a,&b,&t);
            mostrarCampo(a,t);
            limpar_campo(a,t);
            limpar_campo(b,t);
        }

        else break;
    }
    return 0;
}