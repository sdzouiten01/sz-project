#pragma once

//#include "Utility classes/Fra_Stat_Inforce_Manager.h"
//#include "Utility classes/German_Stat_Inforce_Manager.h"
//#include "Utility classes/German_IADB05_Stat_Inforce_Manager.h" 
#include "Utility classes/Stat_Inforce_Manager.h"
//#include "Utility classes/Stat_Inforce_Manager_AUS.h"
#include <string>



Stat_Inforce_Manager* build_stat_inforce_manager(const string& product_code_key)
{

	Stat_Inforce_Manager * stat_inforce_mgr;

	//if(product_code_key == "DEU_IDB01" ||product_code_key == "DEU_IDB02" 
	//	||product_code_key == "DEU_IAB01" ||product_code_key == "DEU_IAB02")
	//{
	//	stat_inforce_mgr = new German_Stat_Inforce_Manager();
	//}
	//else if(product_code_key == "DEU_IDB03"	||product_code_key == "DEU_IAB03" 
	//	||product_code_key == "DEU_IDB04" ||product_code_key == "DEU_IAB04" 
	//	||product_code_key == "DEU_IDB05" ||product_code_key == "DEU_IAB05")
	//
	//{
	//	stat_inforce_mgr = new German_IADB05_Stat_Inforce_Manager();
	//}
	//else if(product_code_key == "FRA_ALWB01")
	//{
	//	stat_inforce_mgr = new Fra_Stat_Inforce_Manager();
	//}
	//else if( product_code_key == "AUS_AB01" || product_code_key == "AUS_AB01N" 
	//			|| product_code_key == "AUS_AB01NR"
	//			|| product_code_key == "AUS_AB01R" || product_code_key == "AUS_AB02"
	//			|| product_code_key == "AUS_AB02R" || product_code_key == "AUS_PG01"
	//			|| product_code_key == "AUS_PG01N" || product_code_key == "AUS_PG02" 
	//			|| product_code_key == "AUS_PI01"
	//			|| product_code_key == "AUS_PI01N" || product_code_key == "AUS_PI02" 
	//			|| product_code_key == "AUS_SWB01" || product_code_key == "AUS_SWB01N"
	//			|| product_code_key == "AUS_SWB02" || product_code_key == "AUS_WB01")
	//{

	//	stat_inforce_mgr = new Stat_Inforce_Manager_AUS();
	//}
	//else 
	//{
		stat_inforce_mgr = new Stat_Inforce_Manager();
	//}

	return stat_inforce_mgr;
}



