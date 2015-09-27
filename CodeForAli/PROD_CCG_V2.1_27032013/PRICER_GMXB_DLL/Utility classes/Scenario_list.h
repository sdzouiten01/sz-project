/*
This class should be changed to map !  OK
Rewrite the searching algo, a loop is bad idea!!! 
*/

#pragma once

#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <fstream>
#include "WildcardManager.h"
#include "GMXBExcelXMLReader.h"

using namespace std;


const unsigned int scenario_list_nCol = 1; // [0]=ESG_scenario_config, [1]=ESG_calibration_config


class Scenario_list
{
	map<int,vector<string>> map_scnPn_ESGCalibPath;
 
public:
	Scenario_list(){};

	Scenario_list(string& parameter_file)  
	{

		 GMXBExcelXMLReader xml_reader(parameter_file);
		 WildcardManager wm( parameter_file, "wildcards" );

		 map<string, vector<string> > map_scenario_list_ip;
		 xml_reader.parse_XML_sheet("scenario_list", map_scenario_list_ip);

		 //scenario_pn.reserve(20);
		 //ESG_calib_config_path.reserve(20);

		 for(map<string, vector<string> >::iterator map_iterator = map_scenario_list_ip.begin();
			               map_iterator!=map_scenario_list_ip.end();
						   map_iterator++)
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

	string get_ESG_config_path(int scenario_pn_ip)
	{
		for(map<int,vector<string>>::iterator itr = map_scnPn_ESGCalibPath.begin();
			            itr != map_scnPn_ESGCalibPath.end(); itr++)
		{
		    if(scenario_pn_ip == itr->first)
				return itr->second[0];
		}
		return "Error";
	}


    void print()
	{
		for(map<int,vector<string>>::iterator itr = map_scnPn_ESGCalibPath.begin();
			            itr != map_scnPn_ESGCalibPath.end(); itr++)
		{
		     	cout <<         "scen_pn = " << itr->first <<"  :";
				cout << "     ESG_path = "   << itr->second[0];
		}
	}

};



//class Scenario_list
//{
//	vector<int>               scenario_pn;
//	vector<vector<string>>    ESG_calib_config_path;
// 
//public:
//	Scenario_list(){};
//
//	Scenario_list(string& parameter_file)  // : scenario_pn(),ESG_calib_config_path()  // not really necessary
//	{
//
//		 GMXBExcelXMLReader xml_reader(parameter_file);
//		 WildcardManager wm( parameter_file, "wildcards" );
//
//		 map<string, vector<string> > map_scenario_list_ip;
//		 xml_reader.parse_XML_sheet("scenario_list", map_scenario_list_ip);
//		 //scenario_list_table = Scenario_list(wm,map_Scenario_list);
//
//		 scenario_pn.reserve(20);
//		 ESG_calib_config_path.reserve(20);
//
//		 for(map<string, vector<string> >::iterator map_iterator = map_scenario_list_ip.begin();
//			               map_iterator!=map_scenario_list_ip.end();
//						   map_iterator++)
//		 {   
//			 string s = map_iterator->first;
//			 //! l'entete
//			 if(s == "scenario_pass_number")
//				 continue;
//
//			 scenario_pn.push_back(atoi(s.c_str()));
//			 
//			 // TODO: replace it with an exception
//			 if(map_iterator->second.size() != scenario_list_nCol )
//			    cout << "Error size of the map_iterator->second != 2 in constructor of class Scenario_list(...) "  << endl;  
//			 
//			 vector<string> scenario_config(map_iterator->second);
//			 scenario_config[0] = wm.getPath(scenario_config[0]);
//			 scenario_config[1] = wm.getPath(scenario_config[1]);
//			 
//			 //! test if file exist
//			 ifstream file_1(scenario_config[0].c_str() , ios::in);
//			 ifstream file_2(scenario_config[1].c_str() , ios::in);
//			 //! TODO throw exception
//			 if(!file_1 || ! file_2)
//			    cout << "Error ESG/calibration file doesn't exist Error in Scenario_list(...) "  << endl;  
//
//			 ESG_calib_config_path.push_back(scenario_config);
//		 }
//	}
//
//	//int get_scenario_pass_number(int scenario_pn_ip)
//	//{
//	//	vector<int>::iterator itr = find(scenario_pn.begin(), scenario_pn.end(), scenario_pn_ip);
//	//	if(itr != scenario_pn.end()) // found
//	//		return *itr;
//	//	else
//	//	{
//	//		 //! TODO Throw exception
//	//	     cout << "Error get_scenario_pass_number" << endl;
//	//		 getchar();
//	//		 return -1;
//	//	}
//	//}
//
//	string get_ESG_config_path(int scenario_pn_ip)
//	{
//		vector<int>::iterator itr = find(scenario_pn.begin(), scenario_pn.end(), scenario_pn_ip);
//		if(itr != scenario_pn.end()) // found
//		{
//			 return  ESG_calib_config_path[*itr][0];
//		}
//		else
//		{
//			 //! TODO Throw exception
//		     cout << "Error get_scenario_pass_number" << endl;
//			 getchar();
//			 return "Error";
//		}
//	}
//
//	string get_Calibration_config_path(int scenario_pn_ip)
//	{
//		vector<int>::iterator itr = find(scenario_pn.begin(), scenario_pn.end(), scenario_pn_ip);
//		if(itr != scenario_pn.end())
//		{// found
//			 return  ESG_calib_config_path[*itr][1];
//		}
//		else
//		{
//			 //! TODO Throw exception
//		     cout << "Error get_scenario_pass_number" << endl;
//			 getchar();
//			 return "Error";
//		}
//	}
//	
//
//    void print()
//	{
//		for(size_t i=0; i<scenario_pn.size(); i++)
//		{
//			cout << "scen_pn = " << scenario_pn[i]<<"  :";
//				cout << "     ESG_path = "   << ESG_calib_config_path[i][0];
//			    cout << "     Calib_path = " << ESG_calib_config_path[i][1]<< "\n" << endl;
//		}
//
//	}
//
//};