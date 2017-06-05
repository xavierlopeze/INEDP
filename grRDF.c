#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*macros*/
#define U(i, j) gr->u[(gr->nx + 1)*(j) + (i)]
#define Uk(i, j) Uk[(gr->nx + 1)*(j) + (i)]

/*Codi de les funcions de grRDF.h*/
typedef struct grRDF{
/* Pas en x i pas en y. */
   double dx;
   double dy;
/* Pendent numèric en x i y (i.e. $\mu_x, \mu_y$). */
   double mux;
   double muy;
/*Nombre de nodes en x menys 1, i en y menys 1 (i.e. $n_x, ny$). */
   int nx;
   int ny;
/*Instant de temps al qual correspon el contingut de la graella. */
   double t;
/*Pas en temps (i.e. $\delta_t$) */
   double dt;
/*Graella.
 *
 * El valor de la funció al punt de coordenades
 * x=i*dx i y=j*dy és u[j*(nx+1)+i]
 * */
   double *u;
} grRDF;

void grRDF_init (grRDF *gr, double dx, double dy, double dt,
      int nx, int ny, double (*h)(double x, double y)){
	/*Variables locals*/
	int i, j;
	
	/*Assignem paràmetres*/
	gr->dx=dx;
	gr->dy=dy;
	gr->dt=dt;
	gr->nx=nx;
	gr->ny=ny;
	
	gr->mux=dt/(dx*dx);
	gr->muy=dt/(dy*dy);
	
	/*Allocatem memòria*/
	gr->u=malloc(sizeof(double)*(nx+1)*(ny+1));
	
	/*Posem temps a zero*/
	gr->t=0;

	/*Imposem condicions inicials*/
	for(i=0; i<=nx; i++){
	  for(j=0; j<=ny; j++){
	    U(i,j)=h(i*dx,j*dy);
	  }
      }
}
void grRDF_escriure (grRDF *gr, FILE *fp){
      int i,j;
      fprintf(fp,"# t %lf\n",gr->t);
      for(i=0; i<=gr->nx; i++){
	for(j=0; j<=gr->ny; j++){
	  fprintf(fp,"%lf %lf %lf\n", i*(gr->dx),j*(gr->dy),U(i,j));
	}
	fprintf(fp,"\n");
      }
      fprintf(fp,"\n");
}
void grRDF_pasCalExpl (grRDF *gr,
      double (*f)(double t, double x, double y),
      double (*g)(double t, double x, double y)){
	  
      /*Variables locals*/
      int i,j;
      double coef=(1-2*gr->mux-2*gr->muy); /*Estalviem càlculs*/
	  
      /*Reservem memòria per Uk*/
      double *Uk=malloc(sizeof(double)*(gr->nx+1)*(gr->ny+1));

      /*Avançem temps*/
      gr->t+=gr->dt;
	  
      /*Condicions de frontera*/
      for(i=0; i<=gr->nx; i++){
	 Uk(i,0)=g(gr->t,i*(gr->dx),0);
	 Uk(i,gr->ny)=g(gr->t,i*(gr->dx),gr->ny*(gr->dy));
      }
      for(j=0; j<=gr->ny; j++){
	 Uk(0,j)=g(gr->t,0,j*gr->dy);
	 Uk(gr->nx,j)=g(gr->t, gr->nx*gr->dx, j*gr->dy);
      }
	  
      /*implementem (3) omplint interior de la graella*/
      for(i=1; i<gr->nx; i++){
	for(j=1; j<gr->ny; j++){ 
	      Uk(i,j)=coef*U(i,j)
		    +(gr->mux)*(U(i+1,j)+U(i-1,j))
		    +(gr->muy)*(U(i,j+1)+U(i,j-1))
		    +(gr->dt)*f(gr->t,i*(gr->dx),j*(gr->dy)); 
	    }
	  }
	  
      /*Copiem la graella nova*/
	for(i=0; i<=gr->nx; i++){
	  for(j=0; j<=gr->ny; j++)
	    U(i,j)=Uk(i,j);
	  }
	  
      /*Alliberem memòria*/
      free(Uk);
	  
}
int grRDF_pasCalCN (grRDF *gr, double w, double tol, int maxit,
      double (*f)(double t, double x, double y),
      double (*g)(double t, double x, double y)){
	  
      /*Variables locals*/
      int i,j, nIterat;
      double error,tmp;
      double *Uk = malloc((gr->nx + 1)*(gr->ny + 1)*sizeof(double));
      double a=1-w, b=w/(1+gr->mux+gr->muy), c=1-gr->mux-gr->muy; /*Estalviem càlculs*/
      	
      /*Avançem temps*/
      gr->t+=gr->dt;
      
      /*Condicions de frontera (2) */
      for (i = 0; i <= gr->nx; i++) {
	Uk(i, 0) = g(gr->t, i*(gr->dx), 0);
	Uk(i, gr->ny) = g(gr->t, i*(gr->dx), (gr->dy)*(gr->ny));     
      }
	for (j = 1; j <= gr->ny - 1; j++) {
	  Uk(0, j) = g(gr->t, 0, j*(gr->dy));
	  Uk(gr->nx, j) = g(gr->t, (gr->dx)*(gr->nx), j*(gr->dy));          
      }
	/*Copiem valor inicial menys a la frontera*/
	for (i = 1; i <= gr->nx - 1; i++) {
	  for (j = 1; j <= gr->ny - 1; j++) {
	    Uk(i, j) = U(i, j);
	  }
	}
	
	 /*Omplim interior de la graella*/
	    
	    /*Inicialitzem variables per entrar al bucle*/
	    nIterat=1; 
	    error=2*tol;
	    
	    /*Sobrerelaxació*/
	    while(nIterat<=maxit && error>tol){
	      error=0;
	      for(j=1; j<gr->ny; j++){
		for(i=1; i<gr->nx; i++){ 
		  tmp=Uk(i,j);
		  Uk(i,j) = a*Uk(i, j) + b*(c*U(i, j)
				+ (gr->mux)*0.5*(Uk(i + 1, j) + Uk(i - 1, j) + U(i + 1, j) + U(i - 1, j))
				+ (gr->muy)*0.5*(Uk(i, j + 1) + Uk(i, j - 1) + U(i, j + 1) + U(i, j - 1))
				+ (gr->dt)*0.5*(f(gr->t, i*gr->dx, j*gr->dy) + f(gr->t - gr->dt, i*gr->dx, j*gr->dy)));
		  
		  if(fabs(tmp-Uk(i,j))>error){
		    error=fabs(tmp-Uk(i,j)); }
		}
	      }
	      nIterat++;
	    }
	 /*copiem graella*/
	 memcpy(gr->u,Uk,(gr->nx+1)*(gr->ny+1)*sizeof(double));
	    
 	 /*alliberem memòria*/
	 free(Uk);
	    
	return nIterat-1;  
}
      
 void grRDF_allib (grRDF *gr){
	free(gr->u);
}