#include "Model_Point.h"





Model_Point::~Model_Point(){
		// delete of pv indexes
		for (int i = 0; i < this->index_shock_size; i++){
			if (this->index_shock_pvs[i]!=0)
				delete[] this->index_shock_pvs[i];
		}
		if(this->index_shock_pvs!=0)
			delete[] this->index_shock_pvs;
	}
Model_Point::Model_Point(){
		this->get_timestamp();
		this->index_shock_size = 0;
		this->index_shock_pvs = 0;
	}	
void Model_Point::get_timestamp()
	{
		char dateStr [9];
		char timeStr [9];
		char* next_token=new char[200];
		_strdate_s( dateStr);
		_strtime_s( timeStr );
		string month = strtok_s(dateStr,"/",&next_token);
		string today = strtok_s(NULL, "/",&next_token) ;
		string year = "20" + (string)strtok_s(NULL, "/",&next_token) ;
		//delete [] next_token;
	}
int Model_Point::get_total_pv_columns_number(){return TOTAL_PV_OUTPUT_COLUMNS;}	
	/** Assignment operator for Model_Point class*/
	/** Copy constructor of Model_Point class*/
Model_Point::Model_Point(const Model_Point& mp)
	{
		this->index_shock_size = mp.index_shock_size;
		for (int i = 0; i < this->index_shock_size; i++){
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = mp.index_shock_pvs[i][j];
		}
	}		
Model_Point::Model_Point(vector<string>& row, int rank, Scenario_configuration& scenario_config)
	{			
		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++){
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = 0.;
		}
		this->mp_position_in_file = rank;
		this->get_timestamp();
		initialise_mp_from_inforce(row);
	}	
Model_Point::Model_Point(vector<string>& row, int rank,Scenario_configuration& scenario_config, int pv_nb)
	{		
		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++){
			this->index_shock_pvs[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->index_shock_pvs[i][j] = 0.;
		}
		this->mp_position_in_file = rank;
		this->get_timestamp();
		this->initialise_mp_from_inforce(row);
	}	
void Model_Point::initialise_mp_from_inforce(vector<string>& row){
		initialise_mp0(row);
		set_Calc_information_to_Zero0();
	}
void Model_Point::set_Calc_information_to_Zero0()
	{
		Calc_Valn_Year=0;
		Calc_Valn_Month=0;
		Calc_Valn_Day=0;
		Calc_IssueDate_Year=0;
		Calc_IssueDate_Month=0;
		Calc_IssueDate_Day=0;
		Calc_Annuity_Begin_Year=0;
		Calc_Annuity_Begin_Month=0;
		Calc_Annuity_Begin_Day=0;
		Calc_Prorata=0.;
		Calc_Fund_Fees_p_m=0.;
		Calc_Ehc_p_m=0.;
		Calc_Freq_Pay=0;	
		Calc_Remaining_Months=0;
		Calc_Duration_Months=0;
		Calc_Deferal_Duration_Months=0;
		Calc_Coupon_by_Freq=0.;
		Calc_Mod_Def_Duration_Months=0;	
	}
void Model_Point::initialise_mp0(vector<string>& row)
	{
		Product_Version=row[PRODUCT];
		Prod_Code=row[PROD_CODE];
		Valn_Date=row[VALN_DATE];
		Fund_Fees= atof(row[FUND_FEES].c_str());
		New_Re_Reinsurance_premium=Ehc= atof(row[INFO].c_str());
		Ehc= atof(row[EHC].c_str());
		New_Business=(atoi(row[NEW_BUSINESS].c_str())==1);
		Pol_Id=row[POL_ID];
	}
void Model_Point::initialise_mp_parameters_by_copy(Model_Point * mp){initialise_mp_parameters_by_copy0(mp);}
void Model_Point::initialise_mp_parameters_by_copy0(Model_Point * mp)
	{
		Product_Version=mp->Product_Version;
		Prod_Code=mp->Prod_Code;
		Valn_Date=mp->Valn_Date;
		Fund_Fees=mp->Fund_Fees;		   
		New_Re_Reinsurance_premium=mp->New_Re_Reinsurance_premium;		  
		Ehc=mp->Ehc;		   
		New_Business=mp->New_Business;		  
		Pol_Id=mp->Pol_Id;		  
		Init_Charge=mp->Init_Charge;		
		Fund_Name=mp->Fund_Name;	
		Currency=mp->Currency;	
		Issue_Date=mp->Issue_Date;
		Inv_Date=mp->Inv_Date;	
		Def_Period=mp->Def_Period;	
		Annuity_Begin=mp->Annuity_Begin;
		Prod_Prem_Type=mp->Prod_Prem_Type;
		Premium=mp->Premium;
		Amount_Inv=mp->Amount_Inv;
		Freq_Mode=mp->Freq_Mode;
		Withdrawal_Rate=mp->Withdrawal_Rate;
		Inv_Protection=mp->Inv_Protection;
		Benefit_Increase_Factor=mp->Benefit_Increase_Factor;
		Guarantee=mp->Guarantee;
		Accum_Withdrawal_Amount=mp->Accum_Withdrawal_Amount;
		Av_After_After=mp->Av_After_After;
		Base_Capital=mp->Base_Capital;
		Fund_Unit_After_After=mp->Fund_Unit_After_After;
		Unit_Price=mp->Unit_Price;
		Nav_Reporting_Date=mp->Nav_Reporting_Date;
		AV0=mp->AV0;	
		mp_position_in_file=mp->mp_position_in_file;

		set_Calc_information_to_Zero0();

	}	
Model_Point::Model_Point(Model_Point * mp)
	{
		this->index_shock_size = mp->index_shock_size;
		for (int i = 0; i < this->index_shock_size; i++){
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = mp->index_shock_pvs[i][j];
		}
		this->get_timestamp();
		initialise_mp_parameters_by_copy(mp);
	}
void Model_Point::set_pv_index_shock_array(Scenario_configuration& scenario_config)
	{
		// delete of pv indexes
		for (int i = 0; i < this->index_shock_size; i++)
		{
			if (this->index_shock_pvs[i]!=0)
				delete[] this->index_shock_pvs[i];
		}

		if(this->index_shock_pvs!=0)
			delete[] this->index_shock_pvs;

		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[get_total_pv_columns_number()];
			for (int j=0 ; j < get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = 0.;
		}

	}
	
