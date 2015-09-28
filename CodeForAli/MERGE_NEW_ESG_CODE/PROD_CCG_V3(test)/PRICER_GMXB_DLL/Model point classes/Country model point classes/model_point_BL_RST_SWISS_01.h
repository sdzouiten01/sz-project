#ifndef __Model_Point_BL_RST_SWISS_01_H_
#define __Model_Point_BL_RST_SWISS_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_SWISS.h"

using namespace std;

class Model_Point_BL_RST_SWISS_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	string   Prod_Name_Bl;
	string   Nb_Date_Bl;
	int      Duration_Bl;
	double   Withdrawal_Mode_discount_Bl;
	double   Perf_Bonus_Amount_Bl;
	double   Av_bef_bef_bl;
	double   Av_After_Bef_Bl;
	double   Fund_Unit_Bef_Bef_Bl;
	double   Fund_Unit_After_Bef_Bl;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	int      Calc_Nb_Date_Year;
	int      Calc_Nb_Date_Month;
	int      Calc_Nb_Date_Day;
	int      Calc_Inv_Date_Year;
	int      Calc_Inv_Date_Month;
	int      Calc_Inv_Date_Day;
	int      Calc_NB_Coupons;
	double   Calc_Perf_By_Freq;
	double   Calc_Max_Bonus_Amount_By_Freq;

	
	virtual ~Model_Point_BL_RST_SWISS_01();
	Model_Point_BL_RST_SWISS_01(vector<string>& row, int rank, Scenario_configuration& scenario_config);
	Model_Point_BL_RST_SWISS_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb);
	Model_Point_BL_RST_SWISS_01(Model_Point * mp);
	
	void set_Calc_information_to_Zero();
	void initialise_mp_from_inforce(vector<string>& row);
	void initialise_mp_parameters_by_copy(Model_Point * m_p);

};

#endif
