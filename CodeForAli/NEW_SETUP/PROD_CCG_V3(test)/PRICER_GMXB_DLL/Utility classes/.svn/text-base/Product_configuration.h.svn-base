#pragma once
#include "stdafx.h"
#include <string>
#include "Utility classes/Scenario_configuration.h"
#include "Utility classes/File_Manager.h"
#include "Fund_Manager.h"

using namespace std;

class Product_configuration
{

private :
	string run_name;
	string model_point_file;
	string config_file;
	string prod_code_key;
	int tot_number_mps;
	bool extract_all_mp;
	bool generate_debug_files;
	vector<int> scen_index_col;
	string result_file_path;
	string debug_file_path;
	int number_of_scenarios_to_run;
	int yield_col_for_pv_index; 
	int lapse_dyn_treas; 
	int rates_indic_index;
	int index_number;
	int write_valo_policy_by_policy;
	bool project_funds;
	bool do_longstaff;
	int modulo_on_scenarios;
	vector<Scenario_configuration> scenarios_configuration;
	int shortsellable_group;
	int time_step;

	string assumption_output_directory;

	string fund_split_file;
	string mgmt_fee_file;
	string fund_valo_file;

	map<string, Fund> map_of_funds;

	string debug_file;
	string proba_file;
	string proba_ls_file;



private:

	void check_file_path()
	{
		File_Manager f_mgr;
		f_mgr.is_file_path_valid(this->fund_split_file);
		f_mgr.is_file_path_valid(this->mgmt_fee_file);
		f_mgr.is_file_path_valid(this->fund_valo_file);
	}

	void build_map_of_funds()
	{
		Fund_Manager* fund_mgr = Fund_Manager::getInstance();
		fund_mgr->build_map_of_funds(this->mgmt_fee_file, this->fund_split_file, this->fund_valo_file, map_of_funds);
	}

public :

	~Product_configuration()
	{
	}


	/*
	* Constructor used for the stat inforce
	*/
	Product_configuration(const string& pCK, const string& mp_file)
	{
		this->prod_code_key = pCK;
		this->model_point_file = mp_file;
		this->project_funds = false;
	}


	Product_configuration(const string& r_name, const string& mp_file, const string& conf_file, const string& pCK,
					int nb_mp, bool extract_all, vector<int>& sc_index_col,
					const string& r_file_path, const string& d_file_path,
					const string& f_split_param_file, const string& m_fees_param_file,
					const string& f_valo_file, const string& assumption_out_dir, 
					int nb_of_scenarios_to_run,int y_col_for_pvs, 
					int l_dyn_treas, int r_indic_index, int i_number, bool g_debug_files,
					int write_val_pol_by_pol, int modulo, bool do_projection_by_funds, bool apply_longstaff, int group_shortsell, int t_step
					)
	{

		this->run_name = r_name;

		this->model_point_file = mp_file;
		this->config_file = conf_file;
		this->prod_code_key = pCK;
		this->tot_number_mps = nb_mp;
		this->extract_all_mp = extract_all;

		int scen_index_columns_size = sc_index_col.size();
		
		for (int i =0 ; i < scen_index_columns_size ; i++)
			this->scen_index_col.push_back(sc_index_col[i]);

		this->result_file_path = r_file_path;
		this->debug_file_path = d_file_path;

		this->number_of_scenarios_to_run = nb_of_scenarios_to_run;
		this->yield_col_for_pv_index = y_col_for_pvs;
		this->lapse_dyn_treas = l_dyn_treas;
		this->rates_indic_index = r_indic_index;
		this->index_number = i_number;
		this->generate_debug_files = g_debug_files;
		this->write_valo_policy_by_policy = write_val_pol_by_pol;
		this->modulo_on_scenarios = modulo;

		this->project_funds = do_projection_by_funds;
		this->do_longstaff = apply_longstaff;
		this->shortsellable_group = group_shortsell;
		this->assumption_output_directory = assumption_out_dir;

		this->fund_split_file = f_split_param_file;
		this->mgmt_fee_file = m_fees_param_file;
		this->fund_valo_file = f_valo_file;
		this->time_step = t_step;

		check_file_path();

		if (this->project_funds)
			this->build_map_of_funds();

	}

	void add_scenario_configuration(Scenario_configuration& scen_conf)
	{
		scenarios_configuration.push_back(scen_conf);
	}


	vector<Scenario_configuration> get_scenario_list()
	{
		return this->scenarios_configuration;
	}

	int get_number_of_scenarios_to_run()
	{
		return this->number_of_scenarios_to_run;
	}

	vector<int> get_scenario_index_columns()
	{
		return this->scen_index_col;
	}

	int get_yield_columns_for_pv()
	{
		return this->yield_col_for_pv_index;
	}

	int get_lapse_dyn_treas()
	{
		return this->lapse_dyn_treas;
	}

	int get_rates_indic_index()
	{
		return this->rates_indic_index;
	}

	string get_model_point_file()
	{
		return this->model_point_file;
	}
	
	int get_tot_number_mps()
	{
		return this->tot_number_mps;
	}

	bool get_extract_all_mp()
	{
		return this->extract_all_mp;
	}

	bool get_generate_debug_file_indic()
	{
		return this->generate_debug_files;
	}

	string get_prod_code_key()
	{
		return this->prod_code_key;
	}

	string get_config_file()
	{
		return this->config_file;
	}

	string get_result_file_path()
	{
		return this->result_file_path;
	
	}

	string get_debug_file_path()
	{
		return this->debug_file_path;
	
	}

	string get_run_name()
	{
		return this->run_name;
	
	}

	int get_index_number()
	{
		return this->index_number;
	}

	int get_write_valo_policy_by_policy()
	{
		return this->write_valo_policy_by_policy;
	}

	int get_modulo_number_on_scenario()
	{
		return this->modulo_on_scenarios;
	}

	bool is_projection_by_fund()
	{
		return this->project_funds;
	}

	bool is_longstaff()
	{
		return this->do_longstaff;
	}

	int get_shortsellable_group()
	{
		return this->shortsellable_group;
	}

	string get_assumption_output_directory()
	{
		return this->assumption_output_directory;
	}

	string get_fund_split_file()
	{
		return this->fund_split_file;
	}

	string get_mgmt_fee_file()
	{
		return this->mgmt_fee_file;
	}

	string get_fund_valo_file()
	{
		return this->fund_valo_file;
	}

	int get_number_of_funds()
	{
		return this->map_of_funds.size();
	}

	int get_time_step()
	{
		return this->time_step;
	
	}
	vector<int> get_list_of_fund_id_by_group_id(int short_sell_group_id)
	{
		vector<int> list_fund_id;
		for (map<string, Fund>::const_iterator iter_map_fund_ids = this->map_of_funds.begin();
			iter_map_fund_ids != this->map_of_funds.end(); ++iter_map_fund_ids)
		{
			Fund f = iter_map_fund_ids->second;
			if (f.check_group_shortsell_id(short_sell_group_id))
				list_fund_id.push_back((int)(f.get_fund_id()));
		}
		return list_fund_id;
	}


	string get_fund_name_by_fund_id(int fund_id)
	{

		for (map<string, Fund>::const_iterator iter_map_fund_ids = this->map_of_funds.begin();
			iter_map_fund_ids != this->map_of_funds.end(); ++iter_map_fund_ids)
		{

			if (iter_map_fund_ids->second.fund_id == fund_id)
			{
				string fund_name = iter_map_fund_ids->first;
				return fund_name;
			}
		}

		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The fund_id %d is not found in the list of funds", fund_id);
		throw new GMXBException(message);

	}

	Fund& get_fund_by_name(const string& fund_to_find)
	{
		for (map<string, Fund>::const_iterator iter_map_fund_ids = this->map_of_funds.begin();
			iter_map_fund_ids != this->map_of_funds.end(); ++iter_map_fund_ids)
		{
			string fund_name = iter_map_fund_ids->first;

			if (fund_name == fund_to_find)
				return this->map_of_funds[fund_name];
		}

		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The fund %s is not find in the list of funds", fund_to_find.c_str());
		throw new GMXBException(message);								
	}

	//risk appetite
	void RA_set_inforce_name()
	{
		string output_path = this->get_model_point_file();
		int position = output_path.find_last_of('\\');
		this->model_point_file = output_path.substr(0,position) + "\\Risk Appetite_" + this->get_run_name() + ".csv";
	}

	void RA_set_run_name()
	{
		this->run_name=this->run_name+"_RA";
	}

	string get_debug_file()
	{
		return 	this->debug_file;
	}

	string get_proba_file()
	{
		return 	this->proba_file;
	}
	
	string get_proba_ls_file()
	{
		return 	this->proba_ls_file;
	}

	void set_longstaff_files(string debug_file, string proba_file, string proba_ls_file)
	{
		this->debug_file = debug_file ;
		this->proba_file = proba_file;
		this->proba_ls_file = proba_ls_file;
	}

};