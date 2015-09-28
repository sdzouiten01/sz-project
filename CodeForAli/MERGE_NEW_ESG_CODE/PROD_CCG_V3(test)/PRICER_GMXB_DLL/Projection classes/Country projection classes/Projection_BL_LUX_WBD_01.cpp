#include "Projection_BL_LUX_WBD_01.h"

//#define min(a, b) ( (a) < (b) ? (a) : (b) )
//#define max(a, b) ( (a) > (b) ? (a) : (b) )

Assumption_BL_LUX_GMWBD_01::~Assumption_BL_LUX_GMWBD_01(){ delete this->mortality_table;}
Assumption_BL_LUX_GMWBD_01::Assumption_BL_LUX_GMWBD_01():Assumption0(){}
Assumption_BL_LUX_GMWBD_01::Assumption_BL_LUX_GMWBD_01(const char * tables):Assumption0(tables)
	{	
		// Assumptions tables------------------------------------------------

		map<string, GMXB_Assumption_Table*> sheet_tables;
		sheet_tables["mortality_table"] = 0;
		mortality_table=sheet_tables["mortality_table"];

		Mortality_Y_N= atoi(this->assump->search("Mortality_Y_N").c_str());
		Max_Age_Mortality= atoi(this->assump->search("Max_Age_Mortality").c_str());
		Best_Estimate_Lapse=atof(this->assump->search("Best_Estimate_Lapse").c_str());
		Min_Lapse_Y_1=atof(this->assump->search("Min_Lapse_Y_1").c_str());
		Min_Lapse=atof(this->assump->search("Min_Lapse").c_str());
		X_5=atof(this->assump->search("X_5").c_str());
		Weight=atof(this->assump->search("Weight").c_str());
		Mortality_Lapse_Y_N= atoi(this->assump->search("Mortality_Lapse_Y_N").c_str());
		Lapse_Charges=atof(this->assump->search("Lapse_Charges").c_str());
		Max_Age_DB= atoi(this->assump->search("Max_Age_DB").c_str());
		Min_Surrender_Charge_Amount=atof(this->assump->search("Min_Surrender_Charge_Amount").c_str());
		index_1_local=0;
		index_2_local=0;
		mort_value_local=0.;
		dyn_lapse_local=0.;
		param_local1=(this->Y_2-this->Y_1)/(this->X_3-this->X_2);
		param_local2=(this->Y_3-this->Y_2)/(this->X_5-this->X_4);		
		
	}
double Assumption_BL_LUX_GMWBD_01::Get_mortality_rates_vector(const string& sex,const int& current_age)
	{
		this->index_2_local=(sex == "MALE")+(sex != "MALE")*2;
		this->index_1_local=current_age;
		this->mort_value_local=this->mortality_table->search(this->index_1_local,this->index_2_local);
		this->mort_value_local=1.-exp(log(1.-this->mort_value_local)/12.);
		return this->mort_value_local;
	}	
double Assumption_BL_LUX_GMWBD_01::Get_Dynamic_Lapse_Value(double moneyness,int current_duration_year)
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
				return this->Base_Lapse_p_m;
		}
		else
				return 0.;
	}



Projection_BL_LUX_GMWBD_01::~Projection_BL_LUX_GMWBD_01(){delete this->Assumption;}
Projection_BL_LUX_GMWBD_01::Projection_BL_LUX_GMWBD_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path)
	:Projection(i_s_a,tables,generate_dbg_files,debug_file_path)
	{
		this->Assumption =new Assumption_BL_LUX_GMWBD_01(tables);
		myConsVector_size=TOTAL_CONSTVECTOR_COLS_BL_LUX;
		myMainProjArrayVector_size=MAIN_PROJ_NUM_COLS;
		myDebugVector_size=DEBUG_NUM_COLS_BL_LUX;		
		
		set_Local_Parameters_to_zero();
	}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LUX_GMWBD_01::mySet_Debug_File_Header(){
		this->debug_file << "TIME,DURATION_MONTH_BL_LUX,DURATION_YEAR_BL_LUX,AGE_BL_LUX,IND_CONTRACT_PERIOD_BL_LUX,IND_DEFERAL_PERIOD_BL_LUX,IND_RATCHET_MONTH_BL_LUX,IND_COUPON_DATE_BL_LUX,IND_BONUS_VALUATION_DATE_BL_LUX,IND_BONUS_PAYMENT_DATE_BL_LUX,IND_DB_PERIOD_BL_LUX,ALLE_VERBLEIBEND_BL_LUX,DEATH_R_BL_LUX,";
		this->debug_file << "YEAR_BL_LUX,MONTH_BL_LUX,DATE_YEAR_BL_LUX,DATE_MONTH_BL_LUX,FUND_RETURN,DISCOUNT_FACTOR,ACCOUNT_VALUE_END_BL_LUX,ACCOUNT_VALUE_BEF_MORT_BL_LUX,SURVIVAL_INDEX_STEP_BL_LUX,SURVIVAL_INDEX_BL_LUX,CLAIMS_WB_BL_LUX,CLAIMS_DB_BL_LUX,";
		this->debug_file << "BASIS_KAPITAL_BL_LUX,GMDB_BL_LUX,COUPON_VALUE_BL_LUX,ADD_COUPON_RATCHET_BL_LUX,PERFORMANCE_BONUS_VAL_BL_LUX,BONUS_VAL_BL_LUX,FUND_FEES_VAL_BL_LUX,DYNAMIC_LAPSE_LUX,ITM_VAL_BL_LUX,LAPSE_VAL_BL_LUX,HEDGE_COST_FEES_BL_LUX,FIXED_CHARGE_VALUE_BL_LUX,";
		this->debug_file << endl;																														
	}
void Projection_BL_LUX_GMWBD_01::set_current_model_point(Model_Point *mp)
	{
		this->ModelPoint = dynamic_cast<Model_Point_LUX_01*>(mp);

		this->initialise_mp_calculation();
		myCalc_Remaining_Months=this->ModelPoint->Calc_Remaining_Months;
		myPosition_MP_in_File=this->ModelPoint->mp_position_in_file;
		myCalc_Prorata=this->ModelPoint->Calc_Prorata;
		this->InitProjection();
	}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LUX_GMWBD_01::initialise_mp_calculation()
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
		

		local_val=atoi(this->ModelPoint->Annuity_Begin.c_str());
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
	
		this->ModelPoint->Calc_Fund_Fees_p_m=this->ModelPoint->Fund_Fees/12.;
		this->ModelPoint->Calc_Ehc_p_m=this->ModelPoint->Ehc/12.;
		this->ModelPoint->Calc_fixed_fees_p_m=0.;
		
		if(this->ModelPoint->Freq_Mode=="MON")
			this->ModelPoint->Calc_Freq_Pay=12;
		else if(this->ModelPoint->Freq_Mode=="QUA")
			this->ModelPoint->Calc_Freq_Pay=4;
		else if(this->ModelPoint->Freq_Mode=="SEM")
			this->ModelPoint->Calc_Freq_Pay=2;
		else
			this->ModelPoint->Calc_Freq_Pay=1;

		this->ModelPoint->Calc_Deferal_Duration_Months=(this->ModelPoint->Calc_Annuity_Begin_Year-this->ModelPoint->Calc_Valn_Year)*12+(this->ModelPoint->Calc_Annuity_Begin_Month-this->ModelPoint->Calc_Valn_Month);
		
		this->ModelPoint->Calc_WB_Rollup=this->ModelPoint->Benefit_Increase_Factor*this->ModelPoint->Inv_Protection*this->ModelPoint->Premium;
		
		this->ModelPoint->Calc_Initial_Coupon_by_Freq=this->ModelPoint->Calc_WB_Rollup*this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay;
		
		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Guarantee/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Mod_Def_Duration_Months=( ( (int) this->ModelPoint->Calc_Deferal_Duration_Months % 12 ) + 11 ) % 12 ;
		
		if(this->ModelPoint->Calc_Deferal_Duration_Months<=0)
			this->ModelPoint->Calc_BK0=this->ModelPoint->Base_Capital;
		else 
			this->ModelPoint->Calc_BK0=this->ModelPoint->Calc_WB_Rollup;
		//else if( (this->ModelPoint->Calc_Valn_Year*100+this->ModelPoint->Calc_Valn_Month) < (this->ModelPoint->Calc_Annuity_Begin_Year*100+this->ModelPoint->Calc_Annuity_Begin_Month) )
		//	this->ModelPoint->Calc_BK0=this->ModelPoint->Calc_WB_Rollup;
		//else
		//	this->ModelPoint->Calc_BK0=max(this->ModelPoint->Calc_WB_Rollup,this->ModelPoint->Av_Lux);

		this->ModelPoint->Calc_RRC_p_m=this->ModelPoint->Fund_Charge_Lux/12.;
		this->ModelPoint->Calc_Conversion_Rate_p_m=this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Min_Bonus_Amount=this->ModelPoint->Premium*this->Assumption->Bonus_rate;
		this->ModelPoint->Calc_Gaur_Charge_Rate=this->ModelPoint->Fund_Charge_Lux-this->ModelPoint->Ong_Acqui_Charge_Lux;


	}
void Projection_BL_LUX_GMWBD_01::Calculate_ConstVector_timing()
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
void Projection_BL_LUX_GMWBD_01::Calculate_ConstVector_guarantees()
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
void Projection_BL_LUX_GMWBD_01::Calculate_ConstVector_decrement()
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
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LUX_GMWBD_01::projection_base_contract(int curr_shock_number)
	{
		set_Local_Parameters_to_zero();

		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==0){
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR][i]*ConstVector[ALLE_VERBLEIBEND_BL_LUX][i]*ConstVector[IND_COUPON_DATE_BL_LUX][i];
					if(ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse/=ConstVector[ALLE_VERBLEIBEND_BL_LUX][t];
				}
				else{
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR][i]*ConstVector[IND_COUPON_DATE_BL_LUX][i];
				}

				account_value=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);
				survival_index=1.;
				survival_index_by_step=1.;
				basis_capital=this->ModelPoint->Calc_BK0;
				annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
				GMDB_level=max(this->ModelPoint->Gmdb_Lux,0.)*(this->ModelPoint->Gmdb_Rider_Lux=="Y");
			}
			else
			{
				if(t < this->ModelPoint->Calc_Remaining_Months)
					this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t-1];

				////////////////////Dynamic lapse
				if((annuity_per_freq*sumprod_dynamic_lapse)==0.)
					dynamic_lapse_ratio=0.;
				else				
					dynamic_lapse_ratio=max(account_value-max(this->Assumption->Lapse_Charges*account_value,this->Assumption->Min_Surrender_Charge_Amount*survival_index),0)/
						(annuity_per_freq*sumprod_dynamic_lapse/MainProjArrayVector[DISCOUNT_FACTOR][t-1]);
				if(t==1)
					this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ITM]+=dynamic_lapse_ratio;

				lapse=(account_value!=0.)*this->Assumption->Get_Dynamic_Lapse_Value(dynamic_lapse_ratio,(int)ConstVector[DURATION_YEAR_BL_LUX][t]);
				
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					if(ConstVector[ALLE_VERBLEIBEND_BL_LUX][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse=(sumprod_dynamic_lapse*ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1]-
							MainProjArrayVector[DISCOUNT_FACTOR][t-1]*ConstVector[ALLE_VERBLEIBEND_BL_LUX][t-1]*ConstVector[IND_COUPON_DATE_BL_LUX][t-1])/
							ConstVector[ALLE_VERBLEIBEND_BL_LUX][t];
				}
				else
					sumprod_dynamic_lapse=sumprod_dynamic_lapse-MainProjArrayVector[DISCOUNT_FACTOR][t-1]*ConstVector[IND_COUPON_DATE_BL_LUX][t-1];
				////////////////////End Dynamic lapse
				
				///////////////////Mortality rate
				mortality_rate=ConstVector[DEATH_R_BL_LUX][t];
				///////////////////End Mortality rate

				///////////////////AV Perf
				fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1));	
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR][t];
				account_value*=(1.+MainProjArrayVector[FUND_RETURN][t]-this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1)));
				///////////////////End AV Perf

				///////////////////GMWB Claims + coupon deduction
				if((int)ConstVector[IND_COUPON_DATE_BL_LUX][t]==1){
				claims_wb_t=max(0.,(annuity_per_freq+add_coupon_from_ratchet)-account_value);				
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR][t];	
				account_value-=min(account_value,(annuity_per_freq+add_coupon_from_ratchet));				
				}
				else 
					claims_wb_t=0.;
				///////////////////End GMWB Claims + coupon deduction

				///////////////////AV: Hedge cost+fixed fees	
				hedge_cost_fees=max(0.,account_value*this->ModelPoint->Calc_RRC_p_m);
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t];
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR][t];
				account_value-=hedge_cost_fees;
				fixed_charges=min(account_value,this->ModelPoint->Calc_fixed_fees_p_m*survival_index);
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]+=fixed_charges*MainProjArrayVector[DISCOUNT_FACTOR][t];
				account_value-=fixed_charges;
				///////////////////End AV: Hedge cost+fixed fees	
				
				///////////////////AV: lapse + mortality	
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LAPSE_CHARGES]+=max(account_value*this->Assumption->Lapse_Charges,this->Assumption->Min_Surrender_Charge_Amount*survival_index)*lapse*MainProjArrayVector[DISCOUNT_FACTOR][t];
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
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB]+=claims_db_t*MainProjArrayVector[DISCOUNT_FACTOR][t];
				
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
			if(myGenerateDebugFiles) //If debuging the porjection : not used in production
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
void Projection_BL_LUX_GMWBD_01::PresentValueCalculation(int curr_shock_number,bool delta)
	{	
		// Calculate account value at the beginning and plo id
		this->ModelPoint->index_shock_pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);
		// Calculate rates
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_RATE_TOT]=this->ModelPoint->Fund_Charge_Lux;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_RATE]=this->ModelPoint->Calc_Gaur_Charge_Rate;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_RATE]=0.;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_RATE]=this->ModelPoint->Ong_Acqui_Charge_Lux;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_RATE]=this->ModelPoint->Trail_Comm_Lux;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_RATE]=this->ModelPoint->Reinsurance_Premium_Lux;

		//Calculate claims 
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]+this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB];	
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB]/=(this->total_number_scn);

		//Calculate real ride charges 
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RETRO_FUND_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]/this->ModelPoint->Fund_Fees*this->Assumption->Retrocessions_Fund;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Calc_Gaur_Charge_Rate;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Ong_Acqui_Charge_Lux;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/this->ModelPoint->Fund_Charge_Lux*this->ModelPoint->Trail_Comm_Lux;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EXPENSES_CHARGES]=0.;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_REAL_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]+this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES];
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LAPSE_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]*this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_RATE]/12.;

		//Calculate EHC internal value 
		if((delta == true && curr_shock_number==0) || delta==false)
		{
			if(this->ModelPoint->New_Business)
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]*12./
						(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]*(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]!=0.)+0.01*(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]==0.))/*(this->total_number_scn)*/;	
			else
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->Ehc/*(this->total_number_scn)*/;
		}
		else
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->index_shock_pvs[0][PV_EHC_INTERNAL_RATE];
		//Calculate EHC internal Charges
		
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]*this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]/12.;
		//Calculate LOVs
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LOV_INTERNAL]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]-this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES];
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LOV_NEW_RE]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]-this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES];
		
		//Calculate ITM
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ITM]/=(this->total_number_scn);
	
	}
	


