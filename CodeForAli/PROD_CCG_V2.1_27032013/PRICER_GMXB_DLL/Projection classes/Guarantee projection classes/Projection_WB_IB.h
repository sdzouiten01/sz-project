#pragma once
#include "Projection classes/Guarantee projection classes/Projection_WB.h"
#include "Projection classes/Guarantee projection classes/Projection_IB.h"

class Projection_WB_IB : public Projection_WB, public Projection_IB {
	
public:
	Projection_WB_IB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
					const char* tables, bool generate_dbg_files, const string& dbg_file_path,
					const int t_high_shorter, bool l_longevity_tables, int time_step): 
		Projection(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step),
		Projection_WB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step),
		Projection_IB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
	}

	void project_t(int t, double **pvs, int curr_shock_nb, double lapse_mult, 
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
		if (this->main_proj_array[t][AV_E] < SMALL_DOUBLE && this->gmib_ind == "Y")
		{
			this->gmib_election_ind = 1;
			this->project_gmib_bases_payments_claims(t);
			this->main_proj_array[t][GMWB_CLAIMS] = 0.0;
		}
		this->finish_t(t);
		this->populate_pvs(t, pvs, curr_shock_nb);
	}
};

