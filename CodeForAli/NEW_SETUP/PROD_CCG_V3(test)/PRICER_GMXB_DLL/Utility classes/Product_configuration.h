#pragma once
#include "../Utility classes/stdafx.h"
#include <string>
#include "../Utility classes/Scenario_configuration.h"

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
	//int modulo_on_scenarios;
	vector<Scenario_configuration> scenarios_configuration;
	int time_step;


	string debug_file;

public :

	~Product_configuration();
	/** Constructor used for the stat inforce*/
	Product_configuration(const string& pCK, const string& mp_file);
	Product_configuration(const string& r_name, const string& mp_file, const string& conf_file, const string& pCK,int nb_mp, bool extract_all, vector<int>& sc_index_col,
					const string& r_file_path, const string& d_file_path, int nb_of_scenarios_to_run,int y_col_for_pvs, int l_dyn_treas, int r_indic_index, 
					int i_number, bool g_debug_files,int write_val_pol_by_pol, /*int modulo,*/  int t_step);

	void add_scenario_configuration(Scenario_configuration& scen_conf);
	vector<Scenario_configuration> get_scenario_list();
	int get_number_of_scenarios_to_run();
	vector<int> get_scenario_index_columns();
	int get_yield_columns_for_pv();
	int get_lapse_dyn_treas();
	int get_rates_indic_index();
	string get_model_point_file();
	int get_tot_number_mps();
	bool get_extract_all_mp();
	bool get_generate_debug_file_indic();
	string get_prod_code_key();
	string get_config_file();
	string get_result_file_path();
	string get_debug_file_path();
	string get_run_name();
	int get_index_number();
	int get_write_valo_policy_by_policy();
	//int get_modulo_number_on_scenario();
	int get_time_step();
	string get_debug_file();

};