#ifndef LIB_H
#define LIB_H
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif // WIN32

#define EXIT_SUCCESS	0
#define ERROR	-1
#define MIN_DISTANCE    25
#define TRUE    1
#define MAX_REPEAT	5
#define MAX_FIL	640
#define	MAX_COL	640

typedef struct 
{
	float x1;
	float y1;
	float x2;
	float y2;
}Line_s;

typedef struct 
{
	float xp;
	float yp;
}Point_s;

typedef struct 
{
	Line_s	line1;
	Line_s	line2;
	Line_s	line3;
	char count;

}Triangle_s;

typedef struct 
{
	char errorMaxCount;
	char errorLenght;
	Triangle_s triangle1;
	Triangle_s triangle2;
	Triangle_s triangle3;
	Point_s point;

}NewTriangles_s;

typedef struct 
{
	char type;
	double lStart, lEnd, lConstant,leftAngle, rightAngle, x0, y0, x1, y1,polygon;
	char help;
}UserData_s;

typedef struct
{
	double x, y;
}point_t;

typedef struct
{
	point_t v[8];
}myOctagon_t;

typedef struct{

    double Re;
    double Im;

}complexT;



Triangle_s calcThreePoints(UserData_s UserData);
/*Funcion que recibe los parametros ingresados por el usuario y calcula los puntos del triangulo. Devuelve una variable del tipo Triangle_s
Se la utiliza de la siguiente manera:
	{
		...
		ParseCmdLine(argc, argv, myCallBack, &myUserData);
		miTriangulo = calcThreePoints(myUserData);
		...
	}
*/

void recursive(NewTriangles_s threetriangls,double lEnd, double lConstant);
/*Funcion recursiva que recibe como parametro un New_Triangles_s, una longitud final y una variable de repeticiones constante, y dibuja el fractal
 de ese triangulo.Se la invoca generando un triangulo y luego igualando ese triangulo con la variable triangle1 de la estructura NewTriangles_s creada:

	{
		...
		NewTriangles_s myTriangles;
		myTriangles.triangle1 = miTriangulo;
		recursive(myTriangles, lEnd, lConstant);
		...
	}
*/

void drawFractal(int s, double l, double lEnd, double xC, double yC);
unsigned int getMandelbrotSet(double Re, double Im, int maxN, double maxPlane);


#endif