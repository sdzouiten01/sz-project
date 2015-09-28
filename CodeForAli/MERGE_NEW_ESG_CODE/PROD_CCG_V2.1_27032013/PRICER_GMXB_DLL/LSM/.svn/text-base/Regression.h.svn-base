#pragma once
const double epsilon = 0.0000000001;

class Regression
{
	public:
		int n;
		int p;
		double* Y;
		double** X;
		double* Coeff;
	
	public:
		Regression(int n0,int p0){
			n=n0;
			p=p0;
			Coeff = new double[p0];
			for (int i=0; i<p0; i++){
				Coeff[i]=0;
			}
		}								

		Regression(int n0,int p0, double* Y0, double** X0){
			n=n0;
			p=p0;
			Y=Y0;
			X=X0;
			Coeff = new double[p0];
			for (int i=0; i<p0; i++){
				Coeff[i]=0;
			}
		}
		
		~Regression(){
			delete [] Coeff;
		}

		// if the matrix is inversible, this function computes the 
		// regression coefficients and returns true; 
		// else, it returns false and deletes the matrix
		bool Calculate(){

			// Matrice represents (tX)*X
			double** Matrix = new double* [p];
			for (int i=0; i<p; i++){
				Matrix[i] = new double [p];
			}
			for (int i=0; i<p; i++){
				for (int j=0; j<p; j++){
					Matrix[i][j] = 0;
				}
			}												

			for (int i=0; i<p; i++){
				for (int j=0; j<p; j++){
					for (int k=0; k<n; k++){
						Matrix[i][j] += X[i][k]*X[j][k];
					}
				}
			}												

			double** Inverse = new double* [p];
			for (int i=0; i<p; i++){
				Inverse[i] = new double [p];
			}
			for (int i=0; i<p; i++){
				for (int j=0; j<p; j++){
					Inverse[i][j] = 0;
				}
			}												
			
			bool inversible = inverse(Matrix,Inverse,p);	
															
			if (inversible == false){
				for (int i=0; i<p; i++){
					delete[] Matrix[i];
				}
				delete[] Matrix;
				for (int i=0; i<p; i++){
					delete[] Inverse[i];
				}
				delete[] Inverse;
				return false;
			}												
															
			else {

				//Vect represents (tX)*Y
				double* Vect = new double [p];
				for (int i=0; i<p; i++){
					Vect[i]=0;
				}											
				for (int i=0; i<p; i++){
					for (int j=0; j<n; j++){
						Vect[i] += X[i][j]*Y[j];
					}
				}											

				for (int i=0; i<p; i++){
					for (int j=0; j<p; j++){
						Coeff[i] += Inverse[i][j]*Vect[j];
					}
				}											

				delete[] Vect;
				for (int i=0; i<p; i++){
					delete[] Matrix[i];
				}
				delete[] Matrix;
				for (int i=0; i<p; i++){
					delete[] Inverse[i];
				}
				delete[] Inverse;
			return true;
			}
		}

		// copies the regression coefficients into a new table, pointed by *Coeff2
		void result (double * Coeff2){
			for(int i=0;i<p;i++){
				Coeff2[i]=Coeff[i];
			}
		}

// returns true if A is inversible and B is the inverse of A 
// returns false if A is not inversible
bool inverse(double **A,double **B, int n){
	int i,j,k,l;
	for (i=0;i<n;i++){
		for (j=0;j<n;j++){
			if (i==j) B[i][j]=1;
			else B[i][j]=0;
		}
	}
	double c;
	double d;
	for (i=0;i<n;i++){
		l = i;
		while (abs(A[l][i])<epsilon){
			l++;
			if (l>=n){return false;}
		}//end while

		exchange (A,i,l,n);
		exchange (B,i,l,n);
		c=A[i][i];
		for (j=0;j<n;j++){
			A[i][j]=A[i][j]/c;
			B[i][j]=B[i][j]/c;
		}
		// normalises lign i
		for (k=i+1;k<n;k++){
			d = A[k][i];
			for (j=0;j<n;j++){
				A[k][j]=-A[i][j]*d+A[k][j];
				B[k][j]=-B[i][j]*d+B[k][j];
			}
		}
		//adds (lign i)*(-A(ki)) to lign k
	}
	for (i=n-1;i>=0;i--){
		for (j=i+1;j<n;j++){
			for (k=0;k<n;k++){
				B[i][k]=B[i][k]-A[i][j]*B[j][k];
			}
		}
	}
	// substracts (Aij * lign j) to lign i
	return true;
	
}																
																

// interchanges lines i and l in the matrix A for the first n columns
void exchange(double **A, int i, int l, int n){
	if(i!=l){
		int j;
		double x;
		for (j=0;j<n;j++){
			x=A[i][j];
			A[i][j]=A[l][j];
			A[l][j]=x;
		}
	}											
													
}
};															
