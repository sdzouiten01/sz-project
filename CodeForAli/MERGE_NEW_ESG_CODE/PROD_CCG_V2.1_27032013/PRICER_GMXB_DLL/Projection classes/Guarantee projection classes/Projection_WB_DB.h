#ifndef __PROJECTION_WB_DB_H_
#define __PROJECTION_WB_DB_H_
#include "Projection classes/Guarantee projection classes/Projection_DB.h"
#include "Projection classes/Guarantee projection classes/Projection_WB.h"

class Projection_WB_DB : public Projection_WB, public Projection_DB {
	
public :
	Projection_WB_DB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a, 
					const char* tables, bool generate_dbg_files, const string& dbg_file_path,
					const int t_high_shorter, bool l_longevity_tables, int time_step): 
		Projection (i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path,t_high_shorter, l_longevity_tables, time_step ),
		Projection_WB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path,t_high_shorter, l_longevity_tables, time_step),
		Projection_DB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path,t_high_shorter, l_longevity_tables, time_step)
	{
	}
	
	void project_gmdb_bases_payments_claims(int t)
	{
		double gmdb_rollup_prev; 
		double gmdb_ratchet_prev;
		double premium = this->regular_premium_calculation(t);
		double he_divider = 1.;
		
		if (this->is_he == true)
			he_divider = 100.;
		 

		//INITIALIZE-------------------------------------------------------------------------------------------------

		if (this->gmdb_rollup_rate_source == Projection_DB::DATA_FILE)
		{
			if (this->time_step!=ANNUAL && this->timing_items[t][POL_YEAR] == int(this->model_point->duration)+1){
				int time_to_anniversary=time_step - this->model_point->elapsed_mths_in_valn_yr;
				this->gmdb_rollup_rate = adjusted_rate(model_point->axa_roll_r / he_divider, time_to_anniversary,1);
			}
			else
				this->gmdb_rollup_rate = adjusted_rate(model_point->axa_roll_r / he_divider, time_step,1);	

			if (this->rollup_frequency == EACH_BEG_YEAR && ((int)this->timing_items[t][POL_MONTH] % 12 == 0 || t==1) )
			this->gmdb_rollup_rate = model_point->axa_roll_r / he_divider;
			else if (this->rollup_frequency == EACH_BEG_YEAR)
				this->gmdb_rollup_rate = 0.0;
		}
			
		if (t == 1) 
			{//from data
				gmdb_rollup_prev = model_point->gmdb_rollup_valn;     
				gmdb_ratchet_prev = model_point->gmdb_ratchet_valn;
			}
		else
			{
				gmdb_ratchet_prev = main_proj_array[t-1][GMDB_AV_IF];

 				if (this->gmdb_av_type == Projection_DB::DB_RATCHET)
 					gmdb_rollup_prev = main_proj_array[t-1][GMDB_PREM_IF];
 				else
 					gmdb_rollup_prev  = main_proj_array[t-1][GMDB_IF]; 

			}

		//1.DB BASES ------------------------------------------------------------------------------------------------
		if (this->timing_items[t][POL_YEAR] > min(this->gmdb_duration, product_charging_duration_s()))
			{
				main_proj_array[t][GMDB_IF_BEF] = 0.0;
				main_proj_array[t][GMDB_PREM_IF_BEF] = 0.0;
				main_proj_array[t][GMDB_AV_IF] = 0.0;	
				main_proj_array[t][GMDB_PREM_IF] = 0.0;
				main_proj_array[t][GMDB_AV_IF] = 0.0;
				main_proj_array[t][GMDB_IF] = 0.0;	
			}
		else 
			{			
			//1.1.ROLLUP¨BASE -------------------------------------------------------------------------------------------   
			double gmdb_vested_base = 0.0;  

			if (this->gmdb_rollup_type == Projection_DB::DB_ROLLUP_LINEAR) 
				gmdb_vested_base = (model_point->prem_paid + premium) * surv_cumul[t-1];
			else if (this->gmdb_rollup_type == Projection_DB::DB_ROLLUP_COMPOUND)
				gmdb_vested_base = gmdb_rollup_prev + premium * surv_cumul[t-1];
		    
			if (this->timing_items[t][POL_YEAR] <= min(model_point->gmxb_deferral_period, this->gmdb_max_bonus_term)) 
    			main_proj_array[t][GMDB_PREM_IF_BEF] = gmdb_rollup_prev + premium + gmdb_vested_base * this->gmdb_rollup_rate / 100.;
			else
    			main_proj_array[t][GMDB_PREM_IF_BEF] = gmdb_rollup_prev + premium;
		      
			//if (this->gmwb_ind == "Y")
			if (this->gmdb_impact_of_gmwb == Projection_DB::IMPACT_DOLLAR)
					main_proj_array[t][GMDB_PREM_IF_BEF]  = max(0., main_proj_array[t][GMDB_PREM_IF_BEF] - main_proj_array[t][GMWB_PAYMENT]) ; 
				else if (this->gmdb_impact_of_gmwb == Projection_DB::IMPACT_PRORATA)
					// if (main_proj_array_2[AV_B_BEF][t][CALC_TOT_INDEX] > SMALL_DOUBLE)
					if (main_proj_array[t][AV_B_BEF] > SMALL_DOUBLE)
						//main_proj_array_2[GMDB_PREM_IF_BEF][t]  = max(0., main_proj_array_2[GMDB_PREM_IF_BEF][t] *( 1.0 - main_proj_array_2[GMWB_PAYMENT][t] / main_proj_array_2[AV_B_BEF][t][CALC_TOT_INDEX]));
						main_proj_array[t][GMDB_PREM_IF_BEF]  = max(0., main_proj_array[t][GMDB_PREM_IF_BEF] *( 1.0 - main_proj_array[t][GMWB_PAYMENT] / main_proj_array[t][AV_B_BEF]));
					else
						main_proj_array[t][GMDB_PREM_IF_BEF] = max(0., main_proj_array[t][GMDB_PREM_IF_BEF]);

			//1.2.RATCHET BASE, DB BASE BEFORE DECREMENTS----------------------------------------------------------------
			if ((this->gmdb_av_type == Projection_DB::DB_RATCHET) && (model_point->gmdb_av_period != 0))
				{	
				if ((((int)(this->timing_items[t][POL_YEAR]) % model_point->gmdb_av_period) == 0) && (this->time_step==ANNUAL || (this->time_step==MONTHLY && (int)this->timing_items[t][POL_MONTH] % 12 == 11)))
					{
				if (this->time_step==ANNUAL || (this->time_step==MONTHLY && (int)this->timing_items[t][POL_MONTH] % 12 == 11)){
						if(this->gmdb_ratchet_wb_adj == Projection_DB::EXIT)
							main_proj_array[t][GMDB_AV_IF_BEF] = gmdb_ratchet_prev + premium + (main_proj_array[t][GMWB_AV_IF_BEF] - main_proj_array[t][GMWB_RATCHET_PREV]);
			   			
						else if(this->gmdb_ratchet_wb_adj == Projection_DB::ADJUST) 
		   				//	main_proj_array_2[GMDB_AV_IF_BEF][t] = max(main_proj_array_2[AV_E][t][CALC_TOT_INDEX], gmdb_ratchet_prev + premium);
							main_proj_array[t][GMDB_AV_IF_BEF] = max(main_proj_array[t][AV_E], gmdb_ratchet_prev + premium);
					}
					else 
						main_proj_array[t][GMDB_AV_IF_BEF] = gmdb_ratchet_prev + premium;
					}
				else 
  					main_proj_array[t][GMDB_AV_IF_BEF] = gmdb_ratchet_prev + premium;
				
					main_proj_array[t][GMDB_AV_IF_BEF] = max(0., main_proj_array[t][GMDB_AV_IF_BEF] - main_proj_array[t][GMWB_PAYMENT]) ; 
				
				main_proj_array[t][GMDB_IF_BEF] = max(main_proj_array[t][GMDB_AV_IF_BEF] , main_proj_array[t][GMDB_PREM_IF_BEF]);
				}
			else 
				main_proj_array[t][GMDB_IF_BEF] = main_proj_array[t][GMDB_PREM_IF_BEF];
			
			double surv_mth = this->surv_mth_calc(t);
			 		
			//1.3.BASES AFTER DECREMENTS---------------------------------------------------------------------------------
			main_proj_array[t][GMDB_AV_IF] = main_proj_array[t][GMDB_AV_IF_BEF] * surv_mth  ; 
			main_proj_array[t][GMDB_PREM_IF] = main_proj_array[t][GMDB_PREM_IF_BEF] * surv_mth  ;
			main_proj_array[t][GMDB_IF]  = main_proj_array[t][GMDB_IF_BEF] * surv_mth ; 

			}//END DB BASE CALCS	

		//2.DB CLAIMS -----------------------------------------------------------------------------------------------
		if ((int)(this->timing_items[t][POL_YEAR])> 
			min(this->gmdb_duration, product_charging_duration_s()))
		{			
			main_proj_array[t][GMDB_CLAIMS] = 0.0;
		}
		else
			{
				// main_proj_array_2[GMDB_CLAIMS][t] = max(0.0, main_proj_array_2[GMDB_IF_BEF][t] - main_proj_array_2[AV_E_BEF][t][CALC_TOT_INDEX])* main_proj_array_2[DEATH_RATE][t];
				main_proj_array[t][GMDB_CLAIMS] = max(0.0, main_proj_array[t][GMDB_IF_BEF] - main_proj_array[t][AV_E_BEF])* main_proj_array[t][DEATH_RATE];
				if (this->correct_med_db == "N")//to be deleted once correction is ok on prod
				{
					// main_proj_array_2[GMDB_CLAIMS][t] = max(0.0, gmdb_rollup_prev - main_proj_array_2[AV_E_BEF][t][CALC_TOT_INDEX])* main_proj_array_2[DEATH_RATE][t];
					main_proj_array[t][GMDB_CLAIMS] = max(0.0, gmdb_rollup_prev - main_proj_array[t][AV_E_BEF])* main_proj_array[t][DEATH_RATE];
				}
			}

};

	virtual void project_t(int t, double **pvs, int curr_shock_nb, double lapse_mult, 
							double mortality_mult, bool shock_av)
	{
		this->avBBef(t);
		if (shock_av)
		{
			if(t==1)
				this->delta_shock(t);
		}
		this->project_gmwb_payments_claims_beg(t);
		this->avBBefBis(t);
		this->avB(t);
		this->avEBef(t);
		this->init_rates(t, lapse_mult, mortality_mult);
		this->avEBefClaims(t);
		this->project_gmwb_bases_payments_claims(t);
		this->avE(t);
		this->project_gmdb_bases_payments_claims(t);
		this->finish_t(t);
		this->populate_pvs(t, pvs, curr_shock_nb);
	}
};

#endif
