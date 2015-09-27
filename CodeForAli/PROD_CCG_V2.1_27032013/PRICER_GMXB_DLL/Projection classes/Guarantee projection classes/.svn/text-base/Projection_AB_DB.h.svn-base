#ifndef __PROJECTION_AB_DB_H_
#define __PROJECTION_AB_DB_H_
#include "Projection classes/Guarantee projection classes/Projection_AB.h"
#include "Projection classes/Guarantee projection classes/Projection_DB.h"

class Projection_AB_DB : public Projection_AB, public Projection_DB {
	
public:
	Projection_AB_DB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
					const char* tables, bool generate_dbg_files, const string& dbg_file_path, 
					const int t_high_shorter, bool l_longevity_tables, int time_step): 
		Projection(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step),
		Projection_AB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step),
		Projection_DB(i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
	}

	virtual void project_t(int t, double **pvs, int curr_shock_nb, double lapse_mult, 
							double mortality_mult, bool shock_av)
	{
		this->avBBef(t);
		if (shock_av)
		{
			if(t==1)
				this->delta_shock(t);
		}
		this->avBBefBis(t);
		this->avB(t);
		this->avEBef(t);
		this->init_rates(t, lapse_mult, mortality_mult);
		this->avEBefClaims(t);
		this->project_gmab_bases_payments_claims(t);
		this->avE(t);
		this->project_gmdb_bases_payments_claims(t);
		this->finish_t(t);
		this->populate_pvs(t,pvs, curr_shock_nb);
	}
};

#endif
