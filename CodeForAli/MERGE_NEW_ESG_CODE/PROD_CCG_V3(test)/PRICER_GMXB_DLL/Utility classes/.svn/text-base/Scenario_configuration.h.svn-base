#pragma once

#include "stdafx.h"
#include "Utility classes/File_Manager.h"
using namespace std;

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
	private :
		string scenario_file_name;
		int scenario_pass_number;
		vector<vector<int> > index_shock_array;
		vector<vector<int> > fund_shock_array;
		
		vector<GMXB_Assumption_Table*> v_lapse_table_multiplicator;
		vector<GMXB_Assumption_Table*> v_mortality_table_multiplicator;
		vector<GMXB_Assumption_Table*> v_paidup_table_multiplicator;
		vector<GMXB_Assumption_Table*> v_election_table_multiplicator;
		bool load_longevity_tables; 
		bool use_scenario_file;
		bool shock_funds;

		int bucket_id;
		double bucketStartDate;
		double bucketKeyDate;
		double bucketEndDate;
		double bucketShockSize;
		string bucket_type;
		
		void check_file_path()
		{
			if (this->use_scenario_file)
			{
				File_Manager file_mgr;
				file_mgr.is_file_path_valid(this->scenario_file_name);
			}
		}
		
	public :
		~Scenario_configuration(){};

		Scenario_configuration(){};

		/*
		* Constructor for the Hedge effectiveness
		*/

		Scenario_configuration(const string& scen_file_name, 
							   int scen_pass_num)
		{
			this->scenario_file_name = scen_file_name;
			this->scenario_pass_number = scen_pass_num;	
			// Case of indexes
			vector<int> delta_step;
			for (int j = 0; j <= MAXIMUM_NUMBER_OF_INDEXES; j++)
					delta_step.push_back(0);
			this->index_shock_array.push_back(delta_step);
			// TO DO add the shocks on funds
		
			/*for (unsigned int i = 0; i < v_mortality_table_multiplicator.size(); i++)
			{
				delete this->v_mortality_table_multiplicator[i];
		
			}*/
		}

		/*
		* Copy constructor of Scenario_configuration class
		*/
		Scenario_configuration(const Scenario_configuration& sc)
		{
			this->scenario_file_name = sc.scenario_file_name;
			this->scenario_pass_number = sc.scenario_pass_number;

			int index_shock_array_size = sc.index_shock_array.size();
			for (int i=0; i < index_shock_array_size; i++)
				this->index_shock_array.push_back(sc.index_shock_array[i]);
	
			int fund_shock_array_size = sc.fund_shock_array.size();
			for (int i=0; i < fund_shock_array_size; i++)
				this->fund_shock_array.push_back(sc.fund_shock_array[i]);


			this->bucketStartDate = sc.bucketStartDate;	
			this->bucketKeyDate = sc.bucketKeyDate;	
			this->bucketEndDate = sc.bucketEndDate;
			this->bucketShockSize = sc.bucketShockSize;
			this->bucket_id = sc.bucket_id;		
			this->bucket_type = sc.bucket_type;

			this->v_lapse_table_multiplicator = sc.v_lapse_table_multiplicator; 
			this->v_election_table_multiplicator = sc.v_election_table_multiplicator; 
			this->v_mortality_table_multiplicator = sc.v_mortality_table_multiplicator;
			this->v_paidup_table_multiplicator = sc.v_paidup_table_multiplicator;
			this->load_longevity_tables = sc.load_longevity_tables;
			this->use_scenario_file = sc.use_scenario_file;
			this->shock_funds = sc.shock_funds;
		}

		/*
		* Assignment operator for Scenarion_configuration class
		*/
		Scenario_configuration & operator = (const Scenario_configuration& sc)
		{
			if(this != & sc)
			{           
				index_shock_array.clear();
				fund_shock_array.clear();
		
				/* 
				* May be this vectors must be cleared
				* v_lapse_table_multiplicator.clear();
				* v_mortality_table_multiplicator.clear();
				*/

				this->scenario_file_name = sc.scenario_file_name;
				this->scenario_pass_number = sc.scenario_pass_number;
				
				int index_shock_array_size = sc.index_shock_array.size();
				for (int i=0; i < index_shock_array_size; i++)
					this->index_shock_array.push_back(sc.index_shock_array[i]);

				int fund_shock_array_size = sc.fund_shock_array.size();
				for (int i=0; i < fund_shock_array_size; i++)
					this->fund_shock_array.push_back(sc.fund_shock_array[i]);			

				this->bucketStartDate = sc.bucketStartDate;	
				this->bucketKeyDate = sc.bucketKeyDate;	
				this->bucketEndDate = sc.bucketEndDate;
				this->bucketShockSize = sc.bucketShockSize;
				this->bucket_id = sc.bucket_id;		
				this->bucket_type = sc.bucket_type;

				this->v_lapse_table_multiplicator = sc.v_lapse_table_multiplicator; 
				this->v_mortality_table_multiplicator = sc.v_mortality_table_multiplicator;
				this->v_paidup_table_multiplicator = sc.v_paidup_table_multiplicator;
				this->load_longevity_tables = sc.load_longevity_tables;
				this->use_scenario_file = sc.use_scenario_file;
				this->shock_funds = sc.shock_funds;
			}
			return *this;
		}

		/*
		* Constructor for File and MoSes modes
		* dimension of the s_array equals to 2
		* No bucket
		*/
		Scenario_configuration(const string& scen_file_name, 
							   int scen_pass_num, vector<vector<int> >& s_array,
							   vector<vector<int> >& f_array, bool use_scn_file, bool shock_f,
							   const vector<GMXB_Assumption_Table*>& v_lapse_mult, 
							   const vector<GMXB_Assumption_Table*>& v_mortality_mult,
							   const vector<GMXB_Assumption_Table*>& v_paidup_mult,
							   const vector<GMXB_Assumption_Table*>& v_election_mult,
							   bool l_longevity_tables)
		{
			this->scenario_file_name = scen_file_name;
			this->scenario_pass_number = scen_pass_num;
			int shock_array_size = s_array.size();
			for (int i=0; i < shock_array_size; i++)
				this->index_shock_array.push_back(s_array[i]);

			this->bucketStartDate = -1;
			this->bucketKeyDate = -1;
			this->bucketEndDate = -1;
			this->bucketShockSize = -1;
			this->bucket_id = -1;;		
			this->bucket_type = "-1";

			this->v_lapse_table_multiplicator = v_lapse_mult;
			this->v_mortality_table_multiplicator = v_mortality_mult;
			this->v_paidup_table_multiplicator = v_paidup_mult;
			this->v_election_table_multiplicator = v_election_mult;
			this->load_longevity_tables = l_longevity_tables;

			this->shock_funds = shock_f;

			if (this->shock_funds)
			{
				for (unsigned int i=0; i < f_array.size(); i++)
					this->fund_shock_array.push_back(f_array[i]);
			}

			this->use_scenario_file = use_scn_file;
			this->check_file_path();
			
		}

		/*
		* Scenario configuration constructor for ESG mode
		* dimension of the s_array equals to 3
		*/
		Scenario_configuration( const string& scen_file_name,
								int scen_pass_num,
								vector<vector<int> >&  s_array,
								vector<vector<int> >&  f_array, 
								double bucketStartDate,
								double bucketKeyDate,
								double bucketEndDate,
								double bucketShockSize,
								string bucket_type,  
								int bucket_id,
								bool use_scn_file, 
								bool shock_f,
								const vector<GMXB_Assumption_Table*>& v_lapse_mult, 
								const vector<GMXB_Assumption_Table*>& v_mortality_mult,
								const vector<GMXB_Assumption_Table*>& v_paidup_mult,
								const vector<GMXB_Assumption_Table*>& v_election_mult,
								bool l_longevity_tables)
			{

			this->scenario_file_name = scen_file_name;
			this->scenario_pass_number = scen_pass_num;

			int shock_array_size = s_array.size();
			for (int i=0; i < shock_array_size; i++)
				this->index_shock_array.push_back(s_array[i]);

			
			this->bucketStartDate = bucketStartDate;	
			this->bucketKeyDate = bucketKeyDate;	
			this->bucketEndDate = bucketEndDate;
			this->bucketShockSize = bucketShockSize;
			this->bucket_id = bucket_id;		
			this->bucket_type = bucket_type;
			
			this->v_lapse_table_multiplicator = v_lapse_mult;
			this->v_mortality_table_multiplicator = v_mortality_mult;
			this->v_paidup_table_multiplicator = v_paidup_mult;
			this->v_election_table_multiplicator = v_election_mult;
			this->load_longevity_tables = l_longevity_tables;

			this->shock_funds = shock_f;

			if (this->shock_funds)
			{
				for (unsigned int i=0; i < f_array.size(); i++)
					this->fund_shock_array.push_back(f_array[i]);
			}

			this->use_scenario_file = use_scn_file;
			this->check_file_path();
		}

		int get_scenario_pass_number()
		{
			return this->scenario_pass_number;
		}
		
		int get_bucket_id()
		{
			return this->bucket_id;
		}
		
		vector<vector<int> > get_index_shock_array()
		{
			return this->index_shock_array;
		}

		vector<vector<int> > get_fund_shock_array()
		{
			return this->fund_shock_array;
		}
		

		/*
		* Is size equalts index_size + shock size ? 
		* To be verified
		*/
		int get_shock_array_size(int i)
		{
			return (this->index_shock_array.size() + this->fund_shock_array.size() );
		
		}

		string get_scenario_file_name()	{
					return this->scenario_file_name;
		
		}
		
		double getBucketStartDate() 
		{
			return this->bucketStartDate;
		}

		double getBucketKeyDate() 
		{
			return this->bucketKeyDate;
		}

		double getBucketEndDate() 
		{
			return this->bucketEndDate;
		}

		double getBucketShockSize() 
		{
			return this->bucketShockSize;
		}
		vector<GMXB_Assumption_Table*> get_lapse_mult()
		{
			return this->v_lapse_table_multiplicator;
		
		}
		vector<GMXB_Assumption_Table*> get_election_mult()
		{
			return this->v_election_table_multiplicator;
		
		}
		vector<GMXB_Assumption_Table*> get_mortality_mult()
		{
			return this->v_mortality_table_multiplicator;
		
		}
		vector<GMXB_Assumption_Table*> get_paidup_mult()
		{
			return this->v_paidup_table_multiplicator;
		
		}

		bool get_load_longevity_table()
		{
			return this->load_longevity_tables;
		
		}
		string getBucketType()
		{
			return this->bucket_type;
		}
		void set_to_basic()
		{
			vector<vector<int>> tmp;
			tmp.push_back(this->index_shock_array[0]);
			this->index_shock_array=tmp;
		}
};