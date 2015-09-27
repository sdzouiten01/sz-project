#ifndef VARIANCE_REDUCTION_H
#define VARIANCE_REDUCTION_H

#include "Equity model.h"
#include "short rate model.h"
#include "Scenario.h"

class Variance_Reduction{
private:bool Antithetic;
		int Martingale;
		int Beta;

public:Variance_Reduction(){}
	   Variance_Reduction(bool antithetic,int martingale, int beta){
		   Antithetic=antithetic;
		   Martingale=martingale;
		   Beta=beta;
	   }
	   Variance_Reduction(Variance_Reduction &t){
		   Antithetic=t.Antithetic;
		   Martingale=t.Martingale;
		   Beta=t.Beta;
	   }
	   Variance_Reduction copy(bool antithetic,int martingale, int beta){
		   Antithetic=antithetic;
		   Martingale=martingale;
		   Beta=beta;
		   return *this;
	   }
	   Variance_Reduction copy(Variance_Reduction t){
		   Antithetic=t.Antithetic;
		   Martingale=t.Martingale;
		   Beta=t.Beta;
		   return *this;
	   }

	   ~Variance_Reduction(){}

	   void ANTITHETIC(double** input_output,Basic_Information &BI, int position_step);

       bool test_martingale(double* tab, double* traj, Basic_Information &BI, double e);

       bool test_beta(double* tab, double* tmp_3, Basic_Information &BI, double e);

       bool test_equity(Scenario *s,Basic_Information &BI,double vol,int position_index,int position_step,double* tmp,double e);

       bool test_rate(Scenario *s,Basic_Information &BI,int position_index,int position_step,double* tmp, double e);

	   int mart(){return Martingale;}
	   int beta(){return Beta;}
};
#endif