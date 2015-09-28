#ifndef __PROJECTION_SB_H_
#define __PROJECTION_SB_H_
#include "Projection classes/Guarantee projection classes/Projection.h"
//#include "Assumption classes/Guarantee assumption classes/GMXBAssumption_SB.h"

class Projection_SB: virtual public Projection {
	
public:
	//Assumption
	//Jiangang
	GMXBParameters *assump_SB;

	/*
	* variable used in project_gmsb_bases_payments_claims
	*/
	string gmsb_prem_basis;

	/*
	* variable used in project_gmsb_bases_payments_claims
	*/
	string gmsb_av_type;
	
	/*
	* variable used in project_gmsb_bases_payments_claims
	*/
	double gmsb_av_level;
	
	/*
	* variable used in project_gmsb_bases_payments_claims - initialized in this function at t = 1
	*/
	double gmsb_vested_basis;
	
	/*
	* Table of volatilities
	*/
	GMXB_Assumption_Table * volatility;

	/*
	* variable used in the function dynamic lapse - TO CHECK for others countries
	*/
	double taxes_new_contract;

	GMXB_Assumption_Table *gmsb_guarantee_level;
	

public:
	Projection_SB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
				const char * tables, bool generate_dbg_files, 
						   const string& dbg_file_path, const int t_high_shorter, bool l_longevity_tables, int time_step):
	  Projection (i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path, t_high_shorter, l_longevity_tables, time_step)
	{
		assump_SB = new GMXBParameters(tables, "assump_SB");
		volatility = new GMXB_Assumption_Table(tables, "volatility");
		gmsb_guarantee_level= new GMXB_Assumption_Table(tables, "gmsb_guarantee_level");
		gmsb_prem_basis = this->assump_SB->search("gmsb_prem_basis");
		gmsb_av_type = this->assump_SB->search("gmsb_av_type");
		gmsb_av_level = atof(this->assump_SB->search("gmsb_av_level").c_str());
		taxes_new_contract = atof(this->assump_SB->search("taxes_new_contract").c_str());
	}

	// Jiangang
	virtual ~Projection_SB( void )
	{
		delete this->assump_SB;
		delete this->volatility;
		delete this->gmsb_guarantee_level;
	}

/*
public :

	Projection_SB(char * prod_code, int shock_size, int ** shock_array, GMXBAssumption * assump):
	   Projection (prod_code, shock_size, shock_array, assump)
	{
	};

	// deprecated constructor

	Projection_SB(Model_Point *mp, char * prod_code, int shock_size, int ** shock_array, GMXBAssumption * assump):
	   Projection (mp, prod_code, shock_size, shock_array, assump)
	{
	};
*/


	virtual void project_gmsb_bases_payments_claims(int t)
	{	
			double pol_yr = (int)(this->timing_items[t][POL_YEAR]);
			// *************************************************************************
			// ** MedRegion ABDB Assumptions specific to this function call:
			// ** Assumed to always use gmwb_av_type == "Not Used"
			// ** Assumed to always use gmwb_prem_type == "RETURN"
			// *************************************************************************
			
			double gmsb_rollup_cap_if_bef;
			double av_e_prev = 0.;
			double premium = 0. ;//= this->regular_premium_calculation(t);
			
			double surv_mth = surv_mth_calc(t);
			double death_rate = main_proj_array[t][DEATH_RATE];
			double lapse_rate = lapse_rate_calc(t);

			if (t == 1) 
				{
				if (this->gmsb_prem_basis == "Prem_paid")
					this->gmsb_vested_basis = model_point->prem_paid ;
				else if (this->gmsb_prem_basis == "GMAB_valn")	
					this->gmsb_vested_basis = model_point->gmab_rollup_valn ;
				}

			if (this->gmsb_av_type == "Not Used")//Switzerland
				{
					gmsb_rollup_cap_if_bef = 0.;
					main_proj_array[t][GMSB_PREM_IF] = this->gmsb_vested_basis * 
						this->gmsb_guarantee_level->search((int)(this->timing_items[t][POL_YEAR]), 1)/100 * surv_cumul[t-1]; 

					if (pol_yr > this->product_charging_duration)
					{
						main_proj_array[t][GMSB_CLAIMS] = 0.0;
					}
					else
					{
						main_proj_array[t][GMSB_CLAIMS] = max(0.0, main_proj_array[t][GMSB_PREM_IF] - main_proj_array[t][AV_E_BEF]) * (1 - death_rate) * lapse_rate;
					}
					main_proj_array[t][GMSB_GUAR_PAYMENT] = main_proj_array[t][GMSB_PREM_IF] * (1 - death_rate);									
				}
			else if (this->gmsb_av_type == "RESET")// Canada
				{
					if (t == 1)
						main_proj_array[t][GMSB_PREM_IF] = this->gmsb_vested_basis * this->gmsb_guarantee_level->search((int)(this->timing_items[t][POL_YEAR]), 1)/100; 
				
					else if (true)//mod(pol_yr , gmsb_av_period) == 1) 
					{
						main_proj_array[t][GMSB_PREM_IF] = max(av_e_prev * this->gmsb_av_level/*()*//100.0, main_proj_array[t-1][GMSB_PREM_IF]) + premium;		
					}
					else 
						main_proj_array[t][GMSB_PREM_IF] = main_proj_array[t-1][GMSB_PREM_IF] * surv_mth;;

					if ((pol_yr <= this->product_charging_duration) && ((int)pol_yr % (int)model_point->gmsb_period) == 0) 
						// main_proj_array_2[GMSB_CLAIMS][t] = max(0.0, main_proj_array_2[GMSB_PREM_IF][t] - main_proj_array_2[AV_E_BEF][t][CALC_TOT_INDEX]) * (1 - death_rate) * lapse_rate;		
						main_proj_array[t][GMSB_CLAIMS] = max(0.0, main_proj_array[t][GMSB_PREM_IF] - main_proj_array[t][AV_E_BEF]) * (1 - death_rate) * lapse_rate;		
					else
						main_proj_array[t][GMSB_CLAIMS] = 0.0;
				
					main_proj_array[t][GMSB_GUAR_PAYMENT] = main_proj_array[t][GMSB_PREM_IF] * (1 - death_rate);					
				}
	}
};

#endif