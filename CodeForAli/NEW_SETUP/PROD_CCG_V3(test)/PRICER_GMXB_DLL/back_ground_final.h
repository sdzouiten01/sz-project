#pragma once
#include "Utility classes/stdafx.h"
#include <string>
#include <vector>
#include <iostream>

#include "Projection classes/Projection.h"
#include "Utility classes/Dispatch_Manager.h"

extern ofstream trace;
extern ofstream trace_out;
extern ofstream trace_in;

using namespace std;

class back_ground
{

private:
	Projection * projection;

public:
	~back_ground();
	back_ground();
	//Projection with Scenario file
	back_ground(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id, string& trace_log_pth,double alpha, double sigma);
	// ESG and Projection by the DLL
	back_ground (vector<Product_configuration>& product_config,string& trace_log_pth,ofstream& log_strm,int nbWorker,int idWorker,Dispatch_Manager& dispatch_manager);
	// Entry point of aging policies
	back_ground(vector<Product_configuration>&  product_configurations_out,  int tot_nb_workers,int w_id, string& trace_log_pth_out);

private:

	void create_payoff(const string& pCK, const char * assumption_file_name,const vector<vector <int>>& i_s_a, bool generate_debug_files, const string& debug_file_path);
	/** Projection by the DLL with a scenario as input*/
	void run_dll_projection_file_V2(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id, double alpha, double sigma);
	void run_dll_projection_esg_V2(vector<Product_configuration>& product_config,ofstream& log_strm,int tot_nb_workers,int w_id,Dispatch_Manager& dispatch_manager);
};
