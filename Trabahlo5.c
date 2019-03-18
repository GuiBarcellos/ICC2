/* 
   Disciplina: SCC0201 - Introdução à Ciência da Computação II - Professor Moacir Antonelli Ponti
   Trabalho 5 - Gerenciando Abas do Navegador
   Nome: Guilherme Targon Marques Barcellos
   Num. USP: 10724181	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct no NO;


struct no{
	int dia;
	int mes;					//Struct "no", é o nó da lista encadeada. Armazena as informacoes da aba navegador e
	int hora;					//um ponteiro p/ proximo nó.
	int minuto;
	char titulo[30];
	char URL[1024];
	NO* proximo;
	
};

typedef struct LISTA{
	NO* inicio;					//Struct "lista", que armazena ponteiro p/ o inicio e fim, além do tamanho da
	NO* fim;					//lista (quantidade de abas).
	int tamanho;
}LISTA;


/*
-Função para criar uma lista.
-Parametros: VOID.
-Retorno: Ponteiro para lista (LISTA*).
*/
LISTA* lista_criar(){
	
	LISTA* l = (LISTA*) malloc (sizeof(LISTA));
	
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	return l;
	
}


/*
-Função para criar um nó.
-Parametros: VOID.
-Retorno: Ponteiro para nó (NO*).
*/
NO* criar_no(){
	
	NO* n = (NO*) malloc (sizeof(NO));
	n->proximo = NULL;
	return n;
		
}

/*
-Função para inserir um NÓ (aba navegador) na lista.
-Parametros: Nó a ser inserido; Lista onde ocorre a inserçao.
-Retorno: VOID.
*/
void insere_na_lista(LISTA* l, NO* n){
	
	if(l->tamanho == 0){		//Caso a lista esteja vazia, primeiro nó a ser inserido na lista e portanto 
		l->inicio = n;			//ponteiro inicio e fim vao apontar para o mesmo nó.
		l->fim = n;
	}
	else{
		l->fim->proximo = n;    //Se já tem algum nó na lista (lista != vazia), entao insere nó depois do 
		l->fim = n;				//ultimo nó existente (lista->fim->proximo) e depois reajusta ponteiro fim.
	}
	(l->tamanho)++;				//Atualiza quantidade nós.
	
}

/*
-Função para criar site (aba navegador) e insere na lista.
-Parametros: Lista onde ocorre a inserçao.
-Retorno: VOID.
*/
void cria_site(LISTA* l){
	
	NO* site = criar_no();				//Cria nó para armazenar os dados da nova aba.
	char *tit;							//Titulo site.
	char *url;							//URL site.
	int dia, mes, hora, minuto;			//Variaveis armazenar horario/data;
	
	
	scanf("%*c");						//Leitura dos dados da nova aba.
	scanf("%m[^\r\n]", &tit);
	scanf("%*c");
	scanf("%m[^\r\n]", &url);
	scanf("%*c");
	scanf("%d %d", &dia, &mes);
	scanf("%d %d", &hora, &minuto);
	
	site->dia = dia;					//Essas 6 linhas armazenam os dados lidos no scanf
	site->mes = mes;					//na struct nó.
	site->hora = hora;
	site->minuto = minuto;
	strcpy(site->titulo, tit);			
	strcpy(site->URL, url);
	
	
	
	free(url);							//%m aloca mem. e precisamos desalocar.
	free(tit);
	insere_na_lista(l, site);			//Chamando a funcao para inserir na lista esta ultima
										//aba que foi criada.
}

/*
-Função para liberar toda memória utilizada.
-Parametros: Lista onde todos os nós foram inseridos.
-Retorno: VOID.
*/
void fecha_tudo(LISTA* l){
	
	NO* aux = l->fim;
	while(aux != NULL){					//Aqui vai percorrer toda a lista, dando free em cada um dos nós.
		aux = l->inicio->proximo;
		free(l->inicio);
		l->inicio = aux;
	}
	free(l);							//Free na lista (que neste ponto esta vazia).
}


/*
-Função para printar todas as abas da lista.
-Parametros: Lista onde todos os nós foram inseridos.
-Retorno: VOID.
*/
void printa_lista(LISTA* l){
	
	NO* aux = l->inicio;
	while(aux != NULL){					//Percorre nó por nó e printa.
		printf("%s %s %02d/%02d %02d:%02d\n", aux->titulo, aux->URL, aux->dia, aux->mes, aux->hora, aux->minuto);
		aux = aux->proximo;
	}
	
	printf("\n");
}

/*
-Função para alterar a posiçao de uma aba.
-Parametros: Lista onde todos os nós foram inseridos.
-Retorno: VOID.
*/
void alterar_posicao(LISTA* l){
	
	
	
	char* alterar;						          //Usuario digita titulo da aba a ser movida, armazena nesta variavel.
	int pos;									  //Usuario digita nova posicao p/ aba.
	scanf("%*c");
	scanf("%m[^\r\n]", &alterar);
	scanf("%*c");
	scanf("%d", &pos);
	
	NO *auxbusca = l->inicio;
	NO *aux = l->inicio;
	int encontrado = 0;							//Pode ser que o titulo digitado pelo usuario nao exista. Dessa forma, caso nao exista, 
												//esta variavel nao é alterada e esta funcao nao faz nada.
	
	while(aux != NULL){							//Este laco percorre a lista e verifica se existe uma aba com o titulo digitado.
		if(strcmp(aux->titulo, alterar) == 0){	//Caso exista, incrementa "encontrado" e sai do laço.
			encontrado++;
			break;
		}
		aux = aux->proximo;
	}
	if(l->tamanho == 1 || l->tamanho == 0){		//Se a lista esta vazia ou tem apenas um elemento, alterar a posicao seria
		encontrado = 0;							//inutil e portanto "encontrado = 0" (Para q esta funcao nao faca nada).
	}
	aux = l->inicio;
	
	if(encontrado != 0){		//Caso tamanho da lista seja maior que 1 E foi encontrada aba para ser movida.
	
	/*
		O "primeiro if" (if + else if + else) procuram pela aba a ser trocada (nó) e a desconectam da lista.
	Ela é armazenada em um nó auxiliar ( auxbusca) e posteriormente inserida novamente na lista, na 
	posicao desejada (int pos).		
	 */
	
		if(strcmp(l->inicio->titulo, alterar) == 0){    //NO a ser trocado esta comeco. Armazenado em auxbusca.
			auxbusca = l->inicio;
			l->inicio = auxbusca->proximo;
			auxbusca->proximo = NULL;
		}
		else if(strcmp(l->fim->titulo, alterar) == 0){    //NO a ser trocado esta no fim. Armazenado auxbusca.
			auxbusca = l->fim;
			while(aux->proximo->proximo != NULL){
				aux = aux->proximo;
			}
			aux->proximo = NULL;
			l->fim = aux;
		}
		else{
			while(strcmp(aux->proximo->titulo, alterar) != 0){  //NO a ser trocado esta no meio. Armazenado auxbusca.
				aux = aux->proximo;
			}
			auxbusca = aux->proximo;
			aux->proximo = auxbusca->proximo;
			auxbusca->proximo = NULL;
		}
		
		/*
			Este segundo if (if + else if + else) insere novamente a aba na lista 
		na nova posicao.
		 */ 
		
		if(pos == 1){							//Caso insercao seja na primeira posicao.
			auxbusca->proximo = l->inicio;
			l->inicio = auxbusca;
		}
		else if(pos > l->tamanho){				//Insercao no final da lista.
			l->fim->proximo = auxbusca;
			l->fim = auxbusca;
		}
		else{									//Insercao no meio da lista.
			aux = l->inicio;
			for(int i = 2; i<pos; i++){
				aux = aux->proximo;
			}
			auxbusca->proximo = aux->proximo;
			aux->proximo = auxbusca;
		}
	}
	
	free(alterar);      //Libera memoria alocada pelo %m.
}


/*
-Função para ordenar a lista em tempo linear.
-Parametros: Lista onde todos os nós foram inseridos.
-Retorno: VOID.
*/
void ordena_lista(LISTA* l){
	
	/*
		Funcao ordena em tempo linear. Usamos um vetor de listas com 10 posicoes para auxiliar. Primeiro ordenados pelo minuto, depois
	hora, dia e por fim mes. E cada uma dessas "categorias" é dividido em duas. Minuto primeiro ordenamos o algarismo da
	direita e depois o da esquerda. O mesmo ocorre com hora/dia/mes. Exemplo:
	 Aba 1 --> 10 : 45 (hora : minutos)
	 Aba 2 --> 08 : 58
	 Aba 3 --> 12 : 11
	 
	 Primeiro ordenamos algarismo da direita (minutos), ou seja, estamos ordenando "5"(aba 1), "8"(aba 2) e "1"(aba 3).
	   Temos entao :  Aba 3 - Aba 1 - Aba 2
	    
	   Note que nao fazemos essa ordenacao comparando. Fazemos i =  resto da divisao de "minuto" por 10 e a aba é inserida
	   no vetor de listas na posicao i. Desta maneira, a Aba 1 teria sido inserida na posicao '5', aba 2 na '8' e aba 3 na '1'.
	   Depois percorremos o vetor e "recolhemos" os nós para nossa lista, e assim, temos ordenado pelo primeiro algarismo. 
	   
	   
	   
	 Segundo passo : Ordenar nova lista (Abas 3 - 1 - 2) pelo algarismo da esquerda (minutos), ou seja, "1" (aba 3), "4" (aba 1) e
	   "5" (aba 2).
	    Fazemos isso dividindo por 10 e depois pegando resto da div por 10.
	    Temos entao : Aba 3 - Aba 1 - Aba 2
	    
	    
	    E assim seguimos com hora, depois dia e finalmente mes. No final, nossa lista estara ordenada.
	 */ 
	
	
	LISTA** vet_lista = (LISTA**) malloc (sizeof(LISTA*) * 10);   //Criando um vetor de 10 posicoes, onde cada posicao é uma lista.
	for(int i =0; i<10; i++){
		vet_lista[i] = lista_criar();  		//Para cada posicao, um ponteiro para lista é criado e atribuido.
	}
	
	int minuto = 0;
	NO* aux = l->inicio;							
	NO* armazena;
	while(aux != NULL){							//Ordenando pelo algarismo direita minuto.
		minuto = aux->minuto % 10;							
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[minuto], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	minuto = 0;
	aux = l->inicio;
	while(aux != NULL){
		minuto = (aux->minuto / 10) % 10;      //Ordenando pelo algarismo esquerda minuto.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[minuto], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	int hora = 0;
	aux = l->inicio;
	while(aux != NULL){
		hora = aux->hora % 10;               //Ordenando pelo algarismo direita hora.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[hora], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	hora = 0;
	aux = l->inicio;
	while(aux != NULL){
		hora = (aux->hora / 10) % 10;                   //Ordenando pelo algarismo esquerda hora.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[hora], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	int dia = 0;
	aux = l->inicio;
	while(aux != NULL){
		dia = aux->dia % 10;                  //Ordenando pelo algarismo direita dia.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[dia], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	dia = 0;
	aux = l->inicio;
	while(aux != NULL){
		dia = (aux->dia / 10) % 10;                       //Ordenando pelo algarismo esquerda dia.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[dia], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	int mes = 0;
	aux = l->inicio;
	while(aux != NULL){
		mes = aux->mes % 10;										  //Ordenando pelo algarismo direita mes.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[mes], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	mes = 0;
	aux = l->inicio;
	while(aux != NULL){
		mes = (aux->mes / 10) % 10;								//Ordenando pelo algarismo esquerda mes.
		armazena = aux;
		aux = aux->proximo;
		armazena->proximo = NULL;
		insere_na_lista(vet_lista[mes], armazena);
	}
	l->inicio = NULL;
	l->fim = NULL;
	l->tamanho = 0;
	
	for(int i =0; i<10; i++){
		aux = vet_lista[i]->inicio;
		while(aux != NULL){
			insere_na_lista(l, aux);
			aux = aux->proximo;
		}
	}
	
	
	for(int i =0; i<10; i++){
		vet_lista[i]->inicio = NULL;
		vet_lista[i]->fim = NULL;
		vet_lista[i]->tamanho = 0;
	}
	
	for(int i =0; i<10; i++){     //Liberando cada posicao do vetor de listas.
		free(vet_lista[i]);
	}
	free(vet_lista);      //Liberando o vetor lista (LISTA**)
	
	
}

int main(){


	int opcao;       //Variavel controle do switch.
	LISTA* lista = lista_criar();     //Criando a lista.
		
	while(1){                                       //Fica no laco ate que a opcao digitada seja "5".
		
		scanf("%d", &opcao);
		
		switch(opcao){
			
			case 1: cria_site(lista);
			break;
			case 2: alterar_posicao(lista);
			break;
			case 3: ordena_lista(lista);
			break;
			case 4: printa_lista(lista);
			break;
			case 5: fecha_tudo(lista);
			return 0;
		}
		
		
	}


	return 0;
}
