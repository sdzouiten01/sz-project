#pragma once


#include "../Utility classes/stdafx.h"

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include "../Utility classes/Product_configuration.h"
#include "../Utility classes/Scenario_configuration.h"
#include "WildcardManager.h"
#include "GMXBExcelXMLReader.h"


//
//#include "GMXBExcelXMLReader.h"
using namespace std;


//const unsigned int scenario_list_nCol = 1; // [0]=ESG_scenario_config, [1]=ESG_calibration_config
class Scenario_list
{
	map<int,vector<string>> map_scnPn_ESGCalibPath;
 
public:
	Scenario_list();
	Scenario_list(string& parameter_file);
	string get_ESG_config_path(int scenario_pn_ip);
};


//const int bucket_col_size = 6;  // contain: bucket_type, _start, _key, _end, _size 

class Bucket_list
{
   vector<int>            bucket_list_id_;
   vector<string>         bucket_list_type_;
   vector<vector<double>> bucket_list_contenance_;
   vector<bool>           bucket_list_if_calib_;

public:
	//! constructor/ destructor
	//! TO DO:  copy_constructor/assignment need to be offered
	Bucket_list(){}; 
	Bucket_list(string& parameter_file);
   ~Bucket_list(){};
  
	string                 get_bucket_list_type(int id_bucket);
    vector<double>         get_bucket_list_contenance(int id_bucket);
	bool                   get_bucket_list_if_calib(int id_bucket);


};



class Regroup_by_Scenario_PN
{
	int scenario_pn; 
	vector<int> bucket_id_list_;
	vector<vector<int>>	product_index_list_; 
	

public:
    
	bool test_regroup_condition(int scenario_pn_ip);

	Regroup_by_Scenario_PN(int scenario_pass_number,vector<Product_configuration>& product_config);
	
	vector<int> get_bucket_id_list();
	vector<int>& get_bucket_id_list_reference();

	vector<vector<int>> get_product_index_list();
	vector<vector<int>>& get_product_index_list_reference();

	int get_scenario_pn(){return scenario_pn;}
};



class Dispatch_Manager
{
    vector<Regroup_by_Scenario_PN> regrouped_scenario_pns;
	Scenario_list				   scenario_list_table;
	Bucket_list					   bucket_list_table;

public:
	//! no copy-constructor/assignment/destructor: Rule of three
	Dispatch_Manager(std::vector<Product_configuration>& product_config, string& parameter_file);

	vector<Regroup_by_Scenario_PN>& get_regrouped_scenario_pns_reference();
	vector<Regroup_by_Scenario_PN> get_regrouped_scenario_pns();

	Scenario_list  get_Scenario_list();
	Scenario_list& get_Scenario_list_reference();

	Bucket_list  get_Bucket_list();
	Bucket_list& get_Bucket_list_reference();
	static vector<int> dispatch_per_scenario(int nb_workers, int id_worker, int number_of_scenarios);
};
