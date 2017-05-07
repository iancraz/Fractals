#ifndef PRSCMD_H
#define	PRSCMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROR	-1
#define EXIT_SUCCESS	0
#define TRUE 	1
#define FALSE 	0
typedef int (*pCallback)(char *, char *, void *);
typedef char * pChar;

typedef struct 
{
    pChar key;
    pChar value;
    pChar parameter;
}Estructura;	
int ParseCmdLine (int argc,char *argv[],pCallback p, void *UserData);

#endif