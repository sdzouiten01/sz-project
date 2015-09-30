#pragma once

#define PRODUCT	0
#define VALN_DATE	1
#define FUND_FEES	2
#define INFO	3
#define PROD_CODE	4
#define EHC	5
#define NEW_BUSINESS	6
#define POL_ID	7


#include "../Utility classes/stdafx.h"
//#include <string>
//#include <time.h>
//#include <string>
#include <fstream>

//#include <string>
#include "../Utility classes/Scenario_configuration.h"


//#include "Utility classes/Product_configuration.h"

using namespace std;
extern ofstream trace;

class Model_Point
{

public:
	string   Product_Version;
	string   Prod_Code;
	string   Valn_Date;
	double   Fund_Fees;
	double   New_Re_Reinsurance_premium;
	double   Ehc;
	int      New_Business;
	string   Pol_Id;
	double   Init_Charge;
	string   Fund_Name;
	string   Currency;
	string   Issue_Date;
	string   Inv_Date;
	int      Def_Period;
	string   Annuity_Begin;
	string   Prod_Prem_Type;
	double   Premium;
	double   Amount_Inv;
	string   Freq_Mode;
	double   Withdrawal_Rate;
	double   Inv_Protection;
	double   Benefit_Increase_Factor;
	double   Guarantee;
	double   Accum_Withdrawal_Amount;
	double   Av_After_After;
	double   Base_Capital;
	double   Fund_Unit_After_After;
	double   Unit_Price;
	string   Nav_Reporting_Date;
	double   AV0;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	int      Calc_Valn_Year;
	int      Calc_Valn_Month;
	int      Calc_Valn_Day;
	int      Calc_IssueDate_Year;
	int      Calc_IssueDate_Month;
	int      Calc_IssueDate_Day;
	int      Calc_Annuity_Begin_Year;
	int      Calc_Annuity_Begin_Month;
	int      Calc_Annuity_Begin_Day;
	double   Calc_Prorata;
	double   Calc_Fund_Fees_p_m;
	double   Calc_Ehc_p_m;
	int      Calc_Freq_Pay;	
	int      Calc_Remaining_Months;
	int      Calc_Duration_Months;
	int      Calc_Deferal_Duration_Months;
	double	 Calc_Coupon_by_Freq;
	int      Calc_Mod_Def_Duration_Months;
	int      Calc_Count_Scen;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////	
	
	int mp_position_in_file;
	double **index_shock_pvs;
	int index_shock_size;


	virtual ~Model_Point();
	Model_Point();
	void get_timestamp();
	virtual int get_total_pv_columns_number();
	/** Assignment operator for Model_Point class*/
	Model_Point & operator = (const Model_Point& mp)
	{
		this->index_shock_size = mp.index_shock_size;
		this->index_shock_pvs = new double* [this->index_shock_size];
		for (int i = 0; i < this->index_shock_size; i++){
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = mp.index_shock_pvs[i][j];
		}
	}
	/** Copy constructor of Model_Point class*/
	Model_Point(const Model_Point& mp);	
	Model_Point(vector<string>& row, int rank, Scenario_configuration& scenario_config);	
	Model_Point(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb);
	virtual void initialise_mp_from_inforce(vector<string>& row);
	void set_Calc_information_to_Zero0();
	void initialise_mp0(vector<string>& row);
	virtual void initialise_mp_parameters_by_copy(Model_Point * mp);
	void initialise_mp_parameters_by_copy0(Model_Point * mp);	
	Model_Point(Model_Point * mp);
	virtual void set_pv_index_shock_array(Scenario_configuration& scenario_config);
	
};
