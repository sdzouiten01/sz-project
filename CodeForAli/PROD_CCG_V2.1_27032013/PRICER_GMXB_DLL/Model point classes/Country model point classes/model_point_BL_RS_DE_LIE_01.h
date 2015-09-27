#ifndef __Model_Point_BL_RS_DE_LIE_01_H_
#define __Model_Point_BL_RS_DE_LIE_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_RS_DE_LIE_01.h"
#include "../../Projection classes/Country projection classes/head_BL_CH.h"

using namespace std;

class Model_Point_BL_RS_DE_LIE_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	
	string Valn_Date_Bl;
	double Fund_fees_Bl;
	//string Info_Bl;
	double Info_Bl;
	string Prod_Code_Bl;
	double Ehc_Bl;
	int    New_Business_Bl;
	string Pol_Id_Bl;
	string Person_Id_1_Bl;
	string Person_Id_2_Bl;
	double Init_Charge_Bl;
	string Account_Code_Bl;
	string Currency_Bl;
	string Gender_1_Bl;
	string Gender_2_Bl;
	string Date_Birth_1_Bl;
	string Date_Birth_2_Bl;
	string Date_Death_1_Bl;
	string Date_Death_2_Bl;
	int    Issue_Age_1_Bl;
	int    Issue_Age_2_Bl;
	string Issue_Date_Bl;
	string NB_Date_Bl;
	string Inv_Date_Bl;
	int    Def_Period_Bl;
	//int    Duration_Bl;	//not in inforce, should be declared in calc!!! Is already declared!
	string Annuity_Begin_Date_Bl;
	string Prem_Pay_Mode_Bl;
	double Insur_Prem_Bl;
	double Amount_Invested_Bl;
	string Withdrawals_Mode_Bl;
	double Conversion_Rate_Bl;
	double Def_Guarantie_Bl;
	double Benefit_Increase_Factor_Bl;
	double Withrawals_Mode_Discount_Bl;
	//string Gmdb_Rider_Bl;
	double Gmdb_Bl;
	double Curr_Gmwb_Bl;
	double Accum_Withrawal_Amount_Bl;
	double Add_Accum_Withrawal_Amount_Bl;
	double Perf_Bonus_Amount_Bl;
	double Av_Bef_Bef_Bl;
	double Av_After_Bef_Bl;
	double Av_After_After_Bl;
	double Base_Capital_Bl;
	double Fund_Unit_Bef_Bef_Bl;
	double Fund_Unit_After_Bef_Bl;
	double Fund_Unit_After_After_Bl;
	double Unit_Price_Bl;
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
	double   Calc_WB_Initial;			//still needed?
	double	 Calc_Coupon_by_Freq;
	double	 Calc_Initial_Coupon_by_Freq;
	int		 Calc_Duration_Bl;
	int      Calc_Duration_Months;		//BG: directly in inforce before
	int      Calc_Remaining_Months;
	int      Calc_Deferal_Duration_Months;
	int      Calc_Mod_Def_Duration_Months;
	double   Calc_BK0;
	double   Calc_RRC_p_m;
	double   Calc_Conversion_Rate_p_m;
	double   Calc_Max_Bonus_Amount;		//max amount?
	int		 Calc_Initial_Deferal_Months;
	double   Calc_Gaur_Charge_Rate;
	double	 Calc_Perf_Bonus_Amount_Bl;	//BG input



	virtual ~Model_Point_BL_RS_DE_LIE_01(){}
	
	Model_Point_BL_RS_DE_LIE_01(vector<string>& row, int rank, Product_configuration& product_conf,
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

	Model_Point_BL_RS_DE_LIE_01(Model_Point_BL_RS_DE_LIE_01 * mp, int index_shock_size_in, int fund_shock_size_in):Model_Point()
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
		Valn_Date_Bl=row[VALN_DATE_LIE_RS_DE];	//BG: mapped to model_point_head_BL_RS_CH_LIE_01.h number?
		Fund_fees_Bl= atof(row[FUND_FEES_LIE_RS_DE].c_str());
		Info_Bl=atof(row[INFO_LIE_RS_DE].c_str());
		Prod_Code_Bl=row[PROD_CODE_LIE_RS_DE];
		Ehc_Bl= atof(row[EHC_LIE_RS_DE].c_str());
		New_Business_Bl=atoi(row[NEW_BUSINESS_LIE_RS_DE].c_str());
		Pol_Id_Bl=row[POLICY_ID_LIE_RS_DE];
		Person_Id_1_Bl=row[PERSON_ID_1_LIE_RS_DE];
		Person_Id_2_Bl=row[PERSON_ID_2_LIE_RS_DE];
		Init_Charge_Bl= atof(row[INIT_CHARGE_LIE_RS_DE].c_str());
		//Ong_Charge_1_Bl=atof(row[ONGOING_CHARGE_1_LIE_RS_CHF].c_str());	//new
		//Ong_Charge_2_Bl=atof(row[ONGOING_CHARGE_2_LIE_RS_CHF].c_str());	//new
		//Ong_Acqui_Charge_Lux= atof(row[ONGOING_ACQUI_CHARGE_LIE_RS_CHF].c_str());
		Account_Code_Bl=row[ACCOUNT_CODE_LIE_RS_DE];
		Currency_Bl=row[CURRENCY_LIE_RS_DE];
		Gender_1_Bl=row[GENDER_1_LIE_RS_DE];
		Gender_2_Bl=row[GENDER_2_LIE_RS_DE];
		Date_Birth_1_Bl=row[DATE_BIRTH_1_LIE_RS_DE];
		Date_Birth_2_Bl=row[DATE_BIRTH_2_LIE_RS_DE];
		Date_Death_1_Bl=row[DATE_DEATH_1_LIE_RS_DE];
		Date_Death_2_Bl=row[DATE_DEATH_2_LIE_RS_DE];
		Issue_Age_1_Bl=atoi(row[ISSUE_AGE_1_LIE_RS_DE].c_str());
		Issue_Age_2_Bl=atoi(row[ISSUE_AGE_2_LIE_RS_DE].c_str());
		//Tech_Issue_Age_Lux=atoi(row[TECHNICAL_ISSUE_AGE_LIE_RS_CHF].c_str());
		Issue_Date_Bl=row[ISSUE_DATE_LIE_RS_DE];
		NB_Date_Bl=row[NB_DATE_LIE_RS_DE];
		Inv_Date_Bl=row[INV_DATE_LIE_RS_DE];
		Def_Period_Bl=atoi(row[DEF_PERIOD_LIE_RS_DE].c_str());
		//Duration_Bl=atoi(row[DURATION_LIE_RS_CHF].c_str());
		Annuity_Begin_Date_Bl=row[ANNUITY_BEGIN_DATE_LIE_RS_DE];
		Prem_Pay_Mode_Bl=row[PREMIUM_PAYMENT_MODE_LIE_RS_DE];	
		Insur_Prem_Bl= atof(row[INSURANCE_PREMIUM_LIE_RS_DE].c_str());
		Amount_Invested_Bl= atof(row[AMOUNT_INVESTED_LIE_RS_DE].c_str());
		Withdrawals_Mode_Bl=row[WITHDRAWAL_MODE_LIE_RS_DE];
		Conversion_Rate_Bl= atof(row[CONVERSION_RATE_LIE_RS_DE].c_str());
		//Rollup_Rate_Bl= atof(row[ROLLUP_RATE_LIE_RS_CHF].c_str());
		Def_Guarantie_Bl= atof(row[DEF_GUAR_LIE_RS_DE].c_str());
		//Inv_Protection_Bl= atof(row[INV_PROTECTION_LIE_RS_CHF].c_str());
		Benefit_Increase_Factor_Bl= atof(row[BENEFIT_INCREASE_FACTOR_LIE_RS_DE].c_str());
		Withrawals_Mode_Discount_Bl= atof(row[WITHDRAWAL_MODE_DISCOUNT_LIE_RS_DE].c_str());
		//Gmdb_Rider_Lux=row[GMDB_RIDER_LIE_RS_CHF];
		//Gmdb_Mode_Lux=atoi(row[GMDB_MODE_LIE_RS_CHF].c_str());
		//Gmdb_Max_Age_Lux=atoi(row[GMDB_MAX_AGE_LIE_RS_CHF].c_str());
		//Gmdb_Duration_Lux=atoi(row[GMDB_DURATION_LIE_RS_CHF].c_str());
		Gmdb_Bl= atof(row[GMDB_LIE_RS_DE].c_str());
		Curr_Gmwb_Bl= atof(row[CURR_GMWB_LIE_RS_DE].c_str());
		Accum_Withrawal_Amount_Bl= atof(row[ACCUM_WITHDRAWAL_AMOUNT_LIE_RS_DE].c_str());
		Add_Accum_Withrawal_Amount_Bl= atof(row[ADD_ACCUM_WITHDRAWAL_AMOUNT_LIE_RS_DE].c_str());
		Av_Bef_Bef_Bl= atof(row[ACC_VAL_BEF_BEF_LIE_RS_DE].c_str());
		Av_After_Bef_Bl= atof(row[ACC_VAL_AFT_BEF_LIE_RS_DE].c_str());
		Av_After_After_Bl= atof(row[ACC_VAL_AFT_AFT_LIE_RS_DE].c_str());
		Base_Capital_Bl= atof(row[BASE_CAPITAL_LIE_RS_DE].c_str());
		Fund_Unit_Bef_Bef_Bl= atof(row[FUND_UNITS_BEF_BEF_LIE_RS_DE].c_str());
		Fund_Unit_After_Bef_Bl= atof(row[FUND_UNITS_AFT_BEF_LIE_RS_DE].c_str());
		Fund_Unit_After_After_Bl= atof(row[FUND_UNITS_AFT_AFT_LIE_RS_DE].c_str());
		Unit_Price_Bl= atof(row[UNIT_PRICE_LIE_RS_DE].c_str());
		AV0=Av_After_After_Bl;	
		prod_code_key=Prod_Code_Bl;
		policy_id=Pol_Id_Bl;		

		av_if_array[0] = atof(row[ACC_VAL_AFT_AFT_LIE_RS_DE].c_str());
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
		Model_Point_BL_RS_DE_LIE_01 * mp = dynamic_cast<Model_Point_BL_RS_DE_LIE_01*>(m_p);

		Valn_Date_Bl  = mp->Valn_Date_Bl;
		Fund_fees_Bl  = mp->Fund_fees_Bl;
		Info_Bl  = mp->Info_Bl;
		Prod_Code_Bl  = mp->Prod_Code_Bl;
		Ehc_Bl  = mp->Ehc_Bl;
		New_Business_Bl  = mp->New_Business_Bl;
		Pol_Id_Bl  = mp->Pol_Id_Bl;
		Person_Id_1_Bl  = mp->Person_Id_1_Bl;
		Person_Id_2_Bl  = mp->Person_Id_2_Bl;
		Init_Charge_Bl  = mp->Init_Charge_Bl;
		Account_Code_Bl  = mp->Account_Code_Bl;
		Currency_Bl  = mp->Currency_Bl;
		Gender_1_Bl  = mp->Gender_1_Bl;
		Gender_2_Bl  = mp->Gender_2_Bl;
		Date_Birth_1_Bl  = mp->Date_Birth_1_Bl;
		Date_Birth_2_Bl  = mp->Date_Birth_2_Bl;
		Date_Death_1_Bl  = mp->Date_Death_1_Bl;
		Date_Death_2_Bl  = mp->Date_Death_2_Bl;
		Issue_Age_1_Bl  = mp->Issue_Age_1_Bl;
		Issue_Age_2_Bl  = mp->Issue_Age_2_Bl;
		Issue_Date_Bl  = mp->Issue_Date_Bl;
		NB_Date_Bl=mp->NB_Date_Bl;
		Inv_Date_Bl  = mp->Inv_Date_Bl;
		Def_Period_Bl  = mp->Def_Period_Bl;
		//Duration_Bl  = mp->Duration_Bl;
		Annuity_Begin_Date_Bl  = mp->Annuity_Begin_Date_Bl;
		Prem_Pay_Mode_Bl  = mp->Prem_Pay_Mode_Bl;
		Insur_Prem_Bl  = mp->Insur_Prem_Bl;
		Amount_Invested_Bl  = mp->Amount_Invested_Bl;
		Withdrawals_Mode_Bl  = mp->Withdrawals_Mode_Bl;
		Conversion_Rate_Bl  = mp->Conversion_Rate_Bl;
		Def_Guarantie_Bl  = mp->Def_Guarantie_Bl;
		Benefit_Increase_Factor_Bl  = mp->Benefit_Increase_Factor_Bl;
		Withrawals_Mode_Discount_Bl  = mp->Withrawals_Mode_Discount_Bl;
		Gmdb_Bl  = mp->Gmdb_Bl;
		Curr_Gmwb_Bl  = mp->Curr_Gmwb_Bl;
		Accum_Withrawal_Amount_Bl  = mp->Accum_Withrawal_Amount_Bl;
		Add_Accum_Withrawal_Amount_Bl  = mp->Add_Accum_Withrawal_Amount_Bl;
		Av_Bef_Bef_Bl= mp->Av_Bef_Bef_Bl;
		Av_After_Bef_Bl= mp->Av_After_Bef_Bl;
		Av_After_After_Bl= mp->Av_After_After_Bl;
		Base_Capital_Bl= mp->Base_Capital_Bl;
		Fund_Unit_Bef_Bef_Bl= mp->Fund_Unit_Bef_Bef_Bl;
		Fund_Unit_After_Bef_Bl= mp->Fund_Unit_After_Bef_Bl;
		Fund_Unit_After_After_Bl= mp->Fund_Unit_After_After_Bl;
		Unit_Price_Bl= mp->Unit_Price_Bl;
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
		Calc_WB_Initial=0.;
		Calc_Coupon_by_Freq=0.;
		Calc_Duration_Months=0;
		Calc_Remaining_Months=0;
		Calc_Deferal_Duration_Months=0;
		Calc_Initial_Coupon_by_Freq=0.;
		Calc_Mod_Def_Duration_Months=0;
		Calc_Conversion_Rate_p_m=0.;
		Calc_Max_Bonus_Amount=0.;
		Calc_Initial_Deferal_Months=0;
		Calc_Gaur_Charge_Rate=0.;
		Calc_Perf_Bonus_Amount_Bl=0.;
		
	}
	int get_total_pv_columns_number()
	{
		return TOTAL_PV_OUTPUT_COLUMNS_BL;
	}
};

#endif
