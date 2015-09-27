#pragma once
#include "../../Projection classes/Projection.h"
#include "../../Model point classes/Country model point classes/model_point_BL_RS_CH_LIE_01.h"
#include "head_BL_LIE.h"

using namespace std;

extern ofstream trace;

class Assumption_BL_LIE_CH_GMWBL_01	:public Assumption0	
{
public:	
	// Assumptions tables------------------------------------------------		
	GMXB_Assumption_Table *mortality_table;		
	GMXB_Assumption_Table *Longevity_mortality_table;	//BG: needed?		
		
	int Mortality_Y_N;	
	int Max_Age_Mortality;	
	int Mortality_Lapse_Y_N;	
	double Lapse_Charges;	
	double Min_Surrender_Charge_Amount;	
	int Longevity_Y_N;
	int index1_l_local;
	int index_2_local;
	double mort_value_local;
	double dyn_lapse_local;

	virtual ~Assumption_BL_LIE_CH_GMWBL_01();
	Assumption_BL_LIE_CH_GMWBL_01();
	Assumption_BL_LIE_CH_GMWBL_01(const char * tables);
	virtual double Get_mortality_rates_vector(const string& sex,const int& current_year,const int& valn_age,const int& valn_year);	
	virtual double Get_longevity_mortality_rates_vector(const string& sex,const int& current_year,const int& valn_age,const int& valn_year);	
};
class Projection_BL_LIE_CH_GMWBL_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LIE_CH_GMWBL_01 *Assumption;

	//Model Point Class
	Model_Point_BL_RS_CH_LIE_01 *ModelPoint;

	double basis_capital_1;
	double account_value_2;	//BG input

	~Projection_BL_LIE_CH_GMWBL_01();
	Projection_BL_LIE_CH_GMWBL_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void set_Local_Parameters_to_zero();
	void mySet_Debug_File_Header();
	void set_current_model_point(Model_Point *mp);
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void initialise_mp_calculation();
	void Calculate_ConstVector_timing();
	void Calculate_ConstVector_guarantees();
	void Calculate_ConstVector_decrement();
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void projection_base_contract(int curr_shock_number);
	void PresentValueCalculation(int curr_shock_number,bool delta);

};