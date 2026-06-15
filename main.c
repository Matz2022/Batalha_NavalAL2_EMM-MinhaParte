#include <stdio.h>

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
}