#include <iostream>
#ifndef MATRICE_H_
#define MATRICE_H_
//#include "Conversion.h"
// Classe Matrices: Classe permettant de manipuler des matrices (addition, produit matriciel, transposition, inversion, ...)


class Matrice
{
		
	
//<attributs>
public:  double** _matrice;
public:	 int _nbLignes;
public:	 int _nbColonnes;

		
		//<Constructeurs>
		//  Cas général:
public: 
Matrice();
Matrice( int n, int p );
	
		//  Copie d'une autre matrice:
Matrice ( Matrice &originale);
		//  Cas initialisée par un tableau:
Matrice( double** tableau, int n, int p );	
Matrice( double* tableau, int n, int p );	
		//  Matrice Carrée:
Matrice(int n);
//</Constructeurs>
		

		//<Propriétés>

public: 
Matrice Transpose();	
double Determinant();
Matrice Comatrice();
		
//Matrice Inverse1();
void Inverse2();
void product(Matrice &B);
static void gaussR(double* mat1,double* &mat2,int dim);
static void copie_matriceR(double* mat1,double *temp,int dim);
static void identite(double* mat,int dim);
double Trace();
		
bool IsCarree();
bool IsInversible();
		//</Propriétés>

		//<Méthodes>
public: 
void Initialise (double** tableau,int n, int p);

Matrice SousMatrice ( int ib , int jb );
		// fin <méthodes>

		//<Opérateurs>
//Matrice operator =(Matrice B );
Matrice operator +(Matrice &B ); 
		
Matrice operator -(Matrice &B );
		
Matrice operator *(Matrice &B );
Matrice operator *=(Matrice &B );
Matrice operator *( double n);
Matrice& operator =(const Matrice & m);
/*{
	std::cout << "copy constructor of the Matrix called! " << std::endl;
    getchar();
}*/
~Matrice();
		//</Opérateurs>
};

#endif