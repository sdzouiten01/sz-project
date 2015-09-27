#pragma once
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "Model point classes/Country model point classes/model_point_BL_RST_CH_LIE_01.h"
#include "head_BL_LIE.h"
#include "head_BL.h"
using namespace std;

extern ofstream trace;
class Assumption_BL_LIE_CH_GMWB_01 
{
	public:
		// Assumptions tables------------------------------------------------
		GMXBParameters  *assump;
		int Ratchet_Y_N;
		int Perf_Bonus_Y_N;
		double Percentage_Perf;
		double Bonus_rate;
		int Use_Lapse_Y_N;
		double Base_Lapse;
		double Base_Lapse_p_m;
		int Dynamic_Lapse_Y_N;
		double X_1;
		double X_2;
		double X_3;
		double X_4;
		double Y_1;
		double Y_2;
		double Y_3;
		double Y_4;
		double Guarantee_Charge;
		double Guarantee_Charge_p_m;
		double Fixed_Charge;
		double Fixed_Charge_p_m;
		double Lapse_Charge; //BGinput
		double Min_Surrender_Charge_Amount; //BGinput
		double Retrocessions_Fund;
		

public:
	virtual ~Assumption_BL_LIE_CH_GMWB_01(){delete this->assump;}
	Assumption_BL_LIE_CH_GMWB_01(){}
	Assumption_BL_LIE_CH_GMWB_01(const char * tables)
	{
		// Assumptions tables------------------------------------------------

		map<string, GMXB_Assumption_Table*> sheet_tables;
		assump = parse_sheets(string(tables), sheet_tables, string("assump") );
		
		Ratchet_Y_N = atoi(this->assump->search("Ratchet_Y_N").c_str());
		Perf_Bonus_Y_N = atoi(this->assump->search("Perf_Bonus_Y_N").c_str());
		Percentage_Perf=atof(this->assump->search("Percentage_Perf").c_str());
		Use_Lapse_Y_N = atoi(this->assump->search("Use_Lapse_Y_N").c_str());
		Base_Lapse=atof(this->assump->search("Base_Lapse").c_str());
		Base_Lapse_p_m=1-exp(log(1-Base_Lapse)/12.);
		Dynamic_Lapse_Y_N = atoi(this->assump->search("Dynamic_Lapse_Y_N").c_str());
		X_1=atof(this->assump->search("X_1").c_str());
		X_2=atof(this->assump->search("X_2").c_str());
		X_3=atof(this->assump->search("X_3").c_str());
		X_4=atof(this->assump->search("X_4").c_str());
		Y_1=atof(this->assump->search("Y_1").c_str());
		Y_2=atof(this->assump->search("Y_2").c_str());
		Y_3=atof(this->assump->search("Y_3").c_str());
		Y_4=atof(this->assump->search("Y_4").c_str());
		Guarantee_Charge=atof(this->assump->search("Guarantee_Charge").c_str());
		Guarantee_Charge_p_m=Guarantee_Charge/12.;
		Fixed_Charge=atof(this->assump->search("Fixed_Charge").c_str());
		Fixed_Charge_p_m=Fixed_Charge/12.;
		Lapse_Charge=atof(this->assump->search("Lapse_Charge").c_str());  //BGinput
		Min_Surrender_Charge_Amount=atof(this->assump->search("Min_Surrender_Charge_Amount").c_str());  //BGinput
		Bonus_rate=atof(this->assump->search("Bonus_rate").c_str());
		Retrocessions_Fund=atof(this->assump->search("Retrocessions_Fund").c_str());

	}
	double Get_Dynamic_Lapse_Value(double moneyness)
	{
		if(Use_Lapse_Y_N==0)
			return 0.;
		else if(Dynamic_Lapse_Y_N==0)
			return Base_Lapse_p_m*(moneyness!=0.);
		else
		{
			double a,b,u,v,plf;
				
			if(moneyness<X_1){a=X_1;b=X_1;}
			else if(moneyness<X_2){a=X_1;b=X_2;}
			else if(moneyness<X_3){a=X_2;b=X_3;}
			else if(moneyness<X_4){a=X_3;b=X_4;}
			else{a=X_4;b=X_4;}
					
			if(a==X_1) u=Y_1;
			else if(a==X_2) u=Y_2;
			else if(a==X_3) u=Y_3;
			else u=Y_4;
				
			if(b==X_1) v=Y_1;
			else if(b==X_2) v=Y_2;
			else if(b==X_3) v=Y_3;
			else v=Y_4;
				
			if(b==a) plf=u;
			else plf= u+(v-u)/(b-a)*(moneyness-a);

			return plf*Base_Lapse_p_m;
		}
	}
};
class Projection_BL_LIE_CH_GMWB_01 : public Projection {

public:
	
	//Assumption Class	
	Assumption_BL_LIE_CH_GMWB_01 *Assumption;

	//Model Point Class
	Model_Point_BL_RST_CH_LIE_01 *ModelPoint;
	
	//Tables used in the projection
	double ConstVector[TOTAL_CONSTVECTOR_COLS_BL_LIE][SUBACCT_MAX_PERIOD_MTH +1];
	double MainProjArrayVector[MAIN_PROJ_NUM_COLS_BL][SUBACCT_MAX_PERIOD_MTH +1];
	double DebugVector[DEBUG_NUM_COLS_BL_LIE][SUBACCT_MAX_PERIOD_MTH +1];

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
	double basis_capital;
	double annuity_per_freq;	
	double add_coupon_from_ratchet;
	double Performance_bonus_valuation;
	double bonus_value;
	double fund_fees;
	double lapse;
	double itm;
	double lapse_val;
	double account_value_1;
	double account_value_2;
	double hedge_cost_fees;
	double fixed_charges;

	~Projection_BL_LIE_CH_GMWB_01()
	{		
		delete this->Assumption;
		for(int i = 0; i <this->ShockSize; i++){
			if(this->ShockArray[i]!=0)
				delete[] this->ShockArray[i];
		}
		if (this->ShockArray!=0)
			delete[] this->ShockArray;	
	}
	Projection_BL_LIE_CH_GMWB_01( const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
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
		this->Assumption =new Assumption_BL_LIE_CH_GMWB_01(tables);
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
		basis_capital=0.;
		annuity_per_freq=0.;	
		add_coupon_from_ratchet=0.;
		Performance_bonus_valuation=0.;
		bonus_value=0.;
		fund_fees=0.;
		lapse=0.;
		itm=0.;
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
		this->debug_file << "TIME,DURATION_MONTH_BL_LIE,IND_CONTRACT_PERIOD_BL_LIE,IND_DEFERAL_PERIOD_BL_LIE,IND_RATCHET_MONTH_BL_LIE,IND_COUPON_DATE_BL_LIE,IND_BONUS_VALUATION_DATE_BL_LIE,FUND_RETURN_BL,DISCOUNT_FACTOR_BL,YEAR_BL_LIE,MONTH_BL_LIE,DURATION_YEAR_BL_LIE,";
		this->debug_file << "DATE_YEAR_BL_LIE,DATE_MONTH_BL_LIE,ACCOUNT_VALUE_END_BL_LIE,ACCOUNT_VALUE_BEF_COUPON_BL_LIE,SURVIVAL_INDEX_STEP_BL_LIE,SURVIVAL_INDEX_BL_LIE,CLAIMS_WB_BL_LIE,BASIS_KAPITAL_BL_LIE,";
		this->debug_file << "COUPON_VALUE_BL_LIE,ADD_COUPON_RATCHET_BL_LIE,PERFORMANCE_BONUS_VAL_BL_LIE,BONUS_VAL_BL_LIE,FUND_FEES_VAL_BL_LIE,DYNAMIC_LAPSE_LIE,ITM_VAL_BL_LIE,LAPSE_VAL_BL_LIE,HEDGE_COST_FEES_BL_LIE,FIXED_CHARGE_VALUE_BL_LIE,";
		this->debug_file << endl;

		for ( int t=0; t<= this->ModelPoint->Calc_Remaining_Months; t++)
		{
			this->debug_file <<setprecision(15)<< t << ",";
			for(int col = 0; col < TOTAL_CONSTVECTOR_COLS_BL_LIE ; col++)
				this->debug_file <<setprecision(15)<< this->ConstVector[col][t] << ",";
			for(int col = 0; col < MAIN_PROJ_NUM_COLS_BL ; col++)
				this->debug_file << setprecision(15)<< this->MainProjArrayVector[col][t] << ",";
			for(int col = 0; col < DEBUG_NUM_COLS_BL_LIE ; col++)
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
		this->ModelPoint = dynamic_cast<Model_Point_BL_RST_CH_LIE_01*>(mp);
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
		local_val=atoi(this->ModelPoint->Valn_Date_Bl.c_str());
		this->ModelPoint->Calc_Valn_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Valn_Month=(int)((local_val-10000*this->ModelPoint->Calc_Valn_Year)/100);
		this->ModelPoint->Calc_Valn_Day=(int)(local_val-10000*this->ModelPoint->Calc_Valn_Year-100*this->ModelPoint->Calc_Valn_Month);

		local_val=atoi(this->ModelPoint->Issue_Date_Bl.c_str());
		this->ModelPoint->Calc_IssueDate_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_IssueDate_Month=(int)((local_val-10000*this->ModelPoint->Calc_IssueDate_Year)/100);
		this->ModelPoint->Calc_IssueDate_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_IssueDate_Month);

		local_val=atoi(this->ModelPoint->Nb_Date_Bl.c_str());
		this->ModelPoint->Calc_Nb_Date_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Nb_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Nb_Date_Year)/100);
		this->ModelPoint->Calc_Nb_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_Nb_Date_Month);

		local_val=atoi(this->ModelPoint->Inv_Date_Bl.c_str());
		this->ModelPoint->Calc_Inv_Date_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Inv_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Inv_Date_Year)/100);
		this->ModelPoint->Calc_Inv_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Inv_Date_Year-100*this->ModelPoint->Calc_Inv_Date_Month);

		local_val=atoi(this->ModelPoint->Annuity_Begin_Bl.c_str());
		this->ModelPoint->Calc_Annuity_Begin_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Annuity_Begin_Month=(int)((local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year)/100);
		this->ModelPoint->Calc_Annuity_Begin_Day=(int)(local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year-100*this->ModelPoint->Calc_Annuity_Begin_Month);


		this->ModelPoint->Calc_Duration_Months=(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_IssueDate_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_IssueDate_Month);
		this->ModelPoint->Calc_Remaining_Months=this->ModelPoint->Duration_Bl-this->ModelPoint->Calc_Duration_Months;
		this->ModelPoint->Calc_Deferal_Duration_Months=-(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Annuity_Begin_Year)*12-(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Annuity_Begin_Month);
		this->ModelPoint->Calc_Prorata=( 31. - min(this->ModelPoint->Calc_Valn_Day,30) )/30.;
		this->ModelPoint->Calc_Fund_Fees_p_m=this->ModelPoint->Fund_Fees_Bl/12.;
		this->ModelPoint->Calc_Ehc_p_m=this->ModelPoint->Ehc_Bl/12.;
		
		if(this->ModelPoint->Freq_Mode_Bl=="monthly")
			this->ModelPoint->Calc_Freq_Pay=12;
		else if(this->ModelPoint->Freq_Mode_Bl=="quarterly")
			this->ModelPoint->Calc_Freq_Pay=4;
		else if(this->ModelPoint->Freq_Mode_Bl=="semi-annually")
			this->ModelPoint->Calc_Freq_Pay=2;
		else
			this->ModelPoint->Calc_Freq_Pay=1;

		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Guarantee_Bl/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Mod_Def_Duration_Months= ( ( (int) this->ModelPoint->Calc_Deferal_Duration_Months % 12 ) + 11 ) % 12 ;
		this->ModelPoint->Calc_NB_Coupons=(this->ModelPoint->Duration_Bl-this->ModelPoint->Def_Period_Bl)/12*this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Perf_By_Freq=1.;//this->Assumption->Percentage_Perf/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Max_Bonus_Amount_By_Freq=this->Assumption->Bonus_rate*this->ModelPoint->Premium_Bl;//this->ModelPoint->Calc_Freq_Pay;
		
		if ( this->ModelPoint->Calc_Deferal_Duration_Months > 0)
			this->ModelPoint->Calc_Base_Capital_Bl=this->ModelPoint->Calc_Coupon_by_Freq*this->ModelPoint->Calc_NB_Coupons;
		else
			this->ModelPoint->Calc_Base_Capital_Bl=this->ModelPoint->Base_Capital_Bl;

		this->ModelPoint->Calc_Perf_Bonus_Amount_Bl=min(max(this->ModelPoint->Av_After_Bef_Bl-this->ModelPoint->Base_Capital_Bl,0.)*this->ModelPoint->Calc_Perf_By_Freq*this->Assumption->Percentage_Perf,this->ModelPoint->Calc_Max_Bonus_Amount_By_Freq)*
							(this->ModelPoint->Calc_Duration_Months % 12 == 0)*(this->ModelPoint->Calc_Duration_Months > this->ModelPoint->Def_Period_Bl);

	}
	void Calculate_ConstVector_timing()
	{
		for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++){
			
			if(t==1){
				ConstVector[DURATION_MONTH_BL_CH][0]=(int)((this->ModelPoint->Calc_Duration_Months+11)%12);
				ConstVector[DURATION_MONTH_BL_CH][1]=(int)(this->ModelPoint->Calc_Duration_Months%12);
			}
			else
				ConstVector[DURATION_MONTH_BL_CH][t]=(int)((ConstVector[DURATION_MONTH_BL_CH][t-1]!=11)*(ConstVector[DURATION_MONTH_BL_CH][t-1]+1));

			if(GenerateDebugFiles) //If debuging the porjection : not used in production
			{
				if(t==1){
					DebugVector[MONTH_BL_CH][0]=0;
					DebugVector[MONTH_BL_CH][1]=1;
					DebugVector[YEAR_BL_CH][1]=1;
					DebugVector[DURATION_YEAR_BL_CH][1]=(int)(this->ModelPoint->Calc_Duration_Months/12.);
					DebugVector[DURATION_YEAR_BL_CH][0]=max((int)((this->ModelPoint->Calc_Duration_Months-1)/12.),0);
					DebugVector[DATE_YEAR_BL_CH][1]=this->ModelPoint->Calc_Valn_Year;
					DebugVector[DATE_MONTH_BL_CH][1]=this->ModelPoint->Calc_Valn_Month;
				}
				else{
					DebugVector[MONTH_BL_CH][t]=(int)DebugVector[MONTH_BL_CH][t-1]+1;
					DebugVector[YEAR_BL_CH][t]=(int)(DebugVector[MONTH_BL_CH][t-1]/12)+1;
					
					DebugVector[DURATION_YEAR_BL_CH][t]=(int)DebugVector[DURATION_YEAR_BL_CH][t-1];
					if((int)ConstVector[DURATION_MONTH_BL_CH][t]==0)
						DebugVector[DURATION_YEAR_BL_CH][t]+=1;
					
					DebugVector[DATE_MONTH_BL_CH][t]=(int)((DebugVector[DATE_MONTH_BL_CH][t-1]!=12)*(DebugVector[DATE_MONTH_BL_CH][t-1]+1)+(DebugVector[DATE_MONTH_BL_CH][t-1]==12));
					DebugVector[DATE_YEAR_BL_CH][t]=DebugVector[DATE_YEAR_BL_CH][t-1];
					if((int)DebugVector[DATE_MONTH_BL_CH][t-1]==12)
						DebugVector[DATE_YEAR_BL_CH][t]+=1;
				}			
			}
		}
	}
	virtual void Calculate_ConstVector_guarantees()
	{
		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			ConstVector[IND_CONTRACT_PERIOD_BL_CH][t]=(int)(t<=this->ModelPoint->Calc_Remaining_Months);
			ConstVector[IND_DEFERAL_PERIOD_BL_CH][t]=(int)(t<=this->ModelPoint->Calc_Deferal_Duration_Months);
			
			if ((this->Assumption->Ratchet_Y_N==1)&&(t==this->ModelPoint->Calc_Deferal_Duration_Months)&&((int)ConstVector[IND_CONTRACT_PERIOD_BL_CH][t]==1) )
				ConstVector[IND_RATCHET_MONTH_BL_CH][t]=1;
			else ConstVector[IND_RATCHET_MONTH_BL_CH][t]=0;

			ConstVector[IND_COUPON_DATE_BL_CH][t]=(int)((1-ConstVector[IND_DEFERAL_PERIOD_BL_CH][t]));
			if(this->ModelPoint->Calc_Freq_Pay==1){
				if(ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)
					ConstVector[IND_COUPON_DATE_BL_CH][t]=0;		
			}
			else if(this->ModelPoint->Calc_Freq_Pay==2){
				if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=6))
					ConstVector[IND_COUPON_DATE_BL_CH][t]=0;			
			}
			else if(this->ModelPoint->Calc_Freq_Pay==3){
				if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=4)
					&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=8))
					ConstVector[IND_COUPON_DATE_BL_CH][t]=0;
			}
			else if(this->ModelPoint->Calc_Freq_Pay==4){
				if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=3)
					&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=6)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=9))
					ConstVector[IND_COUPON_DATE_BL_CH][t]=0;
			}

			if(this->Assumption->Perf_Bonus_Y_N==1){
				if(  ((((t%12)+11)%12) == this->ModelPoint->Calc_Mod_Def_Duration_Months) &&  ( t > this->ModelPoint->Calc_Deferal_Duration_Months  )  )
					ConstVector[IND_BONUS_VALUATION_DATE_BL_CH][t]=1;
			}
			
			//ConstVector[IND_BONUS_PAYMENT_DATE_BL_CH][t]=ConstVector[IND_COUPON_DATE_BL_CH][t]*(this->Assumption->Perf_Bonus_Y_N==1);

		}
	}
	void InitProjection()
	{
		if(GenerateDebugFiles)
			set_DebugVector_to_zero();
		set_ConstVector_to_zero();
		Calculate_ConstVector_timing();
		Calculate_ConstVector_guarantees();

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
		basis_capital=0.;
		annuity_per_freq=0.;	
		add_coupon_from_ratchet=0.;
		Performance_bonus_valuation=0.;
		bonus_value=0.;
		fund_fees=0.;
		lapse=0.;
		itm=0.;
		lapse_val=0.;
		account_value_1=0.;
		account_value_2=0.;
		hedge_cost_fees=0.;
		fixed_charges=0.;

		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==0)
			{
				account_value=this->ModelPoint->Av_After_After_Bl*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
				survival_index=1.;
				survival_index_by_step=1.;
				claims_wb_t=0.;
				basis_capital=this->ModelPoint->Calc_Base_Capital_Bl;
				annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
				add_coupon_from_ratchet=0.;
				Performance_bonus_valuation=this->ModelPoint->Perf_Bonus_Amount_Bl;
				//bonus_value=this->ModelPoint->Perf_Bonus_Amount_Bl;
				bonus_value=this->ModelPoint->Calc_Perf_Bonus_Amount_Bl;
				fixed_charges=(this->Assumption->Fixed_Charge_p_m*(annuity_per_freq+add_coupon_from_ratchet)*this->ModelPoint->Calc_Freq_Pay);
				//hedge_cost_fees=-(account_value-this->ModelPoint->Av_After_Bef_Bl+fixed_charges); //BGinput
				//Change Ali if(this->ModelPoint->New_Business_Bl)
				//pvs[curr_shock_number][PV_AV_TOT_BL]+=this->ModelPoint->Av_After_Bef_Bl;	//BGinput
				//pvs[curr_shock_number][PV_AV_TOT_BL]+=this->ModelPoint->Av_After_After_Bl;	//BGinput
				pvs[curr_shock_number][PV_AV_TOT_BL]+=0;	//BGinput
				pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]+=hedge_cost_fees;
				pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]+=fixed_charges;
			}
			else
			{
				if(t < this->ModelPoint->Calc_Remaining_Months)
					pvs[curr_shock_number][PV_AV_NEW_RE_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t-1];

				fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1));	
				pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				
				if(account_value==0.) itm=0.;
				else if(basis_capital==0.) itm=1000.;
				else itm=account_value/basis_capital;
				lapse=this->Assumption->Get_Dynamic_Lapse_Value(itm);
				
				account_value_1=account_value;

				account_value*=(1.+MainProjArrayVector[FUND_RETURN_BL][t]-this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1)));
				bonus_value=Performance_bonus_valuation*ConstVector[IND_BONUS_VALUATION_DATE_BL_CH][t-1];
				claims_wb_t=max(0.,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t]+bonus_value-account_value);				
				pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];	
				account_value-=min(account_value,bonus_value);				
				account_value-=min(account_value,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t]);				
				account_value_2=account_value;
				hedge_cost_fees=max(0.,account_value*this->Assumption->Guarantee_Charge_p_m)*(t != this->ModelPoint->Calc_Remaining_Months);
				pvs[curr_shock_number][PV_AV_TOT_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t]*(t != this->ModelPoint->Calc_Remaining_Months);
				pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
				account_value-=hedge_cost_fees;
				fixed_charges=min(account_value,this->Assumption->Fixed_Charge_p_m*(annuity_per_freq+add_coupon_from_ratchet)*this->ModelPoint->Calc_Freq_Pay)*(t != this->ModelPoint->Calc_Remaining_Months);
				account_value-=fixed_charges;
				pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]+=fixed_charges*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];		
				pvs[curr_shock_number][PV_LAPSE_CHARGES_BL]+=max(account_value*this->Assumption->Lapse_Charge,this->Assumption->Min_Surrender_Charge_Amount*survival_index)*lapse*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];  //BGinput
				survival_index_by_step=(1.-lapse);
				survival_index*=survival_index_by_step;		
				lapse_val=account_value*lapse;
				account_value*=(1.-lapse);
				annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet)*survival_index_by_step;//Added by Ali
				basis_capital=survival_index_by_step*max(account_value_1*ConstVector[IND_RATCHET_MONTH_BL_CH][t-1],max(0.,basis_capital-annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t]/*ConstVector[IND_COUPON_DATE_BL_CH][t-1]*/));//Added by Ali
				
				if((int)ConstVector[IND_BONUS_VALUATION_DATE_BL_CH][t]==1)
					Performance_bonus_valuation=min(max(account_value_2-basis_capital,0.)*this->ModelPoint->Calc_Perf_By_Freq*this->Assumption->Percentage_Perf,this->ModelPoint->Calc_Max_Bonus_Amount_By_Freq*survival_index);
				else
					Performance_bonus_valuation=0.;
				//annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet)*survival_index_by_step;
				add_coupon_from_ratchet=ConstVector[IND_RATCHET_MONTH_BL_CH][t]*(  max( account_value/this->ModelPoint->Calc_NB_Coupons , annuity_per_freq ) - annuity_per_freq );

			}
			if(GenerateDebugFiles) //If debuging the porjection : not used in production
			{
				this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_BL_LIE, t);
				this->set_value_on_debug_file(this->account_value_1,ACCOUNT_VALUE_BEF_COUPON_BL_LIE, t);
				this->set_value_on_debug_file(this->survival_index_by_step,SURVIVAL_INDEX_STEP_BL_LIE, t);
				this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_BL_LIE, t);
				this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_BL_LIE, t);
				this->set_value_on_debug_file(this->basis_capital,BASIS_KAPITAL_BL_LIE, t);
				this->set_value_on_debug_file(this->annuity_per_freq,COUPON_VALUE_BL_LIE, t);
				this->set_value_on_debug_file(this->add_coupon_from_ratchet,ADD_COUPON_RATCHET_BL_LIE, t);
				this->set_value_on_debug_file(this->Performance_bonus_valuation,PERFORMANCE_BONUS_VAL_BL_LIE, t);	
				this->set_value_on_debug_file(this->bonus_value,BONUS_VAL_BL_LIE, t);
				this->set_value_on_debug_file(this->fund_fees,FUND_FEES_VAL_BL_LIE, t);
				this->set_value_on_debug_file(this->lapse,DYNAMIC_LAPSE_LIE, t);
				this->set_value_on_debug_file(this->itm,ITM_VAL_BL_LIE, t);
				this->set_value_on_debug_file(this->lapse_val,LAPSE_VAL_BL_LIE, t);
				this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_BL_LIE, t);
				this->set_value_on_debug_file(this->fixed_charges,FIXED_CHARGE_VALUE_BL_LIE, t);
			}		
		}
	}
	void PresentValueCalculation(double **pvs,int curr_shock_number,bool delta)
	{
		// Calculate account value at the beginning and plo id
		pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av_After_After_Bl*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
		// Calculate rates
		pvs[curr_shock_number][PV_RRC_RATE_TOT_BL]=this->Assumption->Guarantee_Charge;
		pvs[curr_shock_number][PV_GUARANTEE_FEES_RATE_BL]=this->Assumption->Guarantee_Charge;
		pvs[curr_shock_number][PV_FIXED_FEES_RATE_BL]=this->Assumption->Fixed_Charge;
		pvs[curr_shock_number][PV_ONGOING_COMM_RATE_BL]=0.;
		pvs[curr_shock_number][PV_TAIL_COMM_RATE_BL]=0.;
		pvs[curr_shock_number][PV_EHC_NEW_RE_RATE_BL]=this->ModelPoint->New_Re_Reinsurance_premium_Bl;

		//Calculate claims 
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]=pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+pvs[curr_shock_number][PV_CLAIMS_GMDB_BL];	
		pvs[curr_shock_number][PV_CLAIMS_TOT_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]/=(this->total_number_scn);

		//Calculate real ride charges 
		pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_RETRO_FUND_CHARGES_BL]=pvs[curr_shock_number][PV_FUND_FEES_CHARGES_BL]/this->ModelPoint->Fund_Fees_Bl*this->Assumption->Retrocessions_Fund;
		pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_GUARANTEE_FEES_CHARGES_BL]=pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL];
		pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_ONGOING_COMM_CHARGES_BL]=0.;
		pvs[curr_shock_number][PV_TAIL_COMM_CHARGES_BL]=0.;
		pvs[curr_shock_number][PV_EXPENSES_CHARGES_BL]=0.;
		pvs[curr_shock_number][PV_REAL_CHARGES_BL]=pvs[curr_shock_number][PV_FIXED_FEES_CHARGES_BL]+pvs[curr_shock_number][PV_RRC_TOT_CHARGES_BL];
		pvs[curr_shock_number][PV_LAPSE_CHARGES_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_AV_TOT_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_AV_NEW_RE_BL]/=(this->total_number_scn);
		pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES_BL]=pvs[curr_shock_number][PV_AV_NEW_RE_BL]*pvs[curr_shock_number][PV_EHC_NEW_RE_RATE_BL]/12.;

		//Calculate EHC internal value 
		if((delta == true && curr_shock_number==0) || delta==false)
		{
			if(this->ModelPoint->New_Business_Bl)
				pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]=pvs[curr_shock_number][PV_CLAIMS_TOT_BL]*12./
						(pvs[curr_shock_number][PV_AV_TOT_BL]*(pvs[curr_shock_number][PV_AV_TOT_BL]!=0.)+0.01*(pvs[curr_shock_number][PV_AV_TOT_BL]==0.))/*(this->total_number_scn)*/;	
			else
				pvs[curr_shock_number][PV_EHC_INTERNAL_RATE_BL]=this->ModelPoint->Ehc_Bl/*(this->total_number_scn)*/;
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