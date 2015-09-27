#pragma once

#include "Fund_Manager.h"
#include "GMXBExcelXMLReader.h"
#include <sstream>
#include <map>

using namespace std;

Fund_Manager* Fund_Manager::_singleton = NULL;
map<string, vector<string> > Fund_Manager::map_of_shortsellable_funds;


void Fund_Manager::load_shortsellable_fund(const string& run_conf_file)
{
	File_Manager file_mgr;
	file_mgr.is_file_path_valid(run_conf_file);
	GMXBExcelXMLReader xml_reader(run_conf_file);
	xml_reader.parse_XML_sheet("shortsellable_fund_list", Fund_Manager::map_of_shortsellable_funds);

}


void Fund_Manager::build_map_of_funds(const string& management_fee_param_file,
									  const string& fund_split_param_file, const string& fund_valo_file, 
									  map<string, Fund>& fund_map)
{

	// Open the management fee file
	ifstream file_op(management_fee_param_file.c_str() , ios::in);
	int fund_index = -1;
	while(!file_op.eof())
	{	
		string row;
		getline(file_op,row);
		Fund f;
		if (row.length() > 0)
		{
			fund_index++;
			char *retPtr = new char[row.length() + 1];
			strcpy(retPtr, row.c_str());
			retPtr[row.length()] = '\0';
			string alh_fund_id = string(strtok(retPtr, ","));
			string valuation_date = string(strtok(NULL, ","));
			double mgt_fee_perc = atof(strtok(NULL, ","));
			string f_id;
			f.set_alh_fund_id(alh_fund_id);
			f.set_percentage_fee_amount(mgt_fee_perc);
			f.set_fund_id(fund_index);
			
			for(map<string, vector<string> >::const_iterator iter_map_fund_ids = map_of_shortsellable_funds.begin();
				iter_map_fund_ids!= map_of_shortsellable_funds.end(); ++iter_map_fund_ids)
			{

				f_id = iter_map_fund_ids->first;
				if(f_id == alh_fund_id)
				{
					vector<string> fund_characteristics = iter_map_fund_ids->second;
					istringstream shortsell_group_str;
					shortsell_group_str.str(fund_characteristics[0]);
					string element;
					while (getline(shortsell_group_str, element, ',') )
					{
						f.add_shortsell_goup_id(atoi(element.c_str()));
					}
					// Fill the map of fund_ids
					break;
				}
			}
			
			Fund_Manager::add_fund(alh_fund_id, f, fund_map);
			delete retPtr;
		}
	}
	file_op.close();

	// Management of the fund split file
	ifstream file_op1(fund_split_param_file.c_str() , ios::in);
	while(!file_op1.eof())
	{
		string row;
		getline(file_op1,row);
		if (row.length() > 0)
		{
			char *retPtr = new char[row.length() + 1];
			strcpy(retPtr, row.c_str());
			retPtr[row.length()] = '\0';
			string alh_fund_id = string(strtok(retPtr, ","));
			string split_indicator = string(strtok(NULL, ","));
			string split = row.substr(alh_fund_id.length() + split_indicator.length() + 2, row.length());
			bool found = false;
			// Try to find the fund corresponding to the 
			// alh_fund_id in the fund_map
			// Otherwise raise an exception
			for (map<string, Fund>::const_iterator iter_map_fund_ids = fund_map.begin();
				iter_map_fund_ids != fund_map.end(); ++iter_map_fund_ids)
			{
				string a_fund_id = iter_map_fund_ids->first;
				if (a_fund_id == alh_fund_id)
				{
					found = true;
					if (split_indicator == "Y")
					{
						Fund_Manager::populate_betas(fund_map[a_fund_id], split, true);
						fund_map[a_fund_id].optimize_array();
					}
					else if (split_indicator == "N")
					{
						Fund_Manager::populate_betas(fund_map[a_fund_id], split, false);
						
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf(message, "The 2'nd column of the fund split param file (%s) should be either Y or N", 
							fund_split_param_file.c_str());
						throw new GMXBException(message);														
					}
					break;
				}
			}
			if (found == false)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The fund_id %s in file %s is not found in the management_fee_file (%s)", alh_fund_id.c_str(), 
					fund_split_param_file.c_str(),  management_fee_param_file.c_str());
				throw new GMXBException(message);								
			}



			delete retPtr;
		}
	}
	file_op1.close();


	ifstream file_op2(fund_valo_file.c_str() , ios::in);
	while(!file_op2.eof())
	{
		string row;
		getline(file_op2,row);
		if (row.length() > 0)
		{

			char *retPtr = new char[row.length() + 1];
			strcpy(retPtr, row.c_str());
			retPtr[row.length()] = '\0';
			string alh_fund_id = string(strtok(retPtr, ","));
			string valuation_date = string(strtok(NULL, ","));
			double fund_valo = atof(strtok(NULL, ","));
			string currency = string(strtok(NULL, ","));
			bool found = false;

			// Try to find the fund corresponding to the 
			// alh_fund_id in the map_of_funds
			// Otherwise raise an exception
			for (map<string, Fund>::const_iterator iter_map_fund_ids = fund_map.begin();
				iter_map_fund_ids != fund_map.end(); ++iter_map_fund_ids)
			{
				string a_fund_id = iter_map_fund_ids->first;
				if (a_fund_id == alh_fund_id)
				{
					found = true;
					fund_map[a_fund_id].set_fund_value(fund_valo);
					break;
				}
			}
			if (found == false)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The fund_id %s in file %s is not found in the management_fee_file (%s)", alh_fund_id.c_str(), 
					fund_valo_file.c_str(),   management_fee_param_file.c_str());
				throw new GMXBException(message);								
			}
			delete retPtr;
		}
	}
	file_op2.close();
	
}

Fund_Manager::~Fund_Manager()
{
}
Fund_Manager::Fund_Manager()
{
}

void Fund_Manager::init(const string& run_conf)
{
	if (NULL == Fund_Manager::_singleton)
	{
		Fund_Manager::_singleton = new Fund_Manager();
		load_shortsellable_fund(run_conf);
	}
}


void Fund_Manager::add_fund(const string& key, Fund& fund, map<string, Fund>& map_of_funds)
{
	map_of_funds.insert(make_pair(key, fund));

}

void Fund_Manager::populate_betas(Fund& fund, const string& split, bool is_target_split)
{
	fund.fill_betas(split, is_target_split);
}

