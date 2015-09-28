#pragma once

extern ofstream trace;
using namespace std;

#include "Utility classes/Dispatch_Manager.h"
#include "Utility classes/usefule_function.h"
#include "Utility classes/head_summary_MSO.h"
#include <algorithm>



bool compare_irshoc(vector<double> vec_a, vector<double> vec_b)
{
	return (vec_a[0]==vec_b[0]);
}
class Summary_File_Writer
{
protected :
	int number_of_workers;
	Summary_configuration *summary_configuration;
	
	map<int, vector<vector<int>>> index_map_shocks;
	map<int, vector<vector<int>>> fund_map_shocks;

	map<int, map<int, vector<vector<int>>>> index_map_shocks_esg;
	map<int, map<int, vector<vector<int>>>> fund_map_shocks_esg;

	//! add YYY
    map<int, map<int,vector<string>>> lapse_map_shocks_esg;
	map<int, map<int,vector<string>>> mortality_map_shocks_esg;
	map<int, map<int,vector<string>>> paidup_map_shocks_esg;
	map<int, map<int,vector<string>>> election_map_shocks_esg;
	map<int, map<int,bool>>           longevity_map_esg;

	vector<GMXB_Assumption_Table *> v_mortality;
	
	map<string, vector<string>> list_scenario_names;
	map<string, vector<string>> list_index_shock_names;
	Dispatch_Manager* dispatch_manager;
	bool if_multi_esg;   
	string prod_code_key;
	double nb_of_polices;
	double nb_total_polices;
	double nb_of_policies_NB;
	double nb_of_policies_recalc_old;
	double nb_of_policies_recalc_new;

	vector<vector<double>> get_pvs_vector(const string& product_code_key, int scen_pass_num, 
				int shock_number, const map<string, map<int,map<int, vector<vector<double>>>>>& results_map)
	{
		for(map<string, map<int,map<int, vector<vector<double>>>>>::const_iterator iter_pvs = results_map.begin(); 
			iter_pvs != results_map.end(); ++iter_pvs)
		{
			string pCK = iter_pvs->first;
			if (pCK == product_code_key)
			{
				map<int,map<int, vector<vector<double>>>> map_scen_pass_num = iter_pvs->second;
				for(map<int,map<int, vector<vector<double>>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
					iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
				{
					if (iter_scen_pass_num->first == scen_pass_num)
					{
						map<int, vector<vector<double>>> map_shocks = iter_scen_pass_num->second;
						for(map<int, vector<vector<double>>>::const_iterator iter_shocks = map_shocks.begin();
							iter_shocks != map_shocks.end(); ++iter_shocks)
						{
							if (iter_shocks->first == shock_number)
							{
								return iter_shocks->second;
							}
						}
					}
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs table for prod code key (%s) and scenario pass number (%d) and shock number(%d) not found ", 
							product_code_key.c_str(), scen_pass_num, shock_number );
		throw new GMXBException(message);		
	}


	vector<double> get_pvs_vector(const string& product_code_key, int scen_pass_num, 
				int shock_number, const map<string, map<int,map<int, vector<double>>>>& results_map)
	{
		for(map<string, map<int,map<int, vector<double>>>>::const_iterator iter_pvs = results_map.begin(); 
			iter_pvs != results_map.end(); ++iter_pvs)
		{
			string pCK = iter_pvs->first;
			if (pCK == product_code_key)
			{
				map<int,map<int, vector<double>>> map_scen_pass_num = iter_pvs->second;
				for(map<int,map<int, vector<double>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
					iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
				{
					if (iter_scen_pass_num->first == scen_pass_num)
					{
						map<int, vector<double>> map_shocks = iter_scen_pass_num->second;
						for(map<int, vector<double>>::const_iterator iter_shocks = map_shocks.begin();
							iter_shocks != map_shocks.end(); ++iter_shocks)
						{
							if (iter_shocks->first == shock_number)
							{
								return iter_shocks->second;
							}
						}
					}
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs vector for prod code key (%s) and scenario pass number (%d) and shock number(%d) not found ", 
							product_code_key.c_str(), scen_pass_num, shock_number );
		throw new GMXBException(message);		
	}



	vector<double> get_pvs_vector_esg(const string& product_code_key,
		        int scen_pass_num,
				int bucket_id,
				int shock_number, 
				const map<string, map<int, map<int,map<int, vector<double>>>>>& results_map)
	{
		map<string, map<int, map<int,map<int, vector<double>>>>>::const_iterator itr_product = results_map.find(product_code_key);
		if(itr_product!=results_map.end())
		{
			map<int, map<int,map<int, vector<double>>>>::const_iterator itr_scen = itr_product->second.find(scen_pass_num);
			if(itr_scen != itr_product->second.end())
			{
			    map<int,map<int, vector<double>>>::const_iterator itr_bucket = itr_scen->second.find(bucket_id);
				if(itr_bucket != itr_scen->second.end())
				{
				    map<int, vector<double>>::const_iterator itr_shock = itr_bucket->second.find(shock_number);
					if(itr_shock != itr_bucket->second.end())
					{
						return itr_shock->second;
					}
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs vector for prod code key (%s) and scenario pass number (%d) and bucket id (%d)shock number(%d) not found ", 
							product_code_key.c_str(), scen_pass_num, bucket_id, shock_number );
		throw new GMXBException(message);		
	}


	vector<vector<double>> get_pvs_vector_esg(const string& product_code_key,
											  int scen_pass_num,
											  int bucket_id,
											  int shock_number, 
											  const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& results_map)
	{
		map<string, map<int, map<int,map<int, vector<vector<double>>>>>>::const_iterator itr_product = results_map.find(product_code_key);
		if(itr_product!=results_map.end())
		{
			map<int, map<int,map<int, vector<vector<double>>>>>::const_iterator itr_scen = itr_product->second.find(scen_pass_num);
			if(itr_scen != itr_product->second.end())
			{
			    map<int,map<int, vector<vector<double>>>>::const_iterator itr_bucket = itr_scen->second.find(bucket_id);
				if(itr_bucket != itr_scen->second.end())
				{
				    map<int, vector<vector<double>>>::const_iterator itr_shock = itr_bucket->second.find(shock_number);
					if(itr_shock != itr_bucket->second.end())
					{
						return itr_shock->second;
					}
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs vector for prod code key (%s) and scenario pass number (%d) and bucket id (%d) and shock number(%d) not found ", 
							product_code_key.c_str(), scen_pass_num, bucket_id ,shock_number );
		throw new GMXBException(message);		
	}


	vector<vector<double>> get_pvs_vector_esg2(const string& product_code_key,
											  int scen_pass_num,
											  int bucket_id,
											  string sheet_name, 
											  const map<string, map<int, map<int,map<string, vector<vector<double>>>>>>& results_map)
	{
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>>::const_iterator itr_product = results_map.find(product_code_key);
		if(itr_product!=results_map.end())
		{
			map<int, map<int,map<string, vector<vector<double>>>>>::const_iterator itr_scen = itr_product->second.find(scen_pass_num);
			if(itr_scen != itr_product->second.end())
			{
			    map<int,map<string, vector<vector<double>>>>::const_iterator itr_bucket = itr_scen->second.find(bucket_id);
				if(itr_bucket != itr_scen->second.end())
				{
				    map<string, vector<vector<double>>>::const_iterator itr_sheet= itr_bucket->second.find(sheet_name);
					if(itr_sheet != itr_bucket->second.end())
					{
						return itr_sheet->second;
					}
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs vector for prod code key (%s) and scenario pass number (%d) and bucket id (%d) and sheet (%s) not found ", 
							product_code_key.c_str(), scen_pass_num, bucket_id, sheet_name);
		throw new GMXBException(message);	
	}

	//results_longevity
	vector<vector<double>> get_pvs_vector_esg3(const string& product_code_key,
											  int scen_pass_num,
											  int bucket_id,
											  const map<string, map<int, map<int, vector<vector<double>>>>>& results_map)
	{
		map<string, map<int, map<int,vector<vector<double>>>>>::const_iterator itr_product = results_map.find(product_code_key);
		if(itr_product!=results_map.end())
		{
			map<int, map<int, vector<vector<double>>>>::const_iterator itr_scen = itr_product->second.find(scen_pass_num);
			if(itr_scen != itr_product->second.end())
			{
			    map<int,vector<vector<double>>>::const_iterator itr_bucket = itr_scen->second.find(bucket_id);
				if(itr_bucket != itr_scen->second.end())
				{
					return itr_bucket->second;
				}
			}
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Summary error : pvs vector for prod code key (%s) and scenario pass number (%d) and bucket id (%d) not found ", 
							product_code_key.c_str(), scen_pass_num, bucket_id);
		throw new GMXBException(message);	
	}


	void summary_checker()
	{
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int number_of_products = products.size();
		// Check if the summary contains at least one 
		// run to summarize
		if (number_of_products == 0)
		{	
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization error: No product to summarize");
			throw new GMXBException(message);		
		}

		string inforce_file = products[0].get_model_point_file();
		for(int nb_product = 1; nb_product < number_of_products; nb_product++)
		{
			string inforce_file_name = products[nb_product].get_model_point_file();
			if (inforce_file_name != inforce_file)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization error: Inforce file name inconsistency. Found %s for run %s and %s for run %s"
					, inforce_file.c_str(), products[0].get_run_name().c_str(), inforce_file_name.c_str(),  products[nb_product].get_run_name().c_str() );
				throw new GMXBException(message);			
			}
		}
		// Check the scenario number
		int number_of_scenario =  products[0].get_scenario_list().size();
		if (number_of_scenario == 0)
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization error: No scenario run");
				throw new GMXBException(message);		
		}

		for(int nb_product = 1; nb_product < number_of_products; nb_product++)
		{
			int scen_numb = products[nb_product].get_scenario_list().size();
			if (scen_numb != number_of_scenario)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization error: The number of scenario run for each product should be the same");
				throw new GMXBException(message);		
			}
		}


		//		vector<int> v_longevity;
		//if (scenario_config_0.get_load_longevity_table())
		//{
		//	v_longevity.push_back(bucket_id_0);
		//}

		//for(int iter_scen_and_bucket = 0; iter_scen_and_bucket< scenario_pass_number_0.size(); ++iter_scen_and_bucket)
		//{
		//	int scen_pn = scenario_pass_number_0.get_
		//    if()
		//}
		
		// Check the content of each scenario and bucket
		for (int nb_scen = 0; nb_scen < number_of_scenario; nb_scen++)
		{
			Scenario_configuration scenario_config_0 = products[0].get_scenario_list()[nb_scen];
			// check the scenario pass number
			int scenario_pass_number_0 = scenario_config_0.get_scenario_pass_number();
			int bucket_id_0 = scenario_config_0.get_bucket_id();

			vector<vector<int>> index_s_array_0 = scenario_config_0.get_index_shock_array();
			vector<vector<int>> fund_s_array_0 = scenario_config_0.get_fund_shock_array();

			vector<GMXB_Assumption_Table*> v_lapse_table = scenario_config_0.get_lapse_mult();
			vector<GMXB_Assumption_Table*> v_mortality_table = scenario_config_0.get_mortality_mult();
			vector<GMXB_Assumption_Table*> v_paidup_table = scenario_config_0.get_paidup_mult();
			vector<GMXB_Assumption_Table*> v_election_table = scenario_config_0.get_election_mult();

			vector<string> v_lapse_tables_names;
			vector<string> v_mortality_tables_names;
			vector<string> v_election_tables_names;
			vector<string> v_paidup_tables_names;
			bool  if_longevity_esg = false;

			

			for (int v_lapse_table_i = 0; v_lapse_table_i < v_lapse_table.size(); v_lapse_table_i++)
			{
				v_lapse_tables_names.push_back(v_lapse_table[v_lapse_table_i]->get_sheet_name());
			}

			for (int v_mortality_table_i = 0; v_mortality_table_i < v_mortality_table.size(); v_mortality_table_i++)
			{
				v_mortality_tables_names.push_back(v_mortality_table[v_mortality_table_i]->get_sheet_name());
			}

			for (int v_election_table_i = 0; v_election_table_i < v_election_table.size(); v_election_table_i++)
			{
				v_election_tables_names.push_back(v_election_table[v_election_table_i]->get_sheet_name());
			}

			for (int v_paidup_table_i = 0; v_paidup_table_i < v_paidup_table.size(); v_paidup_table_i++)
			{
				v_paidup_tables_names.push_back(v_paidup_table[v_paidup_table_i]->get_sheet_name());
			}

			if (scenario_config_0.get_load_longevity_table())
			{
				if_longevity_esg = true;
			}




			int index_s_array_0_size = index_s_array_0.size();
			int fund_s_array_0_size = fund_s_array_0.size();


			for(int nb_product = 1; nb_product < number_of_products; nb_product++)
			{
				vector<Scenario_configuration> list_of_scen = products[nb_product].get_scenario_list();
				int nb_scenarios = list_of_scen.size();
				Scenario_configuration scenario;
				bool scen_pass_num_found = false;
				
				for(int scen = 0; scen < nb_scenarios; scen++)
				{
					scenario = list_of_scen[scen];
					// Try to find the scenario pass number
					if (scenario.get_scenario_pass_number() == scenario_pass_number_0 && scenario.get_bucket_id() == bucket_id_0)
					{
						scen_pass_num_found = true;
						break;
					}
				}
				
				// Throw an exception in case the scenario pass number is not found
				if (!scen_pass_num_found)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization error: Inconsistency. The scenario pass num %d is not found for product %s ", 
						scenario_pass_number_0, products[nb_product].get_run_name().c_str());
					throw new GMXBException(message);				
				}

				// The scenario pass num is found. We check the shock arrays
				// Check shock array size
				vector<vector<int>> index_s_array = scenario.get_index_shock_array();
				int index_s_array_size = index_s_array.size();
				if (index_s_array_0_size != index_s_array_size)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization error: Index shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
						products[0].get_run_name().c_str(), products[nb_product].get_run_name().c_str(), scenario_pass_number_0);
					throw new GMXBException(message);
				}

				vector<vector<int>> fund_s_array = scenario.get_fund_shock_array();
				int fund_s_array_size = fund_s_array.size();
				if (fund_s_array_0_size != fund_s_array_size)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization error: Fund shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
						products[0].get_run_name().c_str(), products[nb_product].get_run_name().c_str(), scenario_pass_number_0);
					throw new GMXBException(message);
				}

				// Check the content of the index shock array
				for(int index_s_size = 0; index_s_size < index_s_array_0_size; index_s_size++)
				{
					vector<int> index_shocks_0 = index_s_array_0[index_s_size];
					vector<int> index_shocks = index_s_array[index_s_size];
					for(unsigned int shock = 0; shock < index_shocks_0.size(); shock++)
					{
						if (index_shocks_0[shock]!=index_shocks[shock])
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization error: Index shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
								products[0].get_run_name().c_str(), 
								products[nb_product].get_run_name().c_str(), 
								scenario_pass_number_0
									);
							throw new GMXBException(message);
						}
					}
				}
				// Check the content of the fund shock array
				for(int fund_s_size = 0; fund_s_size < fund_s_array_0_size; fund_s_size++)
				{
					vector<int> fund_shocks_0 = fund_s_array_0[fund_s_size];
					vector<int> fund_shocks = fund_s_array[fund_s_size];
					for(unsigned int shock = 0; shock < fund_shocks_0.size(); shock++)
					{
						if (fund_shocks_0[shock]!=fund_shocks[shock])
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization error: Fund shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
								products[0].get_run_name().c_str(), 
								products[nb_product].get_run_name().c_str(), 
								scenario_pass_number_0
									);
							throw new GMXBException(message);
						}
					}
				}

			}
			// Fill the map of shocks			
			if(if_multi_esg == false)
			{
				this->index_map_shocks.insert (pair<int,vector<vector<int>>>(scenario_pass_number_0,index_s_array_0));
				this->fund_map_shocks.insert (pair<int,vector<vector<int>>>(scenario_pass_number_0,fund_s_array_0));
			}
			else
			{
				this->index_map_shocks_esg[scenario_pass_number_0][bucket_id_0] = index_s_array_0;
				this->fund_map_shocks_esg[scenario_pass_number_0][bucket_id_0]  = fund_s_array_0;

				lapse_map_shocks_esg[scenario_pass_number_0][bucket_id_0]     = v_lapse_tables_names;
				mortality_map_shocks_esg[scenario_pass_number_0][bucket_id_0] = v_mortality_tables_names;
				paidup_map_shocks_esg[scenario_pass_number_0][bucket_id_0]    = v_election_tables_names;
				election_map_shocks_esg [scenario_pass_number_0][bucket_id_0] = v_paidup_tables_names;
				longevity_map_esg[scenario_pass_number_0][bucket_id_0]        = if_longevity_esg;
			}
		}
	}

	// Get the pvs aggregated 
	// per intermediate result file
	void get_pvs_aggregated(const string& prefix_file_name, vector<double>& pvs_aggregated, 
							int scenario_pass_number, int shock_number, 
							//Product_configuration& product, 
							double& calib_a, double& calib_sigma)
	{
		char file_name[MAX_LENGTH_FILE_NAME];
		this->nb_of_polices=0;
		vector<vector<double>> pvs;
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
				// read the intermediate file
			sprintf(file_name, "%s%d.csv", prefix_file_name.c_str(),worker_id);
				// Open the worker file for the given scenario and shock
				ifstream file_op(file_name, ios::in);
				if (file_op)
				{
					int nb_rows = 0;
					while(!file_op.eof())
					{
						string row;
						int tot_num_iterations = 0;
						getline(file_op,row);
						if (row.length() > 0)
						{
							nb_rows++;
							if (nb_rows > 1)
							{
								char message[MAX_LENGTH_MESSAGE_EXCEPTION];
								sprintf_s(message, "Summarization: The result file %s should contain only one row (total summarization)",
												file_name);
								throw new GMXBException(message);
							}
							vector<double> pvs_per_worker;
							char *retPtr(new char[row.length() + 1]);
							copy(row.begin(), row.end(), retPtr);
							retPtr[row.length()] = '\0';
							// The first column in the 
							// file is the record number of the model point
							char* tokenizer = strtok(retPtr,",");
							tot_num_iterations = atoi(tokenizer);
							
							//-- To be test if all are the same;
							// Added by yuan !!
							calib_a = atof(tokenizer = strtok (NULL, ","));
							calib_sigma = atof(tokenizer = strtok (NULL, ","));

							while ((tokenizer = strtok (NULL, ",")) != NULL) 
							{	 
								pvs_per_worker.push_back(atof(tokenizer)/tot_num_iterations);
							} 

							//recuperer le nombre de polices depuis le fichier intermediaire
							nb_of_polices += pvs_per_worker.back() * tot_num_iterations;
					
							
							pvs.push_back(pvs_per_worker);
							delete[] retPtr;
						}
					}
				}
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization: The file %s is not found", file_name);
					throw new GMXBException(message);
				}
		}
		int pvs_size = pvs.size();
		if (pvs_size > 0)
		{
			int nb_columns = pvs[0].size();
			for (int j = 0; j < nb_columns ; j++)
			{
				double val = 0;
				for (int i = 0; i < pvs_size; i++)
				{
					val += pvs[i][j];
				}
				pvs_aggregated.push_back(val);
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization total: The pv size should not be 0");
			throw new GMXBException(message);
		}
	}

	// Get the pvs by policy 
	// per intermediate result file
	void get_pvs_by_policy(const string& prefix_file_name, vector<vector<double>>& pv_results, int scenario_number, int shock_number,
						   double& calib_a, double& calib_sigma)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
			// read the intermediate file
			sprintf(file_name, "%s%d.csv", prefix_file_name.c_str(), worker_id);

			// Open the worker file for the given scenario and shock
			ifstream file_op(file_name, ios::in);
			if (file_op)
			{
				while(!file_op.eof())
				{
					string row;
					int tot_num_iterations = 0;
					getline(file_op,row);
					if (row.length() > 0)
					{
						vector<double> pvs;
						char *retPtr(new char[row.length() + 1]);
						copy(row.begin(), row.end(), retPtr);
						retPtr[row.length()] = '\0';
						char* tokenizer = strtok(retPtr,",");
						// The first column in the 
						// file is the record number of the model point
						int record_number = atoi(tokenizer);
						tot_num_iterations = atoi(strtok (NULL, ","));
						calib_a = atof(strtok (NULL, ","));
						calib_sigma = atof(strtok (NULL, ","));

						while ((tokenizer = strtok (NULL, ",")) != NULL) 
						{	 
							pvs.push_back(atof(tokenizer)/tot_num_iterations);
						} 
						pv_results.push_back(pvs);
						delete[] retPtr;
					}
				}
			}
			else
			{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization: The file %s is not found", file_name);
					throw new GMXBException(message);
			}
		}
	}


	void get_pvs_by_policy_esg(const string& prefix_file_name, vector<vector<double>>& pv_results, double& calib_a, double& calib_sigma)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
			// read the intermediate file
			sprintf(file_name, "%s%d.csv", prefix_file_name.c_str(), worker_id);

			// Open the worker file for the given scenario and shock
			ifstream file_op(file_name, ios::in);
			if (file_op)
			{
				while(!file_op.eof())
				{
					string row;
					int tot_num_iterations = 0;
					getline(file_op,row);
					if (row.length() > 0)
					{
						vector<double> pvs;
						char *retPtr(new char[row.length() + 1]);
						copy(row.begin(), row.end(), retPtr);
						retPtr[row.length()] = '\0';
						char* tokenizer = strtok(retPtr,",");
						// The first column in the 
						// file is the record number of the model point
						int record_number = atoi(tokenizer);
						tot_num_iterations = atoi(strtok (NULL, ","));
						calib_a = atof(strtok (NULL, ","));
						calib_sigma = atof(strtok (NULL, ","));

						while ((tokenizer = strtok (NULL, ",")) != NULL) 
						{	 
							pvs.push_back(atof(tokenizer)/tot_num_iterations);
						} 
						pv_results.push_back(pvs);
						delete[] retPtr;
					}
				}
			}
			else
			{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization: The file %s is not found", file_name);
					throw new GMXBException(message);
			}
		}
	}

	void get_pvs_by_scen(const string& prefix_file_name, vector<vector<double>>& pv_results, int scenario_number, int shock_number,
						   double& calib_a, double& calib_sigma)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
			// read the intermediate file
			sprintf(file_name, "%s%d.csv", prefix_file_name.c_str(), worker_id);

			// Open the worker file for the given scenario and shock
			ifstream file_op(file_name, ios::in);
			if (file_op)
			{
				int compt_row = -1;
				while(!file_op.eof())
				{
					/*
					//cout << row.length()<< endl;
						vector<double> pvs;
						char *retPtr(new char[row.length() + 1]);
						copy(row.begin(), row.end(), retPtr);
						retPtr[row.length()] = '\0';
						char* tokenizer = strtok(retPtr,",");
						// The first column in the 
						// file is the record number of the model point
						int record_number = atoi(tokenizer);
						tot_num_iterations = atoi(strtok (NULL, ","));
						calib_a = atof(strtok (NULL, ","));
						calib_sigma = atof(strtok (NULL, ","));

						while ((tokenizer = strtok (NULL, ",")) != NULL) 
						{	 
							pvs.push_back(atof(tokenizer)/tot_num_iterations);
						} 
					*/
					++compt_row;  
					string row;
					int tot_num_iterations = 0;
					getline(file_op,row);
					if (row.length() > 0)
					{
						vector<double> pvs;
						char *retPtr(new char[row.length() + 1]);
						copy(row.begin(), row.end(), retPtr);
						
						retPtr[row.length()] = '\0';
						char* tokenizer = strtok(retPtr,",");
						// The first column in the 
						// file is the record number of the model point
						int record_number = atoi(tokenizer);
						//tot_num_iterations = atoi(strtok (NULL, ","));
						calib_a = atof(strtok (NULL, ","));
						calib_sigma = atof(strtok (NULL, ","));

						while ((tokenizer = strtok (NULL, ",")) != NULL) 
						{	 
							pvs.push_back(atof(tokenizer));
						} 

						if(worker_id == 1)
						{
							pv_results.push_back(pvs);
						}
						else
						{
							//for(unsigned jj=0; jj<pvs.size(); ++jj)
							//	cout << pv_results[compt_row][jj] << "  ";
							//cout << endl;
							//getchar();
							// add each collomn (from 4rd to last collomn)
							for(unsigned int jj = 0; jj<pvs.size(); ++jj)
								pv_results[compt_row][jj] += pvs[jj]; 
						}

						delete[] retPtr;
					}
				}
			}
			else
			{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization: The file %s is not found", file_name);
					throw new GMXBException(message);
			}
		}
	}
	void get_pvs_by_scen_esg(const string& prefix_file_name, vector<vector<double>>& pv_results,  double& calib_a, double& calib_sigma)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
			// read the intermediate file
			sprintf(file_name, "%s%d.csv", prefix_file_name.c_str(), worker_id);

			// Open the worker file for the given scenario and shock
			ifstream file_op(file_name, ios::in);
			if (file_op)
			{
				int compt_row = -1;
				while(!file_op.eof())
				{
					++compt_row;  
					string row;
					int tot_num_iterations = 0;
					getline(file_op,row);
					if (row.length() > 0)
					{
						vector<double> pvs;
						char *retPtr(new char[row.length() + 1]);
						copy(row.begin(), row.end(), retPtr);
						
						retPtr[row.length()] = '\0';
						char* tokenizer = strtok(retPtr,",");
						// The first column in the 
						// file is the record number of the model point
						int record_number = atoi(tokenizer);
						//tot_num_iterations = atoi(strtok (NULL, ","));
						calib_a = atof(strtok (NULL, ","));
						calib_sigma = atof(strtok (NULL, ","));

						while ((tokenizer = strtok (NULL, ",")) != NULL) 
						{	 
							pvs.push_back(atof(tokenizer));
						} 

						if(worker_id == 1)
						{
							pv_results.push_back(pvs);
						}
						else
						{
							for(unsigned int jj = 0; jj<pvs.size(); ++jj)
								pv_results[compt_row][jj] += pvs[jj]; 
						}

						delete[] retPtr;
					}
				}
			}
			else
			{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization: The file %s is not found", file_name);
					throw new GMXBException(message);
			}
		}
	}
public :
	Summary_File_Writer(int nb_workers,Summary_configuration *summary,
						const map<string,vector<string>>& map_scenario_names, 
						const map<string,vector<string>>& map_index_shock_names,
						Dispatch_Manager* dispatch_manager_ip) : dispatch_manager(dispatch_manager_ip)
	{	
		if(this->dispatch_manager == NULL)
			if_multi_esg = false;
		else
			if_multi_esg = true;

		this->number_of_workers = nb_workers;
		this->summary_configuration = summary;
		//! Y
		this->summary_checker();

		//this->a = calib_parameter_a;
		//this->sigma = calib_parameter_sigma;
		this->list_scenario_names = map_scenario_names;
		this->list_index_shock_names = map_index_shock_names;
	}

	Summary_File_Writer(int nb_workers,
	Summary_configuration *summary,
	string pCK,
	const map<string,vector<string>>& map_scenario_names, 
	const map<string,vector<string>>& map_index_shock_names,
	Dispatch_Manager* dispatch_manager_ip)
	:
    dispatch_manager(dispatch_manager_ip)
	{	
		if(this->dispatch_manager == NULL) if_multi_esg = false;
		else if_multi_esg = true;
		this->number_of_workers = nb_workers;
		this->summary_configuration = summary;
		this->summary_checker();
		this->prod_code_key=pCK;
		//this->a = calib_parameter_a;
		//this->sigma = calib_parameter_sigma;
		this->list_scenario_names = map_scenario_names;
		this->list_index_shock_names = map_index_shock_names;
	}
	~Summary_File_Writer()
	{
		for(unsigned int i = 0; i < this->v_mortality.size(); i++)
			delete this->v_mortality[i];
	}

	int get_shock_number(const vector<vector<int>>& shock_array, int current_shock_number, int maximum_shocks, int maximum_number_of_instrument)
	{
		if (shock_array.size())
		{
			if(shock_array[current_shock_number][0]==0)
				return 0;
			else if(shock_array[current_shock_number][0]==maximum_number_of_instrument+1)
			{
				if(shock_array[current_shock_number][1]>0)
				{
					return (maximum_shocks * maximum_number_of_instrument + shock_array[current_shock_number][1]);
				}
				else
				{
					return (maximum_shocks * (maximum_number_of_instrument+1) + 1 + shock_array[current_shock_number][1]);
				}
			 }
			else if(shock_array[current_shock_number][0]>0 && shock_array[current_shock_number][0]<=maximum_number_of_instrument)
			{
				if(shock_array[current_shock_number][shock_array[current_shock_number][0]]>0)
				{
					return (maximum_shocks * (shock_array[current_shock_number][0] - 1) + shock_array[current_shock_number][shock_array[current_shock_number][0]]);
				
				}
				else
				{
					return (maximum_shocks * shock_array[current_shock_number][0] + 1 + shock_array[current_shock_number][shock_array[current_shock_number][0]]);
				}
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Bad configuration of the shock array");
				throw new GMXBException(message);		
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The shock array is empty");
			throw new GMXBException(message);		
		}
	}

	virtual double calculate_ehc(Model_Point* mp, bool recalculate_ehc, bool is_new_ehc, double pv_charges_1, 
								double pv_charges_0, double pv_claims_1, double pv_claims_0, double pv_dep_fee_0,
								double pv_dep_fee_1, double rrc_1, double rrc_0, double ehc_1, double ehc_0)
	{
		double ehc = 0.;
		if (recalculate_ehc)
		{
			if (!is_new_ehc)
			{
				// Read directly the ehc from the inforce file
				ehc = mp->gmxb_ehc_charge;	
			}
			else
			{	/*recalc file*/	
				if (pv_charges_1 != 0)
					ehc =  (rrc_1/pv_charges_1) * 
					(pv_claims_1 - pv_dep_fee_1 - (pv_claims_0 - pv_dep_fee_0) + ((ehc_0/rrc_0) * pv_charges_0));
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Charges equal 0 for policy number %s ", mp->policy_id.c_str());
					throw new GMXBException(message);			
				}
			}
		}
		else
		{	/*NB file*/
			if (pv_charges_1 != 0)
					ehc = ((pv_claims_1 - pv_dep_fee_1) /pv_charges_1) * mp->gmxb_g_c;
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Charges equal 0 for policy number %s ", mp->policy_id.c_str());
					throw new GMXBException(message);		
				}
		
		}
			
		return ehc;
	}


	virtual void fill_ehc_map(const map<string, vector<Model_Point*>>& mps,
									 map<string, vector<double>>& map_ehc_values,
									const map<string, map<int,map<int, vector<vector<double>>>>>& index_results_map,
									const map<string, map<int,map<int, vector<vector<double>>>>>& fund_results_map,
									map<string, vector<int>>& indexes_policies_NB,
									map<string, vector<int>>& indexes_policies_recalc_old,
									map<string, vector<int>>& indexes_policies_recalc_new,
									map<string, vector<double>>& map_ehc_values_NB,
									map<string, vector<double>>& map_ehc_values_recalc_old,
									map<string, vector<double>>& map_ehc_values_recalc_new
									)
	{
		this->nb_of_policies_NB = 0;
		this->nb_of_policies_recalc_old = 0;
		this->nb_of_policies_recalc_new = 0;
		
		// Loop on the model points vector
		for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
				iter_mp != mps.end(); ++iter_mp)
		{
			string product_code_key = iter_mp->first;
			// Get the pvs from the index_results_map
			
			 vector<vector<double>>  pvs_ehc_scenarios;

			 for (map<string, map<int,map<int, vector<vector<double>>>>>::const_iterator it_index_res = index_results_map.begin();
				 it_index_res != index_results_map.end(); ++it_index_res)
			 {
				 string pCK = it_index_res->first;

				 if (product_code_key == pCK)
				 {
					 for (map<int,map<int, vector<vector<double>>>>::const_iterator it_index_res_scen_1 = it_index_res->second.begin();
						 it_index_res_scen_1 != it_index_res->second.end(); ++it_index_res_scen_1)
					 {
						 if (it_index_res_scen_1->first == summary_configuration->get_ehc_scen_pass_num())
						 {
							 
							 for (map<int, vector<vector<double>>>::const_iterator it_index_res_scen_1_shock_1 = it_index_res_scen_1->second.begin();
								 it_index_res_scen_1_shock_1 != it_index_res_scen_1->second.end(); ++it_index_res_scen_1_shock_1)
							 {

								 if (it_index_res_scen_1_shock_1->first == 0)
								 {
									 pvs_ehc_scenarios = it_index_res_scen_1_shock_1->second;
								 }
							 }
							 break;
						 }
					 }

					 break;
				 }
			 
			 }


			 if (pvs_ehc_scenarios.size() == 0)
			 {
 					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, " No pvs found for the specified ehc scenario");
					throw new GMXBException(message);								 
			 }

			vector<int> v_NB;
			vector<int> v_recalc_old;
			vector<int> v_recalc_new;


			vector<double> ehc_NB;
			vector<double> ehc_recalc_old;
			vector<double> ehc_recalc_new;

			vector<double> ehc_v;
			vector<Model_Point *> v_mps = iter_mp->second;
			for (unsigned int i = 0 ; i < v_mps.size(); i++)
			{
				// If the policy id contains _0 : ehc provided in inforce file
				//							 _1 : ehc recalculation
				Model_Point *mp = v_mps[i];
				double ehc = 0.;
	
				double pv_charges_0 = 0.; 
				double pv_claims_1 = 0.; 
				double pv_charges_1 = 0.; 
				double pv_claims_0 = 0.;
				double pv_dep_fee_1 = 0.; 
				double pv_dep_fee_0 = 0.;


				if (mp->group.substr(mp->group.length() - 2, mp->group.length() - 1) == "_1")
				{
					bool recalculate_ehc = false;
					// Check if a corresponding _0 policy 
					// exist in the vector of model points
					string group_id_0 = mp->group.substr(0, mp->group.length() - 2) + "_0";
					int id_group_id_0 = -1;
					double rrc_0 = -1;
					double ehc_0 = -1;
					for (unsigned int j = 0; j < v_mps.size(); j++)
					{
						Model_Point *model_point = v_mps[j];
						if (model_point->group == group_id_0)
						{
							recalculate_ehc = true;
							id_group_id_0 = j;
							rrc_0 = model_point->gmxb_g_c;
							ehc_0 = model_point->gmxb_ehc_charge;
							break;
						}
					}
					

					pv_charges_1 = pvs_ehc_scenarios[i][PV_GMXB_REAL_CHG];
					pv_claims_1 = pvs_ehc_scenarios[i][PV_GMAB]+ pvs_ehc_scenarios[i][PV_GMDB]
										+ pvs_ehc_scenarios[i][PV_GMIB] + pvs_ehc_scenarios[i][PV_GMWB] 
										+ pvs_ehc_scenarios[i][PV_GMSB];
					pv_dep_fee_1 = pvs_ehc_scenarios[i][PV_DEP_FEE];
					
					if (recalculate_ehc)
					{
						// Get the pvs for the 
						// _0 model point and the _1 model points

						pv_charges_0 = pvs_ehc_scenarios[id_group_id_0][PV_GMXB_REAL_CHG];
						pv_claims_0 = pvs_ehc_scenarios[id_group_id_0][PV_GMAB]+ pvs_ehc_scenarios[id_group_id_0][PV_GMDB]
										+ pvs_ehc_scenarios[id_group_id_0][PV_GMIB] + pvs_ehc_scenarios[id_group_id_0][PV_GMWB] 
										+ pvs_ehc_scenarios[id_group_id_0][PV_GMSB];
						pv_dep_fee_0 = pvs_ehc_scenarios[id_group_id_0][PV_DEP_FEE];

						v_recalc_new.push_back(i);

						/*
						Model_Point* mp, bool recalculate_ehc, bool is_new_ehc, double pv_charges_1, 
								double pv_charges_0, double pv_claims_1, double pv_claims_0, double rrc_1, double rrc_0, 
								double ehc_1, double ehc_0
						*/
						double rrc_1 = mp->gmxb_g_c;
						double ehc_1 = mp->gmxb_ehc_charge;
						ehc = this->calculate_ehc(mp, recalculate_ehc, true, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0, 
												  pv_dep_fee_1, pv_dep_fee_0, rrc_1, rrc_0, ehc_1, ehc_0);
						ehc_recalc_new.push_back(ehc);					
					}
					else
					{
						v_NB.push_back(i);
						ehc = this->calculate_ehc(mp, recalculate_ehc, false, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0,
												  pv_dep_fee_1, pv_dep_fee_0, 0, 0, 0, 0);
						ehc_NB.push_back(ehc);
					}
				}
				else if (mp->group.substr(mp->group.length() - 2, mp->group.length() - 1) == "_0")
				{
					v_recalc_old.push_back(i);
					ehc = this->calculate_ehc(mp, true, false, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0,
											pv_dep_fee_1, pv_dep_fee_0, 0, 0, 0, 0);
					ehc_recalc_old.push_back(ehc);
				}
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "_0 or _1 is necessary for ehc recalculation");
					throw new GMXBException(message);					
				}

				ehc_v.push_back(ehc);
			}
			
			map_ehc_values[product_code_key] = ehc_v;

			indexes_policies_NB[product_code_key] = v_NB;

			indexes_policies_recalc_old[product_code_key] = v_recalc_old;

			indexes_policies_recalc_new[product_code_key] = v_recalc_new;

			this->nb_of_policies_NB += v_NB.size();
			this->nb_of_policies_recalc_old += v_recalc_old.size();
			this->nb_of_policies_recalc_new +=  v_recalc_new.size();

			map_ehc_values_NB[product_code_key] = ehc_NB;
			map_ehc_values_recalc_old[product_code_key] =  ehc_recalc_old;
			map_ehc_values_recalc_new[product_code_key] =  ehc_recalc_new;

		}

	}


	virtual void fill_ehc_map_esg(const map<string, vector<Model_Point*>>& mps,
									 map<string, vector<double>>& map_ehc_values,
									const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& index_results_map,
									const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& fund_results_map,
									map<string, vector<int>>& indexes_policies_NB,
									map<string, vector<int>>& indexes_policies_recalc_old,
									map<string, vector<int>>& indexes_policies_recalc_new,
									map<string, vector<double>>& map_ehc_values_NB,
									map<string, vector<double>>& map_ehc_values_recalc_old,
									map<string, vector<double>>& map_ehc_values_recalc_new
									)
	{
		
		this->nb_of_policies_NB = 0;
		this->nb_of_policies_recalc_old = 0;
		this->nb_of_policies_recalc_new = 0;
		
		// Loop on the model points vector
		for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
				iter_mp != mps.end(); ++iter_mp)
		{
			string product_code_key = iter_mp->first;
			// Get the pvs from the index_results_map
			
			 vector<vector<double>>  pvs_ehc_scenarios;

			 for (map<string, map<int, map<int,map<int, vector<vector<double>>>>>>::const_iterator it_index_res = index_results_map.begin();
				 it_index_res != index_results_map.end(); ++it_index_res)
			 {
				 string pCK = it_index_res->first;

				 if (product_code_key == pCK)
				 {
					 for (map<int, map<int,map<int, vector<vector<double>>>>>::const_iterator it_index_res_scen_1 = it_index_res->second.begin();
						 it_index_res_scen_1 != it_index_res->second.end(); ++it_index_res_scen_1)
					 {
                         if (it_index_res_scen_1->first == summary_configuration->get_ehc_scen_pass_num())
						 {
							 for (map<int,map<int, vector<vector<double>>>>::const_iterator it_index_res_bucket_1 = it_index_res_scen_1->second.begin();
								 it_index_res_bucket_1 != it_index_res_scen_1->second.end(); ++it_index_res_bucket_1)
							 {
								 if (it_index_res_bucket_1->first == 1)
								 { 
									 for (map<int, vector<vector<double>>>::const_iterator it_index_res_scen_1_shock_1 = it_index_res_bucket_1->second.begin();
										 it_index_res_scen_1_shock_1 != it_index_res_bucket_1->second.end(); ++it_index_res_scen_1_shock_1)
									 {

										 if (it_index_res_scen_1_shock_1->first == 0)
										 {
											 pvs_ehc_scenarios = it_index_res_scen_1_shock_1->second;
											 break;
										 }
									 }
									 break;
								 }
							 }
							 break;
						 }
					 }
					 break;
				 }
			 }


			 if (pvs_ehc_scenarios.size() == 0)
			 {
 					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, " No pvs found for the specified ehc scenario");
					throw new GMXBException(message);								 
			 }

			vector<int> v_NB;
			vector<int> v_recalc_old;
			vector<int> v_recalc_new;


			vector<double> ehc_NB;
			vector<double> ehc_recalc_old;
			vector<double> ehc_recalc_new;

			vector<double> ehc_v;
			vector<Model_Point *> v_mps = iter_mp->second;
			for (unsigned int i = 0 ; i < v_mps.size(); i++)
			{
				// If the policy id contains _0 : ehc provided in inforce file
				//							 _1 : ehc recalculation
				Model_Point *mp = v_mps[i];
				double ehc = 0.;
	
				double pv_charges_0 = 0.; 
				double pv_claims_1 = 0.; 
				double pv_charges_1 = 0.; 
				double pv_claims_0 = 0.;
				double pv_dep_fee_1 = 0.; 
				double pv_dep_fee_0 = 0.;

				if (mp->group.substr(mp->group.length() - 2, mp->group.length() - 1) == "_1")
				{
					bool recalculate_ehc = false;
					// Check if a corresponding _0 policy 
					// exist in the vector of model points
					string group_id_0 = mp->group.substr(0, mp->group.length() - 2) + "_0";
					int id_group_id_0 = -1;
					double rrc_0 = -1;
					double ehc_0 = -1;
					for (unsigned int j = 0; j < v_mps.size(); j++)
					{
						Model_Point *model_point = v_mps[j];
						if (model_point->group == group_id_0)
						{
							recalculate_ehc = true;
							id_group_id_0 = j;
							rrc_0 = model_point->gmxb_g_c;
							ehc_0 = model_point->gmxb_ehc_charge;
							break;
						}
					}
					

					pv_charges_1 = pvs_ehc_scenarios[i][PV_GMXB_REAL_CHG];
					pv_claims_1 = pvs_ehc_scenarios[i][PV_GMAB]+ pvs_ehc_scenarios[i][PV_GMDB]
										+ pvs_ehc_scenarios[i][PV_GMIB] + pvs_ehc_scenarios[i][PV_GMWB] 
										+ pvs_ehc_scenarios[i][PV_GMSB];
					pv_dep_fee_1 = pvs_ehc_scenarios[i][PV_DEP_FEE];
					
					if (recalculate_ehc)
					{
						// Get the pvs for the 
						// _0 model point and the _1 model points

						pv_charges_0 = pvs_ehc_scenarios[id_group_id_0][PV_GMXB_REAL_CHG];
						pv_claims_0 = pvs_ehc_scenarios[id_group_id_0][PV_GMAB]+ pvs_ehc_scenarios[id_group_id_0][PV_GMDB]
										+ pvs_ehc_scenarios[id_group_id_0][PV_GMIB] + pvs_ehc_scenarios[id_group_id_0][PV_GMWB] 
										+ pvs_ehc_scenarios[id_group_id_0][PV_GMSB];
						pv_dep_fee_0 = pvs_ehc_scenarios[id_group_id_0][PV_DEP_FEE];

						v_recalc_new.push_back(i);

						/*
						Model_Point* mp, bool recalculate_ehc, bool is_new_ehc, double pv_charges_1, 
								double pv_charges_0, double pv_claims_1, double pv_claims_0, double rrc_1, double rrc_0, 
								double ehc_1, double ehc_0
						*/
						double rrc_1 = mp->gmxb_g_c;
						double ehc_1 = mp->gmxb_ehc_charge;
						ehc = this->calculate_ehc(mp, recalculate_ehc, true, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0, 
												  pv_dep_fee_1, pv_dep_fee_0, rrc_1, rrc_0, ehc_1, ehc_0);
						ehc_recalc_new.push_back(ehc);					
					}
					else
					{
						v_NB.push_back(i);
						ehc = this->calculate_ehc(mp, recalculate_ehc, false, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0,
												  pv_dep_fee_1, pv_dep_fee_0, 0, 0, 0, 0);
						ehc_NB.push_back(ehc);
					}
				}
				else if (mp->group.substr(mp->group.length() - 2, mp->group.length() - 1) == "_0")
				{
					v_recalc_old.push_back(i);
					ehc = this->calculate_ehc(mp, true, false, pv_charges_1, pv_charges_0, pv_claims_1, pv_claims_0,
											  pv_dep_fee_1, pv_dep_fee_0, 0, 0, 0, 0);
					ehc_recalc_old.push_back(ehc);
				}
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "_0 or _1 is necessary for ehc recalculation");
					throw new GMXBException(message);					
				}

				ehc_v.push_back(ehc);
			}
			
			map_ehc_values[product_code_key] = ehc_v;

			indexes_policies_NB[product_code_key] = v_NB;

			indexes_policies_recalc_old[product_code_key] = v_recalc_old;

			indexes_policies_recalc_new[product_code_key] = v_recalc_new;

			this->nb_of_policies_NB += v_NB.size();
			this->nb_of_policies_recalc_old += v_recalc_old.size();
			this->nb_of_policies_recalc_new +=  v_recalc_new.size();

			map_ehc_values_NB[product_code_key] = ehc_NB;
			map_ehc_values_recalc_old[product_code_key] =  ehc_recalc_old;
			map_ehc_values_recalc_new[product_code_key] =  ehc_recalc_new;

		}

	}
	virtual void write_sensi_file(const string& sensi_file_name, const map<string, vector<Model_Point*>>& mps,
						const map<string, map<int,map<int, vector<vector<double>>>>>& index_results_map,
						 map<string, vector<double>>& map_ehc_values, const map<int,vector<double>>& map_calib_parameters,
						 double nb_of_policies)
	{

		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s", sensi_file_name.c_str());

		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;

		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int size_of_index_shock_array = index_shock_array.size();
			for(int s = 0; s < size_of_index_shock_array; s++ )
			{

				int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
				
				double pv_realch = 0.;
				double pv_ehcch = 0.;
				double pv_dep_fee = 0.;
				double pv_sumpremium=0.;
				double pv_paidpremium=0.;
				double pv_claims_ab=0.;
				double pv_claims_ib=0.;
				double pv_claims_db=0.;
				double pv_claims_wb=0.;
				double pv_claims_sb=0.;
				
				// Loop on the products
				for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
				{
					string pCK = iter_mp->first;
					vector<Model_Point*> model_points = iter_mp->second;
					vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, index_shock_number, index_results_map);
					vector<double> ehc_values = map_ehc_values[pCK];
					int number_of_mps = model_points.size();
					for(int i = 0; i < number_of_mps; i++)
					{
						Model_Point *model_p = model_points[i];
						double ehc_value = ehc_values[i];
						vector<double> pv = pvs[i];
						pv_realch += pv[PV_GMXB_REAL_CHG];
						pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
						pv_dep_fee += pv[PV_DEP_FEE];
						pv_sumpremium=0.;
						pv_paidpremium=0.;					
						pv_claims_ab+=pv[PV_GMAB];
						pv_claims_ib+=pv[PV_GMIB];
						pv_claims_db+=pv[PV_GMDB];
						pv_claims_wb+=pv[PV_GMWB];
						pv_claims_sb+=pv[PV_GMSB];
					}
				}

				ostringstream oss_scen_pass_n;
				ostringstream oss_shock_n;

				string scenario_full_name;
				string scenario_name = "";
				string index_shock_name = "";

				oss_scen_pass_n << scen_pass_n;
				oss_shock_n << index_shock_number;
				scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
				
				if (index_shock_number > 0)
				{
					index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
					scenario_full_name = scenario_name + " " + index_shock_name;
				}
				else
					scenario_full_name = scenario_name;

				double a = 0.;
				double sigma = 0.;
				for(map<int, vector<double>>::const_iterator iter_calib = map_calib_parameters.begin();
					iter_calib != map_calib_parameters.end(); ++iter_calib)
				{
					if (iter_calib->first == scen_pass_n)
					{
						vector<double> v_calib = iter_calib->second;
						a = v_calib[0];
						sigma = v_calib[1];
					}
				}				
				// write the output line
				sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
							<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
							<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
							<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << nb_of_policies  << "," << DLL_VERSION << endl;
			}
		}
	}


	virtual void write_sensi_file_esg(const string& sensi_file_name, const map<string, vector<Model_Point*>>& mps,
						const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& index_results_map,
						 map<string, vector<double>>& map_ehc_values, const map<int,map<int,vector<double>>>& map_calib_parameters,
						 double nb_of_policies)
	{

		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s", sensi_file_name.c_str());

		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;

		for (map<int, map<int, vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->index_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->index_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;

			for (map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<vector<int>> index_shock_array = iter_bucket->second;
				int size_of_index_shock_array = index_shock_array.size();
				for(int s = 0; s < size_of_index_shock_array; s++ )
				{
					int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		
					double pv_realch = 0.;
					double pv_ehcch = 0.;
					double pv_dep_fee = 0.;
					double pv_sumpremium=0.;
					double pv_paidpremium=0.;
					double pv_claims_ab=0.;
					double pv_claims_ib=0.;
					double pv_claims_db=0.;
					double pv_claims_wb=0.;
					double pv_claims_sb=0.;

					// Loop on the products
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id, index_shock_number, index_results_map);
						vector<double> ehc_values = map_ehc_values[pCK];
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps; i++)
						{
							Model_Point *model_p = model_points[i];
							double ehc_value = ehc_values[i];
							vector<double> pv = pvs[i];

							pv_realch += pv[PV_GMXB_REAL_CHG];
							pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
							pv_dep_fee += pv[PV_DEP_FEE];
							pv_sumpremium=0.;
							pv_paidpremium=0.;					
							pv_claims_ab+=pv[PV_GMAB];
							pv_claims_ib+=pv[PV_GMIB];
							pv_claims_db+=pv[PV_GMDB];
							pv_claims_wb+=pv[PV_GMWB];
							pv_claims_sb+=pv[PV_GMSB];
						}
					}

					ostringstream oss_scen_pass_n;
					ostringstream oss_shock_n;

					string scenario_full_name;
					string scenario_name = "";
					string index_shock_name = "";

					oss_scen_pass_n << scen_pass_n;
					oss_shock_n << index_shock_number;
					scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
					
					string bucket_str = Converter::convert_to_str(bucket_id);

					if (index_shock_number > 0)
					{
						index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
						scenario_full_name = scenario_name + " " + bucket_str + " " + index_shock_name;
					}	
					else
						scenario_full_name = scenario_name + " " + bucket_str+ " ";

					double a = 0.;
					double sigma = 0.;
					map<int, map<int, vector<double>>>::const_iterator itr_calib_scen = map_calib_parameters.find(scen_pass_n);
					if(itr_calib_scen!=map_calib_parameters.end())
					{
					    map<int, vector<double>>::const_iterator iter_calib_bucket = itr_calib_scen->second.find(bucket_id);
						if(iter_calib_bucket!= itr_calib_scen->second.end())
						{
							vector<double> v_calib = iter_calib_bucket->second;
							a = v_calib[0];
							sigma = v_calib[1];
						}
					}

					// write the output line
    				 sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
							<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
							<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
							<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << nb_of_policies  << "," << DLL_VERSION << endl;
					 
				}
			}
		}
	}
	void get_MP_vector_and_results_for_ehc_recalc(const map <string, vector<Model_Point*>>& ref_mp_map,
												  map <string, vector<Model_Point*>>& mp_map_to_fill,
												  const map<string, map<int,map<int, vector<vector<double>>>>>& ref_res_map,
												  map<string, map<int,map<int, vector<vector<double>>>>>& res_map_to_fill,
												  map <string, vector<int>>& indexes_policies_map)
	{


		for (map <string, vector<Model_Point*>>::const_iterator iter_ref_mp = ref_mp_map.begin(); 
			iter_ref_mp != ref_mp_map.end(); ++iter_ref_mp)
		{

			string prod_code_key = iter_ref_mp->first;

			for(map <string, vector<int>>::const_iterator iter_indexes_policies_map = indexes_policies_map.begin(); 
					iter_indexes_policies_map != indexes_policies_map.end(); ++iter_indexes_policies_map)
				{
					if (prod_code_key == iter_indexes_policies_map->first)
					{
						vector<int> v_indexes = iter_indexes_policies_map->second;
						vector<Model_Point*> v_mp_to_fill;
						vector<Model_Point*> v_mp = iter_ref_mp->second;
						for (unsigned int i = 0 ; i < v_indexes.size(); i++)
						{
							int index = v_indexes[i];
							v_mp_to_fill.push_back(v_mp[index]);
						}
						mp_map_to_fill[prod_code_key] = v_mp_to_fill;
					}
				}
		}

		for (map<string, map<int,map<int, vector<vector<double>>>>>::const_iterator iter_ref_res_map = ref_res_map.begin();
			iter_ref_res_map != ref_res_map.end(); iter_ref_res_map++)
		{
			string prod_code_key = iter_ref_res_map->first;
			
			for(map <string, vector<int>>::const_iterator iter_indexes_policies_map = indexes_policies_map.begin(); 
					iter_indexes_policies_map != indexes_policies_map.end(); ++iter_indexes_policies_map)
				{
					if (prod_code_key == iter_indexes_policies_map->first)
					{
						vector<int> v_indexes = iter_indexes_policies_map->second;
						map<int,map<int, vector<vector<double>>>> map_scen_shock_to_fill;
						map<int,map<int, vector<vector<double>>>> map_scen_shock_ref = iter_ref_res_map->second;

						for(map<int,map<int, vector<vector<double>>>>::const_iterator iter_map_scen_shock_ref = map_scen_shock_ref.begin();
							iter_map_scen_shock_ref != map_scen_shock_ref.end(); ++iter_map_scen_shock_ref)
						{
							int scen_pass_numb = iter_map_scen_shock_ref->first;
							map<int, vector<vector<double>>> map_shock_to_fill;

								map<int, vector<vector<double>>> map_shock_ref = iter_map_scen_shock_ref->second;

								for (map<int, vector<vector<double>>>::const_iterator iter_map_shock_ref = map_shock_ref.begin(); 
											iter_map_shock_ref != map_shock_ref.end(); ++iter_map_shock_ref)
								{
									int shock_nb = iter_map_shock_ref->first;
									vector<vector<double>> pvs_to_fill;

									vector<vector<double>> pvs_to_ref = iter_map_shock_ref->second;
									for (unsigned int i =0; i < v_indexes.size(); i++)
									{
										int index = v_indexes[i];
										pvs_to_fill.push_back(pvs_to_ref[index]);
									}
									map_shock_to_fill[shock_nb] = pvs_to_fill;
									
								}

								map_scen_shock_to_fill[scen_pass_numb] = map_shock_to_fill;
							
						}
						res_map_to_fill[prod_code_key] = map_scen_shock_to_fill;
					}
				}
		}

	}

	void get_MP_vector_and_results_for_ehc_recalc_esg(const map <string, vector<Model_Point*>>& ref_mp_map,
													  map <string, vector<Model_Point*>>& mp_map_to_fill,
													  const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& ref_res_map,
													  map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& res_map_to_fill,
												      map <string, vector<int>>& indexes_policies_map)
	{


		for (map <string, vector<Model_Point*>>::const_iterator iter_ref_mp = ref_mp_map.begin(); 
			iter_ref_mp != ref_mp_map.end(); ++iter_ref_mp)
		{

			string prod_code_key = iter_ref_mp->first;

			for(map <string, vector<int>>::const_iterator iter_indexes_policies_map = indexes_policies_map.begin(); 
					iter_indexes_policies_map != indexes_policies_map.end(); ++iter_indexes_policies_map)
				{
					if (prod_code_key == iter_indexes_policies_map->first)
					{
						vector<int> v_indexes = iter_indexes_policies_map->second;
						vector<Model_Point*> v_mp_to_fill;
						vector<Model_Point*> v_mp = iter_ref_mp->second;
						for (unsigned int i = 0 ; i < v_indexes.size(); i++)
						{
							int index = v_indexes[i];
							v_mp_to_fill.push_back(v_mp[index]);
						}
						mp_map_to_fill[prod_code_key] = v_mp_to_fill;
					}
				}
		}

		for (map<string, map<int, map<int,map<int, vector<vector<double>>>>>>::const_iterator iter_ref_res_map = ref_res_map.begin();
			iter_ref_res_map != ref_res_map.end(); iter_ref_res_map++)
		{
			string prod_code_key = iter_ref_res_map->first;
			
			for(map <string, vector<int>>::const_iterator iter_indexes_policies_map = indexes_policies_map.begin(); 
					iter_indexes_policies_map != indexes_policies_map.end(); ++iter_indexes_policies_map)
				{
					if (prod_code_key == iter_indexes_policies_map->first)
					{
						vector<int> v_indexes = iter_indexes_policies_map->second;
						map<int, map<int,map<int, vector<vector<double>>>>> map_scen_shock_to_fill;
						map<int, map<int,map<int, vector<vector<double>>>>> map_scen_shock_ref = iter_ref_res_map->second;

						for(map<int, map<int,map<int, vector<vector<double>>>>>::const_iterator iter_map_scen_shock_ref = map_scen_shock_ref.begin();
							iter_map_scen_shock_ref != map_scen_shock_ref.end(); ++iter_map_scen_shock_ref)
						{
							int scen_pass_numb = iter_map_scen_shock_ref->first;
							map<int,map<int, vector<vector<double>>>> map_bucket_to_fill;
							map<int, map<int, vector<vector<double>>>> map_bucket_ref = iter_map_scen_shock_ref->second;

							for(map<int,map<int, vector<vector<double>>>>::const_iterator iter_map_bucket_shock_ref = map_bucket_ref.begin();
								iter_map_bucket_shock_ref != map_bucket_ref.end(); ++iter_map_bucket_shock_ref)
							{
								int bucket_id = iter_map_bucket_shock_ref->first;
								map<int, vector<vector<double>>> map_shock_to_fill;
								map<int, vector<vector<double>>> map_shock_ref = iter_map_bucket_shock_ref->second;

								for (map<int, vector<vector<double>>>::const_iterator iter_map_shock_ref = map_shock_ref.begin(); 
											iter_map_shock_ref != map_shock_ref.end(); ++iter_map_shock_ref)
								{
									int shock_nb = iter_map_shock_ref->first;
									vector<vector<double>> pvs_to_fill;

									vector<vector<double>> pvs_to_ref = iter_map_shock_ref->second;
									for (unsigned int i =0; i < v_indexes.size(); i++)
									{
										int index = v_indexes[i];
										pvs_to_fill.push_back(pvs_to_ref[index]);
									}
									map_shock_to_fill[shock_nb] = pvs_to_fill;
								}
								map_bucket_to_fill[bucket_id] = map_shock_to_fill;
							}
							map_scen_shock_to_fill[scen_pass_numb] = map_bucket_to_fill;
							
						}
						res_map_to_fill[prod_code_key] = map_scen_shock_to_fill;
					}
				}
		}

	}


	

	virtual void write_ehc_summary_file(const map<string, vector<Model_Point*>>& mps, 
									const map<string, map<int,map<int, vector<vector<double>>>>>& index_results_map,
									const map<string, map<int,map<int, vector<vector<double>>>>>& fund_results_map,
									const map<int,vector<double>>& map_calib_parameters)
	{
		map<string, vector<double>> map_ehc_values;

		map <string, vector<int>> indexes_policies_1;
		map <string, vector<int>> indexes_policies_0;
		map <string, vector<int>> indexes_policies_1_new;

		map<string, vector<double>> map_ehc_values_NB;
		map<string, vector<double>> map_ehc_values_recalc_old;
		map<string, vector<double>> map_ehc_values_recalc_new;


		// Open a file 
		char file_name[MAX_LENGTH_FILE_NAME];
		
		char dateStr [9];
		char timeStr [9];
		_strdate( dateStr);
		_strtime( timeStr );
		string month = strtok(dateStr,"/");
		string today = strtok (NULL, "/") ;
		string year = "20" + (string)strtok (NULL, "/") ;
		string hour = strtok(timeStr,":");
		string minute = strtok(NULL,":");
		
		sprintf(file_name, "%s%s%s%s%s_%s%s_1_1.txt", this->summary_configuration->get_ehc_file_directory().c_str(), 
									this->summary_configuration->get_ehc_file_name().c_str(),
									year.c_str(), month.c_str(), today.c_str(), hour.c_str(), minute.c_str());
		ofstream ehc_file(file_name);
		// Loop on the products
		for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
				iter_mp != mps.end(); ++iter_mp)
		{
			string pCK = iter_mp->first;
			vector<Model_Point*> model_points = iter_mp->second;
			vector<vector<double>> pvs = this->get_pvs_vector(pCK, summary_configuration->get_ehc_scen_pass_num(), 0, index_results_map);
			int number_of_mps = model_points.size();
			vector<double> ehc_vector;
			for(int i = 0; i < number_of_mps ; i++)
			{
				Model_Point *model_p = model_points[i];
				vector<double> pv = pvs[i];
				ehc_file << "V1.0" << ",";
				string country_code = pCK.substr(0,pCK.find("_"));
				ehc_file << country_code.c_str() << ",";
				ehc_file << model_p->product.c_str() << ",";
				ehc_file << model_p->group.c_str() << ",";
				ehc_file << pCK.c_str() << ",";
				ehc_file << model_p->policy_id.c_str() << ",";
				ehc_file << "1" << ",";
				ehc_file << "1" << ",";
				ehc_file << model_p->gmxb_g_c << ",";
				ehc_file << model_p->gmxb_ehc_charge << ",";
				double agg_real_charges = pv[PV_GMXB_REAL_CHG];
				double agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
				double agg_dep_fee = pv[PV_DEP_FEE];
				ehc_file << setprecision(15) << agg_real_charges << ",";		
				ehc_file << setprecision(15) << agg_claims << ",";				
				ehc_file << setprecision(15) << agg_dep_fee ;
				ehc_file << endl;
				// calculate the EHC for each policy
				// the ehc is (PVClaims/PVCharges) * RRC

				if (!this->summary_configuration->get_recalculate_param_indicator())
				{
					double ehc = 0;
					if (agg_real_charges != 0)
						ehc = ((agg_claims - agg_dep_fee)/agg_real_charges) * model_p->gmxb_g_c;
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Charges equal 0 for policy number %s ", model_p->policy_id.c_str());
						throw new GMXBException(message);		
					}
					ehc_vector.push_back(ehc);
				}
			}
			if (!this->summary_configuration->get_recalculate_param_indicator())
				map_ehc_values[pCK] = ehc_vector;
		}
		ehc_file.close();



		if (this->summary_configuration->get_recalculate_param_indicator())
		{

			fill_ehc_map(mps, map_ehc_values, index_results_map, fund_results_map, indexes_policies_1, 
				indexes_policies_0, indexes_policies_1_new, map_ehc_values_NB, map_ehc_values_recalc_old, 
				map_ehc_values_recalc_new);
		}



		// Generate .OK and .start files

		char ok_file_name[MAX_LENGTH_FILE_NAME];
		char start_file_name[MAX_LENGTH_FILE_NAME];

		sprintf_s(ok_file_name, "%s%s%s%s%s_%s%s_1_1.ok.txt", 
			this->summary_configuration->get_ehc_file_directory().c_str() , this->summary_configuration->get_ehc_file_name().c_str() 
			, year.c_str(), month.c_str(), today.c_str(),  hour.c_str(), minute.c_str());

		sprintf_s(start_file_name, "%s%s%s%s%s_%s%s_1_1.start.txt", 
			this->summary_configuration->get_ehc_file_directory().c_str() , this->summary_configuration->get_ehc_file_name().c_str() 
			, year.c_str(), month.c_str(), today.c_str(),  hour.c_str(), minute.c_str());


		ofstream ok_file(ok_file_name);
		ofstream start_file(start_file_name);

		ok_file.close();
		start_file.close();

		if (!this->summary_configuration->get_recalculate_param_indicator())
		{
			char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
			sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_ehc_sensi_file_directory().c_str(), 
				this->summary_configuration->get_ehc_sensi_file_name().c_str());

			ofstream sensi_file(sensitivities_file_name);
			sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;
			for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
					iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
				int size_of_index_shock_array = index_shock_array.size();
				for(int s = 0; s < size_of_index_shock_array; s++ )
				{

					int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					double pv_realch = 0.;
					double pv_ehcch = 0.;
					double pv_dep_fee = 0.;
					double pv_sumpremium=0.;
					double pv_paidpremium=0.;
					double pv_claims_ab=0.;
					double pv_claims_ib=0.;
					double pv_claims_db=0.;
					double pv_claims_wb=0.;
					double pv_claims_sb=0.;
					// Loop on the products
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, index_shock_number, index_results_map);
						vector<double> ehc_values = map_ehc_values[pCK];
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps; i++)
						{
							Model_Point *model_p = model_points[i];
							double ehc_value = ehc_values[i];
							vector<double> pv = pvs[i];

							pv_realch += pv[PV_GMXB_REAL_CHG];
							pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
							pv_dep_fee += pv[PV_DEP_FEE];
							pv_sumpremium=0.;
							pv_paidpremium=0.;					
							pv_claims_ab+=pv[PV_GMAB];
							pv_claims_ib+=pv[PV_GMIB];
							pv_claims_db+=pv[PV_GMDB];
							pv_claims_wb+=pv[PV_GMWB];
							pv_claims_sb+=pv[PV_GMSB];
						}
					}

					ostringstream oss_scen_pass_n;
					ostringstream oss_shock_n;

					string scenario_full_name;
					string scenario_name = "";
					string index_shock_name = "";

					oss_scen_pass_n << scen_pass_n;
					oss_shock_n << index_shock_number;
					scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
					
					if (index_shock_number > 0)
					{
						index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
						scenario_full_name = scenario_name + " " + index_shock_name;
					}
					else
						scenario_full_name = scenario_name;

					double a = 0.;
					double sigma = 0.;
					for(map<int, vector<double>>::const_iterator iter_calib = map_calib_parameters.begin();
						iter_calib != map_calib_parameters.end(); ++iter_calib)
					{
						if (iter_calib->first == scen_pass_n)
						{
							vector<double> v_calib = iter_calib->second;
							a = v_calib[0];
							sigma = v_calib[1];
						}
					}				
					// write the output line
				 sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
							<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
							<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
							<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << this->nb_total_polices  << "," << DLL_VERSION << endl;
				}
			}


			for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->fund_map_shocks.begin(); 
					iter_scen_pass_num != this->fund_map_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				vector<vector<int>> fund_shock_array = iter_scen_pass_num->second;
				int size_of_fund_shock_array = fund_shock_array.size();
				int number_of_funds = 0;
				if (size_of_fund_shock_array > 0)
					number_of_funds =  fund_shock_array[0].size();

				for(int s = 0; s < size_of_fund_shock_array; s++ )
				{

					int fund_shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
					double pv_realch = 0.;
					double pv_ehcch = 0.;
					double pv_dep_fee = 0.;
					double pv_sumpremium=0.;
					double pv_paidpremium=0.;
					double pv_claims_ab=0.;
					double pv_claims_ib=0.;
					double pv_claims_db=0.;
					double pv_claims_wb=0.;
					double pv_claims_sb=0.;
					// Loop on the products
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, fund_shock_number, fund_results_map);
						vector<double> ehc_values = map_ehc_values[pCK];
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps; i++)
						{
							Model_Point *model_p = model_points[i];
							double ehc_value = ehc_values[i];
							vector<double> pv = pvs[i];
							pv_realch += pv[PV_GMXB_REAL_CHG];
							pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
							pv_dep_fee += pv[PV_DEP_FEE];
							pv_sumpremium=0.;
							pv_paidpremium=0.;					
							pv_claims_ab+=pv[PV_GMAB];
							pv_claims_ib+=pv[PV_GMIB];
							pv_claims_db+=pv[PV_GMDB];
							pv_claims_wb+=pv[PV_GMWB];
							pv_claims_sb+=pv[PV_GMSB];
						}
					}

					ostringstream oss_scen_pass_n;
					ostringstream oss_shock_n;

					string scenario_full_name;
					string scenario_name = "";
					string fund_shock_name = "";

					oss_scen_pass_n << scen_pass_n;
					oss_shock_n << fund_shock_number;
					scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];

					// Get the fund name
					int fund_id = fund_shock_array[s][0];
					int shock_value = fund_shock_array[s][fund_shock_array[s][0]]; 

					vector<Product_configuration> products = this->summary_configuration->get_product_configurations();

					//Fund_Manager *f_mgr = Fund_Manager::getInstance();
					//string fund_name = f_mgr->get_fund_name_by_fund_id(fund_id);

					string fund_name = products[0].get_fund_name_by_fund_id(fund_id);

					ostringstream shock_value_str;
					shock_value_str << shock_value;

					fund_shock_name = fund_name + " " + shock_value_str.str() + "%";
					
					scenario_full_name = scenario_name + " " + fund_shock_name;

					double a = 0.;
					double sigma = 0.;
					for(map<int, vector<double>>::const_iterator iter_calib = map_calib_parameters.begin();
						iter_calib != map_calib_parameters.end(); ++iter_calib)
					{
						if (iter_calib->first == scen_pass_n)
						{
							vector<double> v_calib = iter_calib->second;
							a = v_calib[0];
							sigma = v_calib[1];
						}
					}				
					// write the output line
					 sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
							<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
							<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
							<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << this->nb_total_polices  << "," << DLL_VERSION << endl;
				}
			}


			sensi_file.close();
		}
		else 
		{

			// Write the sensitivity file
			// Case of new business file

			map <string, vector<Model_Point*>> map_mp_NB;
			map <string, vector<Model_Point*>> map_mp_recalc_old;
			map <string, vector<Model_Point*>> map_mp_recalc_new;

			map<string, map<int,map<int, vector<vector<double>>>>> index_results_map_NB;
			map<string, map<int,map<int, vector<vector<double>>>>> index_results_map_recalc_old;
			map<string, map<int,map<int, vector<vector<double>>>>> index_results_map_recalc_new;

			

			this->get_MP_vector_and_results_for_ehc_recalc(mps, map_mp_NB, index_results_map,
																	index_results_map_NB, indexes_policies_1);

			this->get_MP_vector_and_results_for_ehc_recalc(mps, map_mp_recalc_old, index_results_map,
																	index_results_map_recalc_old, indexes_policies_0);

			this->get_MP_vector_and_results_for_ehc_recalc(mps, map_mp_recalc_new, index_results_map,
																	index_results_map_recalc_new, indexes_policies_1_new);
			


			string sensi_NB = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + ".csv";

			string sensi_recalc_old = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + "_recalc_old.csv";

			string sensi_recalc_new = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + "_recalc_new.csv";



			write_sensi_file(sensi_NB, map_mp_NB, index_results_map_NB, map_ehc_values_NB, map_calib_parameters, this->nb_of_policies_NB);

			write_sensi_file(sensi_recalc_old, map_mp_recalc_old, index_results_map_recalc_old, map_ehc_values_recalc_old, map_calib_parameters, this->nb_of_policies_recalc_old);

			write_sensi_file(sensi_recalc_new, map_mp_recalc_new, index_results_map_recalc_new, map_ehc_values_recalc_new, map_calib_parameters, this->nb_of_policies_recalc_new);

		}

	}
                                             //!   product         scen    bucket    shock       pv
	virtual void write_ehc_summary_file_esg(const map<string, vector<Model_Point*>>& mps, 
											const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& index_results_map,
											const map<string, map<int, map<int,map<int, vector<vector<double>>>>>>& fund_results_map,
											const map<int, map<int,vector<double>>>& map_calib_parameters)
	{
		map<string, vector<double>> map_ehc_values;

		map <string, vector<int>> indexes_policies_1;
		map <string, vector<int>> indexes_policies_0;
		map <string, vector<int>> indexes_policies_1_new;

		map<string, vector<double>> map_ehc_values_NB;
		map<string, vector<double>> map_ehc_values_recalc_old;
		map<string, vector<double>> map_ehc_values_recalc_new;


		// Open a file 
		char file_name[MAX_LENGTH_FILE_NAME];
		
		char dateStr [9];
		char timeStr [9];
		_strdate( dateStr);
		_strtime( timeStr );
		string month = strtok(dateStr,"/");
		string today = strtok (NULL, "/") ;
		string year = "20" + (string)strtok (NULL, "/") ;
		string hour = strtok(timeStr,":");
		string minute = strtok(NULL,":");
		
		sprintf(file_name, "%s%s%s%s%s_%s%s_1_1.txt", this->summary_configuration->get_ehc_file_directory().c_str(), 
									this->summary_configuration->get_ehc_file_name().c_str(),
									year.c_str(), month.c_str(), today.c_str(), hour.c_str(), minute.c_str());
		ofstream ehc_file(file_name);
		// Loop on the products
		for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
				iter_mp != mps.end(); ++iter_mp)
		{
			string pCK = iter_mp->first;
			vector<Model_Point*> model_points = iter_mp->second;
			vector<vector<double>> pvs = this->get_pvs_vector_esg(pCK, summary_configuration->get_ehc_scen_pass_num(), 1, 0, index_results_map);
			int number_of_mps = model_points.size();
			vector<double> ehc_vector;
			for(int i = 0; i < number_of_mps ; i++)
			{
				Model_Point *model_p = model_points[i];
				vector<double> pv = pvs[i];
				ehc_file << "V1.0" << ",";
				string country_code = pCK.substr(0,pCK.find("_"));
				ehc_file << country_code.c_str() << ",";
				ehc_file << model_p->product.c_str() << ",";
				ehc_file << model_p->group.c_str() << ",";
				ehc_file << pCK.c_str() << ",";
				ehc_file << model_p->policy_id.c_str() << ",";
				ehc_file << "1" << ",";
				ehc_file << "1" << ",";
				ehc_file << model_p->gmxb_g_c << ",";
				ehc_file << model_p->gmxb_ehc_charge << ",";
				double agg_real_charges = pv[PV_GMXB_REAL_CHG];
				double agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
				double agg_dep_fee = pv[PV_DEP_FEE];
				ehc_file << setprecision(15) << agg_real_charges << ",";		
				ehc_file << setprecision(15) << agg_claims << ",";				
				ehc_file << setprecision(15) << agg_dep_fee ;
				ehc_file << endl;
				// calculate the EHC for each policy
				// the ehc is (PVClaims/PVCharges) * RRC

				if (!this->summary_configuration->get_recalculate_param_indicator())
				{
					double ehc = 0;
					if (agg_real_charges != 0)
						ehc = ((agg_claims - agg_dep_fee)/agg_real_charges) * model_p->gmxb_g_c;
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Charges equal 0 for policy number %s ", model_p->policy_id.c_str());
						throw new GMXBException(message);		
					}
					ehc_vector.push_back(ehc);
				}
			}
			if (!this->summary_configuration->get_recalculate_param_indicator())
				map_ehc_values[pCK] = ehc_vector;
		}
		ehc_file.close();



		if (this->summary_configuration->get_recalculate_param_indicator())
		{

			fill_ehc_map_esg(mps, map_ehc_values, index_results_map, fund_results_map, indexes_policies_1, 
				indexes_policies_0, indexes_policies_1_new, map_ehc_values_NB, map_ehc_values_recalc_old, 
				map_ehc_values_recalc_new);
		}



		// Generate .OK and .start files

		char ok_file_name[MAX_LENGTH_FILE_NAME];
		char start_file_name[MAX_LENGTH_FILE_NAME];

		sprintf_s(ok_file_name, "%s%s%s%s%s_%s%s_1_1.ok.txt", 
			this->summary_configuration->get_ehc_file_directory().c_str() , this->summary_configuration->get_ehc_file_name().c_str() 
			, year.c_str(), month.c_str(), today.c_str(),  hour.c_str(), minute.c_str());

		sprintf_s(start_file_name, "%s%s%s%s%s_%s%s_1_1.start.txt", 
			this->summary_configuration->get_ehc_file_directory().c_str() , this->summary_configuration->get_ehc_file_name().c_str() 
			, year.c_str(), month.c_str(), today.c_str(),  hour.c_str(), minute.c_str());


		ofstream ok_file(ok_file_name);
		ofstream start_file(start_file_name);

		ok_file.close();
		start_file.close();

		if (!this->summary_configuration->get_recalculate_param_indicator())
		{
			char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
			sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_ehc_sensi_file_directory().c_str(), 
				this->summary_configuration->get_ehc_sensi_file_name().c_str());

			ofstream sensi_file(sensitivities_file_name);
			// sensi_file  << "scenario, pv_claims, pv_realch, pv_ehcch, pv_dep_fee, a, sigma" << endl;
			sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;

			for (map<int, map<int,vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->index_map_shocks_esg.begin(); 
					iter_scen_pass_num != this->index_map_shocks_esg.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				for (map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
					iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
				{
					int bucket_id = iter_bucket->first;
					vector<vector<int>> index_shock_array = iter_bucket->second;
					int size_of_index_shock_array = index_shock_array.size();
					for(int s = 0; s < size_of_index_shock_array; s++ )
					{

						int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						// double pv_claims = 0.;
						//double pv_realch = 0.;
						//double pv_ehcch = 0.;
						// double pv_dep_fee = 0.;

						double pv_realch = 0.;
						double pv_ehcch = 0.;
						double pv_dep_fee = 0.;
						double pv_sumpremium=0.;
						double pv_paidpremium=0.;
						double pv_claims_ab=0.;
						double pv_claims_ib=0.;
						double pv_claims_db=0.;
						double pv_claims_wb=0.;
						double pv_claims_sb=0.;

						// Loop on the products
						for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
								iter_mp != mps.end(); ++iter_mp)
						{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;
							vector<vector<double>> pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id, index_shock_number, index_results_map);
							vector<double> ehc_values = map_ehc_values[pCK];
							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps; i++)
							{
								Model_Point *model_p = model_points[i];
								double ehc_value = ehc_values[i];
								vector<double> pv = pvs[i];
								/*
								pv_claims += pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								pv_realch += pv[PV_GMXB_REAL_CHG];
								pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
								*/


								pv_realch += pv[PV_GMXB_REAL_CHG];
								pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
								pv_dep_fee += pv[PV_DEP_FEE];
								pv_sumpremium=0.;
								pv_paidpremium=0.;					
								pv_claims_ab+=pv[PV_GMAB];
								pv_claims_ib+=pv[PV_GMIB];
								pv_claims_db+=pv[PV_GMDB];
								pv_claims_wb+=pv[PV_GMWB];
								pv_claims_sb+=pv[PV_GMSB];



							}
						}

						string scenario_name = this->list_scenario_names[Converter::convert_to_str(scen_pass_n)][0];
							// Converter::convert_to_str(scen_pass_n);
						string bucket_name = Converter::convert_to_str(bucket_id);
						string index_shock_name = Converter::convert_to_str(index_shock_number);
						string scenario_full_name;

						if (index_shock_number > 0)
						{
							index_shock_name = this->list_index_shock_names[index_shock_name.c_str()][0];
							scenario_full_name = scenario_name + " " + bucket_name + " " + index_shock_name;
						}
						else
							scenario_full_name = scenario_name + " " + bucket_name;

						double a = 0.;
						double sigma = 0.;
						map<int, map<int, vector<double>>>::const_iterator itr_calib_scen = map_calib_parameters.find(scen_pass_n);
						if(itr_calib_scen!=map_calib_parameters.end())
						{
						    map<int, vector<double>>::const_iterator iter_calib_bucket = itr_calib_scen->second.find(bucket_id);
							if(iter_calib_bucket!= itr_calib_scen->second.end())
							{
								vector<double> v_calib = iter_calib_bucket->second;
								a = v_calib[0];
								sigma = v_calib[1];
							}
						}

						// write the output line
						/*
						sensi_file << scenario_full_name.c_str() << "," 
								   << setprecision(15) << pv_claims << ","
								   << setprecision(15) << pv_realch << ","
								   << setprecision(15) << pv_ehcch 	<< ","
								   << setprecision(15) << pv_dep_fee << ","
								   << setprecision(15)<< a << ","
								   << setprecision(15) << sigma << endl;
								   */

    				 sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
							<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
							<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
							<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << this->nb_total_polices  << "," << DLL_VERSION << endl;


					}
				}
			}


			for (map<int, map<int, vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->fund_map_shocks_esg.begin(); 
					iter_scen_pass_num != this->fund_map_shocks_esg.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;

				for (map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
					iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
				{
					int bucket_id = iter_bucket->first;
					vector<vector<int>> fund_shock_array = iter_bucket->second;
					int size_of_fund_shock_array = fund_shock_array.size();
					int number_of_funds = 0;
					if (size_of_fund_shock_array > 0)
						number_of_funds =  fund_shock_array[0].size();

					for(int s = 0; s < size_of_fund_shock_array; s++ )
					{
						int fund_shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);

						double pv_realch = 0.;
						double pv_ehcch = 0.;
						double pv_dep_fee = 0.;
						double pv_sumpremium=0.;
						double pv_paidpremium=0.;
						double pv_claims_ab=0.;
						double pv_claims_ib=0.;
						double pv_claims_db=0.;
						double pv_claims_wb=0.;
						double pv_claims_sb=0.;
						
						
						// Loop on the products
						for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
								iter_mp != mps.end(); ++iter_mp)
						{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;
							vector<vector<double>> pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id, fund_shock_number, fund_results_map);
							vector<double> ehc_values = map_ehc_values[pCK];
							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps; i++)
							{
								Model_Point *model_p = model_points[i];
								double ehc_value = ehc_values[i];
								vector<double> pv = pvs[i];
								/*
								pv_claims += pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								pv_realch += pv[PV_GMXB_REAL_CHG];
								pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
*/


								pv_realch += pv[PV_GMXB_REAL_CHG];
								pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
								pv_dep_fee += pv[PV_DEP_FEE];
								pv_sumpremium=0.;
								pv_paidpremium=0.;					
								pv_claims_ab+=pv[PV_GMAB];
								pv_claims_ib+=pv[PV_GMIB];
								pv_claims_db+=pv[PV_GMDB];
								pv_claims_wb+=pv[PV_GMWB];
								pv_claims_sb+=pv[PV_GMSB];

							}
						}

						ostringstream oss_scen_pass_n;
						ostringstream oss_shock_n;

						string scenario_full_name;
						string scenario_name = "";
						string fund_shock_name = "";

						oss_scen_pass_n << scen_pass_n;
						oss_shock_n << fund_shock_number;
						scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];

						// Get the fund name
						int fund_id = fund_shock_array[s][0];
						int shock_value = fund_shock_array[s][fund_shock_array[s][0]]; 

						vector<Product_configuration> products = this->summary_configuration->get_product_configurations();

						//Fund_Manager *f_mgr = Fund_Manager::getInstance();
						//string fund_name = f_mgr->get_fund_name_by_fund_id(fund_id);

						string fund_name = products[0].get_fund_name_by_fund_id(fund_id);

						ostringstream shock_value_str;
						shock_value_str << shock_value;

						fund_shock_name = fund_name + " " + shock_value_str.str() + "%";
						
						string bucket_name = Converter::convert_to_str(bucket_id);

						scenario_full_name = scenario_name + " " + bucket_name + " " + fund_shock_name;


						double a = 0.;
						double sigma = 0.;
						map<int, map<int, vector<double>>>::const_iterator itr_calib_scen = map_calib_parameters.find(scen_pass_n);
						if(itr_calib_scen!=map_calib_parameters.end())
						{
						    map<int, vector<double>>::const_iterator iter_calib_bucket = itr_calib_scen->second.find(bucket_id);
							if(iter_calib_bucket!= itr_calib_scen->second.end())
							{
								vector<double> v_calib = iter_calib_bucket->second;
								a = v_calib[0];
								sigma = v_calib[1];
							}
						}

						// write the output line
					     sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
						<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," 
						<< setprecision(15) << pv_paidpremium << "," << setprecision(15)
						<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db 
						<< "," << setprecision(15) << pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," 
						<< setprecision(15) << a << "," << setprecision(15) << sigma << "," << this->nb_total_polices  << "," << DLL_VERSION << endl;

					}
				}
			}


			sensi_file.close();
		}
		else 
		{

			// Write the sensitivity file
			// Case of new business file

			map <string, vector<Model_Point*>> map_mp_NB;
			map <string, vector<Model_Point*>> map_mp_recalc_old;
			map <string, vector<Model_Point*>> map_mp_recalc_new;

			map<string, map<int, map<int,map<int, vector<vector<double>>>>>> index_results_map_NB;
			map<string, map<int, map<int,map<int, vector<vector<double>>>>>> index_results_map_recalc_old;
			map<string, map<int, map<int,map<int, vector<vector<double>>>>>> index_results_map_recalc_new;

			

			this->get_MP_vector_and_results_for_ehc_recalc_esg(mps, map_mp_NB, index_results_map,
																	index_results_map_NB, indexes_policies_1);

			this->get_MP_vector_and_results_for_ehc_recalc_esg(mps, map_mp_recalc_old, index_results_map,
																	index_results_map_recalc_old, indexes_policies_0);

			this->get_MP_vector_and_results_for_ehc_recalc_esg(mps, map_mp_recalc_new, index_results_map,
																	index_results_map_recalc_new, indexes_policies_1_new);
			


			string sensi_NB = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + ".csv";

			string sensi_recalc_old = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + "_recalc_old.csv";

			string sensi_recalc_new = this->summary_configuration->get_ehc_sensi_file_directory() +  
							  this->summary_configuration->get_ehc_sensi_file_name() + "_recalc_new.csv";



			write_sensi_file_esg(sensi_NB, map_mp_NB, index_results_map_NB, map_ehc_values_NB, map_calib_parameters, this->nb_of_policies_NB);

			write_sensi_file_esg(sensi_recalc_old, map_mp_recalc_old, index_results_map_recalc_old, map_ehc_values_recalc_old, map_calib_parameters, this->nb_of_policies_recalc_old);

			write_sensi_file_esg(sensi_recalc_new, map_mp_recalc_new, index_results_map_recalc_new, map_ehc_values_recalc_new, map_calib_parameters, this->nb_of_policies_recalc_new);

		}

	}


	                                          //    product       scen   bucket_i   shock        pv
	virtual void write_total_summary_file_esg(const map<string, map<int, map<int, map<int, vector<double>>>>>& index_results_map,
										      const map<string, map<int, map<int, map<int, vector<double>>>>>& fund_results_map,
										      const map<int, map<int,vector<double>>>& map_calib_parameters)
	{

		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_total_file_directory().c_str(), 
				this->summary_configuration->get_total_file_name().c_str());

		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;
		
		//! loop over scens
		for (map<int, map<int, vector<vector<int>>>>::const_iterator itr_scen = this->index_map_shocks_esg.begin(); 
				itr_scen != this->index_map_shocks_esg.end(); ++itr_scen)
		{ 
			int scen_pn = itr_scen->first;
			//! loop over buckets
			for (map<int, vector<vector<int>>>::const_iterator itr_bucket = itr_scen->second.begin(); 
				itr_bucket != itr_scen->second.end(); ++itr_bucket)
			{
				int bucket_id = itr_bucket->first;
				vector<vector<int>> index_shock_array = itr_bucket->second;
				//! loop over shocks
				for(int s = 0; s < index_shock_array.size(); ++s)
				{
					int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					//double pv_claims = 0.;
					double pv_realch = 0.;
					double pv_ehcch = 0.;
					double pv_dep_fee = 0.;
					double pv_sumpremium=0.;
					double pv_paidpremium=0.;
					double pv_claims_ab=0.;
					double pv_claims_ib=0.;
					double pv_claims_db=0.;
					double pv_claims_wb=0.;
					double pv_claims_sb=0.;
					// Loop over products
					for(map<string, map<int, map<int, map<int, vector<double>>>>>::const_iterator itr_product = index_results_map.begin();
							itr_product != index_results_map.end(); ++itr_product)
					{
						string pCK = itr_product->first;
						vector<double> index_pvs = this->get_pvs_vector_esg(pCK, scen_pn, bucket_id,index_shock_number, index_results_map);
						//pv_claims += index_pvs[PV_GMAB]+ index_pvs[PV_GMDB]+ index_pvs[PV_GMIB] + index_pvs[PV_GMWB] + index_pvs[PV_GMSB];
						pv_realch += index_pvs[PV_GMXB_REAL_CHG];
						pv_ehcch += index_pvs[PV_GMXB_EHC_CHG];
						pv_dep_fee += index_pvs[PV_DEP_FEE];
						pv_sumpremium=0.;
						pv_paidpremium=0.;					
						pv_claims_ab+=index_pvs[PV_GMAB];
						pv_claims_ib+=index_pvs[PV_GMIB];
						pv_claims_db+=index_pvs[PV_GMDB];
						pv_claims_wb+=index_pvs[PV_GMWB];
						pv_claims_sb+=index_pvs[PV_GMSB];
					}

					ostringstream oss_scen_pass_n;
					oss_scen_pass_n << scen_pn;
					string scenario_name = "";

					ostringstream oss_bucket_n;
					oss_bucket_n    << bucket_id;
					string bucket_name = oss_bucket_n.str();

					ostringstream oss_shock_n;
					oss_shock_n     << index_shock_number;
					string index_shock_name = "";
					
					string scenario_full_name;

					scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
					if (index_shock_number > 0)
						index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
					
				    //! ??? bucket_name strang
					scenario_full_name = scenario_name + " " + bucket_name + " " + index_shock_name;

					double a = 0.;
					double sigma = 0.;

					 for(map<int,map<int, vector<double>>>::const_iterator itr_calib_scen = map_calib_parameters.begin();
						itr_calib_scen != map_calib_parameters.end(); ++itr_calib_scen)
					 {
						int calib_scen = itr_calib_scen->first;
						if (calib_scen == scen_pn)
						{
							for(map<int, vector<double>>::const_iterator itr_calib_bucket = itr_calib_scen->second.begin();
								itr_calib_bucket != itr_calib_scen->second.end(); ++itr_calib_bucket)
							{
                                int calib_bucket =  itr_calib_bucket->first;
								if(calib_bucket == bucket_id)
								{
									vector<double> v_calib = itr_calib_bucket->second;
									a = v_calib[0];
									sigma = v_calib[1];
								}
							}
						}
					 }
					 // write the output line
					 sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," << setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db << "," << setprecision(15)
							<< pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," << setprecision(15) << a << "," << setprecision(15) << sigma << "," << setprecision(15) << this->nb_total_polices  << "," << DLL_VERSION<< endl;
				}
			}
		}


		//! loop over fund
		for (map<int, map<int, vector<vector<int>>>>::const_iterator itr_scen = this->fund_map_shocks_esg.begin(); 
				itr_scen != this->fund_map_shocks_esg.end(); ++itr_scen)
		{
			int scen_pn = itr_scen->first;
			for (map<int, vector<vector<int>>>::const_iterator itr_bucket = itr_scen->second.begin(); 
				itr_bucket != itr_scen->second.end(); ++itr_bucket)
			{
				int bucket_id = itr_bucket->first;
				vector<vector<int>> fund_shock_array = itr_bucket->second;

				int fund_size_of_shock_array = fund_shock_array.size();
				int number_of_funds = 0;
				if (fund_size_of_shock_array > 0)
					number_of_funds = fund_shock_array[0].size(); 

				for(int s = 0; s < fund_size_of_shock_array; s++ )
				{
						int fund_shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
						double pv_realch = 0.;
						double pv_ehcch = 0.;
						double pv_dep_fee = 0.;
						double pv_sumpremium=0.;
						double pv_paidpremium=0.;
						double pv_claims_ab=0.;
						double pv_claims_ib=0.;
						double pv_claims_db=0.;
						double pv_claims_wb=0.;
						double pv_claims_sb=0.;
						// Loop on the products
						for(map<string, map<int, map<int, map<int, vector<double>>>>>::const_iterator itr_product = fund_results_map.begin();
								itr_product != fund_results_map.end(); ++itr_product)
						{
							string pCK = itr_product->first;
							vector<double> fund_pvs = this->get_pvs_vector_esg(pCK, scen_pn, bucket_id, fund_shock_number, fund_results_map);

							pv_realch += fund_pvs[PV_GMXB_REAL_CHG];
							pv_ehcch += fund_pvs[PV_GMXB_EHC_CHG];
							pv_dep_fee += fund_pvs[PV_DEP_FEE];
							pv_sumpremium=0.;
							pv_paidpremium=0.;					
							pv_claims_ab+=fund_pvs[PV_GMAB];
							pv_claims_ib+=fund_pvs[PV_GMIB];
							pv_claims_db+=fund_pvs[PV_GMDB];
							pv_claims_wb+=fund_pvs[PV_GMWB];
							pv_claims_sb+=fund_pvs[PV_GMSB];
						}

						ostringstream oss_scen_pass_n;
						oss_scen_pass_n << scen_pn;
						string scenario_name = "";
						
						ostringstream oss_shock_n;
						oss_shock_n << fund_shock_number;
						string fund_shock_name = "";

						ostringstream oss_bucket_n;
						oss_bucket_n    << bucket_id;
						string bucket_name = oss_bucket_n.str();
						
						string scenario_full_name;
						
						scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];

						// Get the fund name
						int fund_id = fund_shock_array[s][0];
						int shock_value = fund_shock_array[s][fund_shock_array[s][0]]; 

						//Fund_Manager *f_mgr = Fund_Manager::getInstance();
						//string fund_name = f_mgr->get_fund_name_by_fund_id(fund_id);

						vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
						string fund_name = products[0].get_fund_name_by_fund_id(fund_id);

						ostringstream shock_value_str;
						shock_value_str << shock_value;

						fund_shock_name = fund_name + " " + shock_value_str.str() + "%";
						
						scenario_full_name = scenario_name + " " + bucket_name + " " + fund_shock_name;

						double a = 0.;
						double sigma = 0.;

						for(map<int,map<int, vector<double>>>::const_iterator itr_calib_scen = map_calib_parameters.begin();
							itr_calib_scen != map_calib_parameters.end(); ++itr_calib_scen)
						{
							int calib_scen = itr_calib_scen->first;
							if (calib_scen == scen_pn)
							{
								for(map<int, vector<double>>::const_iterator itr_calib_bucket = itr_calib_scen->second.begin();
									itr_calib_bucket != itr_calib_scen->second.end(); ++itr_calib_bucket)
								{
								 int calib_bucket =  itr_calib_bucket->first;
									if(calib_bucket == bucket_id)
									{
										vector<double> v_calib = itr_calib_bucket->second;
										a = v_calib[0];
										sigma = v_calib[1];
									}
								}
							}
						}

						// write the output line
						sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
							<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," << setprecision(15) << pv_paidpremium << "," << setprecision(15)
							<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db << "," << setprecision(15)
							<< pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," << setprecision(15) << a << "," << setprecision(15) << sigma << "," << setprecision(15) << this->nb_total_polices  << "," << DLL_VERSION<< endl;
				}
			}
		}


		sensi_file.close();
	}


                                          //    product       scen     shock        pv
	virtual void write_total_summary_file(const map<string, map<int, map<int, vector<double>>>>& index_results_map,
										  const map<string, map<int, map<int, vector<double>>>>& fund_results_map,
										  const map<int,vector<double>>& map_calib_parameters)
	{
		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_total_file_directory().c_str(), 
				this->summary_configuration->get_total_file_name().c_str());
		
		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, total_agg_real_charges,total_agg_ehc_chi_charges,total_dep_fees,total_sum_prem_fees,total_paid_prem_fees,total_Claims_AB,total_Claims_IB,total_Claims_DB,total_Claims_WB,total_Claims_SB, a, sigma,Total_Police_nb,DLL_Version" << endl;
		
		//! loops over scenario
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int index_size_of_shock_array = index_shock_array.size();

			//! loop over index_shock
			for(int s = 0; s < index_size_of_shock_array; s++ )
			{   
				int index_shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
				//double pv_claims = 0.;
				double pv_realch = 0.;
				double pv_ehcch = 0.;
				double pv_dep_fee = 0.;
				double pv_sumpremium=0.;
				double pv_paidpremium=0.;
				double pv_claims_ab=0.;
				double pv_claims_ib=0.;
				double pv_claims_db=0.;
				double pv_claims_wb=0.;
				double pv_claims_sb=0.;

				// Loop on the products
				for(map<string, map<int, map<int, vector<double>>>>::const_iterator iter_products = index_results_map.begin();
						iter_products != index_results_map.end(); ++iter_products)
				{
					string pCK = iter_products->first;
					vector<double> index_pvs = this->get_pvs_vector(pCK, scen_pass_n, index_shock_number, index_results_map);
					//pv_claims += index_pvs[PV_GMAB]+ index_pvs[PV_GMDB]+ index_pvs[PV_GMIB] + index_pvs[PV_GMWB] + index_pvs[PV_GMSB];
					pv_realch += index_pvs[PV_GMXB_REAL_CHG];
					pv_ehcch += index_pvs[PV_GMXB_EHC_CHG];
					pv_dep_fee += index_pvs[PV_DEP_FEE];
					pv_sumpremium=0.;
					pv_paidpremium=0.;					
					pv_claims_ab+=index_pvs[PV_GMAB];
					pv_claims_ib+=index_pvs[PV_GMIB];
					pv_claims_db+=index_pvs[PV_GMDB];
					pv_claims_wb+=index_pvs[PV_GMWB];
					pv_claims_sb+=index_pvs[PV_GMSB];
				}

				ostringstream oss_scen_pass_n;
				ostringstream oss_shock_n;
				string scenario_full_name;
				string scenario_name = "";
				string index_shock_name = "";
				oss_scen_pass_n << scen_pass_n;
				oss_shock_n << index_shock_number;
				scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
				if (index_shock_number > 0)
					index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
				
				scenario_full_name = scenario_name + " " + index_shock_name;

				double a = 0.;
				double sigma = 0.;

				for(map<int, vector<double>>::const_iterator iter_calib = map_calib_parameters.begin();
					iter_calib != map_calib_parameters.end(); ++iter_calib)
				{
					if (iter_calib->first == scen_pass_n)
					{
						vector<double> v_calib = iter_calib->second;
						a = v_calib[0];
						sigma = v_calib[1];
					}
				}


				// write the output line
				sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
					<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," << setprecision(15) << pv_paidpremium << "," << setprecision(15)
					<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db << "," << setprecision(15)
					<< pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," << setprecision(15) << a << "," << setprecision(15) << sigma << "," << setprecision(15)  << this->nb_total_polices << ","<< DLL_VERSION << endl;
			}
		}

		//! fonds shocks
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->fund_map_shocks.begin(); 
				iter_scen_pass_num != this->fund_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> fund_shock_array = iter_scen_pass_num->second;
			int fund_size_of_shock_array = fund_shock_array.size();
			int number_of_funds = 0;
			if (fund_size_of_shock_array > 0)
				number_of_funds = fund_shock_array[0].size(); 

			for(int s = 0; s < fund_size_of_shock_array; s++ )
			{
				int fund_shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
				double pv_realch = 0.;
				double pv_ehcch = 0.;
				double pv_dep_fee = 0.;
				double pv_sumpremium=0.;
				double pv_paidpremium=0.;
				double pv_claims_ab=0.;
				double pv_claims_ib=0.;
				double pv_claims_db=0.;
				double pv_claims_wb=0.;
				double pv_claims_sb=0.;
				// Loop on the products
				for(map<string, map<int, map<int, vector<double>>>>::const_iterator iter_products = fund_results_map.begin();
						iter_products != fund_results_map.end(); ++iter_products)
				{
					string pCK = iter_products->first;
					vector<double> fund_pvs = this->get_pvs_vector(pCK, scen_pass_n, fund_shock_number, fund_results_map);

					pv_realch += fund_pvs[PV_GMXB_REAL_CHG];
					pv_ehcch += fund_pvs[PV_GMXB_EHC_CHG];
					pv_dep_fee += fund_pvs[PV_DEP_FEE];
					pv_sumpremium=0.;
					pv_paidpremium=0.;					
					pv_claims_ab+=fund_pvs[PV_GMAB];
					pv_claims_ib+=fund_pvs[PV_GMIB];
					pv_claims_db+=fund_pvs[PV_GMDB];
					pv_claims_wb+=fund_pvs[PV_GMWB];
					pv_claims_sb+=fund_pvs[PV_GMSB];
				}

				ostringstream oss_scen_pass_n;
				ostringstream oss_shock_n;
				string scenario_full_name;
				string scenario_name = "";
				string fund_shock_name = "";

				oss_scen_pass_n << scen_pass_n;
				oss_shock_n << fund_shock_number;
				scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];

				// Get the fund name
				int fund_id = fund_shock_array[s][0];
				int shock_value = fund_shock_array[s][fund_shock_array[s][0]]; 

				//Fund_Manager *f_mgr = Fund_Manager::getInstance();
				//string fund_name = f_mgr->get_fund_name_by_fund_id(fund_id);

				vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
				
				string fund_name = products[0].get_fund_name_by_fund_id(fund_id);


				ostringstream shock_value_str;
				shock_value_str << shock_value;

				fund_shock_name = fund_name + " " + shock_value_str.str() + "%";
				
				scenario_full_name = scenario_name + " " + fund_shock_name;



				double a = 0.;
				double sigma = 0.;

				for(map<int, vector<double>>::const_iterator iter_calib = map_calib_parameters.begin();
					iter_calib != map_calib_parameters.end(); ++iter_calib)
				{
					if (iter_calib->first == scen_pass_n)
					{
						vector<double> v_calib = iter_calib->second;
						a = v_calib[0];
						sigma = v_calib[1];
					}
				}


				// write the output line
				sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_realch << ","  << setprecision(15) 
					<< pv_ehcch << "," << setprecision(15) << pv_dep_fee  << "," << setprecision(15) << pv_sumpremium << "," << setprecision(15) << pv_paidpremium << "," << setprecision(15)
					<< pv_claims_ab << "," << setprecision(15) << pv_claims_ib << "," << setprecision(15) << pv_claims_db << "," << setprecision(15)
					<< pv_claims_wb << "," << setprecision(15) << pv_claims_sb << "," << setprecision(15) << a << "," << setprecision(15) << sigma << "," << setprecision(15) << this->nb_total_polices  << "," << DLL_VERSION<< endl;
			}
		}
		sensi_file.close();
	}


	virtual void write_pol_by_pol(const map<string, vector<Model_Point*>>& mps, 
											const map<string, map<int,map<int, vector<vector<double>>>>>& index_results_map,
											const map<string, map<int,map<int, vector<vector<double>>>>>& fund_results_map,
											const map<string, map<int,map<string, vector<vector<double>>>>>& results_lapse,
											const map<string, map<int,map<string, vector<vector<double>>>>>& results_mort,
											const map<string, map<int, vector<vector<double>>>>& results_longevity,
											const map<string, map<int,map<string, vector<vector<double>>>>>& results_paidup,
											const map<string, map<int,map<string, vector<vector<double>>>>>& results_election
											)
	{
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int index_size_of_shock_array = index_shock_array.size();
			for(int s = 0; s < index_size_of_shock_array; s++ )
			{
				int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

				char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
				sprintf(pol_by_pol_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
					this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, shock_number);

				ofstream pol_by_pol_file(pol_by_pol_file_name);

				pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_G_C_Gross,GMXB_H_C,GMXB_CHI,agg_real_charges,agg_EHC_charges,agg_claims,pv_ehc_av_charges,agg_dep_fee,sum_of_premium,paid_premium_charges,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMIB_Fond,PV_GMWB,PV_GMSB,LOV,Policy_moneyness,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;
				// Loop on the products
				for(map<string, map<int, map<int, vector<vector<double>>>>>::const_iterator iter_products = index_results_map.begin();
						iter_products != index_results_map.end(); ++iter_products)
				{
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> index_pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, index_results_map);
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> index_pv = index_pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = index_pv[PV_GMXB_REAL_CHG];
							agg_claims = index_pv[PV_GMAB]+ index_pv[PV_GMDB]+ index_pv[PV_GMIB] + index_pv[PV_GMWB] + index_pv[PV_GMSB];
							agg_dep_fee = index_pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file <<Av0<<",";
							pol_by_pol_file << endl;
						}
				}
			}
			pol_by_pol_file.close();
		}
		}
		
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->fund_map_shocks.begin(); 
				iter_scen_pass_num != this->fund_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> fund_shock_array = iter_scen_pass_num->second;
			int fund_size_of_shock_array = fund_shock_array.size();
			int number_of_funds = 0;
			if (fund_size_of_shock_array > 0)
				number_of_funds = fund_shock_array[0].size();	


			for(int s = 0; s < fund_size_of_shock_array; s++ )
			{
				int shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);

				char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
				sprintf(pol_by_pol_file_name, "%s%s_fund_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
					this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, shock_number);

				ofstream pol_by_pol_file(pol_by_pol_file_name);

				pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;

				// Loop on the products
				for(map<string, map<int, map<int, vector<vector<double>>>>>::const_iterator iter_products = fund_results_map.begin();
						iter_products != fund_results_map.end(); ++iter_products)
				{
					// string pCK = iter_products->first;
					//vector<vector<double>> fund_pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, fund_results_map);

					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> fund_pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, fund_results_map);
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> fund_pv = fund_pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = fund_pv[PV_GMXB_REAL_CHG];
							agg_claims = fund_pv[PV_GMAB]+ fund_pv[PV_GMDB]+ fund_pv[PV_GMIB] + fund_pv[PV_GMWB] + fund_pv[PV_GMSB];
							agg_dep_fee = fund_pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << fund_pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0<< ",";
							pol_by_pol_file << endl;
						}
				}
			}
			pol_by_pol_file.close();
		}
		}

		// lapse
		vector<Product_configuration> v_p_c = this->summary_configuration->get_product_configurations();
		if (v_p_c.size() > 0)
		{
			for(unsigned int pC = 0; pC < v_p_c.size(); pC++)
			{
				Product_configuration p = v_p_c[pC];
				vector<Scenario_configuration> v_scen_conf = p.get_scenario_list();
				for(unsigned int sc_n = 0; sc_n < v_scen_conf.size(); sc_n++)
				{
					Scenario_configuration s = v_scen_conf[sc_n];
					vector<GMXB_Assumption_Table *> v_lapse = s.get_lapse_mult();
					int scen_pass_n = s.get_scenario_pass_number();
					for (unsigned int l = 0 ; l < v_lapse.size(); l++)
					//if (v_lapse.size() > 0)
					{
					GMXB_Assumption_Table *tab = v_lapse[l];

					string sheet_name = tab->get_sheet_name();
					
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, tab->get_sheet_name().c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
				//	pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
                    pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_G_C_Gross,GMXB_H_C,GMXB_CHI,agg_real_charges,agg_EHC_charges,agg_claims,pv_ehc_av_charges,agg_dep_fee,sum_of_premium,paid_premium_charges,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMIB_Fond,PV_GMWB,PV_GMSB,LOV,Policy_moneyness,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs;

						for(map<string, map<int,map<string, vector<vector<double>>>>>::const_iterator iter_pvs = results_lapse.begin();
							iter_pvs != results_lapse.end(); ++iter_pvs)
						{
							string prod_code_k = iter_pvs->first;
							if (prod_code_k == pCK)
							{
								map<int,map<string, vector<vector<double>>>> map_scen_pass_num = iter_pvs->second;
								for(map<int,map<string, vector<vector<double>>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
									iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
									{
										if (iter_scen_pass_num->first == scen_pass_n)
										{
											map<string, vector<vector<double>>> map_lapses = iter_scen_pass_num->second;

											for (map<string, vector<vector<double>>>::const_iterator iter_lapse = map_lapses.begin();
												iter_lapse != map_lapses.end(); ++iter_lapse)
											{
												string s_name = iter_lapse->first;
												if (sheet_name == s_name)
												{
													pvs = iter_lapse->second;
												}
											}
										}
									}
							}
						}
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
						    double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}
							//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					  }
					pol_by_pol_file.close();

					}
				}
			}
		}

		if (v_p_c.size() > 0)
		{
			for(unsigned int pC = 0; pC < v_p_c.size(); pC++)
			{
				Product_configuration p = v_p_c[pC];
				vector<Scenario_configuration> v_scen_conf = p.get_scenario_list();
				for(unsigned int sc_n = 0; sc_n < v_scen_conf.size(); sc_n++)
				{
					Scenario_configuration s = v_scen_conf[sc_n];
					this->v_mortality = s.get_mortality_mult();
					int scen_pass_n = s.get_scenario_pass_number();
					for(unsigned int m = 0; m < this->v_mortality.size(); m++)
					// if (v_mort.size() > 0)
					{
					GMXB_Assumption_Table *tab = this->v_mortality[m];

					string sheet_name = tab->get_sheet_name();
					
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, tab->get_sheet_name().c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					//pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
                      pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_G_C_Gross,GMXB_H_C,GMXB_CHI,agg_real_charges,agg_EHC_charges,agg_claims,pv_ehc_av_charges,agg_dep_fee,sum_of_premium,paid_premium_charges,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMIB_Fond,PV_GMWB,PV_GMSB,LOV,Policy_moneyness,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs;

						for(map<string, map<int,map<string, vector<vector<double>>>>>::const_iterator iter_pvs = results_mort.begin();
							iter_pvs != results_mort.end(); ++iter_pvs)
						{
							string prod_code_k = iter_pvs->first;
							if (prod_code_k == pCK)
							{
								map<int,map<string, vector<vector<double>>>> map_scen_pass_num = iter_pvs->second;
								for(map<int,map<string, vector<vector<double>>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
									iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
									{
										if (iter_scen_pass_num->first == scen_pass_n)
										{
											map<string, vector<vector<double>>> map_morts = iter_scen_pass_num->second;

											for (map<string, vector<vector<double>>>::const_iterator iter_mort = map_morts.begin();
												iter_mort != map_morts.end(); ++iter_mort)
											{
												string s_name = iter_mort->first;
												if (sheet_name == s_name)
												{
													pvs = iter_mort->second;
												}
											}
										}
									}
							}
						}
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
						    pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					  }
					pol_by_pol_file.close();

					}
				}
			}
		}
		//Paid up
		if (v_p_c.size() > 0)
		{
			for(unsigned int pC = 0; pC < v_p_c.size(); pC++)
			{
				Product_configuration p = v_p_c[pC];
				vector<Scenario_configuration> v_scen_conf = p.get_scenario_list();
				for(unsigned int sc_n = 0; sc_n < v_scen_conf.size(); sc_n++)
				{
					Scenario_configuration s = v_scen_conf[sc_n];
					vector<GMXB_Assumption_Table *> v_paidup = s.get_paidup_mult();
					int scen_pass_n = s.get_scenario_pass_number();
					int bucket_id   = s.get_bucket_id();
					for (unsigned int l = 0 ; l < v_paidup.size(); l++)
					
					{
					GMXB_Assumption_Table *tab = v_paidup[l];

					string sheet_name = tab->get_sheet_name();
					
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, tab->get_sheet_name().c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;

					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs;

						for(map<string, map<int,map<string, vector<vector<double>>>>>::const_iterator iter_pvs = results_paidup.begin();
							iter_pvs != results_paidup.end(); ++iter_pvs)
						{
							string prod_code_k = iter_pvs->first;
							if (prod_code_k == pCK)
							{
								map<int,map<string, vector<vector<double>>>> map_scen_pass_num = iter_pvs->second;
								for(map<int,map<string, vector<vector<double>>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
									iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
									{
										if (iter_scen_pass_num->first == scen_pass_n)
										{
											map<string, vector<vector<double>>> map_paidup = iter_scen_pass_num->second;

											for (map<string, vector<vector<double>>>::const_iterator iter_paidup = map_paidup.begin();
												iter_paidup != map_paidup.end(); ++iter_paidup)
											{
												string s_name = iter_paidup->first;
												if (sheet_name == s_name)
												{
													pvs = iter_paidup->second;
												}
											}
										}
									}
							}
						}
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}		
							//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					  }
					pol_by_pol_file.close();

					}
				}
			}
		}

		//election
		if (v_p_c.size() > 0)
		{
			for(unsigned int pC = 0; pC < v_p_c.size(); pC++)
			{
				Product_configuration p = v_p_c[pC];
				vector<Scenario_configuration> v_scen_conf = p.get_scenario_list();
				for(unsigned int sc_n = 0; sc_n < v_scen_conf.size(); sc_n++)
				{
					Scenario_configuration s = v_scen_conf[sc_n];
					vector<GMXB_Assumption_Table *> v_election = s.get_election_mult();
					int scen_pass_n = s.get_scenario_pass_number();
					for (unsigned int l = 0 ; l < v_election.size(); l++)
					
					{
					GMXB_Assumption_Table *tab = v_election[l];

					string sheet_name = tab->get_sheet_name();
					
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, tab->get_sheet_name().c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;

					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs;

						for(map<string, map<int,map<string, vector<vector<double>>>>>::const_iterator iter_pvs = results_election.begin();
							iter_pvs != results_election.end(); ++iter_pvs)
						{
							string prod_code_k = iter_pvs->first;
							if (prod_code_k == pCK)
							{
								map<int,map<string, vector<vector<double>>>> map_scen_pass_num = iter_pvs->second;
								for(map<int,map<string, vector<vector<double>>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
									iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
									{
										if (iter_scen_pass_num->first == scen_pass_n)
										{
											map<string, vector<vector<double>>> map_election = iter_scen_pass_num->second;

											for (map<string, vector<vector<double>>>::const_iterator iter_election = map_election.begin();
												iter_election != map_election.end(); ++iter_election)
											{
												string s_name = iter_election->first;
												if (sheet_name == s_name)
												{
													pvs = iter_election->second;
												}
											}
										}
									}
							}
						}
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}	
							//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					  }
					pol_by_pol_file.close();

					}
				}
			}
		}
		// Longevity part
		if (v_p_c.size() > 0)
		{
			for(unsigned int pC = 0; pC < v_p_c.size(); pC++)
			{
				Product_configuration p = v_p_c[pC];
				vector<Scenario_configuration> v_scen_conf = p.get_scenario_list();
				for(unsigned int sc_n = 0; sc_n < v_scen_conf.size(); sc_n++)
				{
					Scenario_configuration s = v_scen_conf[sc_n];

					if (s.get_load_longevity_table())
					{

					int scen_pass_n = s.get_scenario_pass_number();
					
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_longevity.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n);

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					//pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
                      pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_G_C_Gross,GMXB_H_C,GMXB_CHI,agg_real_charges,agg_EHC_charges,agg_claims,pv_ehc_av_charges,agg_dep_fee,sum_of_premium,paid_premium_charges,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMIB_Fond,PV_GMWB,PV_GMSB,LOV,Policy_moneyness,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs;

						for(map<string, map<int, vector<vector<double>>>>::const_iterator iter_pvs = results_longevity.begin();
							iter_pvs != results_longevity.end(); ++iter_pvs)
						{
							string prod_code_k = iter_pvs->first;
							if (prod_code_k == pCK)
							{
								map<int, vector<vector<double>>> map_scen_pass_num = iter_pvs->second;
								for(map<int, vector<vector<double>>>::const_iterator iter_scen_pass_num = map_scen_pass_num.begin();
									iter_scen_pass_num != map_scen_pass_num.end(); ++iter_scen_pass_num)
									{
										if (iter_scen_pass_num->first == scen_pass_n)
										{
											pvs = iter_scen_pass_num->second;
										}
									}
							}
						}
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							{
							   Av0+=model_p->av_if_array[k];
							}			
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	

							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					  }
					pol_by_pol_file.close();
				}
					}
			}
		}
	}




	virtual void write_pol_by_pol_esg(const map<string, vector<Model_Point*>>& mps, 
											const map<string, map<int,map<int,map<int, vector<vector<double>>>>>>& index_results_map,
											const map<string, map<int,map<int,map<int, vector<vector<double>>>>>>& fund_results_map,
											const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_lapse,
											const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_mort,
											const map<string, map<int,map<int, vector<vector<double>>>>>& results_longevity,
											const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_paidup,
											const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_election
											)
	{
		//! index_shock
		for (map<int, map<int, vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->index_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->index_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
            for(map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<vector<int>> index_shock_array = iter_bucket->second;
				int index_size_of_shock_array = index_shock_array.size();
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, shock_number);

					ofstream pol_by_pol_file(pol_by_pol_file_name);

					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_G_C_Gross,GMXB_H_C,GMXB_CHI,agg_real_charges,agg_EHC_charges,agg_claims,pv_ehc_av_charges,agg_dep_fee,sum_of_premium,paid_premium_charges,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMIB_Fond,PV_GMWB,PV_GMSB,LOV,Policy_moneyness,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;
					// Loop on the products
					for(map<string, map<int, map<int, map<int, vector<vector<double>>>>>>::const_iterator iter_products = index_results_map.begin();
							iter_products != index_results_map.end(); ++iter_products)
					{
						for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
						{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;
							vector<vector<double>> index_pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id, shock_number, index_results_map);
							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> index_pv = index_pvs[i];
								double Av0=0;
							    for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = index_pv[PV_GMXB_REAL_CHG];
								agg_claims = index_pv[PV_GMAB]+ index_pv[PV_GMDB]+ index_pv[PV_GMIB] + index_pv[PV_GMWB] + index_pv[PV_GMSB];
								agg_dep_fee = index_pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMXB_EHC_CHG] << ",";	

								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
						}
					}
					pol_by_pol_file.close();
				}
			}
		}
		
		//! fund shock
		for (map<int,map<int, vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->fund_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->fund_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			
			for(map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;

				vector<vector<int>> fund_shock_array = iter_bucket->second;
				int fund_size_of_shock_array = fund_shock_array.size();
				int number_of_funds = 0;
				if (fund_size_of_shock_array > 0)
					number_of_funds = fund_shock_array[0].size();	

				for(int s = 0; s < fund_size_of_shock_array; s++ )
				{
					int shock_number = this->get_shock_number(fund_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);

					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					sprintf(pol_by_pol_file_name, "%s%s_fund_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, shock_number);

					ofstream pol_by_pol_file(pol_by_pol_file_name);

					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,GMWB_WDL_RATE,AV0" << endl;

					// Loop on the products
					for(map<string, map<int, map<int, map<int, vector<vector<double>>>>>>::const_iterator iter_products = fund_results_map.begin();
							iter_products != fund_results_map.end(); ++iter_products)
					{
						// string pCK = iter_products->first;
						//vector<vector<double>> fund_pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, fund_results_map);

						for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
						{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;
							vector<vector<double>> fund_pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id, shock_number, fund_results_map);
							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> fund_pv = fund_pvs[i];
								double Av0=0;
								for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = fund_pv[PV_GMXB_REAL_CHG];
								agg_claims = fund_pv[PV_GMAB]+ fund_pv[PV_GMDB]+ fund_pv[PV_GMIB] + fund_pv[PV_GMWB] + fund_pv[PV_GMSB];
								agg_dep_fee = fund_pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMXB_EHC_CHG] << ",";	

								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << fund_pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
						}
					}
					pol_by_pol_file.close();
				}
			}
		}

		//! lapse  
		for(map<int, map<int,vector<string>>>::const_iterator iter_scen_pass_num = this->lapse_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->lapse_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			
			for(map<int,vector<string>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<string> v_lapse_names = iter_bucket->second;

				for(int iter_lapse = 0; iter_lapse < v_lapse_names.size(); ++ iter_lapse)
				{
					string sheet_name = v_lapse_names[iter_lapse];
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, sheet_name.c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
					
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;

							vector<vector<double>> pvs = get_pvs_vector_esg2(pCK,
											  scen_pass_n,
											  bucket_id,
											  sheet_name, 
											  results_lapse);

							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> pv = pvs[i];
								double Av0=0;
								for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = pv[PV_GMXB_REAL_CHG];
								agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								agg_dep_fee = pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	
								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
					}
					// Close the file
					pol_by_pol_file.close();
				}
			}
		}


		//! Mortality
		for(map<int, map<int,vector<string>>>::const_iterator iter_scen_pass_num = this->mortality_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->mortality_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			
			for(map<int,vector<string>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<string> v_mortality_names = iter_bucket->second;

				for(int iter_mortality = 0; iter_mortality < v_mortality_names.size(); ++ iter_mortality)
				{
					string sheet_name = v_mortality_names[iter_mortality];
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, sheet_name.c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
					
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;

							vector<vector<double>> pvs = get_pvs_vector_esg2(pCK,
											  scen_pass_n,
											  bucket_id,
											  sheet_name, 
											  results_mort);

							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> pv = pvs[i];
								double Av0=0;
								for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = pv[PV_GMXB_REAL_CHG];
								agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								agg_dep_fee = pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	
								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
					}
					// Close the file
					pol_by_pol_file.close();
				}
			}
		}


		//! Paidup
		for(map<int, map<int,vector<string>>>::const_iterator iter_scen_pass_num = this->paidup_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->paidup_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			
			for(map<int,vector<string>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<string> v_paidup_names = iter_bucket->second;

				for(int iter_paidup = 0; iter_paidup < v_paidup_names.size(); ++ iter_paidup)
				{
					string sheet_name = v_paidup_names[iter_paidup];
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, sheet_name.c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
					
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;

							vector<vector<double>> pvs = get_pvs_vector_esg2(pCK,
											  scen_pass_n,
											  bucket_id,
											  sheet_name, 
											  results_paidup);

							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> pv = pvs[i];
								double Av0=0;
								for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = pv[PV_GMXB_REAL_CHG];
								agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								agg_dep_fee = pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	
								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
					}
					// Close the file
					pol_by_pol_file.close();
				}
			}
		}


		//electoion
		for(map<int, map<int,vector<string>>>::const_iterator iter_scen_pass_num = this->election_map_shocks_esg.begin(); 
				iter_scen_pass_num != this->election_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			
			for(map<int,vector<string>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); 
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;
				vector<string> v_election_names = iter_bucket->second;

				for(int iter_election = 0; iter_election < v_election_names.size(); ++ iter_election)
				{
					string sheet_name = v_election_names[iter_election];
					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id, sheet_name.c_str());

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;
					
					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
							string pCK = iter_mp->first;
							vector<Model_Point*> model_points = iter_mp->second;

							vector<vector<double>> pvs = get_pvs_vector_esg2(pCK,
											  scen_pass_n,
											  bucket_id,
											  sheet_name, 
											  results_election);

							int number_of_mps = model_points.size();
							for(int i = 0; i < number_of_mps ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								Model_Point *model_p = model_points[i];
								vector<double> pv = pvs[i];
								double Av0=0;
								for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
								//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
								pol_by_pol_file << "V1.0" << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";
								pol_by_pol_file << model_p->product.c_str() << ",";
								pol_by_pol_file << model_p->group.c_str() << ",";
								pol_by_pol_file << pCK.c_str() << ",";
								pol_by_pol_file << model_p->policy_id.c_str() << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << "1" << ",";
								pol_by_pol_file << model_p->gmxb_g_c << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
								pol_by_pol_file << "0" << ",";
								agg_real_charges = pv[PV_GMXB_REAL_CHG];
								agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
								agg_dep_fee = pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
								pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	
								pol_by_pol_file << setprecision(15) << agg_claims << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << "0" << ",";
								pol_by_pol_file << model_p->age_valn << ",";
								pol_by_pol_file << model_p->duration << ",";
								pol_by_pol_file << model_p->gmxb_type2 << ",";
								pol_by_pol_file << model_p->sex << ",";
								pol_by_pol_file << model_p->prem_paid << ",";
								pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
								pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
								pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmab_rollup_valn << ",";
								pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
								pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
								pol_by_pol_file << model_p->gmxb_deferral_period << ",";
								pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
								pol_by_pol_file << model_p->gmxb_m_c << ",";
								pol_by_pol_file << model_p->gmxb_a_c << ",";
								pol_by_pol_file << model_p->axa_roll_r << ",";
								pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
								pol_by_pol_file << Av0 << ",";
								pol_by_pol_file << endl;
							}
					}
					// Close the file
					pol_by_pol_file.close();
				}
			}
		}


				/*
			const map<string, map<int,map<int,map<int, vector<vector<double>>>>>>& index_results_map,
			const map<string, map<int,map<int,map<int, vector<vector<double>>>>>>& fund_results_map,
			const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_lapse,
			const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_mort,
			const map<string, map<int,map<int, vector<vector<double>>>>>& results_longevity,
			const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_paidup,
			const map<string, map<int,map<int,map<string, vector<vector<double>>>>>>& results_election
		*/
		/*
		    map<int, map<int,vector<string>>>> lapse_map_shocks_esg;
			map<int, map<int,vector<string>>>> mortality_map_shocks_esg;
			map<int, map<int,vector<string>>>> paidup_map_shocks_esg;
			map<int, map<int,vector<string>>>> election_map_shocks_esg;
		*/

		//map_longevity_esg

        //longetivity
		for(map<int, map<int,bool>>::const_iterator iter_scen_pass_num = this->longevity_map_esg.begin(); 
				iter_scen_pass_num != this->longevity_map_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			map<int,bool> v_bucket = iter_scen_pass_num->second;
			
			for(map<int,bool>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); iter_bucket!= iter_scen_pass_num->second.end(); ++iter_bucket)
			{
		        
				int bucket_id = iter_bucket->first;

				if (iter_bucket->second)
				{

					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_longevity.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, bucket_id);

					ofstream pol_by_pol_file(pol_by_pol_file_name);
					pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R,AV0" << endl;

					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
							iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;

						vector<vector<double>> pvs = get_pvs_vector_esg3(pCK,
																		 scen_pass_n,
																		 bucket_id,
																		 results_longevity);

						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
							double Av0=0;
							for(int k=0;k<MAXIMUM_NUMBER_OF_INDEXES;k++)
							    {
							     Av0+=model_p->av_if_array[k];
							    }
							//cout << "i = "  << i << " size of pvs = " << setprecision(15) << pvs[i].size() << endl;
							pol_by_pol_file << "V1.0" << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";
							pol_by_pol_file << model_p->product.c_str() << ",";
							pol_by_pol_file << model_p->group.c_str() << ",";
							pol_by_pol_file << pCK.c_str() << ",";
							pol_by_pol_file << model_p->policy_id.c_str() << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << "1" << ",";
							pol_by_pol_file << model_p->gmxb_g_c << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							pol_by_pol_file << "0" << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";	
							pol_by_pol_file << setprecision(15) << pv[PV_GMXB_EHC_CHG] << ",";	
							pol_by_pol_file << setprecision(15) << agg_claims << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << "0" << ",";
							pol_by_pol_file << model_p->age_valn << ",";
							pol_by_pol_file << model_p->duration << ",";
							pol_by_pol_file << model_p->gmxb_type2 << ",";
							pol_by_pol_file << model_p->sex << ",";
							pol_by_pol_file << model_p->prem_paid << ",";
							pol_by_pol_file << model_p->av_split_prop_array[0] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[1] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[2] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[3] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[4] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[5] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[6] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[7] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[8] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[9] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[10] << ",";
							pol_by_pol_file << model_p->av_split_prop_array[11] << ",";
							pol_by_pol_file << model_p->gmab_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmab_rollup_valn << ",";
							pol_by_pol_file << model_p->gmdb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmdb_rollup_valn << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_valn << ",";
							pol_by_pol_file << model_p->gmwb_rollup_valn  << ",";
							pol_by_pol_file << model_p->gmxb_deferral_period << ",";
							pol_by_pol_file << model_p->gmwb_ratchet_period << ",";
							pol_by_pol_file << model_p->gmxb_m_c << ",";
							pol_by_pol_file << model_p->gmxb_a_c << ",";
							pol_by_pol_file << model_p->axa_roll_r << ",";
							pol_by_pol_file << model_p->gmwb_wdl_rate << ",";
							pol_by_pol_file << Av0 << ",";
							pol_by_pol_file << endl;
						}
					}
					// Close the file
					pol_by_pol_file.close();
				}
			}
		}
		


	}

	virtual void write_scen_by_scen(const map<string, map<int,map<int, vector<vector<double>>>>>& index_results_map,
									const map<string, map<int,map<int, vector<vector<double>>>>>& fund_results_map,
									const map<string, map<int,map<string, vector<vector<double>>>>>& results_lapse,
									const map<string, map<int,map<string, vector<vector<double>>>>>& results_mort,
									const map<string, map<int, vector<vector<double>>>>& results_longevity,
									const map<string, map<int,map<string, vector<vector<double>>>>>& results_paidup
									)
	{
		//! index shock
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); //! scenPassNumber(fileMode)/bucketid(ESG)
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int index_size_of_shock_array = index_shock_array.size();
			for(int s = 0; s < index_size_of_shock_array; s++ )  //! shock
			{
				int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

				char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];

				if(summary_configuration->get_scen_by_scen_indicator() == true)
				{
				   	sprintf(pol_by_pol_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_scen_by_scen_file_directory().c_str(), 
						this->summary_configuration->get_scen_by_scen_file_name().c_str(), scen_pass_n, shock_number);
				}
				else
				{
				    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Mode of summary Error, in function write_pol_by_pol(...) ");
					throw new GMXBException(message);	
				}

				ofstream pol_by_pol_file(pol_by_pol_file_name);

				pol_by_pol_file  << "idScenPath,Country,prod_type,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,PV_GMXB_EHC_CHG" << endl;

				for(map<string, map<int, map<int, vector<vector<double>>>>>::const_iterator iter_products = index_results_map.begin(); // product
						iter_products != index_results_map.end(); ++iter_products)
				{
						string pCK = iter_products->first;
						vector<vector<double>> index_pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, index_results_map);
						int number_of_scenarioPath = index_pvs.size();
						for(int i = 0; i < number_of_scenarioPath ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							vector<double> index_pv = index_pvs[i];
							pol_by_pol_file << i << ",";
							pol_by_pol_file << pCK << ",";
							string country_code = pCK.substr(0,pCK.find("_"));
							pol_by_pol_file << country_code.c_str() << ",";

							agg_real_charges = index_pv[PV_GMXB_REAL_CHG];
							agg_claims = index_pv[PV_GMAB]+ index_pv[PV_GMDB]+ index_pv[PV_GMIB] + index_pv[PV_GMWB] + index_pv[PV_GMSB];
							agg_dep_fee = index_pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";		
							pol_by_pol_file << setprecision(15) << agg_claims << ",";				
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMIB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMSB] << ",";
							pol_by_pol_file << setprecision(15) << index_pv[PV_GMXB_EHC_CHG] << ",";	
							pol_by_pol_file << endl;
						}
					}
				pol_by_pol_file.close();
			}
		}
	}


	virtual void write_scen_by_scen_esg(const map<string, map<int, map<int,map<int,    vector<vector<double>>>>>>& index_results_map,
										const map<string, map<int, map<int,map<int,    vector<vector<double>>>>>>& fund_results_map,
										const map<string, map<int, map<int,map<string, vector<vector<double>>>>>>& results_lapse,
										const map<string, map<int, map<int,map<string, vector<vector<double>>>>>>& results_mort,
										const map<string, map<int, map<int,            vector<vector<double>>>>> & results_longevity,
										const map<string, map<int, map<int,map<string, vector<vector<double>>>>>>& results_paidup
										)
	{
		//! index shock
		for (map<int, map<int, vector<vector<int>>>>::const_iterator iter_scen_pass_num = this->index_map_shocks_esg.begin(); //! scenPassNumber(fileMode)/bucketid(ESG)
				iter_scen_pass_num != this->index_map_shocks_esg.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			for (map<int, vector<vector<int>>>::const_iterator iter_bucket = iter_scen_pass_num->second.begin(); //! scenPassNumber(fileMode)/bucketid(ESG)
				iter_bucket != iter_scen_pass_num->second.end(); ++iter_bucket)
			{
				int bucket_id = iter_bucket->first;

				vector<vector<int>> index_shock_array = iter_bucket->second;
				int index_size_of_shock_array = index_shock_array.size();
				for(int s = 0; s < index_size_of_shock_array; s++ )  //! shock
				{
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];

					if(summary_configuration->get_scen_by_scen_indicator() == true)
					{
				   		sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_scen_by_scen_file_directory().c_str(), 
							this->summary_configuration->get_scen_by_scen_file_name().c_str(), scen_pass_n, bucket_id, shock_number);
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Mode of summary Error, in function write_pol_by_pol(...) ");
						throw new GMXBException(message);	
					}

					ofstream pol_by_pol_file(pol_by_pol_file_name);

					pol_by_pol_file  << "idScenPath,Country,prod_type,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,PV_GMXB_EHC_CHG" << endl;

					for(map<string, map<int, map<int, map<int, vector<vector<double>>>>>>::const_iterator iter_products = index_results_map.begin(); // product
							iter_products != index_results_map.end(); ++iter_products)
					{
							string pCK = iter_products->first;
							vector<vector<double>> index_pvs = this->get_pvs_vector_esg(pCK, scen_pass_n, bucket_id ,shock_number, index_results_map);
							int number_of_scenarioPath = index_pvs.size();
							for(int i = 0; i < number_of_scenarioPath ; i++)
							{
								double agg_real_charges = 0.;
								double agg_claims = 0.;
								double agg_dep_fee = 0.;
								vector<double> index_pv = index_pvs[i];
								pol_by_pol_file << i << ",";
								pol_by_pol_file << pCK << ",";
								string country_code = pCK.substr(0,pCK.find("_"));
								pol_by_pol_file << country_code.c_str() << ",";

								agg_real_charges = index_pv[PV_GMXB_REAL_CHG];
								agg_claims = index_pv[PV_GMAB]+ index_pv[PV_GMDB]+ index_pv[PV_GMIB] + index_pv[PV_GMWB] + index_pv[PV_GMSB];
								agg_dep_fee = index_pv[PV_DEP_FEE];
								pol_by_pol_file << setprecision(15) << agg_real_charges << ",";		
								pol_by_pol_file << setprecision(15) << agg_claims << ",";				
								pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMAB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMDB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMIB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMWB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMSB] << ",";
								pol_by_pol_file << setprecision(15) << index_pv[PV_GMXB_EHC_CHG] << ",";	
								pol_by_pol_file << endl;
							}
						}
					pol_by_pol_file.close();
				}
			}
		}
	}
	void write_output_summary()
	{

		if (!this->if_multi_esg) //! File mode ou Moses
		{
			if (this->summary_configuration->get_ehc_format_indicator() 
					|| this->summary_configuration->get_policy_by_policy_indicator())
					this->perform_detailed_summary();
			if (this->summary_configuration->get_mso_file_indicator())
        	{this->write_greek_summary_file();}

			if (this->summary_configuration->get_scen_by_scen_indicator())
					 this->perform_scen_summary();

			if (this->summary_configuration->get_total_file_indicator())
					this->perform_total_summary();
		}
		else  // ESG  projection 
		{
			if (this->summary_configuration->get_total_file_indicator())
			{
					this->perform_total_summary_esg();
			}
	
			if (this->summary_configuration->get_ehc_format_indicator() 
						|| this->summary_configuration->get_policy_by_policy_indicator())
			{
					this->perform_detailed_summary_esg();
			}
			if (this->summary_configuration->get_mso_file_indicator())
        	{this->write_greek_summary_file();}

			if (this->summary_configuration->get_scen_by_scen_indicator())
			{
						 this->perform_scen_summary_esg();
			}
		}
	}


	//! Y Rewrite the perform_total_summary_esg_nagib
	void perform_total_summary_esg()
	{
		//! product    scen     bucket   shock    pv
		map<string, map<int, map<int,map<int, vector<double>>>>> index_map_results;
		map<string, map<int, map<int,map<int, vector<double>>>>> fund_map_results;
		map<int, map<int,vector<double>>> map_calib_parameters_scn_pass_num;
          
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();

		this->nb_total_polices=0;
		//! loop over product
		for (int i = 0; i < products.size(); i++)  
		{
            Product_configuration product = products[i];
			string product_key = product.get_prod_code_key();
			vector<Scenario_configuration> v_scen_config = product.get_scenario_list();

			//! loop over scen & bucket
			for(int j=0; j<v_scen_config.size(); ++j)
			{
				Scenario_configuration scen_config = v_scen_config[j];
				int scen_pass_num   = scen_config.get_scenario_pass_number();
				int bucket_id = scen_config.get_bucket_id();

				map<int, vector<double>> index_map_shock; 
				map<int, vector<double>> fund_map_shock;
				vector<double> calib_parameters;

				get_total_results_calib_params(scen_pass_num, bucket_id, scen_config.get_index_shock_array(),
									scen_config.get_fund_shock_array(),index_map_shock, fund_map_shock, 
									calib_parameters, product.get_prod_code_key(),
									product.get_result_file_path(), product.get_run_name());

				index_map_results[product_key][scen_pass_num][bucket_id]    = index_map_shock;
				fund_map_results[product_key][scen_pass_num][bucket_id]     = fund_map_shock;
				map_calib_parameters_scn_pass_num[scen_pass_num][bucket_id] = calib_parameters;
			}

			nb_total_polices+=nb_of_polices;

		}

		this->write_total_summary_file_esg(index_map_results, fund_map_results, map_calib_parameters_scn_pass_num);	
	}


	void get_total_results_calib_params(int scenario_pass_number, int bucket_id, vector<vector<int> > index_shock_array,
										vector<vector<int> > fund_shock_array,
										map<int, vector<double>>& index_map_shock, 
										map<int, vector<double>>& fund_map_shock, 
										vector<double>& calib_parameters, const string& prod_code_keys,
										const string& result_file_path, const string& run_name)
	{
			int index_shock_size = index_shock_array.size();
			int fund_shock_size = fund_shock_array.size();
			double calib_a, calib_sigma;

			string scen_num_str  = Converter::convert_to_str(scenario_pass_number);
			string bucket_id_str = Converter::convert_to_str(bucket_id);

			int number_of_funds = 0;
			if (fund_shock_size > 0)
					number_of_funds = fund_shock_array[0].size();

			for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
			{
					vector<double> index_pv_res;
					int index_shock_num = this->get_shock_number(index_shock_array, index_shock, 
												MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					string shock_num_str = Converter::convert_to_str(index_shock_num);
					string index_shock_file_name = result_file_path + run_name + "_total_index_" 
												   + scen_num_str + "_" + bucket_id_str + "_" + shock_num_str + "_" ;

					this->get_pvs_aggregated(index_shock_file_name, index_pv_res, scenario_pass_number, index_shock_num, calib_a, calib_sigma); 
					index_map_shock[index_shock_num] = index_pv_res;
			}

			for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
			{
					vector<double> fund_pv_res;
					int fund_shock_num = this->get_shock_number(fund_shock_array, fund_shock, 
															MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);

					string shock_num_str = Converter::convert_to_str(fund_shock_num);
					string fund_shock_file_name = result_file_path + run_name + "_total_fund_" 
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					
					this->get_pvs_aggregated(fund_shock_file_name, fund_pv_res, scenario_pass_number, fund_shock_num, calib_a, calib_sigma); 
					fund_map_shock[fund_shock_num] = fund_pv_res;
			}
			calib_parameters.push_back(calib_a);
			calib_parameters.push_back(calib_sigma);
	}



	void perform_total_summary()
	{
		map<string, map<int,map<int, vector<double>>>> index_map_results;
		map<string, map<int,map<int, vector<double>>>> fund_map_results;
		map<int,vector<double>> map_calib_parameters;

		this->nb_total_polices=0;
		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			Product_configuration product = products[i];
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();
			map<int,map<int, vector<double>>> index_map_scen_pass_num;
			map<int,map<int, vector<double>>> fund_map_scen_pass_num;

			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				string scen_num_str;
				stringstream out;
				out << scen_pass_num;
				scen_num_str = out.str();

				int bucket_id     = scenario.get_bucket_id();
				
				string bucket_id_str;
				if(if_multi_esg == true)
				{
					stringstream out2;
					out2 << bucket_id;
					bucket_id_str = out2.str();
				}
				int index_shock_size = scenario.get_index_shock_array().size();
				int fund_shock_size = scenario.get_fund_shock_array().size();
				int number_of_funds = 0;
				if (fund_shock_size > 0)
					number_of_funds = scenario.get_fund_shock_array()[0].size();


				map<int, vector<double>> index_map_shock;
				map<int, vector<double>> fund_map_shock;
				double calib_a;
				double calib_sigma;
				
				for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
				{
					vector<double> index_pv_res;

					int index_shock_num = this->get_shock_number(scenario.get_index_shock_array(), index_shock, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					stringstream out1;
					string shock_num_str;
					out1 << index_shock_num;
					shock_num_str = out1.str();
					
					string index_shock_file_name;
					if(if_multi_esg == false)
						index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_total_index_" 
												   + scen_num_str + "_"  + shock_num_str + "_" ;
					else
						index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_total_index_" 
												   + scen_num_str + "_" + bucket_id_str + "_" + shock_num_str + "_" ;

					this->get_pvs_aggregated(index_shock_file_name, index_pv_res, scen_pass_num, index_shock_num, calib_a, calib_sigma); 
					index_map_shock[index_shock_num] = index_pv_res;
				}

				for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
				{
					vector<double> fund_pv_res;

					int fund_shock_num = this->get_shock_number(scenario.get_fund_shock_array(), fund_shock, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);

					stringstream out1;
					string shock_num_str;
					out1 << fund_shock_num;
					shock_num_str = out1.str();
					
					string fund_shock_file_name;
					if(if_multi_esg == false)					
						fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_total_fund_" 
												   + scen_num_str + "_"  + shock_num_str + "_" ;
					else
						fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_total_fund_" 
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					
					this->get_pvs_aggregated(fund_shock_file_name, fund_pv_res, scen_pass_num, fund_shock_num, calib_a, calib_sigma); 
					fund_map_shock[fund_shock_num] = fund_pv_res;
				}

				vector<double> calib_parameters;
				calib_parameters.push_back(calib_a);
				calib_parameters.push_back(calib_sigma);
				if(if_multi_esg == false)
					map_calib_parameters[scen_pass_num] = calib_parameters;
				else
					map_calib_parameters[bucket_id] = calib_parameters;

				if(if_multi_esg == false)
					index_map_scen_pass_num[scen_pass_num] = index_map_shock;
				else
					index_map_scen_pass_num[bucket_id] = index_map_shock;

				if(if_multi_esg == false)
					fund_map_scen_pass_num[scen_pass_num] = fund_map_shock;
				else
					fund_map_scen_pass_num[bucket_id] = fund_map_shock;
			}
			index_map_results[product.get_prod_code_key()] = index_map_scen_pass_num;
			fund_map_results[product.get_prod_code_key()] = fund_map_scen_pass_num;

			nb_total_polices+=nb_of_polices;
		}
		this->write_total_summary_file(index_map_results, fund_map_results,map_calib_parameters);	
	}	


	void perform_detailed_summary()
	{
		map<string, map<int,map<int, vector<vector<double>>>>> map_index_results;
		map<string, map<int,map<int, vector<vector<double>>>>> map_fund_results;
		map<string, vector<Model_Point*>> mps_per_product;
		map<int,vector<double>> map_calib_parameters;
		map<string, map<int,map<string, vector<vector<double>>>>> map_lapse_results;
		map<string, map<int,map<string, vector<vector<double>>>>> map_mortality_results;
		map<string, map<int,vector<vector<double>>>> map_longevity_results;
		map<string, map<int,map<string, vector<vector<double>>>>> map_paidup_results;
		map<string, map<int,map<string, vector<vector<double>>>>> map_election_results;
		Inforce_Manager* inforce_mgr = NULL;
		
		this->nb_total_polices = 0;
		int nb_of_pv_index_per_worker = 0;
		int nb_of_pv_fund_per_worker = 0;
		bool check = false;

		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			vector<Model_Point*> list_of_mps;
			
			Product_configuration product = products[i];


			inforce_mgr->get_list_of_model_points_per_product(product.get_prod_code_key(), 
													list_of_mps, product, product.get_scenario_list()[0]);
			
			int number_of_model_points = list_of_mps.size();

			mps_per_product[product.get_prod_code_key()] = list_of_mps;
			
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();
			map<int,map<int, vector<vector<double>>>> map_index_scen_pass_num;
			map<int,map<int, vector<vector<double>>>> map_fund_scen_pass_num;

			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_lapse;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_mort;
			map<int, vector<vector<double>>> map_scen_pass_num_longevity;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_paidup;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_election;

			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				string scen_num_str;
				stringstream out;
				out << scen_pass_num;
				scen_num_str  = out.str();

				string bucket_id_str;
				int bucket_id = scenario.get_bucket_id();
				if(if_multi_esg)
				{
					stringstream out2;
					out2 << bucket_id;
					bucket_id_str = out2.str();
				}
				int index_shock_size = scenario.get_index_shock_array().size();
				int fund_shock_size = scenario.get_fund_shock_array().size();
				int number_of_funds = 0;
				if (fund_shock_size > 0)
					number_of_funds = scenario.get_fund_shock_array()[0].size();

				map<int, vector<vector<double>>> map_index_shock;
				map<int, vector<vector<double>>> map_fund_shock;
				map<string, vector<vector<double>>> lapse_shock;
				map<string, vector<vector<double>>> mortality_shock;
				map<string, vector<vector<double>>> paidup_shock;
				map<string, vector<vector<double>>> election_shock;

				double calib_a;
				double calib_sigma;
				//out << scen_pass_num;
				//scen_num_str = out.str();

				for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
				{
					vector<vector<double>> pv_index_res;
					int shock_num = get_shock_number(scenario.get_index_shock_array(), index_shock, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					stringstream out1;
					string shock_num_str;
					out1 << shock_num;
					shock_num_str = out1.str();
					
					string index_shock_file_name;
					if(if_multi_esg == false)
					{
						index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_" 
												   + scen_num_str + "_"  + shock_num_str + "_" ;

					}
					else
					{
						index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_" 
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					}
			
					this->get_pvs_by_policy(index_shock_file_name, pv_index_res, scen_pass_num, shock_num, calib_a, calib_sigma); 
					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					check=true;
					nb_of_pv_index_per_worker = pv_index_res.size();
					if(summary_configuration->get_scen_by_scen_indicator() == false) //! scen_by_scen no check for the moment
					{
						int pv_result_size = pv_index_res.size();
						if (number_of_model_points != pv_result_size)
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
							throw new GMXBException(message);	
						}
					}
					map_index_shock[shock_num] = pv_index_res; 
				}
				
				for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
				{

					vector<vector<double>> pv_fund_res;
					int shock_num = get_shock_number(scenario.get_fund_shock_array(), fund_shock, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
					stringstream out1;
					string shock_num_str;
					out1 << shock_num;
					shock_num_str = out1.str();
					
					string fund_shock_file_name;
					if(if_multi_esg == false)
					{   
						fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_" 
												         + scen_num_str + "_"  + shock_num_str + "_" ;
					}
					else
					{
						fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_" 
												         + scen_num_str +  "_" + bucket_id_str +  "_"  + shock_num_str + "_" ;
					}
					this->get_pvs_by_policy(fund_shock_file_name, pv_fund_res, scen_pass_num, shock_num, calib_a, calib_sigma); 
					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					
					/* To DEFINE check=true; */
					nb_of_pv_fund_per_worker = pv_fund_res.size();
					if(summary_configuration->get_scen_by_scen_indicator() == false) 
					{
					int pv_result_size = pv_fund_res.size();
					if (number_of_model_points != pv_result_size)
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
						throw new GMXBException(message);	
						}
					}
					map_fund_shock[shock_num] = pv_fund_res; 
				}

				// Manage lapse shocks
				vector<GMXB_Assumption_Table*> v_lapse_shock = scenario.get_lapse_mult();
				if (v_lapse_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_lapse_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_lapse_res;
						GMXB_Assumption_Table *tab = v_lapse_shock[l_shock];
						 std::ostringstream oss;
						 oss << scen_pass_num;
						string lapse_prefix_file_name;
						if(if_multi_esg == false)
						{
							lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_";
						}
						else
						{
							lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str() + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_";
						}
						this->get_pvs_by_policy(lapse_prefix_file_name, pv_lapse_res, scen_pass_num, 0, calib_a, calib_sigma);
						lapse_shock[tab->get_sheet_name()] = pv_lapse_res;
					}
					map_scen_pass_num_lapse[scen_pass_num] = lapse_shock;
				}

				// Manage mortality shocks
				vector<GMXB_Assumption_Table*> v_mortality_shock = scenario.get_mortality_mult();
				if (v_mortality_shock.size())
				{
					for (unsigned int m_shock = 0; m_shock < v_mortality_shock.size(); m_shock++)
					{
						vector<vector<double>> pv_mort_res;
						GMXB_Assumption_Table *tab = v_mortality_shock[m_shock];
	    				 std::ostringstream oss;
						 oss << scen_pass_num;
						 string mort_prefix_file_name;
						if(if_multi_esg == false)
						{
							mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_";
						}
						else
						{
							mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str()  + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_";
						}
						this->get_pvs_by_policy(mort_prefix_file_name, pv_mort_res, scen_pass_num, 0, calib_a, calib_sigma);
						mortality_shock[tab->get_sheet_name()] = pv_mort_res;
					}
					if(if_multi_esg == false)
						map_scen_pass_num_mort[scen_pass_num] = mortality_shock;
					else
						map_scen_pass_num_mort[bucket_id] = mortality_shock;
				}

				// Manage longevity shocks
				if (scenario.get_load_longevity_table())
				{
					vector<vector<double>> pv_longevity_res;
    				std::ostringstream oss;
					oss << scen_pass_num;
					string longevity_prefix_file_name;
					if(if_multi_esg == false)
					{
      					longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str() + "_longevity_";
					}
					else
					{
      					longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str()   + "_" + bucket_id_str + "_longevity_";
					}

					this->get_pvs_by_policy(longevity_prefix_file_name, pv_longevity_res, scen_pass_num, 0, calib_a, calib_sigma);
					
					if(if_multi_esg == false)
                        map_scen_pass_num_longevity[scen_pass_num] = pv_longevity_res;  //! 5
					else
						map_scen_pass_num_longevity[bucket_id] = pv_longevity_res;
				}

				// Manage paidup shocks
				vector<GMXB_Assumption_Table*> v_paidup_shock = scenario.get_paidup_mult();
				if (v_paidup_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_paidup_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_paidup_res;
						GMXB_Assumption_Table *tab = v_paidup_shock[l_shock];
						 std::ostringstream oss;
						 oss << scen_pass_num;
						string paidup_prefix_file_name;
						if(if_multi_esg == false)
						{
								paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_";
						}
						else
						{
								paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str()   + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_";
						}
						this->get_pvs_by_policy(paidup_prefix_file_name, pv_paidup_res, scen_pass_num, 0, calib_a, calib_sigma);
						paidup_shock[tab->get_sheet_name()] = pv_paidup_res;
					}
					if(if_multi_esg == false)
						map_scen_pass_num_paidup[scen_pass_num] = paidup_shock;  //! 4
					else
						map_scen_pass_num_paidup[bucket_id] = paidup_shock;
				}
				
				// Manage election shocks
				vector<GMXB_Assumption_Table*> v_election_shock = scenario.get_election_mult();
				
				if (v_election_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_election_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_election_res;
						GMXB_Assumption_Table *tab = v_election_shock[l_shock];
						std::ostringstream oss;
						oss << scen_pass_num;
					
						string election_prefix_file_name;
						if(if_multi_esg == false)
							election_prefix_file_name = product.get_result_file_path() + product.get_run_name()
							+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_";
						else
						    election_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str()   + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_";
					
						this->get_pvs_by_policy(election_prefix_file_name, pv_election_res, scen_pass_num, 0, calib_a, calib_sigma);
						election_shock[tab->get_sheet_name()] = pv_election_res;
					}
					
					if(if_multi_esg == false)
						map_scen_pass_num_election[scen_pass_num] = election_shock;
					else
						map_scen_pass_num_election[bucket_id] = election_shock;
				}
				
				vector<double> calib_parameters;
				calib_parameters.push_back(calib_a);
				calib_parameters.push_back(calib_sigma);
				if(if_multi_esg == false)
					map_calib_parameters[scen_pass_num] = calib_parameters;  //! 3
				else
					map_calib_parameters[bucket_id] = calib_parameters;

				if(if_multi_esg == false)
					map_index_scen_pass_num[scen_pass_num] = map_index_shock; //! 2
				else
					map_index_scen_pass_num[bucket_id] = map_index_shock;

				if(if_multi_esg == false)
					map_fund_scen_pass_num[scen_pass_num] = map_fund_shock;  //! 1
				else
					map_fund_scen_pass_num[bucket_id] = map_fund_shock;

			}
			map_index_results[product.get_prod_code_key()] = map_index_scen_pass_num;
			map_fund_results[product.get_prod_code_key()] = map_fund_scen_pass_num;
			map_lapse_results[product.get_prod_code_key()] = map_scen_pass_num_lapse;
			map_mortality_results[product.get_prod_code_key()] = map_scen_pass_num_mort;
			map_longevity_results[product.get_prod_code_key()] = map_scen_pass_num_longevity;
			map_paidup_results[product.get_prod_code_key()] = map_scen_pass_num_paidup;
			map_election_results[product.get_prod_code_key()] = map_scen_pass_num_election;

			if (check == true) 
				this->nb_total_polices += nb_of_pv_index_per_worker;
			else
				this->nb_total_polices += nb_of_pv_fund_per_worker;
		}
		// write the output files
		if((this->summary_configuration->get_ehc_format_indicator()) && (this->summary_configuration->get_policy_by_policy_indicator()))
		{
		  	char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization : impossible in detail and ehc at the same time" );
			throw new GMXBException(message);	
		}
		
		if (this->summary_configuration->get_ehc_format_indicator())
				this->write_ehc_summary_file(mps_per_product, map_index_results, map_fund_results, map_calib_parameters);
		if(this->summary_configuration->get_policy_by_policy_indicator())
				this->write_pol_by_pol(mps_per_product, map_index_results, map_fund_results, map_lapse_results,
				map_mortality_results, map_longevity_results,map_paidup_results, map_election_results);
 

		// delete all the model points pointers
		for (map<string, vector<Model_Point*>>::const_iterator iter_mp = mps_per_product.begin();
			iter_mp != mps_per_product.end(); ++iter_mp)
		{
			vector<Model_Point*> model_points = iter_mp->second;
			int number_mps = model_points.size();
			for (int i = 0; i < number_mps; i++)
			{
				Model_Point *mp = model_points[i];
				delete mp;
			}
		}
	}


	void perform_detailed_summary_esg()
	{
		
		map<string, map<int, map<int,map<int, vector<vector<double>>>>>> map_index_results;
		map<string, map<int, map<int,map<int, vector<vector<double>>>>>> map_fund_results;
		map<int, map<int,vector<double>>> map_calib_parameters;
		
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_lapse_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_mortality_results;
		map<string, map<int, map<int, vector<vector<double>>>>> map_longevity_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_paidup_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_election_results;
		
		map<string, vector<Model_Point*>> mps_per_product;
		Inforce_Manager* inforce_mgr = NULL;
		this->nb_total_polices = 0;
		int nb_of_pv_index_per_worker = 0;
		int nb_of_pv_fund_per_worker = 0;
		bool check = false;

		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			vector<Model_Point*> list_of_mps;
			
			Product_configuration product = products[i];
			inforce_mgr->get_list_of_model_points_per_product(product.get_prod_code_key(), 
													list_of_mps, product, product.get_scenario_list()[0]);
			
			int number_of_model_points = list_of_mps.size();

			mps_per_product[product.get_prod_code_key()] = list_of_mps;
			
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();

			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				int bucket_id     = scenario.get_bucket_id();

				string scen_num_str = Converter::convert_to_str(scen_pass_num);
				string bucket_id_str = Converter::convert_to_str(bucket_id);

				int index_shock_size = scenario.get_index_shock_array().size();
				int fund_shock_size = scenario.get_fund_shock_array().size();
				int number_of_funds = 0;
				if (fund_shock_size > 0)
					number_of_funds = scenario.get_fund_shock_array()[0].size();


				double calib_a;
				double calib_sigma;

				for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
				{
					vector<vector<double>> pv_index_res;
					int shock_num = get_shock_number(scenario.get_index_shock_array(), index_shock, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					string shock_num_str = Converter::convert_to_str(shock_num);
					
					string index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_" 
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					
					this->get_pvs_by_policy_esg(index_shock_file_name, pv_index_res,  calib_a, calib_sigma); 

					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					check = true;
					nb_of_pv_index_per_worker = pv_index_res.size();
					if(summary_configuration->get_scen_by_scen_indicator() == false) //! scen_by_scen no check for the moment
					{
						int pv_result_size = pv_index_res.size();
						if (number_of_model_points != pv_result_size)
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
							throw new GMXBException(message);	
						}
					}

					map_index_results[product.get_prod_code_key()][scen_pass_num][bucket_id][shock_num] = pv_index_res;
				}
				
				for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
				{

					vector<vector<double>> pv_fund_res;
					int shock_num = get_shock_number(scenario.get_fund_shock_array(), fund_shock, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
					string shock_num_str = Converter::convert_to_str(shock_num);
					
					string fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_" 
												         + scen_num_str +  "_" + bucket_id_str +  "_"  + shock_num_str + "_" ;
					
					this->get_pvs_by_policy_esg(fund_shock_file_name, pv_fund_res, calib_a, calib_sigma); 
					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					
					nb_of_pv_fund_per_worker = pv_fund_res.size();
					if(summary_configuration->get_scen_by_scen_indicator() == false) 
					{
					int pv_result_size = pv_fund_res.size();
					if (number_of_model_points != pv_result_size)
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
						throw new GMXBException(message);	
						}
					}
					map_fund_results[product.get_prod_code_key()][scen_pass_num][bucket_id][shock_num] = pv_fund_res;
				}

				// Manage lapse shocks
				vector<GMXB_Assumption_Table*> v_lapse_shock = scenario.get_lapse_mult();
				if (v_lapse_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_lapse_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_lapse_res;
						GMXB_Assumption_Table *tab = v_lapse_shock[l_shock];

						string lapse_shock_str = Converter::convert_to_str(scen_pass_num);
						string lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +lapse_shock_str + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_";
						
						this->get_pvs_by_policy_esg(lapse_prefix_file_name, pv_lapse_res, calib_a, calib_sigma);
						map_lapse_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_lapse_res;
					}
				}

				// Manage mortality shocks
				vector<GMXB_Assumption_Table*> v_mortality_shock = scenario.get_mortality_mult();
				if (v_mortality_shock.size())
				{
					for (unsigned int m_shock = 0; m_shock < v_mortality_shock.size(); m_shock++)
					{
						vector<vector<double>> pv_mort_res;
						GMXB_Assumption_Table *tab = v_mortality_shock[m_shock];
						 string mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" + scen_num_str + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_";
						
						this->get_pvs_by_policy_esg(mort_prefix_file_name, pv_mort_res, calib_a, calib_sigma);
						map_mortality_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_mort_res;
					}
				}

				// Manage longevity shocks
				if (scenario.get_load_longevity_table())
				{
					vector<vector<double>> pv_longevity_res;
					string longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" + scen_num_str  + "_" + bucket_id_str + "_longevity_";

					this->get_pvs_by_policy_esg(longevity_prefix_file_name, pv_longevity_res, calib_a, calib_sigma);
					map_longevity_results[product.get_prod_code_key()][scen_pass_num][bucket_id] = pv_longevity_res;
				}

				// Manage paidup shocks
				vector<GMXB_Assumption_Table*> v_paidup_shock = scenario.get_paidup_mult();
				if (v_paidup_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_paidup_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_paidup_res;
						GMXB_Assumption_Table *tab = v_paidup_shock[l_shock];
						string paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" + scen_num_str + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_";
						
						this->get_pvs_by_policy_esg(paidup_prefix_file_name, pv_paidup_res, calib_a, calib_sigma);
						map_paidup_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_paidup_res;;
					}
				}
				
				// Manage election shocks
				vector<GMXB_Assumption_Table*> v_election_shock = scenario.get_election_mult();
				if (v_election_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_election_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_election_res;
						GMXB_Assumption_Table *tab = v_election_shock[l_shock];
						string election_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" + scen_num_str  + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_";
		
						this->get_pvs_by_policy_esg(election_prefix_file_name, pv_election_res, calib_a, calib_sigma);
						map_election_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_election_res;
					}
				}
				
				vector<double> calib_parameters;
				calib_parameters.push_back(calib_a);
				calib_parameters.push_back(calib_sigma);
				map_calib_parameters[scen_pass_num][bucket_id] = calib_parameters;
			}
			
			if (check == true) 
				this->nb_total_polices += nb_of_pv_index_per_worker;
			else
				this->nb_total_polices += nb_of_pv_fund_per_worker;
		}

		//// write the output files
		if((this->summary_configuration->get_ehc_format_indicator()) && (this->summary_configuration->get_policy_by_policy_indicator()))
		{
		  char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization : impossible in detail and ehc at the same time" );
			throw new GMXBException(message);	
		}
		
		if (this->summary_configuration->get_ehc_format_indicator())
				this->write_ehc_summary_file_esg(mps_per_product, map_index_results, map_fund_results, map_calib_parameters);

		if(this->summary_configuration->get_policy_by_policy_indicator())
				this->write_pol_by_pol_esg(mps_per_product, map_index_results, map_fund_results, map_lapse_results,
				map_mortality_results, map_longevity_results,map_paidup_results, map_election_results);
 

		// delete all the model points pointers
		for (map<string, vector<Model_Point*>>::const_iterator iter_mp = mps_per_product.begin();
			iter_mp != mps_per_product.end(); ++iter_mp)
		{
			vector<Model_Point*> model_points = iter_mp->second;
			int number_mps = model_points.size();
			for (int i = 0; i < number_mps; i++)
			{
				Model_Point *mp = model_points[i];
				delete mp;
			}
		}
		
	}


	void perform_scen_summary()
	{
		map<string, map<int,map<int, vector<vector<double>>>>> map_index_results;
		map<string, map<int,map<int, vector<vector<double>>>>> map_fund_results;
		map<string, vector<Model_Point*>> mps_per_product;
		map<int,vector<double>> map_calib_parameters;
		map<string, map<int,map<string, vector<vector<double>>>>> map_lapse_results;
		map<string, map<int,map<string, vector<vector<double>>>>> map_mortality_results;
		map<string, map<int,vector<vector<double>>>> map_longevity_results;
		map<string, map<int,map<string, vector<vector<double>>>>> map_paidup_results;

		Inforce_Manager* inforce_mgr = NULL;
	
		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			vector<Model_Point*> list_of_mps;
			
			Product_configuration product = products[i];

			inforce_mgr->get_list_of_model_points_per_product(product.get_prod_code_key(), 
													list_of_mps, product, product.get_scenario_list()[0]);
			
			int number_of_model_points = list_of_mps.size();

			mps_per_product[product.get_prod_code_key()] = list_of_mps;
			
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();
			map<int,map<int, vector<vector<double>>>> map_index_scen_pass_num;
			map<int,map<int, vector<vector<double>>>> map_fund_scen_pass_num;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_lapse;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_mort;
			map<int, vector<vector<double>>> map_scen_pass_num_longevity;
			map<int,map<string, vector<vector<double>>>> map_scen_pass_num_paidup;

			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				string scen_num_str;
				stringstream out;
				out << scen_pass_num;
				scen_num_str  = out.str();

				string bucket_id_str;
				int bucket_id = scenario.get_bucket_id();
				if(if_multi_esg)
				{
					stringstream out2;
					out2 << bucket_id;
					bucket_id_str = out2.str();
				}
				int index_shock_size = scenario.get_index_shock_array().size();
				int fund_shock_size = scenario.get_fund_shock_array().size();
				int number_of_funds = 0;
				if (fund_shock_size > 0)
					number_of_funds = scenario.get_fund_shock_array()[0].size();

				map<int, vector<vector<double>>> map_index_shock;
				map<int, vector<vector<double>>> map_fund_shock;
				map<string, vector<vector<double>>> lapse_shock;
				map<string, vector<vector<double>>> mortality_shock;
				map<string, vector<vector<double>>> paidup_shock;

				double calib_a;
				double calib_sigma;

				for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
				{
					vector<vector<double>> pv_index_res;
					int shock_num = get_shock_number(scenario.get_index_shock_array(), index_shock, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					stringstream out1;
					string shock_num_str;
					out1 << shock_num;
					shock_num_str = out1.str();
					
					string index_shock_file_name;
					//_scen_by_scen_
					if(if_multi_esg == false)
					{
							index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_scen_by_scen_" 
												   + scen_num_str + "_"  + shock_num_str + "_" ;
					}
					else
					{
							index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_scen_by_scen_" 
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					}
			
					this->get_pvs_by_scen(index_shock_file_name, pv_index_res, scen_pass_num, shock_num, calib_a, calib_sigma); 

					map_index_shock[shock_num] = pv_index_res; 
				}
				
				for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
				{

					vector<vector<double>> pv_fund_res;
					int shock_num = get_shock_number(scenario.get_fund_shock_array(), fund_shock, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
					stringstream out1;
					string shock_num_str;
					out1 << shock_num;
					shock_num_str = out1.str();
					
					string fund_shock_file_name;
					if(if_multi_esg == false)
					{   
							fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_scen_by_scen_" 
												         + scen_num_str + "_"  + shock_num_str + "_" ;
					}
					else
					{
							fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_scen_by_scen_" 
												         + scen_num_str +  "_" + bucket_id_str +  "_"  + shock_num_str + "_" ;
					}
					this->get_pvs_by_scen(fund_shock_file_name, pv_fund_res, scen_pass_num, shock_num, calib_a, calib_sigma); 

					map_fund_shock[shock_num] = pv_fund_res; 
				}

				// Manage lapse shocks
				vector<GMXB_Assumption_Table*> v_lapse_shock = scenario.get_lapse_mult();
				if (v_lapse_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_lapse_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_lapse_res;
						GMXB_Assumption_Table *tab = v_lapse_shock[l_shock];
						 std::ostringstream oss;
						 oss << scen_pass_num;
						string lapse_prefix_file_name;
						if(if_multi_esg == false)
						{
								lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name()
										+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_scen_by_scen_" ;
						}
						else
						{
								lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str() + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_scen_by_scen_";
						}
						this->get_pvs_by_scen(lapse_prefix_file_name, pv_lapse_res, scen_pass_num, 0, calib_a, calib_sigma);
						lapse_shock[tab->get_sheet_name()] = pv_lapse_res;
					}
					map_scen_pass_num_lapse[scen_pass_num] = lapse_shock;
				}

				// Manage mortality shocks
				vector<GMXB_Assumption_Table*> v_mortality_shock = scenario.get_mortality_mult();
				if (v_mortality_shock.size())
				{
					for (unsigned int m_shock = 0; m_shock < v_mortality_shock.size(); m_shock++)
					{
						vector<vector<double>> pv_mort_res;
						GMXB_Assumption_Table *tab = v_mortality_shock[m_shock];
	    				 std::ostringstream oss;
						 oss << scen_pass_num;
						 string mort_prefix_file_name;
						if(if_multi_esg == false)
						{
								mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_scen_by_scen_";
						}
						else
						{
								mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str()  + "_" + bucket_id_str + "_" + tab->get_sheet_name() + "_scen_by_scen_";
						}
						this->get_pvs_by_scen(mort_prefix_file_name, pv_mort_res, scen_pass_num, 0, calib_a, calib_sigma);
						mortality_shock[tab->get_sheet_name()] = pv_mort_res;
					}
					if(if_multi_esg == false)
						map_scen_pass_num_mort[scen_pass_num] = mortality_shock;
					else
						map_scen_pass_num_mort[bucket_id] = mortality_shock;
				}

				// Manage longevity shocks
				if (scenario.get_load_longevity_table())
				{
					vector<vector<double>> pv_longevity_res;
    				std::ostringstream oss;
					oss << scen_pass_num;
					string longevity_prefix_file_name;
					if(if_multi_esg == false)
					{
							longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str() + "_longevity_scen_by_scen_";
					}
					else
					{
							longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name()
								+ "_" +oss.str()   + "_" + bucket_id_str + "_longevity_scen_by_scen_";
					}

					this->get_pvs_by_scen(longevity_prefix_file_name, pv_longevity_res, scen_pass_num, 0, calib_a, calib_sigma);
					
					if(if_multi_esg == false)
                        map_scen_pass_num_longevity[scen_pass_num] = pv_longevity_res;  //! 5
					else
						map_scen_pass_num_longevity[bucket_id] = pv_longevity_res;
				}

				// Manage paidup shocks
				vector<GMXB_Assumption_Table*> v_paidup_shock = scenario.get_paidup_mult();
				if (v_paidup_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_paidup_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_paidup_res;
						GMXB_Assumption_Table *tab = v_paidup_shock[l_shock];
						 std::ostringstream oss;
						 oss << scen_pass_num;
						string paidup_prefix_file_name;
						if(if_multi_esg == false)
						{
								paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str() + "_" + tab->get_sheet_name() + "_scen_by_scen_";
						}
						else
						{
								paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()
									+ "_" +oss.str()   + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_scen_by_scen_";
						}
						this->get_pvs_by_scen(paidup_prefix_file_name, pv_paidup_res, scen_pass_num, 0, calib_a, calib_sigma);
						paidup_shock[tab->get_sheet_name()] = pv_paidup_res;
					}
					if(if_multi_esg == false)
						map_scen_pass_num_paidup[scen_pass_num] = paidup_shock;  //! 4
					else
						map_scen_pass_num_paidup[bucket_id] = paidup_shock;
				}
				vector<double> calib_parameters;
				calib_parameters.push_back(calib_a);
				calib_parameters.push_back(calib_sigma);
				if(if_multi_esg == false)
					map_calib_parameters[scen_pass_num] = calib_parameters;  //! 3
				else
					map_calib_parameters[bucket_id] = calib_parameters;

				if(if_multi_esg == false)
					map_index_scen_pass_num[scen_pass_num] = map_index_shock; //! 2
				else
					map_index_scen_pass_num[bucket_id] = map_index_shock;

				if(if_multi_esg == false)
					map_fund_scen_pass_num[scen_pass_num] = map_fund_shock;  //! 1
				else
					map_fund_scen_pass_num[bucket_id] = map_fund_shock;

			}
			map_index_results[product.get_prod_code_key()] = map_index_scen_pass_num;
			map_fund_results[product.get_prod_code_key()] = map_fund_scen_pass_num;
			map_lapse_results[product.get_prod_code_key()] = map_scen_pass_num_lapse;
			map_mortality_results[product.get_prod_code_key()] = map_scen_pass_num_mort;
			map_longevity_results[product.get_prod_code_key()] = map_scen_pass_num_longevity;
			map_paidup_results[product.get_prod_code_key()] = map_scen_pass_num_paidup;

		}
		//! TODO add more condition check! 
		// write the output files
		if((this->summary_configuration->get_ehc_format_indicator()) && (this->summary_configuration->get_policy_by_policy_indicator()))
		{
		  	char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization : impossible in detail and ehc at the same time" );
			throw new GMXBException(message);	
		}
		
		if(this->summary_configuration->get_scen_by_scen_indicator())
			this->write_scen_by_scen(map_index_results,
									 map_fund_results,
									 map_lapse_results,
									 map_mortality_results,
									 map_longevity_results,
									 map_paidup_results);


		////! ?????
		// delete all the model points pointers
		for (map<string, vector<Model_Point*>>::const_iterator iter_mp = mps_per_product.begin();
			iter_mp != mps_per_product.end(); ++iter_mp)
		{
			vector<Model_Point*> model_points = iter_mp->second;
			int number_mps = model_points.size();
			for (int i = 0; i < number_mps; i++)
			{
				Model_Point *mp = model_points[i];
				delete mp;
			}
		}
	}
	void perform_scen_summary_esg()
	{
		
		map<string, map<int, map<int,map<int, vector<vector<double>>>>>> map_index_results;
		map<string, map<int, map<int,map<int, vector<vector<double>>>>>> map_fund_results;
		map<int, map<int,vector<double>>> map_calib_parameters;
		
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_lapse_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_mortality_results;
		map<string, map<int, map<int, vector<vector<double>>>>> map_longevity_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_paidup_results;
		map<string, map<int, map<int,map<string, vector<vector<double>>>>>> map_election_results;
		
		map<string, vector<Model_Point*>> mps_per_product;
		Inforce_Manager* inforce_mgr = NULL;
		
		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			vector<Model_Point*> list_of_mps;
			
			Product_configuration product = products[i];
			inforce_mgr->get_list_of_model_points_per_product(product.get_prod_code_key(), 
													list_of_mps, product, product.get_scenario_list()[0]);
			
			int number_of_model_points = list_of_mps.size();

			mps_per_product[product.get_prod_code_key()] = list_of_mps;
			
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();

			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				int bucket_id     = scenario.get_bucket_id();

				string scen_num_str = Converter::convert_to_str(scen_pass_num);
				string bucket_id_str = Converter::convert_to_str(bucket_id);

				int index_shock_size = scenario.get_index_shock_array().size();
				int fund_shock_size = scenario.get_fund_shock_array().size();
				int number_of_funds = 0;
				if (fund_shock_size > 0)
					number_of_funds = scenario.get_fund_shock_array()[0].size();


				double calib_a;
				double calib_sigma;

				for (int index_shock = 0; index_shock < index_shock_size; index_shock++)
				{
					vector<vector<double>> pv_index_res;
					int shock_num = get_shock_number(scenario.get_index_shock_array(), index_shock, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					string shock_num_str = Converter::convert_to_str(shock_num);
					
					string index_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_index_scen_by_scen_"
												   + scen_num_str + "_" + bucket_id_str + "_"  + shock_num_str + "_" ;
					
					cout << index_shock_file_name << endl;
					this->get_pvs_by_scen_esg(index_shock_file_name, pv_index_res,  calib_a, calib_sigma); 

					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					if(summary_configuration->get_scen_by_scen_indicator() == false) //! scen_by_scen no check for the moment
					{
						int pv_result_size = pv_index_res.size();
						if (number_of_model_points != pv_result_size)
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
							throw new GMXBException(message);	
						}
					}

					map_index_results[product.get_prod_code_key()][scen_pass_num][bucket_id][shock_num] = pv_index_res;
				}
				
				for (int fund_shock = 0; fund_shock < fund_shock_size; fund_shock++)
				{

					vector<vector<double>> pv_fund_res;
					int shock_num = get_shock_number(scenario.get_fund_shock_array(), fund_shock, MAX_DELTA_SHOCKS_PER_INDEX, number_of_funds);
					string shock_num_str = Converter::convert_to_str(shock_num);
					
					string fund_shock_file_name = product.get_result_file_path() + product.get_run_name() + "_fund_scen_by_scen_" 
												         + scen_num_str +  "_" + bucket_id_str +  "_"  + shock_num_str + "_" ;
					
					this->get_pvs_by_scen_esg(fund_shock_file_name, pv_fund_res, calib_a, calib_sigma); 
					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					if(summary_configuration->get_scen_by_scen_indicator() == false) 
					{
					int pv_result_size = pv_fund_res.size();
					if (number_of_model_points != pv_result_size)
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
						throw new GMXBException(message);	
						}
					}
					map_fund_results[product.get_prod_code_key()][scen_pass_num][bucket_id][shock_num] = pv_fund_res;
				}

				// Manage lapse shocks
				vector<GMXB_Assumption_Table*> v_lapse_shock = scenario.get_lapse_mult();
				if (v_lapse_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_lapse_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_lapse_res;
						GMXB_Assumption_Table *tab = v_lapse_shock[l_shock];

						string lapse_prefix_file_name = product.get_result_file_path() + product.get_run_name() + "_" + tab->get_sheet_name() + "_scen_by_scen_"
								+scen_num_str + "_" + bucket_id_str + "_";
						
						this->get_pvs_by_scen_esg(lapse_prefix_file_name, pv_lapse_res, calib_a, calib_sigma);
						map_lapse_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_lapse_res;
					}
				}

				// Manage mortality shocks
				vector<GMXB_Assumption_Table*> v_mortality_shock = scenario.get_mortality_mult();
				if (v_mortality_shock.size())
				{
					for (unsigned int m_shock = 0; m_shock < v_mortality_shock.size(); m_shock++)
					{
						vector<vector<double>> pv_mort_res;
						GMXB_Assumption_Table *tab = v_mortality_shock[m_shock];
						 string mort_prefix_file_name = product.get_result_file_path() + product.get_run_name()  + "_" + tab->get_sheet_name() + "_scen_by_scen_"
								+ scen_num_str + "_" + bucket_id_str+ "_";
						
						this->get_pvs_by_scen_esg(mort_prefix_file_name, pv_mort_res, calib_a, calib_sigma);
						map_mortality_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_mort_res;
					}
				}

				// Manage longevity shocks
				if (scenario.get_load_longevity_table())
				{
					vector<vector<double>> pv_longevity_res;
					string longevity_prefix_file_name = product.get_result_file_path() + product.get_run_name() + "_longevity_scen_by_scen_"
								+ scen_num_str  + "_" + bucket_id_str + "_";

					this->get_pvs_by_scen_esg(longevity_prefix_file_name, pv_longevity_res, calib_a, calib_sigma);
					map_longevity_results[product.get_prod_code_key()][scen_pass_num][bucket_id] = pv_longevity_res;
				}

				// Manage paidup shocks
				vector<GMXB_Assumption_Table*> v_paidup_shock = scenario.get_paidup_mult();
				if (v_paidup_shock.size())
				{
					for (unsigned int l_shock = 0; l_shock < v_paidup_shock.size(); l_shock++)
					{
						vector<vector<double>> pv_paidup_res;
						GMXB_Assumption_Table *tab = v_paidup_shock[l_shock];
						string paidup_prefix_file_name = product.get_result_file_path() + product.get_run_name()  + "_" + tab->get_sheet_name() + "_scen_by_scen_"
									+ scen_num_str + "_" + bucket_id_str + "_";
						
						this->get_pvs_by_scen_esg(paidup_prefix_file_name, pv_paidup_res, calib_a, calib_sigma);
						map_paidup_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_paidup_res;;
					}
				}

				//! no intermediate fille available ...
				//// Manage election shocks
				//vector<GMXB_Assumption_Table*> v_election_shock = scenario.get_election_mult();
				//if (v_election_shock.size())
				//{
				//	for (unsigned int l_shock = 0; l_shock < v_election_shock.size(); l_shock++)
				//	{
				//		vector<vector<double>> pv_election_res;
				//		GMXB_Assumption_Table *tab = v_election_shock[l_shock];
				//		string election_prefix_file_name = product.get_result_file_path() + product.get_run_name() + "_??????_"
				//			  + scen_num_str  + "_" + bucket_id_str  + "_" + tab->get_sheet_name() + "_";
		
				//		this->get_pvs_by_policy_esg(election_prefix_file_name, pv_election_res, calib_a, calib_sigma);
				//		map_election_results[product.get_prod_code_key()][scen_pass_num][bucket_id][tab->get_sheet_name()] = pv_election_res;
				//	}
				//}
				
				vector<double> calib_parameters;
				calib_parameters.push_back(calib_a);
				calib_parameters.push_back(calib_sigma);
				map_calib_parameters[scen_pass_num][bucket_id] = calib_parameters;
			}
		}

		//// write the output files
		if( this->summary_configuration->get_ehc_format_indicator() || this->summary_configuration->get_policy_by_policy_indicator() || this->summary_configuration->get_total_file_indicator())
		{
		   char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization : impossible to do other kind of summary than scen_by_scen summary." );
			throw new GMXBException(message);	
		}

		if(this->summary_configuration->get_scen_by_scen_indicator())
			this->write_scen_by_scen_esg(
				map_index_results, 
				map_fund_results,
				map_lapse_results,
				map_mortality_results, 
				map_longevity_results,
				map_paidup_results);

		// delete all the model points pointers
		for (map<string, vector<Model_Point*>>::const_iterator iter_mp = mps_per_product.begin();
			iter_mp != mps_per_product.end(); ++iter_mp)
		{
			vector<Model_Point*> model_points = iter_mp->second;
			int number_mps = model_points.size();
			for (int i = 0; i < number_mps; i++)
			{
				Model_Point *mp = model_points[i];
				delete mp;
			}
		}
		
	}
	//la inverse funtion of get_shock_number
	string get_index_shock_name(int index_shock_number)
	{
		//MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		int index_number = (index_shock_number - 1) / MAX_DELTA_SHOCKS_PER_INDEX + 1;
		int shock_size = (index_shock_number - 1) % MAX_DELTA_SHOCKS_PER_INDEX + 1;
		if ( shock_size > MAX_DELTA_SHOCKS_PER_INDEX/2)
		{
			shock_size = shock_size - (MAX_DELTA_SHOCKS_PER_INDEX+1);
		}
        char scenario_name[100];
		string scenario_name_prefix;
		string scenario_name_suffix;

		
		if (index_number <= MAXIMUM_NUMBER_OF_INDEXES)
		{
			sprintf(scenario_name,"idx%d",index_number);
		}
		else if (index_number == MAXIMUM_NUMBER_OF_INDEXES+1)
		{
			sprintf(scenario_name,"Global");
		}
		if (shock_size > 0)
		{
			sprintf(scenario_name,"%s+%d",scenario_name,shock_size);
		}
		else
		{
			sprintf(scenario_name,"%s%d",scenario_name,shock_size);
		}
		
		if (index_shock_number == MAX_DELTA_SHOCKS_PER_INDEX*(MAXIMUM_NUMBER_OF_INDEXES+1)+1)
			sprintf(scenario_name,"Global0");
			
		return string(scenario_name);
	}


   vector<vector<string>> read_file_into_vector(const char* file_address, bool keep_head_line)
   {
		ifstream file;
		file.open(file_address, ios::in);
		string row;
		vector<vector<string>> vector_of_file;

		if (file)
		{
			 while (!file.eof())
			 {
				getline(file, row);
				if (!keep_head_line)
					{
						keep_head_line = true;
					}
				else if (row.length() >0)
					{

						istringstream row_streamed;
						string element; 
						vector<string> vector_of_line;
						row_streamed.str(row);
						while (getline(row_streamed, element, ','))
							{
								vector_of_line.push_back(element);
							}
						vector_of_file.push_back(vector_of_line);
					}
				}
     	}
		file.close();
	    return vector_of_file;
	}

    void read_summary_file(map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int& mp_number_total)
	{
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
			iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int index_size_of_shock_array = index_shock_array.size();
			for(int s = 0; s < index_size_of_shock_array; s++ )
			{
				int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

				char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
				sprintf(pol_by_pol_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
					this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, shock_number);

				ifstream pol_by_pol_file(pol_by_pol_file_name);
				map_of_summary_file[scen_pass_n][shock_number]  = read_file_into_vector(pol_by_pol_file_name, true);

				mp_number_total = map_of_summary_file[scen_pass_n][shock_number].size()-1;
				pol_by_pol_file.close();
			}
		}
	}

	void read_summary_file_esg(map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int& mp_number_total)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
			for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
				iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
				int index_size_of_shock_array = index_shock_array.size();
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);

					char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
					/*sprintf(pol_by_pol_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, shock_number);*/
					sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
						this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, 1, shock_number);
					ifstream pol_by_pol_file(pol_by_pol_file_name);
					map_of_summary_file[scen_pass_n][shock_number]  = read_file_into_vector(pol_by_pol_file_name, true);

					mp_number_total = map_of_summary_file[scen_pass_n][shock_number].size()-1;
					pol_by_pol_file.close();
				}
			}
	}

	void init_greek_map(map<int, map<int, map<int, vector<double>>>>& map_of_greek, int mp_number_total, int base_no)
	{	    
      for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
			{
				vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
				int index_size_of_shock_array = index_shock_array.size();

				for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
				{
					for(int s = 0; s < index_size_of_shock_array; s++ )
					{
						int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						vector<double> v(SIZE_OF_GREEK_SUMMARY_FILE);
						map_of_greek[scen_pass_n][mp_number][shock_number] = v;
					}
				}
			}
		}
	}

	void calculate_EHC(map<int, map<int, map<int, vector<double>>>>& map_of_greek,map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int mp_number_total, int base_no)
	{
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
		{
				double Ehc;

	
 				for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
				{
					int scen_pass_n = iter_scen_pass_num->first;
					if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
					{
						vector<string> baseline_vector = map_of_summary_file[SCN_TOTAL*base_no+1][0][mp_number];
						string pCK = baseline_vector[PROD_TYPE_SUMMARISE].c_str();
        				if(pCK == "DEU_IAB01" || pCK == "DEU_IAB02" || pCK == "DEU_IDB01" || pCK == "DEU_IDB02")
        					Ehc = (atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())-atof(baseline_vector[AGG_DEP_FEE_SUMMARISE].c_str())-atof(baseline_vector[AGG_SUM_PREM_FEE_SUMMARISE].c_str()))/ atof(baseline_vector[AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_vector[GMXB_G_C_SUMMARISE].c_str()));
						else if (pCK == "DEU_IAB03" || pCK == "DEU_IAB05" || pCK == "DEU_IDB03" || pCK == "DEU_IDB05")
        					Ehc = atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_vector[AGG_EHC_CHARGES_SUMMARISE].c_str());
						else
							Ehc = (atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())-atof(baseline_vector[AGG_DEP_FEE_SUMMARISE].c_str())/*-atof(baseline_vector[AGG_SUM_PREM_FEE_SUMMARISE].c_str())*/)/ atof(baseline_vector[AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_vector[GMXB_G_C_SUMMARISE].c_str()));//Yasid
                            //  EHC = atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_vector[AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_vector[GMXB_G_C_SUMMARISE].c_str()));
						vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
						int index_size_of_shock_array = index_shock_array.size();
						for(int s = 0; s < index_size_of_shock_array; s++ )
						{
							int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							double IR_shock=get_IR_shock(scen_pass_n, base_no);
							double IR_volshock=get_IR_vol_shock(scen_pass_n, base_no);
							double EQ_volshock=get_EQ_vol_shock(scen_pass_n, base_no);
							double Bucket_year=get_bucket_shock(scen_pass_n, base_no);
							map_of_greek[scen_pass_n][mp_number][shock_number][EHC_BASELINE_GREEK_SUM] = Ehc;
							map_of_greek[scen_pass_n][mp_number][shock_number][IR_SHOCK_GREEK_SUM]=IR_shock;
							map_of_greek[scen_pass_n][mp_number][shock_number][RHO_BUCKET_SHOCK]=Bucket_year;
							map_of_greek[scen_pass_n][mp_number][shock_number][IR_VOL_SHOCK]=IR_volshock;
							map_of_greek[scen_pass_n][mp_number][shock_number][EQ_VOL_SHOCK]=EQ_volshock;
						}
					}
				}
		}

	}
	void calculate_delta_gamma(map<int, map<int, map<int, vector<double>>>>& map_of_greek,map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int mp_number_total, int base_no)
	{
		double const_shock= 1.;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
				iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;

			if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
			{
				vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
				int index_size_of_shock_array = index_shock_array.size();

				for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
				{
					//calculate OV and current EHC
					for(int s = 0; s < index_size_of_shock_array; s++ )
					{
						int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						
						vector<string> summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];

						double OV;
						double Rrc = atof(summarise_vector[GMXB_G_C_SUMMARISE].c_str()) ;
						double claims = atof(summarise_vector[AGG_CLAIMS_SUMMARISE].c_str());
						double charges=0.;
						double agg_dep_fees= atof(summarise_vector[AGG_DEP_FEE_SUMMARISE].c_str()); //yasid
						string pCK = summarise_vector[PROD_TYPE_SUMMARISE].c_str();

						if (pCK == "DEU_IAB03" || pCK == "DEU_IAB04" || pCK == "DEU_IDB03" || pCK == "DEU_IDB04"|| pCK == "DEU_IAB05" || pCK == "DEU_IDB05")
							charges = atof(summarise_vector[AGG_EHC_CHARGES_SUMMARISE].c_str());
						else
							charges = atof(summarise_vector[AGG_REAL_CHARGES_SUMMARISE].c_str());
						double Ehc = map_of_greek[scen_pass_n][mp_number][shock_number][EHC_BASELINE_GREEK_SUM];
						double EHC_current=0.;
						if (pCK == "DEU_IAB03" || pCK == "DEU_IAB04" || pCK == "DEU_IDB03" || pCK == "DEU_IDB04"|| pCK == "DEU_IAB05" || pCK == "DEU_IDB05")
						{EHC_current=claims/charges;
						OV = -(claims - Ehc * charges);}
						else
						{//EHC_current=claims/charges*RRC;
						EHC_current=(claims-agg_dep_fees)/charges*Rrc; // Yasid
						OV = -(claims - Ehc / Rrc * charges);}

						map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM] = OV;
						map_of_greek[scen_pass_n][mp_number][shock_number][EHC_CURRENT_GREEK_SUM] = EHC_current;
					}
					
					//calculate Delta

					//to do : take care of variable steps
					//to do : take care of the case where shock_number = 0
					for(int s = 2; s < index_size_of_shock_array-1; s++ )
					{
						int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						int shock_number_m = this->get_shock_number(index_shock_array, s-1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						vector<string> summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
						double shock_size = 0;
						double OV_p=0.;
						double av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
						if (index_shock_array[s][0] == MAXIMUM_NUMBER_OF_INDEXES+1)
						{
						   if ((index_shock_array[s+1][1]-index_shock_array[s][1])==(index_shock_array[s][1]-index_shock_array[s-1][1]))
						   {
							 int shock_number_p = this->get_shock_number(index_shock_array, s+1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);						 
							 shock_size = (index_shock_array[s+1][1]-index_shock_array[s-1][1]) / 100. / 2;
							 OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];	 
						   }
						   else
						   {
							 int shock_possible=2*index_shock_array[s][1]-index_shock_array[s-1][1];
							 int index_p=search_shock_index(index_shock_array,shock_possible,MAXIMUM_NUMBER_OF_INDEXES);
							 int index_m=-1;
							 
							 if ((index_p==-1)||(abs(index_shock_array[s+1][1]-index_shock_array[s][1])<abs(index_shock_array[s][1]-index_shock_array[s-1][1])))
							 {
							   shock_possible=2*index_shock_array[s][1]-index_shock_array[s+1][1];
							   index_m=search_shock_index(index_shock_array,shock_possible,MAXIMUM_NUMBER_OF_INDEXES);
							 }


							 if ((index_p!=-1)&&(index_m==-1))
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, index_p, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
	 						  shock_size=(index_shock_array[s][1]-index_shock_array[s-1][1])/100.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
							 else if (index_m!=-1)
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, s+1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_number_m = this->get_shock_number(index_shock_array, index_m, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_size=(index_shock_array[s+1][1]-index_shock_array[s][1])/100.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
							 else
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_size=(index_shock_array[s][1]-index_shock_array[s-1][1])/100./2.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
						   }
							double OV_pp, OV_mm = 0.;
							double OV_m = map_of_greek[scen_pass_n][mp_number][shock_number_m][OPTION_VALUE_GREEK_SUM];
							double OV =   map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
							double delta =0;
							double gamma =0;
							if (av0 !=0){
								OV_pp = OV + (OV_p -OV)*(const_shock * ( 1 + index_shock_array[s+1][1]/100. ))/ shock_size ;
								OV_mm = OV + (OV_m -OV)*(const_shock * ( 1 + index_shock_array[s-1][1]/100. ))/ shock_size ;
								delta = (OV_p - OV_m) / ( 2. * av0 * shock_size) ;
								gamma = (OV_p + OV_m -2 * OV) / ( 2. * av0 * shock_size) ;
							}
							map_of_greek[scen_pass_n][mp_number][shock_number][DELTA_GREEK_SUM] = delta;
							map_of_greek[scen_pass_n][mp_number][shock_number][GAMMA_GREEK_SUM] = gamma;
						}
					}

					
				}
			}
		}

	}



	void init_IR_shock(vector<vector<double>>& IR_shock_list, int base_no)
	{
		const int total_shock=30;
		const int init_no=1;
		for(int IR_idx=(init_no+SCN_TOTAL*base_no);IR_idx<(SCN_TOTAL*base_no+total_shock+init_no+2);IR_idx++)
		{
			if (IsExist_Scn(index_map_shocks,IR_idx))
			{
				vector<double> temp_shoc;
				temp_shoc.push_back(get_IR_shock(IR_idx, base_no));
                temp_shoc.push_back(double(IR_idx));
				IR_shock_list.push_back(temp_shoc); 
			}
		}

		sort (IR_shock_list.begin(),IR_shock_list.end());
		vector<vector<double>>::iterator it_temp;
		it_temp=unique(IR_shock_list.begin(),IR_shock_list.end(),compare_irshoc);
        IR_shock_list.resize(it_temp - IR_shock_list.begin());

	}

	void rho_calculate(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		vector<string> summarise_vector ;
		double av0 = 0;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
					iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
			{
				
				int scen_pass_n = iter_scen_pass_num->first;
				if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
									
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						
						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						calculate_parameters_for_rho(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size,IR_shock_list,base_no);
						
						if (shock_size != 0)
						{
							
							if ((IsExist_Scn(index_map_shocks,scen_pass_n_p))&&(IsExist_Scn(index_map_shocks,scen_pass_n_m)))
							{	
								
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0)// for the regular premium contracts
									{
										av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									}
									if ((!map_of_greek[scen_pass_n_p][mp_number][shock_number].empty())&&(!map_of_greek[scen_pass_n_m][mp_number][shock_number].empty()))
									{
										double OV_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double OV_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double rho = (OV_p - OV_m) / (2 *10* shock_size*av0);
										map_of_greek[scen_pass_n][mp_number][shock_number][RHO_GREEK_SUM] = rho;
									}
								}
							}
						}
					}
				}
			}

	}

	void rhoconv_calculate(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		vector<string> summarise_vector ;
		double av0 = 0;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
					iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
					
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						//calculate_parameters_for_rho_convexity(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size);
						calculate_parameters_for_rho(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size,IR_shock_list,base_no);

						if (shock_size != 0)
						{
							if ((IsExist_Scn(index_map_shocks,scen_pass_n_p))&&(IsExist_Scn(index_map_shocks,scen_pass_n_m)))
							{
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0)// for the regular premium contracts
									{
										av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									}
									/*
									double rho_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][RHO_GREEK_SUM];
									double rho_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][RHO_GREEK_SUM];
									double rho_convexity = (rho_p - rho_m) / (2 * shock_size);
									*/
									if ((!map_of_greek[scen_pass_n_p][mp_number][shock_number].empty())&&(!map_of_greek[scen_pass_n_m][mp_number][shock_number].empty()))
									{
										double ov_p=map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double ov_m=map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double ov=map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double rho_convexity=(ov_p+ov_m-2*ov)/(100*shock_size*shock_size*av0);
										map_of_greek[scen_pass_n][mp_number][shock_number][RHOCONV_GREEK_SUM] = rho_convexity;
									}
								}
							}
						}
					}
				}
			}

	}

	void rho_bucket_calc(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
      for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
					iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;

                if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
					vector<string> summarise_vector ;
					double av0 = 0;				
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						calculate_parameters_for_bucket(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size, base_no);

						if (shock_size != 0)
						{
							if ((IsExist_Scn(index_map_shocks,scen_pass_n_p))&&(IsExist_Scn(index_map_shocks,scen_pass_n_m)))
							{						
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0)// for the regular premium contracts
									   {
										   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									   }
									double OV_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
									double OV_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
									double rho = (OV_p - OV_m) / (2 * shock_size*av0);
									map_of_greek[scen_pass_n][mp_number][shock_number][RHO_BUCKET_SUM] = rho;
								}
							}
						}
					}
				}
			}
	}
		void calculate_vega( map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{

		int vegaflg=0;
		int irvolflg=0;
		vector<string> summarise_vector ;
		double av0 = 0;
        for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
					iter_scen_pass_num != this->index_map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
		    {
				if ((scen_pass_n==(IRVOL_P_5+SCN_TOTAL*base_no))&&(irvolflg!=1))
					irvolflg++;
				if ((scen_pass_n==(IRVOL_M_5+SCN_TOTAL*base_no))&&(irvolflg!=2))
					irvolflg=irvolflg+2;
				if ((scen_pass_n==(EQVOL_P_5+SCN_TOTAL*base_no))&&(irvolflg!=1))
					vegaflg++;
				if ((scen_pass_n==(EQVOL_M_5+SCN_TOTAL*base_no))&&(irvolflg!=2))
					vegaflg=vegaflg+2;
        
			}
		}
		map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->index_map_shocks.begin(); 
		vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
		int index_size_of_shock_array = index_shock_array.size();
		// Vega
		if (vegaflg==3)
		{
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{				   
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					summarise_vector = map_of_summary_file[1][shock_number][mp_number];
					av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
					if (av0==0)// for the regular premium contracts
				   {
					   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
				   }
					double ov_volp_5=map_of_greek[(EQVOL_P_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
					double ov_volm_5=map_of_greek[(EQVOL_M_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
					double vega=(ov_volp_5-ov_volm_5)/(0.1*av0);
					map_of_greek[(1+SCN_TOTAL*base_no)][mp_number][shock_number][VEGA_GREEK_SUM] = vega;
				}
			}
		}

		if (irvolflg==3)
		{
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{
				   int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
				   summarise_vector = map_of_summary_file[1][shock_number][mp_number];
				   av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
				   if (av0==0)
				   {
					   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
				   }
				   double ov_irvolp_5=map_of_greek[(IRVOL_P_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
				   double ov_irvolm_5=map_of_greek[(IRVOL_M_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
				   double irvolshock=(ov_irvolp_5-ov_irvolm_5)/(0.1*av0);
				   map_of_greek[(1+SCN_TOTAL*base_no)][mp_number][shock_number][VOL_SHOCK_GREEK_SUM] = irvolshock;
				}
			}
		}

		vegaflg=0;
		irvolflg=0;

	}


	virtual void write_greek_summary_file()
	{
	  try
	  {
		 map<int, vector<vector<int>>> index_shocks;
		 
		 if (index_map_shocks.size()!=0)
			index_shocks = index_map_shocks;
		 else
		 {
			map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		 }
	     vector<int> baseline; //baseline for Hedging (71,71-140), Pricing (1,1-70), Pricing German(141,141-210)
		 if (IsExist_Scn(index_shocks,1))
		 {
            baseline.push_back(1);
		 }
		 if (IsExist_Scn(index_shocks,SCN_TOTAL+1))
		 {
            baseline.push_back(SCN_TOTAL+1);
		 }
		 if (IsExist_Scn(index_shocks,SCN_TOTAL*2+1))
		 {
            baseline.push_back(SCN_TOTAL*2+1);
		 }

	 	//map_of_summary_file[scen_pass_n][shock_number][line][column]
		map<int, map<int, vector<vector<string>>>> map_of_summary_file;
		int mp_number_total;
		//map_of_greek[scen_pass_n][mp_number][shock_number]
		map<int, map<int, map<int, vector<double>>>> map_of_greek;
		if (index_map_shocks.size()!=0)
			read_summary_file(map_of_summary_file,mp_number_total);
		else
			read_summary_file_esg(map_of_summary_file,mp_number_total);
		if (index_map_shocks.size()!=0)
			 for(int b_ind=0;b_ind<baseline.size();b_ind++)
			 {
				int b_no=baseline[b_ind]/SCN_TOTAL;
				init_greek_map(map_of_greek,mp_number_total,b_no); //initialize the map_of_greek
				calculate_EHC(map_of_greek, map_of_summary_file, mp_number_total,b_no); 
				calculate_delta_gamma(map_of_greek, map_of_summary_file, mp_number_total,b_no); 
				vector<vector<double>> IR_shock_list;
				init_IR_shock(IR_shock_list,b_no);
				rho_calculate(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);
				rhoconv_calculate(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);
				rho_bucket_calc(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);						
				calculate_vega(map_of_greek,map_of_summary_file,mp_number_total,b_no);
			 }
		else
			for(int b_ind=0;b_ind<baseline.size();b_ind++)
			 {
				int b_no=baseline[b_ind]/SCN_TOTAL;
				init_greek_map_esg(map_of_greek,mp_number_total,b_no); //initialize the map_of_greek
				calculate_EHC_esg(map_of_greek, map_of_summary_file, mp_number_total,b_no); 
				calculate_delta_gamma_esg(map_of_greek, map_of_summary_file, mp_number_total,b_no); 
				vector<vector<double>> IR_shock_list;
				init_IR_shock_esg(IR_shock_list,b_no);
				rho_calculate_esg(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);
				rhoconv_calculate_esg(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);
				rho_bucket_calc_esg(IR_shock_list,map_of_greek,map_of_summary_file,mp_number_total,b_no);						
				calculate_vega_esg(map_of_greek,map_of_summary_file,mp_number_total,b_no);
			 }

		char output_address[1000];
		sprintf(output_address, "%s%s.csv", this->summary_configuration->get_mso_file_directory().c_str(),this->summary_configuration->get_mso_file_name().c_str());

		ofstream file(output_address);
		vector<string> summarise_vector = map_of_summary_file[baseline[0]][0][0];
		for (int t=0;t<summarise_vector.size();t++)
			file<<summarise_vector[t]<<",";

		file<<"Scenario,Assumptions,Model Point Number,EHC Baseline,EHC,Option Value,Actuarial Shock,AV Shock,Delta,Gamma,IR-shock,Rho,RhoConv,Rho_Bucket_year,Rho_Bucket,IRvol Shock,IRVega,EQvol Shock,EQVega,";//,BusinessMix,EHC Business Mix,OV business Mix,";
		file<< endl;

		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
			int index_size_of_shock_array = index_shock_array.size();
			
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					

					summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
		            
					for (int t=0;t<summarise_vector.size();t++)
							file<<summarise_vector[t]<<",";

					file<<scen_pass_n<<",";
					
					if ((scen_pass_n<=70)&&(scen_pass_n>=1))
					{file<<"Pricing,";}
					else if ((scen_pass_n<=140)&&(scen_pass_n>=71))
                    {file<<"Hedging,";}
					else if ((scen_pass_n<=210)&&(scen_pass_n>=141))
                    {file<<"Germany flat IR aft 30yr,";}
					else
                    {file<<"Not Defined,";}

					file<<mp_number<<",";
					file<<map_of_greek[scen_pass_n][mp_number][shock_number][EHC_BASELINE_GREEK_SUM] << ",";
					file<<map_of_greek[scen_pass_n][mp_number][shock_number][EHC_CURRENT_GREEK_SUM] << ",";
					file<<map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM] << ",";
                    file<<map_of_greek[scen_pass_n][mp_number][shock_number][ACTURIAL] << ",";

					if (index_shock_array[s][1]>0)
						file<<"+"<<index_shock_array[s][1]<<",";
					else if (s==0)
						file<<"Baseline_0"<<index_shock_array[s][1]<<",";
					else
						file<<index_shock_array[s][1]<<",";
					
					for (int i = 4; i < SIZE_OF_GREEK_SUMMARY_FILE; i++)
					{
						file <<setprecision(15)<<map_of_greek[scen_pass_n][mp_number][shock_number][i] << ",";
					}
					
					file << endl;
				}
			}
		}

		//for acturial 
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			Product_configuration product = products[i];
			//if (product.get_prod_code_key()==this->prod_code_key)
			//{
			int number_of_scenarios = product.get_scenario_list().size();
			
			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				vector<GMXB_Assumption_Table*> v_lapse_shock = scenario.get_lapse_mult();
				vector<GMXB_Assumption_Table*> v_mortality_shock = scenario.get_mortality_mult();
				
				if (v_lapse_shock.size())
				{
					for (int l_shock = 0; l_shock < v_lapse_shock.size(); l_shock++)
					{
						GMXB_Assumption_Table *tab = v_lapse_shock[l_shock];
						char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
						char baseline_file_name[MAX_LENGTH_FILE_NAME];
						
						if (index_map_shocks.size()!=0)
						{
							sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, tab->get_sheet_name().c_str());
							sprintf(baseline_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 0);
						}
						else
						{
							sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, 1,tab->get_sheet_name().c_str());
							sprintf(baseline_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 1, 0);
						}
						vector<vector<string>> actu_buffer;
						vector<vector<string>> baseline_buffer;
						actu_buffer = read_file_into_vector(pol_by_pol_file_name, true);
						baseline_buffer= read_file_into_vector(baseline_file_name, true);
						mp_number_total = actu_buffer.size()-1;
						for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
						{

						  double EHC_baseline=atof(baseline_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));
						  double Ehc=atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(actu_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));;
						  double OV=EHC_baseline/(atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()))*atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str())-atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str());
	
						  for (int k=0;k<actu_buffer[mp_number].size();k++)
							  file<<actu_buffer[mp_number][k]<<",";
						  //file<<"0,0,";
						  file<<scen_pass_num<<",";

						  if ((scen_pass_num<=70)&&(scen_pass_num>=1))
					      {file<<"Pricing,";}
					      else if ((scen_pass_num<=140)&&(scen_pass_num>=71))
                          {file<<"Hedging,";}
					      else if ((scen_pass_num<=210)&&(scen_pass_num>=141))
                          {file<<"Germany flat IR aft 30yr,";}
					      else
                          {file<<"Not Defined,";}

						  file<<mp_number<<",";
						  //file<<"0,";
						  
						  file<<EHC_baseline<<",";
						  file<<Ehc<<",";
						  file<<OV<<",";
						  file<<tab->get_sheet_name()<<",";

						  for (int k=0;k<SIZE_OF_GREEK_SUMMARY_FILE-3;k++)
							  file<<"0,";
						  file<<endl;
						}

					}
				}

					if (v_mortality_shock .size())
				 {
					for (int m_shock = 0; m_shock < v_mortality_shock .size(); m_shock++)
					{
						GMXB_Assumption_Table *tab = v_mortality_shock[m_shock];
						char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
						char baseline_file_name[MAX_LENGTH_FILE_NAME];

						if (index_map_shocks.size()!=0)
						{						
							sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, tab->get_sheet_name().c_str());
							sprintf(baseline_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 0);
						}
						else
						{
							sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, 1, tab->get_sheet_name().c_str());
							sprintf(baseline_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 1, 0);
						}

						vector<vector<string>> actu_buffer;
						vector<vector<string>> baseline_buffer;
						actu_buffer = read_file_into_vector(pol_by_pol_file_name, true);
						baseline_buffer= read_file_into_vector(baseline_file_name, true);
						mp_number_total = actu_buffer.size()-1;
						for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
						{
						  double EHC_baseline=atof(baseline_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));
						  double Ehc=atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(actu_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));;
                          
						  double OV=EHC_baseline/(atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()))*atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str())-atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str());
	
						  for (int k=0;k<actu_buffer[mp_number].size();k++)
							  file<<actu_buffer[mp_number][k]<<",";
						  //file<<"0,0,";
						  file<<scen_pass_num<<",";
                          
						  if ((scen_pass_num<=70)&&(scen_pass_num>=1))
					      {file<<"Pricing,";}
					      else if ((scen_pass_num<=140)&&(scen_pass_num>=71))
                          {file<<"Hedging,";}
					      else if ((scen_pass_num<=210)&&(scen_pass_num>=141))
                          {file<<"Germany flat IR aft 30yr,";}
					      else
                          {file<<"Not Defined,";}

						  file<<mp_number<<",";
						  //file<<"0,";
						  
						  file<<EHC_baseline<<",";
						  file<<Ehc<<",";
						  file<<OV<<",";
						  file<<tab->get_sheet_name()<<",";

						  for (int k=0;k<SIZE_OF_GREEK_SUMMARY_FILE-3;k++)
							  file<<"0,";
						  file<<endl;
						}

					}
					
				}
				if (scenario.get_load_longevity_table())
				{
				
						char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
						char baseline_file_name[MAX_LENGTH_FILE_NAME];
						
						if (index_map_shocks.size()!=0)
						{
							sprintf(pol_by_pol_file_name, "%s%s_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, "longevity");
							sprintf(baseline_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 0);
						}
						else
						{
							sprintf(pol_by_pol_file_name, "%s%s_%d_%d_%s.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_num, 1, "longevity");
							sprintf(baseline_file_name, "%s%s_%d_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
								this->summary_configuration->get_policy_by_policy_file_name().c_str(), 1, 1, 0);
						}

                       
						vector<vector<string>> actu_buffer;
						vector<vector<string>> baseline_buffer;
						actu_buffer = read_file_into_vector(pol_by_pol_file_name, true);
						baseline_buffer= read_file_into_vector(baseline_file_name, true);
						mp_number_total = actu_buffer.size()-1;
						for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
						{
						  double EHC_baseline=atof(baseline_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));
						  double Ehc=atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str())/ atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(actu_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()));
                          
						  double OV=EHC_baseline/(atof(baseline_buffer[mp_number][GMXB_G_C_SUMMARISE].c_str()))*atof(actu_buffer[mp_number][AGG_REAL_CHARGES_SUMMARISE].c_str())-atof(actu_buffer[mp_number][AGG_CLAIMS_SUMMARISE].c_str());
	
						  for (int k=0;k<actu_buffer[mp_number].size();k++)
							  file<<actu_buffer[mp_number][k]<<",";
						  
						  //file<<"0,0,";
						  file<<scen_pass_num<<",";

                          if ((scen_pass_num<=70)&&(scen_pass_num>=1))
					      {file<<"Pricing,";}
					      else if ((scen_pass_num<=140)&&(scen_pass_num>=71))
                          {file<<"Hedging,";}
					      else if ((scen_pass_num<=210)&&(scen_pass_num>=141))
                          {file<<"Germany flat IR aft 30yr,";}
					      else
                          {file<<"Not Defined,";}

						  file<<mp_number<<",";
						  //file<<"0,";
						  
						  file<<EHC_baseline<<",";
						  file<<Ehc<<",";
						  file<<OV<<",";
						  file<<"longevity,";

						  for (int k=0;k<SIZE_OF_GREEK_SUMMARY_FILE-3;k++)
							  file<<"0,";
						  file<<endl;
						}
	
				}
                

			}
			
		  //}

		}

		file.close();

			
        }
		catch (exception& e)
		{
          cout<<"Error in Greek Summary"<<e.what()<<endl;
		}
	}



	void calculate_parameters_for_rho(int scen_pass_n, int& scen_pass_n_p, int& scen_pass_n_m, double& shock_size, vector<vector<double>>& IR_shock_list, int base_no)
	{
      double shock_n;

	  shock_n=get_IR_shock(scen_pass_n,base_no);
	  
	  for(int i=1;i<IR_shock_list.size()-1;i++)
	  {
		  if (shock_n==IR_shock_list[i][0])
		  {
            if ((IR_shock_list[i][0]-IR_shock_list[i-1][0])==(IR_shock_list[i+1][0]-IR_shock_list[i][0]))
			{
				scen_pass_n_p=int(IR_shock_list[i+1][1]);
				scen_pass_n_m=int(IR_shock_list[i-1][1]);
				shock_size=(IR_shock_list[i][0]-IR_shock_list[i-1][0])/10.;
			}
			else
			{
				double shock_n_p=2*IR_shock_list[i][0]-IR_shock_list[i-1][0];
				for (int j=i;j<IR_shock_list.size()-1;j++)
				{
					if (shock_n_p==IR_shock_list[j][0])
					{
                      scen_pass_n_p=int(IR_shock_list[j][1]);
					  scen_pass_n_m=int(IR_shock_list[i-1][1]);
				      shock_size=(IR_shock_list[i][0]-IR_shock_list[i-1][0])/10.;
					  break;
					}
				}
				if (shock_size==0)
				{
						double shock_n_m=2*IR_shock_list[i][0]-IR_shock_list[i+1][0];
						for (int j=i;j>0;j--)
					{
						if (shock_n_m==IR_shock_list[j][0])
						{
						  scen_pass_n_p=int(IR_shock_list[i+1][1]);
						  scen_pass_n_m=int(IR_shock_list[j][1]);
						  shock_size=(IR_shock_list[i+1][0]-IR_shock_list[i][0])/10.;
						  break;
						}
					}

				}
				//do not calculate if not symmetric 
				/*if (shock_size==0)
				{
					scen_pass_n_p=int(IR_shock_list[i+1][1]);
				    scen_pass_n_m=int(IR_shock_list[i-1][1]);
				    shock_size=(IR_shock_list[i+1][0]-IR_shock_list[i-1][0])/20.;
				}*/
				
			}
		  }
	  }

	}

	

    void calculate_parameters_for_bucket(int scen_pass_n, int& scen_pass_n_p, int& scen_pass_n_m, double& shock_size, int base_no)
	{
		 const double Size=10.;
		 
		if ((scen_pass_n)==(51+SCN_TOTAL*base_no)) {scen_pass_n_p =(51+SCN_TOTAL*base_no);scen_pass_n_m =(61+SCN_TOTAL*base_no);shock_size = Size; }
        if ((scen_pass_n)==(52+SCN_TOTAL*base_no)) {scen_pass_n_p =(52+SCN_TOTAL*base_no);scen_pass_n_m =(62+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(53+SCN_TOTAL*base_no)) {scen_pass_n_p =(53+SCN_TOTAL*base_no);scen_pass_n_m =(63+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(54+SCN_TOTAL*base_no)) {scen_pass_n_p =(54+SCN_TOTAL*base_no);scen_pass_n_m =(64+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(55+SCN_TOTAL*base_no)) {scen_pass_n_p =(55+SCN_TOTAL*base_no);scen_pass_n_m =(65+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(56+SCN_TOTAL*base_no)) {scen_pass_n_p =(56+SCN_TOTAL*base_no);scen_pass_n_m =(66+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(57+SCN_TOTAL*base_no)) {scen_pass_n_p =(57+SCN_TOTAL*base_no);scen_pass_n_m =(67+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(58+SCN_TOTAL*base_no)) {scen_pass_n_p =(58+SCN_TOTAL*base_no);scen_pass_n_m =(68+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(59+SCN_TOTAL*base_no)) {scen_pass_n_p =(59+SCN_TOTAL*base_no);scen_pass_n_m =(69+SCN_TOTAL*base_no);shock_size = Size; }
		if ((scen_pass_n)==(60+SCN_TOTAL*base_no)) {scen_pass_n_p =(60+SCN_TOTAL*base_no);scen_pass_n_m =(70+SCN_TOTAL*base_no);shock_size = Size; }
		


	}

	double get_IR_shock(int scen_pass_n, int base_no)
	{
		if (scen_pass_n==(1+SCN_TOTAL*base_no))
		{
			return 0;
		}
		if ((scen_pass_n>=2+SCN_TOTAL*base_no)&&(scen_pass_n<=(32+SCN_TOTAL*base_no)))
		{
			return (-150.+(scen_pass_n-2-SCN_TOTAL*base_no)*10.);
		}
		return IR_NOT_EXIST;


	}

	double get_bucket_shock(int scen_pass_n, int base_no)
	{
	    if ((scen_pass_n)==(51+SCN_TOTAL*base_no)) return 1;
	    if ((scen_pass_n)==(52+SCN_TOTAL*base_no)) return 3;
	    if ((scen_pass_n)==(53+SCN_TOTAL*base_no)) return 5;
		if ((scen_pass_n)==(54+SCN_TOTAL*base_no)) return 7;
		if ((scen_pass_n)==(55+SCN_TOTAL*base_no)) return 10;
		if ((scen_pass_n)==(56+SCN_TOTAL*base_no)) return 15;
		if ((scen_pass_n)==(57+SCN_TOTAL*base_no)) return 20;
		if ((scen_pass_n)==(58+SCN_TOTAL*base_no)) return 30;
		if ((scen_pass_n)==(59+SCN_TOTAL*base_no)) return 40;
		if ((scen_pass_n)==(60+SCN_TOTAL*base_no)) return 50;
		if ((scen_pass_n)==(61+SCN_TOTAL*base_no)) return 1;
		if ((scen_pass_n)==(62+SCN_TOTAL*base_no)) return 3;
		if ((scen_pass_n)==(63+SCN_TOTAL*base_no)) return 5;
		if ((scen_pass_n)==(64+SCN_TOTAL*base_no)) return 7;
		if ((scen_pass_n)==(65+SCN_TOTAL*base_no)) return 10;
		if ((scen_pass_n)==(66+SCN_TOTAL*base_no)) return 15;
		if ((scen_pass_n)==(67+SCN_TOTAL*base_no)) return 20;
		if ((scen_pass_n)==(68+SCN_TOTAL*base_no)) return 30;
		if ((scen_pass_n)==(69+SCN_TOTAL*base_no)) return 40;
		if ((scen_pass_n)==(70+SCN_TOTAL*base_no)) return 50;
		
		return IR_NOT_EXIST;
		

	}

	double get_IR_vol_shock(int scen_pass_n, int base_no)
	{
	  if (scen_pass_n==(33+SCN_TOTAL*base_no)) return 1.;
	  else if (scen_pass_n==(34+SCN_TOTAL*base_no)) return -1.;
      else if (scen_pass_n==(35+SCN_TOTAL*base_no)) return 5.;
	  else if (scen_pass_n==(36+SCN_TOTAL*base_no)) return -5.;
	  else if (scen_pass_n==(37+SCN_TOTAL*base_no)) return 125.;
	  else if (scen_pass_n==(38+SCN_TOTAL*base_no)) return 150.;
	  else if (scen_pass_n==(1+SCN_TOTAL*base_no)) return 0;
	  return -999.;

	}

	double get_EQ_vol_shock(int scen_pass_n, int base_no)
	{
	  if (scen_pass_n==(44+SCN_TOTAL*base_no)) return -95.;
	  else if (scen_pass_n==(45+SCN_TOTAL*base_no)) return 1.;
      else if (scen_pass_n==(46+SCN_TOTAL*base_no)) return 5.;
	  else if (scen_pass_n==(47+SCN_TOTAL*base_no)) return -5.;
	  else if (scen_pass_n==(48+SCN_TOTAL*base_no)) return 125.;
	  else if (scen_pass_n==(1+SCN_TOTAL*base_no)) return 0;
	  return -999.;

	}

    int search_shock_index(const vector<vector<int>>& shock_array, int shocksize, int maximum_number_of_instrument)
	{
		int p_index=0;
        if (shock_array.size())
		{
			for(p_index=0;p_index<shock_array.size();p_index++)
			{
				if((shock_array[p_index][0]==maximum_number_of_instrument+1)&&(shock_array[p_index][1]==shocksize))
					return p_index;
			}
		}

		return -1;
			
	}

	bool IsExist_Scn(const map<int, vector<vector<int>>>& index_map,int scn_for_test)
	{
		for(map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_map.begin(); 
					iter_scen_pass_num != index_map.end(); ++iter_scen_pass_num)
		{
			int scn_num=iter_scen_pass_num->first;
			if (scn_num==scn_for_test)
				return 1;
		}
		return 0;
	}
	void rho_calculate_esg(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek,map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
									
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						vector<string> summarise_vector ;
						double av0 = 0;

						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						calculate_parameters_for_rho(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size,IR_shock_list,base_no);

						if (shock_size != 0)
						{
							if ((IsExist_Scn(index_shocks,scen_pass_n_p))&&(IsExist_Scn(index_shocks,scen_pass_n_m)))
							{						
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0)// for the regular premium contracts
									{
										av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									}
									if ((!map_of_greek[scen_pass_n_p][mp_number][shock_number].empty())&&(!map_of_greek[scen_pass_n_m][mp_number][shock_number].empty()))
									{
										double OV_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double OV_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double rho = (OV_p - OV_m) / (2 *10 *shock_size* av0);
										map_of_greek[scen_pass_n][mp_number][shock_number][RHO_GREEK_SUM] = rho;
									}
								}
							}
						}
					}
				}
			}

	}

	void rhoconv_calculate_esg(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
					
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						//calculate_parameters_for_rho_convexity(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size);
						calculate_parameters_for_rho(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size,IR_shock_list,base_no);
						vector<string> summarise_vector ;
						double av0 = 0;
						if (shock_size != 0)
						{
							if ((IsExist_Scn(index_shocks,scen_pass_n_p))&&(IsExist_Scn(index_shocks,scen_pass_n_m)))
							{
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0) // for the regular premium contracts
									{
										av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									}
									/*
									double rho_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][RHO_GREEK_SUM];
									double rho_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][RHO_GREEK_SUM];
									double rho_convexity = (rho_p - rho_m) / (2 * shock_size);
									*/
									if ((!map_of_greek[scen_pass_n_p][mp_number][shock_number].empty())&&(!map_of_greek[scen_pass_n_m][mp_number][shock_number].empty()))
									{
										double ov_p=map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double ov_m=map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double ov=map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
										double rho_convexity=(ov_p+ov_m-2*ov)/(100*shock_size*shock_size*av0);
										map_of_greek[scen_pass_n][mp_number][shock_number][RHOCONV_GREEK_SUM] = rho_convexity;
									}
								}
							}
						}
					}
				}
			}

	}

	void rho_bucket_calc_esg(vector<vector<double>>& IR_shock_list, map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
      for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;

                if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();
					vector<string> summarise_vector ;
					double av0 = 0;				
					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						int scen_pass_n_p, scen_pass_n_m;
						double shock_size = 0;

						calculate_parameters_for_bucket(scen_pass_n, scen_pass_n_p, scen_pass_n_m, shock_size, base_no);

						if (shock_size != 0)
						{
							if ((IsExist_Scn(index_shocks,scen_pass_n_p))&&(IsExist_Scn(index_shocks,scen_pass_n_m)))
							{						
								for(int s = 0; s < index_size_of_shock_array; s++ )
								{
									int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
									summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
									av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
									if (av0==0)// for the regular premium contracts
									   {
										   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
									   }
									double OV_p = map_of_greek[scen_pass_n_p][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
									double OV_m = map_of_greek[scen_pass_n_m][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
									double rho = (OV_p - OV_m) / (2 * shock_size*av0);
									map_of_greek[scen_pass_n][mp_number][shock_number][RHO_BUCKET_SUM] = rho;
								}
							}
						}
					}
				}
			}
	}

		void calculate_vega_esg( map<int, map<int, map<int, vector<double>>>>& map_of_greek, map<int, map<int, vector<vector<string>>>>& map_of_summary_file, int mp_number_total, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		int vegaflg=0;
		int irvolflg=0;
		vector<string> summarise_vector ;
		double av0 = 0;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
		    {
				if ((scen_pass_n==(IRVOL_P_5+SCN_TOTAL*base_no))&&(irvolflg!=1))
					irvolflg++;
				if ((scen_pass_n==(IRVOL_M_5+SCN_TOTAL*base_no))&&(irvolflg!=2))
					irvolflg=irvolflg+2;
				if ((scen_pass_n==(EQVOL_P_5+SCN_TOTAL*base_no))&&(irvolflg!=1))
					vegaflg++;
				if ((scen_pass_n==(EQVOL_M_5+SCN_TOTAL*base_no))&&(irvolflg!=2))
					vegaflg=vegaflg+2;
        
			}
		}

		map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
		vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
		int index_size_of_shock_array = index_shock_array.size();
		// Vega
		if (vegaflg==3)
		{
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{				   
					int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
					summarise_vector = map_of_summary_file[1][shock_number][mp_number];
					av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
					if (av0==0)// for the regular premium contracts
				   {
					   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
				   }
					double ov_volp_5=map_of_greek[(EQVOL_P_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
					double ov_volm_5=map_of_greek[(EQVOL_M_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
					double vega=(ov_volp_5-ov_volm_5)/(0.1*av0);
					map_of_greek[(1+SCN_TOTAL*base_no)][mp_number][shock_number][VEGA_GREEK_SUM] = vega;
				}
			}
		}

		if (irvolflg==3)
		{
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				for(int s = 0; s < index_size_of_shock_array; s++ )
				{
				   int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
				   summarise_vector = map_of_summary_file[1][shock_number][mp_number];
				   av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
				   if (av0==0)// for the regular premium contracts
				   {
					   av0=atof(summarise_vector[SUM_PREMIUM].c_str());
				   }
				   double ov_irvolp_5=map_of_greek[(IRVOL_P_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
				   double ov_irvolm_5=map_of_greek[(IRVOL_M_5+SCN_TOTAL*base_no)][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
				   double irvolshock=(ov_irvolp_5-ov_irvolm_5)/(0.1*av0);
				   map_of_greek[(1+SCN_TOTAL*base_no)][mp_number][shock_number][VOL_SHOCK_GREEK_SUM] = irvolshock;
				}
			}
		}
		

		vegaflg=0;
		irvolflg=0;

	}


	void init_greek_map_esg(map<int, map<int, map<int, vector<double>>>>& map_of_greek, int mp_number_total, int base_no)
		{	    
			 map<int, vector<vector<int>>> index_shocks;
			 map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		  for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
					iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
			{
				int scen_pass_n = iter_scen_pass_num->first;
				if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
				{
					vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
					int index_size_of_shock_array = index_shock_array.size();

					for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
					{
						for(int s = 0; s < index_size_of_shock_array; s++ )
						{
							int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							vector<double> v(SIZE_OF_GREEK_SUMMARY_FILE);
							map_of_greek[scen_pass_n][mp_number][shock_number] = v;
						}
					}
				}
			}
		}
	void calculate_EHC_esg(map<int, map<int, map<int, vector<double>>>>& map_of_greek,map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int mp_number_total, int base_no)
	{
		try{
			map<int, vector<vector<int>>> index_shocks;
			map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
			for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
			{
				double Ehc;

	
 				for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
				iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
				{
					int scen_pass_n = iter_scen_pass_num->first;
					if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
					{
						vector<string> baseline_vector = map_of_summary_file[SCN_TOTAL*base_no+1][0][mp_number];
						string pCK = baseline_vector[PROD_TYPE_SUMMARISE].c_str();
						if(pCK == "DEU_IAB01" || pCK == "DEU_IAB02" || pCK == "DEU_IDB01" || pCK == "DEU_IDB02")
        					Ehc = (atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())-atof(baseline_vector[AGG_DEP_FEE_SUMMARISE].c_str())-atof(baseline_vector[AGG_SUM_PREM_FEE_SUMMARISE].c_str()))/ atof(baseline_vector[AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_vector[GMXB_G_C_SUMMARISE].c_str()));
						else if (pCK == "DEU_IAB03" || pCK == "DEU_IAB05" || pCK == "DEU_IDB03" || pCK == "DEU_IDB05")
        					Ehc = atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_vector[AGG_EHC_CHARGES_SUMMARISE].c_str());
						else
							Ehc = atof(baseline_vector[AGG_CLAIMS_SUMMARISE].c_str())/ atof(baseline_vector[AGG_REAL_CHARGES_SUMMARISE].c_str()) * (atof(baseline_vector[GMXB_G_C_SUMMARISE].c_str()));
						vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
						int index_size_of_shock_array = index_shock_array.size();
						for(int s = 0; s < index_size_of_shock_array; s++ )
						{
							int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							double IR_shock=get_IR_shock(scen_pass_n, base_no);
							double IR_volshock=get_IR_vol_shock(scen_pass_n, base_no);
							double EQ_volshock=get_EQ_vol_shock(scen_pass_n, base_no);
							double Bucket_year=get_bucket_shock(scen_pass_n, base_no);
							map_of_greek[scen_pass_n][mp_number][shock_number][EHC_BASELINE_GREEK_SUM] = Ehc;
							map_of_greek[scen_pass_n][mp_number][shock_number][IR_SHOCK_GREEK_SUM]=IR_shock;
							map_of_greek[scen_pass_n][mp_number][shock_number][RHO_BUCKET_SHOCK]=Bucket_year;
							map_of_greek[scen_pass_n][mp_number][shock_number][IR_VOL_SHOCK]=IR_volshock;
							map_of_greek[scen_pass_n][mp_number][shock_number][EQ_VOL_SHOCK]=EQ_volshock;
						}
					}
				}
			}
		}
		catch (exception& e)
		{
			cout<<"Error in Calculate_EHC(Germany_Summary_File_Writer: )"<<e.what()<<endl;
		}
	}

	void calculate_delta_gamma_esg(map<int, map<int, map<int, vector<double>>>>& map_of_greek,map<int, map<int, vector<vector<string>>>>& map_of_summary_file,int mp_number_total, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		double const_shock= 1.;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = index_shocks.begin(); 
				iter_scen_pass_num != index_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;

			if ((scen_pass_n<=SCN_TOTAL*(base_no+1))&&(scen_pass_n>SCN_TOTAL*(base_no)))
			{
				vector<vector<int>> index_shock_array = iter_scen_pass_num->second;
				int index_size_of_shock_array = index_shock_array.size();

				for (int mp_number = 1; mp_number <= mp_number_total; mp_number++)
				{
					//calculate OV and current EHC
					for(int s = 0; s < index_size_of_shock_array; s++ )
					{
						int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						
						vector<string> summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];

						double OV;
						double Rrc = atof(summarise_vector[GMXB_G_C_SUMMARISE].c_str()) ;
						double claims = atof(summarise_vector[AGG_CLAIMS_SUMMARISE].c_str());
						double agg_dep_fees= atof(summarise_vector[AGG_DEP_FEE_SUMMARISE].c_str()); //yasid
						double charges=0.;
						string pCK = summarise_vector[PROD_TYPE_SUMMARISE].c_str();
						if (pCK == "DEU_IAB03" || pCK == "DEU_IAB05" || pCK == "DEU_IDB03" || pCK == "DEU_IDB05")
							charges = atof(summarise_vector[AGG_EHC_CHARGES_SUMMARISE].c_str());
						else
							charges = atof(summarise_vector[AGG_REAL_CHARGES_SUMMARISE].c_str());
						double Ehc = map_of_greek[scen_pass_n][mp_number][shock_number][EHC_BASELINE_GREEK_SUM];
						double EHC_current=0.;
						if (pCK == "DEU_IAB03" || pCK == "DEU_IAB05" || pCK == "DEU_IDB03" || pCK == "DEU_IDB05")
							EHC_current=claims/charges;
						else
							//EHC_current=claims/charges*RRC;
						    EHC_current=(claims-agg_dep_fees)/charges*Rrc;//Yasid
							
						if (pCK == "DEU_IAB03" || pCK == "DEU_IAB05" || pCK == "DEU_IDB03" || pCK == "DEU_IDB05")
							OV = -(claims - Ehc * charges);
						else
							OV = -(claims - Ehc / Rrc * charges);


						map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM] = OV;
						map_of_greek[scen_pass_n][mp_number][shock_number][EHC_CURRENT_GREEK_SUM] = EHC_current;
					}
					
					//calculate Delta

					//to do : take care of variable steps
					//to do : take care of the case where shock_number = 0
					for(int s = 2; s < index_size_of_shock_array-1; s++ )
					{
						int shock_number = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						int shock_number_m = this->get_shock_number(index_shock_array, s-1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
						vector<string> summarise_vector = map_of_summary_file[scen_pass_n][shock_number][mp_number];
						double shock_size = 0;
						double OV_p=0.;
						double av0 = atof(summarise_vector[AV0_SUMMARISE].c_str());
						if (index_shock_array[s][0] == MAXIMUM_NUMBER_OF_INDEXES+1)
						{
						   if ((index_shock_array[s+1][1]-index_shock_array[s][1])==(index_shock_array[s][1]-index_shock_array[s-1][1]))
						   {
							 int shock_number_p = this->get_shock_number(index_shock_array, s+1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);						 
							 shock_size = (index_shock_array[s+1][1]-index_shock_array[s-1][1]) / 100. / 2;
							 OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];	 
						   }
						   else
						   {
							 int shock_possible=2*index_shock_array[s][1]-index_shock_array[s-1][1];
							 int index_p=search_shock_index(index_shock_array,shock_possible,MAXIMUM_NUMBER_OF_INDEXES);
							 int index_m=-1;
							 
							 if ((index_p==-1)||(abs(index_shock_array[s+1][1]-index_shock_array[s][1])<abs(index_shock_array[s][1]-index_shock_array[s-1][1])))
							 {
							   shock_possible=2*index_shock_array[s][1]-index_shock_array[s+1][1];
							   index_m=search_shock_index(index_shock_array,shock_possible,MAXIMUM_NUMBER_OF_INDEXES);
							 }


							 if ((index_p!=-1)&&(index_m==-1))
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, index_p, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
	 						  shock_size=(index_shock_array[s][1]-index_shock_array[s-1][1])/100.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
							 else if (index_m!=-1)
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, s+1, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_number_m = this->get_shock_number(index_shock_array, index_m, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_size=(index_shock_array[s+1][1]-index_shock_array[s][1])/100.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
							 else
							 {
							  int shock_number_p = this->get_shock_number(index_shock_array, s, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
							  shock_size=(index_shock_array[s][1]-index_shock_array[s-1][1])/100./2.;
							  OV_p = map_of_greek[scen_pass_n][mp_number][shock_number_p][OPTION_VALUE_GREEK_SUM];
							 }
						   }
							double OV_pp, OV_mm = 0.;
							double OV_m = map_of_greek[scen_pass_n][mp_number][shock_number_m][OPTION_VALUE_GREEK_SUM];
							double OV =   map_of_greek[scen_pass_n][mp_number][shock_number][OPTION_VALUE_GREEK_SUM];
							double delta =0;
							double gamma =0;
							if (av0 !=0){
								OV_pp = OV + (OV_p -OV)*(const_shock * (1+index_shock_array[s+1][1]/100.))/ shock_size ;
								OV_mm = OV + (OV_m -OV)*(const_shock * (1+index_shock_array[s-1][1]/100.))/ shock_size ;

								/*delta = (OV_pp - OV_mm) / ( 2. * av0 * const_shock * (1+index_shock_array[s][1]/100.)) ;
								gamma = (OV_pp + OV_mm -2 * OV) / ( 2. * av0 * const_shock * (1+index_shock_array[s][1]/100.)) ;*/
								delta = (OV_p - OV_m) / ( 2. * av0 * shock_size) ;
								gamma = (OV_p + OV_m -2 * OV) / ( 2. * av0 * shock_size) ;
							}
							map_of_greek[scen_pass_n][mp_number][shock_number][DELTA_GREEK_SUM] = delta;
							map_of_greek[scen_pass_n][mp_number][shock_number][GAMMA_GREEK_SUM] = gamma;
						}
					}

					
				}
			}
		}

	}

void init_IR_shock_esg(vector<vector<double>>& IR_shock_list, int base_no)
	{
		map<int, vector<vector<int>>> index_shocks;
		map_shock_esg_to_map_shock(index_map_shocks_esg,index_shocks);
		//map<int, map<int, vector<vector<int>>>> ::const_iterator iter = index_map_shocks_esg.begin(); 
		//index_shocks = iter->second;
		const int total_shock=30;
		const int init_no=1;
		for(int IR_idx=(init_no+SCN_TOTAL*base_no);IR_idx<(SCN_TOTAL*base_no+total_shock+init_no+2);IR_idx++)
		{
			if (IsExist_Scn(index_shocks,IR_idx))
			{
				vector<double> temp_shoc;
				temp_shoc.push_back(get_IR_shock(IR_idx, base_no));
                temp_shoc.push_back(double(IR_idx));
				IR_shock_list.push_back(temp_shoc); 
			}
		}

		sort (IR_shock_list.begin(),IR_shock_list.end());
		vector<vector<double>>::iterator it_temp;
		it_temp=unique(IR_shock_list.begin(),IR_shock_list.end(),compare_irshoc);
        IR_shock_list.resize(it_temp - IR_shock_list.begin());

	}

	void map_shock_esg_to_map_shock(map<int, map<int, vector<vector<int>>>> map_shock_esg, map<int, vector<vector<int>>>& map_shock )
	{
		//map<int, vector<vector<int>>>::iterator iter = map_shock.begin();
		try{
			map<int, vector<vector<int>>>::const_iterator iter_esg2;
			for (map<int, map<int, vector<vector<int>>>>::const_iterator iter_esg = map_shock_esg.begin(); 
					iter_esg != map_shock_esg.end(); ++iter_esg)
			{
				iter_esg2 =iter_esg->second.begin();
				map_shock.insert ( pair<int, vector<vector<int>>>(iter_esg->first,iter_esg2->second));
				//iter_esg2 ++;
			}
		}
		catch(GMXBException *e)
		{
			cout << "Error from Summary_file_writer->map_shock_esg_to_map_shock : " << e->get_message() << endl;
			delete e;
		}
		
	}
};