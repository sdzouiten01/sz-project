//#include "stdafx.h"
#pragma once
#include "stdafx.h"
#include <string>
#include <map>
#include <iostream>
#include "GMXBException.h"
#include <fstream>
#include <vector>


using namespace std;

#define DIM 0
#define ROW_MISSING 1
#define ROW_BEFFIR 2
#define ROW_AFTLAST 3
#define COL_MISSING 4
#define COL_BEFFIR 5
#define COL_AFTLAST 6
#define ROW_NUM 7
#define COL_NUM 8
//redefine for 1_dimention table 
#define MISSING 4
#define BEF_FIRST 5
#define AFT_LAST 6
#define OPTNUM 9

//define of errors
#define DM_N_MATCH_ERROR 0
#define INDEX_MISSING_ERROR 1
#define BEF_FIRST_ERROR 2
#define AFT_LAST_ERROR 3
#define OPT_N_FOUND_ERROR 4
#define VAR_NOT_FOUND_ERROR 5
#define ROW_BEF_FIRST_ERROR 6
#define ROW_AFT_LAST_ERROR 7
#define ROW_INDEX_MISSING_ERROR 8
#define COL_BEF_FIRST_ERROR 9
#define COL_AFT_LAST_ERROR 10
#define COL_INDEX_MISSING_ERROR 11
#define FILE_OPEN_FAILED 12
#define DIM_N_DEF 13
#define KEY_WORD_WRONG 14
#define SENARIOS_SET_WRONG 15
#define DURATION_WRONG 16
#define TYPE_WRONG 17
#define NO_MODEL_POINT 18

//define of option
#define NOT_DEF -1
#define ZERO 0
#define THROW_ERROR 1
#define FIRST 2
#define LINEAR 3
#define LAGRANGE 4
#define LINEAR_LAGRANGE 5
#define PREVIOUS 6
#define SELECT 7
#define LAST 8
#define ULTIMATE 9


using namespace std;



// Deprecated class
// should not be used
class GMXB_Assumption_Table_Deprecated
{
private:
	string sheet_name;
	string file_name;
	int number_of_columns;
	map<double, map<double, double>> my_hash_d2;


	int index(const string& key)
	{
		if(key == "DIM")
			return 0;
		else if(key == "ROW_MISSING")
			return 1;
		else if(key == "ROW_BEFFIR")
			return 2;
		else if(key == "ROW_AFTLAST")
			return 3;
		else if(key == "COL_MISSING")
			return 4;
		else if(key == "COL_BEFFIR")
			return 5;
		else if(key == "COL_AFTLAST")
			return 6;
		else if(key == "MISSING")
			return 4;
		else if(key == "BEF_FIRST")
			return 5;
		else if(key == "AFT_LAST")
			return 6;
		else if(key == "ROW_NUM")
			return 7;
		else if(key == "COL_NUM")
			return 8;
		else if(key == "NOT_DEF")
			return -1;
		else if(key == "ZERO")
			return 0;
		else if(key == "ERROR")
			return 1;
		else if(key == "FIRST")
			return 2;
		else if(key == "LINEAR")
			return 3;
		else if(key == "LAGRANGE")
			return 4;
		else if(key == "LINEAR_LAGRANGE")
			return 5;
		else if(key == "PREVIOUS")
			return 6;
		else if(key == "SELECT")
			return 7;
		else if(key == "LAST")
			return 8;
		else if(key == "ULTIMATE")
			return 9;
		else 
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The key %s is unknown in sheet %s!", key.c_str()
									, sheet_name.c_str());
			throw new GMXBException(message);
		}
	}

	void parse_sheet()
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
				TiXmlElement * p_first_row = h_table.ChildElement("Row", 0).Element();
				if(!p_first_row)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The sheet %s is empty!", sheet_name.c_str());
					throw new GMXBException(message);
				}
				// Management of the OPT
				TiXmlHandle h_row(p_first_row);
				TiXmlElement * p_data_type = h_row.ChildElement("Cell",1).FirstChild("Data").Element();
				if (strcmp(p_data_type->GetText(),"DIM"))
				{
					string type = p_data_type->GetText();
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The first row must contain the keyword DIM in sheet %s", sheet_name.c_str());
					throw new GMXBException(message);				
				}

				TiXmlElement * p_data = h_row.ChildElement("Cell",2).FirstChild("Data").Element();
				this->opt[DIM] = atoi(p_data->GetText());
				
				// Handle all the options. Options are not mandatory 
				TiXmlElement * p_row = p_first_row->NextSiblingElement();

				while(p_row && p_row->Value()== string("Row"))
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if (p_data->GetText()==string("OPT"))
					{
						if(index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())==7 || index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText()) ==8)
							this->opt[index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]=atoi(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
						else
							this->opt[index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]=index(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else 
						break;
					
					p_row = p_row->NextSiblingElement();
				}
				vector<double> vector_of_columns;
				// Handle the table columns. Table columns are mandatory
				//if (p_row && p_row->Value()== *(new string("Row")))
				if (p_row && p_row->Value()== string("Row"))  // by Jiangang
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					// if (p_data->GetText()==*(new string("TABLE_COL")))
					if (p_data->GetText()==string("TABLE_COL"))  // by Jiangang
					{
						TiXmlElement * col;
						// The values of the columns begin from 2
						int i = 2;
						while (col = h_row.ChildElement("Cell",i).FirstChild("Data").Element())
						{
							vector_of_columns.push_back(atof(col->GetText()));
							i++;
							this->number_of_columns++;
						}
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "The keyword TABLE_COL in sheet %s is mandatory", sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
				// Handle the table rows
				p_row = p_row->NextSiblingElement();
				// while (p_row && p_row->Value()== *(new string("Row")))
				while (p_row && p_row->Value()==string("Row"))  // Jiangang
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					// if (p_data->GetText()==*(new string("TABLE_ROW")))
					if (p_data->GetText()==string("TABLE_ROW"))  // Jiangang
					{
						// get the row key
						double row_key = atof(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText());
						
						if(row_key > this->highIndex)
							this->highIndex = row_key;

						map<double, double> col_map;
						for (int col = 0; col < this->number_of_columns; col++)
						{
							col_map[vector_of_columns[col]] = atof(h_row.ChildElement("Cell",2 + col).FirstChild("Data")
																		.Element()->GetText());
						}
						my_hash_d2[row_key] = col_map;
						p_row = p_row->NextSiblingElement();
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "The keyword %s is not allowed in this section, only TABLE_ROW is allowed (sheet: %s) %s", 
							p_data->GetText() , sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
			}
		}
	}

public:
	double highIndex;
	int opt[OPTNUM];
	double * table_d1;
	double ** _to2D_array; 

	
	GMXB_Assumption_Table_Deprecated()
	{
	
	}

	~GMXB_Assumption_Table_Deprecated()
	{
		for (int i =0; i <= (int)this->my_hash_d2.size(); i++)
		{
			if (_to2D_array[i]!=0)
				delete[] _to2D_array[i];
		}
		if (_to2D_array!=0)
			delete[] _to2D_array;
	}

	GMXB_Assumption_Table_Deprecated(const string& config_file_name, const string& sh_name)
	{
		for(int i = 0;i<OPTNUM; i++)
			opt[i]=NOT_DEF;
		this->highIndex = -99999.9;

		this->number_of_columns = 0;

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
		this->parse_sheet();
		this->reform();
	}
	
	double before_first(map<double, double>::iterator& it)
	{
		switch(opt[BEF_FIRST])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw BEF_FIRST;
			break;
		case FIRST:
			return it->second;
			break;
		case LINEAR:
			break;
		case LAGRANGE:
			break;
		case LINEAR_LAGRANGE:
			break;
		default:
			throw OPT_N_FOUND_ERROR;
			break;
		}
		return 0.;
	}

	double before_first(double key2, map<double, map<double, double>>::iterator& it)
	{
		switch(opt[ROW_BEFFIR])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw ROW_BEF_FIRST_ERROR;
			break;
		case FIRST:
			return this->search_d1(key2,it->second);
			break;
		case LINEAR:
			break;
		case LAGRANGE:
			break;
		case LINEAR_LAGRANGE:
			break;
		default:
			throw OPT_N_FOUND_ERROR;
			break;
		}
		return 0.;
	}

	double missing(map<double, double>::iterator& it, double key)
	{
		map<double, double>::iterator temp;
		switch(opt[MISSING])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw INDEX_MISSING_ERROR;
			break;
		case PREVIOUS:
			return (--it)->second;
			break;
		case LINEAR:
			temp = (--it);
			it++;
			return (key-temp->first)/(it->first-temp->first)*(it->second-temp->second)+temp->second;
			break;
		case LAGRANGE:

			break;
		case SELECT:
			break;

		default:
			throw OPT_N_FOUND_ERROR;
			break;	
		}
		return 0.;
	}

	double missing(double key1, double key2, map<double, map<double,double>>::iterator& it)
	{
		
		map<double, map<double,double>>::iterator temp;


		switch(opt[ROW_MISSING])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw ROW_INDEX_MISSING_ERROR;
			break;
		case PREVIOUS:
			return this->search_d1(key2, (--it)->second);
			break;
		case FIRST:
			return this->search_d1(key2,it->second);
			break;
		case LINEAR:
			/* MODIFICATION - LINEAR INTERPOLATION */
			temp = --it;
			it++;
			return this->search_d1(key2,(temp)->second) + ((key1 - temp->first) / (it->first - temp->first))* (this->search_d1(key2,it->second) - this->search_d1(key2, temp->second));
			break;
		case LAGRANGE:
			break;
		case SELECT:
			break;
		default:
			throw OPT_N_FOUND_ERROR;
			break;	
		}
		return 0.;
	}

	double after_last(map<double, double>::iterator& it)
	{
		switch(opt[AFT_LAST])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw AFT_LAST_ERROR;
			break;
		case LAST:
			return (--it)->second;
			break;
		case LINEAR:

			break;
		case LAGRANGE:

			break;
		case LINEAR_LAGRANGE:

			break;
		case ULTIMATE:

			break;
		default:
			throw OPT_N_FOUND_ERROR;
			break;
		}	
		return 0.;
	}

	
	double after_last(double key2, map<double, map<double, double>>::iterator& it)
	{
		switch(opt[ROW_AFTLAST])
		{
		case NOT_DEF:
			throw VAR_NOT_FOUND_ERROR;
			break;
		case ZERO:
			return 0.0;
			break;
		case THROW_ERROR:
			throw ROW_AFT_LAST_ERROR;
			break;
		case LAST:
			return this->search_d1(key2, (--it)->second);
			break;
		case LINEAR:
			break;
		case LAGRANGE:
			break;
		case LINEAR_LAGRANGE:
			break;
		case ULTIMATE:
			break;
		default:
			throw OPT_N_FOUND_ERROR;
			break;
		}
		return 0.;
	}

	double search_d1(double key, map<double, double>& my_map)
	{
		map<double, double>::iterator it = my_map.begin();
	
		/* START - MODIFICATION
		   Code used to correct the fact that my_map begin with an it->first equal to 1
	       This fact do that control structures on columns are false because tests are made
		   thinking that it->first for the first column is 0
		   This fact is due to the initialization of the hash_d2 map in parsesheet with col_map where
		   vector_columns[col] for the first col is 1
		*/
		
		map<double, double> my_second_map;
	
		int i = 0;
		if (it->first != 0)
		{
			for(i = 0, it = my_map.begin(); it != my_map.end(); it++, i++)
			{
				my_second_map[i] = it->second;
			}
		}
		else
			my_second_map = my_map;

		it = my_second_map.begin();

		/* original code */

		if(key<it->first)
			return before_first(it);
		else
		{
			for(it = my_second_map.begin(); it != my_second_map.end(); it++)
			{
				if(key == it->first)
				{
					return it->second;
				}
				else if (key < it->first)
					return missing(it, key);
			}
			if(it == my_second_map.end())
				return after_last(it);
		}
		return 0.;
	}


	// changeb by Yann
	double search_d2(double key1, double key2, map<double, map<double,double>>& my_map)
	{
		map<double, map<double, double>>::iterator it = my_map.begin();
		
		
		if(key1 < it->first)
		{
			return before_first(key2, it);
		}
		else
		{
			for(it = my_map.begin(); it != my_map.end(); it++)
			{
				
				if(key1 == it->first)
					return this->search_d1(key2, it->second);
				else if(key1 < it->first)
					return missing(key1, key2, it);
			}
			if(it ==my_map.end() )
				return after_last(key2, it);
		}
		return 0.;
	}


	public:
	void reform()
	{
		_to2D_array = new double*[this->my_hash_d2.size() + 1];
		for (int i =0; i <= (int)this->my_hash_d2.size(); i++)
		{
			_to2D_array[i] = new double[this->number_of_columns];
				for (int j =0; j < this->number_of_columns; j++)
					_to2D_array[i][j] = this->search_d2(i,j, this->my_hash_d2);
		}
	}


	int insert(double key1, double key2, double value)
	{
		if(opt[DIM]!=2)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "This method cannot be used for a table of dimension %d", opt[DIM]);
			throw new GMXBException(message);
		}
		map<double, double> temp = my_hash_d2[key1];
		temp[key2] = value;
		my_hash_d2[key1]=temp;
		if(key1>this->highIndex)
			this->highIndex = key1;
		return 1;
	}	

	double search(double key1, double key2)
	{
		if(opt[DIM]!=2)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "This method cannot be used for a table of dimension %d", opt[DIM]);
			throw new GMXBException(message);
		}
		else
		{
			return this->search_d2(key1, key2, this->my_hash_d2);
		}
	}
	
	double high_index()
	{
		return this->highIndex;	
	}

	int no_of_columns()
	{
		return this->number_of_columns;
	}

};
