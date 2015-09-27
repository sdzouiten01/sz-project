#include "Scenario_configuration.h"



	



void Scenario_configuration::is_file_path_valid(const string& file_name_path)
		{
			ifstream file_op(file_name_path.c_str() , ios::in);
			if (!file_op)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "The file %s is not found", file_name_path.c_str());
				throw new GMXBException(message);
			}
			file_op.close();
		}
void Scenario_configuration::check_file_path()
		{
			if (this->use_scenario_file){
				/*File_Manager file_mgr;*/
				/*file_mgr.*/is_file_path_valid(this->scenario_file_name);
			}
		}
Scenario_configuration::~Scenario_configuration(){}
Scenario_configuration::Scenario_configuration(){}
/** Copy constructor of Scenario_configuration class*/
Scenario_configuration::Scenario_configuration(const Scenario_configuration& sc)
		{
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
		
/** Constructor for File and MoSes modes     * dimension of the s_array equals to 2      * No bucket*/
Scenario_configuration::Scenario_configuration(const string& scen_file_name, int scen_pass_num, vector<vector<int> >& s_array, bool use_scn_file)
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

			this->use_scenario_file = use_scn_file;
			this->check_file_path();
			
		}

/** Scenario configuration constructor for ESG mode    * dimension of the s_array equals to 3*/
Scenario_configuration::Scenario_configuration( const string& scen_file_name,int scen_pass_num,vector<vector<int> >&  s_array, double bucketStartDate,double bucketKeyDate,
			double bucketEndDate,double bucketShockSize,string bucket_type, int bucket_id,bool use_scn_file)
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
			
			this->use_scenario_file = use_scn_file;
			this->check_file_path();
		}

int Scenario_configuration::get_scenario_pass_number(){return this->scenario_pass_number;}
int Scenario_configuration::get_bucket_id(){return this->bucket_id;}
string Scenario_configuration::get_scenario_file_name()	{return this->scenario_file_name;}		
vector<vector<int> > Scenario_configuration::get_index_shock_array(){return this->index_shock_array;}
		
