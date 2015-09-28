#include "Dispatch_Manager.h"


const unsigned int scenario_list_nCol = 1; // [0]=ESG_scenario_config, [1]=ESG_calibration_config

Scenario_list::Scenario_list(){};
Scenario_list::Scenario_list(string& parameter_file)  
{
	GMXBExcelXMLReader xml_reader(parameter_file);
	WildcardManager wm( parameter_file, "wildcards" );
	
	map<string, vector<string> > map_scenario_list_ip;	 
	xml_reader.parse_XML_sheet("scenario_list", map_scenario_list_ip);
	 
	for(map<string, vector<string> >::iterator map_iterator = map_scenario_list_ip.begin();map_iterator!=map_scenario_list_ip.end();map_iterator++)	 
	{   		
		string s = map_iterator->first;			 
		//! l'entete	
		if(s == "scenario_pass_number")	
			continue;
	
		int scenario_pn = atoi(s.c_str());	
		vector<string> scenario_config(map_iterator->second);	
		scenario_config[0] = wm.getPath(scenario_config[0]);   // esg_address		
		map_scnPn_ESGCalibPath[scenario_pn] = scenario_config;
			 		
		// TODO: replace it with an exception		
		if(map_iterator->second.size() != scenario_list_nCol )	
			cout << "Error size of the map_iterator->second != 2 in constructor of class Scenario_list(...) "  << endl;  	
	}
}
string Scenario_list::get_ESG_config_path(int scenario_pn_ip)
{
	for(map<int,vector<string>>::iterator itr = map_scnPn_ESGCalibPath.begin();itr != map_scnPn_ESGCalibPath.end(); itr++){
		if(scenario_pn_ip == itr->first) return itr->second[0];
	}	
	return "Error";
	
}



//! ne utilise pas .push_back, which copy_coller object!
const int bucket_col_size = 6;  // contain: bucket_type, _start, _key, _end, _size 
Bucket_list :: Bucket_list(string& parameter_file)
	{
		 GMXBExcelXMLReader xml_reader(parameter_file);
		 map<string, vector<string> > map_bucket_id;
		 xml_reader.parse_XML_sheet("bucket_list", map_bucket_id);


		 int bucket_row_size = map_bucket_id.size()-1; // size of the bucket existant in file "bucket_id"
		 bucket_list_id_   = vector<int>(bucket_row_size);
		 bucket_list_type_ = vector<string>(bucket_row_size);
		 bucket_list_contenance_ = vector<vector<double>>(bucket_row_size,vector<double>(bucket_col_size-2));
		 bucket_list_if_calib_ = vector<bool>(bucket_row_size);
	
		 int i=-1;
		 int id_bucket;
		 for(map<string, vector<string> >::iterator map_iterator = map_bucket_id.begin(); map_iterator!=map_bucket_id.end(); map_iterator++)
		 {   
			 string first_col((*map_iterator).first) ;
             if(first_col == "id") continue;  // if it is the head of the excel file 

			 i++;
			 id_bucket = atoi( first_col.c_str() );

			 bucket_list_id_[i] = atoi( first_col.c_str() );

			 stringstream ss; 
			 ss << id_bucket;

			 for(int j=0; j<bucket_col_size; j++)
			 {
					 if(j==0)
					    bucket_list_type_[i] =  map_bucket_id[ss.str()][j].c_str();

					 if(j>0 && j < bucket_col_size - 2)
						bucket_list_contenance_[i][j-1] = atof( map_bucket_id[ss.str()][j].c_str() );

					 if(j == bucket_col_size - 2)
						bucket_list_contenance_[i][j-1] = atof( map_bucket_id[ss.str()][j].c_str() )*1.0/100.0;

					 if(j == bucket_col_size -1)
					 {
						 string if_calib_s (map_bucket_id[ss.str()][j].c_str());
                         //cout << "********** " << if_calib_s << endl;
					     if(if_calib_s == "Y")
						 {
							 bucket_list_if_calib_[i] = true;
						 }
						 else if(if_calib_s == "N")
						 {
							 bucket_list_if_calib_[i] = false;
						 }
						 else
						 {
							 // Error !  throw Exception! 
							 char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							 sprintf_s(message, "Error in the Excel file: variable 'bucket_list' can only take value as: 'Y' or 'N'!  ");
							 throw new GMXBException(message);
						 }
					 }
					//! check if baseline is in the first line of the bucke_list file
                    if(i==0)
					{
					    if(bucket_list_type_[i]!= "baseline" || bucket_list_id_[i]!= 1)
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Error in Excel file bucket_list: the first line is not the baseline ! Please change your configuration file");
							throw new GMXBException(message);
						}
					}

					//! TODO: check also if the baseline's if_calib == 1 (????)
			 }
		 }
		 //print();
	}
string Bucket_list :: get_bucket_list_type(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
		     if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_type_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_type(...) ");
		throw new GMXBException(message);
	}
vector<double> Bucket_list :: get_bucket_list_contenance(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
			if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_contenance_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_contenance(...) ");
		throw new GMXBException(message);
	}
bool Bucket_list :: get_bucket_list_if_calib(int id_bucket)
	{
		for(unsigned int i=0; i<bucket_list_id_.size(); i++)
		{
		     if(bucket_list_id_[i] == id_bucket)
				 return this->bucket_list_if_calib_[i];
		}
		// Error !  throw Exception! 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in fonction : get_bucket_list_if_calib(...) ");
		throw new GMXBException(message);
	}




	
vector<int> Regroup_by_Scenario_PN::get_bucket_id_list(){return bucket_id_list_;}   
vector<int>& Regroup_by_Scenario_PN::get_bucket_id_list_reference(){return bucket_id_list_;} 
vector<vector<int>> Regroup_by_Scenario_PN::get_product_index_list(){return product_index_list_;}
vector<vector<int>>& Regroup_by_Scenario_PN::get_product_index_list_reference(){return product_index_list_;}
Regroup_by_Scenario_PN :: Regroup_by_Scenario_PN(int scenario_pn_ip,vector<Product_configuration>& product_config):scenario_pn(scenario_pn_ip)
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






vector<Regroup_by_Scenario_PN>& Dispatch_Manager::get_regrouped_scenario_pns_reference(){return regrouped_scenario_pns;}
vector<Regroup_by_Scenario_PN> Dispatch_Manager::get_regrouped_scenario_pns(){return regrouped_scenario_pns;}
Scenario_list  Dispatch_Manager::get_Scenario_list(){return scenario_list_table;}
Scenario_list& Dispatch_Manager::get_Scenario_list_reference(){return scenario_list_table;}
Bucket_list  Dispatch_Manager::get_Bucket_list()          {return bucket_list_table;}
Bucket_list& Dispatch_Manager::get_Bucket_list_reference(){return bucket_list_table;}

Dispatch_Manager :: Dispatch_Manager(std::vector<Product_configuration>& product_config,string& parameter_file)
	:scenario_list_table(parameter_file),bucket_list_table(parameter_file)
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