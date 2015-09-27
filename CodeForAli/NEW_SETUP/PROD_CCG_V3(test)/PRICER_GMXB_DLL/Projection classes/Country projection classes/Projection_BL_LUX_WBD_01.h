#pragma once

#include "../../Projection classes/Projection.h"
#include "../../Model point classes/Country model point classes/model_point_BL_LUX_01.h"
#include "head_BL_LUX.h"

using namespace std;

extern ofstream trace;
class Assumption_BL_LUX_GMWBD_01 : public Assumption0
{
public:
	// Assumptions tables------------------------------------------------
	GMXB_Assumption_Table *mortality_table;
		
	int Mortality_Y_N;
	int Max_Age_Mortality;
	double Best_Estimate_Lapse;
	double Min_Lapse_Y_1;
	double Min_Lapse;
	double X_5;
	double Weight;
	int Mortality_Lapse_Y_N;
	double Lapse_Charges;
	int Max_Age_DB;
	double Min_Surrender_Charge_Amount;

	int index_1_local;
	int index_2_local;
	double mort_value_local;
	double dyn_lapse_local;
	double param_local1;
	double param_local2;

	virtual ~Assumption_BL_LUX_GMWBD_01();
	Assumption_BL_LUX_GMWBD_01();
	Assumption_BL_LUX_GMWBD_01(const char * tables);
	virtual double Get_mortality_rates_vector(const string& sex,const int& current_age);
	virtual double Get_Dynamic_Lapse_Value(double moneyness,int current_duration_year);
};
class Projection_BL_LUX_GMWBD_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LUX_GMWBD_01 *Assumption;

	//Model Point Class
	Model_Point_LUX_01 *ModelPoint;

	~Projection_BL_LUX_GMWBD_01();
	Projection_BL_LUX_GMWBD_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
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