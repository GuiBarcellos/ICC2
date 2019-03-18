/* 
   Disciplina: SCC0201 - Introdução à Ciência da Computação II - Professor Moacir Antonelli Ponti
   Trabalho 3 - Corretor Automatico
   Nome: Guilherme Targon Marques Barcellos
   Num. USP: 10724181	
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>   

#define MAX_WORDS 600000 // Numero maximo de palavras

typedef struct PALAVRAS{        
	char** word;			    //Vetor de strings(palavras).
	int npalavras;				//Variavel controle quantidade de palavras armazenadas na struct.
}PALAVRAS;


void lendo_dicionario(FILE* arquivo, PALAVRAS* p){//Funcao que sera usada para ler o .txt(dicionario). Recebe o arquivo de
												  //onde sera lido as palavras e tambem a struct para que possamos armazena-las.
	char aux[50]; //Aux para guardar uma palavra por vez.
	int j = 0;    //Variavel de controle, para definir em qual posicao do vetor de strings a palavra sera guardada.
	
	/*Aqui é onde comeca a leitura do arquivo. Essa mascara do fscanf le tudo que esta entre a-z(minusc) e A-Z(maiusc).Alem 
		disso, le e ignora(*) tudo o que nao estiver entre esse range, portanto, ignora tudo que nao for uma letra. Ignora
		simbolos como *, #, !, ", ', entre outros.
	*/
	while(fscanf(arquivo, "%[a-zA-Z]%*[^a-zA-Z]", aux) != EOF){  
			
			for(int i = 0; aux[i]!= '\0'; i++){			//Neste for coloco a palavra para lower case. A palavra inteira vai
				if(aux[i] >= 'A' && aux[i] <= 'Z'){		//ser armazenada com letras minusculas. Isso pq a comparacao independe
					aux[i]+=32;							//se a palavra esta com letra maiscula/minuscula, entao comparamos 
				}										//todas minusculas.
				
			}
			strcpy(p->word[j], aux);    //Copiando a ultima palavra lida(aux) para o vetor de strings dentro da struct.
			j++;                        //Incremento variavel de controle
			p->npalavras++;			  	//Incremento variavel dentro da struct, que controla quantidade palavras.
	}
}




void mergesort (PALAVRAS* p, int inicio, int fim,char** vetaux) {  //Mergesort, algoritmo do tipo dividir p/ conquistar.
																   //Pior caso O(nlogn).
	if (fim <= inicio) return; // caso de parada (vetor 1 posicao).

	int centro = ((fim+inicio)/2.0);									//Algoritmo faz varias divisoes, ate q chegue no caso parada(vet 1 pos). E utiliza vetor
																		//auxiliar para ir ordenando, ao juntar "pedacos pequenos do vetor".
	
	mergesort(p, inicio, centro,vetaux);    // Primeira metade.
	mergesort(p, centro+1, fim,vetaux); 	// Segunda metade.

	
	int i = inicio;	  // Primeira metade (ini ate centro)
	int j = centro+1; // Segunda metade (centro+1 ate fim)
 	int k = 0;	  // indice do vetor auxiliar (0 ate fim-ini) - Vet recebido por parametro

	// comparar elementos das duas listas  
	while (i <= centro && j <= fim) {
		if (strcmp(p->word[i], p->word[j]) <= 0) {                 
			strcpy(vetaux[k], p->word[i]);				
			i++; // proximo elemento da lista 1
		} 
		else {
			strcpy(vetaux[k], p->word[j]);	
			j++; // proximo elemento da lista 2
		}
		k++; // proximo elemento do aux
	}

	// copia os restantes da l1
	while (i <= centro) {
		strcpy(vetaux[k], p->word[i]);	
		i++; k++;
	}	
	// copia os restantes da l2
	while (j <= fim) {
		strcpy(vetaux[k], p->word[j]);	
		j++; k++;
	}	


	for (k = 0; k <= fim-inicio;k++) {
		strcpy(p->word[k+inicio], vetaux[k]);	
	}

	
}

void filtra_repetidas (PALAVRAS* lida, PALAVRAS* filtro){  
	
																	//Funcao para filtrar palavras repetidas. Recebemos como
	int k = 0;														//parametro uma struct onde as palavras lidas do dicionario
	strcpy(filtro->word[0],lida->word[0]);							//foram armazenadas(lida). É feita a "transferencia" para
	filtro->npalavras++;											//a outra struct recebida como parametro(filtro), porem
	for(int i = 1; i < lida->npalavras;i++){						//sem repeticao de palavras.
		if(strcmp(lida->word[i], filtro->word[k]) != 0){
				k++;
				strcpy(filtro->word[k], lida->word[i]);
				filtro->npalavras++;
		}
	}
}

int busca(PALAVRAS* dicionario, char* chave){   //Funcao busca binaria, mais eficiente que comparar palavra por palavra.
	
	int inicio = 0;   							//Nesta funcaoi, basicamente dividimos o vetor em 2. Comparamos a chave com 
    int meio = 0;        						//a posicao central. Caso seja igual retorna 10(encontrou). Caso contrario,
    int fim = (dicionario->npalavras) - 1;    	//verifica com o strcmp se a palavra pode estar do meio para o fim ou do 
	int aux = 20;								//inicio para o meio(OBS: Está ordenado).E segue essa verificacao ate que	
												//encontre a chave OU inicio<=fim.
    while(inicio <= fim){
        meio = (inicio + fim)/2;
        if (strcmp(dicionario->word[meio], chave) == 0){
			aux = 10;  //Encontrou a palavra e retorna 10. Busca finaliza aqui.
			break;
		}
        else if (strcmp(dicionario->word[meio], chave) > 0){
            fim    = meio - 1;
		}
        else if (strcmp(dicionario->word[meio], chave) < 0){
            inicio = meio + 1;
        }
    }
    
    return aux; //Busca foi realizada e chave nao foi encontrada. Retorna 20.
	
}

void leitura_tweets(FILE* tweet, PALAVRAS* dicionario){   //Funcao para leitura dos tweets. Ja e verificado se cada palavra
														  //lida aqui esta presente no dicionario. Caso nao esteja, é printada
	char aux;     //Leitura comeca char por char. Variavel auxiliar.
	int verificatweet = 0;  //Caso nao haja tweets para ler, verificatweet == 0 e printa "No tweets to check"
	char palav[50];
	char palavminusc[50];
	int achou = 0;
	while(fscanf(tweet, "%c", &aux) != EOF){   //Le char por char ate final do arquivo
		if(aux == '"'){
			fscanf(tweet, "%c", &aux);				//No .json, os tweets estao sempre localizados depois de: ("text": ").
			if(aux == 't'){							//A estrategia utilizada foi checar char por char, e se chegarmos em
				fscanf(tweet, "%c", &aux);			//("te), ja e possivel saber que existe um tweet pela frente a ser lido.
				if(aux == 'e'){						//Uso do %*c para ler e ignorar: (xt": "). Assim, o "ponteiro" de leitura
					fscanf(tweet, "%*c");			//para exatamente onde comeca o tweet.
					fscanf(tweet, "%*c");
					fscanf(tweet, "%*c");
					fscanf(tweet, "%*c");
					fscanf(tweet, "%*c");
					fscanf(tweet, "%*c");
					verificatweet++;                //Incrementa pois existe pelo menos um tweet.
					while(fscanf(tweet, "%[a-zA-Z]%*[^a-zA-Z][^\n]", palav) != EOF){  //Mesma estrategia do le dicionario.
						if(strcmp(palav, "truncated")==0)break;  //Fim do tweet.
						strcpy(palavminusc, palav);
						for(int l = 0; l<50; l++){					//COlocando tudo minusc. para fazer a busca no dicionario.
							if(palavminusc[l] >= 'A' && palavminusc[l] <= 'Z'){
									palavminusc[l]+=32;
							}
						}
						achou = busca(dicionario, palavminusc);  //Faz a busca no dicionario. Retorno 20 = nao encontrou.
						if(achou == 20){
							printf("%s ", palav);   //Caso nao tenha encontrado, printa a palavra(da forma que esta tweet).
						}
					}
					printf("\n");  //Separar tweets com \n
				}
			}
		}
		
	}
	if(verificatweet == 0){   //Nenhum tweet lido
		printf("No tweets to check\n");
	}
	
}



int main(){

	char nomedicionario[200], nometweets[200];    //Para armazenar a entrada do programa(nome dos .txt e .json)
	scanf("%s", nomedicionario);
	scanf("%s", nometweets);
	FILE *DICIONARIO, *TWEETS;
	PALAVRAS *palav_lida;         //Struct que sera usada para armazenar as palavras lidas do "dicionario".
	PALAVRAS *palav_filtrada;     //Dicionario filtrado, isto e, sem repeticoes.
	
	
	
	palav_lida = (PALAVRAS*) malloc(sizeof(PALAVRAS));
	palav_lida->word = (char**) malloc(sizeof(char*) * MAX_WORDS);				//Alocacao dinamina das structs e do vetor
	for(int u = 0; u<MAX_WORDS; u++){											//de strigs presente nelas. MAX_WORDS linhas
		palav_lida->word[u] = (char*) malloc(sizeof(char)*50);					// e 50 colunas(max 50 caracteres).
	}
	palav_filtrada = (PALAVRAS*) malloc(sizeof(PALAVRAS));
	palav_filtrada->word = (char**) malloc(sizeof(char*) * MAX_WORDS);
	for(int u = 0; u<MAX_WORDS; u++){
		palav_filtrada->word[u] = (char*) malloc(sizeof(char)*50);
	}
	
	
	DICIONARIO = fopen(nomedicionario, "r");   //Abrindo os arquivos para leitura.
	TWEETS = fopen(nometweets, "r");

	char** vetaux = (char**) malloc(sizeof(char*) * MAX_WORDS);
	for(int u = 0; u<MAX_WORDS; u++){								//Alocando vetor auxiliar que sera usado no mergesort.
		vetaux[u] = (char*) malloc(sizeof(char)*50);
	}
	
	
	
	lendo_dicionario(DICIONARIO, palav_lida);
	mergesort(palav_lida, 0, palav_lida->npalavras-1,vetaux);
	filtra_repetidas(palav_lida, palav_filtrada);
	leitura_tweets(TWEETS, palav_filtrada);
	
	
	
	for(int q = 0; q<MAX_WORDS; q++)free(vetaux[q]);              //Liberando toda a memoria alocada e fechando os arquivos.
	free(vetaux);
	
	for(int q = 0; q<MAX_WORDS; q++)free(palav_lida->word[q]);
	free(palav_lida->word);

	for(int q = 0; q<MAX_WORDS; q++)free(palav_filtrada->word[q]);
	free(palav_filtrada->word);
	
	free(palav_lida);
	free(palav_filtrada);
	fclose(DICIONARIO);
	fclose(TWEETS);
	
	return 0;
}

