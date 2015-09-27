#pragma once


extern ofstream trace;
using namespace std;


class Summary_File_Writer
{
private :
	int number_of_workers;
	Summary_configuration *summary_configuration;
	map<int, vector<vector<int>>> map_shocks;
	double a;
	double sigma;
	map<string, vector<string>> list_scenario_names;
	map<string, vector<string>> list_index_shock_names;
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

		// Check the content of each scenario
		for (int nb_scen = 0; nb_scen < number_of_scenario; nb_scen++)
		{
			Scenario_configuration scenario_config_0 = products[0].get_scenario_list()[nb_scen];
			// check the scenario pass number
			int scenario_pass_number_0 = scenario_config_0.get_scenario_pass_number();
			vector<vector<int>> s_array_0 = scenario_config_0.get_shock_array();
			int s_array_0_size = s_array_0.size();

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
					if (scenario.get_scenario_pass_number() == scenario_pass_number_0)
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
				vector<vector<int>> s_array = scenario.get_shock_array();
				int s_array_size = s_array.size();
				if (s_array_0_size != s_array_size)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization error: Shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
						products[0].get_run_name().c_str(), products[nb_product].get_run_name().c_str(), scenario_pass_number_0);
					throw new GMXBException(message);
				}
				// Check the content of the shock array
				for(int s_size = 0; s_size < s_array_0_size; s_size++)
				{
					vector<int> index_shocks_0 = s_array_0[s_size];
					vector<int> index_shocks = s_array[s_size];
					for(int shock = 0; shock < index_shocks_0.size(); shock++)
					{
						if (index_shocks_0[shock]!=index_shocks[shock])
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization error: Shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
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
			this->map_shocks.insert (pair<int,vector<vector<int>>>(scenario_pass_number_0,s_array_0));
		}
	}

	// Get the pvs aggregated 
	// per intermediate result file
	void get_pvs_aggregated(vector<double>& pvs_aggregated, int scenario_pass_number, int shock_number, 
		Product_configuration& product)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		vector<vector<double>> pvs;
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
				// read the intermediate file
				sprintf(file_name, "%s%s_total_%d_%d_%d.csv", product.get_result_file_path().c_str(), product.get_run_name().c_str(), 
							scenario_pass_number, shock_number, worker_id);
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
							while ((tokenizer = strtok (NULL, ",")) != NULL) 
							{	 
								pvs_per_worker.push_back(atof(tokenizer)/tot_num_iterations);
							} 
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
	void get_pvs_by_policy(vector<vector<double>>& pv_results, int scenario_number, int shock_number,
		Product_configuration& product)
	{
		char file_name[MAX_LENGTH_FILE_NAME]; 
		for (int worker_id = 1; worker_id <= this->number_of_workers; worker_id++)
		{
			// read the intermediate file
			sprintf(file_name, "%s%s_%d_%d_%d.csv", product.get_result_file_path().c_str(), product.get_run_name().c_str(), 
							scenario_number, shock_number, worker_id);
			
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

public :
	Summary_File_Writer(int nb_workers, Summary_configuration *summary, double calib_parameter_a, 
					double calib_parameter_sigma, const map<string, vector<string>>& map_scenario_names, 
					const map<string, vector<string>>& map_index_shock_names)
	{	
		this->number_of_workers = nb_workers;
		this->summary_configuration = summary;
		this->summary_checker();
		this->a = calib_parameter_a;
		this->sigma = calib_parameter_sigma;
		this->list_scenario_names = map_scenario_names;
		this->list_index_shock_names = map_index_shock_names;
	}

	~Summary_File_Writer()
	{
	}

	static int get_index_shock_number(const vector<vector<int>>& s_array, int s_number)
	{
		if(s_array[s_number][0]==0)
			return 0;
		else if(s_array[s_number][0]==SUBACCT_NUM_INDEX+1)
		{
			if(s_array[s_number][1]>0)
			{
				return (MAX_DELTA_SHOCKS_PER_INDEX * SUBACCT_NUM_INDEX + s_array[s_number][1]);
			}
			else
			{
				return (MAX_DELTA_SHOCKS_PER_INDEX * (SUBACCT_NUM_INDEX+1) + 1 + s_array[s_number][1]);
			}
		}
		else if(s_array[s_number][0]>0 && s_array[s_number][0]<=SUBACCT_NUM_INDEX)
		{
			if(s_array[s_number][s_array[s_number][0]]>0)
			{
				return (MAX_DELTA_SHOCKS_PER_INDEX * (s_array[s_number][0] - 1) + s_array[s_number][s_array[s_number][0]]);
			}
			else
			{
				return (MAX_DELTA_SHOCKS_PER_INDEX * s_array[s_number][0] + 1 + s_array[s_number][s_array[s_number][0]]);
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Bad configuration of the shock array");
			throw new GMXBException(message);		
		}
	}

	void write_ehc_summary_file(const map<string, vector<Model_Point*>>& mps, 
									const map<string, map<int,map<int, vector<vector<double>>>>>& results_map)
	{
		map<string, vector<double>> map_ehc_values;
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
			// vector<vector<double>> pvs = this->get_pvs_vector(pCK, 1, 0, results_map); 
			vector<vector<double>> pvs = this->get_pvs_vector(pCK, summary_configuration->get_ehc_scen_pass_num(), 0, results_map);
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
				double ehc = 0;
				if (agg_real_charges != 0)
					ehc = (agg_claims/agg_real_charges) * model_p->gmxb_g_c;
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Charges equal 0 for policy number %s ", model_p->policy_id.c_str());
					throw new GMXBException(message);		
				}
				ehc_vector.push_back(ehc);
			}
			map_ehc_values[pCK] = ehc_vector;
		}
		ehc_file.close();

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



		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_ehc_file_directory().c_str(), 
							this->summary_configuration->get_ehc_file_name().c_str());

		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, pv_claims, pv_realch, pv_ehcch, pv_dep_fee, a, sigma" << endl;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->map_shocks.begin(); 
				iter_scen_pass_num != this->map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> shock_array = iter_scen_pass_num->second;
			int size_of_shock_array = shock_array.size();
			for(int s = 0; s < size_of_shock_array; s++ )
			{
				int shock_number = this->get_index_shock_number(shock_array, s);
				string shock_full_name = "";
				double pv_claims = 0.;
				double pv_realch = 0.;
				double pv_ehcch = 0.;
				double pv_dep_fee = 0.;
				// Loop on the products
				for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
				{
					string pCK = iter_mp->first;
					vector<Model_Point*> model_points = iter_mp->second;
					vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, results_map);
					vector<double> ehc_values = map_ehc_values[pCK];
					int number_of_mps = model_points.size();
					for(int i = 0; i < number_of_mps; i++)
					{
						Model_Point *model_p = model_points[i];
						double ehc_value = ehc_values[i];
						vector<double> pv = pvs[i];
						pv_claims += pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
						pv_realch += pv[PV_GMXB_REAL_CHG];
						pv_ehcch += pv[PV_GMXB_REAL_CHG] * ehc_value/model_p->gmxb_g_c;
					}
				}

				ostringstream oss_scen_pass_n;
				ostringstream oss_shock_n;

				string scenario_full_name;
				string scenario_name = "";
				string index_shock_name = "";

				oss_scen_pass_n << scen_pass_n;
				oss_shock_n << shock_number;
				scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
				
				if (shock_number > 0)
					index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
				
				scenario_full_name = scenario_name + " " + index_shock_name;
				
				// write the output line
				sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_claims << "," << setprecision(15) << pv_realch << "," << setprecision(15) << pv_ehcch 
					<< "," << setprecision(15) << pv_dep_fee << "," << setprecision(15)<< this->a << "," << setprecision(15) << this->sigma << endl;
			}
		}
		sensi_file.close();
	}

	void write_total_summary_file(const map<string, map<int, map<int, vector<double>>>>& results_map)
	{

		char sensitivities_file_name[MAX_LENGTH_FILE_NAME];
		sprintf(sensitivities_file_name, "%s%s.csv", this->summary_configuration->get_total_file_directory().c_str(), 
				this->summary_configuration->get_total_file_name().c_str());
		
		ofstream sensi_file(sensitivities_file_name);
		sensi_file  << "scenario, pv_claims, pv_realch, pv_ehcch, pv_dep_fee, a, sigma" << endl;
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->map_shocks.begin(); 
				iter_scen_pass_num != this->map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> shock_array = iter_scen_pass_num->second;
			int size_of_shock_array = shock_array.size();
			for(int s = 0; s < size_of_shock_array; s++ )
			{
				int shock_number = this->get_index_shock_number(shock_array, s);
				string shock_full_name = "";
				double pv_claims = 0.;
				double pv_realch = 0.;
				double pv_ehcch = 0.;
				double pv_dep_fee = 0.;
				// Loop on the products
				for(map<string, map<int, map<int, vector<double>>>>::const_iterator iter_products = results_map.begin();
						iter_products != results_map.end(); ++iter_products)
				{
					string pCK = iter_products->first;
					vector<double> pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, results_map);
					pv_claims += pvs[PV_GMAB]+ pvs[PV_GMDB]+ pvs[PV_GMIB] + pvs[PV_GMWB] + pvs[PV_GMSB];
					pv_realch += pvs[PV_GMXB_REAL_CHG];
					pv_ehcch += pvs[PV_GMXB_EHC_CHG];
					pv_dep_fee += pvs[PV_DEP_FEE];


				}

				ostringstream oss_scen_pass_n;
				ostringstream oss_shock_n;

				string scenario_full_name;
				string scenario_name = "";
				string index_shock_name = "";

				oss_scen_pass_n << scen_pass_n;
				oss_shock_n << shock_number;
				scenario_name = this->list_scenario_names[oss_scen_pass_n.str()][0];
				
				if (shock_number > 0)
					index_shock_name = this->list_index_shock_names[oss_shock_n.str()][0];
				
				scenario_full_name = scenario_name + " " + index_shock_name;
				
				// write the output line
				sensi_file << scenario_full_name.c_str() << "," << setprecision(15) << pv_claims << ","  << setprecision(15) 
					<< pv_realch << "," << setprecision(15) << pv_ehcch  << "," << setprecision(15) << pv_dep_fee << "," << setprecision(15) 
					<< this->a << "," << setprecision(15) << this->sigma << endl;
			}
		}
		sensi_file.close();
	}


	// TO DO - Must display all the 
	// fields of the model points
	void write_policy_by_policy_summary_file(const map<string, vector<Model_Point*>>& mps, 
											const map<string, map<int,map<int, vector<vector<double>>>>>& results_map)
	{
		for (map<int, vector<vector<int>>>::const_iterator iter_scen_pass_num = this->map_shocks.begin(); 
				iter_scen_pass_num != this->map_shocks.end(); ++iter_scen_pass_num)
		{
			int scen_pass_n = iter_scen_pass_num->first;
			vector<vector<int>> shock_array = iter_scen_pass_num->second;
			int size_of_shock_array = shock_array.size();
			for(int s = 0; s < size_of_shock_array; s++ )
			{
				int shock_number = this->get_index_shock_number(shock_array, s);

				char pol_by_pol_file_name[MAX_LENGTH_FILE_NAME];
				sprintf(pol_by_pol_file_name, "%s%s_%d_%d.csv", this->summary_configuration->get_policy_by_policy_file_directory().c_str(), 
					this->summary_configuration->get_policy_by_policy_file_name().c_str(), scen_pass_n, shock_number);

				ofstream pol_by_pol_file(pol_by_pol_file_name);

				pol_by_pol_file  << "Version,Country,Product,Group,prod_type,POL_ID,1,1,GMXB_G_C,GMXB_H_C,agg_real_charges,agg_claims,agg_dep_fee,PV_GMAB,PV_GMDB,PV_GMIB,PV_GMWB,PV_GMSB,AGE_VALN,DURATION,GMXB_TYPE2,AX_GENDER,PREM_PAID,SPLIT_IDX0,SPLIT_IDX1,SPLIT_IDX2,SPLIT_IDX3,SPLIT_IDX4,SPLIT_IDX5,SPLIT_IDX6,SPLIT_IDX7,SPLIT_IDX8,SPLIT_IDX9,SPLIT_IDX10,SPLIT_IDX11,GMAB1_VALN,GMAB2_VALN,GMDB1_VALN,GMDB2_VALN,GMWB1_VALN,GMWB2_VALN,GMWB_DEF_P,GMWB_RAT_P,GMXB_M_C,GMXB_A_C,AXA_ROLL_R" << endl;

				// Loop on the products
				for(map<string, map<int, map<int, vector<vector<double>>>>>::const_iterator iter_products = results_map.begin();
						iter_products != results_map.end(); ++iter_products)
				{
					string pCK = iter_products->first;
					vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, results_map);
					int pvs_size = pvs.size();

					for(map<string, vector<Model_Point*>>::const_iterator iter_mp = mps.begin();
						iter_mp != mps.end(); ++iter_mp)
					{
						string pCK = iter_mp->first;
						vector<Model_Point*> model_points = iter_mp->second;
						vector<vector<double>> pvs = this->get_pvs_vector(pCK, scen_pass_n, shock_number, results_map);
						int number_of_mps = model_points.size();
						for(int i = 0; i < number_of_mps ; i++)
						{
							double agg_real_charges = 0.;
							double agg_claims = 0.;
							double agg_dep_fee = 0.;
							Model_Point *model_p = model_points[i];
							vector<double> pv = pvs[i];
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
							pol_by_pol_file << model_p->gmxb_ehc_charge << ",";
							agg_real_charges = pv[PV_GMXB_REAL_CHG];
							agg_claims = pv[PV_GMAB]+ pv[PV_GMDB]+ pv[PV_GMIB] + pv[PV_GMWB] + pv[PV_GMSB];
							agg_dep_fee = pv[PV_DEP_FEE];
							pol_by_pol_file << setprecision(15) << agg_real_charges << ",";		
							pol_by_pol_file << setprecision(15) << agg_claims << ",";				
							pol_by_pol_file << setprecision(15) << agg_dep_fee << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMAB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMDB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMIB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMWB] << ",";
							pol_by_pol_file << setprecision(15) << pv[PV_GMSB] << ",";
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
							pol_by_pol_file << endl;
						}
				}
			}
			pol_by_pol_file.close();
		}
	}	
	}


	void write_output_summary()
	{
		if (this->summary_configuration->get_ehc_format_indicator() 
				|| this->summary_configuration->get_policy_by_policy_indicator())
				this->perform_detailed_summary();

		if (this->summary_configuration->get_total_file_indicator())
				this->perform_total_summary();
	}

	void perform_total_summary()
	{
		map<string, map<int,map<int, vector<double>>>> map_results;

		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			Product_configuration product = products[i];
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();
			map<int,map<int, vector<double>>> map_scen_pass_num;
			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				int shock_size = scenario.get_shock_array().size();
				map<int, vector<double>> map_shock;
				for (int shock = 0; shock < shock_size; shock++)
				{
					vector<double> pv_res;
					int shock_num = this->get_index_shock_number(scenario.get_shock_array(), shock);
					this->get_pvs_aggregated(pv_res, scen_pass_num, shock_num, product); 
					map_shock[shock_num] = pv_res;
				}
				map_scen_pass_num[scen_pass_num] = map_shock;
			}
			map_results[product.get_prod_code_key()] = map_scen_pass_num;
		}
		this->write_total_summary_file(map_results);	
	}	


	void perform_detailed_summary()
	{
		map<string, map<int,map<int, vector<vector<double>>>>> map_results;
		map<string, vector<Model_Point*>> mps_per_product;

		Inforce_Manager *inforce_mgr = NULL;
	
		// Loop over all the products
		vector<Product_configuration> products = this->summary_configuration->get_product_configurations();
		int products_size = products.size();
		for (int i = 0; i < products_size; i++)
		{
			vector<Model_Point*> list_of_mps;

			Product_configuration product = products[i];

			inforce_mgr->get_list_of_model_points_per_product(product.get_prod_code_key(), 
						product.get_model_point_file(),list_of_mps);

			int number_of_model_points = list_of_mps.size();

			mps_per_product[product.get_prod_code_key()] = list_of_mps;
			
			// Loop over all the scenarios
			int number_of_scenarios = product.get_scenario_list().size();
			map<int,map<int, vector<vector<double>>>> map_scen_pass_num;
			for (int j = 0; j < number_of_scenarios; j++)
			{
				Scenario_configuration scenario = product.get_scenario_list()[j];
				int scen_pass_num = scenario.get_scenario_pass_number();
				int shock_size = scenario.get_shock_array().size();
				map<int, vector<vector<double>>> map_shock;
				for (int shock = 0; shock < shock_size; shock++)
				{
					vector<vector<double>> pv_res;
					int shock_num = this->get_index_shock_number(scenario.get_shock_array(), shock);
					this->get_pvs_by_policy(pv_res, scen_pass_num, shock_num, product); 
					// Check if the size of the model point list 
					// is the same as the pv result list.
					// If different size raise an exception
					int pv_result_size = pv_res.size();
					if (number_of_model_points != pv_result_size)
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Summarization: Mismatch between the number of model points (%d) and the size of the pv results (%d)  ", 
								number_of_model_points, pv_result_size);
						throw new GMXBException(message);	
					}
					map_shock[shock_num] = pv_res; 
				}
				map_scen_pass_num[scen_pass_num] = map_shock;
			}
			map_results[product.get_prod_code_key()] = map_scen_pass_num;
		}
		// write the output files
		if (this->summary_configuration->get_ehc_format_indicator())
				this->write_ehc_summary_file(mps_per_product, map_results);
		if (this->summary_configuration->get_policy_by_policy_indicator())
				this->write_policy_by_policy_summary_file(mps_per_product, map_results);


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
};