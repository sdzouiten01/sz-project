#pragma once
#include <vector>
#include "../../Projection classes/Country projection classes/Projection_BL_LIE_CH_WBL_01.h"
#include "../../Model point classes/Country model point classes/model_point_BL_RS_DE_LIE_01.h"

using namespace std;

extern ofstream trace;

class Assumption_BL_LIE_DE_GMWBL_01:public Assumption_BL_LIE_CH_GMWBL_01
{
public:
	// Assumptions tables------------------------------------------------		
	double RRC;
	int DB_Y_N;

	virtual ~Assumption_BL_LIE_DE_GMWBL_01();
	Assumption_BL_LIE_DE_GMWBL_01();
	Assumption_BL_LIE_DE_GMWBL_01(const char * tables);
};
class Projection_BL_LIE_DE_GMWBL_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LIE_DE_GMWBL_01 *Assumption;

	//Model Point Class
	Model_Point_BL_RS_DE_LIE_01 *ModelPoint;

	double basis_capital_1;
	double gmdb;
	double account_value_2;	//BG input
	double add_coupon_from_bonus; //BG input

	~Projection_BL_LIE_DE_GMWBL_01();
	Projection_BL_LIE_DE_GMWBL_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path);
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