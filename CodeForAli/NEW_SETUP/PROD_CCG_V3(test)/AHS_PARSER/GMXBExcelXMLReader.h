#pragma once
#include <vector>
#include <string>
#include <map>
using std::vector;
using std::string;

class GMXBExcelXMLReader
{
private: 
	string xml_file_name;

public:

	~GMXBExcelXMLReader();
	GMXBExcelXMLReader(const string& file_name);

	void parse_XML_sheet(const string& sheet_name, std::map<string, vector<string> >& m);
	void parse_XML_sheet(const string& sheet_name, const string& keyword, vector<string>& v);
};