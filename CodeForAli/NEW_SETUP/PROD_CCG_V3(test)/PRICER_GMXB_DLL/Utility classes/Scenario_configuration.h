#pragma once

#include "../Utility classes/stdafx.h"
//#include "../stdafx.h"
#include "head.h"
//#include "GMXBException.h"
//#include <io.h>   
//#include <sys/types.h>  
//#include <sys/stat.h>  
//#include "Utility classes/File_Manager.h"

using namespace std;
#include "GMXB_Assumption_Table.h"

/*
* This class manage all the scenario configuration
* The scenario configuration is detailed in one sheet of the run configuration
* For a given scenario we have:
		- The scenario name 
		- The scenario pass number which is the id of the scenario
		- index1....index12 liste of index shock arrays
		- An indicator to indicate whether we shock the funds or not
		- A list of fund shocks (The name of the fund and the amplitude of the shocks are described in fund_list sheet of the run_configuration)
		- List of lapse table multiplicator (for lapse shock purpose)
		- List of mortality table multiplicator (for mortality shock purpose)
		- An indicator used for longevity shocks
*/

class Scenario_configuration
{
	public :
		string scenario_file_name;
		int scenario_pass_number;
		vector<vector<int> > index_shock_array;
		bool use_scenario_file;

		int bucket_id;
		double bucketStartDate;
		double bucketKeyDate;
		double bucketEndDate;
		double bucketShockSize;
		string bucket_type;
		
		void is_file_path_valid(const string& file_name_path);
		void check_file_path();
		~Scenario_configuration();
		Scenario_configuration();

		/** Copy constructor of Scenario_configuration class*/
		Scenario_configuration(const Scenario_configuration& sc);	
		/** Assignment operator for Scenarion_configuration class*/
		Scenario_configuration & operator = (const Scenario_configuration& sc)
		{
			if(this != & sc)
			{           
				index_shock_array.clear();

				this->scenario_file_name = sc.scenario_file_name;
				this->scenario_pass_number = sc.scenario_pass_number;
				
				int index_shock_array_size = sc.index_shock_array.size();
				for (int i=0; i < index_shock_array_size; i++)
					this->index_shock_array.push_back(sc.index_shock_array[i]);
			
				this->bucketStartDate = sc.bucketStartDate;	
				this->bucketKeyDate = sc.bucketKeyDate;	
				this->bucketEndDate = sc.bucketEndDate;
				this->bucketShockSize = sc.bucketShockSize;
				this->bucket_id = sc.bucket_id;		
				this->bucket_type = sc.bucket_type;

				this->use_scenario_file = sc.use_scenario_file;
			}
			return *this;
		}

		/** Constructor for File and MoSes modes     * dimension of the s_array equals to 2      * No bucket*/
		Scenario_configuration(const string& scen_file_name, int scen_pass_num, vector<vector<int> >& s_array, bool use_scn_file);

		/** Scenario configuration constructor for ESG mode    * dimension of the s_array equals to 3*/
		Scenario_configuration( const string& scen_file_name,int scen_pass_num,vector<vector<int> >&  s_array, double bucketStartDate,double bucketKeyDate,
			double bucketEndDate,double bucketShockSize,string bucket_type, int bucket_id,bool use_scn_file);

		int get_scenario_pass_number();
		int get_bucket_id();
		string get_scenario_file_name();	
		vector<vector<int> > get_index_shock_array();
		
};