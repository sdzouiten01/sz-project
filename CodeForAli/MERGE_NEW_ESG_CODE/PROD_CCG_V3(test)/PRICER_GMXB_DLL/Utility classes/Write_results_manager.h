#pragma once
//#include "stdafx.h"
////#include "Utility classes\GMXBLogger.h"
//#include "head.h"
//#include <sstream>
//#include <iostream>
//#include <cmath>
//#include <string>
//#include <fstream>
//#include <vector>
//#include "GMXBException.h"

#include "../Projection classes/Projection.h"




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

	void write_total_result(Projection* p, const string& run_name, const vector<vector<int>>& index_shock_array, /*const vector<vector<int>>& fund_shock_array,*/
							vector<Model_Point*>& mp_vector, const string& r_file_path,int w_id, int sc_pass_n, int nb_scenarios, int shock_size, 
							double alpha, double sigma, Scenario_configuration& scen_conf, int bucket_id )
	{
		UNREFERENCED_PARAMETER(scen_conf);
		UNREFERENCED_PARAMETER(shock_size);
		UNREFERENCED_PARAMETER(p);
		int total_pv_columns = 0;
		if (mp_vector.size() > 0)
		{
			Model_Point& mp = *mp_vector[0];
			total_pv_columns = mp.get_total_pv_columns_number();
		}
	
		ofstream total_valorisation_file;
		for(unsigned int k = 0; k < index_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			int shock_numb = get_shock_number(index_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		
			if(bucket_id == -1) // file mode
				sprintf_s(filename, "%s%s_total_index_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf_s(filename, "%s%s_total_index_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
			remove(filename);

			int number_of_policies = mp_vector.size();
			total_valorisation_file.open(filename);
			if (total_pv_columns > 0)
			{
				vector<double> pvs;
				for(int col =0; col < total_pv_columns; col++)
					pvs.push_back(0.);

				for(int i = 0; i < number_of_policies; i++)
				{
					Model_Point& mp = *mp_vector[i];
					for(int j =0; j< total_pv_columns; j++)
						pvs[j] += mp.index_shock_pvs[k][j];
				}

				total_valorisation_file << nb_scenarios << ",";

				total_valorisation_file << alpha << "," << sigma << ",";


				for(int j =0; j < total_pv_columns; j++)
				{   
					total_valorisation_file << setprecision(15) <<	pvs[j] << ",";
				}

				total_valorisation_file << number_of_policies << endl;
			}
			total_valorisation_file.close();
		}


	}


	void write_results_policy_by_policy(Projection* p, const string& run_name, const vector<vector<int>>& index_shock_array, vector<Model_Point*>& mp_vector, 
		const string& r_file_path, /*int w_id,*/ int sc_pass_n, int nb_scenarios,int shock_size, double alpha, double sigma, int bucket_id)
	{
		UNREFERENCED_PARAMETER(shock_size);
		UNREFERENCED_PARAMETER(p);
		ofstream model_points_valorisation_file;
		for(unsigned int k = 0; k < index_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];

			/*int shock_numb = this->get_shock_number(index_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_index_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_index_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);*/
			int index_indicator=index_shock_array[k][0];
			int index_shock_level=0;
			if(index_indicator <= MAXIMUM_NUMBER_OF_INDEXES)
				index_shock_level=index_shock_array[k][index_indicator];
			else
				index_shock_level=index_shock_array[k][MAXIMUM_NUMBER_OF_INDEXES];

			if(bucket_id == -1)
				sprintf_s(filename, "%s%s_index_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, index_indicator, index_shock_level/*, w_id*/);
			else
				sprintf_s(filename, "%s%s_index_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, index_indicator, index_shock_level/*, w_id*/);

			remove(filename);
			model_points_valorisation_file.open(filename);

			int number_of_policies = mp_vector.size();
			//Ali Changes//
			model_points_valorisation_file <<"Position in the file,Number of scenarios,Hull White alpha,Hull White sigma,Prod Code Key,";
			model_points_valorisation_file <<"Policy id,beginning account value ,RRC TOT rate,Guarantee fees rate,Fixed fees rate,";
			model_points_valorisation_file <<"Ongoing commissions rate,Tail commissions rate,EHC (internal) rate,EHC (New-Re) rate,PV Claims WB,";
			model_points_valorisation_file <<"PV Claims DB,PV Claims AB,PV Claims IB,PV Claims TOT,PV AV (Internal),PV AV (New-Re),PV RRC TOT Charges,";
			model_points_valorisation_file <<"PV Guarantee fees Charges,PV Fixed fees Charges,PV Ongoing commissions Charges,PV Tail commissions Charges,PV Fund fees Charges,";
			model_points_valorisation_file <<"PV Retrocessions Charges,PV Expenses Charges,PV Real Charges,PV EHC(internal) Charges,PV EHC(New-Re) Charges,";
			model_points_valorisation_file <<"PV Lapse Charges,LOV(Internal),LOV(New-Re),ITM,";
			model_points_valorisation_file << endl;
			//end change Ali
			for(int i = 0; i < number_of_policies; i++)
			{
				Model_Point& mp = *mp_vector[i];
				model_points_valorisation_file << mp.mp_position_in_file << ",";
				model_points_valorisation_file << nb_scenarios << ",";
			    model_points_valorisation_file << alpha << "," << sigma << ",";
				model_points_valorisation_file << mp.Prod_Code << ",";
				model_points_valorisation_file << mp.Pol_Id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
				   model_points_valorisation_file << setprecision(15) << mp.index_shock_pvs[k][j]<<",";
				model_points_valorisation_file << endl;
			}
			model_points_valorisation_file.close();
		}

	

	}
void write_results(Projection* p,const string& run_name, const vector<vector<int>>& index_shock_array, vector<Model_Point*>& mp_vector, 
		const string& r_file_path,int w_id, int sc_pass_n, int nb_scenarios, int shock_size, int mode_of_writing,double alpha, double sigma, 
		Scenario_configuration& scen_conf, int bucket_id = -1)   // default parameter for file mode
	{
		if (mode_of_writing ==1) // pol_by_pol
		{
			write_results_policy_by_policy(p, run_name, index_shock_array, mp_vector, r_file_path,sc_pass_n, nb_scenarios, shock_size, alpha, sigma, bucket_id);
		}
		else if(mode_of_writing ==0) // total
		{
			write_total_result(p, run_name, index_shock_array, mp_vector, r_file_path, w_id, sc_pass_n, nb_scenarios, shock_size, alpha, sigma, scen_conf, bucket_id);
		}
		else if(mode_of_writing ==2)
		{
			//this->write_results_scen_by_scen(p, run_name, index_shock_array, fund_shock_array, mp_vector, 
			//	r_file_path, w_id, sc_pass_n, nb_scenarios, shock_size, alpha, sigma, scen_conf, bucket_id);
		}
		else
		{
		    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error in function write_results(...), the input parameter: mode_of_summary is wrong!" );
			throw new GMXBException(message);
		}
	
	}

