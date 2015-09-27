#ifndef __PROJECTION_AB_H_
#define __PROJECTION_AB_H_
#include "Projection classes/Guarantee projection classes/Projection.h"
#include "Model point classes/Model_Point.h"

class Projection_AB: virtual public Projection {

public:
	GMXBParameters *assump_AB;
	int gmab_is_recursive;
	int gmab_av_type;
	bool gmab_ind;

	const static int AB_RATCHET = 1;
	
public :
	
	Projection_AB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
					const char * tables, bool generate_dbg_files, const string& dbg_file_path, 
					const int t_high_shorter, bool l_longevity_tables, int time_step): 
	   Projection (i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
		assump_AB = new GMXBParameters(tables, "assump_AB");

		if (this->assump_AB->search("gmab_ind") == "Y")
			this->gmab_ind = true;
		else
			this->gmab_ind = false;

		gmab_is_recursive = atoi(this->assump_AB->search("gmab_is_recursive").c_str());

		if (this->assump_AB->search("gmab_av_type") == "RATCHET")
			this->gmab_av_type = AB_RATCHET;
		else
			this->gmab_av_type = 0;
	}

	virtual ~Projection_AB( void )
	{
	   delete this->assump_AB;
	}

	int maturity_year_s() 
	{
		if (this->gmab_is_recursive == 0)
		{
			if (this->time_step == ANNUAL)
				return min((int)t_high_shorter , min((int)(this->term_to_omega), model_point->gmab_period) - this->model_point->elapsed_years);
				
			else
			{
				return min( min(t_high_shorter , this->term_to_omega) , int(model_point->gmab_period));
				//return min((int)t_high_shorter*this->time_step , min((int)(this->term_to_omega*this->time_step), model_point->gmab_period*this->time_step) - this->model_point->elapsed_years*this->time_step + 1*this->time_step -this->model_point->elapsed_months ); // changed by Ali and Ahmed
			}
		}
		else
		{
			if (this->time_step == ANNUAL)
				return min((int)t_high_shorter , (int)(this->term_to_omega - this->model_point->elapsed_years + 1));
			else
				//return min((int)t_high_shorter*this->time_step , (int)(this->term_to_omega*this->time_step - this->model_point->elapsed_years*this->time_step-this->model_point->elapsed_months + 1*this->time_step));
				return min((int)t_high_shorter , (int)(this->term_to_omega));
		}
	}

	void avE (int t)
	{
		if ((this->gmab_is_recursive == 1) && 
				(main_proj_array[t][AV_E_BEF_CLAIMS] >= SMALL_DOUBLE))
					main_proj_array[t][AV_E] = 
								max(0.0, main_proj_array[t][AV_E_BEF_CLAIMS] 
							  + main_proj_array[t][GMAB_CLAIMS]);
		else
			Projection::avE(t);

	}


	//additional function for avE
	void project_gmab_bases_payments_claims(int t)
	{
		this->main_proj_array[t][GMAB_CLAIMS] = 0.0;
		if (this->gmab_ind)
		{
			double premium = this->main_proj_array[t][PREMIUM];
			double gmab_rollup_prev = 0.0;
			double gmab_ratchet_prev= 0.0;

			if (t == 1) 
			{
				gmab_rollup_prev = model_point->gmab_rollup_valn;
				gmab_ratchet_prev = model_point->gmab_ratchet_valn;
			}	
			else
			{      
				gmab_ratchet_prev = main_proj_array[t-1][GMAB_RATCHET_BASE];
				if (this->gmab_av_type == Projection_AB::AB_RATCHET)
				{
 					gmab_rollup_prev = main_proj_array[t-1][GMAB_ROLLUP_BASE];
				}
				else
				{
 					gmab_rollup_prev  = main_proj_array[t-1][GMAB_IF] ; 
				}
			}

			if (((this->timing_items[t][POL_YEAR] > model_point->gmab_period)&&(!this->gmab_is_recursive)) || ((this->timing_items[t][POL_YEAR] > this->product_charging_duration)&&(this->gmab_is_recursive)))
				main_proj_array[t][GMAB_IF] = 0.0;
		
			else 
			{// GMAB BENEFIT BASE CALCULATION
				//-------------------------------
				
				// Set GMAB Rollup Base

					double gmab_rollup_factor;

					if (this->time_step==MONTHLY)
					{	
						if (this->rollup_frequency==EACH_BEG_YEAR )
						{
							//Aging policies
							if (this->project_aging_policies)
							{
								if (t==1 && (ceil(this->duration_age_step + this->model_point->duration) == ceil(this->model_point->duration) + 1))
								{
									gmab_rollup_factor = 1.0 + model_point->gmab_rollup_rate / 100.0;
								}
								else
								{
									gmab_rollup_factor = 1.0;
								}
							}
							else
							{
							if ((int)this->timing_items[t][POL_MONTH] % 12 == 1)
								gmab_rollup_factor = 1.0 + model_point->gmab_rollup_rate / 100.0;
							else 
								gmab_rollup_factor = 1.0;
							}
						}
						else if (this->rollup_frequency==EACH_END_YEAR )
						{
							//Actuarial tool
							if (this->project_aging_policies)
							{
								if (t==1 && (int(this->duration_age_step + this->model_point->duration) == int(this->model_point->duration) + 1))
								{
									gmab_rollup_factor = 1.0 + model_point->gmab_rollup_rate / 100.0;
								}
								else
								{
									gmab_rollup_factor = 1.0;
								}
							}
							else
							{
							if ((int)this->timing_items[t][POL_MONTH] % 12 == 0)
								gmab_rollup_factor = 1.0 + model_point->gmab_rollup_rate / 100.0;
							else
								gmab_rollup_factor = 1.0;
							}
						}
						else // case when rollup_frequency == EACH_T
						{
							if (this->timing_items[t][POL_YEAR] == int(this->model_point->duration)+1)
							{
								int time_to_anniversary = time_step - this->model_point->elapsed_mths_in_valn_yr;
								gmab_rollup_factor = 1.0 + adjusted_rate(model_point->gmab_rollup_rate / 100.0, time_to_anniversary ,1);
							}
							else 
								gmab_rollup_factor = 1.0 + adjusted_rate(model_point->gmab_rollup_rate / 100.0, time_step, 1);
						}
					}
					else if (this->time_step==ANNUAL)
					{
						if (this->rollup_frequency==EACH_BEG_YEAR )
						{
							if ((int)this->timing_items[t][POL_MONTH] % 12 == 0)
								gmab_rollup_factor = 1.0 + model_point->gmab_rollup_rate / 100.0;
							else 
								gmab_rollup_factor = 1.0;
						}
						else
							gmab_rollup_factor = 1.0 + adjusted_rate(model_point->gmab_rollup_rate / 100.0, time_step, 1);
						}
					//else{}

					
					main_proj_array[t][GMAB_ROLLUP_BASE] = (gmab_rollup_prev + premium) * gmab_rollup_factor;
				
		// Set GMAB Ratchet Base
					 double gmab_ratchet_bef = gmab_ratchet_prev + premium  ;
	   
	   
	   //Reset or ratchet the Benefit Base before decrement 
					double gmab_bef_surv = 0.;
					if ((this->gmab_av_type == Projection_AB::AB_RATCHET) && (model_point->gmab_av_period != 0))
					{	
						//Actuarial tool
						if (this->project_aging_policies)
						{
							if ((t == 1)&&(ceil(this->duration_age_step + this->model_point->duration) ==(ceil(this->model_point->duration) + 1)))
							{
								if (this->time_step == MONTHLY)
								{		
									main_proj_array[t][GMAB_RATCHET_BASE] = max(main_proj_array[t][AV_E_BEF], gmab_ratchet_bef);
								}
							}
							else 
							{
								main_proj_array[t][GMAB_RATCHET_BASE] = gmab_ratchet_bef;
							}
							gmab_bef_surv = max(main_proj_array[t][GMAB_RATCHET_BASE],main_proj_array[t][GMAB_ROLLUP_BASE]);
						}
						else
						{
						if ((fmod((this->timing_items[t][POL_YEAR]), model_point->gmab_av_period) == 0))
						{
							if (this->time_step == ANNUAL)
							{
								main_proj_array[t][GMAB_RATCHET_BASE] = max(main_proj_array[t][AV_E_BEF], gmab_ratchet_bef);
							}
							else if (this->time_step == MONTHLY)
							{
								if ((int)this->timing_items[t][POL_MONTH] % 12 == 0)
								{
									main_proj_array[t][GMAB_RATCHET_BASE] = max(main_proj_array[t][AV_E_BEF], gmab_ratchet_bef);
								}
								else
								{
									main_proj_array[t][GMAB_RATCHET_BASE] = gmab_ratchet_bef;
								}
							}
							else 
							{
								char message[MAX_LENGTH_MESSAGE_EXCEPTION];
								sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
								throw new GMXBException(message);
							}
						}
						else 
						{
							main_proj_array[t][GMAB_RATCHET_BASE] = gmab_ratchet_bef;
						}
						gmab_bef_surv = max(main_proj_array[t][GMAB_RATCHET_BASE],main_proj_array[t][GMAB_ROLLUP_BASE]);
						}
					}
					else
					{ 
						gmab_bef_surv = main_proj_array[t][GMAB_ROLLUP_BASE];
					}

					double surv_mth = this->surv_mth_calc(t);
					main_proj_array[t] [GMAB_RATCHET_BASE]= main_proj_array[t][GMAB_RATCHET_BASE] * surv_mth  ; 
					main_proj_array[t][GMAB_ROLLUP_BASE] = main_proj_array[t][GMAB_ROLLUP_BASE] * surv_mth  ;
					main_proj_array[t] [GMAB_IF]  = gmab_bef_surv * surv_mth ; 

				}//end of gmab benefit base calculation	

			if (this->time_step == ANNUAL)
			{
				if (((int)this->timing_items[t][POL_YEAR] % (int)model_point->gmab_maturity_freq) != 0)
				{
					main_proj_array[t][GMAB_CLAIMS] = 0.0;
				}
				else
				{
					main_proj_array[t][GMAB_CLAIMS] = max(0.0, main_proj_array[t][GMAB_IF] - main_proj_array[t][AV_E_BEF_CLAIMS]);
				}
			}
			else if (this->time_step == MONTHLY)
			{
				if (((int)this->timing_items[t][POL_YEAR] % (int)model_point->gmab_maturity_freq) == 0 && ((int)this->timing_items[t][POL_MONTH] /*+ 1*/) % 12 == 0 )
				{
					main_proj_array[t][GMAB_CLAIMS] = max(0.0, main_proj_array[t][GMAB_IF] - main_proj_array[t][AV_E_BEF_CLAIMS]);
				}
				else
				{
					main_proj_array[t][GMAB_CLAIMS] = 0.0;
				}
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
				throw new GMXBException(message);			
			}
		}
	}
};

#endif