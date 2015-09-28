#ifndef SHORT_RATE_MODEL_H
#define SHORT_RATE_MODEL_H

#include "tools.h"

class Rate_Model{

public:

	   //modele Hull et White a un facteur de vol constante
	   static void Hull_White(double** output1, double** output2,double* input,int position_step,double a,double sigma,double delta_t,Basic_Information &BI);

	   //calcul du taux zero coupon en jugeant le parametre rm a decider le modele a utiliser
	   static double interest_rate(double maturity, int position_step, double delta_t, double** zero_coupon);

	   //calcul du taux swap en jugeant le parametre rm a decider le modele a utiliser
	   static double swap_rate(double maturity,int position_step, double delta_t,double coupon_period, double** zero_coupon);

	   //calcul du bond return en jugeant le parametre rm a decider le modele a utiliser
	   static double bond_return(double maturity, double position_step,double delta_t,double** zero_coupon1, double** zero_coupon2);

	   static double bond_return_coupon(double maturity,double position_step,double delta_t,double** zero_coupon1, double** zero_coupon2, double coupon, double coupon_period);

static double ZCB(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t, double* B1, double* C1);

static double interest_rate(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t, double* B1, double* C1);

static double swap_rate(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t,double coupon_period, double* B1, double* C1);

static double bond_return(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate1,double short_rate2,int position_step,double delta_t, double* B1, double* C1);

static double bond_return_coupon(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate1,double short_rate2,int position_step,double delta_t,double coupon, double coupon_period, double* B1, double* C1);

};

#endif