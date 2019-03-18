#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
	double comprimentosep;				//Struct que armazena as caracteristicas da flor
	double largurasep;					
	double comprimentopet;				
	double largurapet;
	char *especie;
}flor;

typedef struct{
									//Armazena a distancia euclidiana e o nome da flor.
	double disteucl;				
	char esp[100];					
	
}resultado;

int numerolinhas(FILE *arquivo){     //Funcao que retorna o numero de linhas do arquivo "csv" e consequentemente, o "numero de flores"(cada linha do csv é uma amostra)
	int num = 0;
	char caracter;								
	while((caracter = fgetc(arquivo))!= EOF){
		if(caracter == '\n'){
			num++;
		}
	}
	num--; //Retirando a primeira linha do arquivo(cabecalho)
	return num;
}

void lendodados(int nlinhas, FILE* arquivo, flor* aux ){  //Funcao que armazena os dados do csv em um vetor de struct(flor*)
	
	fscanf(arquivo, "%*[^\n]");  //Ignorando a primeira linha de cada arquivo(cabecalho)
	
	for(int i =0; i<nlinhas; i++){
			fscanf(arquivo, "%lf, %lf, %lf, %lf, \"%m[^\"]\"", &aux[i].comprimentosep, &aux[i].largurasep, &aux[i].comprimentopet, &aux[i].largurapet, &aux[i].especie);
	}
}

resultado** alocamatriz(int numlinhas, int numcol){		//Aloca e retorna matriz struct resultado(resultado**)
	resultado** matriz;									//num linhas = tamanho teste
														//num colunas = tamanho train
	matriz = (resultado**) calloc(numlinhas, sizeof(resultado*));
	for(int i = 0; i<numlinhas; i++){
		matriz[i] = (resultado*) calloc(numcol, sizeof(resultado));
	}
	
	return matriz;
}

void distanciaeuclidiana(resultado** distancias, flor* treino, flor* amostra, int tamanhotrain, int tamanhoteste){
//Calcula distancia de cada flor do conjunto teste com todos do conjunto train e armazena na matriz de struct	
	for (int i = 0; i<tamanhoteste; i++){
		for(int j =0; j<tamanhotrain; j++){
			distancias[i][j].disteucl = sqrt((pow(amostra[i].comprimentosep-treino[j].comprimentosep, 2))+(pow(amostra[i].largurasep-treino[j].largurasep, 2))+(pow(amostra[i].comprimentopet-treino[j].comprimentopet, 2))+(pow(amostra[i].largurapet-treino[j].largurapet, 2)));
			strcpy(distancias[i][j].esp, treino[j].especie); //É essencial guardar o nome tambem, para saber com qual especie essa distancia foi calculada
		}
	}	
}

void ordenacao (resultado** distancias, int tamanhotrain, int tamanhoteste){
	
	int controle = 0;			//Nessa funcao as linhas da matriz struct sao ordenadas.
	double aux = 0;	
	char aux2[100];
	while ( controle == 0 ){ //Quando estiver ordenado controle = 1 e sai do while
		controle = 1;
		for( int i = 0; i<tamanhoteste ; i++ ){ //Fixando as linhas
			for( int j = 0; j<tamanhotrain-1; j++ ){ //tamanhotrain-1 pois comparamos [linha][coluna+1]
				if(distancias[i][j].disteucl > distancias[i][j+1].disteucl){   	//Se a coluna sucessora for maior, entra no if e faz as trocas
					aux = distancias[i][j].disteucl;
					strcpy(aux2, distancias[i][j].esp);						
					distancias[i][j].disteucl = distancias[i][j+1].disteucl;
					strcpy(distancias[i][j].esp, distancias[i][j+1].esp); 	
					distancias[i][j+1].disteucl = aux;  //Alem de trocar as distancias(nosso parametro de comparacao), é preciso
					strcpy(distancias[i][j+1].esp, aux2);	//trocar tambem a especie da flor, e para isso strcpy.					
					controle = 0;
				}
			}
		}
	}
}

void analisedistancias(resultado **distancias, flor* amostra, int tamanhotrain, int tamanhoteste, int k){
	
	double acertos = 0; //Acertos do algoritmo
	int setosa = 0;	//Quantidade de vezes que setosa aparece
	int versicolor = 0; // Quantidade vezes versicolor aparece
	int virginica = 0; // Quantidade vezes virginica aparece
	char auxiliar[100];
	
	for(int i = 0; i<tamanhoteste; i++){
		setosa = 0;			//Zerando as variaveis toda vez que mudamos de linha(flor teste)
		versicolor = 0;
		virginica = 0;	
		for(int j = 0; j<k; j++){
			if(strcmp(distancias[i][j].esp, "setosa") == 0){  //Incrementando toda vez que setosa aparece entre as k menores distancias
				setosa++;
			}
			else if(strcmp(distancias[i][j].esp, "versicolor") == 0){ //Incrementando toda vez que versicolor aparece entre as k menores distancias
				versicolor++;
			}
			else if(strcmp(distancias[i][j].esp, "virginica") == 0){ //Incrementando toda vez que virginica aparece entre as k menores distancias
				virginica++;
			}
		}
		if(setosa > versicolor && setosa > virginica){			//Primeiros 3 ifs verificam se ha dominancia de uma das 3 especies, caso
			strcpy(auxiliar, "setosa");							//haja, strcpy para auxiliar, e sera printado logo depois(previsao do algoritmo)
		}
		else if(versicolor > virginica && versicolor > setosa){
			strcpy(auxiliar, "versicolor");
		}
		else if(virginica > setosa && virginica > versicolor){
			strcpy(auxiliar, "virginica");
		}
		else if(setosa == versicolor && setosa == virginica){  //Se houver empate entre as 3 classes, usamos a menor distancia
			strcpy(auxiliar, distancias[i][0].esp);
		}
		else if(setosa == versicolor){							//Ultimos 3 ifs verificam quando ha empate entre 2 das classes, nesses
			for(int l = 0; l<k; l++){							//casos, usamos uma flor q pertence a uma dessas classes empatadas e
				if(strlen(distancias[i][l].esp) == 6){			//menor distancia.
					strcpy(auxiliar, "setosa");
					break;
				}
				else if(strlen(distancias[i][l].esp) == 10){
					strcpy(auxiliar, "versicolor");
					break;
				}
			}
		}
		else if(setosa == virginica){
			for(int l = 0; l<k; l++){
				if(strlen(distancias[i][l].esp) == 6){
					strcpy(auxiliar, "setosa");
					break;
				}
				else if(strlen(distancias[i][l].esp) == 9){
					strcpy(auxiliar, "virginica");
					break;
				}
			}
		}
		else if(virginica == versicolor){
			for(int l = 0; l<k; l++){
				if(strlen(distancias[i][l].esp) == 9){
					strcpy(auxiliar, "virginica");
					break;
				}
				else if(strlen(distancias[i][l].esp) == 10){
					strcpy(auxiliar, "versicolor");
					break;
				}
			}
		}
		
		printf("%s %s\n", auxiliar, amostra[i].especie);  //Printando a previsao do algoritmo e a classe verdadeira(especialista classificou)
		
		if(strcmp(auxiliar, amostra[i].especie) == 0){ //Se forem iguais, houve acerto e incrementa variavel
			acertos++;
		}
	}
	printf("%.4lf\n", acertos/tamanhoteste);  //Printando a taxa de acerto
}

int main(){

	char *train, *teste;    //Armazena entrada do programa(nome dos csv)
	FILE *arquivotrain, *arquivoteste; //Abrir os arquivos
	int k = 0;  //Numero de vizinhos
	int tamanho_train = 0, tamanho_teste = 0; //Numero de linhas de cada arquivo(num linhas = num flores)
	flor *treino, *amostra; //Treino = conjunto dados; Amostra = conjunto de teste
	resultado** distancias; //Matriz resultado para guardar as distancias
	
	scanf("%ms %ms %d", &train, &teste, &k);
	arquivotrain = fopen(train,"r");
	arquivoteste = fopen(teste,"r");
	tamanho_train = numerolinhas(arquivotrain);
	treino = (flor*) calloc(tamanho_train, sizeof(flor));		//Utilizei 2 vetores struct flor, um para teste outro p/ train.
	tamanho_teste = numerolinhas(arquivoteste);				
	amostra = (flor*) calloc(tamanho_teste, sizeof(flor));
	
	if(k<1 || k>tamanho_train){
		printf("k is invalid\n");					//Condicao k invalido
		fclose(arquivotrain);
		fclose(arquivoteste);
		return 0;
	}
	rewind(arquivotrain);					//Para voltar ao comeco do arquivo
	rewind(arquivoteste);
	lendodados(tamanho_train, arquivotrain, treino);
	lendodados(tamanho_teste, arquivoteste, amostra);
	
	    
	distancias = alocamatriz(tamanho_teste, tamanho_train);                          //Utilizei resultado**(matriz), onde cada linha
																					 //representa uma flor do conjunto teste, e as
	distanciaeuclidiana(distancias, treino, amostra, tamanho_train, tamanho_teste);	 //colunas sao as flores do conjunto train. Dessa
																					 //maneira, ordenei cada linha da matriz, para 
	ordenacao(distancias, tamanho_train, tamanho_teste);							//procurar os vizinhos com menor distancia.
	
	analisedistancias(distancias, amostra, tamanho_train, tamanho_teste, k);
	
	
	
	for(int i = 0; i<tamanho_teste; i++){		//Em toda posicao do vetor struct, foi alocado memoria para o char *especie(%m), portanto
		free(amostra[i].especie);				//necessario dar free desde i=0 ate i=tamanho
	}
	for(int i = 0; i<tamanho_train; i++){
		free(treino[i].especie);
	}
	for(int i = 0; i<tamanho_teste; i++){		//Desalocando a matriz struct
		free(distancias[i]);
	}
	free(distancias);
	free(treino);		//Desalocando vetor struct
	free(amostra);		//Desalocando vetor struct
	fclose(arquivotrain); //Fechando arquivo
	fclose(arquivoteste); //Fechando arquivo
	free(train);		//Desalocando char*(%ms)
	free(teste);		//Desalocando char*(%ms)
	return 0;
}
