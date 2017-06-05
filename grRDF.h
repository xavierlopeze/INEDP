/*!
 * Estructura graella d'evolució.
 *
 * Aquesta estructura està preparada per a contenir una
 * llesca de temps de la graella corresponent a la resolució
 * per diferències finites d'una EDP d'evolució sobre un
 * rectangle.
 */
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

/*!
 * \brief Inicialitza un objecte grRDF.
 *
 * Assigna paràmetres, allocata memòria i posa temps a zero.
 */
void grRDF_init (grRDF *gr, double dx, double dy, double dt,
      int nx, int ny, double (*h)(double x, double y));

/*!
 * Per a l'equació de la calor sobre un rectangle, fa
 * un pas en temps del mètode explícit (3) (o sigui, un pas
 * en k de l'algorisme 1.1)
 */
void grRDF_pasCalExpl (grRDF *gr,
      double (*f)(double t, double x, double y),
      double (*g)(double t, double x, double y));

/*!
 * \brief Escriu un objecte grRDF, en aquest format:
 * \verbatim
 * # t <valor de t>
 * <x> <y> <u(x,y)>
 * ...
 * \endverbatim
 * Recorre la graella per files, insertant un salt de línia
 * a cada canvi de fila, i dos salts de línia un cop
 * completada la graella
 */
void grRDF_escriure (grRDF *gr, FILE *fp);

/*Allibera la memòria d'un objecte \c grRDF. */
void grRDF_allib (grRDF *gr);

/*!
 * \brief Per a l'equació de la calor sobre un rectangle, fa
 * un pas en temps del mètode de Crank-Nicolson (5) (o
 * sigui, un pas en k de l'algorisme 1.2)
 */
int grRDF_pasCalCN (grRDF *gr, double w, double tol, int maxit,
      double (*f)(double t, double x, double y),
      double (*g)(double t, double x, double y));
