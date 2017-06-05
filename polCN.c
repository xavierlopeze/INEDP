/*	Compila amb:
 *	gcc -o polCN -g -Wall polCN.c grRDF.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grRDF.h"

#define uErrors(i,j) uErrors[(j)*(nx+1)+(i)]
#define U(i,j) gr.u[(j)*(nx+1)+(i)]

/*
 * Funcio f del problema mixt per l'equació de la
 * calor, imposant que la solució sigui un polinomi.
 */
double f_pol (double t, double x, double y) {
  return 1+x+y;
}
/*
 * Funció g del problema mixt per l'equació de la
 * calor, imposant que la solució sigui un polinomi.
 */
double g_pol (double t, double x, double y) {
 return (1+x+y)+t;
}
/*
 * Funcio h del problema mixt per l'equació de la
 * calor, imposant que la solució sigui un polinomi.
 */
double h_pol (double x, double y) {
return 0;
}

int main (int argc, char *argv[]) {
	
	/*Obrim fitxer*/
	FILE *fp = fopen("polCN.txt", "w");
	
	/*Variables internes*/
	int k,i,j;
	double error, errorMax;
	
	/*Paràmetres a llegir*/
	int nt, nx, ny, maxit;
	double T, Lx, Ly,tol;
	
	/*Llegim paràmetres*/
	if (argc != 9
	  || sscanf(argv[1], "%d", &nt) != 1
	  || sscanf(argv[2], "%d", &nx) != 1
	  || sscanf(argv[3], "%d", &ny) != 1  
	  || sscanf(argv[4], "%lf", &T) != 1
	  || sscanf(argv[5], "%lf", &Lx) != 1
	  || sscanf(argv[6], "%lf", &Ly) != 1
	  || sscanf(argv[7], "%lf", &tol) != 1
	  || sscanf(argv[8], "%d", &maxit) != 1){
	    /*Si no llegim 8 paràmetres o no els llegim bé, acabem*/
	    fprintf(stderr, "Error a l'introduir paràmetres.\n");
	    fprintf(stderr, "Introdueix: %s nt nx ny T Lx Ly tol maxit\n",argv[0]);
	    return 0;
	}
	/* Declarem objecte graella*/
        grRDF gr;
	
	/*Implementem l'algorisme 1.2*/
	
	    /*Inicialitzem la graella (paràmetres i llesca t=0)*/
	    grRDF_init(&gr,Lx/nx,Ly/ny,T/nt,nx,ny,h_pol);
	    
	    /*Escric graella inicial*/
	    grRDF_escriure(&gr,fp);
	      
	    /*Bucle en el temps*/
	    for(k=0; k<nt; k++){
	      /*Fem un pas de la graella en t i imprimim el nombre de passos de SOR*/
	      printf("Convergeix en %d passos\n",grRDF_pasCalCN(&gr,1.7,tol,maxit,f_pol,g_pol)); 
	      grRDF_escriure(&gr,fp); /*Escribim cada graella per a cada pas en temps*/
	    }
	
	/*Calculem errorMax de la graella:*/
	  errorMax=0;
	  for (i=0; i<=nx; i++){
	    for(j=0; j<=ny; j++){
	      error=fabs(g_pol(T,i*(Lx/nx),j*(Ly/ny))-U(i,j));
	      if(error>errorMax){errorMax=error;}
	    }
	  }
	
	/*Imprimim error màxim*/
	fprintf(stderr,"L'error màxim és %lf\n", errorMax);
	
	/*Alliberem memòria*/
	grRDF_allib(&gr);
	
        return 1;
}
