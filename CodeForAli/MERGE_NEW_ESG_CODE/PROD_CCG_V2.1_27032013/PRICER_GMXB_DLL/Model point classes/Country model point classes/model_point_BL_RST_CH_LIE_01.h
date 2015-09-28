#ifndef __Model_Point_BL_RST_CH_LIE_01_H_
#define __Model_Point_BL_RST_CH_LIE_01_H_

#include "../Model_Point.h"
#include "model_point_head_BL_RST_CH_LIE_01.h"
#include "../../Projection classes/Country projection classes/head_BL_CH.h"

using namespace std;

class Model_Point_BL_RST_CH_LIE_01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	string   Prod_Code_Bl;
	string   Valn_Date_Bl;
	double   Fund_Fees_Bl;
	double   New_Re_Reinsurance_premium_Bl;
	double   Ehc_Bl;
	int      New_Business_Bl;
	string   Pol_Id_Bl;
	string   Prod_Name_Bl;
	double   Init_Charge_Bl;
	string   Fund_Name_Bl;
	string   Currency_Bl;
	string   Issue_Date_Bl;
	string   Nb_Date_Bl;
	string   Inv_Date_Bl;
	int      Def_Period_Bl;
	int      Duration_Bl;
	string   Annuity_Begin_Bl;
	string   Prod_Prem_Type_Bl;
	double   Premium_Bl;
	double   Amount_Inv_Bl;
	string   Freq_Mode_Bl;
	double   Withdrawal_Rate_Bl;
	double   Inv_Protection_Bl;
	double   Benefit_Increase_Factor_Bl;
	double   Withdrawal_Mode_discount_Bl;
	double   Guarantee_Bl;
	double   Accum_Withdrawal_Amount_Bl;
	double   Perf_Bonus_Amount_Bl;
	double   Av_bef_bef_bl;
	double   Av_After_Bef_Bl;
	double   Av_After_After_Bl;
	double   Base_Capital_Bl;
	double   Fund_Unit_Bef_Bef_Bl;
	double   Fund_Unit_After_Bef_Bl;
	double   Fund_Unit_After_After_Bl;
	double   AV0;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	int      Calc_Valn_Year;
	int      Calc_Valn_Month;
	int      Calc_Valn_Day;
	int      Calc_IssueDate_Year;
	int      Calc_IssueDate_Month;
	int      Calc_IssueDate_Day;
	int      Calc_Nb_Date_Year;
	int      Calc_Nb_Date_Month;
	int      Calc_Nb_Date_Day;
	int      Calc_Inv_Date_Year;
	int      Calc_Inv_Date_Month;
	int      Calc_Inv_Date_Day;
	int      Calc_Annuity_Begin_Year;
	int      Calc_Annuity_Begin_Month;
	int      Calc_Annuity_Begin_Day;
	int      Calc_Remaining_Months;
	int      Calc_Duration_Months;
	int      Calc_Deferal_Duration_Months;
	double   Calc_Prorata;
	double   Calc_Fund_Fees_p_m;
	double   Calc_Ehc_p_m;
	int      Calc_Freq_Pay;
	double	 Calc_Coupon_by_Freq;
	int      Calc_Mod_Def_Duration_Months;
	int      Calc_NB_Coupons;
	double   Calc_Perf_By_Freq;
	double   Calc_Max_Bonus_Amount_By_Freq;
	double   Calc_Base_Capital_Bl;
	double	 Calc_Perf_Bonus_Amount_Bl;

	virtual ~Model_Point_BL_RST_CH_LIE_01(){}
	
	Model_Point_BL_RST_CH_LIE_01(vector<string>& row, int rank, Product_configuration& product_conf,
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

	Model_Point_BL_RST_CH_LIE_01(Model_Point_BL_RST_CH_LIE_01 * mp, int index_shock_size_in, int fund_shock_size_in):Model_Point()
	{
		this->get_timestamp();
		this->HE_initialise_mp_from_inforce(mp);
		this->mp_position_in_file = mp->mp_position_in_file;
		this->load_longstaff = 0;
		this->load_model_point_funds = 0;
		UNREFERENCED_PARAMETER(index_shock_size_in);
		UNREFERENCED_PARAMETER(fund_shock_size_in);
	}

	void initialise_mp_from_inforce(vector<string>& row)
	{
		Prod_Code_Bl=row[PROD_CODE_BL_LIE_RST_CH];
		Valn_Date_Bl=row[VALN_DATE_BL_LIE_RST_CH];
		Fund_Fees_Bl= atof(row[FUND_FEES_BL_LIE_RST_CH].c_str());
		Ehc_Bl= atof(row[EHC_BL_LIE_RST_CH].c_str());
		New_Business_Bl=(atoi(row[NEW_BUSINESS_BL_LIE_RST_CH].c_str())==1);
		Pol_Id_Bl=row[POL_ID_BL_LIE_RST_CH];
		Prod_Name_Bl=row[PROD_NAME_BL_LIE_RST_CH];
		Init_Charge_Bl= atof(row[INIT_CHARGE_BL_LIE_RST_CH].c_str());
		Fund_Name_Bl=row[FUND_NAME_BL_LIE_RST_CH];
		Currency_Bl=row[CURRENCY_BL_LIE_RST_CH];
		Issue_Date_Bl=row[ISSUE_DATE_BL_LIE_RST_CH];
		Nb_Date_Bl=row[NB_DATE_BL_LIE_RST_CH];
		Inv_Date_Bl=row[INV_DATE_BL_LIE_RST_CH];
		Def_Period_Bl=atoi(row[DEF_PERIOD_BL_LIE_RST_CH].c_str());
		Duration_Bl=atoi(row[DURATION_BL_LIE_RST_CH].c_str());
		Annuity_Begin_Bl=row[ANNUITY_BEGIN_BL_LIE_RST_CH];
		Prod_Prem_Type_Bl=row[PROD_PREM_TYPE_BL_LIE_RST_CH];
		Premium_Bl= atof(row[PREMIUM_BL_LIE_RST_CH].c_str());
		Amount_Inv_Bl= atof(row[AMOUNT_INV_BL_LIE_RST_CH].c_str());
		Freq_Mode_Bl=row[FREQ_MODE_BL_LIE_RST_CH];
		Withdrawal_Rate_Bl= atof(row[WITHDRAWAL_RATE_BL_LIE_RST_CH].c_str());
		Inv_Protection_Bl= atof(row[INV_PROTECTION_BL_LIE_RST_CH].c_str());
		Benefit_Increase_Factor_Bl= atof(row[BENEFIT_INCREASE_FACTOR_BL_LIE_RST_CH].c_str());
		Withdrawal_Mode_discount_Bl= atof(row[WITHDRAWAL_MODE_DISCOUNT_BL_LIE_RST_CH].c_str());
		Guarantee_Bl= atof(row[GUARANTEE_BL_LIE_RST_CH].c_str());
		Accum_Withdrawal_Amount_Bl= atof(row[ACCUM_WITHDRAWAL_AMOUNT_BL_LIE_RST_CH].c_str());
		Perf_Bonus_Amount_Bl= atof(row[PERF_BONUS_AMOUNT_BL_LIE_RST_CH].c_str());
		Av_bef_bef_bl= atof(row[AV_BEF_BEF_BL_LIE_RST_CH].c_str());
		Av_After_Bef_Bl= atof(row[AV_AFTER_BEF_BL_LIE_RST_CH].c_str());
		Av_After_After_Bl= atof(row[AV_AFTER_AFTER_BL_LIE_RST_CH].c_str());
		Base_Capital_Bl= atof(row[BASE_CAPITAL_BL_LIE_RST_CH].c_str());
		Fund_Unit_Bef_Bef_Bl= atof(row[FUND_UNIT_BEF_BEF_BL_LIE_RST_CH].c_str());
		Fund_Unit_After_Bef_Bl= atof(row[FUND_UNIT_AFTER_BEF_BL_LIE_RST_CH].c_str());
		Fund_Unit_After_After_Bl= atof(row[FUND_UNIT_AFTER_AFTER_BL_LIE_RST_CH].c_str());
		AV0=Av_After_After_Bl;
		prod_code_key=Prod_Code_Bl;
		New_Re_Reinsurance_premium_Bl=atof(row[INFO_LIE_RST_CH].c_str());
		policy_id=Pol_Id_Bl;
		
		av_if_array[0] = atof(row[AV_AFTER_AFTER_BL_LIE_RST_CH].c_str());
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
		Model_Point_BL_RST_CH_LIE_01 * mp = dynamic_cast<Model_Point_BL_RST_CH_LIE_01*>(m_p);

		Prod_Code_Bl  = mp->Prod_Code_Bl;
		Valn_Date_Bl  = mp->Valn_Date_Bl;
		Fund_Fees_Bl  = mp->Fund_Fees_Bl;
		Ehc_Bl  = mp->Ehc_Bl;
		New_Business_Bl  = mp->New_Business_Bl;
		Pol_Id_Bl  = mp->Pol_Id_Bl;
		Prod_Name_Bl  = mp->Prod_Name_Bl;
		Init_Charge_Bl  = mp->Init_Charge_Bl;
		Fund_Name_Bl  = mp->Fund_Name_Bl;
		Currency_Bl  = mp->Currency_Bl;
		Issue_Date_Bl  = mp->Issue_Date_Bl;
		Nb_Date_Bl  = mp->Nb_Date_Bl;
		Inv_Date_Bl=mp->Inv_Date_Bl;
		Def_Period_Bl  = mp->Def_Period_Bl;
		Duration_Bl  = mp->Duration_Bl;
		Annuity_Begin_Bl=mp->Annuity_Begin_Bl;
		Prod_Prem_Type_Bl  = mp->Prod_Prem_Type_Bl;
		Premium_Bl  = mp->Premium_Bl;
		Amount_Inv_Bl  = mp->Amount_Inv_Bl;
		Freq_Mode_Bl  = mp->Freq_Mode_Bl;
		Withdrawal_Rate_Bl  = mp->Withdrawal_Rate_Bl;
		Inv_Protection_Bl  = mp->Inv_Protection_Bl;
		Benefit_Increase_Factor_Bl  = mp->Benefit_Increase_Factor_Bl;
		Withdrawal_Mode_discount_Bl= mp->Withdrawal_Mode_discount_Bl; 
		Guarantee_Bl  = mp->Guarantee_Bl;
		Accum_Withdrawal_Amount_Bl  = mp->Accum_Withdrawal_Amount_Bl;
		Perf_Bonus_Amount_Bl  = mp->Perf_Bonus_Amount_Bl;
		Av_bef_bef_bl  = mp->Av_bef_bef_bl;
		Av_After_Bef_Bl  = mp->Av_After_Bef_Bl;
		Av_After_After_Bl  = mp->Av_After_After_Bl;
		Base_Capital_Bl  = mp->Base_Capital_Bl;
		Fund_Unit_Bef_Bef_Bl  = mp->Fund_Unit_Bef_Bef_Bl;
		Fund_Unit_After_Bef_Bl  = mp->Fund_Unit_After_Bef_Bl;
		Fund_Unit_After_After_Bl  = mp->Fund_Unit_After_After_Bl;
		AV0  = mp->AV0;
		prod_code_key= mp->prod_code_key;
		New_Re_Reinsurance_premium_Bl=mp->New_Re_Reinsurance_premium_Bl;
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
		Calc_Valn_Year =0;
		Calc_Valn_Month =0;
		Calc_Valn_Day =0;
		Calc_IssueDate_Year =0;
		Calc_IssueDate_Month =0;
		Calc_IssueDate_Day =0;
		Calc_Nb_Date_Year =0;
		Calc_Nb_Date_Month =0;
		Calc_Nb_Date_Day =0;
		Calc_Inv_Date_Year =0;
		Calc_Inv_Date_Month =0;
		Calc_Inv_Date_Day =0;
		Calc_Annuity_Begin_Year =0;
		Calc_Annuity_Begin_Month =0;
		Calc_Annuity_Begin_Day =0;
		Calc_Remaining_Months =0;
		Calc_Duration_Months =0;
		Calc_Deferal_Duration_Months =0;
		Calc_Prorata =0;
		Calc_Fund_Fees_p_m =0.;
		Calc_Ehc_p_m =0.;
		Calc_Freq_Pay =0;
		Calc_Coupon_by_Freq =0.;
		Calc_Mod_Def_Duration_Months =0;
		Calc_NB_Coupons =0;
		Calc_Perf_By_Freq =0.;
		Calc_Max_Bonus_Amount_By_Freq =0.;
		Calc_Base_Capital_Bl=0.;
		Calc_Perf_Bonus_Amount_Bl=0.;
		
	}
	int get_total_pv_columns_number()
	{
		return TOTAL_PV_OUTPUT_COLUMNS_BL;
	}
};

#endif
