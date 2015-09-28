
#pragma once
#include "Projection classes/Guarantee projection classes/Projection.h"

class Projection_IB : virtual public Projection {
	
public:
	GMXBParameters *assump_IB;
	double gmib_spread_rate;
	double gmib_factor_rate;
	double gmib_election_ind;
	string gmib_ind;


	Projection_IB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
					const char * tables, bool generate_dbg_files, const string& dbg_file_path,
					const int t_high_shorter, bool l_longevity_tables, int time_step):
	Projection (i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
		this->assump_IB = new GMXBParameters(tables, "assump_IB");
		this->gmib_spread_rate = atof(this->assump_IB->search("gmib_spread_rate").c_str());
		this->gmib_factor_rate = atof(this->assump_IB->search("gmib_factor_rate").c_str());
		this->gmib_ind = this->assump_IB->search("gmib_ind").c_str();
	}

	
	virtual void project_gmib_bases_payments_claims(int t)
	{

		//Survival index calculation
		double dth_rate ;
		int table_age;
		int pol_yr = int(timing_items[t][POL_YEAR]);
		table_age = int(timing_items[t][AGE_LAST]);
		int omega_age = this->term_to_omega - table_age + 1;

		//initialisation at t = election year
		//to be changed - for tests - surv_rate
		double surv_rate[100][100];
		surv_rate[t][0] = 1.0;

		for (int i =1; i < omega_age; i++)		
		{
			
			dth_rate = min(1.0, this->mort_table->search(int(table_age + i-1), int(this->model_point->valn_year-this->model_point->age_valn))* this->mort_mult);
			surv_rate[t][i] = (1.0 - dth_rate) * surv_rate[t][i-1];
		}


		//Annuity Factor calculation
		double tenyr = this->afmr[this->model_point->elapsed_mths_in_valn_yr + 1][t][MAXIMUM_NUMBER_OF_INDEXES] / 100.0;
		main_proj_array[t][TECHNICAL_RATE]  =  this->gmib_factor_rate * tenyr + this->gmib_spread_rate;

		for (int i = 0; i < omega_age; i++)
			main_proj_array[t][GMIB_AX_CURR] += surv_rate[t][i] / pow(1 + main_proj_array[t][TECHNICAL_RATE], i);

		//GMIB Claims Calculation
		if (main_proj_array[t-1][GMIB_CLAIMS] < SMALL_DOUBLE)
			main_proj_array[t][GMIB_CLAIMS] = main_proj_array[t][GMIB_AX_CURR] * main_proj_array[t][GMWB_PAYMENT]  - main_proj_array[t][GMWB_PAYMENT_FROM_AV];
		else
			main_proj_array[t][GMIB_CLAIMS] = 0.0;

	}//END OF FUNCTION

	void project()
	{
		for(int sh_n = 0; sh_n < this->index_shock_size; sh_n++)
		{
			this->index_shock_number = sh_n;
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= this->maturity_yr; t++) 
			{
				if (this->timing_items[t][POL_YEAR] > this->product_charging_duration || this->gmib_election_ind == 1) 
					break;
				else
					this->project_t(t, this->model_point->index_shock_pvs,this->index_shock_number, 1, 1, true);
			}
			if(this->longstaff_ind=="Y"){
				this->fill_tables_LS(sh_n);
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}


		// Loop over the funds shocks
		for (int f_sh_n = 0; f_sh_n < this->fund_shock_size; f_sh_n++)
		{
			this->shock_fund = true;
			this->fund_shock_number = f_sh_n;
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= this->maturity_yr; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration || this->gmib_election_ind == 1) 
					break;
				else
				{
					this->project_t(t, this->model_point->fund_shock_pvs, this->fund_shock_number, 1, 1, true);
				}
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}

		// Loop over the lapse shocks
		int lapse_shock_size = this->v_lapse_mult_table.size();
		for(int l = 0; l < lapse_shock_size; l++)
		{
			GMXB_Assumption_Table * m_lapse_tab = this->v_lapse_mult_table[l];
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= this->maturity_yr; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration || this->gmib_election_ind == 1) 
						break;
				else
				{
					int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
					double lapse_mult = m_lapse_tab->search(int(pol_yr), 1)/100.;
					this->project_t(t, this->model_point->pv_lapse, l, lapse_mult,1, false);
				}
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}
		
		// Loop over the mortality shocks
		int mortality_shock_size = this->v_mortality_mult_table.size();
		for(int l = 0; l < mortality_shock_size; l++)
		{
			GMXB_Assumption_Table * m_mortality_tab = this->v_mortality_mult_table[l];
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= this->maturity_yr; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration || this->gmib_election_ind == 1) 
						break;
				else
				{
					int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
					double mortality_mult = m_mortality_tab->search(int(pol_yr), 1)/100.;
					this->project_t(t, this->model_point->pv_mortality, l, 1, mortality_mult, false);
				}
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}

		// perform the longevity shock
		if (this->load_longevity_tables)
		{
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= this->maturity_yr; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration || this->gmib_election_ind == 1) 
						break;
				else
				{
					int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
					int table_age = (int)(this->timing_items[t][AGE_LAST]);
					double mortality_mult = this->longevity_shock_table->search(table_age, pol_yr) ;
					this->project_t(t, this->model_point->pv_longevity, 0, 1, mortality_mult, false);
				}
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}


	}
	virtual ~Projection_IB(void)
	{
		delete this->assump_IB;
	}
}; 


