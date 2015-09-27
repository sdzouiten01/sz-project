#include "Equity model.h"
#include <math.h>

void Equity_Model::Black_Sholes(double* output, double vol,Basic_Information &BI,double delta_t){
	double A = exp(-vol*vol*delta_t/2.0);
	double b = sqrt(delta_t)*vol;
	double c=1;
	int n=BI.nb_scens()/BI.antithetic();
	int antithetic_var = BI.antithetic();
	for(int j=0;j<n;j++){
		c = exp(b*output[j]);
		output[j]=A*c;
		
		if(antithetic_var==2){
			output[j+n]=A/c;
		}
	}
	
	
	// The code below is equivalent but slower when there are antithetic variables (two exponentials to compute)
	// for(int j=0;j<BI.nb_scens();j++){
	//	 output[j]=exp(-vol*vol*delta_t/2.0+output[j]*sqrt(delta_t)*vol);
	// }
}
