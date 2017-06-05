# Optimització
OPT=-g -Wall

# Tot:
all : polExpl polCN placaExpl placaCN

# Utilitats:
# - Polinomi explí­cit
polExpl : polExpl.c grRDF.o grRDF.h
	gcc -o polExpl $(OPT) polExpl.c grRDF.o -lm
# - Placa explícit
placaExpl : placaExpl.c grRDF.o grRDF.h
	gcc -o placaExpl $(OPT) placaExpl.c grRDF.o -lm
# - Polinomi Crank-Nicolson
polCN : polCN.c grRDF.o grRDF.h
	gcc -o polCN $(OPT) polCN.c grRDF.o -lm
# - Placa Crank-Nicolson
placaCN : placaCN.c grRDF.o grRDF.h
	gcc -o placaCN $(OPT) placaCN.c grRDF.o -lm



# Biblioteca
grRDF.o : grRDF.c grRDF.h
	gcc -c $(OPT) grRDF.c

# Neteja
clean :
	rm -f *.o
realclean : clean
	rm -f polExpl placaExp placaCN
