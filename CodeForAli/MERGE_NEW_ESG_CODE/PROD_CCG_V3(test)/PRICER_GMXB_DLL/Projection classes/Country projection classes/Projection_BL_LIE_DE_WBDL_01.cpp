#include "Projection_BL_LIE_DE_WBDL_01.h"
//#define min(a, b) ( (a) < (b) ? (a) : (b) )
//#define max(a, b) ( (a) > (b) ? (a) : (b) )
#include "../../Utility classes/head.h"


Assumption_BL_LIE_DE_GMWBL_01::~Assumption_BL_LIE_DE_GMWBL_01(){}
Assumption_BL_LIE_DE_GMWBL_01::Assumption_BL_LIE_DE_GMWBL_01():Assumption_BL_LIE_CH_GMWBL_01(){}
Assumption_BL_LIE_DE_GMWBL_01::Assumption_BL_LIE_DE_GMWBL_01(const char * tables):Assumption_BL_LIE_CH_GMWBL_01(tables){
		RRC=atof(this->assump->search("RRC").c_str());
		DB_Y_N= atoi(this->assump->search("DB_Y_N").c_str());
	}




Projection_BL_LIE_DE_GMWBL_01::~Projection_BL_LIE_DE_GMWBL_01(){	delete this->Assumption;}
Projection_BL_LIE_DE_GMWBL_01::Projection_BL_LIE_DE_GMWBL_01( const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path)
		:Projection(i_s_a,tables,generate_dbg_files,debug_file_path)
	{
		this->Assumption =new Assumption_BL_LIE_DE_GMWBL_01(tables);
		myConsVector_size=TOTAL_CONSTVECTOR_COLS_LIE;
		myMainProjArrayVector_size=MAIN_PROJ_NUM_COLS;
		myDebugVector_size=DEBUG_NUM_COLS_LIE;		
		
		set_Local_Parameters_to_zero();
	}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LIE_DE_GMWBL_01::set_Local_Parameters_to_zero()
	{
		set_Local_Parameters_to_zero0();
		basis_capital_1=0.;
		gmdb=0.;
		account_value_2=0.;
		add_coupon_from_bonus=0.;	

	}
void Projection_BL_LIE_DE_GMWBL_01::mySet_Debug_File_Header(){
		this->debug_file << "TIME,DURATION_MONTH_LIE_RS_DE,DURATION_YEAR_LIE_RS_DE,AGE_LIE_RS_DE,IND_CONTRACT_PERIOD_LIE_RS_DE,IND_DEFERAL_PERIOD_LIE_RS_DE,IND_RATCHET_MONTH_LIE_RS_DE,IND_COUPON_DATE_LIE_RS_DE,IND_BONUS_VALUATION_DATE_LIE_RS_DE,IND_DB_PERIOD_LIE_RS_DE,ALLE_VERBLEIBEND_LIE_RS_DE,DEATH_R_LIE_RS_DE,";
		this->debug_file << "YEAR_LIE_RS_DE,MONTH_LIE_RS_DE,DATE_YEAR_LIE_RS_DE,DATE_MONTH_LIE_RS_DE,FUND_RETURN_LIE_RS_DE,DISCOUNT_FACTOR_LIE_RS_DE,ACCOUNT_VALUE_END_LIE_RS_DE,ACCOUNT_VALUE_BEF_CHARGES_LIE_RS_DE,SURVIVAL_INDEX_STEP_LIE_RS_DE,SURVIVAL_INDEX_LIE_RS_DE,CLAIMS_WB_LIE_RS_DE,CLAIMS_DB_LIE_RS_DE,";
		this->debug_file << "BASIS_KAPITAL_LIE_RS_DE,GMDB_LIE_RS_DE2,COUPON_VALUE_LIE_RS_DE,ADD_COUPON_RATCHET_LIE_RS_DE,ADD_COUPON_BONUS_LIE_RS_DE,FUND_FEES_VAL_LIE_RS_DE,DYNAMIC_LAPSE_LIE_RS_DE,ITM_VAL_LIE_RS_DE,LAPSE_VAL_LIE_RS_DE,HEDGE_COST_FEES_LIE_RS_DE,FIXED_CHARGE_VALUE_LIE_RS_DE,";
		this->debug_file << endl;																																
	}
void Projection_BL_LIE_DE_GMWBL_01::set_current_model_point(Model_Point *mp)
	{
		this->ModelPoint = dynamic_cast<Model_Point_BL_RS_DE_LIE_01*>(mp);		//BG changed, correct?

		this->initialise_mp_calculation();
		myCalc_Remaining_Months=this->ModelPoint->Calc_Remaining_Months;
		myPosition_MP_in_File=this->ModelPoint->mp_position_in_file;
		myCalc_Prorata=this->ModelPoint->Calc_Prorata;
		this->InitProjection();
	}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LIE_DE_GMWBL_01::initialise_mp_calculation()
	{

//BG: NAMES CHANGED, FORMULAS NOT. HAS STILL TO BE CHECKED AND ADJUSTED! 
		int local_val=0;
		local_val=atoi(this->ModelPoint->Valn_Date.c_str());
		this->ModelPoint->Calc_Valn_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Valn_Month=(int)((local_val-10000*this->ModelPoint->Calc_Valn_Year)/100);
		this->ModelPoint->Calc_Valn_Day=(int)(local_val-10000*this->ModelPoint->Calc_Valn_Year-100*this->ModelPoint->Calc_Valn_Month);
		
		local_val=atoi(this->ModelPoint->Issue_Date.c_str());
		this->ModelPoint->Calc_IssueDate_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_IssueDate_Month=(int)((local_val-10000*this->ModelPoint->Calc_IssueDate_Year)/100);
		this->ModelPoint->Calc_IssueDate_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_IssueDate_Month);
				
		if(this->ModelPoint->Date_Birth_1_Bl!=""){
			local_val=atoi(this->ModelPoint->Date_Birth_1_Bl.c_str());
			this->ModelPoint->Calc_Birth_1_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birth_1_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birth_1_Date_Year)/100);
			this->ModelPoint->Calc_Birth_1_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birth_1_Date_Year-100*this->ModelPoint->Calc_Birth_1_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Birth_1_Date_Year=0;
			this->ModelPoint->Calc_Birth_1_Date_Month=0;
			this->ModelPoint->Calc_Birth_1_Date_Day=0;
		}

		if(this->ModelPoint->Date_Birth_2_Bl!=""){
			local_val=atoi(this->ModelPoint->Date_Birth_2_Bl.c_str());
			this->ModelPoint->Calc_Birth_2_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Birth_2_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Birth_2_Date_Year)/100);
			this->ModelPoint->Calc_Birth_2_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Birth_2_Date_Year-100*this->ModelPoint->Calc_Birth_2_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Birth_2_Date_Year=0;
			this->ModelPoint->Calc_Birth_2_Date_Month=0;
			this->ModelPoint->Calc_Birth_2_Date_Day=0;
		}
		
		if(this->ModelPoint->Date_Death_1_Bl!=""){
			local_val=atoi(this->ModelPoint->Date_Death_1_Bl.c_str());
			this->ModelPoint->Calc_Death_1_Date_Year=(int)(local_val/10000);
			this->ModelPoint->Calc_Death_1_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Death_1_Date_Year)/100);
			this->ModelPoint->Calc_Death_1_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Death_1_Date_Year-100*this->ModelPoint->Calc_Death_1_Date_Month);
		}
		else{
			this->ModelPoint->Calc_Death_1_Date_Year=0;
			this->ModelPoint->Calc_Death_1_Date_Month=0;
			this->ModelPoint->Calc_Death_1_Date_Day=0;
		}
		
		if(this->ModelPoint->Date_Death_2_Bl!=""){
			local_val=atoi(this->ModelPoint->Date_Death_2_Bl.c_str());
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
	
		this->ModelPoint->Calc_Remaining_Months=min(720,(this->Assumption->Max_Age_Mortality+1-this->ModelPoint->Calc_Age_1)*12-this->ModelPoint->Calc_Age_Month_1);

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
		this->ModelPoint->Calc_Initial_Deferal_Months=(this->ModelPoint->Calc_Annuity_Begin_Year-this->ModelPoint->Calc_IssueDate_Year)*12+(this->ModelPoint->Calc_Annuity_Begin_Month-this->ModelPoint->Calc_IssueDate_Month);
		

		this->ModelPoint->Calc_WB_Initial=this->ModelPoint->Benefit_Increase_Factor*this->ModelPoint->Inv_Protection*this->ModelPoint->Premium;
			//BG Calc_WB_Rollup replaced by Calc_WB_Initial
		this->ModelPoint->Calc_Initial_Coupon_by_Freq=this->ModelPoint->Calc_WB_Initial*this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay/(1.+this->ModelPoint->Withrawals_Mode_Discount_Bl);
		
		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Guarantee/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Mod_Def_Duration_Months=( ( (int) this->ModelPoint->Calc_Deferal_Duration_Months % 12 ) + 11 ) % 12 ;
		
		//if(this->ModelPoint->Calc_Deferal_Duration_Months<=0)
		//	this->ModelPoint->Calc_BK0=this->ModelPoint->Base_Capital_Bl;
		//else 
		//	this->ModelPoint->Calc_BK0=this->ModelPoint->Calc_WB_Rollup;
		//since it is run-off business base capital at time 0 can be dicrectly taken from the inforce
		this->ModelPoint->Calc_BK0=this->ModelPoint->Base_Capital;

		//this->ModelPoint->Calc_RRC_p_m=this->ModelPoint->Ong_Charge_1_Bl/100/12.;
		this->ModelPoint->Calc_RRC_p_m=this->Assumption->RRC/12.;
		this->ModelPoint->Calc_Conversion_Rate_p_m=this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay;
		//this->ModelPoint->Calc_Perf_Bonus_Amount_Bl=min(max(this->ModelPoint->Av_After_Bef_Bl-this->ModelPoint->Base_Capital_Bl,0.)*this->Assumption->Percentage_Perf,this->ModelPoint->Calc_Max_Bonus_Amount)*(this->ModelPoint->Calc_Duration_Months % 12 == 0)*(this->ModelPoint->Calc_Duration_Months > this->ModelPoint->Def_Period_Bl);
		//this->ModelPoint->Calc_Gaur_Charge_Rate=this->ModelPoint->Fund_Charge_Bl-this->ModelPoint->Ong_Acqui_Charge_Bl;	//BG: no ong_acqui_charge (taken out), fund_charge replaced by ong_charge_1
		this->ModelPoint->Calc_Gaur_Charge_Rate=this->Assumption->RRC;
		
		
	}
void Projection_BL_LIE_DE_GMWBL_01::Calculate_ConstVector_timing()
	{
		int age_month_local=0;
		for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++){
			
			if(t==1){
				ConstVector[DURATION_MONTH_LIE][0]=(int)((this->ModelPoint->Calc_Duration_Months+11)%12);
				ConstVector[DURATION_MONTH_LIE][1]=(int)(this->ModelPoint->Calc_Duration_Months%12);
				ConstVector[DURATION_YEAR_LIE][1]=(int)(this->ModelPoint->Calc_Duration_Months/12.);
				ConstVector[DURATION_YEAR_LIE][0]=max((int)((this->ModelPoint->Calc_Duration_Months-1)/12.),0);
				age_month_local=this->ModelPoint->Calc_Age_Month_1;
				ConstVector[AGE_LIE][1]=this->ModelPoint->Calc_Age_1;
				ConstVector[AGE_LIE][0]=(int)ConstVector[AGE_LIE][0]-(age_month_local==0);
				ConstVector[MONTH_LIE][0]=0;
				ConstVector[MONTH_LIE][1]=1;
				ConstVector[YEAR_LIE][1]=1;
				ConstVector[DATE_YEAR_LIE][1]=this->ModelPoint->Calc_Valn_Year;
				ConstVector[DATE_MONTH_LIE][1]=this->ModelPoint->Calc_Valn_Month;	
			}
			else
			{
				ConstVector[DURATION_MONTH_LIE][t]=(int)((ConstVector[DURATION_MONTH_LIE][t-1]!=11)*(ConstVector[DURATION_MONTH_LIE][t-1]+1));
				ConstVector[DURATION_YEAR_LIE][t]=(int)ConstVector[DURATION_YEAR_LIE][t-1];
				if((int)ConstVector[DURATION_MONTH_LIE][t]==0)
					ConstVector[DURATION_YEAR_LIE][t]+=1;
				age_month_local=(age_month_local!=11)*(age_month_local+1);
				ConstVector[AGE_LIE][t]=(int)ConstVector[AGE_LIE][t-1]+(age_month_local==0);
				ConstVector[MONTH_LIE][t]=(int)ConstVector[MONTH_LIE][t-1]+1;
				ConstVector[YEAR_LIE][t]=(int)(ConstVector[MONTH_LIE][t-1]/12)+1;			
				ConstVector[DATE_MONTH_LIE][t]=(int)((ConstVector[DATE_MONTH_LIE][t-1]!=12)*(ConstVector[DATE_MONTH_LIE][t-1]+1)+(ConstVector[DATE_MONTH_LIE][t-1]==12));
				ConstVector[DATE_YEAR_LIE][t]=ConstVector[DATE_YEAR_LIE][t-1];
				if((int)ConstVector[DATE_MONTH_LIE][t-1]==12)
					ConstVector[DATE_YEAR_LIE][t]+=1;
			}
		}
	}
void Projection_BL_LIE_DE_GMWBL_01::Calculate_ConstVector_guarantees()
	{
		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			ConstVector[IND_CONTRACT_PERIOD_LIE][t]=(int)(t<=this->ModelPoint->Calc_Remaining_Months);
			ConstVector[IND_DEFERAL_PERIOD_LIE][t]=(int)(t<=this->ModelPoint->Calc_Deferal_Duration_Months);
			
		//BG: instead of model point bonus = "Y" here the new constant Bonus_Y_N from the assumption has been used (now integer instead of string)
			if(this->Assumption->Perf_Bonus_Y_N ==1){
				if(  ( (((t%12)+11)%12) == (((this->ModelPoint->Calc_Deferal_Duration_Months%12)+11)%12) ) &&  ( t > this->ModelPoint->Calc_Deferal_Duration_Months  )  )
					//ConstVector[IND_RATCHET_MONTH_LIE_RS_CHF][t]=1;
					  ConstVector[IND_BONUS_VALUATION_DATE_LIE][t]=1;	//BG: Formula changed, Need to be tested!
			}
			//else if ((this->Assumption->Ratchet_Y_N==1)&&(t==this->ModelPoint->Calc_Deferal_Duration_Months)&&((int)ConstVector[IND_CONTRACT_PERIOD_LIE_RS_CHF][t]==1) ) //BG: Check it, initial_def_per_dur<>0 needed?
			if ((this->Assumption->Ratchet_Y_N==1)&&(t==this->ModelPoint->Calc_Deferal_Duration_Months)&&((int)ConstVector[IND_CONTRACT_PERIOD_LIE][t]==1) ) //BG: Check it, initial_def_per_dur<>0 needed?
				ConstVector[IND_RATCHET_MONTH_LIE][t]=1;
			else ConstVector[IND_RATCHET_MONTH_LIE][t]=0;

			ConstVector[IND_COUPON_DATE_LIE][t]=(int)((1-ConstVector[IND_DEFERAL_PERIOD_LIE][t]));
			if(this->ModelPoint->Calc_Freq_Pay==1){
				if(ConstVector[DURATION_MONTH_LIE][t]+1!=12)
					ConstVector[IND_COUPON_DATE_LIE][t]=0;		
			}
			else if(this->ModelPoint->Calc_Freq_Pay==2){
				if((ConstVector[DURATION_MONTH_LIE][t]+1!=12)&&(ConstVector[DURATION_MONTH_LIE][t]+1!=6))
					ConstVector[IND_COUPON_DATE_LIE][t]=0;			
			}
			else if(this->ModelPoint->Calc_Freq_Pay==3){
				if((ConstVector[DURATION_MONTH_LIE][t]+1!=12)&&(ConstVector[DURATION_MONTH_LIE][t]+1!=4)
					&&(ConstVector[DURATION_MONTH_LIE][t]+1!=8))
					ConstVector[IND_COUPON_DATE_LIE][t]=0;
			}
			else if(this->ModelPoint->Calc_Freq_Pay==4){
				if((ConstVector[DURATION_MONTH_LIE][t]+1!=12)&&(ConstVector[DURATION_MONTH_LIE][t]+1!=3)
					&&(ConstVector[DURATION_MONTH_LIE][t]+1!=6)&&(ConstVector[DURATION_MONTH_LIE][t]+1!=9))
					ConstVector[IND_COUPON_DATE_LIE][t]=0;
			}
			
			ConstVector[IND_DB_PERIOD_LIE][t]=ConstVector[IND_CONTRACT_PERIOD_LIE][t]*(this->Assumption->DB_Y_N ==1);
			
		}
	}
void Projection_BL_LIE_DE_GMWBL_01::Calculate_ConstVector_decrement()
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

		double qx_month_1_local_longevity=1.;
		double qx_month_2_local_longevity=1.;
		double local_surival_t_bef_longevity=1.;
		double local_surival_t_longevity=0.;
		double local_nur_1VP_t_bef_longevity=0.;
		double local_nur_1VP_t_longevity=0.;
		double local_nur_2VP_t_bef_longevity=0.;
		double local_nur_2VP_t_longevity=0.;
		double alle_verbleibend_t_bef_longevity=1.;
		double alle_verbleibend_t_longevity=0.;
		double local_Alle_Tote_longevity=0.;




		int indic_1=1;
		int indic_2=1;
		if( (this->ModelPoint->Calc_Birth_1_Date_Year==0) || (this->ModelPoint->Calc_Death_1_Date_Year!=0) )
			indic_1=0;
		if( (this->ModelPoint->Calc_Birth_2_Date_Year==0) || (this->ModelPoint->Calc_Death_2_Date_Year!=0) )
			indic_2=0;

		ConstVector[DEATH_R_LIE][0]=0.;
		ConstVector[ALLE_VERBLEIBEND_LIE][0]=1.;

		if(this->Assumption->Mortality_Y_N==1)
		{
			for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
			{
				if(indic_1!=0)
					qx_month_1_local=this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_1_Bl,
											(int)ConstVector[DATE_YEAR_LIE][t],this->ModelPoint->Calc_Age_1,this->ModelPoint->Calc_Valn_Year);
				if(indic_2!=0)
					qx_month_2_local=this->Assumption->Get_mortality_rates_vector(this->ModelPoint->Gender_2_Bl,
											(int)ConstVector[DATE_YEAR_LIE][t],this->ModelPoint->Calc_Age_2,this->ModelPoint->Calc_Valn_Year);
														
				local_surival_t=local_surival_t_bef*(1.-qx_month_1_local-qx_month_2_local+qx_month_1_local*qx_month_2_local);
				local_nur_1VP_t=(local_surival_t_bef*qx_month_2_local+local_nur_1VP_t_bef)*(1.-qx_month_1_local);
				local_nur_2VP_t=(local_surival_t_bef*qx_month_1_local+local_nur_2VP_t_bef)*(1.-qx_month_2_local);
				ConstVector[ALLE_VERBLEIBEND_LIE][t]=local_surival_t+local_nur_1VP_t+local_nur_2VP_t;
				local_Alle_Tote=local_surival_t_bef*qx_month_1_local*qx_month_2_local+
					local_nur_1VP_t_bef*qx_month_1_local+local_nur_2VP_t_bef*qx_month_2_local;

				if(ConstVector[ALLE_VERBLEIBEND_LIE][t-1]!=0.)
					ConstVector[DEATH_R_LIE][t]=local_Alle_Tote/ConstVector[ALLE_VERBLEIBEND_LIE][t-1];
				else
					ConstVector[DEATH_R_LIE][t]=1.;

				local_surival_t_bef=local_surival_t;
				local_nur_1VP_t_bef=local_nur_1VP_t;
				local_nur_2VP_t_bef=local_nur_2VP_t;

				if(this->Assumption->Longevity_Y_N==1)
				{
					if(indic_1!=0)
						qx_month_1_local_longevity=this->Assumption->Get_longevity_mortality_rates_vector(this->ModelPoint->Gender_1_Bl,
											(int)ConstVector[DATE_YEAR_LIE][t],this->ModelPoint->Calc_Age_1,this->ModelPoint->Calc_Valn_Year);
					if(indic_2!=0)
						qx_month_2_local_longevity=this->Assumption->Get_longevity_mortality_rates_vector(this->ModelPoint->Gender_2_Bl,
											(int)ConstVector[DATE_YEAR_LIE][t],this->ModelPoint->Calc_Age_2,this->ModelPoint->Calc_Valn_Year);
														
					local_surival_t_longevity=local_surival_t_bef_longevity*(1.-qx_month_1_local_longevity-qx_month_2_local_longevity+qx_month_1_local_longevity*qx_month_2_local_longevity);
					local_nur_1VP_t_longevity=(local_surival_t_bef_longevity*qx_month_2_local_longevity+local_nur_1VP_t_bef_longevity)*(1.-qx_month_1_local_longevity);
					local_nur_2VP_t_longevity=(local_surival_t_bef_longevity*qx_month_1_local_longevity+local_nur_2VP_t_bef_longevity)*(1.-qx_month_2_local_longevity);
					alle_verbleibend_t_longevity=local_surival_t_longevity+local_nur_1VP_t_longevity+local_nur_2VP_t_longevity;
					local_Alle_Tote_longevity=local_surival_t_bef_longevity*qx_month_1_local_longevity*qx_month_2_local_longevity+
						local_nur_1VP_t_bef_longevity*qx_month_1_local_longevity+local_nur_2VP_t_bef_longevity*qx_month_2_local_longevity;

					if(alle_verbleibend_t_bef_longevity!=0.)
						ConstVector[DEATH_R_LIE][t]=local_Alle_Tote_longevity/alle_verbleibend_t_bef_longevity;
					else
						ConstVector[DEATH_R_LIE][t]=1.;

					local_surival_t_bef_longevity=local_surival_t_longevity;
					local_nur_1VP_t_bef_longevity=local_nur_1VP_t_longevity;
					local_nur_2VP_t_bef_longevity=local_nur_2VP_t_longevity;
					alle_verbleibend_t_bef_longevity=alle_verbleibend_t_longevity;
				}
			}
		}
		else{
			for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
				ConstVector[ALLE_VERBLEIBEND_LIE][t]=1.;
		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_LIE_DE_GMWBL_01::projection_base_contract(int curr_shock_number)
	{
		
		set_Local_Parameters_to_zero();

		for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
		{
			if(t==0){
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR][i]*ConstVector[ALLE_VERBLEIBEND_LIE][i]*ConstVector[IND_COUPON_DATE_LIE][i];
					if(ConstVector[ALLE_VERBLEIBEND_LIE][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse/=ConstVector[ALLE_VERBLEIBEND_LIE][t];
				}
				else{
					for( int i=0; i <= this->ModelPoint->Calc_Remaining_Months ; i++)
						sumprod_dynamic_lapse+=MainProjArrayVector[DISCOUNT_FACTOR][i]*ConstVector[IND_COUPON_DATE_LIE][i];
				}

				account_value=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);
				survival_index=1.;
				survival_index_by_step=1.;
				basis_capital=this->ModelPoint->Calc_BK0;
				Performance_bonus_valuation=0.; //this->ModelPoint->Perf_Bonus_Amount_Bl;
				add_coupon_from_bonus=0.; //this->ModelPoint->Calc_Perf_Bonus_Amount_Bl;
				gmdb=this->ModelPoint->Gmdb_Bl;
				annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
				fixed_charges=0.; //this->ModelPoint->Ong_Charge_2_Bl;
			}
			else
			{
				/*if(t < this->ModelPoint->Calc_Remaining_Months)
					this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t-1];*/

				////////////////////Dynamic lapse
				if((annuity_per_freq*sumprod_dynamic_lapse)==0.)
					dynamic_lapse_ratio=0.;
				else
					dynamic_lapse_ratio=max(account_value-max(this->Assumption->Lapse_Charges*account_value,this->Assumption->Min_Surrender_Charge_Amount*survival_index),0)/
							(annuity_per_freq*sumprod_dynamic_lapse/MainProjArrayVector[DISCOUNT_FACTOR][t-1]);
				if(t==1)
					this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ITM]+=dynamic_lapse_ratio;

				lapse=(account_value!=0.)*this->Assumption->Get_Dynamic_Lapse_Value(dynamic_lapse_ratio);
				
				if(this->Assumption->Mortality_Lapse_Y_N==1){
					if(ConstVector[ALLE_VERBLEIBEND_LIE][t]==0.)
						sumprod_dynamic_lapse=0.;
					else
						sumprod_dynamic_lapse=(sumprod_dynamic_lapse*ConstVector[ALLE_VERBLEIBEND_LIE][t-1]-
							MainProjArrayVector[DISCOUNT_FACTOR][t-1]*ConstVector[ALLE_VERBLEIBEND_LIE][t-1]*ConstVector[IND_COUPON_DATE_LIE][t-1])/
							ConstVector[ALLE_VERBLEIBEND_LIE][t];
				}
				else
					sumprod_dynamic_lapse=sumprod_dynamic_lapse-MainProjArrayVector[DISCOUNT_FACTOR][t-1]*ConstVector[IND_COUPON_DATE_LIE][t-1];
				////////////////////End Dynamic lapse
				
				///////////////////Mortality rate
				mortality_rate=ConstVector[DEATH_R_LIE][t];
				///////////////////End Mortality rate

				account_value_1=account_value;


				///////////////////AV Perf
				fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1));	
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR][t];
				account_value*=(1.+MainProjArrayVector[FUND_RETURN][t]-this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1)));
				///////////////////End AV Perf

				//bonus_value=Performance_bonus_valuation*ConstVector[IND_BONUS_VALUATION_DATE_LIE_RS_DE][t-1];

				///////////////////GMWB Claims + coupon deduction
				claims_wb_t=max(0.,annuity_per_freq*ConstVector[IND_COUPON_DATE_LIE][t]-account_value);	
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR][t];	
				account_value-=min(account_value,((annuity_per_freq+add_coupon_from_ratchet*ConstVector[IND_RATCHET_MONTH_LIE][t-1]+add_coupon_from_bonus*ConstVector[IND_BONUS_VALUATION_DATE_LIE][t-1]))*ConstVector[IND_COUPON_DATE_LIE][t]);
				///////////////////End GMWB Claims + coupon deduction

				account_value_2=account_value;	//BG input, for Base Capital adjustment

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
				claims_db_t=max(0.,gmdb*(1.-lapse)-account_value)*(1 == ConstVector[IND_DB_PERIOD_LIE][t])*mortality_rate;	
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB]+=claims_db_t*MainProjArrayVector[DISCOUNT_FACTOR][t];	
				account_value*=(1.-mortality_rate);
				survival_index_by_step=(1.-lapse)*(1.-mortality_rate);
				survival_index*=survival_index_by_step;			
				gmdb=max(0.,survival_index_by_step*gmdb)*(1 == ConstVector[IND_DB_PERIOD_LIE][t]);
				///////////////////End AV: lapse + mortality	

				if(t < this->ModelPoint->Calc_Remaining_Months)
					this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t];

				///////////////////Basis Capital + coupon calculus
				//basis_capital=survival_index_by_step*max(account_value_1*ConstVector[IND_RATCHET_MONTH_LIE_RS_CHF][t-1],max(0.,basis_capital-annuity_per_freq*ConstVector[IND_COUPON_DATE_LIE_RS_CHF][t-1]));
				basis_capital=survival_index_by_step*max(0.,basis_capital-annuity_per_freq*ConstVector[IND_COUPON_DATE_LIE][t-1]);
				basis_capital_1=basis_capital;
				if((int)ConstVector[IND_BONUS_VALUATION_DATE_LIE][t]==1)
					Performance_bonus_valuation=max(account_value_2-basis_capital,0.);					
				else
					Performance_bonus_valuation=0.;
				annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet+add_coupon_from_bonus)*survival_index_by_step;		
				add_coupon_from_ratchet=ConstVector[IND_RATCHET_MONTH_LIE][t]*(  max( account_value*this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay/(1.+this->ModelPoint->Withrawals_Mode_Discount_Bl),annuity_per_freq ) - annuity_per_freq );
				add_coupon_from_bonus=ConstVector[IND_BONUS_VALUATION_DATE_LIE][t]*(  max( account_value*this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay/(1.+this->ModelPoint->Withrawals_Mode_Discount_Bl),annuity_per_freq ) - annuity_per_freq );
				if((int)ConstVector[IND_RATCHET_MONTH_LIE][t]==1 || (int)ConstVector[IND_BONUS_VALUATION_DATE_LIE][t]==1 )
					basis_capital=max(account_value,(annuity_per_freq+add_coupon_from_ratchet+add_coupon_from_bonus)/(this->ModelPoint->Withdrawal_Rate/this->ModelPoint->Calc_Freq_Pay/(1.+this->ModelPoint->Withrawals_Mode_Discount_Bl)));
				///////////////////End Basis Capital + coupon calculus				
			}
			if(myGenerateDebugFiles) //If debuging the porjection : not used in production
			{
				this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_LIE, t);
				this->set_value_on_debug_file(this->account_value_2,ACCOUNT_VALUE_BEF_CHARGES_LIE, t);
				this->set_value_on_debug_file(this->survival_index_by_step,SURVIVAL_INDEX_STEP_LIE, t);
				this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_LIE, t);
				this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_LIE, t);
				this->set_value_on_debug_file(this->claims_db_t,CLAIMS_DB_LIE, t);
				this->set_value_on_debug_file(this->basis_capital,BASIS_KAPITAL_LIE, t);
				this->set_value_on_debug_file(this->gmdb,GMDB_LIE, t);
				this->set_value_on_debug_file(this->annuity_per_freq,COUPON_VALUE_LIE, t);
				this->set_value_on_debug_file(this->add_coupon_from_ratchet,ADD_COUPON_RATCHET_LIE, t);
				this->set_value_on_debug_file(this->add_coupon_from_bonus,ADD_COUPON_BONUS_LIE, t);
				this->set_value_on_debug_file(this->fund_fees,FUND_FEES_VAL_LIE, t);
				this->set_value_on_debug_file(this->lapse,DYNAMIC_LAPSE_LIE, t);
				this->set_value_on_debug_file(this->dynamic_lapse_ratio,ITM_VAL_LIE, t);
				this->set_value_on_debug_file(this->lapse_val,LAPSE_VAL_LIE, t);
				this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_LIE, t);
				this->set_value_on_debug_file(this->fixed_charges,FIXED_CHARGE_VALUE_LIE, t);

			}		
		}
	}
void Projection_BL_LIE_DE_GMWBL_01::PresentValueCalculation(int curr_shock_number,bool delta)
	{
		// Calculate account value at the beginning and plo id
		this->ModelPoint->index_shock_pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);	
		// Calculate rates
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_RATE_TOT]=this->ModelPoint->Calc_Gaur_Charge_Rate;		
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_RATE]=this->ModelPoint->Calc_Gaur_Charge_Rate;		
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_RATE]=this->ModelPoint->Calc_fixed_fees_p_m*12.;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_RATE]=0.;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_RATE]=0.;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_RATE]=this->ModelPoint->New_Re_Reinsurance_premium;

		//Calculate claims 
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]+this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB];	
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]/=(this->total_number_scn);								//BG: total_number_scn where?
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB]/=(this->total_number_scn);

		//Calculate real ride charges 
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RETRO_FUND_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]/(this->ModelPoint->Fund_Fees)*this->Assumption->Retrocessions_Fund;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/=(this->total_number_scn);
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/(this->ModelPoint->Calc_RRC_p_m*12.)*this->ModelPoint->Calc_Gaur_Charge_Rate;
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]/=(this->total_number_scn);
		//this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/this->ModelPoint->Ong_Charge_1*0.;//*this->ModelPoint->Ong_Acqui_Charge_Lux;
		//this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/this->ModelPoint->Ong_Charge_1*0.;//*this->ModelPoint->Trail_Comm_Lux;
		//this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EXPENSES_CHARGES]=0.;
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



