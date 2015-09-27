#pragma once

#include "GMXBException.h"
#include <string>
#include <map>
#include <vector>

//! The safe_read functions provide garantee of giveing Error message (by throwing exception) when programme crash.

using namespace std;

//! ???????? TODO: we can use template to do this ! 

string safe_map_read(const map<string,vector<string>> & map_ip,
				     const string& s_ip, 
				     const unsigned int index_ip,
				     const string& map_name_ip = "")      //! the map name is optional
{
	map<string,vector<string>>::const_iterator itr_end = map_ip.end();
    map<string,vector<string>>::const_iterator itr     = map_ip.find(s_ip);
    
	if(itr != itr_end)  // found string
	{                                
		if(itr->second.size() > index_ip)    
			return itr->second[index_ip];
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Map_read Error: the element %s of map %s 's size is too small" , s_ip.c_str(), map_name_ip.c_str());
			throw new GMXBException(message);
		}
	}
	else              // not found string
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Map_read Error: the element %s is not found in map %s", s_ip.c_str(), map_name_ip.c_str());
		throw new GMXBException(message);
	}
}



////! some modification of Yuan 
//In File Interface_dll.cpp: 
//Three functions are modified for load_condig    
//       		1. load_run_configuration         // For file mode and Moses
//    		2. load_run_configuration_esg     // For ESG
//    		3. load_run_configuration_outer   // Hedg Effect
//    		
//    		where four maps' read are not using: safe_map_read
//    		    -- map_products_configuration
//    			-- map_of_shock_settings
//    			-- map_run_settings
//    			-- map_bucket_list (only for load_run_configuration_esg)
    		
    		