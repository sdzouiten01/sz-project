#include "model_point_BL_RST_SWISS_01.h"





	
Model_Point_BL_RST_SWISS_01::~Model_Point_BL_RST_SWISS_01(){}
Model_Point_BL_RST_SWISS_01::Model_Point_BL_RST_SWISS_01(vector<string>& row, int rank, Scenario_configuration& scenario_config):Model_Point(row,rank,scenario_config){initialise_mp_from_inforce(row);}
Model_Point_BL_RST_SWISS_01::Model_Point_BL_RST_SWISS_01(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb):Model_Point(row,rank,scenario_config,pv_nb){initialise_mp_from_inforce(row);}
Model_Point_BL_RST_SWISS_01::Model_Point_BL_RST_SWISS_01(Model_Point * mp):Model_Point(mp){}
	
void Model_Point_BL_RST_SWISS_01::set_Calc_information_to_Zero()
	{
		set_Calc_information_to_Zero0();
		Calc_Nb_Date_Year =0;
		Calc_Nb_Date_Month =0;
		Calc_Nb_Date_Day =0;
		Calc_Inv_Date_Year =0;
		Calc_Inv_Date_Month =0;
		Calc_Inv_Date_Day =0;
		Calc_NB_Coupons =0;
		Calc_Perf_By_Freq =0.;
		Calc_Max_Bonus_Amount_By_Freq =0.;		
	}
void Model_Point_BL_RST_SWISS_01::initialise_mp_from_inforce(vector<string>& row)
	{
		initialise_mp0(row);

		Prod_Name_Bl=row[PROD_NAME_BL];
		Init_Charge= atof(row[INIT_CHARGE_BL].c_str());
		Fund_Name=row[FUND_NAME_BL];
		Currency=row[CURRENCY_BL];
		Issue_Date=row[ISSUE_DATE_BL];
		Nb_Date_Bl=row[NB_DATE_BL];
		Inv_Date=row[INV_DATE_BL];
		Def_Period=atoi(row[DEF_PERIOD_BL].c_str());
		Duration_Bl=atoi(row[DURATION_BL].c_str());
		Annuity_Begin=row[ANNUITY_BEGIN_BL];
		Prod_Prem_Type=row[PROD_PREM_TYPE_BL];
		Premium= atof(row[PREMIUM_BL].c_str());
		Amount_Inv= atof(row[AMOUNT_INV_BL].c_str());
		Freq_Mode=row[FREQ_MODE_BL];
		Withdrawal_Rate= atof(row[WITHDRAWAL_RATE_BL].c_str());
		Inv_Protection= atof(row[INV_PROTECTION_BL].c_str());
		Benefit_Increase_Factor= atof(row[BENEFIT_INCREASE_FACTOR_BL].c_str());
		Withdrawal_Mode_discount_Bl= atof(row[WITHDRAWAL_MODE_DISCOUNT_BL].c_str());
		Guarantee= atof(row[GUARANTEE_BL].c_str());
		Accum_Withdrawal_Amount= atof(row[ACCUM_WITHDRAWAL_AMOUNT_BL].c_str());
		Perf_Bonus_Amount_Bl= atof(row[PERF_BONUS_AMOUNT_BL].c_str());
		Av_bef_bef_bl= atof(row[AV_BEF_BEF_BL].c_str());
		Av_After_Bef_Bl= atof(row[AV_AFTER_BEF_BL].c_str());
		Av_After_After= atof(row[AV_AFTER_AFTER_BL].c_str());
		Base_Capital= atof(row[BASE_CAPITAL_BL].c_str());
		Fund_Unit_Bef_Bef_Bl= atof(row[FUND_UNIT_BEF_BEF_BL].c_str());
		Fund_Unit_After_Bef_Bl= atof(row[FUND_UNIT_AFTER_BEF_BL].c_str());
		Fund_Unit_After_After= atof(row[FUND_UNIT_AFTER_AFTER_BL].c_str());
		AV0=Av_After_After;
				
		set_Calc_information_to_Zero();
	}
void Model_Point_BL_RST_SWISS_01::initialise_mp_parameters_by_copy(Model_Point * m_p)
	{
		initialise_mp_parameters_by_copy0(m_p);
		Model_Point_BL_RST_SWISS_01 * mp = dynamic_cast<Model_Point_BL_RST_SWISS_01*>(m_p);
	
		Prod_Name_Bl = mp->Prod_Name_Bl;
		Nb_Date_Bl = mp->Nb_Date_Bl;
		Duration_Bl = mp->Duration_Bl;
		Withdrawal_Mode_discount_Bl = mp->Withdrawal_Mode_discount_Bl;
		Perf_Bonus_Amount_Bl = mp->Perf_Bonus_Amount_Bl;
		Av_bef_bef_bl = mp->Av_bef_bef_bl;
		Av_After_Bef_Bl = mp->Av_After_Bef_Bl;
		Fund_Unit_Bef_Bef_Bl = mp->Fund_Unit_Bef_Bef_Bl;
		Fund_Unit_After_Bef_Bl = mp->Fund_Unit_After_Bef_Bl;
		
		set_Calc_information_to_Zero();
	}

	
