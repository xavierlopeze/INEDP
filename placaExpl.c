 /* Compila amb:
  * gcc -o placaExpl -g -Wall grRDF.c placaExpl.c -lm
  */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grRDF.h"

#define U(i,j) gr.u[(j)*(nx+1)+(i)]

/*
 * Funcio f del problema mixt per l'equació de la
 * calor, en el cas de la font de calor sobre la placa d'acer.
 */
double f_cal (double t, double x, double y) {
  double k=0.13; //constant de la placa d'acer
  if(fabs((x-0.5)*(x-0.5)+(y-0.5)*(y-0.5))<0.2*0.2) {return 100/k;}
  return 0;
}
/*
 * Funcio g del problema mixt per l'equació de la
 * calor, condició de frontera per la placa d'acer.
 */
double g_cal (double t, double x, double y) {
  return 0;
}
/*
 * Funcio h del problema mixt per l'equació de la
 * calor, condició inicial per la placa d'acer.
 */
double h_cal (double x, double y) {
return 0;
}

int main (int argc, char *argv[]) {
  
	/*Obrim fitxer*/
	FILE *fp = fopen("placaExpl.txt", "w");
	
	/*Variables internes*/
	int i;
	
	/*Constants de la placa*/
	double k=0.13, c=0.11, p=7.8;
	
	/*Paràmetres a llegir*/
	int nt, nx, ny;
	double T, Lx, Ly;
	
	/*Llegim paràmetres*/
	if (argc != 7
	  || sscanf(argv[1], "%d", &nt) != 1
	  || sscanf(argv[2], "%d", &nx) != 1
	  || sscanf(argv[3], "%d", &ny) != 1  
	  || sscanf(argv[4], "%lf", &T) != 1
	  || sscanf(argv[5], "%lf", &Lx) != 1
	  || sscanf(argv[6], "%lf", &Ly) != 1){
	    /*Si no llegim 6 paràmetres o no els llegim bé, acabem*/
	    fprintf(stderr, "Error a l'introduir paràmetres.\n");
	    fprintf(stderr, "Introdueix: %s nt nx ny T Lx Ly\n",argv[0]);
	    return 0;
	}
	/*Canvi de temps*/
	T*=(k/(c*p));
	
	/*Declarem objecte graella*/
        grRDF gr;
	
	/*Implementem l'Algorisme 1.1*/
	
	    /*Inicialitzem la graella (paràmetres i llesca t=0)*/
	    grRDF_init(&gr,Lx/nx,Ly/ny,T/nt,nx,ny,h_cal);
	    
	    /*Escric graella inicial*/
	    grRDF_escriure(&gr,fp);
	      
	    /*Bucle en el temps*/
	    for(i=0; i<nt; i++){
	      grRDF_pasCalExpl(&gr,f_cal,g_cal); /*Fem un pas de la graella en t*/
	      grRDF_escriure(&gr,fp); /*Escribim cada graella per a cada pas en temps*/ 
	    }
	
	/*Alliberem memòria*/  
	grRDF_allib(&gr);

	return 1;
}