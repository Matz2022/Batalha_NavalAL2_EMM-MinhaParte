#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define AGUA 0
#define AGUADESCONHECIDA 7
#define TAMANHO 8
#define NAVIO 1
#define NAVIOATINGIDO 2
//BNVEMM007 - tamanho do jogo e quantidades de barco na matriz
typedef struct {
    int tamanho;  
    int quantidade_navios;  
} Config;
//BNVEMM007 - criar linha e criar colunas com vetor de inteiros
int **AlocarMatriz(int tam_matriz){
    int **m = malloc(tam_matriz * sizeof(int*));
    for(int i = 0; i < tam_matriz; i++){ //percorrer cada linha
        m[i] = malloc(tam_matriz * sizeof(int)); //vetor de inteiro para cada linha
        for(int j = 0; j < tam_matriz; j++) m[i][j] = AGUA; //
    }
    return m;
}
//BNVEMM007 - liberar memoria de cada linha da matriz
void limpar_campo(int **m, int tam_matriz){
    for(int i = 0; i < tam_matriz; i++) free(m[i]);
    free(m);
}
//BNVEMM007 - salvar config e gravar diretamente do arquivo binario.
void salvarConfig(Config cfg){
    FILE *f = fopen("config.bin","wb");
    if(f){
        fwrite(&cfg,sizeof(Config),1,f); //verificar e gravar a estruct do arq.
        fclose(f);
    }
}
//BNVEMM007 - inicio com valores padrao tam_matriz 8 e 4 nav, 
Config carregarConfig(){
    Config cfg = {8,4};
    
    FILE *f = fopen("config.bin","rb"); // leitura de arquivo binario
        if(f){  //abertura do arquivo != NULL
        fread(&cfg,sizeof(Config),1,f);
        fclose(f);
    }
    return cfg; 
}
//BNVEMM007 - salva jogo atual mat A mat B
void salvarJogo(int **a, int **b, int tam_matriz){
    FILE *f = fopen("save.bin","wb");
    fwrite(&tam_matriz,sizeof(int),1,f);

    for(int i=0;i<tam_matriz;i++) fwrite(a[i],sizeof(int),tam_matriz,f);
    for(int i=0;i<tam_matriz;i++) fwrite(b[i],sizeof(int),tam_matriz,f);

    fclose(f);
}
//BNVEMM009 - carregar jogo salvo
void carregarJogo(int ***a, int ***b, int *tam_matriz){
    FILE *f = fopen("save.bin","rb");// abre arquivo

    if(!f){ 
        printf("Sem save!\n");
        return;
    }
    fread(tam_matriz,sizeof(int),1,f); // lê tamanho

    *a = AlocarMatriz(*tam_matriz); // cria matriz A
    *b = AlocarMatriz(*tam_matriz); // cria matriz B

    for(int i=0;i<*tam_matriz;i++) 
        fread((*a)[i],sizeof(int),*tam_matriz,f); // lê matriz A

    for(int i=0;i<*tam_matriz;i++) 
        fread((*b)[i],sizeof(int),*tam_matriz,f); // lê matriz B
    fclose(f); 
}
//BNVEMM007 - mostrar campo e percorrer as linhas
void mostrarCampo(int **m, int tam_matriz){
    for(int i=0;i<tam_matriz;i++){
        for(int j=0;j<tam_matriz;j++){
            if(m[i][j]==AGUA) printf("~ ");
            else if(m[i][j]==NAVIO) printf("O ");
            else if(m[i][j]==NAVIOATINGIDO) printf("X ");
            else printf("7 ");
        }
        printf("\n");
    }
}
//BNVEMM007 Posicionar navios aleatoriamente e quantidade.
void posicionarBarcos(int **c, int tam_matriz, int qtd){

    for(int i=0;i<qtd;i++){ 
        while(1){

            int l=rand()%tam_matriz;     // linha aleatória
            int col=rand()%tam_matriz;  // coluna aleatória
            int d=rand()%2;      // direção (0 ou 1)

            if(d==0 && col<tam_matriz-1 &&
               c[l][col]==AGUA && c[l][col+1]==AGUA){

                c[l][col]=c[l][col+1]=NAVIO;
                break;
            }
            if(d==1 && l<tam_matriz-1 &&
               c[l][col]==AGUA && c[l+1][col]==AGUA){

                c[l][col]=c[l+1][col]=NAVIO;
                break;
            }
        }
    }
}
int verificarVitoria(int **c,int tam_matriz){
    for(int i=0;i<tam_matriz;i++)
        for(int j=0;j<tam_matriz;j++)
            if(c[i][j]==NAVIO) return 0;
    return 1;
}
void turnoJogador(int **inimigo,int **visao,int tam_matriz){
    for(int i=0;i<3;i++){
        int pos;
        printf("Ataque %d (-1 sair): ",i+1);
        scanf("%d",&pos);

        if(pos==-1) exit(0);

        int l=pos/tam_matriz, c=pos%tam_matriz;

        if(l<0||l>=tam_matriz||c<0||c>=tam_matriz){
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
void turnoComputador(int **seu,int **visao,int tam_matriz){
    int a=0;
    while(a<3){
        int l=rand()%tam_matriz,c=rand()%tam_matriz;

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
//BNVEMM009 - iniciar o jogo
void iniciarJogo(Config cfg){
    int tam_matriz = cfg.tamanho;

    int **seu = AlocarMatriz(tam_matriz);
    int **ini = AlocarMatriz(tam_matriz);
    int **v1  = AlocarMatriz(tam_matriz);
    int **v2  = AlocarMatriz(tam_matriz);
    
    posicionarBarcos(seu,tam_matriz,cfg.quantidade_navios);
    posicionarBarcos(ini,tam_matriz,cfg.quantidade_navios);

    for(int i=0;i<tam_matriz;i++)
        for(int j=0;j<tam_matriz;j++)
            v1[i][j] = v2[i][j] = AGUADESCONHECIDA;

    while(1){
        printf("\nSEU CAMPO\n");
        mostrarCampo(seu,tam_matriz);

        printf("\nSUA VISAO\n");
        mostrarCampo(v1,tam_matriz);

        turnoJogador(ini,v1,tam_matriz);
        if(verificarVitoria(ini,tam_matriz)){
            printf("VOCE VENCEU!\n");
            break;
        }

        turnoComputador(seu,v2,tam_matriz);
        if(verificarVitoria(seu,tam_matriz)){
            printf("VOCE PERDEU!\n");
            break;
        }

        salvarJogo(seu,ini,tam_matriz);
    }

    limpar_campo(seu,tam_matriz);
    limpar_campo(ini,tam_matriz);
    limpar_campo(v1,tam_matriz);
    limpar_campo(v2,tam_matriz);
}
//BNVEMM009 pede o tamanho da mat e qtd de navios e salva
void configurarJogo(Config *cfg){
    printf("Tamanho do campo: ");
    scanf("%d",&cfg->tamanho);

    //BNVEMM010 - Validando o Tamnho dos Campos
    if(cfg->tamanho < 8 ){
        cfg->tamanho = 8;//Designando um valor fixo para o tamanho dos campos, caso o valor seja inferior a 8.
        printf("Campo Informado Inferior a 8!\nAVISO MARUJO: Valor dos Campos foi resetado para 8.\n");
    }

    printf("Quantidade de navios: ");
    scanf("%d",&cfg->quantidade_navios);

    //BNVEMM010 - Validando a Qtd. de Navios
    if(cfg->quantidade_navios < 4){
        cfg->quantidade_navios = 4; //Designando um valor fixo de navios, caso o valor seja inferior a 4.
        printf("Qtd. de Navios Informado Inferior a 4!\nATENCAO MARUJO: Qtd. de Navios foi resetada para 4 navios para cada Jogador.\n");
    } else if (cfg->quantidade_navios > 20){
        printf("\n PERIGO AVISTA MARUJO: Com essa Qtd. de Navios na Frota, podemos ter lentidao ao Carregar um Novo Jogo!!!\n");
    }

    salvarConfig(*cfg);
}
//BNVEMM009 carregar o jogo salvo do arquivo e mostra o campo carregado
void carregarPartida(){
    int **a, **b, tam_matriz;

    carregarJogo(&a,&b,&tam_matriz);

    printf("\nJogo carregado:\n");
    mostrarCampo(a,tam_matriz);

    limpar_campo(a,tam_matriz);
    limpar_campo(b,tam_matriz);
}

int main(){
    srand(time(NULL));

    Config cfg = carregarConfig();
    int op;

    do{
        printf("\n1 - Novo Jogo\n");
        printf("2 - Configurar\n");
        printf("3 - Carregar Jogo\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        scanf("%d",&op);

        switch(op){

            case 1:
                iniciarJogo(cfg);
                break;

            case 2:
                configurarJogo(&cfg);
                break;

            case 3:
                carregarPartida();
                break;

            case 4:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    }while(op != 4);

    return 0;
}