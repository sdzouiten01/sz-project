#include "GMXBParameters.h"
#include <fstream>
#include "GMXBException.h"
#include "tinyxml.h"
using namespace std;

void GMXBParameters::parse_sheet()
{
	TiXmlDocument doc(this->file_name.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", file_name.c_str());
		throw new GMXBException(message);
	}

	TiXmlHandle doch(&doc);
	TiXmlElement * p_root = doch.FirstChildElement().Element();

	TiXmlHandle h_root(p_root);
	TiXmlElement * p_worksheet = h_root.FirstChild("Worksheet").Element();
	while(p_worksheet && p_worksheet->Attribute("ss:Name")!=this->sheet_name)
		p_worksheet = p_worksheet->NextSiblingElement();

	if (!p_worksheet)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The sheet %s is not found in file %s", sheet_name.c_str(), file_name.c_str());
		throw new GMXBException(message);
	}

	if(p_worksheet)
	{
		TiXmlHandle h_worksheet(p_worksheet);
		TiXmlElement * p_table = h_worksheet.FirstChild("Table").Element();
		if(p_table)
		{
			TiXmlHandle h_table(p_table);
			TiXmlElement * p_row = h_table.ChildElement("Row", 0).Element();
			while(p_row && p_row->Value()== string("Row"))
			{
				TiXmlHandle h_row(p_row);
				TiXmlElement * p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
				if(p_data->GetText()== string("DATA"))
				{
					this->insert(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText(), h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
				}
				else 
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Only the keyword DATA is allowed in the sheet %s", sheet_name.c_str());
					throw new GMXBException(message);
				}
				p_row = p_row->NextSiblingElement();
			}
		}
	}
}
#ifdef __ZYK_OPT__
GMXBParameters::GMXBParameters(){}
#else
GMXBParameters::GMXBParameters():highIndex("null"){}
#endif

GMXBParameters::GMXBParameters(const string& config_file_name, const string& sh_name)
{
	this->sheet_name = sh_name;
	this->file_name = config_file_name;
	ifstream conf_file(config_file_name.c_str(), ios::in);
	if (!conf_file)
	{ 
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The file %s is not found", config_file_name.c_str());
		throw new GMXBException(message);
	}
	conf_file.close();
#ifndef __ZYK_OPT__
	this->highIndex = "null";
#endif
	this->parse_sheet();
}

int GMXBParameters::insert(const string& key, const string& value)
{
	my_hash_d1[key] = value;
#ifndef __ZYK_OPT__
	if(key>this->highIndex)
		this->highIndex = key;
#endif
	return 1;
}

string GMXBParameters::search(const string& key, bool with_exception /* = 1 */)
{
#ifdef __ZYK_OPT__
	map<string, string>::iterator it = my_hash_d1.find(key);
	if (it != my_hash_d1.end())
		return it->second;
#else
	map<string, string>::iterator it = this->my_hash_d1.begin();
	for(it = this->my_hash_d1.begin(); it != this->my_hash_d1.end(); it++)
	{
		if(key == it->first)
			return it->second;
	}
#endif
	if (with_exception)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The parameter %s is not found in sheet %s", key.c_str(), sheet_name.c_str());
		throw new GMXBException(message);
	}
	else
	{
		return "Not Found";
	}
}

void GMXBParameters::clear()
{
	this->my_hash_d1.clear();
}

#ifndef __ZYK_OPT__
string GMXBParameters::high_index()
{
	return this->highIndex;		
}
#endif
