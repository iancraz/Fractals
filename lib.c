#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#endif // WIN32
#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

#define M_PI	3.14159265359

double change2Rad(double angle);
/*Funcion que recibe un angulo en grados y lo cambia a radianes, si se quiere aumentar la precision, se deben agregar mas decimales al define PI
	{
		...
		angulo = change2Rad(angulo);
		...
	}
*/
float calcDistance(float x1, float y1,float x2, float y2);
/*Funcion que calcula la distancia entre 2 puntos en un pano bidimensional
	{
		...
		distancia = calcDistance(x1,y1,x2,y2);
		...
	}
*/
NewTriangles_s drawTriangle(Triangle_s myTriange, double lEnd, double lConstant);
/*Funcion que recibe un Triangle_s, una longitud final, y una constante de repeticiones lo dibuja y devuelve 3 nuevos triangulos del tipo NewTriangles_s
	{
		...
		int lEnd = 50, lConstant = 5;
		NewTriangles_s NuevosTriangulos;
		NuevosTriangulos = drawTriangle(myTriangle, lEnd, lConstant);
		...
	}
*/

myOctagon_t drawPoly(int s, double l, double xC, double yC, double offset);
complexT complexSum (complexT x, complexT y);
complexT complexMul (complexT x, complexT y);
double complexMod (complexT x);


NewTriangles_s drawTriangle(Triangle_s myTriange,double lEnd, double lConstant)
{
	ALLEGRO_COLOR colorLine;
	colorLine = al_map_rgb(63,91,167);		
	NewTriangles_s threetriangls;	//Creo una variable que sera una estructura de 3 nuevos triangulos.
	float Leg1, Leg2, Leg3;
	Leg1 = calcDistance(myTriange.line1.x1,myTriange.line1.y1,myTriange.line1.x2,myTriange.line1.y2);//aca calculo las distancias de cada lado para despues compararlas con la minima distancia posible
	Leg2 = calcDistance(myTriange.line2.x1,myTriange.line2.y1,myTriange.line2.x2,myTriange.line2.y2);
	Leg3 = calcDistance(myTriange.line3.x1,myTriange.line3.y1,myTriange.line3.x2,myTriange.line3.y2);
	
	if ((myTriange.count >= lConstant) || (Leg1 < lEnd))//a su vez tambien chequeo que el numero de repeticiones sea el mismo para todos los triangulos
	{
		threetriangls.errorMaxCount = TRUE;
		return threetriangls;
	} 

	if ((myTriange.count >= lConstant) || (Leg2 < lEnd))
	{
		threetriangls.errorMaxCount = TRUE;
		return threetriangls;
	}
	if ((myTriange.count >= lConstant) || (Leg3 < lEnd))
	{
		threetriangls.errorMaxCount = TRUE;
		return threetriangls;
	}

	al_draw_line(myTriange.line1.x1,myTriange.line1.y1,myTriange.line1.x2,myTriange.line1.y2,colorLine,1);	//dibujo el triangulo que me mandaron
	al_draw_line(myTriange.line2.x1,myTriange.line2.y1,myTriange.line2.x2,myTriange.line2.y2,colorLine,1);
	al_draw_line(myTriange.line3.x1,myTriange.line3.y1,myTriange.line3.x2,myTriange.line3.y2,colorLine,1);
	al_flip_display();

	//Este bloque se encarga de generar los nuevos triangulos
	{
		NewTriangles_s newReturnedT;
		//-----------------------------------///      Aca calculo el centro del triangulo y lo pongo en la nueva variable.
		newReturnedT.point.xp = (myTriange.line1.x1 + myTriange.line2.x1 + myTriange.line3.x1)/3;
		newReturnedT.point.yp = (myTriange.line1.y1 + myTriange.line2.y1 + myTriange.line3.y1)/3;
		//-----------------------------------///
		//-----------------------------------///		Calculo los puntos del 1er triangulo de los 3 que se forman.
														/*Cabe aclarar que las lineas estan numeradas en orden horario*/
		newReturnedT.triangle1.line1 = myTriange.line1;	//Linea 1

		newReturnedT.triangle1.line2.x1 = myTriange.line1.x2;	//Linea 2
		newReturnedT.triangle1.line2.y1 = myTriange.line1.y2;
		newReturnedT.triangle1.line2.x2 = newReturnedT.point.xp;
		newReturnedT.triangle1.line2.y2 = newReturnedT.point.yp;

		newReturnedT.triangle1.line3.x1 = newReturnedT.point.xp;//Linea 3
		newReturnedT.triangle1.line3.y1 = newReturnedT.point.yp;
		newReturnedT.triangle1.line3.x2 = myTriange.line1.x1;
		newReturnedT.triangle1.line3.y2 = myTriange.line1.y1;

		newReturnedT.triangle1.count = myTriange.count + 1;	//subo el contador
		//-----------------------------------///

			//-----------------------------------///		Calculo los puntos del 2do triangulo de los 3 que se forman.
		newReturnedT.triangle2.line1.x1 = newReturnedT.point.xp;
		newReturnedT.triangle2.line1.y1 = newReturnedT.point.yp;
		newReturnedT.triangle2.line1.x2 = myTriange.line1.x2;
		newReturnedT.triangle2.line1.y2 = myTriange.line1.y2;

		newReturnedT.triangle2.line2 = myTriange.line2;	//Linea 2

		newReturnedT.triangle2.line3.x1 = myTriange.line2.x2;//Linea 3
		newReturnedT.triangle2.line3.y1 = myTriange.line2.y2;
		newReturnedT.triangle2.line3.x2 = newReturnedT.point.xp;
		newReturnedT.triangle2.line3.y2 = newReturnedT.point.yp;

		newReturnedT.triangle2.count = myTriange.count + 1;	//Subo el contador
		//-----------------------------------///

		//-----------------------------------///		Calculo los puntos del 3er triangulo de los 3 que se forman.
		newReturnedT.triangle3.line1.x1 = myTriange.line1.x1;
		newReturnedT.triangle3.line1.y1 = myTriange.line1.y1;
		newReturnedT.triangle3.line1.x2 = newReturnedT.point.xp;
		newReturnedT.triangle3.line1.y2 = newReturnedT.point.yp;

		newReturnedT.triangle3.line2.x1 = newReturnedT.point.xp;//Linea2
		newReturnedT.triangle3.line2.y1 = newReturnedT.point.yp;
		newReturnedT.triangle3.line2.x2 = myTriange.line3.x1;
		newReturnedT.triangle3.line2.y2 = myTriange.line3.y1;	

		newReturnedT.triangle3.line3 = myTriange.line3;				//Linea3

		newReturnedT.triangle3.count = myTriange.count + 1;//Subo el contador

		threetriangls= newReturnedT;
	}
	
	return threetriangls;	//Devuelvo los 3 nuevos triangulos
}


float calcDistance(float x1, float y1,float x2, float y2) //Funcion que clacula la distancia entre 2 ptos.
{
	float answer;
	answer = sqrt((pow(x2-x1,2)+pow(y2-y1,2)));
	return answer;
}

void recursive(NewTriangles_s threetriangls,double lEnd, double lConstant)
{
	if (threetriangls.errorMaxCount == TRUE)
		return;
	recursive(drawTriangle(threetriangls.triangle1,lEnd,lConstant),lEnd,lConstant);	//Llamo a la funcion recursive por cada nuevo triangulo.
	recursive(drawTriangle(threetriangls.triangle2,lEnd,lConstant),lEnd,lConstant);
	recursive(drawTriangle(threetriangls.triangle3,lEnd,lConstant),lEnd,lConstant);
	return;
}

Triangle_s calcThreePoints(UserData_s UserData)
{
	Point_s sPoint;
	double thirdAngle;
	double rightLenght, leftLenght;
	double legg1, legg2;
	Triangle_s TriangleAnswer;

	thirdAngle = 180 - UserData.rightAngle - UserData.leftAngle;		//Calculo el angulo faltante

	thirdAngle = change2Rad(thirdAngle);							//cambio los 3 angulos a radianes
	UserData.rightAngle = change2Rad(UserData.rightAngle);
	UserData.leftAngle = change2Rad(UserData.leftAngle);

	leftLenght = (UserData.lStart/sin(thirdAngle));		//Uso teorema del seno para calcular las distancias de los lados restantes
	leftLenght = leftLenght * sin(UserData.rightAngle);

	legg1 = cos(UserData.leftAngle)*leftLenght;		//Calculo catetos de triangulo rectagulo caracteristico para calcular los ptos.	
	legg2 = sin(UserData.leftAngle)*leftLenght;

	if((UserData.x0 == 0) && (UserData.y0 == 0))		//En caso de que se tenga el pto inicial de defualt,
	{													//Se centra el triangulo al medio del display.
		sPoint.xp = (MAX_COL/2)-(UserData.lStart/2); 	
		sPoint.yp = (MAX_FIL/2)+(legg2/2);
	}
	//Bolque que calcula los puntos del triangulo con los datos obtenidos anteriormente.
	{
		Triangle_s firstTriangle;
				//Calculo de primera linea
		firstTriangle.line1.x1 = sPoint.xp;
		firstTriangle.line1.y1 = sPoint.yp;
		firstTriangle.line1.x2 = sPoint.xp+legg1;
		firstTriangle.line1.y2 = sPoint.yp-legg2;

				//Calculo de tercera Linea
		firstTriangle.line3.x1 = firstTriangle.line1.x1 + UserData.lStart;
		firstTriangle.line3.y1 = firstTriangle.line1.y1;
		firstTriangle.line3.x2 = sPoint.xp;
		firstTriangle.line3.y2 = sPoint.yp;

				//Calculo de segunda linea
		firstTriangle.line2.x1 = firstTriangle.line1.x2;
		firstTriangle.line2.y1 = firstTriangle.line1.y2;
		firstTriangle.line2.x2 = firstTriangle.line3.x1;
		firstTriangle.line2.y2 = firstTriangle.line3.y1;
		TriangleAnswer= firstTriangle;
	}
	return TriangleAnswer;
}

double change2Rad(double angle)		//Funciones que recibe un angulo y lo transforma a radianes.
{
	double temp;
	temp = angle* M_PI /180;
	return temp;
}

void drawFractal(int s, double l, double lEnd, double xC, double yC)
{

	double len = l;
	myOctagon_t oct;
	oct = drawPoly(s, l, xC, yC, 0);
	len *= 0.5;
	int i;
	for (i = 0;i<s;i++)
	{
		if (len<lEnd)
			return;
		else
			drawFractal(8, len, lEnd, oct.v[i].x, oct.v[i].y);
	}
}

myOctagon_t drawPoly(int s, double l, double xC, double yC, double offset)
{
	myOctagon_t o;
	int i;
	//point_t o[s];
	double r = l / (2 * sin(M_PI / s));

	for (i = 0; i < s; i++)
	{
		o.v[i].x = xC + r * cos(i * 2 * M_PI / s + offset);
		o.v[i].y = yC + r * sin(i * 2 * M_PI / s + offset);

		if (i != 0)
			al_draw_line(o.v[i - 1].x, o.v[i - 1].y, o.v[i].x, o.v[i].y, al_map_rgb(0, 0, 0), 1);
	}
	al_draw_line(o.v[s - 1].x, o.v[s - 1].y, o.v[0].x, o.v[0].y, al_map_rgb(0, 0, 0), 1);
	//    al_rest(0.2);
	al_flip_display();
	return o;
}


unsigned int getMandelbrotSet(double Re, double Im, int maxN, double maxPlane)
{
    complexT z;
    complexT sum;
    z.Re=Re;
    z.Im=Im;
    sum.Re=0.0;
    sum.Im=0.0;
    unsigned int i;
    
    for(i=0;(i<maxN)&&((complexMod(sum))<maxPlane);i++)  // casteo el modulo del numero complejo???
    {
        sum=complexSum(complexMul(sum,sum),z);
    }
    return i;
}

// suma dos numeros complejos ---> (a+bi) + (c+di) = (a+c) + (b+d)i

complexT complexSum (complexT x, complexT y)
{
   complexT  i;
   i.Re=x.Re+y.Re;
   i.Im=x.Im+y.Im;
   return i;
}

   // multiplica dos numeros complejos ---> (a+bi).(c+di) = (ac - bd) + (ad + bc)i
   
complexT complexMul (complexT x, complexT y)
{
   complexT  i;
   i.Re=(x.Re*y.Re)-(x.Im*y.Im);
   i.Im=(x.Re*y.Im)+(x.Im*y.Re);
   return i;
}   
   
   
   // saca el modulo de un numero complejo --->  |a+bi|= raiz cuadrada (a^2 + b^2)
   
double complexMod (complexT x)
{
    double module;
    module = sqrt(pow(x.Re,2)*pow(x.Im,2));
    return module;
    }
    

