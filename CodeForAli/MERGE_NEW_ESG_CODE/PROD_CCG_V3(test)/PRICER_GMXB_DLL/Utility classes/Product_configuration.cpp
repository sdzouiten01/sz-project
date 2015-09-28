#include "Product_configuration.h"



Product_configuration::~Product_configuration(){}
	/** Constructor used for the stat inforce*/
Product_configuration::Product_configuration(const string& pCK, const string& mp_file)
	{
		this->prod_code_key = pCK;
		this->model_point_file = mp_file;
	}
Product_configuration::Product_configuration(const string& r_name, const string& mp_file, const string& conf_file, const string& pCK,int nb_mp, bool extract_all, vector<int>& sc_index_col,
					const string& r_file_path, const string& d_file_path, int nb_of_scenarios_to_run,int y_col_for_pvs, int l_dyn_treas, int r_indic_index, 
					int i_number, bool g_debug_files,int write_val_pol_by_pol, /*int modulo,*/  int t_step)
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
		/*this->modulo_on_scenarios = modulo;*/

		this->time_step = t_step;

	}
void Product_configuration::add_scenario_configuration(Scenario_configuration& scen_conf){scenarios_configuration.push_back(scen_conf);}
vector<Scenario_configuration> Product_configuration::get_scenario_list(){return this->scenarios_configuration;}
int Product_configuration::get_number_of_scenarios_to_run(){return this->number_of_scenarios_to_run;}
vector<int> Product_configuration::get_scenario_index_columns(){return this->scen_index_col;}
int Product_configuration::get_yield_columns_for_pv(){return this->yield_col_for_pv_index;}
int Product_configuration::get_lapse_dyn_treas(){return this->lapse_dyn_treas;}
int Product_configuration::get_rates_indic_index(){return this->rates_indic_index;}
string Product_configuration::get_model_point_file(){return this->model_point_file;}
int Product_configuration::get_tot_number_mps(){return this->tot_number_mps;}
bool Product_configuration::get_extract_all_mp(){return this->extract_all_mp;}
bool Product_configuration::get_generate_debug_file_indic(){return this->generate_debug_files;}
string Product_configuration::get_prod_code_key(){return this->prod_code_key;}
string Product_configuration::get_config_file(){return this->config_file;}
string Product_configuration::get_result_file_path(){return this->result_file_path;}
string Product_configuration::get_debug_file_path(){return this->debug_file_path;}
string Product_configuration::get_run_name(){return this->run_name;}
int Product_configuration::get_index_number(){return this->index_number;}
int Product_configuration::get_write_valo_policy_by_policy(){return this->write_valo_policy_by_policy;}
//int Product_configuration::get_modulo_number_on_scenario(){return this->modulo_on_scenarios;}
int Product_configuration::get_time_step(){return this->time_step;}
string Product_configuration::get_debug_file(){return 	this->debug_file;}

