#pragma once
#include "stdafx.h"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#include "GMXBException.h"
#include "tinystr.h"
#include "tinyxml.h"

using namespace std;

#define ASSUMP_ROW_BEF_FIRST 0
#define ASSUMP_ROW_AFT_LAST 1
#define ASSUMP_ROW_MISSING 2
#define ASSUMP_COL_BEF_FIRST 3
#define ASSUMP_COL_AFT_LAST 4
#define ASSUMP_COL_MISSING 5
#define ASSUMP_OPTNUM 6

//define of option
#define ASSUMP_ZERO 0
#define ASSUMP_THROW_ERROR 1
#define ASSUMP_FIRST 2
#define ASSUMP_PREVIOUS 3
#define ASSUMP_LAST 4

#define WRONG_DOUBLE -9999.9999e-99


class GMXBParameters;
class GMXB_Assumption_Table
{
	friend GMXBParameters* parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, string para_sheet);
	friend void parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, map<string, GMXBParameters*>& tables_para);
private:
	string sheet_name;
	string file_name;
	#ifdef __ZYK_OPT__
	map<int, map<int, double>> file_to_map;
	#else
	map<double, map<double, double>> file_to_map;
	#endif
	// Dimension of the map
	#ifdef __ZYK_OPT__
	int map_row_highIndex;
	int map_column_highIndex;
	int map_row_lowIndex;
	int map_column_lowIndex;
	#define _MAX_INDEX_	99999
	#else
	double map_row_highIndex;
	double map_column_highIndex;
	double map_row_lowIndex;
	double map_column_lowIndex;
	#define _MAX_INDEX_	99999.9
	#endif

	int row_before_first_index;
	int column_before_first_index;
	int row_after_last_index;
	int column_after_last_index;

	vector<int> row_indices;
	vector<int> column_indices;

	int opt[ASSUMP_OPTNUM];
	double ** _to2D_array; 

	/*
	ZERO
	ERROR
	FIRST
	PREVIOUS
	LAST
	*/
	#ifdef __ZYK_OPT__
	static int index(const string& key)
	{
		// Option ....
		switch(key[0])
		{
		case 'Z':
			return ASSUMP_ZERO;
		case 'E':
			return ASSUMP_THROW_ERROR;
		case 'F':
			return ASSUMP_FIRST;
		case 'P':
			return ASSUMP_PREVIOUS;
		case 'L':
			return ASSUMP_LAST;
		case 'R':
			// ROW_BEF_FIRST
			// ROW_AFT_LAST
			// ROW_MISSING
			switch(key[4])
			{
			case 'B':
				return ASSUMP_ROW_BEF_FIRST;
			case 'A':
				return ASSUMP_ROW_AFT_LAST;
			case 'M':
				return ASSUMP_ROW_MISSING;
			default:
				;
			}
		case 'C':
			switch(key[4])
			{
			case 'B':
				return ASSUMP_COL_BEF_FIRST;
			case 'A':
				return ASSUMP_COL_AFT_LAST;
			case 'M':
				return ASSUMP_COL_MISSING;
			default:
				;
			}
		default:
			;
		}
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Syntax error in the table (Option part) : The key %s is unknown", key.c_str() );
								//, sheet_name.c_str());
		throw new GMXBException(message);
	}
	#else
	static int index(const string& key)
	{
		// Option ....
		if(key == "ROW_BEF_FIRST")
			return ASSUMP_ROW_BEF_FIRST;
		else if(key == "ROW_AFT_LAST")
			return ASSUMP_ROW_AFT_LAST;
		else if(key == "ROW_MISSING")
			return ASSUMP_ROW_MISSING;
		else if(key == "COL_BEF_FIRST")
			return ASSUMP_COL_BEF_FIRST;
		else if(key == "COL_AFT_LAST")
			return ASSUMP_COL_AFT_LAST;
		else if(key == "COL_MISSING")
			return ASSUMP_COL_MISSING;
		// ... allowed values
		else if(key == "ZERO")
			return ASSUMP_ZERO;
		else if(key == "ERROR")
			return ASSUMP_THROW_ERROR;
		else if(key == "FIRST")
			return ASSUMP_FIRST;
		else if(key == "PREVIOUS")
			return ASSUMP_PREVIOUS;
		else if(key == "LAST")
			return ASSUMP_LAST;
		else 
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table (Option part) : The key %s is unknown", key.c_str() );
									//, sheet_name.c_str());
			throw new GMXBException(message);
		}
	}
	#endif

	void check_options()
	{
		// ROW_BEF_FIRST option can only 
		// take ERROR and FIRST values
		if(this->opt[ASSUMP_ROW_BEF_FIRST] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_BEF_FIRST is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_ROW_BEF_FIRST] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_ROW_BEF_FIRST] != ASSUMP_FIRST)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_BEF_FIRST can only take value ERROR or FIRST ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		// ROW_AFT_LAST option can only 
		// take ERROR and LAST values
		if(this->opt[ASSUMP_ROW_AFT_LAST] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_AFT_LAST is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_ROW_AFT_LAST] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_ROW_AFT_LAST] != ASSUMP_LAST)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_AFT_LAST can only take value ERROR or FIRST ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		// ROW_MISSING option can only 
		// take ERROR and PREVIOUS values
		if(this->opt[ASSUMP_ROW_MISSING] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_MISSING is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_ROW_MISSING] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_ROW_MISSING] != ASSUMP_PREVIOUS)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_MISSING can only take value ERROR or PREVIOUS ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		// COL_BEF_FIRST option can only 
		// take ERROR and FIRST values
		if(this->opt[ASSUMP_COL_BEF_FIRST] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : COL_BEF_FIRST is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_COL_BEF_FIRST] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_COL_BEF_FIRST] != ASSUMP_FIRST)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : COL_BEF_FIRST can only take value ERROR or FIRST ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		// COL_AFT_LAST option can only 
		// take ERROR and LAST values
		if(this->opt[ASSUMP_COL_AFT_LAST] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : COL_AFT_LAST is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_COL_AFT_LAST] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_COL_AFT_LAST] != ASSUMP_LAST)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : COL_AFT_LAST can only take value ERROR or FIRST ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		// COL_MISSING option can only 
		// take ERROR and PREVIOUS values
		if(this->opt[ASSUMP_COL_MISSING] == -1)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : COL_MISSING is not configured ", sheet_name.c_str());
			throw new GMXBException(message);
		}
		else if(this->opt[ASSUMP_COL_MISSING] != ASSUMP_THROW_ERROR && this->opt[ASSUMP_COL_MISSING] != ASSUMP_PREVIOUS)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Syntax error in the table %s : ROW_MISSING can only take value ERROR or PREVIOUS ", sheet_name.c_str());
			throw new GMXBException(message);
		}
	}


	// This function parses the xml file
	// to build a table of 2 dimension _to_2D_array
#if 1
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
					sprintf_s(message, "The first row of sheet %s should not be empty!", sheet_name.c_str());
					throw new GMXBException(message);
				}
				// Handle all the options. Options are not mandatory 
				TiXmlElement * p_data;
				TiXmlElement * p_row = p_first_row;

				while(p_row && p_row->Value()== string("Row"))
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if (p_data->GetText()==string("OPT"))
					{
						this->opt[index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]
												= index(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else 
						break;
					
					p_row = p_row->NextSiblingElement();
				}
				// Check that all the mandatory options are 
				// filled using the allowed options
				check_options();
				if (p_row && p_row->Value()== string("Row"))  
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					string name = p_data->GetText();
					if (p_data->GetText()==string("TABLE_SETTINGS"))  
					{
						// Management of the columns
						TiXmlElement * col;
						// Check if the TABLE_COL_INDEXES is present in the file
						col = h_row.ChildElement("Cell",1).FirstChild("Data").Element();
						if (col->GetText()!=string("TABLE_COL_INDEXES"))  
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Syntax error in the table. The keyword TABLE_COL_INDEXES in sheet %s is mandatory after the options", sheet_name.c_str());
							throw new GMXBException(message);	
						}
						int i = 2;
						while (col == h_row.ChildElement("Cell",i).FirstChild("Data").Element())
						{
							int col_key = atoi(col->GetText());						
							if(col_key > this->map_column_highIndex)
								this->map_column_highIndex = col_key;
							if(col_key < this->map_column_lowIndex)
								this->map_column_lowIndex = col_key;
							this->column_indices.push_back(col_key);
							i++;
						}
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_SETTINGS in sheet %s is mandatory after the options", sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
				// Handle the table rows
				p_row = p_row->NextSiblingElement();
				while (p_row && p_row->Value()==string("Row"))  
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if (p_data->GetText()==string("TABLE_ROW_INDEXES"))  
					{
						// get the row key
						int row_key = atoi(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText());
						if(row_key > this->map_row_highIndex)
							this->map_row_highIndex = row_key;
						if(row_key < this->map_row_lowIndex)
							this->map_row_lowIndex = row_key;
						this->row_indices.push_back(row_key);

						#ifdef __ZYK_OPT__
						map<int, double> col_map;
						#else
						map<double, double> col_map;
						#endif

						for (unsigned int col = 0; col < this->column_indices.size(); col++)
						{
							col_map[this->column_indices[col]] = atof(h_row.ChildElement("Cell",2 + col).FirstChild("Data")
																		.Element()->GetText());
						}
						file_to_map[row_key] = col_map;
						p_row = p_row->NextSiblingElement();
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Syntax error in the table. The keyword TABLE_ROW_INDEXES in sheet %s is mandatory after the options", sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
			}
		}

		set_index();
	}
#endif
	void set_index()
	{
		// store after last, before first (row and columns) indexes.
		// -1 in case we want to thow an exception 
		this->column_before_first_index = this->get_boundary_indexes(opt[ASSUMP_COL_BEF_FIRST], this->column_indices);
		this->column_after_last_index = this->get_boundary_indexes(opt[ASSUMP_COL_AFT_LAST], this->column_indices);
		this->row_before_first_index = this->get_boundary_indexes(opt[ASSUMP_ROW_BEF_FIRST], this->row_indices);
		this->row_after_last_index = this->get_boundary_indexes(opt[ASSUMP_ROW_AFT_LAST], this->row_indices);
	}
	#ifdef __ZYK_OPT__
	void reform()
	{
		_to2D_array = new double*[this->map_row_highIndex + 1];
		for (int i =0; i < this->map_row_highIndex + 1; ++i)
		{
			_to2D_array[i] = new double[this->map_column_highIndex + 1];
		}
		this->fill_2D_array();
	}
	#else
	void reform()
	{
		_to2D_array = new double*[(int)this->map_row_highIndex + 1];
		for (int i =0; i < (int)this->map_row_highIndex + 1; i++)
		{
			_to2D_array[i] = new double[(int)this->map_column_highIndex + 1];
		}
		this->fill_2D_array();
	}
	#endif
	int get_boundary_indexes(int option, const vector<int>& indices)
	{
		switch(option)
		{
		case ASSUMP_THROW_ERROR:
			return -1;
			break;
		case ASSUMP_FIRST:
			return indices[0];
			break;
		case ASSUMP_LAST:
			return indices[indices.size() - 1];
			break;
		default:
			return -1;
			break;
		}	
	}

	int get_missing_indexes(int option, const vector<int>& indices, int value_to_find)
	{
		switch(option)
		{
		case ASSUMP_THROW_ERROR:
			return -1;
			break;		
		case ASSUMP_PREVIOUS:
			{
			int indice = 0;
			for(unsigned int i = 0; i < indices.size(); i++)
			{
				if (indices[i] > value_to_find)
				{
					indice = indices[i-1];
					break;
				}
			}
			return indice;
			}
			break;
		default:
			return -1;
			break;
		}
	}
	#ifdef __ZYK_OPT__
	void fill_2D_array()
	{
		int last_row_indice = -1;
		
		for (vector<int>::iterator it_row_indices = this->row_indices.begin(); it_row_indices != this->row_indices.end(); ++it_row_indices)
		{
			int map_row_indice = *it_row_indices;
			for(int i = last_row_indice + 1; i <= map_row_indice; i++ )
			{
				int row_indice;
				if (i < this->map_row_lowIndex)
					row_indice = this->row_before_first_index;
				else if(i > this->map_row_highIndex)
					row_indice = this->row_after_last_index;
				else if (i == map_row_indice)
					row_indice = map_row_indice;
				else
					row_indice = this->get_missing_indexes(opt[ASSUMP_ROW_MISSING], this->row_indices, i);

				map<int, double>& _tab = this->file_to_map[row_indice];
				int last_column_indice = -1;
				for (vector<int>::iterator it_col_indices = this->column_indices.begin(); it_col_indices != this->column_indices.end(); ++it_col_indices)
				{
					int map_col_indice = *it_col_indices;
					for(int j = last_column_indice+1; j <= map_col_indice; j++)
					{
						int col_indice;
						if (j < this->map_column_lowIndex)
							col_indice = this->column_before_first_index;
						else if(j > this->map_column_highIndex)
							col_indice = this->column_after_last_index ;
						else if (j == map_col_indice)
							col_indice = map_col_indice;
						else
							col_indice = this->get_missing_indexes(opt[ASSUMP_COL_MISSING], this->column_indices, j);

						if (row_indice >= 0 && col_indice >=0 )
						{
							this->_to2D_array[i][j] = _tab[col_indice];
						}
						else
						{
							this->_to2D_array[i][j] = WRONG_DOUBLE;
						}
					}
					last_column_indice = map_col_indice;
				}
			}
			last_row_indice = map_row_indice;
		}
	}
	#else
	void fill_2D_array()
	{
		int last_row_indice = -1;
		vector<int>::iterator it_row_indices = this->row_indices.begin();
		while(it_row_indices != this->row_indices.end())
		{
			int map_row_indice = *it_row_indices;
			for(int i = last_row_indice + 1; i <= map_row_indice; i++ )
			{
				int row_indice;
				if (i < this->map_row_lowIndex)
					row_indice = this->row_before_first_index;
				else if(i > this->map_row_highIndex)
					row_indice = this->row_after_last_index;
				else if (i == map_row_indice)
					row_indice = map_row_indice;
				else
					row_indice = this->get_missing_indexes(opt[ASSUMP_ROW_MISSING], this->row_indices, i);

				int last_column_indice = -1;
				vector<int>::iterator it_col_indices = this->column_indices.begin();
				while(it_col_indices != this->column_indices.end())
				{
					int map_col_indice = *it_col_indices;
					for(int j = last_column_indice+1; j <= map_col_indice; j++)
					{
						int col_indice;
						if (j < this->map_column_lowIndex)
							col_indice = this->column_before_first_index;
						else if(j > this->map_column_highIndex)
							col_indice = this->column_after_last_index ;
						else if (j == map_col_indice)
							col_indice = map_col_indice;
						else
							col_indice = this->get_missing_indexes(opt[ASSUMP_COL_MISSING], this->column_indices, j);

						if (row_indice >= 0 && col_indice >=0 )
						{
							map<double, double> m = this->file_to_map[row_indice];
							this->_to2D_array[i][j] = m[col_indice];
						}
						else
						{
							this->_to2D_array[i][j] = WRONG_DOUBLE;
						}
					}
					last_column_indice = map_col_indice;
					it_col_indices++;
				}
			}
			last_row_indice = map_row_indice;
			it_row_indices++;
		}
	}
	#endif
public :

	GMXB_Assumption_Table(): map_row_highIndex(-INT_MAX), map_row_lowIndex(INT_MAX), map_column_highIndex(-INT_MAX), map_column_lowIndex(INT_MAX)
	{
		memset(opt, -1, sizeof(opt));
	}

	~GMXB_Assumption_Table()
	{
			for (int i =0; i < (int)this->map_row_highIndex + 1; i++)
			{
				delete[] _to2D_array[i];
			}
			delete _to2D_array;
	}
#if 1
	GMXB_Assumption_Table(const string& config_file_name, const string& sh_name)
	{
		for(int i = 0;i<ASSUMP_OPTNUM; i++)
			opt[i]=-1;

		this->map_row_highIndex = -_MAX_INDEX_;
		this->map_row_lowIndex = _MAX_INDEX_;
		this->map_column_highIndex = -_MAX_INDEX_;
		this->map_column_lowIndex = _MAX_INDEX_;

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
#endif
	double get_row_high_index()
	{
		return this->map_row_highIndex;	
	}

	double get_row_low_index()
	{
		return this->map_row_lowIndex;	
	}

	double get_column_high_index()
	{
		return this->map_column_highIndex;
	}

	double get_column_low_index()
	{
		return this->map_column_lowIndex;
	}

	string get_sheet_name()
	{
		return this->sheet_name;
	
	}
	inline double quick_search(int x, int y)
	{
		return this->_to2D_array[x][y];
	}

	inline double search(long long x, long long y)
	{
		int _x, _y;
		double value;
		// Determine x index
		if (x < this->row_before_first_index)
		{			
			if(this->row_before_first_index == -1)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "row %d is before first in table %s", x , sheet_name.c_str());
				throw new GMXBException(message);
			}
			else
			{
				_x = this->row_before_first_index;
			}
		}
		else if( x > this->row_after_last_index)
		{
			if (this->row_after_last_index == -1)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "row %d is after last in table %s", x , sheet_name.c_str());
				throw new GMXBException(message);			
			}
			else
			{
				_x = this->row_after_last_index;
			
			}
		}
		else
		{
			_x = (int)x;
		}
		// Determine y index
		if (y < this->column_before_first_index)
		{			
			if(this->column_before_first_index == -1)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "column %d is before first in table %s", y , sheet_name.c_str());
				throw new GMXBException(message);
			}
			else
			{
				_y = this->column_before_first_index;
			}
		}
		else if( y > this->column_after_last_index)
		{
			if (this->column_after_last_index == -1)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "column %d is after last in table %s", y , sheet_name.c_str());
				throw new GMXBException(message);			
			}
			else
			{
				_y = this->column_after_last_index;
			
			}
		}
		else
		{
			_y = (int)y;
		}
		// Determine missing values
		value = this->_to2D_array[_x][_y];

		if (value == WRONG_DOUBLE)
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Either %d or %d are missing in table %s", x,  y , sheet_name.c_str());
				throw new GMXBException(message);
		}

		return value;
	}


};
// Yaokun: Function to read all sheets in one xml file, in one time
GMXBParameters* parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, string para_sheet);