#pragma once

#include "../../Projection classes/Country projection classes/Projection_BL_LUX_WBD_01.h"
#include "../../Model point classes/Country model point classes/model_point_BL_LUX_01.h"

using namespace std;

extern ofstream trace;
class Assumption_BL_LUX_GMWBLD_01 : public Assumption_BL_LUX_GMWBD_01
{
public:
	// Assumptions tables------------------------------------------------
	GMXB_Assumption_Table *Longevity_mortality_table;
	int Longevity_Y_N;

	virtual ~Assumption_BL_LUX_GMWBLD_01();
	Assumption_BL_LUX_GMWBLD_01();
	Assumption_BL_LUX_GMWBLD_01(const char * tables);
	double Get_mortality_rates_vector(const string& sex,const int& current_year,const int& valn_age,const int& valn_year);
	double Get_longevity_mortality_rates_vector(const string& sex,const int& current_year,const int& valn_age,const int& valn_year);
};
class Projection_BL_LUX_GMWBLD_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LUX_GMWBLD_01 *Assumption;

	//Model Point Class
	Model_Point_LUX_01 *ModelPoint;

	~Projection_BL_LUX_GMWBLD_01();
	Projection_BL_LUX_GMWBLD_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void mySet_Debug_File_Header();
	void set_current_model_point(Model_Point *mp);
	void init_Count_scen_to_zero();
	int Count_scen();
	void Increase_Count_scen();
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