#include "Matrice.h"
#include <Math.h>

Matrice::Matrice(){
}


	  		//<Constructeurs>
		//  Cas général:
 Matrice::Matrice( int n, int p )
{			
			this->_nbLignes = n;
			this->_nbColonnes = p;
			this->_matrice=new double *[n];
			for(int i = 0; i<n;i++)
			{
				this->_matrice[i]=new double [p];
			   for(int j = 0; j< p ; j++) 
			   {
			      this->_matrice[i][j] = 0.0;
			   }
			}
}

		//  Copie d'une autre matrice:
Matrice::Matrice ( Matrice &originale)
{
			this->_nbColonnes = originale._nbColonnes;
			this->_nbLignes = originale._nbLignes;
			this->_matrice=new double *[this->_nbLignes];
			for(int i = 0; i<this->_nbLignes;i++){
				this->_matrice[i]=new double [originale._nbColonnes];
				for(int j = 0; j< originale._nbColonnes ; j++) {
					this->_matrice[i][j] = originale._matrice[i][j];
			   }
			}
}
		//  Cas initialisée par un tableau:
Matrice::Matrice( double** tableau, int n, int p )
{
			this->_matrice = new double *[n];
			for(int i=0;i<n;i++){
				this->_matrice[i]=new double [p];
				for(int j=0;j<p;j++){
					this->_matrice[i][j]=tableau[i][j];
				}
			}
			this->_nbLignes = n;
			this->_nbColonnes = p;
}	

Matrice::Matrice( double* tableau, int n, int p )
{
			this->_matrice = new double *[n];
			for(int i = 0;i<n;i++)
			{
				this->_matrice[i]=new double [p];
			   for(int j =0; j<p;j++)
			   {
			       this->_matrice[i][j] = tableau[i*p + j];
			   }
			}
	       
			this->_nbLignes = n;
			this->_nbColonnes = p;
}	
		//  Matrice Carrée:
Matrice::Matrice(int n)
{
	this->_matrice=new double *[n];
	for(int i=0;i<n;i++){
		this->_matrice[i]=new double [n];
		for(int j=0;j<n;j++){
			this->_matrice[i][j]=0;
		}
	}
	this->_nbColonnes=n;
	this->_nbLignes=n;
}
//</Constructeurs>

Matrice Matrice::Transpose()
		{	
			Matrice A(_nbColonnes,_nbLignes);
			for ( int j = 0 ; j < _nbLignes ; j++ )
			{
					for ( int i = 0 ; i < _nbColonnes ; i++ )
					{
						A._matrice[i][j] = _matrice[j][i];
					}
			}
			return 	A	;
		}
		
double Matrice::Determinant()
{
                
				double det = 0;
				Matrice B ;
				
				//Conditions d'arrêt
				if ( this->_nbLignes == 1 ) return _matrice[0][0];
				if ( this->_nbLignes == 2 ) return ( _matrice[0][0]*_matrice[1][1] - _matrice[0][1]*_matrice[1][0] );
				
				//Traitement par récursivité
				int nL = this->_nbLignes;
				for ( int j = 0 ; j < nL ; j++ )
				{
					B = this->SousMatrice( 0 , j );
					if ( j % 2 == 0 ) {
						det += this->_matrice[0][j]*B.Determinant(); 
					}
					else {
						det += -1*this->_matrice[0][j]*B.Determinant(); 
					}
				}
				return det;

}
		
Matrice Matrice::Comatrice()
		{
				Matrice B( this->_nbLignes , this->_nbColonnes );
				Matrice S;
				
				for ( int i = 0 ; i < B._nbColonnes ; i++ )
				{
					for ( int j = 0 ; j < B._nbColonnes ; j++ )
					{
						S = this->SousMatrice( i , j );
						if( ( i + j ) % 2 == 0 ) { B._matrice[i][j] = S.Determinant(); }
						else {B._matrice[i][j] = -1 * S.Determinant(); }
					}
				}
				
				return B;

		}

/*
Matrice Matrice::Inverse1()
		{

				double det = this->Determinant();
				Matrice t_Comatrice = this->Comatrice().Transpose();
				
				Matrice Inverse;
				Inverse = t_Comatrice * (1/det);
				return Inverse;
		}
*/

void Matrice::Inverse2()
{
	int nL=this->_nbLignes;
	int nC=this->_nbColonnes;
	double * M_tab = new  double[nL*nL]; 
	for(int i = 0 ;i< nL ; i++)
	{
		for(int j = 0 ;j< nC ; j++)
		 {
		    M_tab[i*nC+j] = this->_matrice[i][j];
		 }
	}
	
	double* inv = new double[nL*nC];
	Matrice::gaussR(M_tab,inv,nL);
	for(int i=0;i<nL;i++){
		for(int j=0;j<nC;j++){
			this->_matrice[i][j]=inv[i*nC+j];
		}
	}
	delete[] M_tab;
	delete[] inv;

}

void Matrice::identite(double* mat,int dim)
{
	for(int i=0;i<dim;i++)
	{
		for(int j=0;j<dim;j++)
		{
			if(i==j){
			mat[i*dim + j]=1.;
			}
			else{
			mat[i*dim + j]=0.;
			}
		}
	}
}


void Matrice::gaussR(double* mat1,double* &mat2,int dim)
{
	double *temp=new double[dim*dim];

	Matrice::copie_matriceR(mat1,temp,dim);
	double a,b;

	a=0.;
	b=0.;
	int c=0;

	Matrice::identite(mat2,dim);

	for(int k=0;k<dim;k++)
	{
		a=temp[k + k*dim];

//verifie la condition "a" different de zero

//sinon on copie une ligne pour resoudre le probleme

		c=0;

		while(fabs(a)< 0.000000001)
		{
		c++;

			for(int q=0; q<dim; q++)
			{
			temp[k*dim + q]=temp[k*dim + q] + temp[(k+c)*dim + q];
			mat2[k*dim + q]=mat2[k*dim + q]+mat2[(k+c)*dim + q];
			}
		a=temp[k+k*dim];
		}
//normalisation la ligne k

		for(int l=0; l<dim; l++)
		{
		temp[k*dim + l]=temp[k*dim + l]/a;
		mat2[k*dim + l]=mat2[k*dim + l]/a;
		}

//reduction de gauss-jordan

		for(int i=0;i<dim;i++)
		{
		b=temp[i*dim + k];
			if(i!=k)
			{
				for(int j=0;j<dim;j++)
				{
				temp[i*dim + j]=temp[i*dim + j]-b*temp[k*dim + j];
				mat2[i*dim + j]=mat2[i*dim + j]-b*mat2[k*dim + j];
				}
			}
		}
	}
	delete[] temp;
}


void Matrice::copie_matriceR(double* mat1, double* mat2, int dim)
{
	for(int i=0;i<dim;i++)
	{
		for(int j=0;j<dim;j++)
		{
		mat2[i*dim + j]=mat1[i*dim + j];
		}
	}
}


double Matrice::Trace()
		{
			double Trace = 0;

					if ( this->_nbLignes == this->_nbColonnes )
					{
						for ( int i = 0 ; i < this->_nbLignes ; i++ )
						{
							Trace += this->_matrice[i][i];
						}
						return Trace;
					}
					else return NULL;
		}
		
bool Matrice::IsCarree()
		{

				if ( this->_nbLignes == this->_nbColonnes ) { return true; }
				else { return false; }
		}
		
bool Matrice::IsInversible()
		{
				if ( this->Determinant() != 0 ) { return true; }
				else { return false; }
		}
		//</Propriétés>

		//<Méthodes>

// Write a function which does the same thing but without needing _nbLignes and _nbColonnes already initialised
void Matrice::Initialise (double** tableau,int n, int p)
		{
			if ( this->_nbLignes == n && this->_nbColonnes == p)
			{
				_matrice = tableau;	
			}
}


Matrice Matrice::SousMatrice ( int ib , int jb )
		{
            Matrice B ( this->_nbLignes - 1 , this->_nbColonnes - 1 );
			int ir = 0, jr = 0;
			for ( int i = 0 ; i < this->_nbLignes ; i++ )
			{
				for  ( int j = 0 ; j < this->_nbColonnes ; j++ )
				{
					if( i != (ib) && j != (jb) )
					{
						B._matrice[ir][jr]=this->_matrice[i][j];
						if ( jr < B._nbLignes-1 ) jr++;
						else { jr = 0; ir++; }
					}
				}
			}
			return B;
		}

		// fin <méthodes>

		//<Opérateurs>
Matrice& Matrice::operator =(const Matrice& B ){
	//! Y
	//! add condition if(...) TO DO
	this->_nbColonnes = B._nbColonnes;
	this->_nbLignes = B._nbLignes;

	//! Y
	//! delete the existing double**
    for(int i = 0; i<this->_nbLignes; i++){
		delete[] this->_matrice[i];
	}
	delete[] this->_matrice;

	this->_matrice=new double *[this->_nbLignes];
	for(int i = 0; i<this->_nbLignes;i++){
		this->_matrice[i]=new double [this->_nbColonnes];
		for(int j = 0; j< this->_nbColonnes ; j++) {
			this->_matrice[i][j] = B._matrice[i][j];
		}
	}
	return *this;
}

Matrice Matrice::operator +(Matrice &B )
		{

			 	if ( this->_nbLignes == B._nbLignes && this->_nbColonnes == B._nbColonnes )
			 	{
                    Matrice C ( this->_nbLignes , this->_nbColonnes );
					for ( int i = 0 ; i < this->_nbLignes ; i++ )
			 		{
			 			for (int j = 0 ; j < this->_nbColonnes ; j++ )
			 			{
			 				C._matrice[i][j] = this->_matrice[i][j] + B._matrice[i][j];
			 			}
			 		}
			 		return C;
			 	}
				else return NULL;
			 	
		}
			 
		
Matrice Matrice::operator -(Matrice &B )
		{
			 
			 	if ( this->_nbLignes == B._nbLignes && this->_nbColonnes == B._nbColonnes )
			 	{
	                Matrice C( this->_nbLignes , this->_nbColonnes ); 
					for ( int i = 0 ; i < this->_nbLignes ; i++ )
			 		{
			 			for (int j = 0 ; j < this->_nbColonnes ; j++ )
			 			{
			 				C._matrice[i][j] = this->_matrice[i][j] - B._matrice[i][j];
			 			}
			 		}
			 		return C;
			 	}
				else return NULL;
		}
		
Matrice Matrice::operator *(Matrice &B )
	{	
				if ( this->_nbColonnes == B._nbLignes )
				{
                    Matrice C ( this->_nbLignes , B._nbColonnes );
					for ( int i = 0 ; i < this->_nbLignes ; i++ )
					{
						for ( int j = 0 ; j < B._nbColonnes ; j++ )
						{
							for ( int k = 0 ; k < this->_nbColonnes ; k++ )
							{
								double x = this->_matrice[i][k]*B._matrice[k][j];
								C._matrice[i][j] += x;
							}
						}
					}
					return C;
				}
				else return NULL;
				
		}

void Matrice::product(Matrice &B){
	if (this->_nbColonnes == B._nbLignes ){
		Matrice C(this->_nbLignes,B._nbColonnes);
		for ( int i = 0 ; i < this->_nbLignes ; i++ ){
			for ( int j = 0 ; j < B._nbColonnes ; j++ ){
				for ( int k = 0 ; k < this->_nbColonnes ; k++ ){
					double x = this->_matrice[i][k]*B._matrice[k][j];
					C._matrice[i][j] += x;
				}
			}
		}
		for ( int i = 0 ; i < this->_nbLignes ; i++ ){
			for ( int j = 0 ; j < B._nbColonnes ; j++ ){
				this->_matrice[i][j]=C._matrice[i][j];
			}
		}
	//	*this=C;
	}
}
Matrice Matrice::operator *( double n)
		{
            Matrice B( this->_nbLignes, this->_nbColonnes); 
			for ( int i = 0 ; i < this->_nbLignes ; i++ )
			{
				for (int j = 0 ; j < this->_nbColonnes ; j++ )
				{
					B._matrice[i][j] = n * this->_matrice[i][j];
				}
			}
			
			return B;
		}
		
		//</Opérateurs>
Matrice::~Matrice(){
	for(int i=0;i<_nbLignes;i++){
		delete [] _matrice[i];
	}
	delete [] _matrice;
}