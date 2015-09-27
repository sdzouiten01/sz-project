#include "model_point_BL_LUX_01.h"






Model_Point_LUX_01::~Model_Point_LUX_01(){}
Model_Point_LUX_01::Model_Point_LUX_01(vector<string>& row, int rank, Scenario_configuration& scenario_config):Model_Point(row,rank,scenario_config){initialise_mp_from_inforce(row);}
Model_Point_LUX_01::Model_Point_LUX_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb):Model_Point(row,rank,scenario_config,pv_nb){initialise_mp_from_inforce(row);}
Model_Point_LUX_01::Model_Point_LUX_01(Model_Point * mp):Model_Point(mp){}	
void Model_Point_LUX_01::set_Calc_information_to_Zero()
	{
		set_Calc_information_to_Zero0();
		
		Calc_Birth_1_Date_Year =0;
		Calc_Birth_1_Date_Month =0;
		Calc_Birth_1_Date_Day =0;
		Calc_Birth_2_Date_Year =0;
		Calc_Birth_2_Date_Month =0;
		Calc_Birth_2_Date_Day =0;
		Calc_Death_1_Date_Year =0;
		Calc_Death_1_Date_Month =0;
		Calc_Death_1_Date_Day =0;
		Calc_Death_2_Date_Year =0;
		Calc_Death_2_Date_Month =0;
		Calc_Death_2_Date_Day =0;
		Calc_Age_1 =0;
		Calc_Age_Month_1 =0;
		Calc_Age_2 =0;	
		Calc_fixed_fees_p_m =0.;
		Calc_WB_Rollup =0.;
		Calc_Initial_Coupon_by_Freq =0.;
		Calc_BK0 =0.;
		Calc_RRC_p_m =0.;
		Calc_Conversion_Rate_p_m =0.;
		Calc_Min_Bonus_Amount =0.;	
		Calc_Initial_Deferal_Months =0;
		Calc_Gaur_Charge_Rate =0.;
	
	}
void Model_Point_LUX_01::initialise_mp_from_inforce(vector<string>& row)
	{	
		initialise_mp0(row);

		Person_Id_1_Lux=row[PERSON_ID_1_BL_LUX];
		Person_Id_2_Lux=row[PERSON_ID_2_BL_LUX];
		Init_Charge= atof(row[INIT_CHARGE_BL_LUX].c_str());
		Ong_Acqui_Charge_Lux= atof(row[ONGOING_ACQUI_CHARGE_BL_LUX].c_str());
		Fund_Name=row[ACCOUNT_CODE_BL_LUX];
		Currency=row[CURRENCY_BL_LUX];
		Gender_1_Lux=row[GENDER_1_BL_LUX];
		Gender_2_Lux=row[GENDER_2_BL_LUX];
		Date_Birth_1_Lux=row[DATE_BIRTH_1_BL_LUX];
		Date_Birth_2_Lux=row[DATE_BIRTH_2_BL_LUX];
		Date_Death_1_Lux=row[DATE_DEATH_1_BL_LUX];
		Date_Death_2_Lux=row[DATE_DEATH_2_BL_LUX];
		Issue_Age_1_Lux=atoi(row[ISSUE_AGE_1_BL_LUX].c_str());
		Issue_Age_2_Lux=atoi(row[ISSUE_AGE_2_BL_LUX].c_str());
		Tech_Issue_Age_Lux=atoi(row[TECHNICAL_ISSUE_AGE_BL_LUX].c_str());
		Issue_Date=row[ISSUE_DATE_BL_LUX];
		Inv_Date=row[INV_DATE_BL_LUX];
		Def_Period=atoi(row[DEF_PERIOD_BL_LUX].c_str());
		Duration_Lux=atoi(row[DURATION_BL_LUX].c_str());
		Annuity_Begin=row[ANNUITY_BEGIN_DATE_BL_LUX];
		Prod_Prem_Type=row[PREMIUM_PAYMENT_MODE_BL_LUX];	
		Premium= atof(row[INSURANCE_PREMIUM_BL_LUX].c_str());
		Amount_Inv= atof(row[AMOUNT_INVESTED_BL_LUX].c_str());
		Freq_Mode=row[WITHDRAWAL_MODE_BL_LUX];
		Withdrawal_Rate= atof(row[CONVERSION_RATE_BL_LUX].c_str());
		Rollup_Rate_Lux= atof(row[ROLLUP_RATE_BL_LUX].c_str());
		Def_Guarantie_Lux= atof(row[DEF_GUAR_BL_LUX].c_str());
		Inv_Protection= atof(row[INV_PROTECTION_BL_LUX].c_str());
		Benefit_Increase_Factor= atof(row[BENEFIT_INCREASE_FACTOR_BL_LUX].c_str());
		Withrawals_Mode_Discount_Lux= atof(row[WITHDRAWAL_MODE_DISCOUNT_BL_LUX].c_str());
		Gmdb_Rider_Lux=row[GMDB_RIDER_BL_LUX];
		Gmdb_Mode_Lux=atoi(row[GMDB_MODE_BL_LUX].c_str());
		Gmdb_Max_Age_Lux=atoi(row[GMDB_MAX_AGE_BL_LUX].c_str());
		Gmdb_Duration_Lux=atoi(row[GMDB_DURATION_BL_LUX].c_str());
		Gmdb_Lux= atof(row[GMDB_LUX].c_str());
		Guarantee= atof(row[CURR_GMWB_BL_LUX].c_str());
		Accum_Withdrawal_Amount= atof(row[ACCUM_WITHDRAWAL_AMOUNT_BL_LUX].c_str());
		Perf_Bonus_Lux=row[PERF_BONUS_BL_LUX];
		Perf_Bonus_Level_Lux= atof(row[PERF_BONUS_LEVEL_BL_LUX].c_str());
		Perf_Bonus_Type_Lux=atoi(row[PERF_BONUS_TYPE_BL_LUX].c_str());
		Add_Accum_Withrawal_Amount_Lux= atof(row[ADD_ACCUM_WITHDRAWAL_AMOUNT_BL_LUX].c_str());
		Perf_Bonus_Amount_Lux= atof(row[PERF_BONUS_AMOUNT_BL_LUX].c_str());
		Av_After_After= atof(row[ACCOUNT_VALUE_BL_LUX].c_str());
		Base_Capital= atof(row[BASE_CAPITAL_BL_LUX].c_str());
		Fund_Unit_After_After= atof(row[FUND_UNITS_BL_LUX].c_str());
		Unit_Price= atof(row[UNIT_PRICE_BL_LUX].c_str());
		Reinsurance_Premium_Lux= atof(row[REINSURANCE_PREMIUM_BL_LUX].c_str());
		Fund_Charge_Lux= atof(row[FUND_CHARGE_BL_LUX].c_str());
		Upfront_Comm_Lux= atof(row[UPFRONT_COMM_BL_LUX].c_str());
		Trail_Comm_Lux= atof(row[TRAIL_COMM_BL_LUX].c_str());
		AV0=Av_After_After;

		set_Calc_information_to_Zero();
	}	
void Model_Point_LUX_01::initialise_mp_parameters_by_copy(Model_Point * m_p)
	{
		initialise_mp_parameters_by_copy0(m_p);
		Model_Point_LUX_01 * mp = dynamic_cast<Model_Point_LUX_01*>(m_p);
	
		Person_Id_1_Lux = mp->Person_Id_1_Lux;
		Person_Id_2_Lux = mp->Person_Id_2_Lux;
		Ong_Acqui_Charge_Lux = mp->Ong_Acqui_Charge_Lux;
		Gender_1_Lux = mp->Gender_1_Lux;
		Gender_2_Lux = mp->Gender_2_Lux;
		Date_Birth_1_Lux = mp->Date_Birth_1_Lux;
		Date_Birth_2_Lux = mp->Date_Birth_2_Lux;
		Date_Death_1_Lux = mp->Date_Death_1_Lux;
		Date_Death_2_Lux = mp->Date_Death_2_Lux;
		Issue_Age_1_Lux = mp->Issue_Age_1_Lux;
		Issue_Age_2_Lux = mp->Issue_Age_2_Lux;
		Tech_Issue_Age_Lux = mp->Tech_Issue_Age_Lux;
		Duration_Lux = mp->Duration_Lux;
		Rollup_Rate_Lux = mp->Rollup_Rate_Lux;
		Def_Guarantie_Lux = mp->Def_Guarantie_Lux;
		Withrawals_Mode_Discount_Lux = mp->Withrawals_Mode_Discount_Lux;
		Gmdb_Rider_Lux = mp->Gmdb_Rider_Lux;
		Gmdb_Mode_Lux = mp->Gmdb_Mode_Lux;
		Gmdb_Max_Age_Lux = mp->Gmdb_Max_Age_Lux;
		Gmdb_Duration_Lux = mp->Gmdb_Duration_Lux;
		Gmdb_Lux = mp->Gmdb_Lux;
		Perf_Bonus_Lux = mp->Perf_Bonus_Lux;
		Perf_Bonus_Level_Lux = mp->Perf_Bonus_Level_Lux;
		Perf_Bonus_Type_Lux = mp->Perf_Bonus_Type_Lux;
		Add_Accum_Withrawal_Amount_Lux = mp->Add_Accum_Withrawal_Amount_Lux;
		Perf_Bonus_Amount_Lux = mp->Perf_Bonus_Amount_Lux;
		Reinsurance_Premium_Lux = mp->Reinsurance_Premium_Lux;
		Fund_Charge_Lux = mp->Fund_Charge_Lux;
		Upfront_Comm_Lux = mp->Upfront_Comm_Lux;
		Trail_Comm_Lux = mp->Trail_Comm_Lux;
			
		set_Calc_information_to_Zero();
	}	
	
	
