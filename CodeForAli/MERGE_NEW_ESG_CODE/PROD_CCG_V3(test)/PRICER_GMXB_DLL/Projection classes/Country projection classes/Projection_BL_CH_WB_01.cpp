#include "Projection_BL_CH_WB_01.h"




Projection_BL_CH_GMWB_01::~Projection_BL_CH_GMWB_01(){delete this->Assumption;}
Projection_BL_CH_GMWB_01::Projection_BL_CH_GMWB_01(const vector<vector <int>>& i_s_a,const char * tables, bool generate_dbg_files,const string& debug_file_path)
	:Projection(i_s_a,tables,generate_dbg_files,debug_file_path)
{	
	this->Assumption =new Assumption0(tables);
	myConsVector_size=TOTAL_CONSTVECTOR_COLS_BL_CH;
	myMainProjArrayVector_size=MAIN_PROJ_NUM_COLS;
	myDebugVector_size=DEBUG_NUM_COLS_BL_CH;		
		
	set_Local_Parameters_to_zero();
}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_CH_GMWB_01::mySet_Debug_File_Header(){
	this->debug_file << "TIME,DURATION_MONTH_BL_CH,IND_CONTRACT_PERIOD_BL_CH,IND_DEFERAL_PERIOD_BL_CH,IND_RATCHET_MONTH_BL_CH,IND_COUPON_DATE_BL_CH,IND_BONUS_VALUATION_DATE_BL_CH,IND_BONUS_PAYMENT_DATE_BL_CH,FUND_RETURN,DISCOUNT_FACTOR,YEAR_BL_CH,MONTH_BL_CH,DURATION_YEAR_BL_CH,";
	this->debug_file << "DATE_YEAR_BL_CH,DATE_MONTH_BL_CH,ACCOUNT_VALUE_END_BL_CH,ACCOUNT_VALUE_BEF_COUPON_BL_CH,SURVIVAL_INDEX_STEP_BL_CH,SURVIVAL_INDEX_BL_CH,CLAIMS_WB_BL_CH,BASIS_KAPITAL_BL_CH,";
	this->debug_file << "COUPON_VALUE_BL_CH,ADD_COUPON_RATCHET_BL_CH,PERFORMANCE_BONUS_VAL_BL_CH,BONUS_VAL_BL_CH,FUND_FEES_VAL_BL_CH,DYNAMIC_LAPSE_CH,ITM_VAL_BL_CH,LAPSE_VAL_BL_CH,HEDGE_COST_FEES_BL_CH,FIXED_CHARGE_VALUE_BL_CH,";
	this->debug_file << endl;																																
}
void Projection_BL_CH_GMWB_01::set_current_model_point(Model_Point *mp)
{
	this->ModelPoint = dynamic_cast<Model_Point_BL_RST_SWISS_01*>(mp);

	this->initialise_mp_calculation();
	myCalc_Remaining_Months=this->ModelPoint->Calc_Remaining_Months;
	myPosition_MP_in_File=this->ModelPoint->mp_position_in_file;
	myCalc_Prorata=this->ModelPoint->Calc_Prorata;
	this->InitProjection();
}
void Projection_BL_CH_GMWB_01::init_Count_scen_to_zero(){this->ModelPoint->Calc_Count_Scen=0;}
int Projection_BL_CH_GMWB_01::Count_scen(){return this->ModelPoint->Calc_Count_Scen;}
void Projection_BL_CH_GMWB_01::Increase_Count_scen(){this->ModelPoint->Calc_Count_Scen +=1;}
	///////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_CH_GMWB_01::initialise_mp_calculation()
{
		if((this->ModelPoint->Prod_Name_Bl=="RVATAp_13")||(this->ModelPoint->Prod_Name_Bl=="RVATp_13")||(this->ModelPoint->Prod_Name_Bl=="RVATAp_14")||(this->ModelPoint->Prod_Name_Bl=="RVATp_14")
			||(this->ModelPoint->Prod_Name_Bl=="RVATAp_15")||(this->ModelPoint->Prod_Name_Bl=="RVATp_15")){
			this->Assumption->Guarantee_Charge_p_m=this->Assumption->Guarantee_Charge_Before_p_m;
			this->Assumption->Guarantee_Charge=this->Assumption->Guarantee_Charge_Before;
		}
		else{
			this->Assumption->Guarantee_Charge_p_m=this->Assumption->Guarantee_Charge_After_04_2015_p_m;
			this->Assumption->Guarantee_Charge=this->Assumption->Guarantee_Charge_After_04_2015;
		}
		
		
		int local_val=0;
		local_val=atoi(this->ModelPoint->Valn_Date.c_str());
		this->ModelPoint->Calc_Valn_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Valn_Month=(int)((local_val-10000*this->ModelPoint->Calc_Valn_Year)/100);
		this->ModelPoint->Calc_Valn_Day=(int)(local_val-10000*this->ModelPoint->Calc_Valn_Year-100*this->ModelPoint->Calc_Valn_Month);

		local_val=atoi(this->ModelPoint->Issue_Date.c_str());
		this->ModelPoint->Calc_IssueDate_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_IssueDate_Month=(int)((local_val-10000*this->ModelPoint->Calc_IssueDate_Year)/100);
		this->ModelPoint->Calc_IssueDate_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_IssueDate_Month);

		local_val=atoi(this->ModelPoint->Nb_Date_Bl.c_str());
		this->ModelPoint->Calc_Nb_Date_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Nb_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Nb_Date_Year)/100);
		this->ModelPoint->Calc_Nb_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_IssueDate_Year-100*this->ModelPoint->Calc_Nb_Date_Month);

		local_val=atoi(this->ModelPoint->Inv_Date.c_str());
		this->ModelPoint->Calc_Inv_Date_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Inv_Date_Month=(int)((local_val-10000*this->ModelPoint->Calc_Inv_Date_Year)/100);
		this->ModelPoint->Calc_Inv_Date_Day=(int)(local_val-10000*this->ModelPoint->Calc_Inv_Date_Year-100*this->ModelPoint->Calc_Inv_Date_Month);

		local_val=atoi(this->ModelPoint->Annuity_Begin.c_str());
		this->ModelPoint->Calc_Annuity_Begin_Year=(int)(local_val/10000);
		this->ModelPoint->Calc_Annuity_Begin_Month=(int)((local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year)/100);
		this->ModelPoint->Calc_Annuity_Begin_Day=(int)(local_val-10000*this->ModelPoint->Calc_Annuity_Begin_Year-100*this->ModelPoint->Calc_Annuity_Begin_Month);


		this->ModelPoint->Calc_Duration_Months=(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_IssueDate_Year)*12+(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_IssueDate_Month);
		this->ModelPoint->Calc_Remaining_Months=this->ModelPoint->Duration_Bl-this->ModelPoint->Calc_Duration_Months;
		this->ModelPoint->Calc_Deferal_Duration_Months=-(this->ModelPoint->Calc_Valn_Year-this->ModelPoint->Calc_Annuity_Begin_Year)*12-(this->ModelPoint->Calc_Valn_Month-this->ModelPoint->Calc_Annuity_Begin_Month);
		this->ModelPoint->Calc_Prorata=( 31. - min(this->ModelPoint->Calc_Valn_Day,30) )/30.;
		this->ModelPoint->Calc_Fund_Fees_p_m=this->ModelPoint->Fund_Fees/12.;
		this->ModelPoint->Calc_Ehc_p_m=this->ModelPoint->Ehc/12.;
		
		if(this->ModelPoint->Freq_Mode=="monthly") this->ModelPoint->Calc_Freq_Pay=12;
		else if(this->ModelPoint->Freq_Mode=="quarterly") this->ModelPoint->Calc_Freq_Pay=4;
		else if(this->ModelPoint->Freq_Mode=="semi-annually") this->ModelPoint->Calc_Freq_Pay=2;
		else this->ModelPoint->Calc_Freq_Pay=1;

		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Guarantee/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Mod_Def_Duration_Months= ( ( (int) this->ModelPoint->Calc_Deferal_Duration_Months % 12 ) + 11 ) % 12 ;
		this->ModelPoint->Calc_NB_Coupons=(this->ModelPoint->Duration_Bl-this->ModelPoint->Def_Period)/12*this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Perf_By_Freq=this->Assumption->Percentage_Perf/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Max_Bonus_Amount_By_Freq=this->Assumption->Bonus_rate*this->ModelPoint->Premium/this->ModelPoint->Calc_Freq_Pay;
}
void Projection_BL_CH_GMWB_01::Calculate_ConstVector_timing()
{
	for( int t=1; t <= this->ModelPoint->Calc_Remaining_Months ; t++){	
		if(t==1){
			ConstVector[DURATION_MONTH_BL_CH][0]=(int)((this->ModelPoint->Calc_Duration_Months+11)%12);
			ConstVector[DURATION_MONTH_BL_CH][1]=(int)(this->ModelPoint->Calc_Duration_Months%12);
		}
		else
			ConstVector[DURATION_MONTH_BL_CH][t]=(int)((ConstVector[DURATION_MONTH_BL_CH][t-1]!=11)*(ConstVector[DURATION_MONTH_BL_CH][t-1]+1));

		if(myGenerateDebugFiles) //If debuging the porjection : not used in production
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
void Projection_BL_CH_GMWB_01::Calculate_ConstVector_guarantees()
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
			if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=5))
				ConstVector[IND_COUPON_DATE_BL_CH][t]=0;			
		}
		else if(this->ModelPoint->Calc_Freq_Pay==3){
			if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=4)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=8))
				ConstVector[IND_COUPON_DATE_BL_CH][t]=0;
		}
		else if(this->ModelPoint->Calc_Freq_Pay==4){
			if((ConstVector[DURATION_MONTH_BL_CH][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=3)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=6)&&(ConstVector[DURATION_MONTH_BL_CH][t]+1!=9))
				ConstVector[IND_COUPON_DATE_BL_CH][t]=0;
		}

		if(this->Assumption->Perf_Bonus_Y_N==1){
			if(  ((((t%12)+11)%12) == this->ModelPoint->Calc_Mod_Def_Duration_Months) &&  ( t > this->ModelPoint->Calc_Deferal_Duration_Months  )  )
				ConstVector[IND_BONUS_VALUATION_DATE_BL_CH][t]=1;
		}
			
		ConstVector[IND_BONUS_PAYMENT_DATE_BL_CH][t]=ConstVector[IND_COUPON_DATE_BL_CH][t]*(this->Assumption->Perf_Bonus_Y_N==1);
	}
}
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
void Projection_BL_CH_GMWB_01::projection_base_contract(int curr_shock_number)
{
	set_Local_Parameters_to_zero();

	for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
	{
		if(t==0)
		{
			account_value=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);
			survival_index=1.;
			survival_index_by_step=1.;
			claims_wb_t=0.;
			basis_capital=this->ModelPoint->Base_Capital;
			annuity_per_freq=this->ModelPoint->Calc_Coupon_by_Freq;
			add_coupon_from_ratchet=0.;
			Performance_bonus_valuation=this->ModelPoint->Perf_Bonus_Amount_Bl/this->ModelPoint->Calc_Freq_Pay;
			bonus_value=this->ModelPoint->Perf_Bonus_Amount_Bl/this->ModelPoint->Calc_Freq_Pay;
			fixed_charges=(this->Assumption->Fixed_Charge_p_m*(annuity_per_freq+add_coupon_from_ratchet)*this->ModelPoint->Calc_Freq_Pay);
			hedge_cost_fees=-(account_value-this->ModelPoint->Av_After_Bef_Bl+fixed_charges);
			//Change Ali if(this->ModelPoint->New_Business_Bl)
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]+=this->ModelPoint->Av_After_Bef_Bl;
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]+=hedge_cost_fees;
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]+=fixed_charges;
		}
		else
		{
			if(t < this->ModelPoint->Calc_Remaining_Months)
				this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t-1];

			fund_fees=account_value*this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1));	
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]+=fund_fees*MainProjArrayVector[DISCOUNT_FACTOR][t];
			if(account_value==0.) itm=0.;
			else if(basis_capital==0.) itm=1000.;
			else itm=account_value/basis_capital;
			lapse=this->Assumption->Get_Dynamic_Lapse_Value(itm);
			survival_index_by_step=(1.-lapse);
			survival_index*=survival_index_by_step;
			account_value*=(1.+MainProjArrayVector[FUND_RETURN][t]-this->ModelPoint->Calc_Fund_Fees_p_m*(this->ModelPoint->Calc_Prorata*(t==1)+(t!=1)));
			lapse_val=account_value*lapse;
			account_value*=(1.-lapse);
			account_value_1=account_value;
			annuity_per_freq = (annuity_per_freq+add_coupon_from_ratchet)*survival_index_by_step;
			account_value-=min(account_value,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t]);				
			bonus_value=Performance_bonus_valuation*survival_index_by_step*ConstVector[IND_BONUS_PAYMENT_DATE_BL_CH][t];
			account_value-=min(account_value,bonus_value);				
			add_coupon_from_ratchet=ConstVector[IND_RATCHET_MONTH_BL_CH][t]*(  max( account_value/this->ModelPoint->Calc_NB_Coupons , annuity_per_freq ) - annuity_per_freq );
			basis_capital=(basis_capital*survival_index_by_step+add_coupon_from_ratchet*this->ModelPoint->Calc_NB_Coupons*ConstVector[IND_RATCHET_MONTH_BL_CH][t])-annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t];
			if((int)ConstVector[IND_BONUS_VALUATION_DATE_BL_CH][t]==1)
				Performance_bonus_valuation=min(max(account_value-basis_capital,0.)*this->ModelPoint->Calc_Perf_By_Freq,this->ModelPoint->Calc_Max_Bonus_Amount_By_Freq*survival_index);
			else
				Performance_bonus_valuation=Performance_bonus_valuation*survival_index_by_step;

			hedge_cost_fees=max(0.,account_value*this->Assumption->Guarantee_Charge_p_m)*(t != this->ModelPoint->Calc_Remaining_Months);
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR][t]*(t != this->ModelPoint->Calc_Remaining_Months);
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR][t];
			account_value-=hedge_cost_fees;
			fixed_charges=min(account_value,this->Assumption->Fixed_Charge_p_m*(annuity_per_freq+add_coupon_from_ratchet)*this->ModelPoint->Calc_Freq_Pay)*(t != this->ModelPoint->Calc_Remaining_Months);
			account_value-=fixed_charges;
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]+=fixed_charges*MainProjArrayVector[DISCOUNT_FACTOR][t];
			claims_wb_t=max(0.,annuity_per_freq*ConstVector[IND_COUPON_DATE_BL_CH][t]+bonus_value-account_value_1);				
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR][t];	
		}
		if(myGenerateDebugFiles) //If debuging the porjection : not used in production
		{
			this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_BL_CH, t);
			this->set_value_on_debug_file(this->account_value_1,ACCOUNT_VALUE_BEF_COUPON_BL_CH, t);
			this->set_value_on_debug_file(this->survival_index_by_step,SURVIVAL_INDEX_STEP_BL_CH, t);
			this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_BL_CH, t);
			this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_BL_CH, t);
			this->set_value_on_debug_file(this->basis_capital,BASIS_KAPITAL_BL_CH, t);
			this->set_value_on_debug_file(this->annuity_per_freq,COUPON_VALUE_BL_CH, t);
			this->set_value_on_debug_file(this->add_coupon_from_ratchet,ADD_COUPON_RATCHET_BL_CH, t);
			this->set_value_on_debug_file(this->Performance_bonus_valuation,PERFORMANCE_BONUS_VAL_BL_CH, t);	
			this->set_value_on_debug_file(this->bonus_value,BONUS_VAL_BL_CH, t);
			this->set_value_on_debug_file(this->fund_fees,FUND_FEES_VAL_BL_CH, t);
			this->set_value_on_debug_file(this->lapse,DYNAMIC_LAPSE_CH, t);
			this->set_value_on_debug_file(this->itm,ITM_VAL_BL_CH, t);
			this->set_value_on_debug_file(this->lapse_val,LAPSE_VAL_BL_CH, t);
			this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_BL_CH, t);
			this->set_value_on_debug_file(this->fixed_charges,FIXED_CHARGE_VALUE_BL_CH, t);
		}		
	}
}
void Projection_BL_CH_GMWB_01::PresentValueCalculation(int curr_shock_number,bool delta)
{
	// Calculate account value at the beginning and plo id
	this->ModelPoint->index_shock_pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]=this->ModelPoint->Av_After_After*(this->ShockArray[curr_shock_number][1]*0.01+1.);
	// Calculate rates
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_RATE_TOT]=this->Assumption->Guarantee_Charge;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_RATE]=this->Assumption->Guarantee_Charge;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_RATE]=this->Assumption->Fixed_Charge;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_RATE]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_RATE]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_RATE]=this->ModelPoint->New_Re_Reinsurance_premium;

	//Calculate claims 
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]
		+this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB];	
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMWB]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_GMDB]/=(this->total_number_scn);

	//Calculate real ride charges 
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RETRO_FUND_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FUND_FEES_CHARGES]
		/this->ModelPoint->Fund_Fees*this->Assumption->Retrocessions_Fund;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_GUARANTEE_FEES_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES];
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ONGOING_COMM_CHARGES]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_TAIL_COMM_CHARGES]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EXPENSES_CHARGES]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_REAL_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_FIXED_FEES_CHARGES]
		+this->ModelPoint->index_shock_pvs[curr_shock_number][PV_RRC_TOT_CHARGES];
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LAPSE_CHARGES]=0.;
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]/=(this->total_number_scn);
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_NEW_RE]
		*this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_RATE]/12.;

	//Calculate EHC internal value 
	if((delta == true && curr_shock_number==0) || delta==false)
	{
		if(this->ModelPoint->New_Business)
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]*12./
					(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]*(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]!=0.)
					+0.01*(this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]==0.))/*(this->total_number_scn)*/;	
		else
			this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->Ehc/*(this->total_number_scn)*/;
	}
	else
		this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]=this->ModelPoint->index_shock_pvs[0][PV_EHC_INTERNAL_RATE];
	//Calculate EHC internal Charges
	
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_RATE]
		*this->ModelPoint->index_shock_pvs[curr_shock_number][PV_AV_TOT]/12.;
	//Calculate LOVs
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LOV_INTERNAL]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]
		-this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_INTERNAL_CHARGES];
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_LOV_NEW_RE]=this->ModelPoint->index_shock_pvs[curr_shock_number][PV_CLAIMS_TOT]
		-this->ModelPoint->index_shock_pvs[curr_shock_number][PV_EHC_NEW_RE_CHARGES];
		
	//Calculate ITM
	this->ModelPoint->index_shock_pvs[curr_shock_number][PV_ITM]/=(this->total_number_scn);		
}