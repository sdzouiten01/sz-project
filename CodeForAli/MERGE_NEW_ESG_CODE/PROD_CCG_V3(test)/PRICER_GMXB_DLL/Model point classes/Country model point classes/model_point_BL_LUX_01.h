#ifndef __Model_Point_LUX_01_H_
#define __Model_Point_LUX_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_LUX.h"

using namespace std;

class Model_Point_LUX_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	string Person_Id_1_Lux;
	string Person_Id_2_Lux;
	double Ong_Acqui_Charge_Lux;
	string Gender_1_Lux;
	string Gender_2_Lux;
	string Date_Birth_1_Lux;
	string Date_Birth_2_Lux;
	string Date_Death_1_Lux;
	string Date_Death_2_Lux;
	int    Issue_Age_1_Lux;
	int    Issue_Age_2_Lux;
	int    Tech_Issue_Age_Lux;
	int    Duration_Lux;
	double Rollup_Rate_Lux;
	double Def_Guarantie_Lux;
	double Withrawals_Mode_Discount_Lux;
	string Gmdb_Rider_Lux;
	int    Gmdb_Mode_Lux;
	int    Gmdb_Max_Age_Lux;
	int    Gmdb_Duration_Lux;
	double Gmdb_Lux;
	string Perf_Bonus_Lux;
	double Perf_Bonus_Level_Lux;
	int    Perf_Bonus_Type_Lux;
	double Add_Accum_Withrawal_Amount_Lux;
	double Perf_Bonus_Amount_Lux;
	double Reinsurance_Premium_Lux;
	double Fund_Charge_Lux;
	double Upfront_Comm_Lux;
	double Trail_Comm_Lux;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////

	int      Calc_Birth_1_Date_Year;
	int      Calc_Birth_1_Date_Month;
	int      Calc_Birth_1_Date_Day;	
	int      Calc_Birth_2_Date_Year;
	int      Calc_Birth_2_Date_Month;
	int      Calc_Birth_2_Date_Day;	
	int      Calc_Death_1_Date_Year;
	int      Calc_Death_1_Date_Month;
	int      Calc_Death_1_Date_Day;	
	int      Calc_Death_2_Date_Year;
	int      Calc_Death_2_Date_Month;
	int      Calc_Death_2_Date_Day;	
	int      Calc_Age_1;
	int      Calc_Age_Month_1;
	int      Calc_Age_2;
	double   Calc_fixed_fees_p_m;
	double   Calc_WB_Rollup;
	double	 Calc_Initial_Coupon_by_Freq;
	double   Calc_BK0;
	double   Calc_RRC_p_m;
	double   Calc_Conversion_Rate_p_m;
	double   Calc_Min_Bonus_Amount;
	int		 Calc_Initial_Deferal_Months;
	double   Calc_Gaur_Charge_Rate;


	virtual ~Model_Point_LUX_01();
	Model_Point_LUX_01(vector<string>& row, int rank, Scenario_configuration& scenario_config);
	Model_Point_LUX_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb);
	Model_Point_LUX_01(Model_Point * mp);
	
	void set_Calc_information_to_Zero();
	void initialise_mp_from_inforce(vector<string>& row);	
	void initialise_mp_parameters_by_copy(Model_Point * m_p);	
	

};

#endif
