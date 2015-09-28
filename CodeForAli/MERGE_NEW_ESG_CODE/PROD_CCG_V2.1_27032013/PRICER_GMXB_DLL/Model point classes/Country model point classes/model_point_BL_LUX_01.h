#ifndef __Model_Point_LUX_01_H_
#define __Model_Point_LUX_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_LUX_01.h"
#include "../../Projection classes/Country projection classes/head_BL_LUX.h"

using namespace std;

class Model_Point_LUX_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	string Valn_Date_Lux;
	double Fund_fees_Lux;
	string Info_Lux;
	string Prod_Code_Lux;
	double Ehc_Lux;
	int    New_Business_Lux;
	string Pol_Id_Lux;
	string Person_Id_1_Lux;
	string Person_Id_2_Lux;
	double Init_Charge_Lux;
	double Ong_Acqui_Charge_Lux;
	string Account_Code_Lux;
	string Currency_Lux;
	string Gender_1_Lux;
	string Gender_2_Lux;
	string Date_Birth_1_Lux;
	string Date_Birth_2_Lux;
	string Date_Death_1_Lux;
	string Date_Death_2_Lux;
	int    Issue_Age_1_Lux;
	int    Issue_Age_2_Lux;
	int    Tech_Issue_Age_Lux;
	string Issue_Date_Lux;
	string Inv_Date_Lux;
	int    Def_Period_Lux;
	int    Duration_Lux;
	string Annuity_Begin_Date_Lux;
	string Prem_Pay_Mode_Lux;
	double Insur_Prem_Lux;
	double Amount_Invested_Lux;
	string Withdrawals_Mode_Lux;
	double Conversion_Rate_Lux;
	double Rollup_Rate_Lux;
	double Def_Guarantie_Lux;
	double Inv_Protection_Lux;
	double Benefit_Increase_Factor_Lux;
	double Withrawals_Mode_Discount_Lux;
	string Gmdb_Rider_Lux;
	int    Gmdb_Mode_Lux;
	int    Gmdb_Max_Age_Lux;
	int    Gmdb_Duration_Lux;
	double Gmdb_Lux;
	double Curr_Gmwb_Lux;
	double Accum_Withrawal_Amount_Lux;
	string Perf_Bonus_Lux;
	double Perf_Bonus_Level_Lux;
	int    Perf_Bonus_Type_Lux;
	double Add_Accum_Withrawal_Amount_Lux;
	double Perf_Bonus_Amount_Lux;
	double Av_Lux;
	double Base_Capital_Lux;
	double Fund_Units_Lux;
	double Unit_Price_Lux;
	double Reinsurance_Premium_Lux;
	double Fund_Charge_Lux;
	double Upfront_Comm_Lux;
	double Trail_Comm_Lux;
	double AV0;


	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	int      Calc_Valn_Year;
	int      Calc_Valn_Month;
	int      Calc_Valn_Day;
	int      Calc_IssueDate_Year;
	int      Calc_IssueDate_Month;
	int      Calc_IssueDate_Day;
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
	int      Calc_Annuity_Begin_Year;
	int      Calc_Annuity_Begin_Month;
	int      Calc_Annuity_Begin_Day;
	int      Calc_Age_1;
	int      Calc_Age_Month_1;
	int      Calc_Age_2;
	double   Calc_Prorata;
	double   Calc_Fund_Fees_p_m;
	double   Calc_Ehc_p_m;
	double   Calc_fixed_fees_p_m;
	int      Calc_Freq_Pay;
	double   Calc_WB_Rollup;
	double	 Calc_Coupon_by_Freq;
	double	 Calc_Initial_Coupon_by_Freq;
	int      Calc_Duration_Months;
	int      Calc_Remaining_Months;
	int      Calc_Deferal_Duration_Months;
	int      Calc_Mod_Def_Duration_Months;
	double   Calc_BK0;
	double   Calc_RRC_p_m;
	double   Calc_Conversion_Rate_p_m;
	double   Calc_Min_Bonus_Amount;
	int		 Calc_Initial_Deferal_Months;
	double   Calc_Gaur_Charge_Rate;



	virtual ~Model_Point_LUX_01(){}
	
	Model_Point_LUX_01(vector<string>& row, int rank, Product_configuration& product_conf,
							Scenario_configuration& scenario_config):Model_Point()
	{	
		// Initialize the pv table for delta shock
		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the pv table
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->index_shock_pvs[i][j] = 0.;
		}
		// Initialise pv for lapse shocks
		this->lapse_size = scenario_config.get_lapse_mult().size();
		this->pv_lapse = new double* [this->lapse_size];
		
		for (int i = 0; i < this->lapse_size; i++)
		{
			this->pv_lapse[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->pv_lapse[i][j] = 0.;
		}
		// Initialise pv for Election shocks
		this->election_size = scenario_config.get_election_mult().size();
		this->pv_election = new double* [this->election_size];
		
		for (int i = 0; i < this->election_size; i++)
		{
			this->pv_election[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->pv_election[i][j] = 0.;
		}
		// Initialise pv for paidup shocks
		this->paidup_size = scenario_config.get_paidup_mult().size();
		this->pv_paidup = new double* [this->paidup_size];
		for (int i = 0; i < this->paidup_size; i++)
		{
			this->pv_paidup[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->pv_paidup[i][j] = 0.;
		}
		// Initialise pv for mortality shocks
		this->mortality_size = scenario_config.get_mortality_mult().size();
		this->pv_mortality = new double* [this->mortality_size];
		
		for (int i = 0; i < this->mortality_size; i++)
		{
			this->pv_mortality[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->pv_mortality[i][j] = 0.;
		}
		// Initialise pv for longevity
		this->longevity_size = 1;
		this->pv_longevity = new double* [this->longevity_size];
		
		for (int i = 0; i < this->longevity_size; i++)
		{
			this->pv_longevity[i] = new double[TOTAL_PV_OUTPUT_COLUMNS_BL];
			for (int j=0 ; j < TOTAL_PV_OUTPUT_COLUMNS_BL; j++)
				this->pv_longevity[i][j] = 0.;
		}
		this->mp_position_in_file = rank;

		this->initialise_mp_from_inforce(row);
		this->load_longstaff = product_conf.is_longstaff();
		this->load_model_point_funds = product_conf.is_projection_by_fund();
		this->parse_fund_line(row[row.size() - 1], product_conf);
		this->parse_fund_line_stat(row[row.size() - 1]);
	}

	Model_Point_LUX_01(Model_Point_LUX_01 * mp, int index_shock_size_in, int fund_shock_size_in):Model_Point()
	{
		this->get_timestamp();
		this->HE_initialise_mp_from_inforce(mp);
		this->mp_position_in_file = mp->mp_position_in_file;
		this->load_longstaff = 0;
		this->load_model_point_funds = 0;
		UNREFERENCED_PARAMETER(fund_shock_size_in);
		UNREFERENCED_PARAMETER(index_shock_size_in);
	}

	void initialise_mp_from_inforce(vector<string>& row)
	{	
		Valn_Date_Lux=row[VALN_DATE_BL_LUX];
		Fund_fees_Lux= atof(row[FUND_FEES_BL_LUX].c_str());
		Info_Lux=row[INFO_BL_LUX];
		Prod_Code_Lux=row[PROD_CODE_BL_LUX];
		Ehc_Lux= atof(row[EHC_BL_LUX].c_str());
		New_Business_Lux=atoi(row[NEW_BUSINESS_BL_LUX].c_str());
		Pol_Id_Lux=row[POLICY_ID_BL_LUX];
		Person_Id_1_Lux=row[PERSON_ID_1_BL_LUX];
		Person_Id_2_Lux=row[PERSON_ID_2_BL_LUX];
		Init_Charge_Lux= atof(row[INIT_CHARGE_BL_LUX].c_str());
		Ong_Acqui_Charge_Lux= atof(row[ONGOING_ACQUI_CHARGE_BL_LUX].c_str());
		Account_Code_Lux=row[ACCOUNT_CODE_BL_LUX];
		Currency_Lux=row[CURRENCY_BL_LUX];
		Gender_1_Lux=row[GENDER_1_BL_LUX];
		Gender_2_Lux=row[GENDER_2_BL_LUX];
		Date_Birth_1_Lux=row[DATE_BIRTH_1_BL_LUX];
		Date_Birth_2_Lux=row[DATE_BIRTH_2_BL_LUX];
		Date_Death_1_Lux=row[DATE_DEATH_1_BL_LUX];
		Date_Death_2_Lux=row[DATE_DEATH_2_BL_LUX];
		Issue_Age_1_Lux=atoi(row[ISSUE_AGE_1_BL_LUX].c_str());
		Issue_Age_2_Lux=atoi(row[ISSUE_AGE_2_BL_LUX].c_str());
		Tech_Issue_Age_Lux=atoi(row[TECHNICAL_ISSUE_AGE_BL_LUX].c_str());
		Issue_Date_Lux=row[ISSUE_DATE_BL_LUX];
		Inv_Date_Lux=row[INV_DATE_BL_LUX];
		Def_Period_Lux=atoi(row[DEF_PERIOD_BL_LUX].c_str());
		Duration_Lux=atoi(row[DURATION_BL_LUX].c_str());
		Annuity_Begin_Date_Lux=row[ANNUITY_BEGIN_DATE_BL_LUX];
		Prem_Pay_Mode_Lux=row[PREMIUM_PAYMENT_MODE_BL_LUX];	
		Insur_Prem_Lux= atof(row[INSURANCE_PREMIUM_BL_LUX].c_str());
		Amount_Invested_Lux= atof(row[AMOUNT_INVESTED_BL_LUX].c_str());
		Withdrawals_Mode_Lux=row[WITHDRAWAL_MODE_BL_LUX];
		Conversion_Rate_Lux= atof(row[CONVERSION_RATE_BL_LUX].c_str());
		Rollup_Rate_Lux= atof(row[ROLLUP_RATE_BL_LUX].c_str());
		Def_Guarantie_Lux= atof(row[DEF_GUAR_BL_LUX].c_str());
		Inv_Protection_Lux= atof(row[INV_PROTECTION_BL_LUX].c_str());
		Benefit_Increase_Factor_Lux= atof(row[BENEFIT_INCREASE_FACTOR_BL_LUX].c_str());
		Withrawals_Mode_Discount_Lux= atof(row[WITHDRAWAL_MODE_DISCOUNT_BL_LUX].c_str());
		Gmdb_Rider_Lux=row[GMDB_RIDER_BL_LUX];
		Gmdb_Mode_Lux=atoi(row[GMDB_MODE_BL_LUX].c_str());
		Gmdb_Max_Age_Lux=atoi(row[GMDB_MAX_AGE_BL_LUX].c_str());
		Gmdb_Duration_Lux=atoi(row[GMDB_DURATION_BL_LUX].c_str());
		Gmdb_Lux= atof(row[GMDB_LUX].c_str());
		Curr_Gmwb_Lux= atof(row[CURR_GMWB_BL_LUX].c_str());
		Accum_Withrawal_Amount_Lux= atof(row[ACCUM_WITHDRAWAL_AMOUNT_BL_LUX].c_str());
		Perf_Bonus_Lux=row[PERF_BONUS_BL_LUX];
		Perf_Bonus_Level_Lux= atof(row[PERF_BONUS_LEVEL_BL_LUX].c_str());
		Perf_Bonus_Type_Lux=atoi(row[PERF_BONUS_TYPE_BL_LUX].c_str());
		Add_Accum_Withrawal_Amount_Lux= atof(row[ADD_ACCUM_WITHDRAWAL_AMOUNT_BL_LUX].c_str());
		Perf_Bonus_Amount_Lux= atof(row[PERF_BONUS_AMOUNT_BL_LUX].c_str());
		Av_Lux= atof(row[ACCOUNT_VALUE_BL_LUX].c_str());
		Base_Capital_Lux= atof(row[BASE_CAPITAL_BL_LUX].c_str());
		Fund_Units_Lux= atof(row[FUND_UNITS_BL_LUX].c_str());
		Unit_Price_Lux= atof(row[UNIT_PRICE_BL_LUX].c_str());
		Reinsurance_Premium_Lux= atof(row[REINSURANCE_PREMIUM_BL_LUX].c_str());
		Fund_Charge_Lux= atof(row[FUND_CHARGE_BL_LUX].c_str());
		Upfront_Comm_Lux= atof(row[UPFRONT_COMM_BL_LUX].c_str());
		Trail_Comm_Lux= atof(row[TRAIL_COMM_BL_LUX].c_str());
		AV0=Av_Lux;
		prod_code_key=Prod_Code_Lux;
		policy_id=Pol_Id_Lux;
		
		av_if_array[0] = atof(row[ACCOUNT_VALUE_BL_LUX].c_str());
		av_if_array[1] = 0.;
		av_if_array[2] = 0.;
		av_if_array[3] = 0.;
		av_if_array[4] = 0.;
		av_if_array[5] = 0.;
		av_if_array[6] = 0.;
		av_if_array[7] = 0.;
		av_if_array[8] = 0.;
		av_if_array[9] = 0.;
		av_if_array[10] = 0.;
		av_if_array[11] = 0.;
		av_split_prop_array[0] = 100.;
		av_split_prop_array[1] = 0.;
		av_split_prop_array[2] = 0.;
		av_split_prop_array[3] = 0.;
		av_split_prop_array[4] = 0.;
		av_split_prop_array[5] = 0.;
		av_split_prop_array[6] = 0.;
		av_split_prop_array[7] = 0.;
		av_split_prop_array[8] = 0.;
		av_split_prop_array[9] = 0.;
		av_split_prop_array[10] = 0.;
		av_split_prop_array[11] = 0.;
		
		set_Calc_information_to_Zero();

	}
	void HE_initialise_mp_from_inforce(Model_Point * m_p)
	{
		Model_Point_LUX_01 * mp = dynamic_cast<Model_Point_LUX_01*>(m_p);

		Valn_Date_Lux  = mp->Valn_Date_Lux;
		Fund_fees_Lux  = mp->Fund_fees_Lux;
		Info_Lux  = mp->Info_Lux;
		Prod_Code_Lux  = mp->Prod_Code_Lux;
		Ehc_Lux  = mp->Ehc_Lux;
		New_Business_Lux  = mp->New_Business_Lux;
		Pol_Id_Lux  = mp->Pol_Id_Lux;
		Person_Id_1_Lux  = mp->Person_Id_1_Lux;
		Person_Id_2_Lux  = mp->Person_Id_2_Lux;
		Init_Charge_Lux  = mp->Init_Charge_Lux;
		Ong_Acqui_Charge_Lux  = mp->Ong_Acqui_Charge_Lux;
		Account_Code_Lux  = mp->Account_Code_Lux;
		Currency_Lux  = mp->Currency_Lux;
		Gender_1_Lux  = mp->Gender_1_Lux;
		Gender_2_Lux  = mp->Gender_2_Lux;
		Date_Birth_1_Lux  = mp->Date_Birth_1_Lux;
		Date_Birth_2_Lux  = mp->Date_Birth_2_Lux;
		Date_Death_1_Lux  = mp->Date_Death_1_Lux;
		Date_Death_2_Lux  = mp->Date_Death_2_Lux;
		Issue_Age_1_Lux  = mp->Issue_Age_1_Lux;
		Issue_Age_2_Lux  = mp->Issue_Age_2_Lux;
		Tech_Issue_Age_Lux  = mp->Tech_Issue_Age_Lux;
		Issue_Date_Lux  = mp->Issue_Date_Lux;
		Inv_Date_Lux  = mp->Inv_Date_Lux;
		Def_Period_Lux  = mp->Def_Period_Lux;
		Duration_Lux  = mp->Duration_Lux;
		Annuity_Begin_Date_Lux  = mp->Annuity_Begin_Date_Lux;
		Prem_Pay_Mode_Lux  = mp->Prem_Pay_Mode_Lux;
		Insur_Prem_Lux  = mp->Insur_Prem_Lux;
		Amount_Invested_Lux  = mp->Amount_Invested_Lux;
		Withdrawals_Mode_Lux  = mp->Withdrawals_Mode_Lux;
		Conversion_Rate_Lux  = mp->Conversion_Rate_Lux;
		Rollup_Rate_Lux  = mp->Rollup_Rate_Lux;
		Def_Guarantie_Lux  = mp->Def_Guarantie_Lux;
		Inv_Protection_Lux  = mp->Inv_Protection_Lux;
		Benefit_Increase_Factor_Lux  = mp->Benefit_Increase_Factor_Lux;
		Withrawals_Mode_Discount_Lux  = mp->Withrawals_Mode_Discount_Lux;
		Gmdb_Rider_Lux  = mp->Gmdb_Rider_Lux;
		Gmdb_Mode_Lux  = mp->Gmdb_Mode_Lux;
		Gmdb_Max_Age_Lux  = mp->Gmdb_Max_Age_Lux;
		Gmdb_Duration_Lux  = mp->Gmdb_Duration_Lux;
		Gmdb_Lux  = mp->Gmdb_Lux;
		Curr_Gmwb_Lux  = mp->Curr_Gmwb_Lux;
		Accum_Withrawal_Amount_Lux  = mp->Accum_Withrawal_Amount_Lux;
		Perf_Bonus_Lux  = mp->Perf_Bonus_Lux;
		Perf_Bonus_Level_Lux  = mp->Perf_Bonus_Level_Lux;
		Perf_Bonus_Type_Lux  = mp->Perf_Bonus_Type_Lux;
		Add_Accum_Withrawal_Amount_Lux  = mp->Add_Accum_Withrawal_Amount_Lux;
		Perf_Bonus_Amount_Lux  = mp->Perf_Bonus_Amount_Lux;
		Av_Lux  = mp->Av_Lux;
		Base_Capital_Lux  = mp->Base_Capital_Lux;
		Fund_Units_Lux  = mp->Fund_Units_Lux;
		Unit_Price_Lux  = mp->Unit_Price_Lux;
		Reinsurance_Premium_Lux  = mp->Reinsurance_Premium_Lux;
		Fund_Charge_Lux  = mp->Fund_Charge_Lux;
		Upfront_Comm_Lux  = mp->Upfront_Comm_Lux;
		Trail_Comm_Lux  = mp->Trail_Comm_Lux;
		AV0  = mp->AV0;
		prod_code_key= mp->prod_code_key;
		policy_id=mp->policy_id;

		
		av_if_array[0]      = mp->av_if_array[0];
		av_if_array[1]      = mp->av_if_array[1];
		av_if_array[2]      = mp->av_if_array[2];
		av_if_array[3]      = mp->av_if_array[3];
		av_if_array[4]      = mp->av_if_array[4];
		av_if_array[5]      = mp->av_if_array[5];
		av_if_array[6]      = mp->av_if_array[6];
		av_if_array[7]      = mp->av_if_array[7];
		av_if_array[8]      = mp->av_if_array[8];
		av_if_array[9]      = mp->av_if_array[9];
		av_if_array[10]     = mp->av_if_array[10];
		av_if_array[11]     = mp->av_if_array[11];
        av_split_prop_array[0] = mp->av_split_prop_array[0];
		av_split_prop_array[1] = mp->av_split_prop_array[1];
		av_split_prop_array[2] = mp->av_split_prop_array[2];
		av_split_prop_array[3] = mp->av_split_prop_array[3];
		av_split_prop_array[4] = mp->av_split_prop_array[4];
		av_split_prop_array[5] = mp->av_split_prop_array[5];
		av_split_prop_array[6] = mp->av_split_prop_array[6];
		av_split_prop_array[7] = mp->av_split_prop_array[7];
		av_split_prop_array[8] = mp->av_split_prop_array[8];
		av_split_prop_array[9] = mp->av_split_prop_array[9];
		av_split_prop_array[10] = mp->av_split_prop_array[10];
		av_split_prop_array[11] = mp->av_split_prop_array[11];

		set_Calc_information_to_Zero();

	}
	void set_Calc_information_to_Zero()
	{
		Calc_Valn_Year=0;
		Calc_Valn_Month=0;
		Calc_Valn_Day=0;
		Calc_IssueDate_Year=0;
		Calc_IssueDate_Month=0;
		Calc_IssueDate_Day=0;
		Calc_Birth_1_Date_Year=0;
		Calc_Birth_1_Date_Month=0;
		Calc_Birth_1_Date_Day=0;	
		Calc_Birth_2_Date_Year=0;
		Calc_Birth_2_Date_Month=0;
		Calc_Birth_2_Date_Day=0;	
		Calc_Death_1_Date_Year=0;
		Calc_Death_1_Date_Month=0;
		Calc_Death_1_Date_Day=0;	
		Calc_Death_2_Date_Year=0;
		Calc_Death_2_Date_Month=0;
		Calc_Death_2_Date_Day=0;	
		Calc_Annuity_Begin_Year=0;
		Calc_Annuity_Begin_Month=0;
		Calc_Annuity_Begin_Day=0;
		Calc_Age_1=0;
		Calc_Age_Month_1=0;
		Calc_Age_2=0;
		Calc_Prorata=0.;
		Calc_Fund_Fees_p_m=0.;
		Calc_Ehc_p_m=0.;
		Calc_fixed_fees_p_m=0.;
		Calc_Freq_Pay=0;
		Calc_WB_Rollup=0.;
		Calc_Coupon_by_Freq=0.;
		Calc_Duration_Months=0;
		Calc_Remaining_Months=0;
		Calc_Deferal_Duration_Months=0;
		Calc_Initial_Coupon_by_Freq=0.;
		Calc_Mod_Def_Duration_Months=0;
		Calc_Conversion_Rate_p_m=0.;
		Calc_Min_Bonus_Amount=0.;
		Calc_Initial_Deferal_Months=0;
		Calc_Gaur_Charge_Rate=0.;
		
	}
	int get_total_pv_columns_number()
	{
		return TOTAL_PV_OUTPUT_COLUMNS_BL;
	}
};

#endif
