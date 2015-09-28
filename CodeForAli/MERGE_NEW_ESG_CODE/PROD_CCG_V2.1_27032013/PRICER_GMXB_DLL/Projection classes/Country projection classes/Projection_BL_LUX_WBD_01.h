#pragma once
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "Model point classes/Country model point classes/model_point_BL_LUX_01.h"
#include "head_BL_LUX.h"
#include "head_BL.h"
using namespace std;

extern ofstream trace;
class Assumption_BL_LUX_GMWBD_01 
{
	public:
		// Assumptions tables------------------------------------------------
		GMXBParameters  *assump;
		GMXB_Assumption_Table *mortality_table;
		
		int Ratchet_Y_N;
		double Bonus_Rate;
		int Mortality_Y_N;
		int Max_Age_Mortality;
		int Use_Lapse_Y_N;
		double Base_Static_Lapse;
		double Base_Static_Lapse_p_m;
		int Dynamic_Lapse_Y_N;
		double Best_Estimate_Lapse;
		double Min_Lapse_Y_1;
		double Min_Lapse;
		double X_1;
		double X_2;
		double X_3;
		double X_4;
		double X_5;
		double Y_1;
		double Y_2;
		double Y_3;
		double Weight;
		int Mortality_Lapse_Y_N;
		double Lapse_Charges;
		int Max_Age_DB;
		double Min_Surrender_Charge_Amount;
		double Retrocessions_Fund;

		int index_1_local;
		int index_2_local;
		double mort_value_local;
		double dyn_lapse_local;
		double param_local1;
		double param_local2;



		

public:
	virtual ~Assumption_BL_LUX_GMWBD_01(){
		delete this->assump;
		delete this->mortality_table;
	}
	Assumption_BL_LUX_GMWBD_01(){}
	Assumption_BL_LUX_GMWBD_01(const char * tables)
	{
		// Assumptions tables------------------------------------------------

		map<string, GMXB_Assumption_Table*> sheet_tables;
		sheet_tables["mortality_table"] = 0;
		assump = parse_sheets(string(tables), sheet_tables, string("assump") );
		mortality_table=sheet_tables["mortality_table"];

		Ratchet_Y_N= atoi(this->assump->search("Ratchet_Y_N").c_str());
		Bonus_Rate=atof(this->assump->search("Bonus_Rate").c_str());
		Mortality_Y_N= atoi(this->assump->search("Mortality_Y_N").c_str());
		Max_Age_Mortality= atoi(this->assump->search("Max_Age_Mortality").c_str());
		Use_Lapse_Y_N= atoi(this->assump->search("Use_Lapse_Y_N").c_str());
		Base_Static_Lapse=atof(this->assump->search("Base_Static_Lapse").c_str());
		Dynamic_Lapse_Y_N= atoi(this->assump->search("Dynamic_Lapse_Y_N").c_str());
		Best_Estimate_Lapse=atof(this->assump->search("Best_Estimate_Lapse").c_str());
		Min_Lapse_Y_1=atof(this->assump->search("Min_Lapse_Y_1").c_str());
		Min_Lapse=atof(this->assump->search("Min_Lapse").c_str());
		X_1=atof(this->assump->search("X_1").c_str());
		X_2=atof(this->assump->search("X_2").c_str());
		X_3=atof(this->assump->search("X_3").c_str());
		X_4=atof(this->assump->search("X_4").c_str());
		X_5=atof(this->assump->search("X_5").c_str());
		Y_1=atof(this->assump->search("Y_1").c_str());
		Y_2=atof(this->assump->search("Y_2").c_str());
		Y_3=atof(this->assump->search("Y_3").c_str());
		Weight=atof(this->assump->search("Weight").c_str());
		Mortality_Lapse_Y_N= atoi(this->assump->search("Mortality_Lapse_Y_N").c_str());
		Lapse_Charges=atof(this->assump->search("Lapse_Charges").c_str());
		Max_Age_DB= atoi(this->assump->search("Max_Age_DB").c_str());
		Min_Surrender_Charge_Amount=atof(this->assump->search("Min_Surrender_Charge_Amount").c_str());
		Base_Static_Lapse_p_m=Base_Static_Lapse/12.;
		Retrocessions_Fund=atof(this->assump->search("Retrocessions_Fund").c_str());

		index_1_local=0;
		index_2_local=0;
		mort_value_local=0.;
		dyn_lapse_local=0.;
		param_local1=(this->Y_2-this->Y_1)/(this->X_3-this->X_2);
		param_local2=(this->Y_3-this->Y_2)/(this->X_5-this->X_4);
	}
	double Get_mortality_rates_vector(const string& sex,const int& current_age)
	{
		this->index_2_local=(sex == "MALE")+(sex != "MALE")*2;
		this->index_1_local=current_age;
		this->mort_value_local=this->mortality_table->search(this->index_1_local,this->index_2_local);
		this->mort_value_local=1.-exp(log(1.-this->mort_value_local)/12.);
		return this->mort_value_local;
	}	
	double Get_Dynamic_Lapse_Value(double moneyness,int current_duration_year)
	{
		if(Use_Lapse_Y_N==1)
		{
			if(Dynamic_Lapse_Y_N==1)
			{
				this->dyn_lapse_local=this->Best_Estimate_Lapse;
				if(moneyness>=this->X_5)
					this->dyn_lapse_local*=this->Y_3;
				else if(moneyness>this->X_4)
					this->dyn_lapse_local*=this->Y_2+this->param_local2*(moneyness-this->X_4);
				else if(moneyness>=this->X_3)
					this->dyn_lapse_local*=this->Y_2;
				else if(moneyness>this->X_2)
					this->dyn_lapse_local*=this->Y_1+this->param_local1*(moneyness-this->X_2);
				else if(moneyness>=this->X_1)
					this->dyn_lapse_local*=this->Y_1;
				else
					this->dyn_lapse_local*=this->Y_3;
		
				if(current_duration_year==0)
					this->dyn_lapse_local=max(this->Min_Lapse_Y_1,this->dyn_lapse_local);
				else
					this->dyn_lapse_local=max(this->Min_Lapse,this->dyn_lapse_local);

				this->dyn_lapse_local=min(1.,this->dyn_lapse_local);
				return this->dyn_lapse_local/12.;
			}
			else
				return this->Base_Static_Lapse_p_m;
		}
		else
				return 0.;
	}
};
class Projection_BL_LUX_GMWBD_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LUX_GMWBD_01 *Assumption;

	//Model Point Class
	Model_Point_LUX_01 *ModelPoint;
	
	//Tables used in the projection
	double ConstVector[TOTAL_CONSTVECTOR_COLS_BL_LUX][SUBACCT_MAX_PERIOD_MTH +1];
	double MainProjArrayVector[MAIN_PROJ_NUM_COLS_BL][SUBACCT_MAX_PERIOD_MTH +1];
	double DebugVector[DEBUG_NUM_COLS_BL_LUX][SUBACCT_MAX_PERIOD_MTH +1];

	//Other Assumptions
	bool GenerateDebugFiles;
	string DbgFilePath;
	int ** ShockArray;
	int ShockSize;		
	int ShockNumber;
	int CurrentScen;

	double account_value;
	double survival_index;
	double survival_index_by_step;
	double claims_wb_t;
	double claims_db_t;
	double basis_capital;
	double GMDB_level;
	double annuity_per_freq;	
	double add_coupon_from_ratchet;
	double Performance_bonus_valuation;
	double bonus_value;
	double fund_fees;
	double lapse;
	double mortality_rate;
	double sumprod_dynamic_lapse;
	double dynamic_lapse_ratio;
	double lapse_val;
	double account_value_1;
	double hedge_cost_fees;
	double fixed_charges;




	~Projection_BL_LUX_GMWBD_01()
	{		
		delete this->Assumption;
		for(int i = 0; i <this->ShockSize; i++){
			if(this->ShockArray[i]!=0)
				delete[] this->ShockArray[i];
		}
		if (this->ShockArray!=0)
			delete[] this->ShockArray;	
	}
	Projection_BL_LUX_GMWBD_01( const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
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
		this->Assumption =new Assumption_BL_LUX_GMWBD_01(tables);
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
		account_value=0.;
		survival_index=0.;
		survival_index_by_step=0.;
		claims_wb_t=0.;
		claims_db_t=0.;
		basis_capital=0.;
		GMDB_level=0.;
		annuity_per_freq=0.;	
		add_coupon_from_ratchet=0.;
		Performance_bonus_valuation=0.;
		bonus_value=0.;
		fund_fees=0.;
		lapse=0.;
		mortality_rate=0.;
		sumprod_dynamic_lapse=0.;
		dynamic_lapse_ratio=0.;
		lapse_val=0.;
		account_value_1=0.;
		hedge_cost_fees=0.;
		fixed_charges=0.;
		UNREFERENCED_PARAMETER(f_s_a);
	}

	virtual void Write_Debug_File()
	{
		char debug_file_name[MAX_LENGTH_FILE_NAME];
		sprintf_s(debug_file_name, "%smain_array_mp%d_scn%d.csv" ,this->DbgFilePath.c_str(), this->ModelPoint->mp_position_in_file, this->CurrentScen);
		this->debug_file.open(debug_file_name);
		this->debug_file << "TIME,DURATION_MONTH_BL_LUX,DURATION_YEAR_BL_LUX,AGE_BL_LUX,IND_CONTRACT_PERIOD_BL_LUX,IND_DEFERAL_PERIOD_BL_LUX,IND_RATCHET_MONTH_BL_LUX,IND_COUPON_DATE_BL_LUX,IND_BONUS_VALUATION_DATE_BL_LUX,IND_BONUS_PAYMENT_DATE_BL_LUX,IND_DB_PERIOD_BL_LUX,ALLE_VERBLEIBEND_BL_LUX,DEATH_R_BL_LUX,";
		this->debug_file << "YEAR_BL_LUX,MONTH_BL_LUX,DATE_YEAR_BL_LUX,DATE_MONTH_BL_LUX,FUND_RETURN_BL,DISCOUNT_FACTOR_BL,ACCOUNT_VALUE_END_BL_LUX,ACCOUNT_VALUE_BEF_MORT_BL_LUX,SURVIVAL_INDEX_STEP_BL_LUX,SURVIVAL_INDEX_BL_LUX,CLAIMS_WB_BL_LUX,CLAIMS_DB_BL_LUX,";
		this->debug_file << "BASIS_KAPITAL_BL_LUX,GMDB_BL_LUX,COUPON_VALUE_BL_LUX,ADD_COUPON_RATCHET_BL_LUX,PERFORMANCE_BONUS_VAL_BL_LUX,BONUS_VAL_BL_LUX,FUND_FEES_VAL_BL_LUX,DYNAMIC_LAPSE_LUX,ITM_VAL_BL_LUX,LAPSE_VAL_BL_LUX,HEDGE_COST_FEES_BL_LUX,FIXED_CHARGE_VALUE_BL_LUX,";
		this->debug_file << endl;

		for ( int t=0; t<= this->ModelPoint->Calc_Remaining_Months; t++)
		{
			this->debug_file <<setprecision(15)<< t << ",";
			for(int col = 0; col < TOTAL_CONSTVECTOR_COLS_BL_LUX ; col++)
				this->debug_file <<setprecision(15)<< this->ConstVector[col][t] << ",";
			for(int col = 0; col < MAIN_PROJ_NUM_COLS_BL ; col++)
				this->debug_file << setprecision(15)<< this->MainProjArrayVector[col][t] << ",";
			for(int col = 0; col < DEBUG_NUM_COLS_BL_LUX ; col++)
				this->debug_file << setprecision(15)<< this->DebugVector[col][t] << ",";
			this->debug_file << endl;
		}
		this->debug_file.close();
	}	
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
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
		MainProjArrayVector[FUND_RETURN_BL][1]*=this->ModelPoint->Calc_Prorata;
	}
	void set_pvsScenOutput(Model_Point& mp,  unsigned int nScenPath_ip)
	{
	   	char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "DEU don't support scen_by_scen output");
		throw new GMXBException(message);	
		UNREFERENCED_PARAMETER(nScenPath_ip);
		UNREFERENCED_PARAMETER(mp);
	}
	void set_current_model_point(Model_Point *mp)
	{
		this->ModelPoint = dynamic_cast<Model_Point_LUX_01*>(mp);
		this->initialise_mp_calculation();
		this->InitProjection();
	}
	void set_value_on_debug_file ( double value, int row, int t){DebugVector[row][t]=value;}
	virtual void project_t(int t, double ** pvs, int curr_shock_nb, double lapse_mult, double mortality_mult, bool shock_av) 
	{
		UNREFERENCED_PARAMETER(t);
		UNREFERENCED_PARAMETER(pvs);
		UNREFERENCED_PARAMETER(curr_shock_nb);
		UNREFERENCED_PARAMETER(lapse_mult);
		UNREFERENCED_PARAMETER(mortality_mult);
		UNREFERENCED_PARAMETER(shock_av);


	
	}
	void set_DebugVector_to_zero(){memset(this->DebugVector, 0, sizeof(this->DebugVector));}
	void set_ConstVector_to_zero(){memset(this->ConstVector, 0, sizeof(this->ConstVector));}
	void set_MainProjArrayVector_to_zero(){memset(this->MainProjArrayVector, 0, sizeof(this->MainProjArrayVector));}
	virtual void initialise_index_map(){}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
	virtual void initialise_mp_calculation()
	{
		int local_val=0;
		local_val=atoi(this->ModelPoint->Valn_Date_Lux.c_str());
		this->ModelPoint->Calc_Valn_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Valn_Month=(int)((local_val-10000*this->ModelPoint->Calc_Valn_Year)/100);
		this->ModelPoint->Calc_Valn_Day=(int)(local_val-10000*this->ModelPoint->Calc_Valn_Year-100*this->ModelPoint->Calc_Valn_Month);
		
		local_val=atoi(this->ModelPoint->Issue_Date_Lux.c_str());
		this->ModelPoint->Calc_IssueDate_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_IssueDate_Month=(int)((local_val-10000*this->ModelPoint->Calc_IssueDate_Year)/100);
		this->ModelPoint->Calc_IssueDate_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_IssueDate_Month);
				
		if(this->ModelPoint->Date_Birth_1_Lux!=""){
			local_val=atoi(this->ModelPoint->Date_Birth_1_Lux.c_str());
			this->ModelPoint->Calc_Birth_1_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birth_1_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birth_1_Date_Year)/100);
			this->ModelPoint->Calc_Birth_1_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birth_1_Date_Year-100*this->ModelPoint->Calc_Birth_1_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Birth_1_Date_Year=0;
			this->ModelPoint->Calc_Birth_1_Date_Month=0;
			this->ModelPoint->Calc_Birth_1_Date_Day=0;
		}

		if(this->ModelPoint->Date_Birth_2_Lux!=""){
			local_val=atoi(this->ModelPoint->Date_Birth_2_Lux.c_str());
			this->ModelPoint->Calc_Birth_2_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birth_2_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birth_2_Date_Year)/100);
			this->ModelPoint->Calc_Birth_2_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birth_2_Date_Year-100*this->ModelPoint->Calc_Birth_2_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Birth_2_Date_Year=0;
			this->ModelPoint->Calc_Birth_2_Date_Month=0;
			this->ModelPoint->Calc_Birth_2_Date_Day=0;
		}
		
		if(this->ModelPoint->Date_Death_1_Lux!=""){
			local_val=atoi(this->ModelPoint->Date_Death_1_Lux.c_str());
			this->ModelPoint->Calc_Death_1_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Death_1_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Death_1_Date_Year)/100);
			this->ModelPoint->Calc_Death_1_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Death_1_Date_Year-100*this->ModelPoint->Calc_Death_1_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Death_1_Date_Year=0;
			this->ModelPoint->Calc_Death_1_Date_Month=0;
			this->ModelPoint->Calc_Death_1_Date_Day=0;
		}
		
		if(this->ModelPoint->Date_Death_2_Lux!=""){
			local_val=atoi(this->ModelPoint->Date_Death_2_Lux.c_str());
			this->ModelPoint->Calc_Death_2_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Death_2_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Death_2_Date_Year)/100);
			this->ModelPoint->Calc_Death_2_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Death_2_Date_Year-100*this->ModelPoint->Calc_Death_2_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Death_2_Date_Year=0;
			this->ModelPoint->Calc_Death_2_Date_Month=0;
			this->ModelPoint->Calc_Death_2_Date_Day=0;
		}
		

		local_val=atoi(this->ModelPoint->Annuity_Begin_Date_Lux.c_str());
		this->ModelPoint->Calc_Annuity_Begin_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Annuity_Begin_Month=(int)((local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year)/100);
		this->ModelPoint->Calc_Annuity_Begin_Day=(int)(local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year-100*this->ModelPoint->Calc_Annuity_Begin_Month);
		
		this->ModelPoint->Calc_Duration_Months=(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_IssueDate_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_IssueDate_Month);
		this->ModelPoint->Calc_Remaining_Months=this->ModelPoint->Duration_Lux-this->ModelPoint->Calc_Duration_Months;
		this->ModelPoint->Calc_Prorata=( 31. - min(this->ModelPoint->Calc_Valn_Day,30) )/30.;
		
		if(this->ModelPoint->Calc_Birth_1_Date_Year!=0)
			this->ModelPoint->Calc_Age_1=(int)(((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birth_1_Date_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birth_1_Date_Month))/12.);
		else
			this->ModelPoint->Calc_Age_1=120;
		
		if(this->ModelPoint->Calc_Birth_2_Date_Year!=0)		
			this->ModelPoint->Calc_Age_2=(int)(((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birth_2_Date_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birth_2_Date_Month))/12.);
		else
			this->ModelPoint->Calc_Age_2=0;
		
		if(this->ModelPoint->Calc_Birth_1_Date_Year!=0)
			this->ModelPoint->Calc_Age_Month_1=((this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Birth_1_Date_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Birth_1_Date_Month))%12;	
	
		this->ModelPoint->Calc_Fund_Fees_p_m=this->ModelPoint->Fund_fees_Lux/12.;
		this->ModelPoint->Calc_Ehc_p_m=this->ModelPoint->Ehc_Lux/12.;
		this->ModelPoint->Calc_fixed_fees_p_m=0.;
		
		if(this->ModelPoint->Withdrawals_Mode_Lux=="MON")
			this->ModelPoint->Calc_Freq_Pay=12;
		else if(this->ModelPoint->Withdrawals_Mode_Lux=="QUA")
			this->ModelPoint->Calc_Freq_Pay=4;
		else if(this->ModelPoint->Withdrawals_Mode_Lux=="SEM")
			this->ModelPoint->Calc_Freq_Pay=2;
		else
			this->ModelPoint->Calc_Freq_Pay=1;

		this->ModelPoint->Calc_Deferal_Duration_Months=(this->ModelPoint->Calc_Annuity_Begin_Year-this->ModelPoint->Calc_Valn_Year)*12+(this->ModelPoint->Calc_Annuity_Begin_Month-this->ModelPoint->Calc_Valn_Month);
		
		this->ModelPoint->Calc_WB_Rollup=this->ModelPoint->Benefit_Increase_Factor_Lux*this->ModelPoint->Inv_Protection_Lux*this->ModelPoint->Insur_Prem_Lux;
		
		this->ModelPoint->Calc_Initial_Coupon_by_Freq=this->ModelPoint->Calc_WB_Rollup*this->ModelPoint->Conversion_Rate_Lux/this->ModelPoint->Calc_Freq_Pay;
		
		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Curr_Gmwb_Lux/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Mod_Def_Duration_Months=( ( (int) this->ModelPoint->Calc_Deferal_Duration_Months % 12 ) + 11 ) % 12 ;
		
		if(this->ModelPoint->Calc_Deferal_Duration_Months<=0)
			this->ModelPoint->Calc_BK0=this->ModelPoint->Base_Capital_Lux;
		else 
			this->ModelPoint->Calc_BK0=this->ModelPoint->Calc_WB_Rollup;
		//else if( (this->ModelPoint->Calc_Valn_Year*100+this->ModelPoint->Calc_Valn_Month) < (this->ModelPoint->Calc_Annuity_Begin_Year*100+this->ModelPoint->Calc_Annuity_Begin_Month) )
		//	this->ModelPoint->Calc_BK0=this->ModelPoint->Calc_WB_Rollup;
		//else
		//	this->ModelPoint->Calc_BK0=max(this->ModelPoint->Calc_WB_Rollup,this->ModelPoint->Av_Lux);

		this->ModelPoint->Calc_RRC_p_m=this->ModelPoint->Fund_Charge_Lux/12.;
		this->ModelPoint->Calc_Conversion_Rate_p_m=this->ModelPoint->Conversion_Rate_Lux/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Min_Bonus_Amount=this->ModelPoint->Insur_Prem_Lux*this->Assumption->Bonus_Rate;
		this->ModelPoint->Calc_Gaur_Charge_Rate=this->ModelPoint->Fund_Charge_Lux-this->ModelPoint->Ong_Acqui_Charge_Lux;


	}
	void Calculate_ConstVector_timing()
	{
		int age_month_local=0;
		for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++){
			
			if(t==1){
				ConstVector[DURATION_MONTH_BL_LUX][0]=(int)((this->ModelPoint->Calc_Duration_Months+11)%12);
				ConstVector[DURATION_MONTH_BL_LUX][1]=(int)(this->ModelPoint->Calc_Duration_Months%12);
				ConstVector[DURATION_YEAR_BL_LUX][1]=(int)(this->ModelPoint->Calc_Duration_Months/12.);
				ConstVector[DURATION_YEAR_BL_LUX][0]=max((int)((this->ModelPoint->Calc_Duration_Months-1)/12.),0);
				age_month_local=this->ModelPoint->Calc_Age_Month_1;
				ConstVector[AGE_BL_LUX][1]=this->ModelPoint->Calc_Age_1;
				ConstVector[AGE_BL_LUX][0]=(int)ConstVector[AGE_BL_LUX][0]-(age_month_local==0);
				ConstVector[MONTH_BL_LUX][0]=0;
				ConstVector[MONTH_BL_LUX][1]=1;
				ConstVector[YEAR_BL_LUX][1]=1;
				ConstVector[DATE_YEAR_BL_LUX][1]=this->ModelPoint->Calc_Valn_Year;
				ConstVector[DATE_MONTH_BL_LUX][1]=this->ModelPoint->Calc_Valn_Month;	
			}
			else
			{
				ConstVector[DURATION_MONTH_BL_LUX][t]=(int)((ConstVector[DURATION_MONTH_BL_LUX][t-1]!=11)*(ConstVector[DURATION_MONTH_BL_LUX][t-1]+1));
				ConstVector[DURATION_YEAR_BL_LUX][t]=(int)ConstVector[DURATION_YEAR_BL_LUX][t-1];
				if((int)ConstVector[DURATION_MONTH_BL_LUX][t]==0)
					ConstVector[DURATION_YEAR_BL_LUX][t]+=1;
				age_month_local=(age_month_local!=11)*(age_month_local+1);
				ConstVector[AGE_BL_LUX][t]=(int)ConstVector[AGE_BL_LUX][t-1]+(age_month_local==0);
				ConstVector[MONTH_BL_LUX][t]=(int)ConstVector[MONTH_BL_LUX][t-1]+1;
				ConstVector[YEAR_BL_LUX][t]=(int)(ConstVector[MONTH_BL_LUX][t-1]/12)+1;			
				ConstVector[DATE_MONTH_BL_LUX][t]=(int)((ConstVector[DATE_MONTH_BL_LUX][t-1]!=12)*(ConstVector[DATE_MONTH_BL_LUX][t-1]+1)+(ConstVector[DATE_MONTH_BL_LUX][t-1]==12));
				ConstVector[DATE_YEAR_BL_LUX][t]=ConstVector[DATE_YEAR_BL_LUX][t-1];
				if((int)ConstVector[DATE_MONTH_BL_LUX][t-1]==12)
					ConstVector[DATE_YEAR_BL_LUX][t]+=1;
			}
		}
	}
	virtual void Calculate_ConstVector_guarantees()
	{
		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			ConstVector[IND_CONTRACT_PERIOD_BL_LUX][t]=(int)(t<=this->ModelPoint->Calc_Remaining_Months);
			ConstVector[IND_DEFERAL_PERIOD_BL_LUX][t]=(int)(t<=this->ModelPoint->Calc_Deferal_Duration_Months);
			
			if ((this->Assumption->Ratchet_Y_N==1)&&(t==this->ModelPoint->Calc_Deferal_Duration_Months)&&((int)ConstVector[IND_CONTRACT_PERIOD_BL_LUX][t]==1) )
				ConstVector[IND_RATCHET_MONTH_BL_LUX][t]=1;
			else ConstVector[IND_RATCHET_MONTH_BL_LUX][t]=0;

			ConstVector[IND_COUPON_DATE_BL_LUX][t]=(int)((1-ConstVector[IND_DEFERAL_PERIOD_BL_LUX][t]));
			if(this->ModelPoint->Calc_Freq_Pay==1){
				if(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=12)
					ConstVector[IND_COUPON_DATE_BL_LUX][t]=0;		
			}
			else if(this->ModelPoint->Calc_Freq_Pay==2){
				if((ConstVector[DURATION_MONTH_BL_LUX][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=6))
					ConstVector[IND_COUPON_DATE_BL_LUX][t]=0;			
			}
			else if(this->ModelPoint->Calc_Freq_Pay==3){
				if((ConstVector[DURATION_MONTH_BL_LUX][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=4)
					&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=8))
					ConstVector[IND_COUPON_DATE_BL_LUX][t]=0;
			}
			else if(this->ModelPoint->Calc_Freq_Pay==4){
				if((ConstVector[DURATION_MONTH_BL_LUX][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=3)
					&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=6)&&(ConstVector[DURATION_MONTH_BL_LUX][t]+1!=9))
					ConstVector[IND_COUPON_DATE_BL_LUX][t]=0;
			}

			if(this->ModelPoint->Perf_Bonus_Lux =="Y"){
				if(  ((((t%12)+11)%12) == this->ModelPoint->Calc_Mod_Def_Duration_Months) &&  ( t > this->ModelPoint->Calc_Deferal_Duration_Months  )  )
					ConstVector[IND_BONUS_VALUATION_DATE_BL_LUX][t]=1;
			}
			
			ConstVector[IND_BONUS_PAYMENT_DATE_BL_LUX][t]=ConstVector[IND_BONUS_VALUATION_DATE_BL_LUX][t]*(this->ModelPoint->Perf_Bonus_Lux =="Y");
			ConstVector[IND_DB_PERIOD_BL_LUX][t]=ConstVector[IND_CONTRACT_PERIOD_BL_LUX][t]*(this->ModelPoint->Gmdb_Rider_Lux =="Y")*(this->Assumption->Max_Age_DB > ConstVector[AGE_BL_LUX][t]);
		}
	}
	virtual void Calculate_ConstVector_decrement()
	{
		double qx_month_1_local=1.;
		double qx_month_2_local=1.;
		double local_surival_t_bef=1.;
		double local_surival_t=0.;
		double local_nur_1VP_t_bef=0.;
		double local_nur_1VP_t=0.;
		double local_nur_2VP_t_bef=0.;
		double local_nur_2VP_t=0.;
		double local_Alle_Tote=0.;
		int indic_1=1;
		int indic_2=1;
		if( (this->ModelPoint->Calc_Birth_1_Date_Year==0) || (this->ModelPoint->Calc_Death_1_Date_Year!=0) )
			indic_1=0;
		if( (this->ModelPoint->Calc_Birth_2_Date_Year==0) || (this->ModelPoint->Calc_Death_2_Date_Year!=0) )
			indic_2=0;

		ConstVector[DEATH_R_BL_LUX][0]=0.;
		ConstVector[ALLE_VERBLEIBEND_BL_LUX][0]=1.;

		if(this->Assumption->Mortality_Y_N==1)
		{
			for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
			{
				if(indic_1!=0)
					qx_month_1_local=this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_1_Lux,(int)ConstVector[AGE_BL_LUX][t]);
				if(indic_2!=0)
					qx_month_2_local=this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_2_Lux,(int)ConstVector[AGE_BL_LUX][t]);
														
				local_surival_t=local_surival_t_bef*(1.-qx_month_1_local-qx_month_2_local+qx_month_1_local*qx_month_2_local);

				local_nur_1VP_t=(local_surival_t_bef*qx_month_2_local+local_nur_1VP_t_bef)*(1.-qx_month_1_local);
				local_nur_2VP_t=(local_surival_t_bef*qx_month_1_local+local_nur_2VP_t_bef)*(1.-qx_month_2_local);


				ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]=local_surival_t+local_nur_1VP_t+local_nur_2VP_t;

				local_Alle_Tote=local_surival_t_bef*qx_month_1_local*qx_month_2_local+
					local_nur_1VP_t_bef*qx_month_1_local+local_nur_2VP_t_bef*qx_month_2_local;

				if(ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1]!=0.)
					ConstVector[DEATH_R_BL_LUX][t]=local_Alle_Tote/ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1];
				else
					ConstVector[DEATH_R_BL_LUX][t]=1.;

				local_surival_t_bef=local_surival_t;
				local_nur_1VP_t_bef=local_nur_1VP_t;
				local_nur_2VP_t_bef=local_nur_2VP_t;				
			}
		}
		else{
			for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
				ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]=1.;
		}
	}
	void InitProjection()
	{
		if(GenerateDebugFiles)
			set_DebugVector_to_zero();
		set_ConstVector_to_zero();
		Calculate_ConstVector_timing();
		Calculate_ConstVector_guarantees();
		Calculate_ConstVector_decrement();

	}
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	virtual void projection_base_contract(double **pvs,int curr_shock_number)
	{
		account_value=0.;
		survival_index=0.;
		survival_index_by_step=0.;
		claims_wb_t=0.;
		claims_db_t=0.;
		basis_capital=0.;
		GMDB_level=0.;
		annuity_per_freq=0.;	
		add_coupon_from_ratchet=0.;
		Performance_bonus_valuation=0.;
		bonus_value=0.;
		fund_fees=0.;
		lapse=0.;
		mortality_rate=0.;
		sumprod_dynamic_lapse=0.;
		dynamic_lapse_ratio=0.;
		lapse_val=0.;
		account_value_1=0.;
		hedge_cost_fees=0.;
		fixed_charges=0.;

		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==0){
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR_BL][i]*ConstVector[ALLE_VERBLEIBEND_BL_LUX][i]*ConstVector[IND_COUPON_DATE_BL_LUX][i];
					if(ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse/=ConstVector[ALLE_VERBLEIBEND_BL_LUX][t];
				}
				else{
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR_BL][i]*ConstVector[IND_COUPON_DATE_BL_LUX][i];
				}

				account_value=this->ModelPoint->Av_Lux*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
				survival_index=1.;
				survival_index_by_step=1.;
				basis_capital=this->ModelPoint->Calc_BK0;
				annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
				GMDB_level=max(this->ModelPoint->Gmdb_Lux,0.)*(this->ModelPoint->Gmdb_Rider_Lux=="Y");
			}
			else
			{
				if(t < this->ModelPoint->Calc_Remaining_Months)
					pvs[curr_shock_number][PV_AV_NEW_RE_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1];

				////////////////////Dynamic lapse
				if((annuity_per_freq*sumprod_dynamic_lapse)==0.)
					dynamic_lapse_ratio=0.;
				else				
					dynamic_lapse_ratio=max(account_value-max(this->Assumption->Lapse_Charges*account_value,this->Assumption->Min_Surrender_Charge_Amount*survival_index),0)/
						(annuity_per_freq*sumprod_dynamic_lapse/MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1]);
				if(t==1)
					pvs[curr_shock_number][PV_ITM_BL]+=dynamic_lapse_ratio;

				lapse=(account_value!=0.)*this->Assumption->Get_Dynamic_Lapse_Value(dynamic_lapse_ratio,(int)ConstVector[DURATION_YEAR_BL_LUX][t]);
				
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					if(ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse=(sumprod_dynamic_lapse*ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1]-
							MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1]*ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1]*ConstVector[IND_COUPON_DATE_BL_LUX][t-1])/
							ConstVector[ALLE_VERBLEIBEND_BL_LUX][t];
				}
				else
					sumprod_dynamic_lapse=sumprod_dynamic_lapse-MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1]*ConstVector[IND_COUPON_DATE_BL_LUX][t-1];
				////////////////////End Dynamic lapse
				
				///////////////////Mortality rate
				mortality_rate=ConstVector[DEATH_R_BL_LUX][t];
				///////////////////End Mortality rate

				///////////////////AV Perf
				fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1));	
				pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				account_value*=(1.+MainProjArrayVector[FUND_RETURN_BL][t]-this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1)));
				///////////////////End AV Perf

				///////////////////GMWB Claims + coupon deduction
				if((int)ConstVector[IND_COUPON_DATE_BL_LUX][t]==1){
				claims_wb_t=max(0.,(annuity_per_freq+add_coupon_from_ratchet)-account_value);				
				pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];	
				account_value-=min(account_value,(annuity_per_freq+add_coupon_from_ratchet));				
				}
				else 
					claims_wb_t=0.;
				///////////////////End GMWB Claims + coupon deduction

				///////////////////AV: Hedge cost+fixed fees	
				hedge_cost_fees=max(0.,account_value*this->ModelPoint->Calc_RRC_p_m);
				pvs[curr_shock_number][PV_AV_TOT_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				account_value-=hedge_cost_fees;
				fixed_charges=min(account_value,this->ModelPoint->Calc_fixed_fees_p_m*survival_index);
				pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]+=fixed_charges*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				account_value-=fixed_charges;
				///////////////////End AV: Hedge cost+fixed fees	
				
				///////////////////AV: lapse + mortality	
				pvs[curr_shock_number][PV_LAPSE_CHARGES_BL]+=max(account_value*this->Assumption->Lapse_Charges,this->Assumption->Min_Surrender_Charge_Amount*survival_index)*lapse*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				lapse_val=account_value*lapse;
				account_value*=(1.-lapse);
				account_value_1=account_value;
				account_value*=(1.-mortality_rate);
				survival_index_by_step=(1.-lapse)*(1.-mortality_rate);
				survival_index*=survival_index_by_step;
				///////////////////End AV: lapse + mortality	
				
				///////////////////GMDB + DB Claims	
				if((int)ConstVector[IND_DB_PERIOD_BL_LUX][t]==1){
					GMDB_level-=min(GMDB_level,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_LUX][t]);
					GMDB_level*=(1.-lapse);
					claims_db_t=max(GMDB_level-account_value_1,0.)*mortality_rate;
					GMDB_level*=(1.-mortality_rate);
				}
				else{
					GMDB_level=0.;
					claims_db_t=0.;
				}		
				pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]+=claims_db_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				
				///////////////////End GMDB + DB Claims	

				///////////////////Basis Capital + coupon calculus
				if((int)ConstVector[IND_RATCHET_MONTH_BL_LUX][t]==1)
					basis_capital=max(account_value,basis_capital*survival_index_by_step);
				else if ((int)ConstVector[IND_COUPON_DATE_BL_LUX][t]==1)
					basis_capital=max(basis_capital-annuity_per_freq,0.)*survival_index_by_step;
				else
					basis_capital=basis_capital*survival_index_by_step;
				
				annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet)*survival_index_by_step;
				///////////////////End Basis Capital + coupon calculus
				
				///////////////////Additional coupon from ratchet
				if((int)ConstVector[IND_RATCHET_MONTH_BL_LUX][t]==1)
					add_coupon_from_ratchet=max(account_value*this->ModelPoint->Calc_Conversion_Rate_p_m,annuity_per_freq)-annuity_per_freq;
				else
					add_coupon_from_ratchet=0.;
				///////////////////End Additional coupon from ratchet

				///////////////////Bonus Calculus + AV deducted by Bonus
				if((int)ConstVector[IND_BONUS_VALUATION_DATE_BL_LUX][t]==1)
					Performance_bonus_valuation=min(max(0,account_value-basis_capital)*this->ModelPoint->Perf_Bonus_Level_Lux,this->ModelPoint->Calc_Min_Bonus_Amount*survival_index);
				else
					Performance_bonus_valuation=Performance_bonus_valuation*survival_index_by_step;

				bonus_value=Performance_bonus_valuation*ConstVector[IND_BONUS_PAYMENT_DATE_BL_LUX][t];
				account_value-=min(account_value,bonus_value);				
				///////////////////End Bonus Calculus + AV deducted by Bonus

			}
			if(GenerateDebugFiles) //If debuging the porjection : not used in production
			{
				this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_BL_LUX, t);
				this->set_value_on_debug_file(this->account_value_1,ACCOUNT_VALUE_BEF_MORT_BL_LUX, t);
				this->set_value_on_debug_file(this->survival_index_by_step,SURVIVAL_INDEX_STEP_BL_LUX, t);
				this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_BL_LUX, t);
				this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_BL_LUX, t);
				this->set_value_on_debug_file(this->claims_db_t,CLAIMS_DB_BL_LUX, t);				
				this->set_value_on_debug_file(this->basis_capital,BASIS_KAPITAL_BL_LUX, t);
				this->set_value_on_debug_file(this->annuity_per_freq,COUPON_VALUE_BL_LUX, t);
				this->set_value_on_debug_file(this->add_coupon_from_ratchet,ADD_COUPON_RATCHET_BL_LUX, t);
				this->set_value_on_debug_file(this->Performance_bonus_valuation,PERFORMANCE_BONUS_VAL_BL_LUX, t);	
				this->set_value_on_debug_file(this->bonus_value,BONUS_VAL_BL_LUX, t);
				this->set_value_on_debug_file(this->fund_fees,FUND_FEES_VAL_BL_LUX, t);
				this->set_value_on_debug_file(this->lapse,DYNAMIC_LAPSE_LUX, t);
				this->set_value_on_debug_file(this->dynamic_lapse_ratio,ITM_VAL_BL_LUX, t);
				this->set_value_on_debug_file(this->lapse_val,LAPSE_VAL_BL_LUX, t);
				this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_BL_LUX, t);
				this->set_value_on_debug_file(this->fixed_charges,FIXED_CHARGE_VALUE_BL_LUX, t);
				this->set_value_on_debug_file(this->GMDB_level,GMDB_BL_LUX, t);

			}		
		}
	}
	void PresentValueCalculation(double **pvs,int curr_shock_number,bool delta)
	{	
		// Calculate account value at the beginning and plo id
		pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av_Lux*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
		// Calculate rates
		pvs[curr_shock_number][PV_RRC_RATE_TOT_BL]=this->ModelPoint->Fund_Charge_Lux;
		pvs[curr_shock_number][PV_GUARANTEE_FEES_RATE_BL]=this->ModelPoint->Calc_Gaur_Charge_Rate;
		pvs[curr_shock_number][PV_FIXED_FEES_RATE_BL]=0.;
		pvs[curr_shock_number][PV_ONGOING_COMM_RATE_BL]=this->ModelPoint->Ong_Acqui_Charge_Lux;
		pvs[curr_shock_number][PV_TAIL_COMM_RATE_BL]=this->ModelPoint->Trail_Comm_Lux;
		pvs[curr_shock_number][PV_EHC_NEW_RE_RATE_BL]=this->ModelPoint->Reinsurance_Premium_Lux;

		//Calculate claims 
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]=pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+pvs[curr_shock_number][PV_CLAIMS_GMDB_BL];	
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]/=(this->total_number_scn);

		//Calculate real ride charges 
		pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_RETRO_FUND_CHARGES_BL]=pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]/this->ModelPoint->Fund_fees_Lux*this->Assumption->Retrocessions_Fund;
		pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_GUARANTEE_FEES_CHARGES_BL]=pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Calc_Gaur_Charge_Rate;
		pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_ONGOING_COMM_CHARGES_BL]=pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Ong_Acqui_Charge_Lux;
		pvs[curr_shock_number][PV_TAIL_COMM_CHARGES_BL]=pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Trail_Comm_Lux;
		pvs[curr_shock_number][PV_EXPENSES_CHARGES_BL]=0.;
		pvs[curr_shock_number][PV_REAL_CHARGES_BL]=pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]+pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL];
		pvs[curr_shock_number][PV_LAPSE_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_AV_TOT_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_AV_NEW_RE_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES_BL]=pvs[curr_shock_number][PV_AV_NEW_RE_BL]*pvs[curr_shock_number][PV_EHC_NEW_RE_RATE_BL]/12.;

		//Calculate EHC internal value 
		if((delta == true && curr_shock_number==0) || delta==false)
		{
			if(this->ModelPoint->New_Business_Lux)
				pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]=pvs[curr_shock_number][PV_CLAIMS_TOT_BL]*12./
						(pvs[curr_shock_number][PV_AV_TOT_BL]*(pvs[curr_shock_number][PV_AV_TOT_BL]!=0.)+0.01*(pvs[curr_shock_number][PV_AV_TOT_BL]==0.))/*(this->total_number_scn)*/;	
			else
				pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]=this->ModelPoint->Ehc_Lux/*(this->total_number_scn)*/;
		}
		else
			pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]=this->ModelPoint->index_shock_pvs[0][PV_EHC_INTERNAL_RATE_BL];
		//Calculate EHC internal Charges
		
		pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES_BL]=pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]*pvs[curr_shock_number][PV_AV_TOT_BL]/12.;
		//Calculate LOVs
		pvs[curr_shock_number][PV_LOV_INTERNAL_BL]=pvs[curr_shock_number][PV_CLAIMS_TOT_BL]-pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES_BL];
		pvs[curr_shock_number][PV_LOV_NEW_RE_BL]=pvs[curr_shock_number][PV_CLAIMS_TOT_BL]-pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES_BL];
		
		//Calculate ITM
		pvs[curr_shock_number][PV_ITM_BL]/=(this->total_number_scn);
	
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