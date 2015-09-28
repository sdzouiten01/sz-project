#pragma once
#include "../../Projection classes/Projection.h"
#include "../../Model point classes/Country model point classes/model_point_BL_RST_CH_LIE_01.h"
#include "head_BL_LIE.h"

using namespace std;

extern ofstream trace;

class Assumption_BL_LIE_CH_GMWB_01 :public Assumption0
{
public:
	// Assumptions tables------------------------------------------------
	double Lapse_Charge; //BGinput
	double Min_Surrender_Charge_Amount; //BGinput
		
	~Assumption_BL_LIE_CH_GMWB_01();
	Assumption_BL_LIE_CH_GMWB_01();
	Assumption_BL_LIE_CH_GMWB_01(const char * tables);
};
class Projection_BL_LIE_CH_GMWB_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LIE_CH_GMWB_01 *Assumption;
	//Model Point Class
	Model_Point_BL_RST_CH_LIE_01 *ModelPoint;

	double account_value_2;	//BG input
		
	~Projection_BL_LIE_CH_GMWB_01();
	Projection_BL_LIE_CH_GMWB_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
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
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void projection_base_contract(int curr_shock_number);
	void PresentValueCalculation(int curr_shock_number,bool delta);

};