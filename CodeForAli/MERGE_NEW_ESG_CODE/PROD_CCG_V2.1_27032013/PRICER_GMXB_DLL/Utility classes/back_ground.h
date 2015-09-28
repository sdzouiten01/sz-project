#ifndef __BACK_GROUND_H_
#define __BACK_GROUND_H_
#include "stdafx.h"
#include <string>
#include "Projection classes/Guarantee projection classes/Projection.h"
#include "GMXBLogger.h"
#include "GMXBException.h"
#include "Scenario_manager.h"
#include <iostream>

#include "Projection classes/Country projection classes/Projection_ITA_AB_DB.h"
#include "Projection classes/Country projection classes/Projection_ITA_WB_DB.h"
#include "Projection classes/Country projection classes/Projection_ESP_WB_DB.h"
#include "Projection classes/Country projection classes/Projection_ESP_AB_DB.h"
#include "Projection classes/Country projection classes/Projection_CHE_AB_DB_SB.h"

#include "Utility classes/Inforce_Manager.h"


using namespace std;

extern struct shock {
	int index;
	double value;
	char * name;
}; 

class back_ground
{

private:
	Projection * projection;
	Model_Point * mp;
	// Inforce variable
	Inforce_Manager *if_manager;

	scenario_manager * sm;
	GMXBAssumption * assumption;
	
	GMXBLogger *log_file;
	int number_of_product_to_run;

	// projection fields
	string *prod_code_keys;
	string *inforce_file_names; 
	string *total_number_model_point;
	int ** shock_array;
	BOOL * extract_all_mp;

	// scenario manager fields
	char * scenario_file_name;
	int number_of_scenario_to_run;
	int *scen_fund_col;
	int yield_columns_for_pvs;
	int lapse_dyn_treas;
	int rates_indic_index;
	// int number_index;
	char scenario_file_separator;
	int shock_size;
	bool use_scenario_file;
	
	double *** va_scenario;


	// Assumption set fields
	string * assumption_set_file;

	// Table of results
	double *** results;
	string result_file_path;

	// Worker fields
	int tot_number_of_workers;
	int worker_id;

	// TO DO temporary variable

	double *** table_of_pvs;



	ofstream * l_strm;

public:
	~back_ground()
	{
	}
	
	back_ground()
	{
	}

	back_ground(string *pCK, string *if_file_name, int ** s_a, int ss
		, string *tot_number_mps, BOOL *ex_all_mp,  char * scen_file_name, int nb_of_scenario_to_run, int * s_fund_col,
		int y_columns_for_pvs, int l_dyn_treas, int r_indic_index, 
		char separator, string *assump_set_file, int nb_product,
		string r_path, string debug_file_path, int nb_tot_workers, int w_id, ofstream& log_strm, double *** pvs)
	{

		this->number_of_product_to_run = nb_product;

		this->prod_code_keys = pCK;
		this->inforce_file_names = if_file_name;
		this->shock_array = s_a;
		this->shock_size = ss;
		this->total_number_model_point = tot_number_mps;
		this->extract_all_mp = ex_all_mp;

		this->scenario_file_name = scen_file_name;
		this->use_scenario_file = true;
		this->number_of_scenario_to_run = nb_of_scenario_to_run;
		this->scen_fund_col = s_fund_col;
		this->yield_columns_for_pvs = y_columns_for_pvs;
		this->lapse_dyn_treas = l_dyn_treas;
		this->rates_indic_index = r_indic_index;

		this->scenario_file_separator = separator;

		this->assumption_set_file = assump_set_file;
		
		this->result_file_path = r_path;
		char log_name[100];
		sprintf_s(log_name,"%s%s", debug_file_path.c_str(),"trace.log");
		this->log_file = new GMXBLogger(log_name);
		this->tot_number_of_workers = nb_tot_workers;
		this->worker_id = w_id;


		this->l_strm = &log_strm;

		this->table_of_pvs = pvs;

		
	}

	back_ground(string *pCK, string *if_file_name, int ** s_a, int ss
		, string *tot_number_mps, BOOL *ex_all_mp,  double *** va_scen, int nb_of_scenario_to_run, 
		int * s_fund_col, int y_columns_for_pvs, int l_dyn_treas, int r_indic_index, 
		char separator, string *assump_set_file, int nb_product,
		string r_path, string debug_file_path, int nb_tot_workers, int w_id, ofstream& log_strm, double *** pvs)
	{

		this->number_of_product_to_run = nb_product;

		this->prod_code_keys = pCK;
		this->inforce_file_names = if_file_name;
		this->shock_array = s_a;
		this->shock_size = ss;
		this->total_number_model_point = tot_number_mps;
		this->extract_all_mp = ex_all_mp;
		
		this->va_scenario = va_scen;
		this->use_scenario_file = false;
		this->number_of_scenario_to_run = nb_of_scenario_to_run;
		this->scen_fund_col = s_fund_col;
		this->yield_columns_for_pvs = y_columns_for_pvs;
		this->lapse_dyn_treas = l_dyn_treas;
		this->rates_indic_index = r_indic_index;
		// this->number_index = nb_index;
		this->scenario_file_separator = separator;

		this->assumption_set_file = assump_set_file;
		
		this->result_file_path = r_path;
		char log_name[100];
		sprintf_s(log_name,"%s%s", debug_file_path.c_str(),"trace.log");
		this->log_file = new GMXBLogger(log_name);
		this->tot_number_of_workers = nb_tot_workers;
		this->worker_id = w_id;


		this->l_strm = &log_strm;

		this->table_of_pvs = pvs;

		
	}





	//function for setting the arrays in projection to zero
private:
	void initialize_result_table(int shock_nb, int number_of_mp)
	{
		this->results = new double ** [shock_nb];
		for(int j =0 ; j < shock_nb; j++)
		{
			results[j] = new double *[number_of_mp + 1];
			for(int i =0; i<= number_of_mp ; i++)
			{
				results[j][i] = new double [TOTAL_OUTPUT_COLUMNS];
				for(int k=0;k<TOTAL_OUTPUT_COLUMNS; k++)
				{
					results[j][i][k] = 0.0;
				}
			}
		}
	}

	void write_results()
	{
		this->table_of_pvs = new double**[this->if_manager->nb_model_points + 1];
		for (int i = 0; i < this->if_manager->nb_model_points + 1; i++)
		{
			this->table_of_pvs[i] = new double*[TOTAL_OUTPUT_COLUMNS];
				for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				{
					this->table_of_pvs[i][j] = new double[this->shock_size];
						for (int k = 0; k < this->shock_size ; k++)	
							this->table_of_pvs[i][j][k] = 0.;
				}
		}

		double total_beg_av = 0.;
		double total_pv_gmdb = 0.;
		double total_pv_gmwb = 0.;
		double total_pv_gmab = 0.;
		double total_pv_gmib = 0.;
		double total_pv_gmsb = 0.;
		double total_pv_real_chg = 0.;
		double total_pv_ehc_chg = 0.;
		double total_pv_dep_fee = 0.;

		for (int mp = 1; mp < this->if_manager->nb_model_points + 1; mp++)
		{
			Model_Point model_point = this->if_manager->get_model_points_vector()[mp -1];
			for(int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
			{
				for (int k = 0; k < model_point.shock_size ; k++)
				{
					this->table_of_pvs[mp][j][k] = model_point.pv[j][k];
					this->table_of_pvs[0][j][k] += this->table_of_pvs[mp][j][k];
				}
			}
		}

	}

	// TO DO change this function
	void write_res( char *output_filename)
	{
		ofstream output;
		(*l_strm) << " output file path " << output_filename << endl;
		output.open(output_filename);
		for (int i = 0; i< this->if_manager->nb_model_points; i++)
		{
			Model_Point mp = this->if_manager->get_model_points_vector()[i];
			for (int j = 0; j < mp.shock_size ; j++)
			{
				for (int col = 0; col < TOTAL_OUTPUT_COLUMNS; col++)
					output << setprecision(15) <<  mp.pv[j][col] << ",";
			
			output << endl;
			}
		}
		output.close();
	}

	void create_inforce_manager(string mp_file_name, int nb_of_mp, bool retrieve_all_mp, char separator, 
								const char * pCK, int ss, int total_number_of_workers, int worker_id)
	{
		this->if_manager = new Inforce_Manager(mp_file_name, nb_of_mp, retrieve_all_mp, separator, pCK, ss, 
							total_number_of_workers, worker_id);
	}

	void create_scenario()
	{
		
		if (this->use_scenario_file)
		{

			this->sm = new scenario_manager(this->scenario_file_name, this->number_of_scenario_to_run, this->scen_fund_col, 
				this->yield_columns_for_pvs, this->lapse_dyn_treas, this->rates_indic_index, 
				this->scenario_file_separator, this->log_file);
		}
		else
		{
			this->sm = new scenario_manager(this->va_scenario, this->number_of_scenario_to_run, this->scen_fund_col, 
				this->yield_columns_for_pvs, this->lapse_dyn_treas, this->rates_indic_index, this->log_file);
		
		}
	}

	void create_projection(string pCK, const char * assumption_file_name)
	{
		Projection * pro;

		if (pCK == "ITA_WDB01")
		{
			GMXBAssumption *assump = new GMXBAssumption_WB_DB(assumption_file_name);
			pro = new Projection_ITA_WB_DB(this->mp, this->shock_size, shock_array, assump);
		}
		else if (pCK == "ITA_ADB01")
		{
			GMXBAssumption *assump = new GMXBAssumption_AB_DB (assumption_file_name);
			pro = new Projection_ITA_AB_DB(mp, this->shock_size, shock_array, assump);
		}
		else if (pCK == "ESP_WDB01")
		{
			GMXBAssumption *assump = new GMXBAssumption_WB_DB (assumption_file_name);
			pro = new Projection_ESP_WB_DB(mp, this->shock_size, shock_array, assump);
		}	
		else if (pCK == "ESP_ADB01")
		{
			(*l_strm) << " Before assumption creation " << endl;
			GMXBAssumption *assump = new GMXBAssumption_AB_DB (assumption_file_name);
			(*l_strm) << " After assumption creation " << endl;
			(*l_strm) << " Before projection creation " << endl;
			pro = new Projection_ESP_AB_DB(mp, this->shock_size, shock_array, assump);
			(*l_strm) << " After projection creation " << endl;
		}
		else if (pCK == "CHE_ADB01")
		{
			GMXBAssumption *assump = new GMXBAssumption_AB_DB (assumption_file_name);
			pro = new Projection_CHE_AB_DB_SB(mp, this->shock_size, shock_array, assump);
		}
		else 
		{
			char message[1000];
			sprintf_s(message, "The prod code key %s is not found", pCK.c_str());
			throw new GMXBException(message);
		}
		this->projection = pro;
	}

	void check_file_path(const char * file_name)
	{
		ifstream file_to_check(file_name, ios::in);
		if (!file_to_check)
		{ 
			char message[1000];
			sprintf_s(message, "The file %s is not found", file_name);
			throw new GMXBException(message);
		}
		file_to_check.close();
	}

public:
	void run()
	{	
	
		(*l_strm) << " Begining background "  << endl;
			
		this->create_scenario();

		for (int run_number = 0 ; run_number < this->number_of_product_to_run ;  run_number++)
		{

			this->create_inforce_manager(this->inforce_file_names[run_number].c_str(),
										atoi(this->total_number_model_point[run_number].c_str()),
										this->extract_all_mp[run_number],',',
										this->prod_code_keys[run_number].c_str(),
										this->shock_size, this->tot_number_of_workers, this->worker_id); 


			this->create_projection(this->prod_code_keys[run_number], this->assumption_set_file[run_number].c_str());


			for( int j=0; j < this->if_manager->nb_model_points ; j++ )
			{
				this->projection->set_current_model_point(&this->if_manager->get_model_points_vector()[j]);
				this->projection->load_account_vars();
				this->projection->initialise_fund_map();
				
			
				for(int i=0; i< this->number_of_scenario_to_run ; i++)
				{
					this->projection->set_annual_from_monthly(sm->get_ann_from_mthly_rates(i+1));
					this->projection->project();
				}
			}

		// TO DO change this function write the results 
		// char* output_file = new char[100];
		// sprintf(output_file, "%s%s%d%s%d%s%d%s%d%s", result_file_path.c_str(), 
		// 	this->prod_code_keys[run_number].c_str(),this->sm->get_nb_scenarios(), "_scenaris_", 
		// 	this->if_manager->nb_model_points, "_modelpoints_", this->shock_size, "_shocks_worker_", 
		//	this->worker_id ,".csv");
		// write_res(output_file);
		

			write_results();

		}
	}
};

#endif