#pragma once

#include <vector>
#include "stdafx.h"
#include <string>
#include <algorithm>
#include "Utility classes/Product_configuration.h"
#include "Utility classes/Scenario_configuration.h"
#include "Utility classes/Bucket_list.h"
#include "Utility classes/Scenario_list.h"

using namespace std;

class Regroup_by_Scenario_PN
{
	int scenario_pn; 
	vector<int> bucket_id_list_;
	vector<vector<int>>	product_index_list_; 
	

public:
    
	bool test_regroup_condition(int scenario_pn_ip);

	Regroup_by_Scenario_PN(int scenario_pass_number,
		                   vector<Product_configuration>& product_config);
	
	vector<int> get_bucket_id_list()		   {return bucket_id_list_;}   
	vector<int>& get_bucket_id_list_reference(){return bucket_id_list_;} 

	vector<vector<int>> get_product_index_list()		   {return product_index_list_;}
	vector<vector<int>>& get_product_index_list_reference(){return product_index_list_;}



	int get_scenario_pn(){return scenario_pn;}
};

Regroup_by_Scenario_PN :: Regroup_by_Scenario_PN(int scenario_pn_ip,
						   vector<Product_configuration>& product_config):scenario_pn(scenario_pn_ip)
{
	 bool if_fund_bucket_id = false;

	 for(unsigned int i=0; i<product_config.size(); i++)
	 {
		 vector<Scenario_configuration> scenario_list_temp(product_config[i].get_scenario_list());
	     for(unsigned int j=0; j<scenario_list_temp.size(); j++)
		 {
			  // Added for multi-ESG
			  // if scenario_pn is not the one that we want to regroup
			  if(!test_regroup_condition(scenario_list_temp[j].get_scenario_pass_number()))
				 continue;

		      if(bucket_id_list_.size()==0)
			  {
				  bucket_id_list_.push_back(scenario_list_temp[j].get_bucket_id());
				  vector<int> vector_temp(1,i);
                  product_index_list_.push_back(vector_temp);
			  }
			  else
			  {
				 // test if bucket already exist in the bucket_list
				 for(unsigned int k=0; k < bucket_id_list_.size(); k++)
				 {
					 if(bucket_id_list_[k] == scenario_list_temp[j].get_bucket_id())
					 {	 
						  product_index_list_[k].push_back(i);
						  if_fund_bucket_id = true;
						  break;
					 }
				 }

				 if(if_fund_bucket_id == false)
				 {
					 bucket_id_list_.push_back(scenario_list_temp[j].get_bucket_id());
					 vector<int> vector_temp(1,i);
					 product_index_list_.push_back(vector_temp);
				 }

				 if_fund_bucket_id = false;
			  }
		 }
	 }
	 //Check if first bucket in each secnario is baseline, if not throw exception
	 if(bucket_id_list_[0] != 1)
	 {
	 		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error in the scenario's bucket config: the first bucket of scenario_pass_number %d should be baseline (bucket_id shoubld be 1)", scenario_pn );
			throw new GMXBException(message);
	 }
}

bool Regroup_by_Scenario_PN ::  test_regroup_condition(int scenario_pn_ip)
{ 
	if(scenario_pn == scenario_pn_ip)
		return true;
	else 
		return false;
}


class Dispatch_Manager
{
    vector<Regroup_by_Scenario_PN> regrouped_scenario_pns;
	Scenario_list				   scenario_list_table;
	Bucket_list					   bucket_list_table;

public:
	//! no copy-constructor/assignment/destructor: Rule of three
	Dispatch_Manager(std::vector<Product_configuration>& product_config, string& parameter_file);

	vector<Regroup_by_Scenario_PN>& get_regrouped_scenario_pns_reference(){return regrouped_scenario_pns;}
	vector<Regroup_by_Scenario_PN> get_regrouped_scenario_pns(){return regrouped_scenario_pns;}

	Scenario_list  get_Scenario_list()          {return scenario_list_table;}
	Scenario_list& get_Scenario_list_reference(){return scenario_list_table;}

	Bucket_list  get_Bucket_list()          {return bucket_list_table;}
	Bucket_list& get_Bucket_list_reference(){return bucket_list_table;}
	static vector<int> dispatch_per_scenario(int nb_workers, int id_worker, int number_of_scenarios);
};


Dispatch_Manager :: Dispatch_Manager(std::vector<Product_configuration>& product_config,
									 string& parameter_file):
									 scenario_list_table(parameter_file),
									 bucket_list_table(parameter_file)
{
	 vector<int> scenario_pns;
	 scenario_pns.reserve(20);

	 for(unsigned int i=0; i<product_config.size(); i++)
	 {
		 vector<Scenario_configuration> scenario_list_temp(product_config[i].get_scenario_list());
	     for(unsigned int j=0; j<scenario_list_temp.size(); j++)
		 {
			 int scen_this_pn = scenario_list_temp[j].get_scenario_pass_number();

			 //! we can optimise this code by put it out of loop of j
			 vector<int>::iterator itr = find(scenario_pns.begin(),scenario_pns.end(),scen_this_pn);
			 if(itr == scenario_pns.end())
				 scenario_pns.push_back(scen_this_pn);
		 }
	 }
     sort(scenario_pns.begin(),scenario_pns.end());

	 //! I want to replace it by bind! 
	 for(size_t i=0; i<scenario_pns.size(); ++i)
	 {
		 regrouped_scenario_pns.push_back(Regroup_by_Scenario_PN(scenario_pns[i],product_config)); 
	 }
}

vector<int> Dispatch_Manager :: dispatch_per_scenario(int nb_workers, int id_worker, int number_of_scenarios)
{
	vector<int> scenario_per_worker;

	for(int index_scen=0; index_scen < number_of_scenarios; index_scen++)
	{      
		if(nb_workers == 1)
		{
			scenario_per_worker.push_back(index_scen);
		}
		else if(nb_workers <= number_of_scenarios)
		{
			if( (index_scen < nb_workers && index_scen == id_worker-1)
				|| (index_scen >= nb_workers && index_scen%nb_workers+1 == id_worker))
					scenario_per_worker.push_back(index_scen);
			else
				continue;
		}
		else
		{   
				/*
				* If this option is activated, we need to convert the workers ids to continous ids for
				* the dispatching by policies in inforce manager class
				*/

				/* 
					if((id_worker<=N_scen && id_worker-1 == index_scen) 
					|| (id_worker>N_scen && id_worker%N_scen ==0 && index_scen == N_scen-1)
					|| (id_worker>N_scen && id_worker%N_scen !=0 && id_worker%N_scen-1 == index_scen) )
						scenario_per_worker.push_back(index_scen);
					else
						continue;
				*/

			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error in dispatch manager : The number of workers is greater than the number of scenarios.");
			throw new GMXBException(message);
		}
	}
	return scenario_per_worker;
}