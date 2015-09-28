#ifndef DIF_MODEL0_H
#define DIF_MODEL0_H

using namespace std;
#define  UNREFERENCED_PARAMETER(P) (P)
class DIF_MODEL0{
public:

	virtual void Calculate_Short_rate( double* &input_forward,double** &output_shortRate_tab){
		UNREFERENCED_PARAMETER(input_forward);
		UNREFERENCED_PARAMETER(output_shortRate_tab);
	}
	virtual void Calculate_Beta(double* &input_inverse_ZCB,double** &output_beta_tab){
		UNREFERENCED_PARAMETER(input_inverse_ZCB);
		UNREFERENCED_PARAMETER(output_beta_tab);
	}
	virtual void Calculate_Notional_Rate(double &maturity, double* &input_Log_ZCB,double** &output_rate_tab){
		UNREFERENCED_PARAMETER(maturity);
		UNREFERENCED_PARAMETER(input_Log_ZCB);
		UNREFERENCED_PARAMETER(output_rate_tab);
	}
	virtual void Calculate_Swap_Rate(double &maturity, double* &input_Log_ZCB,double** &output_rate_tab, double &coupon_period){
		UNREFERENCED_PARAMETER(maturity);
		UNREFERENCED_PARAMETER(input_Log_ZCB);
		UNREFERENCED_PARAMETER(output_rate_tab);
		UNREFERENCED_PARAMETER(coupon_period);
	}
	virtual void Calculate_Bond_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab){
		UNREFERENCED_PARAMETER(maturity);
		UNREFERENCED_PARAMETER(input_Log_ZCB);
		UNREFERENCED_PARAMETER(output_bond_tab);
	}
	virtual void Calculate_Bond_Coupon_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab, double coupon,double coupon_period){
		UNREFERENCED_PARAMETER(maturity);
		UNREFERENCED_PARAMETER(input_Log_ZCB);
		UNREFERENCED_PARAMETER(output_bond_tab);
		UNREFERENCED_PARAMETER(coupon);
		UNREFERENCED_PARAMETER(coupon_period);
	}
	virtual void Calculate_Equity_Return(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab){
		UNREFERENCED_PARAMETER(index);
		UNREFERENCED_PARAMETER(input_gaussian);
		UNREFERENCED_PARAMETER(input_beta);
		UNREFERENCED_PARAMETER(output_equity_return_tab);
	}
	virtual void Initialize_Calib_parameters(double hW_Alpha,double hW_Sigma){
		UNREFERENCED_PARAMETER(hW_Alpha);
		UNREFERENCED_PARAMETER(hW_Sigma);
	}
	virtual void Initialize_Class_Parameters(double** &input_Gaussian1_ShortRate_tab,double** &input_Gaussian2_ShortRate_tab){
		UNREFERENCED_PARAMETER(input_Gaussian1_ShortRate_tab);
		UNREFERENCED_PARAMETER(input_Gaussian2_ShortRate_tab);
	}
	virtual	void Calculate_Equity_Return_With_Drift(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab, 
		double* &input_inverse_ZCB, double* &input_inverse_ZCB_Drift)
	{
		UNREFERENCED_PARAMETER(index);
		UNREFERENCED_PARAMETER(input_gaussian);
		UNREFERENCED_PARAMETER(input_beta);
		UNREFERENCED_PARAMETER(output_equity_return_tab);
		UNREFERENCED_PARAMETER(input_inverse_ZCB);
		UNREFERENCED_PARAMETER(input_inverse_ZCB_Drift);
	}
	DIF_MODEL0(){}
	virtual ~DIF_MODEL0(){}
};

#endif