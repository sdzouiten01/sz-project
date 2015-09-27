#pragma once
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "Projection classes/Country projection classes/Projection_BL_WDB_01.h"
#include "head_BL.h"
using namespace std;

extern ofstream trace;
class Projection_BL_WDBL_01 : public Projection_BL_CH_WB_01 {

public:
	
	~Projection_BL_WDBL_01(){}
	Projection_BL_WDBL_01( const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
			const char * tables, bool generate_dbg_files,const string& debug_file_path, 
			int t_high_shorter, bool l_longevity_tables):Projection_BL_CH_WB_01( i_s_a,f_s_a,
			tables,generate_dbg_files,debug_file_path,t_high_shorter,l_longevity_tables)
	{}

	virtual void initialise_index_map(){}
	virtual void project_t(int t, double ** pvs, int curr_shock_nb, double lapse_mult, double mortality_mult, bool shock_av) {}
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
		
		this->ModelPoint->Calc_Remaining_Months=720;

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

		

		if(this->ModelPoint->Freq_Pay=="MON")
		{
			this->ModelPoint->Calc_Freq_Pay=12;
			//this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate*12./12.36;
		}
		else if(this->ModelPoint->Freq_Pay=="QUA")
		{
			this->ModelPoint->Calc_Freq_Pay=4;
			//this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate*4./4.08;
		}
		else if(this->ModelPoint->Freq_Pay=="SEM")
		{
			this->ModelPoint->Calc_Freq_Pay=2;
			//this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate*2./2.02;
		}
		else
		{
			this->ModelPoint->Calc_Freq_Pay=1;
			//this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate;
		}
		
		this->ModelPoint->Calc_Withdrawals_rate=this->ModelPoint->Withdrawals_rate;
		
		
		this->ModelPoint->Calc_Bonus_Freq_Pay=1;


		this->ModelPoint->Calc_Factor_Adj_Bonus=1.;
		this->ModelPoint->Calc_Bonus_Freq_Pay=1;
		this->ModelPoint->Calc_Coupon_by_Freq=this->ModelPoint->Annuity_Yearly/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Initial_Guar_Coupon_by_Freq=this->ModelPoint->Rollup_GMWB*this->ModelPoint->Calc_Withdrawals_rate/this->ModelPoint->Calc_Freq_Pay;
		this->ModelPoint->Calc_Bonus_Base=this->ModelPoint->Premium_Paid;
	}

	//void Calculate_ConstVector_guarantees()
	//{
	//	ConstVector[IND_DEFERAL_PERIOD_BL][1]=(int)(ConstVector[MONTH_BL][1]<=this->ModelPoint->Calc_Deferal_Duration_Months);
	//	ConstVector[IND_BONUS_VALUATION_DATE_BL][0]=(1-ConstVector[IND_DEFERAL_PERIOD_BL][1])*(this->ModelPoint->Last_Bonus_Valuation!=0.);
	//	for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
	//	{
	//		ConstVector[IND_CONTRACT_PERIOD_BL][t]=(int)(ConstVector[MONTH_BL][t]<=this->ModelPoint->Calc_Remaining_Months);
	//		ConstVector[IND_DEFERAL_PERIOD_BL][t]=(int)(ConstVector[MONTH_BL][t]<=this->ModelPoint->Calc_Deferal_Duration_Months);
	//		ConstVector[IND_RATCHET_MONTH_BL][t]=(int)(ConstVector[MONTH_BL][t]==this->ModelPoint->Calc_Deferal_Duration_Months)*this->Assumption->ratchet_indicator;
	//		ConstVector[IND_BONUS_VALUATION_DATE_BL][t]=(int)(ConstVector[DURATION_MONTH_BL][t]==11)*(ConstVector[MONTH_BL][t]>=(this->ModelPoint->Calc_Deferal_Duration_Months+12));
	//		ConstVector[IND_BONUS_DATE_BL][t]=(int)(ConstVector[IND_BONUS_VALUATION_DATE_BL][t]*this->Assumption->use_bonus_option*(1.-this->Use_bonus_to_incr_coupon_payment));//this is a change vs RentaSafe Time
	//		ConstVector[GROSS_PREMIUM_BL][t]=this->ModelPoint->Premium_Paid*ConstVector[IND_CONTRACT_PERIOD_BL][t];


	//		ConstVector[IND_COUPON_DATE_BL][t]=(int)((1-ConstVector[IND_DEFERAL_PERIOD_BL][t])*ConstVector[IND_CONTRACT_PERIOD_BL][t]);
	//		if(this->ModelPoint->Calc_Freq_Pay==1){
	//			if(ConstVector[DURATION_MONTH_BL][t]+1!=12)
	//				ConstVector[IND_COUPON_DATE_BL][t]=0;		
	//		}
	//		else if(this->ModelPoint->Calc_Freq_Pay==2){
	//			if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=5))
	//				ConstVector[IND_COUPON_DATE_BL][t]=0;			
	//		}
	//		else if(this->ModelPoint->Calc_Freq_Pay==3){
	//			if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=4)
	//				&&(ConstVector[DURATION_MONTH_BL][t]+1!=8))
	//				ConstVector[IND_COUPON_DATE_BL][t]=0;
	//		}
	//		else if(this->ModelPoint->Calc_Freq_Pay==4){
	//			if((ConstVector[DURATION_MONTH_BL][t]+1!=12)&&(ConstVector[DURATION_MONTH_BL][t]+1!=3)
	//				&&(ConstVector[DURATION_MONTH_BL][t]+1!=6)&&(ConstVector[DURATION_MONTH_BL][t]+1!=9))
	//				ConstVector[IND_COUPON_DATE_BL][t]=0;
	//		}
	//	}
	//}
	///////////////////////////////////////////////////////////////////////////////////
	////////////////////////////projection_base_contract()////////// //////////////////
	//////////////////////////////////////////////////////////////////////////////////
	//void projection_base_contract(double **pvs,int curr_shock_number)
	//{
	//	survival_index=0.;
	//	account_value=0.;
	//	gmwb_guarantee=0.;
	//	coupon_value=0.;
	//	bonus_value=0.;
	//	hedge_cost_fees=0.;
	//	lapse=0.;
	//	mortality=0.;
	//	claims_wb_t=0.;
	//	year_bonus_valuation=0.;
	//	gmdb_value=0.;
	//	claims_db_t=0.;
	//	for( int t=0; t <= this->ModelPoint->Calc_Remaining_Months ; t++)
	//	{
	//		if(t==0)
	//		{
	//			account_value=this->ModelPoint->Av0*(this->ShockArray[this->ShockNumber][1]*0.01+1.);
	//			gmwb_guarantee=max(this->ModelPoint->Rollup_GMWB,this->ModelPoint->Ratchet_GMWB);
	//			survival_index=1.;
	//			claims_wb_t=0.;
	//			year_bonus_valuation=this->ModelPoint->Last_Bonus_Valuation;
	//			if(this->Assumption->use_BD_guarantee==1)
	//				gmdb_value=this->ModelPoint->Rollup_GMDB;
	//		}
	//		else
	//		{
	//			account_value*=ConstVector[IND_CONTRACT_PERIOD_BL][t];
	//			account_value*=(1.+MainProjArrayVector[FUND_RETURN_BL][t]-this->ModelPoint->Calc_Fund_Fees_p_m-
	//				this->ModelPoint->Calc_TER_p_m);

	//			gmwb_guarantee=max((gmwb_guarantee+this->Use_bonus_to_incr_coupon_payment*year_bonus_valuation*ConstVector[IND_BONUS_VALUATION_DATE_BL][t-1])*(1.-lapse)*(1.-mortality),
	//				this->Assumption->ratchet_indicator*account_value*ConstVector[IND_RATCHET_MONTH_BL][t]);
	//			coupon_value=this->ModelPoint->Calc_Withdrawals_rate*gmwb_guarantee*ConstVector[IND_COUPON_DATE_BL][t]/this->ModelPoint->Calc_Freq_Pay*this->ModelPoint->Calc_Factor_Adj_Bonus;
	//			
	//			if((this->ModelPoint->Calc_Initial_Deferal_Duration_Months=0)&&(ConstVector[DURATION_YEAR_BL][t]<this->Assumption->years_guarantee_bonus)&&(this->Assumption->use_guarantee_bonus_option==1))
	//				year_bonus_valuation=this->ModelPoint->Bonus_Rate*ConstVector[GROSS_PREMIUM_BL][t]*survival_index;
	//			else if(ConstVector[IND_BONUS_VALUATION_DATE_BL][t]==0.)
	//				year_bonus_valuation*=(1.-lapse)*(1.-mortality);
	//			else
	//				year_bonus_valuation=min(this->ModelPoint->Bonus_Rate*ConstVector[GROSS_PREMIUM_BL][t]*survival_index,max(0.,account_value-gmwb_guarantee)*this->Assumption->Percentage_perf)*
	//				ConstVector[IND_BONUS_VALUATION_DATE_BL][t];

	//			bonus_value=year_bonus_valuation*ConstVector[IND_BONUS_DATE_BL][t]/this->ModelPoint->Calc_Freq_Pay*this->ModelPoint->Calc_Factor_Adj_Bonus;


	//			claims_wb_t=max(0.,coupon_value+bonus_value-account_value);				
	//			pvs[curr_shock_number][PV_CLAIMS_GMWB_BL]+=claims_wb_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];

	//			account_value-=min(account_value,bonus_value+coupon_value);
	//			hedge_cost_fees=account_value*this->ModelPoint->Calc_RRC_p_m;
	//			pvs[curr_shock_number][PV_HC_AV_BASE_BL]+=account_value*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
	//			pvs[curr_shock_number][PV_REAL_CHARGES_BL]+=hedge_cost_fees*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
	//			
	//			
	//			account_value-=hedge_cost_fees;
	//			account_value-=min(account_value,this->ModelPoint->Fixed_Fees/12.*survival_index);
	//			lapse=ConstVector[LAPSE_RATE_BL][t]*(account_value!=0.);
	//			mortality=ConstVector[DEATH_R_BL][t];
	//			account_value*=(1.-lapse);
	//			
	//			if(this->Assumption->use_BD_guarantee==1)
	//			{
	//				gmdb_value=max(gmdb_value-bonus_value-coupon_value,0)*(1.-lapse);
	//				claims_db_t=(ConstVector[AGE_YEAR_BL][t]<this->Assumption->Max_Age_for_DB)*max(gmdb_value-account_value,0)*mortality;
	//				gmdb_value*=(1.-mortality);
	//				pvs[curr_shock_number][PV_CLAIMS_GMDB_BL]+=claims_db_t*MainProjArrayVector[DISCOUNT_FACTOR_BL][t];
	//			}

	//			account_value*=(1.-mortality);
	//			survival_index*=(1.-lapse)*(1.-mortality);
	//		}
	//		if(GenerateDebugFiles) //If debuging the porjection : not used in production
	//		{
	//			this->set_value_on_debug_file(this->gmdb_value,GUAR_GMDB_BL, t);
	//			this->set_value_on_debug_file(this->gmwb_guarantee,GUAR_GMWB_BL, t);
	//			this->set_value_on_debug_file(this->coupon_value,COUPON_VALUE_BL, t);
	//			this->set_value_on_debug_file(this->bonus_value,BONUS_VALUE_BL, t);
	//			this->set_value_on_debug_file(this->hedge_cost_fees,HEDGE_COST_FEES_BL, t);
	//			this->set_value_on_debug_file(this->account_value,ACCOUNT_VALUE_END_BL, t);
	//			this->set_value_on_debug_file(this->claims_wb_t,CLAIMS_WB_BL, t);
	//			this->set_value_on_debug_file(this->claims_db_t,CLAIMS_DB_BL, t);
	//			this->set_value_on_debug_file(this->survival_index,SURVIVAL_INDEX_BL, t);
	//		}
	//		
	//	}
	//}
};