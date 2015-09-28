#ifndef VARIANCEREDUCTION_H
#define VARIANCEREDUCTION_H

#include <iostream>
#include <fstream>
#include "../Instruments/InterfaceESG0.h"
class VarianceReduction{
public:
	
	int NB_index;
	int NB_Scen;
	int Antithetic;	
	int NB_Scen_Antithetic;
	int NB_Time_Step;
	double Time_Step_t;
	double HW_a;
	double HW_sigma;
	int* Vol_Time;
	int Vol_Time_Size;	
	double** Vol_Equity;
	double** Error_table;
	double **Martingale_level_try_tab;

		
	VarianceReduction(){}
	void Init_VarianceReduction( int nB_index,int nB_Scen,int antithetic,int nB_Time_Step,int delta_t,int* vol_Time,int vol_Time_Size,	
						double** vol_Equity,double** error_table,double **martingale_level_try_tab);
	VarianceReduction(InterfaceESG0 * &interface_Esg);
	~VarianceReduction();

	void Black_Scholes_Formula(double* random_input_tab,double* scen_ouput_tmp_tab, double vol);
	bool Test_Martingale(double* scen_input_tmp_tab,double* cumul_scen_ouput_tmp_tab, int index_position, int step_position);
	bool Test_beta(double* scen_input_tmp_tab,double* cumul_scen_ouput_tmp_tab, int index_position, int step_position);
	
	bool Test_Equity(double** random_index_input_table, int index_position);
	double Interpole_Volatility(int index_position,int step_position);
	void ANTITHETIC(double* random_input_tab);
	void Hull_White_Formula( double* random_IR_gaussian1_input_table, double* random_IR_gaussian2_input_table,double* input_integral_gaussian,double* scen_ouput_tmp_tab,int step_position);
	bool Test_Rate(double** random_IR_gaussian1_input_table,double** random_IR_gaussian2_input_table);
	void Adjust_Error_table();
	void Init_Calib_parrameters(double hW_a,double hW_sigma);
	//Constructor using a class of interface


};
#endif