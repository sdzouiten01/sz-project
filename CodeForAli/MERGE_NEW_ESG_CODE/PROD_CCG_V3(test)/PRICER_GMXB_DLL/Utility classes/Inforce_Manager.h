#pragma once

#include "../Utility classes/stdafx.h"
#include <sstream>
#include "GMXBException.h"
#include "../Model point classes/Model_Point.h"
//#include "Utility classes/Builder.h"
#include "../Utility classes/Product_configuration.h"
/*
*	This class manages the inforce (file containing all the insurance policies)
*	It reads the inforce file, create the appropriate model points according to the inforce file
*	and finally build a vector of Model_point objects
*	This class also manages the split of the inforce file (to be able to make runs using several machines)
*	string model_point_file_name : inforec file path and name
*	const char * product_code_key : product code keys to extract from the inforce file
*	int index_shock_size : Number of indexes shocks (necessary to resize the index_pvs)
*	int fund_shock_size : Number of funds shocks (necessary to resize the fund_pvs)
*	vector<Model_Point*> mp_vector: vector of Model points (one model_point is added in the vector each time we read one row of the inforce file)
*	int tot_nb_workers : Total number of workers (Necessary for the dispatching)
*	int w_id : The current worker id (Necessary for the dispatching)
*	int l_size : Number of lapse shocks (necessary to resize the lapse_pv)
*   int mort_size : Number of mortality shocks (necessary to resize the mort_pv)
*	int lon_size :  Number of longevity shocks (necessary to resize the lon_pv)
*/
class Inforce_Manager
{

private :
	vector<Model_Point*> mp_vector;
	int tot_nb_workers; 
	int w_id;
	//HE Debug
	string** inforce;
	int nb_mp;
	bool is_he;

	// Actuarial tool
	bool is_ra;
	bool is_ap;
	vector<vector<string>> inforce_ap;


	static int get_number_of_columns_in_row(const string& row);
	void extract_model_points_from_file(Product_configuration& product_config, Scenario_configuration & scenario_config, char separator);

public:

	int nb_model_points;
	int number_of_columns;
	~Inforce_Manager();
	Inforce_Manager();
	Inforce_Manager(char separator, int total_number_of_workers, int worker_id, Product_configuration& product_config,
					Scenario_configuration& scenario_config, bool is_HE, bool is_AP, bool is_RA);

	// Add by Yaokun
	// 04-02-2010
//#pragma region Updated Inforce Manager
	Inforce_Manager(int total_number_of_workers, int worker_id, bool is_HE, bool is_AP, bool is_RA);

	static void Create_Inforce_Managers(char separator, int total_number_of_workers, int worker_id, vector<Product_configuration>& vec_prod_cfg,
		Scenario_configuration& scen_cfg, bool is_HE, bool is_AP, bool is_RA, map<string, Inforce_Manager*>& inforce_mgr_map);

static void extract_model_points_from_file(char separator, vector<Product_configuration*>& vec_prod_cfg, Scenario_configuration& scen_cfg, 
										   vector<Inforce_Manager*>& inforce_mgrs);
//#pragma endregion

	void get_list_of_model_points_per_product(const string& product_code, vector<Model_Point*>& mps, Product_configuration& product_config,
											Scenario_configuration& scenario_config);

	void get_list_of_all_model_points(const string& product_code, vector<Model_Point*>& mps, Product_configuration& product_config,
											Scenario_configuration& scenario_config);

	void get_list_of_model_points_per_product_and_gmxb_type(const string& product_code,string& gmxb_type,vector<Model_Point*>& mps,Product_configuration& product_config,
															Scenario_configuration& scenario_config);
	vector<Model_Point*>& get_model_points_vector();

	string** get_inforce();

	int get_nb_mp();

	// For actuarial tool
	void set_ap_indicator (bool ind);

	vector<vector<string>>& get_inforce_ap(void);
};

