#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prscmd.h"
#define ERROR	-1
#define EXIT_SUCCESS	0
#define TRUE 	1
#define FALSE 	0

int ParseCmdLine (int argc,char *argv[],pCallback p, void *UserData)
{
	unsigned int pParams = 1;
	char * string;
	int contador = 0,check = 1;
	char mistake = FALSE;
	if (argc == 1 && (mistake != TRUE))		//Chequeo que haya parametros u opciones
	{
		mistake = TRUE;
	}
	else
	{	
		while(pParams < argc && (mistake !=TRUE) && (check != 0))	//Creo un while para chequear todos los argumentos de la linea de comandos
		{
			string = argv[pParams];
			if (string[0]!='-')					//Chequeo si es un argumento, si lo es llamo a p()
			{
				check = p(NULL, string, UserData);
				contador++;
			}
			else if ((string[0] == '-') && (string[1] != '\0') && (string[1] != '-'))	//Chequeo que sea una opcion y que sea valida
			{
				if(argv[pParams+1] != NULL && (*argv[pParams+1]!='-'))	//Chequeo q la opcion tenga un argumento propio
				{
					check = p(string, argv[pParams+1], UserData);
					contador++;
				}
				else
					mistake = TRUE;
				pParams++;
			}
			else
				mistake = TRUE;
			pParams++;

		}
	}
	if (mistake == TRUE || check == 0)		//Chequeo si hubo errores.
	{
		return ERROR;
	}
	else
		return contador;
}
