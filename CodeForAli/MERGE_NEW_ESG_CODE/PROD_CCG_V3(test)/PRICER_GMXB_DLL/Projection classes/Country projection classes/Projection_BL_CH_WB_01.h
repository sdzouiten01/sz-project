#pragma once

#include "../../Projection classes/Projection.h"
#include "../../Model point classes/Country model point classes/model_point_BL_RST_SWISS_01.h"
#include "head_BL_CH.h"

using namespace std;

extern ofstream trace;

class Projection_BL_CH_GMWB_01 : public Projection {

public:
	
	Assumption0 *Assumption;
	//Model Point Class
	Model_Point_BL_RST_SWISS_01 *ModelPoint;


	~Projection_BL_CH_GMWB_01();
	Projection_BL_CH_GMWB_01(const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
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
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	void projection_base_contract(int curr_shock_number);
	void PresentValueCalculation(int curr_shock_number,bool delta);
	
};