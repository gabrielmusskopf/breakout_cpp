#ifndef SFML_FUNCTIONS_HPP
#define SFML_FUNCTIONS_HPP

#include <windows.h>
#include "classes.hpp"


using namespace sf;

bool Collision (Sprite s1, Sprite s2){
return s1.getGlobalBounds().intersects(s2.getGlobalBounds());   //Testa se está sobreposto, retorna 1 se verdadeiro
}

HANDLE AbreComm(char *nomecom, int baudrate)
{


    	HANDLE hcom; //cria um ponteiro de nome hcom
    	DCB cdcb;    //nomeia a estrutura DCB (Device Control Block) utilizada 		para definir todos os parâmetros da comunicação
    	COMMTIMEOUTS comto; //nomeia a estrutura COMMTIMEOUTS (COMMon TIME OUTS) utilizada para definir os timeouts da comunicação
 	hcom = CreateFile(
      	nomecom, //nome do arquivo
           	GENERIC_READ | GENERIC_WRITE, //abre arquivo para leitura 						ou escrita
           	0, //indica que o arquivo não pode ser compartilhado
           	NULL, //utiliza a estrutura default para as funções de segurança
           	OPEN_EXISTING, //abre o arquivo, se não existir, retorna erro
           	FILE_ATTRIBUTE_NORMAL, //o arquivo deve ser utilizado 					sozinho
           	NULL); //sem gabarito de atributos
if(hcom == INVALID_HANDLE_VALUE) // testa falha na abertura do arquivo
    	{
      	fprintf(stderr, "Nao abriu a %s\n", nomecom);
      	return hcom;
    	}

    	GetCommState(hcom, &cdcb); //le os parâmetros de comunicação 					atuais
    	cdcb.BaudRate    = baudrate; //define a taxa de transmissão
    	cdcb.ByteSize    = 8; //define o tamanho do dado - 8 bits
   	 cdcb.StopBits    = ONESTOPBIT; //define o tamanho do stop bit - 1 					stop bit
    	cdcb.Parity      = NOPARITY;	 //define o tipo de paridade - sem 						paridade
if(!SetCommState(hcom,&cdcb)) //seta os novos parâmetros de comunicação
    {
      	fputs("SetCommState", stderr);
      	return INVALID_HANDLE_VALUE;
    }

	GetCommTimeouts(hcom, &comto);
	 //Le os parâmetros atuais de COMMTIMEOUTS
    comto.ReadIntervalTimeout         = MAXDWORD;
	//tempo máximo entre a chegada de dois caracters consecutivos(ms)
    comto.ReadTotalTimeoutMultiplier  =    0;
    comto.ReadTotalTimeoutConstant    =    0;
    comto.WriteTotalTimeoutMultiplier =    0;
    comto.WriteTotalTimeoutConstant   =    0;
    SetCommTimeouts(hcom, &comto);
	//seta os parâmetros de COMMTIMEOUTS
    return hcom;
}


int t_mushroom(void){
    std::srand(std::time(NULL));
    int a = rand()%3;
    int b = rand()%3;

    if(a == b) return 1;
    else return 0;
}

int t_flower(void){
    std::srand(std::time(NULL));
    int a = rand()%4;
    int b = rand()%4;

    if(a == b) return 1;
    else return 0;
}

int t_star(void){
    std::srand(std::time(NULL));
    int a = rand()%4;
    int b = rand()%4;

    if(a == b) return 1;
    else return 0;
}

#endif // SFML_FILENAME_HPP


