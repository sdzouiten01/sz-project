#pragma once
#include "stdafx.h"
#include <string>
#include <map>


using namespace std;

class GMXB_Assumption_Table;
class GMXBParameters
{
	friend GMXBParameters* parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, string para_sheet);
	friend void parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, map<string, GMXBParameters*>& tables_para);
private:	
	map<string, string> my_hash_d1;
	#ifndef __ZYK_OPT__
	string highIndex;
	#endif

	string sheet_name;
	string file_name;

	void parse_sheet();
public:
	
	GMXBParameters();
	GMXBParameters(const string& config_file_name, const string& sh_name);
	
	int insert(const string& key, const string& value);
	string search(const string& key, bool with_exception=1);
	void clear();
	#ifndef __ZYK_OPT__
	string high_index();
	#endif
};