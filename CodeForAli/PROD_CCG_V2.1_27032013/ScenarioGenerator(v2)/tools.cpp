#include "tools.h"
#include "GMXBException.h"
#include <math.h>

const double EPSILON = 1e-10;

// Cholesky decomposition :	takes a matrix A as input which must be definite positive
//							returns the unique matrix T triangular inferior with positive coefficients on the diagonal
//							such that A = TT*

// Can be dangerous to use with matrixes that are close to not being definite positive (ex : correl near 1)


void Cholesky(double** input, int dimension){
	for(int i=0;i<dimension;i++){
		for(int k=0;k<=i-1;k++){
			input[i][i]-=input[i][k]*input[i][k];
		}
		if(input[i][i]<EPSILON){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Correlation Matrix is not definite positive");
			throw new GMXBException(message);
		}
		input[i][i]=sqrt(input[i][i]);
		for(int j=i+1;j<dimension;j++){
			for(int k=0;k<=i-1;k++){
				input[j][i]-=input[i][k]*input[j][k];
			}
			input[j][i]=input[j][i]/input[i][i];
		}			
	}
}


// num_vol : nb of time steps for which a volatility is given as input (size of vol[])
// vol[k] : volatilities for duration vol_time[k] (vol_time[k] is expressed in months if BI.delta_t = 12)
// num_step : date (in months if BI.delta_t = 12) for which we want to calculate the volatility (the first month corresponds to num_step = 0)

// Interpolates the volatility at time num_step given volatilities at fixed time steps
double interpole(int num_vol,int num_step,SmartPointer<int> vol_time, double* vol){
	double output=0;
	bool finished = false; // Indicates whethter the correct interpolation interval has already been found
	if(num_vol==1) output=vol[0];
	if(num_vol>1){
		if(num_step>=vol_time[num_vol-1]-1){
			// step by step extrapolation
			output=vol[num_vol-1];
			finished = true;
		}
		for(int j=2;j<=num_vol&&finished==false;j++){
			if(num_step>=vol_time[num_vol-j]-1){
				// step by step interpolation, matches Moses (takes the vol given for the next maturity)
				if(num_step==vol_time[num_vol-j]-1){
					output=vol[num_vol-j];
				}
				else{
					output=vol[num_vol-j+1];
				}
				finished = true;
			}
		}
		if(finished==false){
			// if vol_time[0]>1
			output=vol[0];
		}
	}
	return output;
}

double linear_interp(double* arrayT, double* arrayY, int num, double term)
{
	//	Given the array of terms arrayT and the array of yields arrayY, and given a value 
	//	of "term" this routine returns a linearly interpolated value "yield"

	int index_low = 0;
	int index_high = num - 1;
	int index = 0;
	double h = 0.0;
	double b = 0.0;
	double a = 0.0;
		
	// Finds the right place in the table by means of bisection
	while (index_high - index_low > 1)
	{
		index = (index_low + index_high)/2;
		if (term > arrayT[index])
			index_low = index;
		else 
			index_high = index;
	}

	h = arrayT[index_high] - arrayT[index_low];
	
	a = (arrayT[index_high] - term) / h;
	b = (term - arrayT[index_low]) / h;

	return (a * arrayY[index_low] + b * arrayY[index_high]);
}