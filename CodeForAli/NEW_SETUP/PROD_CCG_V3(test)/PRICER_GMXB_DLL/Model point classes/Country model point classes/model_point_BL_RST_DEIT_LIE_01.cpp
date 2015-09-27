#include "model_point_BL_RST_DEIT_LIE_01.h"





Model_Point_BL_RST_DEIT_LIE_01::~Model_Point_BL_RST_DEIT_LIE_01(){}
Model_Point_BL_RST_DEIT_LIE_01::Model_Point_BL_RST_DEIT_LIE_01(vector<string>& row, int rank, Scenario_configuration& scenario_config):Model_Point(row,rank,scenario_config){initialise_mp_from_inforce(row);}
Model_Point_BL_RST_DEIT_LIE_01::Model_Point_BL_RST_DEIT_LIE_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb):Model_Point(row,rank,scenario_config,pv_nb){initialise_mp_from_inforce(row);}
Model_Point_BL_RST_DEIT_LIE_01::Model_Point_BL_RST_DEIT_LIE_01(Model_Point * mp):Model_Point(mp){}
	
void Model_Point_BL_RST_DEIT_LIE_01::set_Calc_information_to_Zero()
	{
		set_Calc_information_to_Zero0();
		
		Calc_Birth_1_Date_Year =0;
		Calc_Birth_1_Date_Month =0;
		Calc_Birth_1_Date_Day =0;
		Calc_Age_1 =0;
		Calc_Age_Month_1 =0;
		Calc_fixed_fees_p_m =0.;
		Calc_NB_Coupons =0.;
		Calc_WB_Initial =0.;
		Calc_Initial_Coupon_by_Freq =0.;
		Calc_Duration_Bl =0;
		Calc_BK0 =0.;
		Calc_RRC_p_m =0.;
		Calc_Conversion_Rate_p_m =0.;
		Calc_Max_Bonus_Amount =0.;
		Calc_Initial_Deferal_Months =0;
		Calc_Gaur_Charge_Rate =0.;
		Calc_Perf_Bonus_Amount_Bl =0.;
		Calc_GMDB_Y_N =0;

	}
void Model_Point_BL_RST_DEIT_LIE_01::initialise_mp_from_inforce(vector<string>& row)
	{	
		initialise_mp0(row);

		Person_Id_1_Bl=row[PERSON_ID_1_LIE_RST_DE];
		Init_Charge= atof(row[INIT_CHARGE_LIE_RST_DE].c_str());
		Fund_Name=row[ACCOUNT_CODE_LIE_RST_DE];
		Currency=row[CURRENCY_LIE_RST_DE];
		Gender_1_Bl=row[GENDER_1_LIE_RST_DE];
		Date_Birth_1_Bl=row[DATE_BIRTH_1_LIE_RST_DE];
		Issue_Age_1_Bl=atoi(row[ISSUE_AGE_1_LIE_RST_DE].c_str());
		Issue_Date=row[ISSUE_DATE_LIE_RST_DE];
		Inv_Date=row[INV_DATE_LIE_RST_DE];
		Def_Period=atoi(row[DEF_PERIOD_LIE_RST_DE].c_str());
		Duration_Bl=atoi(row[DURATION_LIE_RST_DE].c_str());
		Annuity_Begin=row[ANNUITY_BEGIN_DATE_LIE_RST_DE];
		Prod_Prem_Type=row[PREMIUM_PAYMENT_MODE_LIE_RST_DE];	
		Premium= atof(row[INSURANCE_PREMIUM_LIE_RST_DE].c_str());
		Amount_Inv= atof(row[AMOUNT_INVESTED_LIE_RST_DE].c_str());
		Freq_Mode=row[WITHDRAWAL_MODE_LIE_RST_DE];
		Withdrawal_Rate= atof(row[CONVERSION_RATE_LIE_RST_DE].c_str());
		Inv_Protection= atof(row[DEF_GUAR_LIE_RST_DE].c_str());
		Benefit_Increase_Factor= atof(row[BENEFIT_INCREASE_FACTOR_LIE_RST_DE].c_str());	
		Gmdb_Bl= atof(row[GMDB_LIE_RST_DE].c_str());
		Guarantee= atof(row[CURR_GMWB_LIE_RST_DE].c_str());
		Accum_Withdrawal_Amount= atof(row[ACCUM_WITHDRAWAL_AMOUNT_LIE_RST_DE].c_str());
		Perf_Bonus_Amount_Bl= atof(row[PERF_BONUS_AMOUNT_LIE_RST_DE].c_str());
		Av_Bef_Bef_Bl= atof(row[ACC_VAL_BEF_BEF_LIE_RST_DE].c_str());
		Av_After_Bef_Bl= atof(row[ACC_VAL_AFT_BEF_LIE_RST_DE].c_str());
		Av_After_After= atof(row[ACC_VAL_AFT_AFT_LIE_RST_DE].c_str());
		Base_Capital= atof(row[BASE_CAPITAL_LIE_RST_DE].c_str());
		Fund_Unit_Bef_Bef_Bl= atof(row[FUND_UNITS_BEF_BEF_LIE_RST_DE].c_str());
		Fund_Unit_After_Bef_Bl= atof(row[FUND_UNITS_AFT_BEF_LIE_RST_DE].c_str());
		Fund_Unit_After_After= atof(row[FUND_UNITS_AFT_AFT_LIE_RST_DE].c_str());
		Unit_Price= atof(row[UNIT_PRICE_LIE_RST_DE].c_str());
		AV0=Av_After_After;	

		set_Calc_information_to_Zero();
	}
void Model_Point_BL_RST_DEIT_LIE_01::initialise_mp_parameters_by_copy(Model_Point * m_p)
	{
		initialise_mp_parameters_by_copy0(m_p);
		Model_Point_BL_RST_DEIT_LIE_01 * mp = dynamic_cast<Model_Point_BL_RST_DEIT_LIE_01*>(m_p);
	
		
		Person_Id_1_Bl = mp->Person_Id_1_Bl;
		Gender_1_Bl = mp->Gender_1_Bl;
		Date_Birth_1_Bl = mp->Date_Birth_1_Bl;
		Issue_Age_1_Bl = mp->Issue_Age_1_Bl;
		NB_Date_Bl = mp->NB_Date_Bl;
		Duration_Bl = mp->Duration_Bl;
		Gmdb_Bl = mp->Gmdb_Bl;
		Perf_Bonus_Amount_Bl = mp->Perf_Bonus_Amount_Bl;
		Av_Bef_Bef_Bl = mp->Av_Bef_Bef_Bl;
		Av_After_Bef_Bl = mp->Av_After_Bef_Bl;
		Fund_Unit_Bef_Bef_Bl = mp->Fund_Unit_Bef_Bef_Bl;
		Fund_Unit_After_Bef_Bl = mp->Fund_Unit_After_Bef_Bl;
		
		set_Calc_information_to_Zero();
	}




