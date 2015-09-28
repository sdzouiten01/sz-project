#ifndef __PROJECTION_DB_H_
#define __PROJECTION_DB_H_
#include "Projection classes/Guarantee projection classes/Projection.h"
#include "Model point classes/Model_Point.h"

class Projection_DB : virtual public Projection {
	
public :
	GMXBParameters *assump_DB;
	int gmdb_rollup_rate_source;
	int gmdb_duration ; 
	double gmdb_rollup_rate ;
	int gmdb_av_type ;
	int gmdb_rollup_type;
	double gmdb_max_bonus_term ;
	int gmdb_ratchet_wb_adj ;
	int gmdb_impact_of_gmwb ;
	bool gmdb_ind;


	const static int DB_RATCHET = 1;

	const static int DB_ROLLUP_LINEAR = 1;
	const static int DB_ROLLUP_COMPOUND = 2;

	const static int DATA_FILE = 1;

	const static int ADJUST = 1;
	const static int EXIT = 2;

	const static int IMPACT_DOLLAR = 1;
	const static int IMPACT_PRORATA = 2;

public:
	Projection_DB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a, 
				  const char * tables, bool generate_dbg_files, const string& dbg_file_path,
				  const int t_high_shorter, bool l_longevity_tables, int time_step):
		Projection (i_s_a, f_s_a , tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
		assump_DB = new GMXBParameters(tables, "assump_DB");
		gmdb_duration = atoi(this->assump_DB->search("gmdb_duration").c_str());
		gmdb_rollup_rate = atof(this->assump_DB->search("gmdb_rollup_rate").c_str());

		if (this->assump_DB->search("gmdb_av_type") == "RATCHET")
			this->gmdb_av_type = DB_RATCHET;
		else 
			this->gmdb_av_type = 0;

		if (this->assump_DB->search("gmdb_rollup_type") == "LINEAR")
				this->gmdb_rollup_type = DB_ROLLUP_LINEAR;

		else if (this->assump_DB->search("gmdb_rollup_type") == "COMPOUND")
				this->gmdb_rollup_type = DB_ROLLUP_COMPOUND;

		else
				this->gmdb_rollup_type = 0;

		gmdb_max_bonus_term = atof(this->assump_DB->search("gmdb_max_bonus_term").c_str());

		if (this->assump_DB->search("gmdb_ratchet_wb_adj") == "ADJUST")
				this->gmdb_ratchet_wb_adj = ADJUST;
		else if (this->assump_DB->search("gmdb_ratchet_wb_adj") == "EXIT")
				this->gmdb_ratchet_wb_adj = EXIT;
		else
				this->gmdb_ratchet_wb_adj = 0;

		if (this->assump_DB->search("gmdb_impact_of_gmwb") == "DOLLAR")
			this->gmdb_impact_of_gmwb = IMPACT_DOLLAR;
		
		else if (this->assump_DB->search("gmdb_impact_of_gmwb") == "PRORATA")
			this->gmdb_impact_of_gmwb = IMPACT_PRORATA;
		else
			this->gmdb_impact_of_gmwb = 0;



		if (this->assump_DB->search("gmdb_ind") == "Y")
			this->gmdb_ind = true;
		else
			this->gmdb_ind = false;

		if (this->assump_DB->search("gmdb_rollup_rate_source") == "DATA_FILE")
			this->gmdb_rollup_rate_source = DATA_FILE;
		else
			this->gmdb_rollup_rate_source = 0;

	}

	virtual ~Projection_DB( void )
	{
		delete this->assump_DB;
	}

	virtual void project_gmdb_bases_payments_claims(int t)
	{
		double gmdb_rollup_prev; 
		double gmdb_ratchet_prev;
		double premium = this->regular_premium_calculation(t);

		if (this->gmdb_rollup_rate_source == Projection_DB::DATA_FILE)
			this->gmdb_rollup_rate = model_point->axa_roll_r;	
		if (t == 1) 
		{
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
		if (this->timing_items[t][POL_YEAR] > 
			min(this->gmdb_duration, this->product_charging_duration))
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
		    
			if (this->timing_items[t][POL_YEAR]<= min(model_point->gmxb_deferral_period, this->gmdb_max_bonus_term)) 
    			main_proj_array[t][GMDB_PREM_IF_BEF] = gmdb_rollup_prev + premium + gmdb_vested_base * this->gmdb_rollup_rate / 100.;
			else
    			main_proj_array[t][GMDB_PREM_IF_BEF] = gmdb_rollup_prev + premium;
		    	
			//1.2.RATCHET BASE, DB BASE BEFORE DECREMENTS----------------------------------------------------------------
			if ((this->gmdb_av_type == Projection_DB::DB_RATCHET) && (model_point->gmdb_av_period != 0))
				{
					if (((int)(this->timing_items[t][POL_YEAR]) % model_point->gmdb_av_period) == 0)
					{
						 if(this->gmdb_ratchet_wb_adj == Projection_DB::ADJUST) 
		   					main_proj_array[t][GMDB_AV_IF_BEF] = 
								max(main_proj_array[t][AV_E_BEF], gmdb_ratchet_prev + premium);
					}
					else 
  						main_proj_array[t][GMDB_AV_IF_BEF] = gmdb_ratchet_prev + premium;
								
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
			min(this->gmdb_duration, this->product_charging_duration))
		{			
			main_proj_array[t][GMDB_CLAIMS] = 0.0;
		}
		else
		{
			main_proj_array[t][GMDB_CLAIMS] = max(0.0, main_proj_array[t][GMDB_IF_BEF] - main_proj_array[t][AV_E_BEF])* main_proj_array[t][DEATH_RATE];	
			if (this->correct_med_db == "N")//to be deleted once correction is ok on prod
			{
				main_proj_array[t][GMDB_CLAIMS] = max(0.0, gmdb_rollup_prev - main_proj_array[t][AV_E_BEF])* main_proj_array[t][DEATH_RATE];
			}
		}
}
};

#endif