//////////////////////////////////////////////////////////////////////////////////
//Estructura de Datos y Algoritmos.												//
//Trabajo Practico N° 2		"Fractales"											//
//Integrantes:	Valentina Lago													//
//				Kevin Kohn														//
//				Ian Diaz														//
//																				//
//Repo GitHub: https://github.com/idiaz97/Fractals.git							//
//////////////////////////////////////////////////////////////////////////////////

/*El programa recibe por linea de comandos parametros, si se desea saber como utilizarlo, se debe llamar al programa con:
	$~./Triangle -help now
y se mostraran en el display como se deben mandar parametros para que el programa haga difenetes cosas.*/

#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif // WIN32

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <string.h>
#include "lib.h"
#include "prscmd.h"

#define TRIANGULO	0
#define OCTOGONO	1
#define MANDELBROT	2

#define DEFAULT_BASE	200
#define DEFAULT_END		20
#define DEFAULT_REPS	5

int parseCallback (char * key, char * value, void * userdata);
char initAllegro (void);
void initUserData (UserData_s *UserData);

int main(int argc, char** argv) 
{
	UserData_s UserData;
	ALLEGRO_DISPLAY * display;
	ALLEGRO_SAMPLE *sample=NULL;
	initUserData(&UserData);

	if(initAllegro() == ERROR)
		return ERROR;

	if(ParseCmdLine(argc, argv, parseCallback, &UserData) == ERROR)		//Chequeo que el pasaje de parametros halla sido correcto, si no salgo del programa.
	{
		printf("Abort! Form Error\n");
		return ERROR;
	}
	if(UserData.help == TRUE)	//Imprime el manual de uso del programa en la consola.
	{
		printf("\n\nSi quiere dibujar un triangulo llame al programa con:\n./Triangle -type UNIFORME -lStart (longitud Inicial) -rightAngle (Angulo derecho en grados)");
		printf(" -leftAngle (Angulo izquierdo en grados)\n\nTambien puede modificar variables como -lEnd, -lConstant, o el punto inicial con");
		printf(" -x0, -y0, -x1, -y1, y sus respectivos numeros en el plano.\nEn caso de no inicializar al menos rightAngle, leftAngle y lStart,");
		printf(" el programa no funcionara\n\n");

		printf("Si se quiere dibujar el fractal de un poligono de n lados llame al programa con:\n./Triangle -type POLIGONO -polygon (numero de lados)\n");
		printf(" -lStart (largo de un lado) -lEnd (lado minimo) -x0 (numero en eje x del centro del poligono) -y0 (numero en eje y del centro del poligono)\n\n");

		printf("Si se quiere dibujar el fractal de Mandelbrot llame al programa con:\n./Triangle -type MANDELBROT -x0 (distancia hacia la izquierda del eje y * (-1))\n");
		printf("-y0 (distancia hacia abajo del eje x * (-1)) -x1 (distancia hacia la derecha del eje y) -y1 (distancia hacia arriba del eje x)\n\n");
		printf("\nPara salir de esta pantalla presione Enter...\n\n");
		getchar();
		return EXIT_SUCCESS;
	}
	else if (UserData.type == ERROR)
	{	
		printf("No se ha elegio el parametro -type, Abort!\n\n");
		return ERROR;
	}
	
	display = al_create_display(MAX_COL,MAX_FIL);				//Chequeo si se pudo crear el display.
	if (display == NULL)
	{
		printf("Could not initialize display! Abort!\n");
		return ERROR;
	}
    sample = al_load_sample( "Peek_82454201.wav" );
    if(sample == NULL)
    {
    	printf("Could not open Peek_82454201.wav! Abort.\n");
    	return ERROR;
    }

    al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);
    al_clear_to_color(al_map_rgb(223,224,194));
	al_flip_display();

	if(UserData.type == TRIANGULO)		//En caso de que el usuario haya ingresado la opcion del triangulo.
	{
		NewTriangles_s threetriangls;
		Triangle_s Triagulo;

		Triagulo = calcThreePoints(UserData);
		threetriangls.triangle1= Triagulo;
		threetriangls.triangle1.count = 1;
		recursive(threetriangls,UserData.lEnd,UserData.lConstant);		//llamo a la funcio recursive que a su vez llama a draw triangle.
	}

	else if(UserData.type == OCTOGONO && UserData.rightAngle == 0 && UserData.leftAngle == 0)
	{
		drawFractal(UserData.polygon,UserData.lStart,UserData.lEnd,UserData.x0,UserData.y0);
		al_flip_display();
	}

	else if(UserData.type == MANDELBROT && UserData.lStart == DEFAULT_BASE && UserData.lEnd == DEFAULT_END && UserData.rightAngle == 0 \
		&& UserData.leftAngle == 0 && UserData.lConstant == DEFAULT_REPS)
	{
		double af= UserData.x1,ao=UserData.x0 * (-1),bf=UserData.y1,bo=UserData.y0 * (-1);
    	double xmax=  MAX_FIL;
    	double ymax=  MAX_COL;
    	double maxPlane= af - ao ;  
    	double cant_pix = (af-ao)/xmax;
    	int maxN=255;
		unsigned int i,j,l;
		for(i=0;i<xmax;i++)
		{
        	for(j=0;j<ymax;j++)
        	{
        	    l=getMandelbrotSet( (i*cant_pix) + ao ,  (j*cant_pix) + bo , maxN,  maxPlane );
        	    al_draw_pixel( (float)i, (float)(ymax-j), al_map_rgb(0,(maxN*3)/(l*2),maxN-l)); 
        	}
			al_flip_display();
    	}
	}

	else
	{
		al_destroy_display(display);
		printf("Error! el tipo elegido incorrecto, eliga entre UNIFORME, OCTOGONO, TRIANGULO!\n");
		printf("To exit, press Enter...\n");
		getchar();
		return EXIT_SUCCESS;
	}
	printf("To exit, press Enter...\n");
	getchar();
	al_destroy_display(display);

	
	return (EXIT_SUCCESS);
}


int parseCallback (char *key, char *value, void *UserData)
{
	UserData_s * ptr2str = UserData;
	int state = FALSE;
	double temp;
	char cont = 1;

    if (key != NULL)                                        //CASO: ESTA PROCESANDO UNA OPCION//
    {
    	if(strcmp(key, "-type") == 0)
    	{
    		if(strcmp(value, "UNIFORME")==0)
    		{
				ptr2str->type = TRIANGULO;
				state = TRUE;
    		}
    		else if(strcmp(value, "POLIGONO")==0)
    		{
				ptr2str->type = OCTOGONO;
				state = TRUE;
    		}
    		else if(strcmp(value, "MANDELBROT")==0)
    		{
				ptr2str->type = MANDELBROT;
				state = TRUE;
    		}
    		else
    			state = FALSE;
    	}
    	else if(strcmp(key, "-lStart") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->lStart = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-lEnd") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->lEnd = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-lConstant") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->lConstant = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-leftAngle") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->leftAngle = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-rightAngle") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->rightAngle = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-x0") == 0)
    	{
			temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->x0 = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-y0") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->y0 = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-x1") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->x1 = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-y1") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->y1 = temp;
			state = TRUE;
    	}
    	else if(strcmp(key, "-help") == 0)
    	{
    		if(strcmp(value, "now")==0)
    		{
    			ptr2str->help = TRUE;
				state = TRUE;
			}
			else
				state = FALSE;
    	}
    	else if(strcmp(key, "-polygon") == 0)
    	{
    		temp = (*value)-'0';
    		while(value[cont]!= '\0')
			{
				temp = (temp*10)+value[cont]-'0';
				cont++;
			}		
			ptr2str->polygon = temp;
			state = TRUE;
    	}
    	else
    		state = FALSE;
    }
    else if (key == NULL)                                   //CASO: ESTA PROCESANDO UN PARAMETRO//
    {	
    	state = FALSE;
    }
    return state;
}


char initAllegro (void)
{
	if(!al_init())			//chequeo si se pudo inicializar allegro
	{
		printf("Could not init Allegro! Abort!\n");		//SI no, aborto el programa
		return ERROR;
	}
	if(!al_init_primitives_addon())			//Chequeo si se pudo inicializar el addon de primitivas, sino aborto
	{
		printf("Could not init primitives addon! Abort!\n");
		return ERROR;
	}
	if(!al_install_audio())
    {
      fprintf(stderr, "failed to initialize audio!\n");
      return ERROR;
     }    

    if(!al_init_acodec_addon())
    {
          fprintf(stderr, "failed to initialize audio codecs!\n");
       return ERROR;
	}
	if (!al_reserve_samples(1))
    {
       fprintf(stderr, "failed to reserve samples!\n");
       return ERROR;
    }
    return EXIT_SUCCESS;
}

void initUserData (UserData_s * UserData)
{
	UserData->lStart = DEFAULT_BASE;
	UserData->lEnd = DEFAULT_END;
	UserData->lConstant = DEFAULT_REPS;
	UserData->x0=0;
	UserData->y0=0;
	UserData->x1=0;
	UserData->y1=0;
	UserData->rightAngle=0;
	UserData->leftAngle=0;
	UserData->type=ERROR;
	UserData->polygon = 8;
	UserData->help = FALSE;
}