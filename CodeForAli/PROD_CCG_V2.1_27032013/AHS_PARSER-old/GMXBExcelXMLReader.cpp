#include "GmXBExcelXMLReader.h"
#include "tinyxml.h"
#include "GMXBException.h"
#include <fstream>
using namespace std;

GMXBExcelXMLReader::~GMXBExcelXMLReader()
{
}
GMXBExcelXMLReader::GMXBExcelXMLReader(const string& file_name)
{
	this->xml_file_name = file_name;
	ifstream file_op(this->xml_file_name.c_str() , ios::in);
	if (!file_op)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The file %s is not found", this->xml_file_name.c_str());
		throw new GMXBException(message);
	}
	file_op.close();

}
void GMXBExcelXMLReader::parse_XML_sheet(const string& sheet_name, map<string, vector<string> >& m)
{

	TiXmlDocument doc(this->xml_file_name.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", this->xml_file_name.c_str());
		throw new GMXBException(message);
	}

	TiXmlHandle doch(&doc);
	TiXmlElement * p_root = doch.FirstChildElement().Element();

	TiXmlHandle h_root(p_root);
	TiXmlElement * p_worksheet = h_root.FirstChild("Worksheet").Element();
	while(p_worksheet && p_worksheet->Attribute("ss:Name")!=sheet_name)
		p_worksheet = p_worksheet->NextSiblingElement();

	if (!p_worksheet)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The sheet %s is not found in file %s", sheet_name.c_str(), this->xml_file_name.c_str());
		throw new GMXBException(message);
	}

	if(p_worksheet)
	{

		TiXmlHandle h_worksheet(p_worksheet);
		TiXmlElement * p_table = h_worksheet.FirstChild("Table").Element();
		if (!p_table)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "No table found. The sheet is empty");
			throw new GMXBException(message);
		}

		if(p_table)
		{
			TiXmlHandle h_table(p_table);
			int j = 0;
			TiXmlElement * p_row = h_table.ChildElement("Row", j).Element();
			while(p_row)
			{
				TiXmlHandle h_row(p_row);
				TiXmlElement * p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
				if (p_data)
				{
					string key = string(p_data->GetText());
					int i =1;
					vector<string> row;
					p_data = h_row.ChildElement("Cell",i).FirstChild("Data").Element();
					while (p_data)
					{
						row.push_back(string(p_data->GetText()));
						i++;
						p_data = h_row.ChildElement("Cell",i).FirstChild("Data").Element();
					}
					m[key] = row;
					p_row = p_row->NextSiblingElement();
					j++;
				}
				else
					break;
				p_row = h_table.ChildElement("Row", j).Element();
			}
		}
	}
}

void GMXBExcelXMLReader::parse_XML_sheet(const string& sheet_name, const string& keyword, vector<string>& v)
{

	TiXmlDocument doc(this->xml_file_name.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", this->xml_file_name.c_str());
		throw new GMXBException(message);
	}

	TiXmlHandle doch(&doc);
	TiXmlElement * p_root = doch.FirstChildElement().Element();

	TiXmlHandle h_root(p_root);
	TiXmlElement * p_worksheet = h_root.FirstChild("Worksheet").Element();
	while(p_worksheet && p_worksheet->Attribute("ss:Name")!=sheet_name)
		p_worksheet = p_worksheet->NextSiblingElement();

	if (!p_worksheet)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "The sheet %s is not found in file %s", sheet_name.c_str(), this->xml_file_name.c_str());
		throw new GMXBException(message);
	}

	if(p_worksheet)
	{
		TiXmlHandle h_worksheet(p_worksheet);
		TiXmlElement * p_table = h_worksheet.FirstChild("Table").Element();
		if (!p_table)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "No table found. The sheet is empty");
			throw new GMXBException(message);
		}
		if(p_table)
		{
			TiXmlHandle h_table(p_table);
			TiXmlElement * p_row = h_table.ChildElement("Row", 0).Element();
			while(p_row && p_row->Value()== string("Row"))
			{
				TiXmlHandle h_row(p_row);
				TiXmlElement * p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
				if(p_data->GetText()==string(keyword))
				{
					int i =1;
					while (p_data = h_row.ChildElement("Cell",i).FirstChild("Data").Element())
					{
						v.push_back(p_data->GetText());
						i++;
					}
					break;
				}
				p_row = p_row->NextSiblingElement();
			}
		}
	}
}