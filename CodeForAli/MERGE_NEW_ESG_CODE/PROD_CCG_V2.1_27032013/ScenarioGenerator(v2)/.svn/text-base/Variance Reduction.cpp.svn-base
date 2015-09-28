#include "Variance Reduction.h"
#include <math.h>

// Fills the second half of the scenario array with antithetic variables.
void Variance_Reduction::ANTITHETIC(double** input_output,Basic_Information &BI, int position_step){
	if(Antithetic){
		int nb_scen = BI.nb_scens()/2;
		for(int j=0;j<nb_scen;j++){
			input_output[position_step][j+nb_scen]=-input_output[position_step][j];
		}
	}
}

// Tests if the product over time X1*...*Xt of the returns of the equity is satisfactory (for equities).
// Xi = exp(-voli*voli*delta_t/2+voli*sqrt(delta_t)*Wi) where Wi are Gaussian independent variables Wi ~ N(0,1) 
bool Variance_Reduction::test_martingale(double* tab, double* traj, Basic_Information &BI, double e){
	bool control=1;
	double mean=0;
	for(int j=0;j<BI.nb_scens();j++){ // tab is an array containing the input Xt
		traj[j]*=tab[j]; // traj is an array which memorizes the product  X1*...*Xt-1 (starts initialised at 1)
		mean+=traj[j]; // E[X1*...*Xt] = 1;
	}
	mean=mean/BI.nb_scens();
	mean=abs(mean-1);
	if(mean>e){control=0;}
	return control;
}

// Tests if B(0,t)/beta(t) is worth 1 in average (for rates).
bool Variance_Reduction::test_beta(double* tab, double* traj, Basic_Information &BI, double e){
	bool control=1;
	double mean=0;
	for(int j=0;j<BI.nb_scens();j++){
		traj[j]*=tab[j];
		mean+=1/traj[j];
	}
	mean=mean/BI.nb_scens();
	mean=abs(mean-1);
	if(mean>e){control=0;}
	return control;
}

// Checks various martingality conditions and calls the rate model (Hull and White)  to fill the scenario array.
bool Variance_Reduction::test_rate(Scenario *s,Basic_Information &BI,int position_index,int position_step,double* tmp, double e){
	bool control=1;
	// Needs to call antithetic now because the variables in scenario[3+position_index+BI.nb_rate()] will be
	// replaced one by one during the calculation of short_rate(...) and aren't also given as input in random.
	ANTITHETIC(s->scenario[3+position_index+BI.nb_rate()],BI,position_step+1);
	// Calls the Hull and White model.
	Rate_Model::Hull_White(s->scenario[2],s->scenario[3+BI.nb_rate()],s->random[0][position_step],position_step,BI.a(),BI.sigma(),(1.0/BI.delta_t()),BI);
	// s->scenario[3+BI.nb_rate()] contains the second gaussian variable when flag==1.
	// Notice that the call is made with 1/BI.delta_t() because BI.delta_t is a frequency, not a duration. Ex : BI.delta_t = 12 for monthly simulation.
	if(Beta==1||Beta==3){
		control=test_beta(s->scenario[3+BI.nb_rate()][position_step+1],tmp,BI,e);
	}
	return control;
}

// Checks various martingality conditions and calls the equity model (Black Scholes)  to fill the scenario array.
bool Variance_Reduction::test_equity(Scenario *s,Basic_Information &BI,double vol,int position_index,int position_step,double* tmp,double e){
	bool control=1;
	//ANTITHETIC(s->scenario[3+position_index+BI.nb_rate()],BI,position_step+1);
	// Calls the Black and Scholes model.
	Equity_Model::Black_Sholes(s->scenario[3+position_index+BI.nb_rate()][position_step+1], vol, BI,(1.0/BI.delta_t()));
	// Notice that the call is made with 1/BI.delta_t() because BI.delta_t is a frequency, not a duration. Ex : BI.delta_t = 12 for monthly simulation.
	if(Martingale==1||Martingale==3){
		control=test_martingale(s->scenario[3+position_index+BI.nb_rate()][position_step+1], tmp, BI,e);
	}
	// tmp are used to test martingality conditions on a sum X1+...+Xt or a product Y1*...*Yt over time
	// tmp1[1] and tmp2[1] memorize the value of tmp at the previous time step, calculations are done on tmp1[0] ad tmp2[0].
	return control;
}