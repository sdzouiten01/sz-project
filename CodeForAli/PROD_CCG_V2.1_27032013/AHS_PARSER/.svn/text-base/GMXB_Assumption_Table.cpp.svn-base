#include "GMXB_Assumption_Table.h"
#include "GMXBParameters.h"
#include <iostream>
#include <string>

using namespace std;

#ifdef __ZYK_OPT__
GMXBParameters* parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, string para_sheet)
{
	GMXBParameters* p_para = 0;
	TiXmlDocument doc(xml.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", xml.c_str());
		throw new GMXBException(message);
	}

	int total_sheets = tables.size();

	if (para_sheet != "")
		++total_sheets;

	int cur_parsed_sheets = 0;
	string cur_sheet_name;
	map<string, GMXB_Assumption_Table*>::const_iterator end_iter = tables.end();

	TiXmlHandle doch(&doc);
	TiXmlElement* p_root = doch.FirstChildElement().Element();
	TiXmlHandle h_root(p_root);
	TiXmlElement* p_worksheet = h_root.FirstChild("Worksheet").Element();
	
	TiXmlElement* p_table;
	GMXB_Assumption_Table* p_assump_table;

	const char* s_ROW = "Row";
	const char* s_DATA = "DATA";
	const char* s_OPT = "OPT";
	const char* s_TABLE_ROW_INDEXES = "TABLE_ROW_INDEXES";
	const char* s_TABLE_COL_INDEXES = "TABLE_COL_INDEXES";
	const char* s_TABLE_SETTINGS = "TABLE_SETTINGS";

	while(p_worksheet && cur_parsed_sheets != total_sheets)
	{
		cur_sheet_name = p_worksheet->Attribute("ss:Name");
		// current sheet is not in our map, go on to next sheet, otherwise parse it
		
		if (cur_sheet_name == para_sheet)
		{
			p_para = new GMXBParameters();

			TiXmlHandle h_worksheet(p_worksheet);
			TiXmlElement * p_table = h_worksheet.FirstChild("Table").Element();
			if(p_table)
			{
				TiXmlHandle h_table(p_table);
				TiXmlElement * p_row = h_table.ChildElement("Row", 0).Element();
				while(p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					TiXmlElement * p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if( !strcmp(p_data->GetText(), s_DATA) )
					{
						p_para->insert(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText(), h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else 
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Only the keyword DATA is allowed in the sheet %s", cur_sheet_name.c_str());
						throw new GMXBException(message);
					}
					p_row = p_row->NextSiblingElement();
				}
			}
		}
		else
		{
			if (tables.find(cur_sheet_name) == end_iter)
			{
				// continue to parse next sheet
				p_worksheet = p_worksheet->NextSiblingElement();
				continue;
			}
			p_assump_table = new GMXB_Assumption_Table();
			p_assump_table->sheet_name = cur_sheet_name;
			p_assump_table->file_name = xml;
			tables[cur_sheet_name] = p_assump_table;

			TiXmlHandle h_worksheet(p_worksheet);
			p_table = h_worksheet.FirstChild("Table").Element();

			if(p_table)
			{
				TiXmlHandle h_table(p_table);
				TiXmlElement* p_first_row = h_table.ChildElement("Row", 0).Element();
				if(!p_first_row)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The first row of sheet %s should not be empty!", cur_sheet_name.c_str());
					throw new GMXBException(message);
				}
				// Handle all the options. Options are not mandatory 
				TiXmlElement* p_data;
				TiXmlElement* p_row = p_first_row;

				string tmp =  p_row->Value() ;
				while(p_row &&  !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(), s_OPT) )
					{
						p_assump_table->opt[GMXB_Assumption_Table::index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]
						= GMXB_Assumption_Table::index(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else
						break;

					p_row = p_row->NextSiblingElement();
				}
				// Check that all the mandatory options are 
				// filled using the allowed options
				p_assump_table->check_options();
				if (p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(),  s_TABLE_SETTINGS)  ) 
					{
						// Management of the columns
						TiXmlElement * col;
						// Check if the TABLE_COL_INDEXES is present in the file
						col = h_row.ChildElement("Cell",1).FirstChild("Data").Element();
						if ( strcmp(col->GetText(), s_TABLE_COL_INDEXES) )
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Syntax error in the table. The keyword TABLE_COL_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
							throw new GMXBException(message);	
						}
						int i = 2;
						col = h_row.ChildElement("Cell",i++).FirstChild("Data").Element();
						while (col)
						{
							int col_key = atoi(col->GetText());
							if(col_key > p_assump_table->map_column_highIndex)
								p_assump_table->map_column_highIndex = col_key;
							if(col_key < p_assump_table->map_column_lowIndex)
								p_assump_table->map_column_lowIndex = col_key;
							p_assump_table->column_indices.push_back(col_key);

							col = h_row.ChildElement("Cell",i++).FirstChild("Data").Element();
						}
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_SETTINGS in sheet %s is mandatory after the options", cur_sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
				// Handle the table rows
				p_row = p_row->NextSiblingElement();
				size_t ind_size = p_assump_table->column_indices.size();
				while (p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(), s_TABLE_ROW_INDEXES) )
					{
						// get the row key
						int row_key = atoi(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText());
						map<int, double>& col_map = p_assump_table->file_to_map[row_key];
						if(row_key > p_assump_table->map_row_highIndex)
							p_assump_table->map_row_highIndex = row_key;
						if(row_key < p_assump_table->map_row_lowIndex)
							p_assump_table->map_row_lowIndex = row_key;
						p_assump_table->row_indices.push_back(row_key);

						for (unsigned int col = 0; col < ind_size; ++col )
						{
							col_map[p_assump_table->column_indices[col]] = atof(h_row.ChildElement("Cell",2 + col).FirstChild("Data")
								.Element()->GetText());
						}
						p_row = p_row->NextSiblingElement();
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_ROW_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
			}
			// store after last, before first (row and columns) indexes.
			// -1 in case we want to thow an exception 
			p_assump_table->set_index();
		}

		// parse next sheets
		p_worksheet = p_worksheet->NextSiblingElement();
		++cur_parsed_sheets;
	}

	for ( map<string, GMXB_Assumption_Table*>::iterator iter = tables.begin(); iter != end_iter; ++iter )
	{
		iter->second->reform();
	}
	return p_para;
}
void parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, map<string, GMXBParameters*>& tables_para)
{
	GMXBParameters* p_para = 0;
	TiXmlDocument doc(xml.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", xml.c_str());
		throw new GMXBException(message);
	}

	int nb_tab = tables.size();
	int nb_para = tables_para.size();
	int nb_total = nb_tab + nb_para;

	string cur_sheet_name;
	map<string, GMXB_Assumption_Table*>::const_iterator end_iter = tables.end();
	map<string, GMXBParameters*>::const_iterator end_iter_para = tables_para.end();

	TiXmlHandle doch(&doc);
	TiXmlElement* p_root = doch.FirstChildElement().Element();
	TiXmlHandle h_root(p_root);
	TiXmlElement* p_worksheet = h_root.FirstChild("Worksheet").Element();
	
	TiXmlElement* p_table;
	GMXB_Assumption_Table* p_assump_table;

	const char* s_ROW = "Row";
	const char* s_DATA = "DATA";
	const char* s_OPT = "OPT";
	const char* s_TABLE_ROW_INDEXES = "TABLE_ROW_INDEXES";
	const char* s_TABLE_COL_INDEXES = "TABLE_COL_INDEXES";
	const char* s_TABLE_SETTINGS = "TABLE_SETTINGS";

	while(p_worksheet && nb_total)
	{
		cur_sheet_name = p_worksheet->Attribute("ss:Name");
		// current sheet is not in our map, go on to next sheet, otherwise parse it
		
		if (nb_para && tables_para.find(cur_sheet_name) != end_iter_para)
		{
			p_para = new GMXBParameters();
			tables_para[cur_sheet_name] = p_para;

			TiXmlHandle h_worksheet(p_worksheet);
			TiXmlElement * p_table = h_worksheet.FirstChild("Table").Element();
			if(p_table)
			{
				TiXmlHandle h_table(p_table);
				TiXmlElement * p_row = h_table.ChildElement("Row", 0).Element();
				while(p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					TiXmlElement * p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if( !strcmp(p_data->GetText(), s_DATA) )
					{
						p_para->insert(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText(), h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else 
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Only the keyword DATA is allowed in the sheet %s", cur_sheet_name.c_str());
						throw new GMXBException(message);
					}
					p_row = p_row->NextSiblingElement();
				}
			}
			--nb_para;
			--nb_total;
			continue;
		}
		if (nb_tab)
		{
			if (tables.find(cur_sheet_name) == end_iter)
			{
				// continue to parse next sheet
				p_worksheet = p_worksheet->NextSiblingElement();
				continue;
			}
			p_assump_table = new GMXB_Assumption_Table();
			p_assump_table->sheet_name = cur_sheet_name;
			p_assump_table->file_name = xml;
			tables[cur_sheet_name] = p_assump_table;

			TiXmlHandle h_worksheet(p_worksheet);
			p_table = h_worksheet.FirstChild("Table").Element();

			if(p_table)
			{
				TiXmlHandle h_table(p_table);
				TiXmlElement* p_first_row = h_table.ChildElement("Row", 0).Element();
				if(!p_first_row)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The first row of sheet %s should not be empty!", cur_sheet_name.c_str());
					throw new GMXBException(message);
				}
				// Handle all the options. Options are not mandatory 
				TiXmlElement* p_data;
				TiXmlElement* p_row = p_first_row;

				string tmp =  p_row->Value() ;
				while(p_row &&  !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(), s_OPT) )
					{
						p_assump_table->opt[GMXB_Assumption_Table::index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]
						= GMXB_Assumption_Table::index(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else
						break;

					p_row = p_row->NextSiblingElement();
				}
				// Check that all the mandatory options are 
				// filled using the allowed options
				p_assump_table->check_options();
				if (p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(),  s_TABLE_SETTINGS)  ) 
					{
						// Management of the columns
						TiXmlElement * col;
						// Check if the TABLE_COL_INDEXES is present in the file
						col = h_row.ChildElement("Cell",1).FirstChild("Data").Element();
						if ( strcmp(col->GetText(), s_TABLE_COL_INDEXES) )
						{
							char message[MAX_LENGTH_MESSAGE_EXCEPTION];
							sprintf_s(message, "Syntax error in the table. The keyword TABLE_COL_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
							throw new GMXBException(message);	
						}
						int i = 2;
						col = h_row.ChildElement("Cell",i++).FirstChild("Data").Element();
						while (col)
						{
							int col_key = atoi(col->GetText());
							if(col_key > p_assump_table->map_column_highIndex)
								p_assump_table->map_column_highIndex = col_key;
							if(col_key < p_assump_table->map_column_lowIndex)
								p_assump_table->map_column_lowIndex = col_key;
							p_assump_table->column_indices.push_back(col_key);

							col = h_row.ChildElement("Cell",i++).FirstChild("Data").Element();
						}
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_SETTINGS in sheet %s is mandatory after the options", cur_sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
				// Handle the table rows
				p_row = p_row->NextSiblingElement();
				size_t ind_size = p_assump_table->column_indices.size();
				while (p_row && !strcmp(p_row->Value(), s_ROW) )
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if ( !strcmp(p_data->GetText(), s_TABLE_ROW_INDEXES) )
					{
						// get the row key
						int row_key = atoi(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText());
						map<int, double>& col_map = p_assump_table->file_to_map[row_key];
						if(row_key > p_assump_table->map_row_highIndex)
							p_assump_table->map_row_highIndex = row_key;
						if(row_key < p_assump_table->map_row_lowIndex)
							p_assump_table->map_row_lowIndex = row_key;
						p_assump_table->row_indices.push_back(row_key);

						for (unsigned int col = 0; col < ind_size; ++col )
						{
							col_map[p_assump_table->column_indices[col]] = atof(h_row.ChildElement("Cell",2 + col).FirstChild("Data")
								.Element()->GetText());
						}
						p_row = p_row->NextSiblingElement();
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_ROW_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
			}
			// store after last, before first (row and columns) indexes.
			// -1 in case we want to thow an exception 
			p_assump_table->set_index();
			--nb_tab;
			--nb_total;
		}
		// parse next sheets
		p_worksheet = p_worksheet->NextSiblingElement();
	}

	for ( map<string, GMXB_Assumption_Table*>::iterator iter = tables.begin(); iter != end_iter; ++iter )
	{
		iter->second->reform();
	}
}
#else
GMXBParameters* parse_sheets(string xml, map<string, GMXB_Assumption_Table*>& tables, string para_sheet)
{
	GMXBParameters* p_para = 0;
	TiXmlDocument doc(xml.c_str());
	bool loadOK = doc.LoadFile();
	if(!loadOK) 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Unable to load the file %s. Check if it is open!", xml.c_str());
		throw new GMXBException(message);
	}

	int total_sheets = tables.size()+1;

	if (para_sheet != "")
		++total_sheets;

	int cur_parsed_sheets = 0;
	string cur_sheet_name;
	map<string, GMXB_Assumption_Table*>::const_iterator end_iter = tables.end();

	TiXmlHandle doch(&doc);
	TiXmlElement* p_root = doch.FirstChildElement().Element();
	TiXmlHandle h_root(p_root);
	TiXmlElement* p_worksheet = h_root.FirstChild("Worksheet").Element();
	
	TiXmlElement* p_table;
	GMXB_Assumption_Table* p_assump_table;
	while(p_worksheet && cur_parsed_sheets != total_sheets)
	{
		cur_sheet_name = p_worksheet->Attribute("ss:Name");
		// current sheet is not in our map, go on to next sheet, otherwise parse it
		
		if (cur_sheet_name == para_sheet)
		{
			p_para = new GMXBParameters();

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
						p_para->insert(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText(), h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else 
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Only the keyword DATA is allowed in the sheet %s", cur_sheet_name.c_str());
						throw new GMXBException(message);
					}
					p_row = p_row->NextSiblingElement();
				}
			}
		}
		else
		{
			if (tables.find(cur_sheet_name) == end_iter)
			{
				// continue to parse next sheet
				p_worksheet = p_worksheet->NextSiblingElement();
				continue;
			}
			p_assump_table = new GMXB_Assumption_Table();
			p_assump_table->sheet_name = cur_sheet_name;
			p_assump_table->file_name = xml;
			tables[cur_sheet_name] = p_assump_table;

			TiXmlHandle h_worksheet(p_worksheet);
			p_table = h_worksheet.FirstChild("Table").Element();

			if(p_table)
			{
				TiXmlHandle h_table(p_table);
				TiXmlElement* p_first_row = h_table.ChildElement("Row", 0).Element();
				if(!p_first_row)
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The first row of sheet %s should not be empty!", cur_sheet_name.c_str());
					throw new GMXBException(message);
				}
				// Handle all the options. Options are not mandatory 
				TiXmlElement* p_data;
				TiXmlElement* p_row = p_first_row;

				while(p_row && p_row->Value()== string("Row"))
				{
					TiXmlHandle h_row(p_row);
					p_data = h_row.ChildElement("Cell",0).FirstChild("Data").Element();
					if (p_data->GetText()==string("OPT"))
					{
						p_assump_table->opt[GMXB_Assumption_Table::index(h_row.ChildElement("Cell",1).FirstChild("Data").Element()->GetText())]
						= GMXB_Assumption_Table::index(h_row.ChildElement("Cell",2).FirstChild("Data").Element()->GetText());
					}
					else
						break;

					p_row = p_row->NextSiblingElement();
				}
				// Check that all the mandatory options are 
				// filled using the allowed options
				p_assump_table->check_options();
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
							sprintf_s(message, "Syntax error in the table. The keyword TABLE_COL_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
							throw new GMXBException(message);	
						}
						int i = 2;
						while (col = h_row.ChildElement("Cell",i).FirstChild("Data").Element())
						{
							int col_key = atoi(col->GetText());						
							if(col_key > p_assump_table->map_column_highIndex)
								p_assump_table->map_column_highIndex = col_key;
							if(col_key < p_assump_table->map_column_lowIndex)
								p_assump_table->map_column_lowIndex = col_key;
							p_assump_table->column_indices.push_back(col_key);
							i++;
						}
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_SETTINGS in sheet %s is mandatory after the options", cur_sheet_name.c_str());
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
						if(row_key > p_assump_table->map_row_highIndex)
							p_assump_table->map_row_highIndex = row_key;
						if(row_key < p_assump_table->map_row_lowIndex)
							p_assump_table->map_row_lowIndex = row_key;
						p_assump_table->row_indices.push_back(row_key);
						map<double, double> col_map;
						for (unsigned int col = 0; col < p_assump_table->column_indices.size(); col++)
						{
							col_map[p_assump_table->column_indices[col]] = atof(h_row.ChildElement("Cell",2 + col).FirstChild("Data")
								.Element()->GetText());
						}
						p_assump_table->file_to_map[row_key] = col_map;
						p_row = p_row->NextSiblingElement();
					}
					else
					{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "Syntax error in the table. The keyword TABLE_ROW_INDEXES in sheet %s is mandatory after the options", cur_sheet_name.c_str());
						throw new GMXBException(message);				
					}
				}
			}
			// store after last, before first (row and columns) indexes.
			// -1 in case we want to thow an exception 
			p_assump_table->set_index();
		}

		// parse next sheets
		p_worksheet = p_worksheet->NextSiblingElement();
		++cur_parsed_sheets;
	}

	for ( map<string, GMXB_Assumption_Table*>::iterator iter = tables.begin(); iter != end_iter; ++iter )
	{
		iter->second->reform();
	}
	return p_para;
}
#endif