#ifndef __Model_Point_BL_RS_CH_LIE_01_H_
#define __Model_Point_BL_RS_CH_LIE_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_LIE.h"

using namespace std;

class Model_Point_BL_RS_CH_LIE_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	string Person_Id_1_Bl;
	string Person_Id_2_Bl;
	double Ong_Charge_1_Bl;	
	double Ong_Charge_2_Bl;	
	string Gender_1_Bl;
	string Gender_2_Bl;
	string Date_Birth_1_Bl;
	string Date_Birth_2_Bl;
	string Date_Death_1_Bl;
	string Date_Death_2_Bl;
	int    Issue_Age_1_Bl;
	int    Issue_Age_2_Bl;	
	double Withrawals_Mode_Discount_Bl;
	double Perf_Bonus_Amount_Bl;		
	double Av_Bef_Bef_Bl;		
	double Av_After_Bef_Bl;		
	double Fund_Unit_Bef_Bef_Bl; 
	double Fund_Unit_After_Bef_Bl; 

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
	double   Calc_WB_Initial;			
	double	 Calc_Initial_Coupon_by_Freq;
	int		 Calc_Duration_Bl;
	double   Calc_BK0;
	double   Calc_RRC_p_m;
	double   Calc_Conversion_Rate_p_m;
	double   Calc_Max_Bonus_Amount;		
	int		 Calc_Initial_Deferal_Months;
	double   Calc_Gaur_Charge_Rate;
	double	 Calc_Perf_Bonus_Amount_Bl;	

	virtual ~Model_Point_BL_RS_CH_LIE_01();
	Model_Point_BL_RS_CH_LIE_01(vector<string>& row, int rank, Scenario_configuration& scenario_config);
	Model_Point_BL_RS_CH_LIE_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb);
	Model_Point_BL_RS_CH_LIE_01(Model_Point * mp);
	
	void set_Calc_information_to_Zero();
	void initialise_mp_from_inforce(vector<string>& row);	
	void initialise_mp_parameters_by_copy(Model_Point * m_p);

};

#endif
