#ifndef __MODEL_POINT_DEU_IADB05_H_
#define __MODEL_POINT_DEU_IADB05_H_

#include "../Model_Point.h"
#include "model_point_head_BL_01.h"
#include "../../Projection classes/Country projection classes/head_BL.h"

using namespace std;

class Model_Point_BL01 : public Model_Point
{
public:

	///////////////////////////Information from inforce ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////
	string   Product;
	string   Version_Inforce;
	string   Pol_Id;
	string   Group;
	string   Prod_Code;
	string   Valn_Date;
	int      Duration_Month;
	string   Issue_Date;
	string   Inv_Date;
	string   Annuity_Begin;
	//string   End_Date;
	string   Birthdate_1;
	string   Gender_1;
	string   Date_death_1;
	string   Birthdate_2;
	string   Gender_2;
	string   Date_death_2;
	double   Premium_Paid;
	double   Net_Premium;
	string   Freq_Pay;
	double   Annuity_Yearly;
	double   Withdrawals_rate;
	double   Rollup_GMWB;
	double   Ratchet_GMWB;
	double	 Basis_Kapital;
	double   Rollup_GMAB;
	double   Ratchet_GMAB;
	double   Rollup_GMDB;
	double   Last_Bonus_Valuation;
	double   Bonus_Rate;
	string   Currency;
	double   Nav;
	double   Nb_units;
	string   Fund_ID;
	double   Av0;
	double   Rrc;
	double   Fund_Fees;
	double   Ehc;
	double   Fund_Fees_TER;
	double   Fixed_Fees;
	string   Pol_Status;
	bool     New_Business;
	double   Claims_Last_Month;
	double   Charges_Last_Month;
	double   initial_charges;

	///////////////////////////Calculated Information ////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////


	int      Calc_Valn_Year;
	int      Calc_Valn_Month;
	int      Calc_Valn_Day;
	int      Calc_IssueDate_Year;
	int      Calc_IssueDate_Month;
	int      Calc_IssueDate_Day;
	int      Calc_Birthdate1_Year;
	int      Calc_Birthdate1_Month;
	int      Calc_Birthdate1_Day;
	int      Calc_Birthdate2_Year;
	int      Calc_Birthdate2_Month;
	int      Calc_Birthdate2_Day;
	int      Calc_StartAnnuityPay_Year;
	int      Calc_StartAnnuityPay_Month;
	int      Calc_StartAnnuityPay_Day;
	int      Calc_Remaining_Months;
	int      Calc_Duration_Months;
	int      Calc_Deferal_Duration_Months;
	int      Calc_Initial_Deferal_Duration_Months;
	int      Calc_First_Withdrawal_Age;
	int      Calc_Age_1;
	int      Calc_Age_Month_1;
	int      Calc_Age_2;
	double   Calc_RRC_p_m;
	double   Calc_Fund_Fees_p_m;
	double   Calc_TER_p_m;
	int      Calc_Freq_Pay;
	int      Calc_Bonus_Freq_Pay;
	double   Calc_Withdrawals_rate;
	double   Calc_Factor_Adj_Bonus;
	double   Calc_Bonus_Base;
	//int		 Calc_Bonus_Freq_Pay;
	double	 Calc_Coupon_by_Freq;
	double   Calc_Initial_Guar_Coupon_by_Freq;



	virtual ~Model_Point_BL01(){}
	
	Model_Point_BL01(vector<string>& row, int rank, Product_configuration& product_conf,
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

	Model_Point_BL01(Model_Point_BL01 * mp, int index_shock_size_in, int fund_shock_size_in):Model_Point()
	{
		this->get_timestamp();
		this->HE_initialise_mp_from_inforce(mp);
		this->mp_position_in_file = mp->mp_position_in_file;
		this->load_longstaff = 0;
		this->load_model_point_funds = 0;
	}


	void initialise_mp_from_inforce(vector<string>& row)
	{
		Product=row[PRODUCT];
		Version_Inforce=row[VERSION_INFORCE];
		Pol_Id=row[POL_ID];
		Group=row[GROUP_BL];
		Prod_Code=row[PROD_CODE];
		Valn_Date=row[VALN_DATE];
		Duration_Month=atoi(row[DURATION_MONTH].c_str());
		Issue_Date=row[ISSUE_DATE];
		Inv_Date=row[INV_DATE];
		Annuity_Begin=row[ANNUITY_BEGIN];
		//End_Date=row[END_DATE];
		Birthdate_1=row[BIRTHDATE_1];
		Gender_1=row[GENDER_1];
		Date_death_1=row[DATE_DEATH_1];
		Birthdate_2=row[BIRTHDATE_2];
		Gender_2=row[GENDER_2];
		Date_death_2=row[DATE_DEATH_1];
		Premium_Paid= atof(row[PREMIUM_PAID].c_str());
		Net_Premium= atof(row[NET_PREMIUM].c_str());
		Freq_Pay=row[FREQ_PAY];
		Annuity_Yearly= atof(row[ANNUITY_YEARLY].c_str());
		Withdrawals_rate= atof(row[WITHDRAWALS_RATE].c_str());
		Rollup_GMWB= atof(row[ROLLUP_GMWB].c_str());
		Ratchet_GMWB= atof(row[RATCHET_GMWB].c_str());
		Basis_Kapital=atof(row[BASIS_KAPITAL].c_str());
		Rollup_GMAB= atof(row[ROLLUP_GMAB].c_str());
		Ratchet_GMAB= atof(row[RATCHET_GMAB].c_str());
		Rollup_GMDB= atof(row[ROLLUP_GMDB].c_str());
		Last_Bonus_Valuation= atof(row[LAST_BONUS_VALUATION].c_str());
		Bonus_Rate= atof(row[BONUS_RATE].c_str());
		Currency=row[CURRENCY];
		Nav= atof(row[NAV].c_str());
		Nb_units= atof(row[NB_UNITS].c_str());
		Fund_ID=row[FUND_ID];
		Av0= atof(row[AV0].c_str());
		Rrc= atof(row[RRC].c_str());
		Fund_Fees= atof(row[FUND_FEES].c_str());
		Ehc= atof(row[EHC].c_str());
		Fund_Fees_TER= atof(row[FUND_FFEES_TER].c_str());
		Fixed_Fees= atof(row[FIXED_FFEES].c_str());
		Pol_Status=row[POL_STATUS];
		New_Business=(atoi(row[NEW_BUSINESS].c_str())==1);
		Claims_Last_Month= atof(row[CLAIMS_LAST_MONTH].c_str());
		Charges_Last_Month= atof(row[CHARGES_LAST_MONTH].c_str());
		initial_charges= atof(row[INITIAL_CHARGES].c_str());

		av_if_array[0] = atof(row[AV0].c_str());
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
		Model_Point_BL01 * mp = dynamic_cast<Model_Point_BL01*>(m_p);

		Product  = mp->Product;
		Version_Inforce  = mp->Version_Inforce;
		Pol_Id  = mp->Pol_Id;
		Group  = mp->Group;
		Prod_Code  = mp->Prod_Code;
		Valn_Date  = mp->Valn_Date;
		Duration_Month  = mp->Duration_Month;
		Issue_Date  = mp->Issue_Date;
		Inv_Date  = mp->Inv_Date;
		Annuity_Begin  = mp->Annuity_Begin;
		//End_Date  = mp->End_Date;
		Birthdate_1  = mp->Birthdate_1;
		Gender_1  = mp->Gender_1;
		Date_death_1=mp->Date_death_1;
		Birthdate_2  = mp->Birthdate_2;
		Gender_2  = mp->Gender_2;
		Date_death_2=mp->Date_death_2;
		Premium_Paid  = mp->Premium_Paid;
		Net_Premium  = mp->Net_Premium;
		Freq_Pay  = mp->Freq_Pay;
		Annuity_Yearly  = mp->Annuity_Yearly;
		Withdrawals_rate  = mp->Withdrawals_rate;
		Rollup_GMWB  = mp->Rollup_GMWB;
		Ratchet_GMWB  = mp->Ratchet_GMWB;
		Basis_Kapital= mp->Basis_Kapital; 
		Rollup_GMAB  = mp->Rollup_GMAB;
		Ratchet_GMAB  = mp->Ratchet_GMAB;
		Rollup_GMDB  = mp->Rollup_GMDB;
		Last_Bonus_Valuation  = mp->Last_Bonus_Valuation;
		Bonus_Rate  = mp->Bonus_Rate;
		Currency  = mp->Currency;
		Nav  = mp->Nav;
		Nb_units  = mp->Nb_units;
		Fund_ID  = mp->Fund_ID;
		Av0  = mp->Av0;
		Rrc  = mp->Rrc;
		Fund_Fees  = mp->Fund_Fees;
		Ehc  = mp->Ehc;
		Fund_Fees_TER  = mp->Fund_Fees_TER;
		Fixed_Fees  = mp->Fixed_Fees;
		Pol_Status  = mp->Pol_Status;
		New_Business  = mp->New_Business;
		Claims_Last_Month  = mp->Claims_Last_Month;
		Charges_Last_Month  = mp->Charges_Last_Month;
		initial_charges  = mp->initial_charges;
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
		Calc_Birthdate1_Year =0;
		Calc_Birthdate1_Month =0;
		Calc_Birthdate1_Day =0;
		Calc_Birthdate2_Year =0;
		Calc_Birthdate2_Month =0;
		Calc_Birthdate2_Day =0;
		Calc_StartAnnuityPay_Year =0;
		Calc_StartAnnuityPay_Month =0;
		Calc_StartAnnuityPay_Day =0;
		Calc_Remaining_Months =0;
		Calc_Duration_Months =0;
		Calc_Deferal_Duration_Months =0;
		Calc_First_Withdrawal_Age =0;
		Calc_Age_1 =0;
		Calc_Age_Month_1 =0;
		Calc_Age_2 =0;
		Calc_RRC_p_m =0.;
		Calc_Fund_Fees_p_m =0.;
		Calc_TER_p_m =0.;
		Calc_Freq_Pay =0;
		Calc_Withdrawals_rate =0.;
		Calc_Factor_Adj_Bonus=0.;
		Calc_Initial_Deferal_Duration_Months=0;
		Calc_Bonus_Freq_Pay=0;
		Calc_Bonus_Base=0.;
		Calc_Bonus_Freq_Pay=0;
		Calc_Coupon_by_Freq=0.;
		Calc_Initial_Guar_Coupon_by_Freq=0.;
	}
	int get_total_pv_columns_number()
	{
		return TOTAL_PV_OUTPUT_COLUMNS_BL;
	}
};

#endif
