#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "Projection classes/Guarantee projection classes/Projection.h"
#include "GMXBLogger.h"
#include "GMXBException.h"
#include "Scenario_manager.h"
#include <iostream>
#include <ctime>

#include "Utility classes/Builder.h"
#include "Utility classes/Inforce_Manager.h"
#include "Utility classes/Scenario_configuration.h"
#include "Utility classes/Product_configuration.h"
#include "Utility classes/Summary_configuration.h"
#include "Summary classes/Summary_File_Writer.h"
#include "Utility classes/Summary_Builder.h"
#include "InterfaceEsg.h"
#include "InterfaceCalibration.h"
#include "Utility classes/Dispatch_Manager.h"
#include "Utility classes/Bucket_list.h"
#include "Utility classes/Scenario_list.h"
#include "Utility classes/Stat_Inforce_Manager.h"
#include "Utility classes/Stat_Inforce_Builder.h"

#include "Utility classes/HE_configuration.h"
#include "Utility classes/HE_Writter.h"

#include "Utility classes/Aging_Policies_configuration.h"


extern ofstream trace;
extern ofstream trace_out;
extern ofstream trace_in;
using namespace std;

class back_ground
{

private:
	Projection * projection;
	HE_configuration* he_conf;

public:
	~back_ground()
	{
		if (this->projection!=0)
			delete this->projection;
	}
	
	back_ground()
	{
		this->projection = 0;
	}


	back_ground(vector<Product_configuration>& product_config, int scen_pass_num,
				double*** va_scen, int tot_nb_workers, int w_id, string& trace_log_pth,
				double alpha, double sigma, int total_period)
	{
		this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name, "%strace.log", trace_log_pth.c_str());
		trace.open(trace_log_name);

		run_dll_projection_moses_esg(product_config, scen_pass_num, va_scen,
								tot_nb_workers, w_id, alpha, sigma, total_period);
		trace.close();
	}


	back_ground(vector<Product_configuration>& product_config, 
					 int tot_nb_workers, int w_id, string& trace_log_pth
					 ,double alpha, double sigma)
	{
		this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name, "%strace.log", trace_log_pth.c_str());
		trace.open(trace_log_name);

		run_dll_projection_file(product_config, tot_nb_workers, w_id, alpha, sigma);
		trace.close();
	}

	back_ground(vector<Product_configuration>& product_config, 
				int tot_nb_workers, int w_id, string& trace_log_pth,
				string& axagen_config_file,
				double alpha, double sigma)
	{

		this->projection=0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name, "%strace.log", trace_log_pth.c_str());
		trace.open(trace_log_name);

		//back_ground::price_with_different_seed(	product_config, tot_nb_workers, w_id, axagen_config_file);

		trace.close();
	}

	/* 
	* ESG and Projection by the DLL
	*/
	back_ground (vector<Product_configuration>& product_config,
				 string& trace_log_pth,
				 ofstream& log_strm,
				 int nbWorker,
				 int idWorker,
				 //int total_period,
				 Dispatch_Manager& dispatch_manager)
	{
	   	this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name, "%strace_%d.log", trace_log_pth.c_str(), idWorker);
		trace.open(trace_log_name);

		run_dll_projection_esg(product_config, log_strm, nbWorker,
		idWorker, /*total_period,*/ dispatch_manager);

		trace.close();
	}

	/*
	* Entry point of the hedge effectiveness pricing
	*/
	back_ground(vector<Product_configuration>&  product_configurations_out, 
				   vector<Product_configuration>& product_configurations_in,  
					int tot_nb_workers, int w_id, 
					string& trace_log_pth_out, string& trace_log_pth_in, 
					InterfaceCalibration& iCalib_in, InterfaceEsg& iEsg_in,
					HE_configuration* he_conf,ofstream& log_strm)
	{

		this->projection = 0;
		this->he_conf = he_conf;

		char  trace_log_name_out[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name_out, "%strace_out.log", trace_log_pth_out.c_str());
		trace_out.open(trace_log_name_out);
		char  trace_log_name_in[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name_in, "%strace_in.log", trace_log_pth_in.c_str());
		trace_in.open(trace_log_name_in);

		run_hedge_effectiveness_projection(product_configurations_out, product_configurations_in, 
				he_conf, iCalib_in, iEsg_in, tot_nb_workers, w_id,log_strm);

		trace_in.close();
		trace_out.close();
	}

		/*For Risk Appetite*/
	back_ground(vector<Product_configuration>& product_config, 
					 int tot_nb_workers, int w_id, string& trace_log_pth)
	{
		this->projection = 0;
		char  trace_log_name[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name, "%strace.log", trace_log_pth.c_str());
		trace.open(trace_log_name);

		run_dll_projection_RA(product_config, tot_nb_workers, w_id);
		trace.close();
	}

	/*
	* Entry point of aging policies
	*/

	back_ground(vector<Product_configuration>&  product_configurations_out,  
				int tot_nb_workers,
				int w_id, 
				string& trace_log_pth_out,
				Aging_Policies_configuration* ap_conf)
	{

		this->projection = 0;

		char  trace_log_name_out[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(trace_log_name_out, "%strace_out.log", trace_log_pth_out.c_str());
		trace_out.open(trace_log_name_out);
		
		run_aging_policies_projection(product_configurations_out,
		ap_conf, tot_nb_workers, w_id);

		trace_out.close();
	}



	void summarize_results( vector<Summary_configuration>& summarization_config, int number_of_workers,
							map<string, vector<string>>& map_scenario_names,
							map<string, vector<string>>& map_index_shock_names,
							Dispatch_Manager* dispatch_manager)
	{
		
		int number_of_products_to_summarize = summarization_config.size();
		
		for (int summary_configuration_number = 0; summary_configuration_number < number_of_products_to_summarize; summary_configuration_number++)
		{
			// Get the product configuration
			Summary_configuration summary = summarization_config[summary_configuration_number];
			this->generate_summarized_file(summary, number_of_workers, map_scenario_names, map_index_shock_names, dispatch_manager);
		}
	}

	/*
	* Summarize of the hedge effectiveness 
	*/
	void summarize_he_results(vector<Summary_configuration>& summary_configurations,
							  HE_configuration* he_conf,
							  int tot_nb_workers)
	{

		int number_of_products_to_summarize = summary_configurations.size();
		for (int summary_configuration_number = 0; summary_configuration_number < number_of_products_to_summarize; 
			summary_configuration_number++)
		{
			Summary_configuration summary = summary_configurations[summary_configuration_number];
			string pCK = summary.get_v_product_out()[0].get_prod_code_key();
			HE_Summary_File_Writer * he_summary = he_summary_builder(pCK);
			he_summary->HE_summerize(summary.get_v_product_out(),summary.get_v_product_in(),
			he_conf, summary.get_he_file_path(), summary.get_he_file_name());
			delete he_summary;
		}
	
	}
	
	void generate_stat_inforces(vector<Stat_Inforce_Configuration>& v_stat_inforces)
	{
		int number_of_stat_inforce = v_stat_inforces.size();
		if (number_of_stat_inforce > 0)
		{
			for (int i = 0; i < number_of_stat_inforce; i++)
			{
				Stat_Inforce_Configuration stat_inforce = v_stat_inforces[i];

				string pCK = stat_inforce.get_v_product_codes_key()[0];
			
				Stat_Inforce_Manager * stat_inforce_mgr = build_stat_inforce_manager(pCK);

				stat_inforce_mgr->perform_statistics(stat_inforce);

				delete stat_inforce_mgr;
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The vector of stat inforce configuration is empty ");
			throw new GMXBException(message);		
		}
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

private:

	void calculate_total_claims_and_charges(vector<vector<double>>& charges_and_claims_vector,
								vector<Model_Point>& mp_vector, int shock_size)
	{	

			for(int ss = 0 ; ss < shock_size; ss++)
			{
				double total_bav = 0.;
				double total_gmdb = 0.;
				double total_gmwb = 0.;
				double total_gmab = 0.;
				double total_gmib = 0.;
				double total_gmsb = 0.;
				double total_real_chg = 0.;
				double total_ehc_chg = 0.;
				double total_dep_fee = 0.;
				double discount_factor = 0.;
				int model_points_vector_size = mp_vector.size();
				for(int i = 0; i < model_points_vector_size; i++)
				{
					Model_Point mp = mp_vector[i];
					total_bav += mp.index_shock_pvs[ss][BEGINING_ACCOUNT_VALUE];
					total_gmdb += mp.index_shock_pvs[ss][PV_GMDB];
					total_gmwb += mp.index_shock_pvs[ss][PV_GMWB];
					total_gmab += mp.index_shock_pvs[ss][PV_GMAB];
					total_gmib += mp.index_shock_pvs[ss][PV_GMIB];
					total_gmsb += mp.index_shock_pvs[ss][PV_GMSB];
					total_real_chg += mp.index_shock_pvs[ss][PV_GMXB_REAL_CHG];
					total_ehc_chg += mp.index_shock_pvs[ss][PV_GMXB_EHC_CHG];
					total_dep_fee += mp.index_shock_pvs[ss][PV_DEP_FEE];
					discount_factor += mp.index_shock_pvs[ss][DISCOUNT_FACTOR_VALUE];
				}
				vector<double> v_pvs;
				v_pvs.push_back(total_bav);
				v_pvs.push_back(total_gmdb);
				v_pvs.push_back(total_gmwb);
				v_pvs.push_back(total_gmab);
				v_pvs.push_back(total_gmib);
				v_pvs.push_back(total_gmsb);
				v_pvs.push_back(total_real_chg);
				v_pvs.push_back(total_ehc_chg);
				v_pvs.push_back(total_dep_fee);
				v_pvs.push_back(discount_factor);
				charges_and_claims_vector.push_back(v_pvs);
			}
	}

	/*
	* Write the inforce file (Risk Appetite)
	*/
	void write_inforce_RA(string ** inforce1,string ** inforce2,string ** inforce3, string product_name, int nb_mp, int nb_col, string file_name,const string& r_file_path){
		
		ofstream RA_inforce_file;
		char file[MAX_LENGTH_FILE_NAME];
		sprintf(file, "%s\\%s_%s.csv", r_file_path.c_str(),file_name.c_str(),product_name.c_str());	
		RA_inforce_file.open(file);
		for(int i=0;i<nb_mp;i++){
			for(int j=0;j<nb_col;j++){
				RA_inforce_file<<inforce1[i][j] <<",";
			}
			RA_inforce_file<<endl;
		}
				
		for(int i=nb_mp;i<2*nb_mp;i++){
			for(int j=0;j<nb_col;j++){
				RA_inforce_file<<inforce2[i-nb_mp][j] <<",";
			}
			RA_inforce_file<<endl;
		}
						
		for(int i=2*nb_mp;i<3*nb_mp;i++){
			for(int j=0;j<nb_col;j++){
				RA_inforce_file<<inforce3[i-2*nb_mp][j] <<",";
			}
			RA_inforce_file<<endl;
		}
		RA_inforce_file.close();

	}

	void write_inforce_all(string ***inforce, string product_name,int nb_scn, int nb_mp, int nb_col, string file_name, const string& r_file_path)
	{
		ofstream all_inforce;
		char file[MAX_LENGTH_FILE_NAME];
		sprintf(file,"%s\\%s_%s_%s.csv", r_file_path.c_str(),"ALL",file_name.c_str(),product_name.c_str());
		all_inforce.open(file);
		for(int i=0;i<nb_mp;i++)		 
		{
		  for(int k=0; k<nb_scn; k++)
		 {
		 	 for(int j=0;j<nb_col;j++)
			 {
				all_inforce<<inforce[k][i][j] <<",";
			 }
             all_inforce<<k+1<<","<<i+1<<endl;
		 }
		}
		all_inforce.close();

	}
	void write_results(Projection* p,
					   const string& run_name, const vector<vector<int>>& index_shock_array, 
					   const vector<vector<int>>& fund_shock_array,
					   vector<Model_Point*>& mp_vector, const string& r_file_path,
					   int w_id, int sc_pass_n, int nb_scenarios, int shock_size, int mode_of_writing,
					   double alpha, double sigma, Scenario_configuration& scen_conf, int bucket_id = -1)   // default parameter for file mode
	{
		if (mode_of_writing ==1) // pol_by_pol
		{
			this->write_results_policy_by_policy(p, run_name, index_shock_array, fund_shock_array, mp_vector, 
				r_file_path, w_id, sc_pass_n, nb_scenarios, shock_size, alpha, sigma, scen_conf, bucket_id);
		}
		else if(mode_of_writing ==0) // total
		{
			this->write_total_result(p, run_name, index_shock_array, fund_shock_array,mp_vector, 
				r_file_path, w_id, sc_pass_n, nb_scenarios, shock_size, alpha, sigma, scen_conf, bucket_id);
		}
		else if(mode_of_writing ==2)
		{
			this->write_results_scen_by_scen(p, run_name, index_shock_array, fund_shock_array, mp_vector, 
				r_file_path, w_id, sc_pass_n, nb_scenarios, shock_size, alpha, sigma, scen_conf, bucket_id);
		}
		else
		{
		    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Error in function write_results(...), the input parameter: mode_of_summary is wrong!" );
			throw new GMXBException(message);
		}
	
	}

	void write_total_result(Projection* p, const string& run_name, const vector<vector<int>>& index_shock_array, 
							const vector<vector<int>>& fund_shock_array,
							vector<Model_Point*>& mp_vector, const string& r_file_path,
							int w_id, int sc_pass_n, int nb_scenarios, int shock_size, 
							double alpha, double sigma, Scenario_configuration& scen_conf, int bucket_id )
	{
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
			int shock_numb = this->get_shock_number(index_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		
			if(bucket_id == -1) // file mode
				sprintf(filename, "%s%s_total_index_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_total_index_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
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


		for(unsigned int k = 0; k < fund_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			int shock_numb = this->get_shock_number(fund_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, fund_shock_array[0].size());
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_total_fund_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_total_fund_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
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
						pvs[j] += mp.fund_shock_pvs[k][j];
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


	void write_results_policy_by_policy(Projection* p, const string& run_name, const vector<vector<int>>& index_shock_array, 
										const vector<vector<int>>& fund_shock_array, vector<Model_Point*>& mp_vector, 
										const string& r_file_path, int w_id, int sc_pass_n, int nb_scenarios,
										int shock_size, double alpha, double sigma, Scenario_configuration& scen_conf, int bucket_id)
	{
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
				sprintf(filename, "%s%s_index_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, index_indicator, index_shock_level/*, w_id*/);
			else
				sprintf(filename, "%s%s_index_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, index_indicator, index_shock_level/*, w_id*/);

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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
				   model_points_valorisation_file << setprecision(15) << mp.index_shock_pvs[k][j]<<",";
				model_points_valorisation_file << endl;
			}
			model_points_valorisation_file.close();
		}

		for(unsigned int k = 0; k < fund_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			int shock_numb = this->get_shock_number(fund_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, fund_shock_array[0].size());
			
			if(bucket_id == -1)
				sprintf(filename, "%s%s_fund_%d_%d_%d.csv",    r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_fund_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.fund_shock_pvs[k][j]<<",";
				model_points_valorisation_file << endl;
			}
			model_points_valorisation_file.close();
		}


		// Lapse shock part
		vector<GMXB_Assumption_Table *> v_lapse_mult = scen_conf.get_lapse_mult();
		for (unsigned int lapse_shock_nb = 0; lapse_shock_nb < v_lapse_mult.size(); lapse_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * lapse_mult_table = v_lapse_mult[lapse_shock_nb];
			string sheet_name = lapse_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%d_%s_%d.csv",    r_file_path.c_str(),run_name.c_str(), sc_pass_n, sheet_name.c_str(), w_id);
			else
				sprintf(filename, "%s%s_%d_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, sheet_name.c_str(), w_id);
		
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.pv_lapse[lapse_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}

		// Mortality shock part
		vector<GMXB_Assumption_Table *> v_mortality_mult = scen_conf.get_mortality_mult();
		for (unsigned int mortality_shock_nb = 0; mortality_shock_nb < v_mortality_mult.size(); mortality_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * mortality_mult_table = v_mortality_mult[mortality_shock_nb];
			string sheet_name = mortality_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, sheet_name.c_str(), w_id);
			else
				sprintf(filename, "%s%s_%d_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, sheet_name.c_str(), w_id);
		
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.pv_mortality[mortality_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}
		// Paidup shock part
		vector<GMXB_Assumption_Table *> v_paidup_mult = scen_conf.get_paidup_mult();
		for (unsigned int paidup_shock_nb = 0; paidup_shock_nb < v_paidup_mult.size(); paidup_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * paidup_mult_table = v_paidup_mult[paidup_shock_nb];
			string sheet_name = paidup_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, sheet_name.c_str(), w_id);
			else
				sprintf(filename, "%s%s_%d_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, sheet_name.c_str(), w_id);
		
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.pv_paidup[paidup_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}
		
		// election shock part
		vector<GMXB_Assumption_Table *> v_election_mult = scen_conf.get_election_mult();
		for (unsigned int election_shock_nb = 0; election_shock_nb < v_election_mult.size(); election_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * election_mult_table = v_election_mult[election_shock_nb];
			string sheet_name = election_mult_table->get_sheet_name();
			
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, sheet_name.c_str(), w_id);
			else
				sprintf(filename, "%s%s_%d_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, sheet_name.c_str(), w_id);
		
			remove(filename);
			sprintf(filename, "%s%s_%d_%s_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, sheet_name.c_str(), w_id);
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.pv_election[election_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}	
		// Longevity shock part
		if (scen_conf.get_load_longevity_table())
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%d_longevity_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, w_id);
			else
				sprintf(filename, "%s%s_%d_%d_longevity_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, w_id);
		
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
				model_points_valorisation_file << mp.prod_code_key << ",";
				model_points_valorisation_file << mp.policy_id << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << mp.pv_longevity[0][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}
	}

	//! Y
	void write_results_scen_by_scen(Projection* p,
									const string& run_name, 
									const vector<vector<int>>& index_shock_array, 
									const vector<vector<int>>& fund_shock_array,
									vector<Model_Point*>& mp_vector, 
									const string& r_file_path, int w_id, int sc_pass_n, int nb_scenarios,
									int shock_size, double alpha, double sigma, Scenario_configuration& scen_conf, int bucket_id)
	{
		ofstream model_points_valorisation_file;
		Model_Point& mp = *mp_vector[0];

		//! index_shock
		for(unsigned int k = 0; k < index_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			int shock_numb = this->get_shock_number(index_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, MAXIMUM_NUMBER_OF_INDEXES);
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_index_scen_by_scen_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_index_scen_by_scen_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);

			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
			    model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
				{
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.index_shock_pvs_scen_output[i][k][j]<<",";
				}
				model_points_valorisation_file << endl;
			}
			model_points_valorisation_file.close();
		}

		//! fund_shock
		for(unsigned int k = 0; k < fund_shock_array.size(); k++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			int shock_numb = this->get_shock_number(fund_shock_array, k, MAX_DELTA_SHOCKS_PER_INDEX, fund_shock_array[0].size());
			
			if(bucket_id == -1)
				sprintf(filename, "%s%s_fund_scen_by_scen_%d_%d_%d.csv",    r_file_path.c_str(),run_name.c_str(), sc_pass_n, shock_numb, w_id);
			else
				sprintf(filename, "%s%s_fund_scen_by_scen_%d_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, shock_numb, w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);

			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
				model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.fund_shock_pvs_scen_output[i][k][j]<<",";
				model_points_valorisation_file << endl;
			}
			model_points_valorisation_file.close();
		}


		//! Lapse shock
		vector<GMXB_Assumption_Table *> v_lapse_mult = scen_conf.get_lapse_mult();
		for (unsigned int lapse_shock_nb = 0; lapse_shock_nb < v_lapse_mult.size(); lapse_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * lapse_mult_table = v_lapse_mult[lapse_shock_nb];
			string sheet_name = lapse_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d.csv",    r_file_path.c_str(),run_name.c_str(), sheet_name.c_str(), sc_pass_n,  w_id);
			else
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sheet_name.c_str(), sc_pass_n, bucket_id,  w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);

			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
				model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.pv_lapse_scen_output[i][lapse_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}

		// Mortality shock part
		vector<GMXB_Assumption_Table *> v_mortality_mult = scen_conf.get_mortality_mult();
		for (unsigned int mortality_shock_nb = 0; mortality_shock_nb < v_mortality_mult.size(); mortality_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * mortality_mult_table = v_mortality_mult[mortality_shock_nb];
			string sheet_name = mortality_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d.csv", r_file_path.c_str(), run_name.c_str(), sheet_name.c_str(), sc_pass_n,  w_id);
			else
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d_%d.csv", r_file_path.c_str(), run_name.c_str(), sheet_name.c_str(), sc_pass_n, bucket_id,  w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);

			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
				model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.pv_mortality_scen_output[i][mortality_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}

		// Paidup shock part
		vector<GMXB_Assumption_Table *> v_paidup_mult = scen_conf.get_paidup_mult();
		for (unsigned int paidup_shock_nb = 0; paidup_shock_nb < v_paidup_mult.size(); paidup_shock_nb++)
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
			GMXB_Assumption_Table * paidup_mult_table = v_paidup_mult[paidup_shock_nb];
			string sheet_name = paidup_mult_table->get_sheet_name();
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d.csv", r_file_path.c_str(),run_name.c_str(),sheet_name.c_str(),sc_pass_n, w_id);
			else
				sprintf(filename, "%s%s_%s_scen_by_scen_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sheet_name.c_str(), sc_pass_n, bucket_id, w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);
			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
				model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.pv_paidup_scen_output[i][paidup_shock_nb][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}
		
		// Longevity shock part
		if (scen_conf.get_load_longevity_table())
		{
			char filename[MAX_LENGTH_MESSAGE_EXCEPTION];
		
			if(bucket_id == -1)
				sprintf(filename, "%s%s_longevity_scen_by_scen_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, w_id);
			else
				sprintf(filename, "%s%s_longevity_scen_by_scen_%d_%d_%d.csv", r_file_path.c_str(),run_name.c_str(), sc_pass_n, bucket_id, w_id);
		
			remove(filename);
			model_points_valorisation_file.open(filename);
			int number_of_policies = mp_vector.size();
			for(int i = 0; i < nb_scenarios; i++)
			{
				model_points_valorisation_file << i << ",";
				model_points_valorisation_file << alpha << "," << sigma << ",";
				for(int j =0; j< mp.get_total_pv_columns_number(); j++)
					model_points_valorisation_file << setprecision(15) << p->pvs_scenByscen.pv_longevity_scen_output[i][0][j]<<",";
				model_points_valorisation_file << endl;

			}
			model_points_valorisation_file.close();
		}
	}
	void create_payoff(const string& pCK, const char * assumption_file_name,
						const vector<vector <int>>& i_s_a, const vector<vector <int>>& f_s_a,
						bool generate_debug_files, const string& debug_file_path,
						const int t_high_shorter, bool load_longevity, int time_step)
	{
		if (this->projection!=0)
		{
			delete this->projection;
			this->projection = 0;
		}
		this->projection = projectionBuilder(pCK, assumption_file_name, i_s_a, f_s_a, generate_debug_files, debug_file_path, t_high_shorter, load_longevity, time_step);
	}


	Projection * create_projection(const string& pCK, const char * assumption_file_name,
						const vector<vector <int>>& i_s_a, const vector<vector <int>>& f_s_a,
						bool generate_debug_files, const string& debug_file_path,
						const int t_high_shorter, bool load_longevity, int time_step)
	{

		Projection * p = projectionBuilder(pCK, assumption_file_name, i_s_a, f_s_a, generate_debug_files, debug_file_path, t_high_shorter, load_longevity, time_step);

		return p;
	} 


	/*
	* Projection by the DLL and ESG launched by MoSes
	*/
	void run_dll_projection_moses_esg(vector<Product_configuration>& product_config,
								 int scenario_pass_num,
								 double *** va_scen,
								 int tot_nb_workers,
								 int w_id,
								 double alpha,
								 double sigma,
								 int total_period)
	{
		int t_high_shorter;

		if((total_period-1)%12!=0 || total_period > 721)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Error in function int scenario_manager :: init_t_high_shorter, (file_mode) (total_period-1)%12!=0 || total_period > 721" );
			throw new GMXBException(message);
		}
		else
		{
			t_high_shorter = (total_period-1)/12;
		}

		int number_of_products_to_run = product_config.size();

		for (int prod_configuration_number = 0; prod_configuration_number < number_of_products_to_run; prod_configuration_number++)
		{
			// Get the product configuration
			Product_configuration product = product_config[prod_configuration_number];
			// Get the scenario list
			vector<Scenario_configuration> list_of_scenarios = product.get_scenario_list();

			// Check if the list of scenario contain the passed scenario
			scenario_manager scen_mgr(va_scen,
				product.get_number_of_scenarios_to_run(),
				product.get_scenario_index_columns(), 
				product.get_yield_columns_for_pv(),
				product.get_lapse_dyn_treas(),
				product.get_rates_indic_index(),
				product.get_index_number(),
				t_high_shorter,
				product.get_time_step());

			int number_of_scenarios_to_run = list_of_scenarios.size();
			for (int scenario_number = 0; scenario_number < number_of_scenarios_to_run; scenario_number++)
			{

				Scenario_configuration scen_configuration = list_of_scenarios[scenario_number];
				if (scen_configuration.get_scenario_pass_number() == scenario_pass_num)
				{
					// scenario manager initialized and converted to monthly
					scen_mgr.init_scenario_manager(1);

					// Create the Inforce manager
					Inforce_Manager inforce_mgr(
						',',
						tot_nb_workers,
						w_id,
						product,
						scen_configuration,
						false,
						false,
						false
						);
					this->create_payoff(product.get_prod_code_key(), product.get_config_file().c_str(), 
										scen_configuration.get_index_shock_array(),
										scen_configuration.get_fund_shock_array(),
										product.get_generate_debug_file_indic(), product.get_debug_file_path(),
										t_high_shorter, scen_configuration.get_load_longevity_table(), product.get_time_step());


					//! Add the following line with Ali & Alex, 16.30/21/12/2009
					this->projection->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario()));


					// Set the lapse multiplicator tables end the mortality multiplicator tables
					this->projection->set_lapse_mult_table(scen_configuration.get_lapse_mult());
					this->projection->set_mortality_mult_table(scen_configuration.get_mortality_mult());
					this->projection->set_paidup_mult_table(scen_configuration.get_paidup_mult());
					this->projection->set_election_mult_table(scen_configuration.get_election_mult());

					// Loop over all the model points 
					for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
					{
						this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
						this->projection->initialise_index_map();
						Projection::compt_sen = 0;
						
						// Loop over all the scenarios
						for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
						{
							this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
							this->projection->project();
						}
					}

					write_results(this->projection,
						product.get_run_name(), 
						scen_configuration.get_index_shock_array(), 
						scen_configuration.get_fund_shock_array(), 
						inforce_mgr.get_model_points_vector(), 
						product.get_result_file_path(),
						w_id, 
						scenario_pass_num, 
						product.get_number_of_scenarios_to_run()/product.get_modulo_number_on_scenario(), 
						scen_configuration.get_index_shock_array().size(), 
						product.get_write_valo_policy_by_policy(),
						alpha, 
						sigma,
						scen_configuration);					
				}
			}
		}
	}

/*
	* Risk Appetite
	*/
	void run_dll_projection_RA(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id)
	{
		int number_of_products_to_run = product_config.size();
		vector<bool> RA_flg(number_of_products_to_run,0);//risk appetite = Y by default in assump file
		int t_high_shorter = -1;
		string** inforce_1;
		string** inforce_2;
		string** inforce_3;
		string*** inforce_sum;
		string RA_inforce_file;

		for (int prod_configuration_number = 0; prod_configuration_number < number_of_products_to_run; prod_configuration_number++)
		{
			//cout<<"Ageing Product "<<prod_configuration_number+1<<endl;
			// Get the product configuration
			Product_configuration product = product_config[prod_configuration_number];

			// Get the scenario list
			vector<Scenario_configuration> list_of_scenarios = product.get_scenario_list();

			// Initialize the scenario manager
			// ---- Suppose that: all the scenario files used for the same product have the same maturity

			scenario_manager scen_mgr(product.get_number_of_scenarios_to_run(),
				product.get_scenario_index_columns(), 
				product.get_yield_columns_for_pv(),
				product.get_lapse_dyn_treas(),
				product.get_rates_indic_index(),
				product.get_index_number(),
				t_high_shorter,
				list_of_scenarios[0].get_scenario_file_name(),
				product.get_time_step());    // initialize t_high_shorter !!

			int number_of_scenarios_to_run = list_of_scenarios.size();
			for (int scenario_number = 0; scenario_number <1; scenario_number++) //only the baseline
			{
				Scenario_configuration scen_configuration = list_of_scenarios[scenario_number];
				scen_configuration.set_to_basic(); //no shock, baseline only
               // cout<<"Loading Scenario File for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;
				scen_mgr.init_scenario_manager(scen_configuration.get_scenario_file_name(), 1);
                
				// Create the Inforce manager
				//cout<<"Creating inforce for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;
				Inforce_Manager inforce_mgr(
					',',
					tot_nb_workers,
					w_id,
					product,
					scen_configuration,
					false,
					false,
					true
					);

				// Create the projection
				//cout<<"Creating payoff for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;
				this->create_payoff(product.get_prod_code_key(), product.get_config_file().c_str(), 
									scen_configuration.get_index_shock_array(), 
									scen_configuration.get_fund_shock_array(),
									product.get_generate_debug_file_indic(),
									product.get_debug_file_path(),
									t_high_shorter,
									scen_configuration.get_load_longevity_table(),
									product.get_time_step()
									);

				this->projection->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario()));

				//// Set the lapse multiplicator tables end the mortality multiplicator tables
				this->projection->set_lapse_mult_table(scen_configuration.get_lapse_mult());
				this->projection->set_mortality_mult_table(scen_configuration.get_mortality_mult());
				this->projection->set_paidup_mult_table(scen_configuration.get_paidup_mult());

                //Initialization of string inforce
				if (this->projection->get_risk_appetite()=="Y")
				{
					inforce_1= new string*[inforce_mgr.nb_model_points];
					inforce_2= new string*[inforce_mgr.nb_model_points];
					inforce_3= new string*[inforce_mgr.nb_model_points];

					for (int i=0;i< inforce_mgr.nb_model_points; i++)
					{	inforce_1[i]=new string[inforce_mgr.number_of_columns];
						inforce_2[i]=new string[inforce_mgr.number_of_columns];
						inforce_3[i]=new string[inforce_mgr.number_of_columns];
					}
			
					this->projection->initialize_copied_inforce(inforce_mgr.get_inforce(),inforce_1,inforce_mgr.number_of_columns,inforce_mgr.nb_model_points);
					this->projection->initialize_copied_inforce(inforce_mgr.get_inforce(),inforce_2,inforce_mgr.number_of_columns,inforce_mgr.nb_model_points);
					this->projection->initialize_copied_inforce(inforce_mgr.get_inforce(),inforce_3,inforce_mgr.number_of_columns,inforce_mgr.nb_model_points);
					//for all output, if all==1
					if (this->projection->get_output()==1)
					{
						inforce_sum=new string**[scen_mgr.get_nb_scenarios()];

						for (int i=0;i<scen_mgr.get_nb_scenarios();i++)
						{
							inforce_sum[i]=new string*[inforce_mgr.nb_model_points];
							for (int j=0;j<inforce_mgr.nb_model_points;j++)
							{
								inforce_sum[i][j]=new string[inforce_mgr.number_of_columns];
							}
						}

						for (int i=0;i<scen_mgr.get_nb_scenarios();i++)
						{
						 this->projection->initialize_copied_inforce(inforce_mgr.get_inforce(),inforce_sum[i],inforce_mgr.number_of_columns,inforce_mgr.nb_model_points);
						}
					}

				}
				else
				{
				 RA_flg[prod_configuration_number]=true;
				 continue;
				}

				// Loop over all the model points 
				for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
				{
					//cout<<"Projecting Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<" Model Point "<<mp_number+1<<endl;
					this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
					this->projection->initialise_index_map();
					Projection::compt_sen =0;
					this->projection->clear_ITM_RA();
					
					// Loop over all the scenarios
					for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
					{
						this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
						this->projection->project();

						// Risk Appetite
						if (this->projection->get_risk_appetite()=="Y")
						{
                          this->projection->set_recup_ITM_RA(this->projection->get_t_RA(), scen_nb, mp_number);
						  if (this->projection->get_output()==1)
						  {this->projection->get_older_policy(this->projection->get_t_RA(),mp_number,inforce_sum[scen_nb],1., 1);}
						}						
					}

                    if (this->projection->get_risk_appetite()=="Y")
					{	
						double perc_1(this->projection->get_perc_1());
						double perc_2(this->projection->get_perc_2());
						double perc_3(this->projection->get_perc_3());

						this->projection->sort_ITM_RA();
						this->projection->get_older_policy(this->projection->get_t_RA(),mp_number,inforce_1,perc_1,0);
						this->projection->get_older_policy(this->projection->get_t_RA(),mp_number,inforce_2,perc_2,0);
						this->projection->get_older_policy(this->projection->get_t_RA(),mp_number,inforce_3,perc_3,0);
						
                     }
				}

				if (this->projection->get_risk_appetite()=="Y")
				{
					string output_path=product.get_model_point_file();
                    int position=output_path.find_last_of('\\');
                    this->write_inforce_RA(inforce_1,inforce_2,inforce_3,product.get_run_name(),inforce_mgr.nb_model_points,inforce_mgr.number_of_columns,"Risk Appetite",output_path.substr(0,position));

					if (this->projection->get_output()==1)
					{
						this->write_inforce_all(inforce_sum,product.get_run_name(),scen_mgr.get_nb_scenarios(),inforce_mgr.nb_model_points, inforce_mgr.number_of_columns, "RAF", output_path.substr(0,position)) ;
					//to do: clear inforce_sum
						for (int i=0;i<scen_mgr.get_nb_scenarios();i++)
						{
							
							for (int j=0;j<inforce_mgr.nb_model_points;j++)
							{
								delete[] inforce_sum[i][j];
							}
							delete[] inforce_sum[i];
						}
						delete[] inforce_sum;

					}
					for (int i=0;i< inforce_mgr.nb_model_points; i++)
					{	delete[] inforce_1[i];
						delete[] inforce_2[i];
						delete[] inforce_3[i];
					}
					delete[] inforce_1;
					delete[] inforce_2;
					delete[] inforce_3;


				}
			}
		}

		// run projection dll file mode entire function
		number_of_products_to_run = product_config.size();
		t_high_shorter = -1;

		for (int prod_configuration_number = 0; prod_configuration_number < number_of_products_to_run; prod_configuration_number++)
		{
			// Get the product configuration
			//cout<<"Pricing Product "<<prod_configuration_number+1<<" with generated RA inforce"<<endl;
			if (RA_flg[prod_configuration_number]==1)
				continue;

			Product_configuration product = product_config[prod_configuration_number];
			product.RA_set_inforce_name();
			product.RA_set_run_name();

			// Get the scenario list
			vector<Scenario_configuration> list_of_scenarios = product.get_scenario_list();

			// Initialize the scenario manager
			// ---- Suppose that: all the scenario files used for the same product have the same maturity
			scenario_manager scen_mgr(product.get_number_of_scenarios_to_run(),
				product.get_scenario_index_columns(), 
				product.get_yield_columns_for_pv(),
				product.get_lapse_dyn_treas(),
				product.get_rates_indic_index(),
				product.get_index_number(),
				t_high_shorter,
				list_of_scenarios[0].get_scenario_file_name(),
				product.get_time_step()
				); 

			int number_of_scenarios_to_run = list_of_scenarios.size();
			for (int scenario_number = 0; scenario_number < number_of_scenarios_to_run; scenario_number++)
			{
				Scenario_configuration scen_configuration = list_of_scenarios[scenario_number];
				//cout<<"Loading Scenario File for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;

				scen_mgr.init_scenario_manager(scen_configuration.get_scenario_file_name(), 1);
				
                //cout<<"Creating inforce for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;
				// Create the Inforce manager
				Inforce_Manager inforce_mgr(
					',',
					tot_nb_workers,
					w_id,
					product,
					scen_configuration,
					false,
					false,
					true
					);

				// Create the projection
				//cout<<"Creating payoff for Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<endl;
				this->create_payoff(product.get_prod_code_key(), product.get_config_file().c_str(), 
									scen_configuration.get_index_shock_array(), 
									scen_configuration.get_fund_shock_array(),
									product.get_generate_debug_file_indic(),
									product.get_debug_file_path(),
									t_high_shorter,
									scen_configuration.get_load_longevity_table(),
									product.get_time_step()
									);

				this->projection->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario()));

				//// Set the lapse multiplicator tables end the mortality multiplicator tables
				this->projection->set_lapse_mult_table(scen_configuration.get_lapse_mult());
				this->projection->set_mortality_mult_table(scen_configuration.get_mortality_mult());
				this->projection->set_paidup_mult_table(scen_configuration.get_paidup_mult());

				// Loop over all the model points 
				for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
				{
					//cout<<"Projecting Product "<<prod_configuration_number+1<<" Scenario "<<scenario_number+1<<" Model Point "<<mp_number+1<<endl;
					this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
					this->projection->initialise_index_map();
					Projection::compt_sen = 0;
					
					// Loop over all the scenarios
					for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
					{
						this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
						this->projection->project();
					}
 				}

				//cout<<"Writing RA Final Results Product "<<prod_configuration_number+1<<endl;
				write_results(this->projection,
					product.get_run_name(),
					scen_configuration.get_index_shock_array(), 
					scen_configuration.get_fund_shock_array(), 
					inforce_mgr.get_model_points_vector(), 
					product.get_result_file_path(),
					w_id, 
					scen_configuration.get_scenario_pass_number(), 
					product.get_number_of_scenarios_to_run()/product.get_modulo_number_on_scenario(), 
					scen_configuration.get_index_shock_array().size(),
					product.get_write_valo_policy_by_policy(), 
					-1, //alpha
					-1, //sigma
					scen_configuration
					);		
			}
		}
	}


	/*
	* Projection by the DLL with a scenario as input
	*/
	void run_dll_projection_file(vector<Product_configuration>& product_config, int tot_nb_workers, int w_id, double alpha, double sigma)
	{
		int number_of_products_to_run = product_config.size();
		int t_high_shorter = -1;

		for (int prod_configuration_number = 0; prod_configuration_number < number_of_products_to_run; prod_configuration_number++)
		{
			// Get the product configuration
			Product_configuration product = product_config[prod_configuration_number];

			// Get the scenario list
			vector<Scenario_configuration> list_of_scenarios = product.get_scenario_list();

			// Initialize the scenario manager
			// ---- Suppose that: all the scenario files used for the same product have the same maturity
			scenario_manager scen_mgr(product.get_number_of_scenarios_to_run(),
				product.get_scenario_index_columns(), 
				product.get_yield_columns_for_pv(),
				product.get_lapse_dyn_treas(),
				product.get_rates_indic_index(),
				product.get_index_number(),
				t_high_shorter,
				list_of_scenarios[0].get_scenario_file_name(),
				product.get_time_step()); 

			int number_of_scenarios_to_run = list_of_scenarios.size();
			for (int scenario_number = 0; scenario_number < number_of_scenarios_to_run; scenario_number++)
			{
				Scenario_configuration scen_configuration = list_of_scenarios[scenario_number];

				scen_mgr.init_scenario_manager(scen_configuration.get_scenario_file_name(), 1);

				// Create the Inforce manager
				Inforce_Manager inforce_mgr(
					',',
					tot_nb_workers,
					w_id,
					product,
					scen_configuration,
					false,
					false,
					false
					);

				// Create the projection
				this->create_payoff(product.get_prod_code_key(), product.get_config_file().c_str(), 
									scen_configuration.get_index_shock_array(), 
									scen_configuration.get_fund_shock_array(),
									product.get_generate_debug_file_indic(),
									product.get_debug_file_path(),
									t_high_shorter,
									scen_configuration.get_load_longevity_table(),
									product.get_time_step()
									);

				this->projection->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario()));

				//// Set the lapse multiplicator tables end the mortality multiplicator tables
				this->projection->set_lapse_mult_table(scen_configuration.get_lapse_mult());
				this->projection->set_mortality_mult_table(scen_configuration.get_mortality_mult());
				this->projection->set_paidup_mult_table(scen_configuration.get_paidup_mult());
				this->projection->set_election_mult_table(scen_configuration.get_election_mult());
				//!  for scen_by_scen output
				if(product.get_write_valo_policy_by_policy()==2)
					this->projection->set_pvsScenOutput(*(inforce_mgr.get_model_points_vector()[0]),scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario());

				/*
				* Partie pour LS et frontire
				*/

				this->projection->init_longstaff_parameters(product.is_longstaff());
				
				string debug_file = product.get_debug_file();
				string proba_file = product.get_proba_file();
				string proba_ls_file = product.get_proba_ls_file();

				string longstaff_debug_info = this->projection->get_longstaff_debug_info();
				string longstaff_proba_info = this->projection->get_longstaff_proba_info();

				if (this->projection->get_longstaff_pricer()=="Y"){
					if(longstaff_debug_info == "Y"){
						write_debug_file(debug_file);
						write_proba_file(proba_file);
					}
					if(longstaff_debug_info == "USE"){
						read_debug_file(debug_file);
						write_proba_file(proba_file);
					}
					if(longstaff_proba_info == "Y")
						write_proba_LS_file(proba_ls_file);
					if(longstaff_proba_info == "USE")
						read_proba_LS_file(proba_ls_file);
				}
				// Loop over all the model points 
				for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
				{
					this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
					this->projection->initialise_index_map();
					Projection::compt_sen = 0;
					string longstaff_ind = this->projection->get_longstaff_pricer();
					
					// Loop over all the scenarios
					if(this->projection->get_longstaff_pricer()=="Y"){
						this->projection->initialise_LS(scen_mgr.get_nb_scenarios());
						
					}
					for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
					{
						if (projection->get_time_step() == ANNUAL) 
							projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
						else
							projection->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);

						//this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
						this->projection->project();
						//! for scen_by_scen output
						if(product.get_write_valo_policy_by_policy()==2)
									this->projection->update_pvsScenOutput(scen_nb,scen_mgr.get_nb_scenarios(),product.get_modulo_number_on_scenario());
					}
					if(this->projection->get_longstaff_pricer()=="Y"){
						this->projection->terminate_LS(mp_number);
					}
 				}
				this->projection->file_stock.close();
				if(longstaff_debug_info == "Y"){
					this->projection->file_stock2.close();
				}
				if(longstaff_debug_info == "USE"){
					this->projection->file_stock3.close();
				}
				if(longstaff_proba_info == "USE"){
					this->projection->file_stock4.close();
				}
				if(longstaff_proba_info == "Y"){
					this->projection->file_stock5.close();
 				}

				write_results(this->projection, product.get_run_name(),
					scen_configuration.get_index_shock_array(), 
					scen_configuration.get_fund_shock_array(), 
					inforce_mgr.get_model_points_vector(), 
					product.get_result_file_path(),
					w_id, 
					scen_configuration.get_scenario_pass_number(), 
					product.get_number_of_scenarios_to_run()/product.get_modulo_number_on_scenario(), 
					scen_configuration.get_index_shock_array().size(),
					product.get_write_valo_policy_by_policy(), 
					alpha, 
					sigma,
					scen_configuration
					);		
			}
		}
	}


	void run_dll_projection_esg(vector<Product_configuration>& product_config,
								ofstream& log_strm,
								int tot_nb_workers,
								int w_id,
								// int total_period,
								Dispatch_Manager& dispatch_manager)	
	{


		double b = clock();

		map<string, Inforce_Manager*> map_infMgr;
		vector<Projection*> v_of_projections;

		Inforce_Manager::Create_Inforce_Managers(',',
			tot_nb_workers,
			w_id,
			product_config,
			product_config[0].get_scenario_list()[0], false ,false,false, 
			map_infMgr);

		double e = clock();

		log_strm << endl << "Time for inforce reading : " << e - b  << endl; 


		b = clock();

		for (int i = 0; i <  product_config.size(); i++)
		{
			Product_configuration product = product_config[i];

			bool load_longevity_table = false;
			vector<Scenario_configuration> v_scn = product.get_scenario_list();

			for (int scn_i = 0; scn_i < v_scn.size(); scn_i++)
			{
				if (v_scn[scn_i].get_load_longevity_table())
				{
					load_longevity_table = true;
					break;			
				}
			
			}
			vector<vector <int>> index_shock_array;
			vector<vector <int>> fund_shock_array;

			v_of_projections.push_back(create_projection(product.get_prod_code_key(),
											product.get_config_file().c_str(), 
											index_shock_array,
											fund_shock_array,
											product.get_generate_debug_file_indic(), product.get_debug_file_path(),
											60, load_longevity_table,
											product.get_time_step()));
		
		}

		e = clock();

		log_strm << "Time for projection creation : " << e - b  << endl; 

		//Dispatch_Manager dispatch_manager(product_config);
		Scenario_list scenario_list_table = dispatch_manager.get_Scenario_list_reference();
		//scenario_list_table.print();

		Bucket_list   bucket_list_table   = dispatch_manager.get_Bucket_list_reference();

		vector<Regroup_by_Scenario_PN> regrouped_scenario_product(dispatch_manager.get_regrouped_scenario_pns());
		vector<int>         bucket_id_list_;
		vector<vector<int>>	product_index_list_;  
       
		for(size_t itr_regrouped = 0; itr_regrouped < regrouped_scenario_product.size(); itr_regrouped++)
		{
			int scen_pn = regrouped_scenario_product[itr_regrouped].get_scenario_pn();

			string config_ESG(scenario_list_table.get_ESG_config_path(scen_pn)); 

			InterfaceCalibration iCalib( config_ESG );
			InterfaceEsg iEsg(config_ESG);

		    vector<int>         bucket_id_list_(regrouped_scenario_product[itr_regrouped].get_bucket_id_list_reference());
			vector<vector<int>>	product_index_list_(regrouped_scenario_product[itr_regrouped].get_product_index_list_reference());   

			int t_high_shorter;

			int total_period = (iEsg.year * iEsg.delta_t) + 1;

			if((total_period-1)%12!=0 || total_period > 721)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Error in function int scenario_manager :: init_t_high_shorter, (file_mode) (total_period-1)%12!=0 || total_period > 721" );
				throw new GMXBException(message);
			}
			else
			{
				t_high_shorter = (total_period-1)/12;
			}


			double a_baseline = -99999;
			double sigma_baseline = -99999;

		for(unsigned int iterator_total_bucket_list = 0; iterator_total_bucket_list< bucket_id_list_.size(); iterator_total_bucket_list++)
		{
			int id_total_bucket_list = bucket_id_list_[iterator_total_bucket_list];

			string bucket_type = bucket_list_table.get_bucket_list_type(id_total_bucket_list);
			
			vector<double> bucekt_contenance_temp = bucket_list_table.get_bucket_list_contenance(id_total_bucket_list);

			bool  bucket_if_calib = bucket_list_table.get_bucket_list_if_calib(id_total_bucket_list);

			iCalib.ShockSwap(bucekt_contenance_temp[0],
							 bucekt_contenance_temp[1],
							 bucekt_contenance_temp[2],
							 bucekt_contenance_temp[3],
							 bucket_type );

				double aCalib= -99999;
				double sigmaCalib = -99999;
			//! keep the "a && sigma" for the baseline
			if(id_total_bucket_list != 1 && !bucket_if_calib)
			{
				aCalib = a_baseline;
				sigmaCalib = sigma_baseline;
			}

			log_strm << endl << "Begining of scenario generation number :  " << id_total_bucket_list  << endl;
			
			b = clock();
				
			double *** va_scen = iEsg.Gen(iCalib,
											  aCalib,
											  sigmaCalib,
											  scen_pn,
											  bucket_if_calib,
											  id_total_bucket_list, w_id); 

			e = clock();

			log_strm << "End of scenario generation number :  " << id_total_bucket_list << " time : " << e - b  << endl;

			if (id_total_bucket_list == 1)
			{
				a_baseline = aCalib ;
				sigma_baseline = sigmaCalib ;
			}


			// iterate the list of products, whose use iterator_total_bucket_list
			for (unsigned int iterator_product_index = 0; iterator_product_index < product_index_list_[iterator_total_bucket_list].size(); iterator_product_index++)
			{
				// Get the product configuration
				Product_configuration product = product_config[ product_index_list_[iterator_total_bucket_list][iterator_product_index] ];  
                  
			    // Check if the list of scenario contain the passed scenario
			    scenario_manager scen_mgr(  va_scen,
											product.get_number_of_scenarios_to_run(),
											product.get_scenario_index_columns(), 
											product.get_yield_columns_for_pv(),
											product.get_lapse_dyn_treas(),
											product.get_rates_indic_index(),
											product.get_index_number(),
											t_high_shorter,
											product.get_time_step());

				bool if_he_inner_mode = false;
				scen_mgr.init_scenario_manager_ESG(1, if_he_inner_mode);

				// Initialize the table of shocks
				//vector<Scenario_configuration> scen_configuration(product.get_scenario_list()); // copy constructor
				vector<Scenario_configuration> scen_configuration;
				vector<Scenario_configuration> p_v_scen = product.get_scenario_list();
                for(int i_iterator=0; i_iterator<product.get_scenario_list().size(); ++i_iterator)
				{
					Scenario_configuration scn = p_v_scen[i_iterator];
                    if(scn.get_scenario_pass_number() == scen_pn)
						scen_configuration.push_back(scn);
				}
				
				for(unsigned int iterator_bucket_of_SF = 0; iterator_bucket_of_SF<scen_configuration.size(); iterator_bucket_of_SF++)
				{
					int id_bucket_of_SF = scen_configuration[iterator_bucket_of_SF].get_bucket_id();
					if(id_total_bucket_list == id_bucket_of_SF)
					{
						/*

						Inforce_Manager inforce_mgr(',',
													tot_nb_workers,
													w_id,
													product,
													scen_configuration[iterator_bucket_of_SF], false ,false,false
													);
													*/

						Inforce_Manager *inforce_mgr =  map_infMgr[product.get_run_name()];

						log_strm << "    Beg Processing run : " << product.get_run_name() << " for bucket id " << id_bucket_of_SF << endl;
						
						b = clock();
						// Remove create payoff, set t_high, set longevity , index shock array + fund_shock_array

						Projection * proj = v_of_projections[product_index_list_[iterator_total_bucket_list][iterator_product_index]];
						proj->set_total_number_scn((int)(scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario()));
						proj->set_t_high(t_high_shorter);
						proj->set_index_shock_array(scen_configuration[iterator_bucket_of_SF].get_index_shock_array());
						proj->set_fund_shock_array(scen_configuration[iterator_bucket_of_SF].get_fund_shock_array());
						proj->set_longevity_table_indic(scen_configuration[iterator_bucket_of_SF].get_load_longevity_table());



						// Set the lapse multiplicator tables end the mortality multiplicator tables
						proj->set_lapse_mult_table(scen_configuration[iterator_bucket_of_SF].get_lapse_mult());
						proj->set_mortality_mult_table(scen_configuration[iterator_bucket_of_SF].get_mortality_mult());
						proj->set_paidup_mult_table(scen_configuration[iterator_bucket_of_SF].get_paidup_mult());

						log_strm << "    Model point size : " << inforce_mgr->get_model_points_vector().size() << endl;

						for (int k = 0; k < inforce_mgr->get_model_points_vector().size(); k++)
						{
							Model_Point * mp = inforce_mgr->get_model_points_vector()[k];
							mp->set_pv_index_shock_array(scen_configuration[iterator_bucket_of_SF]);
						}

						// Loop over all the model points 
						//! suppse all the mps are homogene (same size of pvs)
						//! initialize table in Projection.h for pvs_output
						//! furture improvement: if we have the same nScenPath, we can get it out out the scen-loop
						if(product.get_write_valo_policy_by_policy()==2)
							proj->set_pvsScenOutput(*(inforce_mgr->get_model_points_vector()[0]),scen_mgr.get_nb_scenarios()/product.get_modulo_number_on_scenario());
						/*
						* Partie pour LS et frontire
						*/


						proj->init_longstaff_parameters(product.is_longstaff());
				
						string debug_file = product.get_debug_file();
						string proba_file = product.get_proba_file();
						string proba_ls_file = product.get_proba_ls_file();

						string longstaff_debug_info = proj->get_longstaff_debug_info();
						string longstaff_proba_info = proj->get_longstaff_proba_info();

						if (proj->get_longstaff_pricer()=="Y"){
							if(longstaff_debug_info == "Y"){
								write_debug_file(debug_file);
								write_proba_file(proba_file);
							}
							if(longstaff_debug_info == "USE"){
								read_debug_file(debug_file);
								write_proba_file(proba_file);
							}
							if(longstaff_proba_info == "Y")
								write_proba_LS_file(proba_ls_file);
							if(longstaff_proba_info == "USE")
								read_proba_LS_file(proba_ls_file);
						}
						for( int mp_number=0; mp_number < inforce_mgr->nb_model_points ; mp_number++ )
						{
							proj->set_current_model_point(inforce_mgr->get_model_points_vector()[mp_number]);
							proj->initialise_index_map();
							Projection::compt_sen = 0;
							string longstaff_ind = proj->get_longstaff_pricer();
							
							// Loop over all the scenarios
							if(proj->get_longstaff_pricer()=="Y"){
								proj->initialise_LS(scen_mgr.get_nb_scenarios());}
						
							for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
							{
								//ALI CHANGE
								if (proj->get_time_step() == ANNUAL)
									proj->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
								else
									proj->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);
								proj->project();
								//! Y -- paidup is not considered here! 
								if(product.get_write_valo_policy_by_policy()==2)
									proj->update_pvsScenOutput(scen_nb,scen_mgr.get_nb_scenarios(),product.get_modulo_number_on_scenario());
							}
							if(proj->get_longstaff_pricer()=="Y"){
								proj->terminate_LS(mp_number);
							}
						}

						proj->file_stock.close();
						if(longstaff_debug_info == "Y"){
							proj->file_stock2.close();
						}
						if(longstaff_debug_info == "USE"){
							proj->file_stock3.close();
						}
						if(longstaff_proba_info == "USE"){
							proj->file_stock4.close();
						}
						if(longstaff_proba_info == "Y"){
							proj->file_stock5.close();
						}

						write_results(proj, 
							    product.get_run_name(), 
								scen_configuration[iterator_bucket_of_SF].get_index_shock_array(), 
								scen_configuration[iterator_bucket_of_SF].get_fund_shock_array(), 
								inforce_mgr->get_model_points_vector(), 
								product.get_result_file_path(),
								w_id, 
								scen_pn,
								product.get_number_of_scenarios_to_run()/product.get_modulo_number_on_scenario(), 
								scen_configuration[iterator_bucket_of_SF].get_index_shock_array().size(), 
								product.get_write_valo_policy_by_policy(),
								aCalib, 
								sigmaCalib,
								scen_configuration[iterator_bucket_of_SF],
								id_total_bucket_list); 

						e = clock();
						log_strm << "    End Processing run : " << product.get_run_name() << " for bucket id " << id_bucket_of_SF << " Time : " << e - b << endl;

						}
					}
				}
			}
		}

		// Delete all projections

		int v_of_projection_size = v_of_projections.size();
		for (int p = 0 ; p < v_of_projection_size; p++)
			delete v_of_projections[p];

		// Delete all inforce managers

		for (map<string, Inforce_Manager*>::iterator it_inf_mgr = map_infMgr.begin();
			it_inf_mgr!= map_infMgr.end(); ++it_inf_mgr)
		{
			Inforce_Manager * if_mgr = it_inf_mgr->second;
			delete if_mgr;
		}

	}
	/*
	* Projection and ESG by the DLL bup
	*/
	void run_dll_projection_esg_bup(vector<Product_configuration>& product_config,
								ofstream& log_strm,
								int tot_nb_workers,
								int w_id,
								// int total_period,
								Dispatch_Manager& dispatch_manager)	
	{

		//Dispatch_Manager dispatch_manager(product_config);
		Scenario_list scenario_list_table = dispatch_manager.get_Scenario_list_reference();
		//scenario_list_table.print();

		Bucket_list   bucket_list_table   = dispatch_manager.get_Bucket_list_reference();

		vector<Regroup_by_Scenario_PN> regrouped_scenario_product(dispatch_manager.get_regrouped_scenario_pns());
		vector<int>         bucket_id_list_;
		vector<vector<int>>	product_index_list_;  
       
        //scenario_list_table.print();
		for(size_t itr_regrouped = 0; itr_regrouped < regrouped_scenario_product.size(); itr_regrouped++)
		{
			int scen_pn = regrouped_scenario_product[itr_regrouped].get_scenario_pn();

			string config_ESG(scenario_list_table.get_ESG_config_path(scen_pn)); 
            //string config_calib(scenario_list_table.get_Calibration_config_path(scen_pn));

			//cout <<  "***********  " <<  config_ESG  << endl;
			InterfaceCalibration iCalib( config_ESG );
			InterfaceEsg iEsg(config_ESG);

		    vector<int>         bucket_id_list_(regrouped_scenario_product[itr_regrouped].get_bucket_id_list_reference());
			vector<vector<int>>	product_index_list_(regrouped_scenario_product[itr_regrouped].get_product_index_list_reference());   


			int t_high_shorter;

			int total_period = (iEsg.year * iEsg.delta_t) + 1;

			if((total_period-1)%12!=0 || total_period > 721)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Error in function int scenario_manager :: init_t_high_shorter, (file_mode) (total_period-1)%12!=0 || total_period > 721" );
				throw new GMXBException(message);
			}
			else
			{
				t_high_shorter = (total_period-1)/12;
			}


			double a_baseline = -99999;
			double sigma_baseline = -99999;


		for(unsigned int iterator_total_bucket_list = 0; iterator_total_bucket_list< bucket_id_list_.size(); iterator_total_bucket_list++)
		{
			int id_total_bucket_list = bucket_id_list_[iterator_total_bucket_list];

			string bucket_type = bucket_list_table.get_bucket_list_type(id_total_bucket_list);
			
			vector<double> bucekt_contenance_temp = bucket_list_table.get_bucket_list_contenance(id_total_bucket_list);

			bool  bucket_if_calib = bucket_list_table.get_bucket_list_if_calib(id_total_bucket_list);

			iCalib.ShockSwap(bucekt_contenance_temp[0],
							 bucekt_contenance_temp[1],
							 bucekt_contenance_temp[2],
							 bucekt_contenance_temp[3],
							 bucket_type );

				double aCalib= -99999;
				double sigmaCalib = -99999;
				

			//! keep the "a && sigma" for the baseline
			if(id_total_bucket_list != 1 && !bucket_if_calib)
			{
				aCalib = a_baseline;
				sigmaCalib = sigma_baseline;
			}

			double *** va_scen = iEsg.Gen(iCalib,
											  aCalib,
											  sigmaCalib,
											  scen_pn,
											  bucket_if_calib,
											  id_total_bucket_list, w_id); 

			if (id_total_bucket_list == 1)
			{
				a_baseline = aCalib ;
				sigma_baseline = sigmaCalib ;
			}


			// iterate the list of products, whose use iterator_total_bucket_list
			for (unsigned int iterator_product_index = 0; iterator_product_index < product_index_list_[iterator_total_bucket_list].size(); iterator_product_index++)
			{
				// Get the product configuration
				Product_configuration product = product_config[ product_index_list_[iterator_total_bucket_list][iterator_product_index] ];  
                  
			    // Check if the list of scenario contain the passed scenario
			    scenario_manager scen_mgr(  va_scen,
											product.get_number_of_scenarios_to_run(),
											product.get_scenario_index_columns(), 
											product.get_yield_columns_for_pv(),
											product.get_lapse_dyn_treas(),
											product.get_rates_indic_index(),
											product.get_index_number(),
											t_high_shorter,
											product.get_time_step());

				bool if_he_inner_mode = false;
				scen_mgr.init_scenario_manager_ESG(1, if_he_inner_mode);

				// Initialize the table of shocks
				vector<Scenario_configuration> scen_configuration(product.get_scenario_list()); // copy constructor
				
				for(unsigned int iterator_bucket_of_SF = 0; iterator_bucket_of_SF<scen_configuration.size(); iterator_bucket_of_SF++)
				{
					int id_bucket_of_SF = scen_configuration[iterator_bucket_of_SF].get_bucket_id();
					if(id_total_bucket_list == id_bucket_of_SF)
					{
						Inforce_Manager inforce_mgr(',',
													tot_nb_workers,
													w_id,
													product,
													scen_configuration[iterator_bucket_of_SF], false, false, false 
													);

						this->create_payoff(product.get_prod_code_key(),
											product.get_config_file().c_str(), 
											scen_configuration[iterator_bucket_of_SF].get_index_shock_array(),
											scen_configuration[iterator_bucket_of_SF].get_fund_shock_array(),
											product.get_generate_debug_file_indic(), product.get_debug_file_path(),
											t_high_shorter, scen_configuration[iterator_bucket_of_SF].get_load_longevity_table(),
											product.get_time_step());

						// Set the lapse multiplicator tables end the mortality multiplicator tables
						this->projection->set_lapse_mult_table(scen_configuration[iterator_bucket_of_SF].get_lapse_mult());
						this->projection->set_mortality_mult_table(scen_configuration[iterator_bucket_of_SF].get_mortality_mult());
						this->projection->set_paidup_mult_table(scen_configuration[iterator_bucket_of_SF].get_paidup_mult());
						this->projection->set_election_mult_table(scen_configuration[iterator_bucket_of_SF].get_election_mult());

						// Loop over all the model points 
						for( int mp_number=0; mp_number < inforce_mgr.nb_model_points ; mp_number++ )
						{
							this->projection->set_current_model_point(inforce_mgr.get_model_points_vector()[mp_number]);
							this->projection->initialise_index_map();
							Projection::compt_sen = 0;
							
							// Loop over all the scenarios
							for(int scen_nb=mp_number%product.get_modulo_number_on_scenario(); scen_nb < scen_mgr.get_nb_scenarios() ; scen_nb = scen_nb + product.get_modulo_number_on_scenario())
							{
								//ALI CHANGE
								if (this->projection->get_time_step() == ANNUAL)
									this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
								else
									this->projection->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);

								//this->projection->set_annual_from_monthly(scen_mgr.get_ann_from_mthly_rates(scen_nb+1), scen_nb+1);
								//this->projection->set_monthly_rates(scen_mgr.get_monthly_rates(scen_nb+1), scen_nb+1);
								this->projection->project();
							}
						}

						write_results(projection,
								product.get_run_name(), 
								scen_configuration[iterator_bucket_of_SF].get_index_shock_array(), 
								scen_configuration[iterator_bucket_of_SF].get_fund_shock_array(), 
								inforce_mgr.get_model_points_vector(), 
								product.get_result_file_path(),
								w_id, 
								scen_pn,
								product.get_number_of_scenarios_to_run()/product.get_modulo_number_on_scenario(), 
								scen_configuration[iterator_bucket_of_SF].get_index_shock_array().size(), 
								product.get_write_valo_policy_by_policy(),
								aCalib, 
								sigmaCalib,
								scen_configuration[iterator_bucket_of_SF],
								id_total_bucket_list); 		
						}
					}
				}
			}
		}
	}


	
	void check_projection_consistency(vector<Product_configuration>& product_config_out,
		vector<Product_configuration>& product_config_in)
	{

		if (product_config_out.size() == 0)
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "No projection outer to run");
				throw new GMXBException(message);
		}
		
		int number_of_projection_out = product_config_out.size();
		int number_of_projection_in = product_config_in.size();

		if (number_of_projection_out != number_of_projection_in)
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "The number of projection inner and outer are different");
				throw new GMXBException(message);
		}
		for (int i = 0; i < number_of_projection_out; i++)
		{
			Product_configuration prod_out = product_config_out[i];
			bool found = false;
			for (int j =0; j < number_of_projection_in; j++)
			{
				Product_configuration prod_in = product_config_in[j];

				if (prod_in.get_prod_code_key() == prod_out.get_prod_code_key())
				{
					found = true;
					break;
				}
			}
			if (found == false)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "No projection inner found for projection outre : %s" , prod_out.get_run_name().c_str());
				throw new GMXBException(message);
			}
		}

		// Check that the number of scenarios
		// is the same in all the outer projections
		int number_of_outer_scenarios = product_config_out[0].get_number_of_scenarios_to_run();

		for (int i = 0; i < number_of_projection_out; i++)
		{
			Product_configuration prod_out = product_config_out[i];
			if (prod_out.get_number_of_scenarios_to_run() != number_of_outer_scenarios)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "All the projection outer should have the same number of scenarios");
				throw new GMXBException(message);			
			}
		}

		// Check that all the mappings
		// index_columns are the same for all the puter scenarios

		Product_configuration product_0 = product_config_out[0];

		for (int i = 1; i < number_of_projection_out; i++)
		{

			Product_configuration product_i = product_config_out[i];
			// Compare the yield columns for pv
			if (product_i.get_yield_columns_for_pv() != product_0.get_yield_columns_for_pv())
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Different discount factor configuration between %s and %s ", 
										product_i.get_run_name().c_str(), product_0.get_run_name().c_str() );
				throw new GMXBException(message);
			}

			// Compare the lapse dyn treas
			if (product_i.get_lapse_dyn_treas() != product_0.get_lapse_dyn_treas())
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Different lapse dyn treas configuration between %s and %s ", 
										product_i.get_run_name().c_str(), product_0.get_run_name().c_str() );
				throw new GMXBException(message);
			}

			// Compare the scen fund columns
			int index_columns_size = product_0.get_scenario_index_columns().size();
			for (int j = 0; j < index_columns_size; j++)
			{
				if (product_i.get_scenario_index_columns()[j] != product_0.get_scenario_index_columns()[j])
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Different mapping scen fund column between %s and %s ", 
										product_i.get_run_name().c_str(), 
										product_0.get_run_name().c_str());
					throw new GMXBException(message);
				}
			}
			// TO DO - Check the scenario list


			// Check the list of bucket for the projection in
			Product_configuration product_config_in_0 = product_config_in[0];
			int number_of_scenario_in_0 = product_config_in_0.get_scenario_list().size();

			for (unsigned int nb_product_in = 1; nb_product_in < product_config_in.size() ; nb_product_in++)
			{
				Product_configuration product_config_in_i = product_config_in[nb_product_in];
				if (number_of_scenario_in_0 != product_config_in_i.get_scenario_list().size())
				{

					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Different number of buckets to launch between %s and %s ", 
										product_config_in_0.get_run_name().c_str(), 
										product_config_in_i.get_run_name().c_str());
					throw new GMXBException(message);
				}
			}


			for (int nb_bucket_in = 0 ; nb_bucket_in < number_of_scenario_in_0; nb_bucket_in++)
			{

				int bucket_id_0 = product_config_in_0.get_scenario_list()[nb_bucket_in].get_bucket_id();

				for (unsigned int nb_product_in = 1; nb_product_in < product_config_in.size() ; nb_product_in++)
				{
					Product_configuration product_config_in_i = product_config_in[nb_product_in];

					int bucket_id_i = product_config_in_i.get_scenario_list()[nb_bucket_in].get_bucket_id();
					
					if (bucket_id_0 != bucket_id_i)
					{

						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "The bucket id %d is not found in  %s ",  bucket_id_0, 
										product_config_in_i.get_run_name().c_str());
						throw new GMXBException(message);

					}
				}
			}
		}
	}

	// TODO Optimize this function
	int get_total_number_of_policies(Summary_configuration& summary)
	{
		int total_number_of_policies = 0;
		vector<Product_configuration> products = summary.get_product_configurations();
		int number_of_products = products.size();
		if (number_of_products > 0)
		{
			for(int p_nb = 0; p_nb < number_of_products; p_nb++)
			{
				Product_configuration product = products[p_nb];

				Inforce_Manager inforce_mgr(
						',',
						1 ,
						1, 
						product,
						product.get_scenario_list()[0], false, false,false
						);

				total_number_of_policies += inforce_mgr.get_model_points_vector().size();
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "No products to summarize");
			throw new GMXBException(message);		
		}
		return total_number_of_policies;
	}


		
	void check_summary_consistency(Summary_configuration& summary)
	{
		vector<Product_configuration> products = summary.get_product_configurations();
		int number_of_products = products.size();
		// Check if the sumary contains at least one 
		// run to summarize
		if (number_of_products <= 0)
		{	
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Summarization checker: Number of products to summarize inconsistency");
			throw new GMXBException(message);		
		}
		// Check if the inforce files are the same
		string inforce_file = products[0].get_model_point_file();
		for(int nb_product = 1; nb_product < number_of_products; nb_product++)
		{
			string inforce_file_name = products[nb_product].get_model_point_file();
			if (inforce_file_name != inforce_file)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization checker: Inforce file name inconsistency. Found %s for run %s and %s for run %s"
					, inforce_file.c_str(), products[0].get_run_name().c_str(), inforce_file_name.c_str(),  products[nb_product].get_run_name().c_str() );
				throw new GMXBException(message);			
			}
		}
		// Check the scenario number
		int number_of_scenario =  products[0].get_scenario_list().size();
		if (number_of_scenario <= 0)
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization checker: Size of the scenario inconsistency");
				throw new GMXBException(message);		
		}

		for(int nb_product = 1; nb_product < number_of_products; nb_product++)
		{
			int scen_numb = products[nb_product].get_scenario_list().size();
			if (scen_numb != number_of_scenario)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Summarization checker: Number of scenario inconsistency");
				throw new GMXBException(message);		
			}
		}

		// Check the content of each scenario
		for (int nb_scen = 0; nb_scen < number_of_scenario; nb_scen++)
		{
			Scenario_configuration scenario_config_0 = products[0].get_scenario_list()[nb_scen];
			// check the scenario pass number
			int scenario_pass_number_0 = scenario_config_0.get_scenario_pass_number();
			vector<vector<int>> s_array_0 = scenario_config_0.get_index_shock_array();
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
					sprintf_s(message, "Summarization checker: Inconsistency. The scenario pass num %d is not found for product %s ", 
						scenario_pass_number_0, products[nb_product].get_run_name().c_str());
					throw new GMXBException(message);				
				}

				// The scenario pass num is found. We check the shock arrays
				// Check shock array size
				vector<vector<int>> s_array = scenario.get_index_shock_array();
				int s_array_size = s_array.size();
				if (s_array_0_size != s_array_size)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Summarization checker: Shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
						products[0].get_run_name().c_str(), products[nb_product].get_run_name().c_str(), scenario_pass_number_0);
					throw new GMXBException(message);
				}
				// Check the content of the shock array
				for(int s_size = 0; s_size < s_array_0_size; s_size++)
				{
					vector<int> index_shocks_0 = s_array_0[s_size];
					vector<int> index_shocks = s_array[s_size];
					for(unsigned int shock = 0; shock < index_shocks_0.size(); shock++)
					{
						if (index_shocks_0[shock]!=index_shocks[shock])
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Summarization checker: Shock array inconsistency. Different configuration between %s and %s for scenario pass num %d", 
								products[0].get_run_name().c_str(), 
								products[nb_product].get_run_name().c_str(), 
								scenario_pass_number_0
									);
							throw new GMXBException(message);
						}
					}
				}
			}
		}
	}

	void generate_summarized_file( Summary_configuration& summary, int nb_workers,
				map<string, vector<string>>& map_scenario_names,
				map<string, vector<string>>& map_index_shock_names,
				Dispatch_Manager* dispatch_manager)
	{
		
		int number_of_products = summary.get_product_configurations().size();
		if (number_of_products > 0)
		{
			string prod_code_key = summary.get_product_configurations()[0].get_prod_code_key();

			Summary_File_Writer* summary_file_writer = summary_builder(prod_code_key, nb_workers, &summary, map_scenario_names, 
										   								map_index_shock_names, dispatch_manager);
			summary_file_writer->write_output_summary();

			delete summary_file_writer;
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The summarization contain no product to summarize ");
			throw new GMXBException(message);
		}
	}

	int get_maximum_time_steps(map<string, map<int, vector<Model_Point*>>>& map_of_model_points_inner_per_product_outer)
	{

		//Get the time step maximum among the map_of_model_points_inner_per_product_outer
		int time_step_maximum = 0;
		// Loop on products
		for (map<string, map<int, vector<Model_Point*>>>::iterator it_map_of_model_points_inner_per_product_outer= map_of_model_points_inner_per_product_outer.begin();
				it_map_of_model_points_inner_per_product_outer != map_of_model_points_inner_per_product_outer.end(); 
				++it_map_of_model_points_inner_per_product_outer)
		{
				map<int, vector<Model_Point*>> map_mp_outer = it_map_of_model_points_inner_per_product_outer->second;
				// Loop on outer model points
				for(map<int, vector<Model_Point*>>::iterator it_map_mp_outer = map_mp_outer.begin();
					it_map_mp_outer != map_mp_outer.end(); ++it_map_mp_outer)
					{
						vector<Model_Point*> mp_inners = it_map_mp_outer->second;
						time_step_maximum = max(time_step_maximum, (int)mp_inners.size());
					}
		}
	
		return time_step_maximum;
	}


	// New projection optimized for the hedge effectiveness
	void run_hedge_effectiveness_projection( vector<Product_configuration>& vector_of_product_config_out,
		vector<Product_configuration>& vector_of_product_config_in, HE_configuration* he_conf, 
		InterfaceCalibration& iCalib_in, InterfaceEsg& iEsg_in, int tot_nb_workers, int w_id,ofstream& log_strm)
	{
		log_strm << "Hedge effectiveness :  begining outer loop " << endl;
		// TO DO check this hard coded value
		int t_high_shorter = 60;

		Inforce_Manager *inforce_mgr_out;

		// Check the consistency between vector_of_product_config_out and vector_of_product_config_in
		check_projection_consistency(vector_of_product_config_out, vector_of_product_config_in);

		HE_Writter *he_writter =  build_he_writter(vector_of_product_config_out[0].get_prod_code_key());

		map<string, vector<Model_Point*>> map_of_model_points_outer;
		map<string, Projection*> map_of_outer_projections, map_of_inner_projections;
		map<string, Inforce_Manager*> map_of_inforce_manager;
		map<int, map<int, double>> map_of_a;
		map<int, map<int, double>> map_of_sigma;
		map<int, map<int, double*>> map_of_ZC;
		map<int, map<int, double*>> map_of_Swap;
		

		cout << "Charging product configuration outer..." << endl;
		for (unsigned int nb_product_outer = 0; nb_product_outer < vector_of_product_config_out.size(); nb_product_outer++)
		{
			Product_configuration product_out = vector_of_product_config_out[nb_product_outer];
			int nb_product_inner = nb_product_outer;
			Product_configuration product_in = vector_of_product_config_in[nb_product_inner];

			inforce_mgr_out = new Inforce_Manager(',', 1, 1, product_out, product_out.get_scenario_list()[0], true, false, false);
			
			map_of_inforce_manager[product_out.get_prod_code_key()] = inforce_mgr_out;
			map_of_model_points_outer[product_out.get_prod_code_key()] = inforce_mgr_out->get_model_points_vector();	

			Projection * projection_out = projectionBuilder(product_out.get_prod_code_key(), 
					product_out.get_config_file().c_str(), 
					product_out.get_scenario_list()[0].get_index_shock_array(),
					product_out.get_scenario_list()[0].get_fund_shock_array(), 
					product_out.get_generate_debug_file_indic(),
					product_out.get_debug_file_path(), 
					t_high_shorter, 
					false, 
					product_out.get_time_step());

			map_of_outer_projections[product_out.get_prod_code_key()] = projection_out;


			Projection * projection_in = projectionBuilder(product_in.get_prod_code_key(), 
					product_in.get_config_file().c_str(), 
					product_in.get_scenario_list()[0].get_index_shock_array(),
					product_in.get_scenario_list()[0].get_fund_shock_array(), 
					product_in.get_generate_debug_file_indic(),
					product_in.get_debug_file_path(), 
					t_high_shorter, 
					false, 
					product_in.get_time_step());

			map_of_inner_projections[product_in.get_prod_code_key()] = projection_in;
		}


		// Get the number of outer scenarios
		int number_of_outer_scenarios = vector_of_product_config_out[0].get_number_of_scenarios_to_run();

		 
		// The vector scenario_per_worker contains the id(s) of scenario(s) that will be used 
		// for working on the current worker. The function dispatch_per_scenario creates this vector
		// The number of scenarios must be greater than the number of workers
		
		vector<int> scenario_per_worker = Dispatch_Manager::dispatch_per_scenario(tot_nb_workers, w_id, number_of_outer_scenarios);
		cout << "Beginning outer loop..." << endl;
		log_strm << "Beginning outer loop..." << endl;
		// TO DO the dispatch should be implemented here
		for (int nb_scenario_outer = 0; nb_scenario_outer < number_of_outer_scenarios; nb_scenario_outer++)
		{
			log_strm << "scenario outer nb : " << nb_scenario_outer << endl;
			// Check if the current scenario outer is found in a vector scenario_per_worker
			vector<int>::iterator it = find(scenario_per_worker.begin(), scenario_per_worker.end(), nb_scenario_outer);
			if(it == scenario_per_worker.end())
				continue;

			// Initialise map_of_model_points_outer

			for(map<string, vector<Model_Point*>>::iterator it_map_of_model_points_outer = map_of_model_points_outer.begin();
				it_map_of_model_points_outer != map_of_model_points_outer.end(); ++it_map_of_model_points_outer)
			{
				vector<Model_Point*> mps_outer = it_map_of_model_points_outer->second;
				for (unsigned int i =0; i < mps_outer.size(); i++)
				{
					Model_Point *mp = mps_outer[i];
					mp->initialize_index_shock_array();
				}
			}

			// Begining outer loop
			map<string, map<int, vector<Model_Point*>>> map_of_model_points_inner_per_product_outer;

			// Create the scenario manager out
			scenario_manager scen_mgr_out(vector_of_product_config_out[0].get_number_of_scenarios_to_run(), 
				vector_of_product_config_out[0].get_scenario_index_columns(), 
				vector_of_product_config_out[0].get_yield_columns_for_pv(),
				vector_of_product_config_out[0].get_lapse_dyn_treas(), 
				vector_of_product_config_out[0].get_rates_indic_index(), 
				vector_of_product_config_out[0].get_index_number(),
				t_high_shorter,
				// TODO Useless
				vector_of_product_config_out[0].get_scenario_list()[0].get_scenario_file_name(),
				vector_of_product_config_out[0].get_time_step());

			// Check the parameter calib nb_date
			if (he_conf->get_use_scenario_file_inner()==0)
					scen_mgr_out.init_scenario_manager_he(vector_of_product_config_out[0].get_scenario_list()[0].get_scenario_file_name(), 
														  1,iCalib_in.SwapRateTable,iCalib_in.nb_date_initial);
			else
				scen_mgr_out.init_scenario_manager(vector_of_product_config_out[0].get_scenario_list()[0].get_scenario_file_name()
												   ,1);

			// Loop on all the outer products 
			int number_of_outer_projections = vector_of_product_config_out.size();

			
			for (int nb_projection_out = 0; nb_projection_out < number_of_outer_projections; nb_projection_out++)
			{
				log_strm << "Projection outer number  :: begin " << nb_projection_out<< endl;
				Product_configuration prod_config_out = vector_of_product_config_out[nb_projection_out];

				Projection * projection_out = map_of_outer_projections[prod_config_out.get_prod_code_key()];

				// Get the vector of outer model points
				vector<Model_Point*> vector_outer_mp_per_product = 
								map_of_model_points_outer[prod_config_out.get_prod_code_key()];
				
				map<int, vector<Model_Point*>> model_points_inner;
				
				// Loop on the outer model points
				for(unsigned int nb_model_point_outer = 0; nb_model_point_outer < vector_outer_mp_per_product.size();
					nb_model_point_outer++)
				{
					Model_Point * p_mp = vector_outer_mp_per_product[nb_model_point_outer];
					projection_out->set_is_he(true);
					projection_out->set_current_model_point(p_mp);
					projection_out->initialise_index_map();
					if (prod_config_out.get_time_step() == ANNUAL) 
						projection_out->set_annual_from_monthly(scen_mgr_out.get_ann_from_mthly_rates(nb_scenario_outer+1), 
																nb_scenario_outer+1);
					else
						projection_out->set_monthly_rates(scen_mgr_out.get_monthly_rates(nb_scenario_outer+1), 
															nb_scenario_outer+1);

					projection_out->project(); 

					model_points_inner[nb_model_point_outer] = projection_out->age_policies_from_projection();
				}

				map_of_model_points_inner_per_product_outer[prod_config_out.get_prod_code_key()] = model_points_inner;

				log_strm << "Projection outer number  :: end " << nb_projection_out<< endl;

			}
			// write the inner inforces if necessary
			if (he_conf->get_write_inforce_in_file())
			{
				he_writter->write_inner_inforce_files(
					he_conf->get_inforce_in_path_file().c_str(), he_conf->get_inforce_in_file_name().c_str(),
					nb_scenario_outer, map_of_inforce_manager,
					map_of_model_points_inner_per_product_outer);
			}
			// End outer loop

			cout << "Beginning inner loop..." << endl;
			log_strm << "Beginning inner loop..." << endl;
			// Begining inner loop
			int number_of_buckets = vector_of_product_config_in[0].get_scenario_list().size();

			int time_step_maximum = this->get_maximum_time_steps(map_of_model_points_inner_per_product_outer);
			
			cout << "Beginning calibrate loop..." << endl;
			log_strm << "Beginning calibrate loop..." << endl;
			// Calibrate all
			if (he_conf->get_use_scenario_file_inner()==0)
			{
				for (int t_step = 0 ; t_step <= time_step_maximum; t_step++)
				{
					// Get the swap table from 
					// the scenario manager out	
					iCalib_in.UpdateSwapTable(scen_mgr_out.get_swap_table(nb_scenario_outer + 1),t_step);

					for (int nb_bucket_in = 0; nb_bucket_in < number_of_buckets; nb_bucket_in++)
					{
						double startDate = vector_of_product_config_in[0].get_scenario_list()[nb_bucket_in].getBucketStartDate();
						double keyDate = vector_of_product_config_in[0].get_scenario_list()[nb_bucket_in].getBucketKeyDate();
						double endDate = vector_of_product_config_in[0].get_scenario_list()[nb_bucket_in].getBucketEndDate();
						double shockSize = vector_of_product_config_in[0].get_scenario_list()[nb_bucket_in].getBucketShockSize();
						string shockType = vector_of_product_config_in[0].get_scenario_list()[nb_bucket_in].getBucketType();
								
						iCalib_in.ShockSwap(startDate, keyDate, endDate, shockSize, shockType);

						double a, sigma;
						/*a= atof(iEsg_in.assumptions->search("a").c_str());
						sigma=atof(iEsg_in.assumptions->search("sigma").c_str());*/

						double * ZC = new double[iEsg_in.num_initial];
						double * Swap_C = new double[iCalib_in.nb_date];
						for (int i=0; i<iCalib_in.nb_date; i++) Swap_C[i]=iCalib_in.zeroCurve[i];

						cout << "t_step=" << t_step << endl;
						
						if(iEsg_in.calibration==0 && t_step==0 && shockType =="baseline"){
							a = atof(iEsg_in.assumptions->search("a").c_str());
							sigma = atof(iEsg_in.assumptions->search("sigma").c_str());
						}

						if(iEsg_in.calibration==0 || t_step!=0 || shockType !="baseline"){
							iCalib_in.HE_mode_calibration=0;
						}

						iCalib_in.Calibrate(a, sigma, ZC,1, nb_bucket_in,w_id);
						
			
						map_of_a[t_step][nb_bucket_in] = a;
						map_of_sigma[t_step][nb_bucket_in] = sigma;
						map_of_ZC[t_step][nb_bucket_in] = ZC;
						map_of_Swap[t_step][nb_bucket_in] = Swap_C;
					}
				}
			}
			else
			{
					// Use a scenario file
			}

			cout << "end of calibrate loop..." << endl;
			log_strm << "end of calibrate loop..." << endl;
			if (he_conf->get_output_calibration_parameters() == 1)
			{
				he_writter->write_output_calibration_parameters(
						he_conf->get_calibration_file_path().c_str(),he_conf->get_calibration_file_name().c_str(),
						nb_scenario_outer,
						map_of_a, 
						map_of_sigma, 
						map_of_Swap, 
						iCalib_in.nb_date,
						iCalib_in.dateCurve);
			}


			Product_configuration product_in = vector_of_product_config_in[0];

			int time_step_in = product_in.get_time_step();

			int use_scenarios_file=he_conf->get_use_scenario_file_inner();
			int t_high_local=iEsg_in.year;
			scenario_manager* scen_mgr_in = new scenario_manager(product_in.get_number_of_scenarios_to_run(), 
					product_in.get_scenario_index_columns(), 
					product_in.get_yield_columns_for_pv(),
					product_in.get_lapse_dyn_treas(), 
					product_in.get_rates_indic_index(), 
					product_in.get_index_number(),
					t_high_shorter,
					product_in.get_scenario_list()[0].get_scenario_file_name(),
					time_step_in,use_scenarios_file,t_high_local);

			cout << "Beginning bucket loop..." << endl;
			log_strm << "Beginning bucket loop..." << endl;
			//Loop on all buckets
			for (int nb_bucket_in = 0; nb_bucket_in < number_of_buckets; nb_bucket_in++)
			{
				double begin = clock();
				log_strm << "Projection inner bucket : begin " << nb_bucket_in<< endl;

				if (he_conf->get_use_scenario_file_inner()==0)
				{
					// set_index_shock_array for all the 
					//inner projections
					for (map<string, Projection*>::iterator it_projection_in = map_of_inner_projections.begin();
						it_projection_in != map_of_inner_projections.end(); ++it_projection_in)
					{
						string pCK = it_projection_in->first;
						
						Projection * projection_in = map_of_inner_projections[pCK];
						projection_in->set_index_shock_array(product_in.get_scenario_list()[nb_bucket_in].get_index_shock_array());
												
						map<int, vector<Model_Point*>> map_mp_outer = map_of_model_points_inner_per_product_outer[pCK];

						for(map<int, vector<Model_Point*>>::iterator it_map_mp_outer = map_mp_outer.begin();
							it_map_mp_outer != map_mp_outer.end(); ++it_map_mp_outer)
						{
							vector<Model_Point*> mp_outers = it_map_mp_outer->second;

							for (unsigned int nb_mp = 0; nb_mp < mp_outers.size(); nb_mp++)
							{
								Model_Point * p_MP = mp_outers[nb_mp];
								Scenario_configuration scen_configuration_in = product_in.get_scenario_list()[nb_bucket_in];
								
								p_MP->set_pv_index_shock_array(scen_configuration_in);
							}
						}
					}
					cout << "End of bucket loop..." << endl;
					//Project the inner model points
					//Loop on all time steps
					for (int nb_mp_inner = 0; nb_mp_inner < time_step_maximum; nb_mp_inner++)
					{
						cout << " Model point inner number : " << nb_mp_inner + 1 << endl;
						log_strm << " Model point inner number : " << nb_mp_inner + 1 << endl;

						double aCalib = map_of_a[nb_mp_inner][nb_bucket_in];
						double sigmaCalib = map_of_sigma[nb_mp_inner][nb_bucket_in];
						iEsg_in.set_a_without_calib(aCalib);
						iEsg_in.set_sigma_without_calib(sigmaCalib);
						double * ZC_calib = map_of_ZC[nb_mp_inner][nb_bucket_in];
						iEsg_in.set_initial_spot_rate(ZC_calib);

						if(he_conf->step_scenarios_write[nb_mp_inner]==1)
						{
							//iEsg_in.output_scenario=true;
							iEsg_in.output_scenario=true;
							
						}
						else
						{
							iEsg_in.output_scenario = false;
						}
						
						// Generate the scenarios
				
						// TO SEE this function YY !!!!!!!!!
						// the outputs are not sure - to correct this function 

						double*** va_scen = iEsg_in.Gen_he_mode(iCalib_in, aCalib, sigmaCalib, nb_mp_inner, false, product_in.get_scenario_list()[nb_bucket_in].get_bucket_id());
							
						// Initialize the scenario manager
						scen_mgr_in->set_va_scenario(va_scen);
						bool if_he_inner_mode = true;
						scen_mgr_in->init_scenario_manager_ESG(1, true);

						//Loop on all products
						for (map<string, map<int, vector<Model_Point*>>>::iterator it_map_of_model_points_inner_per_product_outer= map_of_model_points_inner_per_product_outer.begin();
						it_map_of_model_points_inner_per_product_outer != map_of_model_points_inner_per_product_outer.end(); 
						++it_map_of_model_points_inner_per_product_outer)
						{

							Projection * projection_in = map_of_inner_projections[it_map_of_model_points_inner_per_product_outer->first];
			
							map<int, vector<Model_Point*>> map_mp_outer = it_map_of_model_points_inner_per_product_outer->second;
							// Loop on outer model points
							for(map<int, vector<Model_Point*>>::iterator it_map_mp_outer = map_mp_outer.begin();
								it_map_mp_outer != map_mp_outer.end(); ++it_map_mp_outer)
							{
								vector<Model_Point*> mp_inners = it_map_mp_outer->second;
								//int mp_time_step_maximum = mp_inners.size();//change Ali

								//if (mp_inners.size() <= time_step_maximum)//Change :Ali
								if (nb_mp_inner < mp_inners.size())//Change :Ali
								{
									Model_Point* pMP_inner = mp_inners[nb_mp_inner];
									projection_in->set_is_he(true);
									projection_in->set_current_model_point(pMP_inner);
									projection_in->initialise_index_map();
									projection_in->compt_sen =0;  // Line added for integration - why ? 
									// Loop on scenario inner
									//Added by Ali
									projection_in->set_total_number_scn((int)(scen_mgr_in->get_nb_scenarios()));  //Line added for integration - why ?

									for (int nb_scenario_inner = 0; nb_scenario_inner < scen_mgr_in->get_nb_scenarios();
										 nb_scenario_inner++)
									{
	
										if (time_step_in == ANNUAL) 
											projection_in->set_annual_from_monthly(scen_mgr_in->get_ann_from_mthly_rates(nb_scenario_inner+1), nb_scenario_inner+1);
										else
											projection_in->set_monthly_rates(scen_mgr_in->get_monthly_rates(nb_scenario_inner+1), nb_scenario_inner+1);
 
										projection_in->project();
									}
								}
							}
						}

						// Delete va_scen
						iEsg_in.delete_s();
						va_scen = NULL;
						
						
					}
				}

				log_strm << "Writing inner files "  << endl;

				if (he_conf->get_output_inner_pvs() ==1)
				{
				
					he_writter->write_model_point_inner_pvs(
						he_conf->get_inner_pvs_file_path().c_str(),
						he_conf->get_inner_pvs_file_name().c_str(),
						nb_scenario_outer,
						nb_bucket_in,
						scen_mgr_in->get_nb_scenarios(),
						map_of_model_points_inner_per_product_outer,
						// code of Ali - to check
						vector_of_product_config_in[0].get_scenario_list()
						);
				}

				double end = clock();
				log_strm << "Projection inner bucket : end time " << (end - begin)/60000 << endl;

			}

			delete scen_mgr_in;
			// End inner loop




			// delete map_of_ZC
			for (map<int, map<int, double*>>::iterator it_map_of_ZC_per_t_step = map_of_ZC.begin(); it_map_of_ZC_per_t_step != map_of_ZC.end(); 
				++it_map_of_ZC_per_t_step)
			{
				map<int, double *> map_ZC = it_map_of_ZC_per_t_step->second;
				for(map<int, double*>::iterator it_ZC = map_ZC.begin(); it_ZC != map_ZC.end(); ++it_ZC)
					delete[] it_ZC->second;
			}
			// delete map_of_Swap
			for (map<int, map<int, double*>>::iterator it_map_of_Swap_per_t_step = map_of_Swap.begin(); it_map_of_Swap_per_t_step != map_of_Swap.end(); 
				++it_map_of_Swap_per_t_step)
			{
				map<int, double *> map_Swap = it_map_of_Swap_per_t_step->second;
				for(map<int, double*>::iterator it_Swap = map_Swap.begin(); it_Swap != map_Swap.end(); ++it_Swap)
					delete[] it_Swap->second;
			}

			// delete the map_of_model_points_per_product
			for (map<string, map<int, vector<Model_Point*>>>::iterator it_map_of_model_points_inner_per_product_outer= map_of_model_points_inner_per_product_outer.begin();
				it_map_of_model_points_inner_per_product_outer != map_of_model_points_inner_per_product_outer.end(); ++it_map_of_model_points_inner_per_product_outer)
			{
				// Loop on products
				map<int, vector<Model_Point*>> map_mp_outer = it_map_of_model_points_inner_per_product_outer->second;

				for(map<int, vector<Model_Point*>>::iterator it_map_mp_outer = map_mp_outer.begin();
					it_map_mp_outer != map_mp_outer.end(); ++it_map_mp_outer)
				{
					vector<Model_Point*> mp_outers = it_map_mp_outer->second;

					for (unsigned int nb_mp = 0; nb_mp < mp_outers.size(); nb_mp++)
					{
						Model_Point * p_MP = mp_outers[nb_mp];
						delete p_MP;
					}
				}
			}
		}


		// delete the map_of_inforce_manager
		for (map<string, Inforce_Manager*>::iterator it_map_inforce_manager = map_of_inforce_manager.begin();
			it_map_inforce_manager!= map_of_inforce_manager.end(); ++it_map_inforce_manager)
		{
			delete it_map_inforce_manager->second;
		}


		// delete all the projection outer
		for (map<string, Projection*>::iterator it_map_of_outer_projections = map_of_outer_projections.begin();
			it_map_of_outer_projections != map_of_outer_projections.end(); ++it_map_of_outer_projections)
		{
			Projection * projection_outer = it_map_of_outer_projections->second;
			delete projection_outer;
		}

		// delete all the projection inner
		for (map<string, Projection*>::iterator it_map_of_inner_projections = map_of_inner_projections.begin();
			it_map_of_inner_projections != map_of_inner_projections.end(); ++it_map_of_inner_projections)
		{
			Projection * projection_inner = it_map_of_inner_projections->second;
			delete projection_inner;
		}
		
		// delete the he_writter
		delete he_writter;
	}

	void write_proba_file(string file_name) {
		
		char proba_file_name[MAX_LENGTH_FILE_NAME];
		sprintf (proba_file_name, "%sproba.csv", file_name.c_str());
		this->projection->file_stock.open(proba_file_name);
		this->projection->file_stock<<" Model_point \\ Time "<<",";
		for (int i=0; i<61; i++){
			this->projection->file_stock<<(i+1)<<",";
		}
		this->projection->file_stock<<'\n';
	}

	void write_proba_LS_file (string file_name) {
		
		char proba_ls_file_name[MAX_LENGTH_FILE_NAME];
		sprintf (proba_ls_file_name, "%sproba_ls.csv", file_name.c_str());
		this->projection->file_stock5.open(proba_ls_file_name);
		this->projection->file_stock5<<" Model_point \\ Time "<<",";
		for (int i=0; i<60; i++){
			this->projection->file_stock5<<(i+1)<<",";
		}
		this->projection->file_stock5<<'\n';
	}

	void write_debug_file (string file_name) {
		
		char debug_file_name[MAX_LENGTH_FILE_NAME];
		sprintf (debug_file_name, "%sdebug.csv", file_name.c_str());
		this->projection->file_stock2.open(debug_file_name);
		this->projection->file_stock2<<" Model_point \\ Time "<<",";
		for (int i=0; i<61; i++){
			this->projection->file_stock2<<(i+1)<<",";
		}
		this->projection->file_stock2<<'\n';
	}

	void read_debug_file (string file_name) {
		string row;
		char debug_file_name[MAX_LENGTH_FILE_NAME];
		sprintf (debug_file_name, "%sdebug.csv", file_name.c_str());
		this->projection->file_stock3.open(debug_file_name);
		if (this->projection->file_stock3)
			getline(this->projection->file_stock3,row);
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The file %s is not found",debug_file_name);
			throw new GMXBException(message);
		}
	}

	void read_proba_LS_file (string file_name) {
		string row;
		char proba_ls_file_name[MAX_LENGTH_FILE_NAME];
		sprintf (proba_ls_file_name, "%sproba_ls.csv", file_name.c_str());
		this->projection->file_stock4.open(proba_ls_file_name);
		if(this->projection->file_stock4)
			getline(this->projection->file_stock4,row);
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The file %s is not found",proba_ls_file_name);
			throw new GMXBException(message);
		}
	}

	// New projection optimized aging policies
	void run_aging_policies_projection( vector<Product_configuration>& vector_of_product_config_out,
		Aging_Policies_configuration* ap_conf, int tot_nb_workers, int w_id)
	{

		// TO DO check this hard coded value
		int t_high_shorter = 60;

		Inforce_Manager *inforce_mgr_out;

		HE_Writter *he_writter =  build_he_writter(vector_of_product_config_out[0].get_prod_code_key());

		map<string, vector<Model_Point*>> map_of_model_points_outer;
		map<string, Projection*> map_of_outer_projections;
		map<string, Inforce_Manager*> map_of_inforce_manager;
		map<string, vector<int>> map_of_index_vector_to_build;

		//AP modif
		map<string, Inforce_Manager*> map_of_inforce_manager_for_aging_policies;
		//map<string, vector<int>> map_of_index_vector_to_build_for_aging_policies;

		for (int nb_product_outer = 0; nb_product_outer < vector_of_product_config_out.size(); nb_product_outer++)
		{
			Product_configuration product_out = vector_of_product_config_out[nb_product_outer];
			int nb_product_inner = nb_product_outer;

			inforce_mgr_out = new Inforce_Manager(',', 1, 1, product_out, product_out.get_scenario_list()[0], false, true, false);

			map_of_inforce_manager[product_out.get_prod_code_key()] = inforce_mgr_out;
			map_of_model_points_outer[product_out.get_prod_code_key()] = inforce_mgr_out->get_model_points_vector();
			// map_of_index_vector_to_build[product_out.get_prod_code_key()] = inforce_mgr_out->get_index_vect_mp_to_build();

			Projection * projection_out = projectionBuilder(product_out.get_prod_code_key(), 
					product_out.get_config_file().c_str(), 
					product_out.get_scenario_list()[0].get_index_shock_array(),
					product_out.get_scenario_list()[0].get_fund_shock_array(), 
					product_out.get_generate_debug_file_indic(),
					product_out.get_debug_file_path(), 
					t_high_shorter, 
					false, 
					product_out.get_time_step());

			map_of_outer_projections[product_out.get_prod_code_key()] = projection_out;
		}

		// Get the number of outer scenarios
		//int number_of_outer_scenarios = vector_of_product_config_out[0].get_number_of_scenarios_to_run();
        int number_of_outer_scenarios  = 1;

		// Initialise map_of_model_points_outer
		for(map<string, vector<Model_Point*>>::iterator it_map_of_model_points_outer = map_of_model_points_outer.begin();
			it_map_of_model_points_outer != map_of_model_points_outer.end(); ++it_map_of_model_points_outer)
		{
			vector<Model_Point*> mps_outer = it_map_of_model_points_outer->second;
			for (int i =0; i < mps_outer.size(); i++)
			{
				Model_Point *mp = mps_outer[i];
				mp->initialize_index_shock_array();
			}
		}

		// Begining outer loop
		map<string, map<int, vector<Model_Point*>>> map_of_model_points_inner_per_product_outer;
		
		//AP modif
		map<string, map<int, vector<Model_Point*>>> map_of_model_points_inner_per_product_outer_for_aging_policies;

		// The scenario manager is not useful because of the fact that growth rates are reading in an input file
		scenario_manager scen_mgr_out;

		// Loop on all the outer products 
		int number_of_outer_projections = vector_of_product_config_out.size();
		
		for (int nb_projection_out = 0; nb_projection_out < number_of_outer_projections; nb_projection_out++)
		{
			Product_configuration prod_config_out = vector_of_product_config_out[nb_projection_out];

			Projection * projection_out = map_of_outer_projections[prod_config_out.get_prod_code_key()];

			projection_out->set_lapse_mult_for_aging_policies(ap_conf->get_lapse_multiplier());
			projection_out->set_mortality_mult_for_aging_policies(ap_conf->get_death_rate_multiplier());
			projection_out->set_charges_mult_for_aging_policies(ap_conf->get_charges_multiplier());
			projection_out->set_duration_age_step_for_aging_policies(ap_conf->get_duration_age_step());
			projection_out->set_aging_policies_indicator(true);

			// Get the vector of outer model points
			vector<Model_Point*> vector_outer_mp_per_product = 
							map_of_model_points_outer[prod_config_out.get_prod_code_key()];

			//vector<int> index_vect_mp_to_build = map_of_index_vector_to_build[prod_config_out.get_prod_code_key()];

			int nb_of_mp_outers = vector_outer_mp_per_product.size();

			// AP - reading growths given in a "scenario" file in an array
			double * growth_rate_by_mp = new double[nb_of_mp_outers];

			/*scen_mgr_out.read_growth_for_aging_policies(vector_of_product_config_out[nb_projection_out].get_scenario_list()[0].get_scenario_file_name()
											   ,1, growth_rate_by_mp, nb_of_mp_outers, index_vect_mp_to_build);*/
			//AP modif
			scen_mgr_out.read_growth_for_aging_policies(ap_conf->get_perfs_s()[nb_projection_out],1, growth_rate_by_mp,nb_of_mp_outers);
			
			map<int, vector<Model_Point*>> model_points_inner;
			
			// Loop on the outer model points
			for(int nb_model_point_outer = 0; nb_model_point_outer < vector_outer_mp_per_product.size();
				nb_model_point_outer++)
			{
				Model_Point * p_mp = vector_outer_mp_per_product[nb_model_point_outer];
				projection_out->set_current_model_point(p_mp);
				projection_out->initialise_index_map();
				
				// AP - create a new function for get the av growths directly
				projection_out->set_monthly_rates_for_aging_policies(growth_rate_by_mp[nb_model_point_outer]);

				//AP - specific project() function for Aging policies because of time of projection (one step)
				//and the reading of av_growth directly as inputs.
				projection_out->project_for_aging_policies();  

				//AP - Specific aging policies function because of the time of projection (one step)
				model_points_inner[nb_model_point_outer] = projection_out->age_policies_from_projection_for_aging_mode();
			}

			map_of_model_points_inner_per_product_outer[prod_config_out.get_prod_code_key()] = model_points_inner;
			delete [] growth_rate_by_mp;

			map_of_inforce_manager_for_aging_policies.clear();
			map_of_inforce_manager_for_aging_policies.insert(pair<string,Inforce_Manager*>(prod_config_out.get_prod_code_key(),map_of_inforce_manager[prod_config_out.get_prod_code_key()]));

			map_of_model_points_inner_per_product_outer_for_aging_policies.clear();
			map_of_model_points_inner_per_product_outer_for_aging_policies.insert(pair<string, map<int, vector<Model_Point*>>>(prod_config_out.get_prod_code_key(),map_of_model_points_inner_per_product_outer[prod_config_out.get_prod_code_key()]));

			//map_of_index_vector_to_build_for_aging_policies.clear();
			//map_of_index_vector_to_build_for_aging_policies.insert(pair<string, vector<int>>(prod_config_out.get_prod_code_key(),map_of_index_vector_to_build[prod_config_out.get_prod_code_key()]));
			
			if (ap_conf->get_write_inforce_in_file())
			{
				he_writter->write_inner_inforce_for_aging_policies(
					"",
					ap_conf->get_eom_model_s()[nb_projection_out].c_str(),
					0, 
					map_of_inforce_manager_for_aging_policies,
					map_of_model_points_inner_per_product_outer_for_aging_policies,
					ap_conf->get_cw_s()[nb_projection_out],
					ap_conf->get_tw_s()[nb_projection_out]);
			}

		}
		// End outer loop


		// delete the map_of_model_points_per_product
		for (map<string, map<int, vector<Model_Point*>>>::iterator it_map_of_model_points_inner_per_product_outer= map_of_model_points_inner_per_product_outer.begin();
			it_map_of_model_points_inner_per_product_outer != map_of_model_points_inner_per_product_outer.end(); ++it_map_of_model_points_inner_per_product_outer)
		{
			// Loop on products
			map<int, vector<Model_Point*>> map_mp_outer = it_map_of_model_points_inner_per_product_outer->second;

			for(map<int, vector<Model_Point*>>::iterator it_map_mp_outer = map_mp_outer.begin();
				it_map_mp_outer != map_mp_outer.end(); ++it_map_mp_outer)
			{
				vector<Model_Point*> mp_outers = it_map_mp_outer->second;

				for (int nb_mp = 0; nb_mp < mp_outers.size(); nb_mp++)
				{
					Model_Point * p_MP = mp_outers[nb_mp];
					delete p_MP;
				}
			}
		}

		// delete the map_of_inforce_manager
		for (map<string, Inforce_Manager*>::iterator it_map_inforce_manager = map_of_inforce_manager.begin();
			it_map_inforce_manager!= map_of_inforce_manager.end(); ++it_map_inforce_manager)
		{
			delete it_map_inforce_manager->second;
		}


		// delete all the projection outer
		for (map<string, Projection*>::iterator it_map_of_outer_projections = map_of_outer_projections.begin();
			it_map_of_outer_projections != map_of_outer_projections.end(); ++it_map_of_outer_projections)
		{
			Projection * projection_outer = it_map_of_outer_projections->second;
			delete projection_outer;
		}

		// delete the he_writter
		delete he_writter;

	}



};
