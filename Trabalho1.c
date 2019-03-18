#include <stdio.h>


int main(int argc, char* argv[]){

	FILE* arquivo = NULL;
	int aux = 0;   //Auxiliar para "sinalizar" qual imagem estamos
	unsigned char imagem[512]; //512 bytes vao ser armazenados aqui
	char nomeimagem[8]; //Nome da imagem Ex: "000.jpg"
	
	while (fread(imagem, sizeof(char), 512, stdin)){  //Enquanto consegue ler 512 bytes, fica no loop, caso contrario retorna 0 e sai do while
		if(imagem[0] == 0xff && imagem[1] == 0xd8 && imagem[2] == 0xff && aux == 0 ){ //Primeira imagem encontrada(aux==0 + cabecalho)
			sprintf(nomeimagem, "%03d.jpg", aux); //Salva o nome da imagem em uma string
			arquivo = fopen(nomeimagem, "wb"); //Abrindo arquivo para escrita binaria(wb)
			fwrite(imagem, sizeof(char), 512, arquivo); //Escrevendo os bytes que estao em "imagem" no "arquivo"
			aux++; //Caso haja uma imagem depois dessa, ja incrementando a numeracao
		}
		else if(imagem[0] == 0xff && imagem[1] == 0xd8 && imagem[2] == 0xff){ //Nesse caso ja abrimos pelo menos uma imagem anteriormente e achamos o comeca de outra
			fclose(arquivo); //Antes de gravar a proxima, fechamos a anterior
			sprintf(nomeimagem, "%03d.jpg", aux);
			arquivo = fopen(nomeimagem, "wb");
			fwrite(imagem, sizeof(char), 512, arquivo); 
			aux++;
		}
		//Como nao necessariamente as imagens tem 512 bytes, apos ler e gravar os 512 bytes iniciais(entrou em um dos ifs anteriores),
		//entra nesse if e continua gravando de onde parou. Ou seja, nao tem o cabecalho e arquivo !=NULL, pois ja tem alguns bytes
		//gravados no arquivo.
		else if(arquivo != NULL){ 
			
			fwrite(imagem, sizeof(char), 512, arquivo); 
		}
		
	}
	if(aux == 0){ //Condicao de que nao ha nenhuma imagem(se tivesse pelo menos uma o aux teria incrementado)
		printf("Could not find pictures\n");
	}
	else{	//Se foram encontradas imagens, antes de finalizar o programa fclose na ultima
		fclose(arquivo);
	}
	
	return 0;

}
