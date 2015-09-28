#include <algorithm>
#pragma once
//#include "stdafx.h"

#include "../Model point classes/Model_Point.h"
//#include <string>
//#include <fstream>
#include "GMXBParameters.h"
#include "../Utility classes/head.h"

using namespace std;

extern ofstream trace;

class Assumption0 
{
	public:
		// Assumptions tables------------------------------------------------
		GMXBParameters  *assump;
		int Ratchet_Y_N;
		int Perf_Bonus_Y_N;
		double Percentage_Perf;
		double Bonus_rate;
		int Use_Lapse_Y_N;
		double Base_Lapse;
		double Base_Lapse_p_m;
		int Dynamic_Lapse_Y_N;
		double X_1;
		double X_2;
		double X_3;
		double X_4;
		double Y_1;
		double Y_2;
		double Y_3;
		double Y_4;
		double Guarantee_Charge_Before;
		double Guarantee_Charge_After_04_2015;
		double Guarantee_Charge_Before_p_m;
		double Guarantee_Charge_After_04_2015_p_m;
		double Guarantee_Charge;
		double Guarantee_Charge_p_m;
		double Fixed_Charge;
		double Fixed_Charge_p_m;
		double Retrocessions_Fund;
		
public:
	virtual ~Assumption0(){delete this->assump;}
	Assumption0();
	Assumption0(const char * tables);
	virtual double Get_Dynamic_Lapse_Value(double moneyness);
};

class Projection{
public:
	
	double ConstVector[TOTAL_CONSTVECTOR_COLS][SUBACCT_MAX_PERIOD_MTH +1];
	double MainProjArrayVector[MAIN_PROJ_NUM_COLS][SUBACCT_MAX_PERIOD_MTH +1];
	double DebugVector[DEBUG_NUM_COLS][SUBACCT_MAX_PERIOD_MTH +1];
	int myConsVector_size;
	int myMainProjArrayVector_size;
	int myDebugVector_size;

	Model_Point *myModel_Point;
	int myPosition_MP_in_File;
	int myCalc_Remaining_Months;
	double myCalc_Prorata;

	double ** myMonthly_Return_table;
	int myCurrentScen;
	static int compt_sen; //AJOUT
	int total_number_scn;

	int ** ShockArray;
	int ShockSize;
	int myShockNumber;

	ofstream debug_file;
	string myDbgFilePath;
	bool myGenerateDebugFiles;

	double account_value;
	double survival_index;
	double survival_index_by_step;
	double claims_wb_t;
	double claims_db_t;
	double GMDB_level;
	double basis_capital;
	double annuity_per_freq;	
	double add_coupon_from_ratchet;
	double Performance_bonus_valuation;
	double bonus_value;	
	double fund_fees;
	double lapse;
	double itm;
	double lapse_val;
	double account_value_1;
	double hedge_cost_fees;
	double fixed_charges;
	double mortality_rate;
	double sumprod_dynamic_lapse;
	double dynamic_lapse_ratio;
	
	
	
	virtual ~Projection();
	virtual void mySet_Debug_File_Header();
	virtual void projection_base_contract(int curr_shock_number);
	virtual void PresentValueCalculation(int curr_shock_number,bool delta);
	virtual void Calculate_ConstVector_timing();
	virtual void Calculate_ConstVector_guarantees();
	virtual void Calculate_ConstVector_decrement();
	virtual void initialise_mp_calculation();
	virtual void set_current_model_point(Model_Point *mp);
	virtual void set_Local_Parameters_to_zero();
	
	void myWrite_Debug_File();
	void set_value_on_debug_file ( double value, int row, int t);
	void set_DebugVector_to_zero();
	void set_ConstVector_to_zero();
	void set_MainProjArrayVector_to_zero();
	
	void set_Local_Parameters_to_zero0();	
	void set_monthly_rates(double ** monthly_rates, int scen_num);
	void init_market_data();
	void set_index_shock_array(const vector<vector <int>>& i_s_a);
	void project();
	void InitProjection();
	void set_total_number_scn( int scn_numb);
	Projection(const vector<vector <int>>& i_s_a, const char * tables, bool generate_dbg_files, const string& debug_file_path);	

};

//int Projection::compt_sen=0;
