#pragma once
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "Model point classes/Country model point classes/model_point_BL_01.h"
#include "head_BL.h"
using namespace std;

extern ofstream trace;
class Assumption_BL_CH_WB_01 
{
	public:
		// Assumptions tables------------------------------------------------
		GMXB_Assumption_Table *mortality_table;
		GMXB_Assumption_Table *Withrawals_rate_table;
		GMXBParameters  *assump;
		int use_lapse;
		double Lapse_rate_value_wb;
		double Lapse_rate_value_ab;
		double lapse_rate_wb_p_m;
		double lapse_rate_ab_p_m;
		double Lapse_charges;
		int use_mortality;
		int Max_Age;
		int ratchet_indicator;
		int use_bonus_option;
		int Use_bonus_to_incr_coupon_payment;
		int use_guarantee_bonus_option;
		int years_guarantee_bonus;		
		int Reduction_BK_by_bonus;
		int Reduction_BK_by_coupon;
		int Reduction_BK_by_ratchet;
		double Percentage_perf;
		int use_BD_guarantee;
		int Max_Age_for_DB;		
		int DB_Mode;	
		int DB_guarantee_period;
		int Bonus_freq;
		double Percentage_perf_2;
		int Min_def_period_perf_1;
		
		double Best_estimate_lapse;
		double min_lapse_y1;
		double min_lapse;
		double x1;
		double x2;
		double x3;
		double x4;
		double x5;
		double y1;
		double y2;
		double y3;
		double weight_lapse;
		int Mortality_Lapse_Y_N;
		int Dynamic_lapse_Y_N;
		double min_surrender_charge_amount;


public:
	virtual ~Assumption_BL_CH_WB_01()
	{
		delete this->mortality_table;
		delete this->Withrawals_rate_table;
		delete this->assump;
	}
	Assumption_BL_CH_WB_01(){}
	Assumption_BL_CH_WB_01(const char * tables)
	{
		// Assumptions tables------------------------------------------------


		map<string, GMXB_Assumption_Table*> sheet_tables;

		sheet_tables["mortality_table"] = 0;
		sheet_tables["Withrawals_rate_table"] = 0;

		assump = parse_sheets(string(tables), sheet_tables, string("assump") );

		mortality_table=sheet_tables["mortality_table"];
		Withrawals_rate_table=sheet_tables["Withrawals_rate_table"];

		use_lapse = atoi(this->assump->search("use_lapse").c_str());
		use_mortality = atoi(this->assump->search("use_mortality").c_str());
		Lapse_rate_value_wb=atof(this->assump->search("Lapse_rate_value_wb").c_str());
		Lapse_rate_value_ab=atof(this->assump->search("Lapse_rate_value_ab").c_str());

		Percentage_perf=atof(this->assump->search("Percentage_perf").c_str());
		ratchet_indicator= atoi(this->assump->search("ratchet_indicator").c_str());
		use_bonus_option= atoi(this->assump->search("use_bonus_option").c_str());
		Use_bonus_to_incr_coupon_payment = atoi(this->assump->search("Use_bonus_to_incr_coupon_payment").c_str());
		use_guarantee_bonus_option= atoi(this->assump->search("use_guarantee_bonus_option").c_str());
		years_guarantee_bonus= atoi(this->assump->search("years_guarantee_bonus").c_str());
		use_BD_guarantee= atoi(this->assump->search("use_BD_guarantee").c_str());
		Max_Age_for_DB= atoi(this->assump->search("Max_Age_for_DB").c_str());
		Reduction_BK_by_bonus=atoi(this->assump->search("Reduction_BK_by_bonus").c_str());
		Reduction_BK_by_coupon=atoi(this->assump->search("Reduction_BK_by_coupon").c_str());
		Reduction_BK_by_ratchet=atoi(this->assump->search("Reduction_BK_by_ratchet").c_str());
		lapse_rate_wb_p_m=Lapse_rate_value_wb/12.;
		lapse_rate_ab_p_m=Lapse_rate_value_ab/12.;
		Lapse_charges=atof(this->assump->search("Lapse_charges").c_str());
		DB_Mode=atoi(this->assump->search("DB_Mode").c_str());	
		DB_guarantee_period=atoi(this->assump->search("DB_guarantee_period").c_str());	
		Max_Age=atoi(this->assump->search("Max_Age").c_str());
		Bonus_freq=atoi(this->assump->search("Bonus_freq").c_str());
		Percentage_perf_2=atof(this->assump->search("Percentage_perf_2").c_str());
		Min_def_period_perf_1=atoi(this->assump->search("Min_def_period_perf_1").c_str());


		Best_estimate_lapse=atof(this->assump->search("Best_estimate_lapse").c_str());
		min_lapse_y1=atof(this->assump->search("min_lapse_y1").c_str());
		min_lapse=atof(this->assump->search("min_lapse").c_str());
		x1=atof(this->assump->search("x1").c_str());
		x2=atof(this->assump->search("x2").c_str());
		x3=atof(this->assump->search("x3").c_str());
		x4=atof(this->assump->search("x4").c_str());
		x5=atof(this->assump->search("x5").c_str());
		y1=atof(this->assump->search("y1").c_str());
		y2=atof(this->assump->search("y2").c_str());
		y3=atof(this->assump->search("y3").c_str());
		weight_lapse=atof(this->assump->search("weight_lapse").c_str());
		Mortality_Lapse_Y_N=atoi(this->assump->search("Mortality_Lapse_Y_N").c_str());
		Dynamic_lapse_Y_N=atoi(this->assump->search("Dynamic_lapse_Y_N").c_str());
		min_surrender_charge_amount=atof(this->assump->search("min_surrender_charge_amount").c_str());

	}
	virtual void Get_mortality_rates_vector(const string& sex,const int& valn_age,const int& valn_year,
												const int& max_projection, const double* years_table,
												double* mort_table,double* age_table, const string& Birthdate,
												const string& Date_death)
	{
		int t;
		int index1_l_local;
		int index_2_local;
		for (t=1; t<=max_projection;t++){
			//if((use_BD_guarantee==0)||(age_table[t]>=Max_Age_for_DB)) mort_table[t]=0.;
			if(use_mortality==0) mort_table[t]=0.;
			else if ( (Birthdate == "")||(Date_death != "")) mort_table[t]=1.;
			//else if(valn_age==0) mort_table[t]=1.;
			//else if(age_table[t]>120) mort_table[t]=1.;
			else{
				index1_l_local=(sex == "M")*((int)years_table[t]-2007)+(sex != "M")*((int)years_table[t]-2007+88);
				index_2_local=valn_age-29-valn_year+2008;
				mort_table[t]=this->mortality_table->search(index1_l_local,index_2_local);
				mort_table[t]=1.-exp(log(1.-mort_table[t])/12.);
			}
		}
	}	
	void Get_base_lapse_vector(const int& Calc_Remaining_Months,double* base_lapse_vector, int wb_or_ab_indicator)
	{
		int t;
		for (t=1; t<=Calc_Remaining_Months;t++){
			base_lapse_vector[t] =wb_or_ab_indicator*lapse_rate_wb_p_m*use_lapse+(1.-wb_or_ab_indicator)*lapse_rate_ab_p_m*use_lapse;
		}
	}
};
class Projection_BL_CH_WB_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_CH_WB_01 *Assumption;

	//Model Point Class
	Model_Point_BL01 *ModelPoint;
	
	//Tables used in the projection
	double ConstVector[TOTAL_CONSTVECTOR_COLS_BL][SUBACCT_MAX_PERIOD_MTH +1];
	double MainProjArrayVector[MAIN_PROJ_NUM_COLS_BL][SUBACCT_MAX_PERIOD_MTH +1];
	double DebugVector[DEBUG_NUM_COLS_BL][SUBACCT_MAX_PERIOD_MTH +1];

	//Other Assumptions
	bool GenerateDebugFiles;
	string DbgFilePath;
	int ** ShockArray;
	int ShockSize;		
	int ShockNumber;
	int CurrentScen;

	double survival_index;
	double account_value;
	double gmwb_guarantee;
	double coupon_value;
	double bonus_value;
	double hedge_cost_fees;
	double lapse;
	double mortality;
	double claims_wb_t;
	double year_bonus_valuation;
	double gmdb_value;
	double claims_db_t;
	double basis_capital;
	double account_value_1;
	double initial_basis_capital;
	double fund_fees;
	double add_coupon_from_ratchet;
	double Performance;
	double annuity_per_freq;
	double annuity_per_freq_bef;
	double survival_index_by_step;
	double fixed_charges;
	double lapse_val;
	double discount_mortality;
	double itm;
	double dynamic_lapse_rate;
	double lapse_temp;
	double lapse_first_year;


	~Projection_BL_CH_WB_01()
	{		
		delete this->Assumption;
		for(int i = 0; i <this->ShockSize; i++){
			if(this->ShockArray[i]!=0)
				delete[] this->ShockArray[i];
		}
		if (this->ShockArray!=0)
			delete[] this->ShockArray;	
	}
	Projection_BL_CH_WB_01( const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
			const char * tables, bool generate_dbg_files,const string& debug_file_path, 
			int t_high_shorter, bool l_longevity_tables):Projection(t_high_shorter)
	{
		if(t_high_shorter != t_high){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, " Tt_high_shorter != t_high (60)");
			throw new GMXBException(message);	
		}
		this->mort_table_m = 0;
		this->mort_table_f = 0;
		this->lapse_rates = 0;
		this->assump = 0;
		this->index_shock_array = 0;
		this->index_shock_size = 0;
		this->load_longevity_tables = l_longevity_tables;
		this->Assumption =new Assumption_BL_CH_WB_01(tables);
		this->set_MainProjArrayVector_to_zero();
		this->set_ConstVector_to_zero();
		this->ShockSize = i_s_a.size();
		this->ShockNumber=0;
		this->CurrentScen=1;
		this->GenerateDebugFiles = generate_dbg_files;
		this->DbgFilePath  = debug_file_path;
		this->ShockArray = new int* [this->ShockSize];
		for(int i = 0; i < this->ShockSize; i++)
		{
			this->ShockArray[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
			for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
				this->ShockArray[i][j] = i_s_a[i][j];
		}
		this->CurrentScen=1;

		this->load_longevity_tables = l_longevity_tables;

		if (this->load_longevity_tables)
		{
			longevity_shock_table_m = new GMXB_Assumption_Table(tables , "longevity_m");
			longevity_shock_table_f = new GMXB_Assumption_Table(tables, "longevity_f");
		}	
		survival_index=0.;
		account_value=0.;
		gmwb_guarantee=0.;
		coupon_value=0.;
		bonus_value=0.;
		hedge_cost_fees=0.;
		lapse=0.;
		mortality=0.;
		claims_wb_t=0.;
		year_bonus_valuation=0.;
		gmdb_value=0.;
		claims_db_t=0.;
	}
	void set_DebugVector_to_zero(){memset(this->DebugVector, 0, sizeof(this->DebugVector));}
	void set_ConstVector_to_zero(){memset(this->ConstVector, 0, sizeof(this->ConstVector));}
	void set_MainProjArrayVector_to_zero(){memset(this->MainProjArrayVector, 0, sizeof(this->MainProjArrayVector));}
	virtual void initialise_index_map(){}
	virtual void project_t(int t, double ** pvs, int curr_shock_nb, double lapse_mult, double mortality_mult, bool shock_av) {}
	void set_value_on_debug_file ( double value, int row, int t){DebugVector[row][t]=value;}
	void set_current_model_point(Model_Point *mp)
	{
		this->ModelPoint = dynamic_cast<Model_Point_BL01*>(mp);
		this->initialise_mp_calculation();
		this->InitProjection();
	}
	virtual void set_annual_from_monthly(double *** ann_from_mthly, int scen_num){
		this->afmr = ann_from_mthly;
		this->CurrentScen= scen_num;
	}
	virtual void set_monthly_rates(double ** monthly_rates, int scen_num)
	{
		this->mr = monthly_rates;
		this->CurrentScen= scen_num;
	}
	virtual void set_index_shock_array(const vector<vector <int>>& i_s_a)
	{

		// Deletes the old shock array
		for(int i = 0; i <this->ShockSize; i++)
		{
			if(this->ShockArray[i] != 0)
				delete[] this->ShockArray[i];
		}

		if (this->ShockSize > 0)
			delete[] this->ShockArray;

		// Initilize the new shock array
		this->ShockSize = i_s_a.size();
		if (this->ShockSize > 0)
		{
			this->ShockArray = new int* [this->ShockSize];
			for(int i = 0; i < this->ShockSize; i++)
			{
				this->ShockArray[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
				for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
					this->ShockArray[i][j] = i_s_a[i][j];
			}
		}
	}
	void init_market_data()
	{	
		//initialize discount factor at t=0
		MainProjArrayVector[DISCOUNT_FACTOR_BL][0] = 1.;
		//initialize fund return at t=0
		MainProjArrayVector[FUND_RETURN_BL][0] = 0.;
		//initialize market data for each time step		
		for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			MainProjArrayVector[DISCOUNT_FACTOR_BL][t] = this->mr[t][MAXIMUM_NUMBER_OF_INDEXES + 1]; 
			MainProjArrayVector[FUND_RETURN_BL][t] = exp(log(1.+this->mr[t][0])/12.)-1.;
		}
	}
	virtual void Write_Debug_File()
	{
		char debug_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(debug_file_name, "%smain_array_mp%d_scn%d.csv" ,this->DbgFilePath.c_str(), this->ModelPoint->mp_position_in_file, this->CurrentScen);
		this->debug_file.open(debug_file_name);
		this->debug_file << "TIME_STEP,YEAR_BL,MONTH_BL,DURATION_YEAR_BL,DURATION_MONTH_BL,AGE_YEAR_BL,AGE_MONTH_BL,DATE_YEAR_BL,DATE_MONTH_BL,IND_CONTRACT_PERIOD_BL,IND_DEFERAL_PERIOD_BL,IND_RATCHET_MONTH_BL,IND_COUPON_DATE_BL,";
		this->debug_file << "IND_BONUS_DATE_BL,IND_BONUS_VALUATION_DATE_BL,GROSS_PREMIUM_BL,DEATH_R_1_BL,DEATH_R_2_BL,DEATH_R_BL,LAPSE_RATE_BL,NUMBER_COUPON_PAID,SRVIVAL_WITHOUT_LAPSE,FUND_RETURN_BL,DISCOUNT_FACTOR_BL,INITIAL_BASIS_CAPITAL_BL,";
		this->debug_file << "GUAR_GMDB_BL,COUPON_VALUE_BL,BONUS_VALUE_BL,HEDGE_COST_FEES_BL,ACCOUNT_VALUE_END_BL,CLAIMS_WB_BL,CLAIMS_DB_BL,SURVIVAL_INDEX_BL,BASIS_KAPITAL_BL,ACCOUNT_VALUE_BEF_COUPON_BL,ADD_COUPON_RATCHET_BL,SURVIVAL_INDEX_STEP_BL,PERFORMANCE_BL,DYNAMIC_LAPSE";
		this->debug_file << endl;

		for ( int t=0; t<= this->ModelPoint->Calc_Remaining_Months; t++)
		{
			this->debug_file <<setprecision(15)<< t << ",";
			for(int col = 0; col < TOTAL_CONSTVECTOR_COLS_BL ; col++)
				this->debug_file <<setprecision(15)<< this->ConstVector[col][t] << ",";
			for(int col = 0; col < MAIN_PROJ_NUM_COLS_BL ; col++)
				this->debug_file << setprecision(15)<< this->MainProjArrayVector[col][t] << ",";
			for(int col = 0; col < DEBUG_NUM_COLS_BL ; col++)
				this->debug_file << setprecision(15)<< this->DebugVector[col][t] << ",";
			this->debug_file << endl;
		}
		this->debug_file.close();
	}	
	void set_pvsScenOutput(Model_Point& mp,  unsigned int nScenPath_ip)
	{
	   	char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "DEU don't support scen_by_scen output");
		throw new GMXBException(message);	
	}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	virtual void initialise_mp_calculation()
	{
		int local_val=0;
		local_val=atoi(this->ModelPoint->Valn_Date.c_str());
		this->ModelPoint->Calc_Valn_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Valn_Month=(int)((local_val-10000*this->ModelPoint->Calc_Valn_Year)/100);
		this->ModelPoint->Calc_Valn_Day=(int)(local_val-10000*this->ModelPoint->Calc_Valn_Year-100*this->ModelPoint->Calc_Valn_Month);

		local_val=atoi(this->ModelPoint->Issue_Date.c_str());
		this->ModelPoint->Calc_IssueDate_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_IssueDate_Month=(int)((local_val-10000*this->ModelPoint->Calc_IssueDate_Year)/100);
		this->ModelPoint->Calc_IssueDate_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_IssueDate_Month);

		if(this->ModelPoint->Birthdate_1!="")
		{
			local_val=atoi(this->ModelPoint->Birthdate_1.c_str());
			this->ModelPoint->Calc_Birthdate1_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birthdate1_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birthdate1_Year)/100);
			this->ModelPoint->Calc_Birthdate1_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birthdate1_Year-100*this->ModelPoint->Calc_Birthdate1_Month);
		}
		else
		{
			this->ModelPoint->Calc_Birthdate1_Year=0;
			this->ModelPoint->Calc_Birthdate1_Month=0;
			this->ModelPoint->Calc_Birthdate1_Day=0;
		}
		if(this->ModelPoint->Birthdate_2!="")
		{
			local_val=atoi(this->ModelPoint->Birthdate_2.c_str());
			this->ModelPoint->Calc_Birthdate2_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birthdate2_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birthdate2_Year)/100);
			this->ModelPoint->Calc_Birthdate2_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birthdate2_Year-100*this->ModelPoint->Calc_Birthdate2_Month);
		}
		else
		{
			this->ModelPoint->Calc_Birthdate2_Year=0;
			this->ModelPoint->Calc_Birthdate2_Month=0;
			this->ModelPoint->Calc_Birthdate2_Day=0;
		}
		local_val=atoi(this->ModelPoint->Annuity_Begin.c_str());
		this->ModelPoint->Calc_StartAnnuityPay_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_StartAnnuityPay_Month=(int)((local_val-10000*this->ModelPoint->Calc_StartAnnuityPay_Year)/100);
		this->ModelPoint->Calc_StartAnnuityPay_Day=(int)(local_val-10000*this->ModelPoint->Calc_StartAnnuityPay_Year-100*this->ModelPoint->Calc_StartAnnuityPay_Month);


		this->ModelPoint->Calc_Duration_Months=(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_IssueDate_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_IssueDate_Month);
		
		this->ModelPoint->Calc_Remaining_Months=this->ModelPoint->Duration_Month-this->ModelPoint->Calc_Duration_Months;

		this->ModelPoint->Calc_Deferal_Duration_Months=-(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_StartAnnuityPay_Year)*12-(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_StartAnnuityPay_Month);
		this->ModelPoint->Calc_Initial_Deferal_Duration_Months=-(this->ModelPoint->Calc_IssueDate_Year-this->ModelPoint->Calc_StartAnnuityPay_Year)*12-(this->ModelPoint->Calc_IssueDate_Month-this->ModelPoint->Calc_StartAnnuityPay_Month);
		
		if(this->ModelPoint->Birthdate_1!="")
			this->ModelPoint->Calc_Age_1=(int)(((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birthdate1_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birthdate1_Month))/12.);
		else
			this->ModelPoint->Calc_Age_1=0.;
		if(this->ModelPoint->Birthdate_2!="")		
			this->ModelPoint->Calc_Age_2=(int)(((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birthdate2_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birthdate2_Month))/12.);
		else
			this->ModelPoint->Calc_Age_2=0.;

		this->ModelPoint->Calc_First_Withdrawal_Age=(int)(((this->ModelPoint->Calc_StartAnnuityPay_Year-this->ModelPoint->Calc_Birthdate1_Year)*12+(this->ModelPoint->Calc_StartAnnuityPay_Month-this->ModelPoint->Calc_Birthdate1_Month))/12.);
		if(this->ModelPoint->Birthdate_1!="")
			this->ModelPoint->Calc_Age_Month_1=((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birthdate1_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birthdate1_Month))%12;	
		else this->ModelPoint->Calc_Age_Month_1=0;
		this->ModelPoint->Calc_RRC_p_m=this->ModelPoint->Rrc/12.;
		this->ModelPoint->Calc_Fund_Fees_p_m=this->ModelPoint->Fund_Fees/12.;
		this->ModelPoint->Calc_TER_p_m=this->ModelPoint->Fund_Fees_TER;
		this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate;

		if(this->ModelPoint->Freq_Pay=="MON")
			this->ModelPoint->Calc_Freq_Pay=12;
		else if(this->ModelPoint->Freq_Pay=="QUA")
			this->ModelPoint->Calc_Freq_Pay=4;
		else if(this->ModelPoint->Freq_Pay=="SEM")
			this->ModelPoint->Calc_Freq_Pay=2;
		else
			this->ModelPoint->Calc_Freq_Pay=1;
		
		this->ModelPoint->Calc_Bonus_Freq_Pay=1;

		this->ModelPoint->Calc_Factor_Adj_Bonus=1.;
		this->ModelPoint->Calc_Bonus_Freq_Pay=1;
		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Annuity_Yearly/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Initial_Guar_Coupon_by_Freq=this->ModelPoint->Rollup_GMWB*this->ModelPoint->Calc_Withdrawals_rate/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Bonus_Base=this->ModelPoint->Premium_Paid;
	}
	void Calculate_ConstVector_timing()
	{
		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==1){
				ConstVector[MONTH_BL][0]=0;
				ConstVector[MONTH_BL][t]=1;
				ConstVector[YEAR_BL][t]=1;
				ConstVector[DURATION_YEAR_BL][t]=(int)(this->ModelPoint->Calc_Duration_Months/12.);
				ConstVector[DURATION_YEAR_BL][0]=max((int)((this->ModelPoint->Calc_Duration_Months-1)/12.),0);

				ConstVector[DURATION_MONTH_BL][t]=(int)(this->ModelPoint->Calc_Duration_Months%12);
				ConstVector[DURATION_MONTH_BL][0]=(int)((this->ModelPoint->Calc_Duration_Months-1)%12);
				ConstVector[AGE_YEAR_BL][t]=this->ModelPoint->Calc_Age_1;
				ConstVector[AGE_MONTH_BL][t]=this->ModelPoint->Calc_Age_Month_1;
				ConstVector[AGE_YEAR_BL][0]=ConstVector[AGE_YEAR_BL][t]-((int)ConstVector[AGE_MONTH_BL][0]==0);
				ConstVector[AGE_MONTH_BL][0]=((int)ConstVector[AGE_MONTH_BL][t]!=0)*((int)ConstVector[AGE_MONTH_BL][t]-1)+((int)ConstVector[AGE_MONTH_BL][t]==0)*11;
				ConstVector[DATE_YEAR_BL][t]=this->ModelPoint->Calc_Valn_Year;
				ConstVector[DATE_MONTH_BL][t]=this->ModelPoint->Calc_Valn_Month;
			}
			else{
				ConstVector[MONTH_BL][t]=ConstVector[MONTH_BL][t-1]+1;
				ConstVector[YEAR_BL][t]=(int)(ConstVector[MONTH_BL][t-1]/12)+1;
				
				ConstVector[DURATION_MONTH_BL][t]=(int)((ConstVector[DURATION_MONTH_BL][t-1]!=11)*(ConstVector[DURATION_MONTH_BL][t-1]+1));
				ConstVector[DURATION_YEAR_BL][t]=(int)ConstVector[DURATION_YEAR_BL][t-1];
				if((int)ConstVector[DURATION_MONTH_BL][t]==0)
					ConstVector[DURATION_YEAR_BL][t]+=1;
				
				ConstVector[AGE_MONTH_BL][t]=(int)((ConstVector[AGE_MONTH_BL][t-1]!=11)*(ConstVector[AGE_MONTH_BL][t-1]+1));
				ConstVector[AGE_YEAR_BL][t]=(int)ConstVector[AGE_YEAR_BL][t-1];
				if((int)ConstVector[AGE_MONTH_BL][t]==0)
					ConstVector[AGE_YEAR_BL][t]+=1;
				
				ConstVector[DATE_MONTH_BL][t]=(int)((ConstVector[DATE_MONTH_BL][t-1]!=12)*(ConstVector[DATE_MONTH_BL][t-1]+1)+(ConstVector[DATE_MONTH_BL][t-1]==12));
				//ConstVector[DATE_MONTH_BL][t]=(int)ConstVector[DATE_MONTH_BL][t-1];
				ConstVector[DATE_YEAR_BL][t]=ConstVector[DATE_YEAR_BL][t-1];
				if((int)ConstVector[DATE_MONTH_BL][t-1]==12)
					ConstVector[DATE_YEAR_BL][t]+=1;
			}
		}
	}
	virtual void Calculate_ConstVector_guarantees()
	{
		//ConstVector[IND_BONUS_VALUATION_DATE_BL][0]=(1-ConstVector[IND_DEFERAL_PERIOD_BL][1])*(this->ModelPoint->Last_Bonus_Valuation!=0.);
		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			ConstVector[IND_CONTRACT_PERIOD_BL][t]=(int)(ConstVector[MONTH_BL][t]<=this->ModelPoint->Calc_Remaining_Months);
			ConstVector[IND_DEFERAL_PERIOD_BL][t]=(int)(ConstVector[MONTH_BL][t]<=this->ModelPoint->Calc_Deferal_Duration_Months);
			
			if(this->Assumption->use_bonus_option==1)
			{
				if(  ( ((((int) ConstVector[MONTH_BL][t]%(this->Assumption->Bonus_freq*12))+11)%(this->Assumption->Bonus_freq*12)) ==  ((((int) this->ModelPoint->Calc_Deferal_Duration_Months%(this->Assumption->Bonus_freq*12))+11)%(this->Assumption->Bonus_freq*12)) )      
					&&  ( ConstVector[MONTH_BL][t] > ( this->ModelPoint->Calc_Deferal_Duration_Months - (this->Assumption->use_guarantee_bonus_option==1) ) ) )
					ConstVector[IND_BONUS_VALUATION_DATE_BL][t]=1;
			}

			if(this->Assumption->ratchet_indicator==1)
			{
				if((int)ConstVector[MONTH_BL][t]==this->ModelPoint->Calc_Deferal_Duration_Months)
					ConstVector[IND_RATCHET_MONTH_BL][t]=1;
				else if(this->Assumption->Use_bonus_to_incr_coupon_payment==1)
					ConstVector[IND_RATCHET_MONTH_BL][t]=ConstVector[IND_BONUS_VALUATION_DATE_BL][t];
				else
					ConstVector[IND_RATCHET_MONTH_BL][t]=0;
			}
			else
			{
				ConstVector[IND_RATCHET_MONTH_BL][t]=(this->Assumption->Use_bonus_to_incr_coupon_payment==1)*ConstVector[IND_BONUS_VALUATION_DATE_BL][t];
			}
			ConstVector[GROSS_PREMIUM_BL][t]=this->ModelPoint->Premium_Paid*ConstVector[IND_CONTRACT_PERIOD_BL][t];

			ConstVector[IND_COUPON_DATE_BL][t]=(int)((1-ConstVector[IND_DEFERAL_PERIOD_BL][t])*ConstVector[IND_CONTRACT_PERIOD_BL][t]);
			if(this->ModelPoint->Calc_Freq_Pay==1){
				if(ConstVector[DURATION_MONTH_BL][t]+1!=12)
					ConstVector[IND_COUPON_DATE_BL][t]=0;		
			}
			else if(this->ModelPoint->Calc_Freq_Pay==2){
				if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=5))
					ConstVector[IND_COUPON_DATE_BL][t]=0;			
			}
			else if(this->ModelPoint->Calc_Freq_Pay==3){
				if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=4)
					&&(ConstVector[DURATION_MONTH_BL][t]+1!=8))
					ConstVector[IND_COUPON_DATE_BL][t]=0;
			}
			else if(this->ModelPoint->Calc_Freq_Pay==4){
				if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=3)
					&&(ConstVector[DURATION_MONTH_BL][t]+1!=6)&&(ConstVector[DURATION_MONTH_BL][t]+1!=9))
					ConstVector[IND_COUPON_DATE_BL][t]=0;
			}
			
			if((this->Assumption->use_bonus_option==1) && this->Assumption->Use_bonus_to_incr_coupon_payment==0)
			{
				if(this->Assumption->use_guarantee_bonus_option==1)
					ConstVector[IND_BONUS_DATE_BL][t]=ConstVector[IND_COUPON_DATE_BL][t];
				else
					ConstVector[IND_BONUS_DATE_BL][t]=ConstVector[IND_BONUS_VALUATION_DATE_BL][t];
			}
			if (t==1)
				ConstVector[NUMBER_COUPON_PAID][t]= (int)(ConstVector[IND_CONTRACT_PERIOD_BL][t]*
							max(this->ModelPoint->Calc_Duration_Months-this->ModelPoint->Calc_Initial_Deferal_Duration_Months,0)
							/12*this->ModelPoint->Calc_Freq_Pay);
			else if(t>1)
				ConstVector[NUMBER_COUPON_PAID][t]=(int)(ConstVector[IND_CONTRACT_PERIOD_BL][t]*(ConstVector[NUMBER_COUPON_PAID][t-1]+ConstVector[IND_COUPON_DATE_BL][t-1]));
		}
	}
	void Calculate_ConstVector_decrements()
	{
		double local_surival_t_bef=1.;
		double local_surival_t=0.;
		double local_nur_1VP_t_bef=0.;
		double local_nur_1VP_t=0.;
		double local_nur_2VP_t_bef=0.;
		double local_nur_2VP_t=0.;
		double local_Alle_Verbleibend_t_bef=1.;
		double local_Alle_Verbleibend_t=0.;
		double local_Alle_Tote=0.;
		ConstVector[SRVIVAL_WITHOUT_LAPSE][0]=1.;

		this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_1,this->ModelPoint->Calc_Age_1,this->ModelPoint->Calc_Valn_Year,
			this->ModelPoint->Calc_Remaining_Months,ConstVector[DATE_YEAR_BL],ConstVector[DEATH_R_1_BL],ConstVector[AGE_YEAR_BL],
			this->ModelPoint->Birthdate_1,this->ModelPoint->Date_death_1);
		
		this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_2,this->ModelPoint->Calc_Age_2,this->ModelPoint->Calc_Valn_Year,
			this->ModelPoint->Calc_Remaining_Months,ConstVector[DATE_YEAR_BL],ConstVector[DEATH_R_2_BL],ConstVector[AGE_YEAR_BL],
			this->ModelPoint->Birthdate_2,this->ModelPoint->Date_death_2);

		for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			local_surival_t=local_surival_t_bef*(1.-ConstVector[DEATH_R_1_BL][t]-ConstVector[DEATH_R_2_BL][t]+
				ConstVector[DEATH_R_1_BL][t]*ConstVector[DEATH_R_2_BL][t]);

			local_nur_1VP_t=local_surival_t_bef*ConstVector[DEATH_R_2_BL][t]*(1.-ConstVector[DEATH_R_1_BL][t])+
				local_nur_1VP_t_bef*(1.-ConstVector[DEATH_R_1_BL][t]);

			local_nur_2VP_t=local_surival_t_bef*ConstVector[DEATH_R_1_BL][t]*(1.-ConstVector[DEATH_R_2_BL][t])+
				local_nur_2VP_t_bef*(1.-ConstVector[DEATH_R_2_BL][t]);

			local_Alle_Verbleibend_t=local_surival_t+local_nur_1VP_t+local_nur_2VP_t;

			local_Alle_Tote=local_surival_t_bef*ConstVector[DEATH_R_1_BL][t]*ConstVector[DEATH_R_2_BL][t]+
				local_nur_1VP_t_bef*ConstVector[DEATH_R_1_BL][t]+local_nur_2VP_t_bef*ConstVector[DEATH_R_2_BL][t];

			if(local_Alle_Verbleibend_t_bef!=0.)
				ConstVector[DEATH_R_BL][t]=local_Alle_Tote/local_Alle_Verbleibend_t_bef;
			else
				ConstVector[DEATH_R_BL][t]=1.;

			local_surival_t_bef=local_surival_t;
			local_nur_1VP_t_bef=local_nur_1VP_t;
			local_nur_2VP_t_bef=local_nur_2VP_t;
			local_Alle_Verbleibend_t_bef=local_Alle_Verbleibend_t;

			ConstVector[SRVIVAL_WITHOUT_LAPSE][t]=ConstVector[SRVIVAL_WITHOUT_LAPSE][t-1]*(1.-ConstVector[DEATH_R_BL][t]);

		}

		this->Assumption->Get_base_lapse_vector(this->ModelPoint->Calc_Remaining_Months,ConstVector[LAPSE_RATE_BL],( (this->ModelPoint->Duration_Month-this->ModelPoint->Calc_Initial_Deferal_Duration_Months)>12));		
	}

	void InitProjection()
	{
		if(GenerateDebugFiles)
			set_DebugVector_to_zero();
		set_ConstVector_to_zero();
		Calculate_ConstVector_timing();
		Calculate_ConstVector_guarantees();
		Calculate_ConstVector_decrements();
		
	}
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	virtual void projection_base_contract(double **pvs,int curr_shock_number)
	{
		survival_index=0.;
		account_value=0.;
		gmwb_guarantee=0.;
		coupon_value=0.;
		bonus_value=0.;
		hedge_cost_fees=0.;
		lapse=0.;
		mortality=0.;
		claims_wb_t=0.;
		year_bonus_valuation=0.;
		gmdb_value=0.;
		claims_db_t=0.;
		basis_capital=0.;
		account_value_1=0.;
		lapse_val=0.;
		initial_basis_capital=0.;
		fund_fees=0.;
		add_coupon_from_ratchet=0.;
		Performance=0.;
		annuity_per_freq=0.;
		annuity_per_freq_bef=0.;
		survival_index_by_step=0.;
		fixed_charges=0.;
		discount_mortality=0.;
		lapse_temp=0.;
		lapse_first_year=0.;

		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==0)
			{
				account_value=this->ModelPoint->Av0*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
				survival_index=1.;
				survival_index_by_step=1.;
				claims_wb_t=0.;
				if(this->Assumption->use_BD_guarantee==1) gmdb_value=this->ModelPoint->Rollup_GMDB;
				basis_capital=this->ModelPoint->Basis_Kapital;
				annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
				add_coupon_from_ratchet=0.;
				Performance=0.;
				if((this->Assumption->use_bonus_option==1)&&(this->Assumption->Use_bonus_to_incr_coupon_payment==0))
				{
					if((this->Assumption->use_guarantee_bonus_option==1)&&(ConstVector[DURATION_YEAR_BL][t]<(this->Assumption->years_guarantee_bonus+this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12-1)))
						bonus_value=this->ModelPoint->Bonus_Rate*this->ModelPoint->Premium_Paid*survival_index;
					else
						bonus_value=min(this->ModelPoint->Bonus_Rate*this->ModelPoint->Premium_Paid*survival_index,
								Performance*this->Assumption->Percentage_perf);
				}
				bonus_value*=ConstVector[IND_BONUS_DATE_BL][t];

				for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
				{
					discount_mortality+= ConstVector[SRVIVAL_WITHOUT_LAPSE][i]*MainProjArrayVector[DISCOUNT_FACTOR_BL][i]*ConstVector[IND_COUPON_DATE_BL][i];
				}

				//Lapse
				if(t>0) discount_mortality-=ConstVector[SRVIVAL_WITHOUT_LAPSE][t-1]*MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1]*ConstVector[IND_COUPON_DATE_BL][t-1];
				
				if(ConstVector[SRVIVAL_WITHOUT_LAPSE][t]!=0)
					itm=max(0,account_value-max(account_value*this->Assumption->Lapse_charges,this->Assumption->min_surrender_charge_amount))/
						(annuity_per_freq*((discount_mortality/ConstVector[SRVIVAL_WITHOUT_LAPSE][t])/MainProjArrayVector[DISCOUNT_FACTOR_BL][t]));
				else
					itm=0.;

				lapse_first_year=(ConstVector[DURATION_YEAR_BL][t]==0) * this->Assumption->min_lapse_y1 + (ConstVector[DURATION_YEAR_BL][t]!=0) * this->Assumption->min_lapse;
				if( (itm>=this->Assumption->x1) && (itm<=this->Assumption->x2) )
					lapse_temp=this->Assumption->Best_estimate_lapse*this->Assumption->y1;
				else if( (itm>this->Assumption->x2) && (itm<this->Assumption->x3) )
					lapse_temp= this->Assumption->y1+(this->Assumption->y2 - this->Assumption->y1)*(itm - this->Assumption->x2)/(this->Assumption->x3 - this->Assumption->x2 );
				else if( (itm>=this->Assumption->x3) && (itm<=this->Assumption->x4) )
					lapse_temp=this->Assumption->y2;
				else if( (itm>this->Assumption->x4) && (itm<this->Assumption->x5) )
					lapse_temp= this->Assumption->y2+(this->Assumption->y3 - this->Assumption->y2)*(itm - this->Assumption->x4)/(this->Assumption->x5 - this->Assumption->x4 );
				else
					lapse_temp=this->Assumption->y3;
				
				lapse_temp=this->Assumption->Best_estimate_lapse*lapse_temp;
				dynamic_lapse_rate=min(1., max(lapse_first_year,lapse_temp));

				//gmwb_guarantee=max(this->ModelPoint->Rollup_GMWB,this->ModelPoint->Ratchet_GMWB);
				//year_bonus_valuation=this->ModelPoint->Last_Bonus_Valuation;	
			}
			else
			{
				fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m;
				account_value*=(1.+MainProjArrayVector[FUND_RETURN_BL][t]-this->ModelPoint->Calc_Fund_Fees_p_m-this->ModelPoint->Calc_TER_p_m);
					
				if(this->Assumption->use_lapse==1){
					if(this->Assumption->Dynamic_lapse_Y_N==1){
						if(t==1) lapse = dynamic_lapse_rate/12.;
						else lapse=this->Assumption->weight_lapse * lapse + (1. - this->Assumption->weight_lapse) * dynamic_lapse_rate/12.;
					}
					else
						lapse=ConstVector[LAPSE_RATE_BL][t];
				}
				else
					lapse=0.;

				lapse=lapse*(account_value!=0.);
				//lapse=ConstVector[LAPSE_RATE_BL][t]*(account_value!=0.);
				lapse_val=account_value*lapse;
				account_value*=(1.-lapse);
				
				
				if(this->Assumption->use_BD_guarantee==1)
				{
					if (this->Assumption->DB_Mode==1)
					{
						gmdb_value=max(gmdb_value,0.)*(1.-lapse);
					}
					else
					{
						if((int)ConstVector[IND_DEFERAL_PERIOD_BL][t]==1)
							gmdb_value=max(gmdb_value,0.)*(1.-lapse);
						else
							gmdb_value=max(this->Assumption->DB_guarantee_period*this->ModelPoint->Calc_Freq_Pay-ConstVector[NUMBER_COUPON_PAID][t],0.)
									*max(annuity_per_freq,this->ModelPoint->Calc_Coupon_by_Freq*survival_index)*(1.-lapse);
					}
				}

				mortality=ConstVector[DEATH_R_BL][t];

				if(this->Assumption->use_BD_guarantee==1)
				{
					claims_db_t=(ConstVector[AGE_YEAR_BL][t]<this->Assumption->Max_Age_for_DB)*max(gmdb_value-account_value,0)*mortality;
					gmdb_value*=(1.-mortality);
					pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]+=claims_db_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				}		

				account_value*=(1.-mortality);
				account_value_1=account_value;
				survival_index_by_step=(1.-mortality)*(1.-lapse);
				survival_index*=survival_index_by_step;
				
				annuity_per_freq_bef=annuity_per_freq;
				annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet)*survival_index_by_step;

				account_value-=min(account_value,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL][t]);
				hedge_cost_fees=max(0.,account_value*this->ModelPoint->Calc_RRC_p_m);
				pvs[curr_shock_number][PV_HC_AV_BASE_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				pvs[curr_shock_number][PV_REAL_CHARGES_BL]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				account_value-=hedge_cost_fees;
				
				fixed_charges=min(account_value,this->ModelPoint->Fixed_Fees/12.*survival_index);
				account_value-=fixed_charges;

				initial_basis_capital=basis_capital*survival_index_by_step;
				if(this->Assumption->Reduction_BK_by_coupon==1){
					if(this->Assumption->Reduction_BK_by_ratchet==1)
						initial_basis_capital-=annuity_per_freq_bef*ConstVector[IND_COUPON_DATE_BL][t-1];
					else
						initial_basis_capital-=this->ModelPoint->Calc_Initial_Guar_Coupon_by_Freq*survival_index*ConstVector[IND_COUPON_DATE_BL][t-1];
				}
				else{
					if(this->Assumption->Reduction_BK_by_ratchet==1)
						initial_basis_capital-=annuity_per_freq_bef*ConstVector[IND_COUPON_DATE_BL][t-1]
							-this->ModelPoint->Calc_Initial_Guar_Coupon_by_Freq*survival_index*ConstVector[IND_COUPON_DATE_BL][t];			
				}		
				if(this->Assumption->Reduction_BK_by_bonus==1) initial_basis_capital-=bonus_value*ConstVector[IND_BONUS_DATE_BL][t-1];
				initial_basis_capital=max(0.,initial_basis_capital);
				
				Performance=max(0.,account_value-initial_basis_capital);
				if((this->Assumption->ratchet_indicator==1)||(this->Assumption->use_bonus_option==1))
				{
					add_coupon_from_ratchet=Performance*this->ModelPoint->Calc_Withdrawals_rate/
							this->ModelPoint->Calc_Freq_Pay*ConstVector[IND_RATCHET_MONTH_BL][t];

					if( ((int)ConstVector[MONTH_BL][t]>this->ModelPoint->Calc_Deferal_Duration_Months))
					{
						add_coupon_from_ratchet*=this->Assumption->Percentage_perf_2*((this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12)<this->Assumption->Min_def_period_perf_1) +
							((this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12)>=this->Assumption->Min_def_period_perf_1)*this->Assumption->Percentage_perf;
					}
					//add_coupon_from_ratchet=Performance*this->ModelPoint->Calc_Withdrawals_rate/
					//		this->ModelPoint->Calc_Freq_Pay*ConstVector[IND_RATCHET_MONTH_BL][t]*
					//		(((int)ConstVector[MONTH_BL][t]>this->ModelPoint->Calc_Deferal_Duration_Months)*this->Assumption->Percentage_perf+((int)ConstVector[MONTH_BL][t]<=this->ModelPoint->Calc_Deferal_Duration_Months));
				}
				if((this->Assumption->use_bonus_option==1)&&(this->Assumption->Use_bonus_to_incr_coupon_payment==0))
				{
					if((this->Assumption->use_guarantee_bonus_option==1)&&(ConstVector[DURATION_YEAR_BL][t]<(this->Assumption->years_guarantee_bonus+this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12-1)))
						bonus_value=this->ModelPoint->Bonus_Rate*this->ModelPoint->Premium_Paid*survival_index;
					else
						bonus_value=min(this->ModelPoint->Bonus_Rate*this->ModelPoint->Premium_Paid*survival_index,
								Performance*(this->Assumption->Percentage_perf_2*((this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12)<this->Assumption->Min_def_period_perf_1) +
							((this->ModelPoint->Calc_Initial_Deferal_Duration_Months/12)>=this->Assumption->Min_def_period_perf_1)*this->Assumption->Percentage_perf));
				}			
				bonus_value*=ConstVector[IND_BONUS_DATE_BL][t];

				account_value-=min(account_value,bonus_value)*ConstVector[IND_BONUS_DATE_BL][t];
				account_value=max(0.,account_value);
				
				//Lapse
				if(t>0) discount_mortality-=ConstVector[SRVIVAL_WITHOUT_LAPSE][t-1]*MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1]*ConstVector[IND_COUPON_DATE_BL][t-1];
				if(ConstVector[SRVIVAL_WITHOUT_LAPSE][t]!=0)
					itm=max(0,account_value-max(account_value*this->Assumption->Lapse_charges,this->Assumption->min_surrender_charge_amount))/
						(annuity_per_freq*((discount_mortality/ConstVector[SRVIVAL_WITHOUT_LAPSE][t])/MainProjArrayVector[DISCOUNT_FACTOR_BL][t]));
				else
					itm=0.;
				lapse_first_year=(ConstVector[DURATION_YEAR_BL][t]==0) * this->Assumption->min_lapse_y1 + (ConstVector[DURATION_YEAR_BL][t]!=0) * this->Assumption->min_lapse;
				if( (itm>=this->Assumption->x1) && (itm<=this->Assumption->x2) )
					lapse_temp=this->Assumption->Best_estimate_lapse*this->Assumption->y1;
				else if( (itm>this->Assumption->x2) && (itm<this->Assumption->x3) )
					lapse_temp= this->Assumption->y1+(this->Assumption->y2 - this->Assumption->y1)*(itm - this->Assumption->x2)/(this->Assumption->x3 - this->Assumption->x2 );
				else if( (itm>=this->Assumption->x3) && (itm<=this->Assumption->x4) )
					lapse_temp=this->Assumption->y2;
				else if( (itm>this->Assumption->x4) && (itm<this->Assumption->x5) )
					lapse_temp= this->Assumption->y2+(this->Assumption->y3 - this->Assumption->y2)*(itm - this->Assumption->x4)/(this->Assumption->x5 - this->Assumption->x4 );
				else
					lapse_temp=this->Assumption->y3;
				lapse_temp=this->Assumption->Best_estimate_lapse*lapse_temp;
				dynamic_lapse_rate=min(1., max(lapse_first_year,lapse_temp));

							
				if(ConstVector[IND_RATCHET_MONTH_BL][t]==1)
					basis_capital=max(initial_basis_capital,account_value);
				else
					basis_capital=initial_basis_capital;

				if(this->Assumption->use_BD_guarantee==1)
				{
					if (this->Assumption->DB_Mode==1)
					{
						gmdb_value-=(bonus_value+annuity_per_freq*ConstVector[IND_COUPON_DATE_BL][t]);
						gmdb_value=max(gmdb_value,0.);
					}
				}
				
				claims_wb_t=max(0.,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL][t]+bonus_value-account_value_1);				
				pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];

				pvs[curr_shock_number][PV_FUND_CHARGES_BL]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				pvs[curr_shock_number][PV_FIXED_CHARGES_BL]+=fixed_charges*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				pvs[curr_shock_number][PV_LAPSE_CHARGES_BL]+=lapse_val*this->Assumption->Lapse_charges*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
		
	
			}
			if(GenerateDebugFiles) //If debuging the porjection : not used in production
			{
				this->set_value_on_debug_file(this->gmdb_value,GUAR_GMDB_BL, t);
				this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_BL, t);
				this->set_value_on_debug_file(this->claims_db_t,CLAIMS_DB_BL, t);
				this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_BL, t);
				this->set_value_on_debug_file(this->basis_capital,BASIS_KAPITAL_BL, t);	
				this->set_value_on_debug_file(this->bonus_value,BONUS_VALUE_BL, t);
				this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_BL, t);
				this->set_value_on_debug_file(this->annuity_per_freq,COUPON_VALUE_BL, t);
				this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_BL, t);
				this->set_value_on_debug_file(this->account_value_1,ACCOUNT_VALUE_BEF_COUPON_BL, t);
				this->set_value_on_debug_file(this->initial_basis_capital,INITIAL_BASIS_CAPITAL_BL, t);
				this->set_value_on_debug_file(this->add_coupon_from_ratchet,ADD_COUPON_RATCHET_BL, t);
				this->set_value_on_debug_file(this->survival_index_by_step,SURVIVAL_INDEX_STEP_BL, t);
				this->set_value_on_debug_file(this->Performance,PERFORMANCE_BL, t);	
				this->set_value_on_debug_file(this->lapse,DYNAMIC_LAPSE, t);	

			}		
		}
	}
	void PresentValueCalculation(double **pvs,int curr_shock_number,bool delta)
	{
		// Calculate account value at the beginning : BEGINING_ACCOUNT_VALUE 0
		pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av0;
		//Calculate claims :PV_CLAIMS_TOT_BL 5
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]=pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]+
			pvs[curr_shock_number][PV_CLAIMS_GMAB_BL]+pvs[curr_shock_number][PV_CLAIMS_GMIB_BL];	
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMAB_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMIB_BL]/=(this->total_number_scn);
		//Calculate real ride charges :PV_RRC 6
		pvs[curr_shock_number][PV_REAL_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_HC_AV_BASE_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_RRC_BL]=this->ModelPoint->Rrc;	
		//Calculate EHC value : PV_EHC_BL 7
		if((delta == true && curr_shock_number==0) || delta==false)
		{
			if(this->ModelPoint->New_Business)
				pvs[curr_shock_number][PV_EHC_BL]=pvs[curr_shock_number][PV_RRC_BL]*pvs[curr_shock_number][PV_CLAIMS_TOT_BL]/
						(pvs[curr_shock_number][PV_REAL_CHARGES_BL]*(pvs[curr_shock_number][PV_REAL_CHARGES_BL]!=0)+0.01*(pvs[curr_shock_number][PV_REAL_CHARGES_BL]==0))/*(this->total_number_scn)*/;	
			else
				pvs[curr_shock_number][PV_EHC_BL]=this->ModelPoint->Ehc/*(this->total_number_scn)*/;
		}
		else
			pvs[curr_shock_number][PV_EHC_BL]=this->ModelPoint->index_shock_pvs[0][PV_EHC_BL];
		//Calculate EHC Charges : PV_EHC_CHARGES_BL 10
		
		pvs[curr_shock_number][PV_EHC_CHARGES_BL]=pvs[curr_shock_number][PV_EHC_BL]*pvs[curr_shock_number][PV_HC_AV_BASE_BL]/12.;
		//Calculate LOV :PV_LOV_DEU 11
		pvs[curr_shock_number][PV_LOV_BL]=pvs[curr_shock_number][PV_CLAIMS_TOT_BL]-pvs[curr_shock_number][PV_EHC_CHARGES_BL];
		//Calculate LOV :PV_ITM_BL 12
		pvs[curr_shock_number][PV_ITM_BL]=this->ModelPoint->Av0/(this->ModelPoint->Rollup_GMWB+this->ModelPoint->Ratchet_GMWB);
		
	}
	void project()
	{
		Projection::compt_sen +=1;
		
		// Loop over the indexes shocks
		set_MainProjArrayVector_to_zero();
		init_market_data();
		for(int sh_n = 0; sh_n < this->ShockSize; sh_n++)
		{	
			this->ShockNumber = sh_n;
		
			this->projection_base_contract(this->ModelPoint->index_shock_pvs, this->ShockNumber);

			if(GenerateDebugFiles)
				this->Write_Debug_File();

			if(Projection::compt_sen==this->total_number_scn)
				this->PresentValueCalculation(this->ModelPoint->index_shock_pvs, this->ShockNumber,true);
		}
		
	}
};