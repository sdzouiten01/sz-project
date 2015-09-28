#pragma once

#include "../stdafx.h"
#include <string>
#include <fstream>
#include <map>
#include "File_Manager.h"
#include "Fund.h"

using namespace std;

class Fund_Manager
{

private:

	static Fund_Manager *_singleton;
	static map<string, vector<string> > map_of_shortsellable_funds;

	static void load_shortsellable_fund(const string& run_conf_file);

	static void add_fund(const string& key, Fund& fund, map<string, Fund>& map_of_funds);

	static void populate_betas(Fund& fund, const string& split, bool is_target_split);

	~Fund_Manager();
	Fund_Manager();


public :
	static void init(const string& run_conf);

	static void build_map_of_funds(const string& management_fee_param_file,
		const string& fund_split_param_file, const string& fund_valo_file, map<string, Fund>& fund_map);


	static Fund_Manager* getInstance()
	{
		if (NULL == Fund_Manager::_singleton)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Fund_Manager instance not created");
			throw new GMXBException(message);								
		}
		else
		{
			return Fund_Manager::_singleton;
		}
	}

};










