#ifndef BS_WITHOUT_CIR_H
#define BS_WITHOUT_CIR_H
#include "Diffusion_Model0.h"
#include "../Instruments/InterfaceESG0.h"
#include <Math.h>
using namespace std;

class BS_WITHOUT_CIR: public DIF_MODEL0{
public:
	//int Time_step;
	//double Delta_t;
	//double HW_Alpha;
	//double HW_Sigma;
	//double Prorata;
	//int Antithetic;
	//int NB_Time_Step;
	//int NB_index;
	//int NB_Scen;
	//int NB_Scen_Antithetic;
	//double** Vol_Equity;	
	//double** X_t;
	//double** Z_t;
	//double *Y_t;
	//double Y_t_0;
	//double *Q_t;
	//double **Integrale_Vol_t;
	//double **Integrale_Var_t;

	//void Calculate_Short_rate( double* &input_forward,double** &output_shortRate_tab);
	//void Calculate_Beta( double* &input_inverse_ZCB,double** &output_beta_tab);
	//void Calculate_Notional_Rate( double &maturity, double* &input_Log_ZCB,double** &output_rate_tab);
	//double ZCB(double maturity,double* &input_Log_ZCB, int step_position,int scn_position);
	//void Calculate_Swap_Rate(double &maturity, double* &input_Log_ZCB,double** &output_rate_tab, double &coupon_period);
	//void Calculate_Bond_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab);
	//void Calculate_Bond_Coupon_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab, double coupon,double coupon_period);
	//void Calculate_Equity_Return(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab);
	//void Initialize_Class_Parameters(double** &input_Gaussian1_ShortRate_tab,double** &input_Gaussian2_ShortRate_tab);
	//void Calculate_Equity_Return_With_Drift(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab, 
	//											 double* &input_inverse_ZCB, double* &input_inverse_ZCB_Drift);
	BS_WITHOUT_CIR(InterfaceESG0 * &interface_Esg);
	//void Initialize_Calib_parameters(double hW_Alpha,double hW_Sigma);
	~BS_WITHOUT_CIR();


};

#endif