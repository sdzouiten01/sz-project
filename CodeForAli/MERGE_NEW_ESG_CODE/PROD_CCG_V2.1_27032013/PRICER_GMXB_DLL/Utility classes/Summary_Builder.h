#pragma once
#include <string>

#include "Summary classes/Summary_File_Writer.h"
//#include "Summary classes/Country summary classes/Germany_Summary_File_Writer.h"
//#include "Summary classes/Country summary classes/AGD_UK_WB_Summary_File_Writer.h"
//#include "Summary classes/Country summary classes/AGD_UK_AB_Summary_File_Writer.h"
//#include "Summary classes/Country summary classes/AUS_WB_Summary_File_Writer.h"
#include "Utility classes/Dispatch_Manager.h"
#include "Utility classes/HE_Writter.h"
#include "Utility classes/HE_Summary_File_Writer.h"
//#include "Utility classes/Germany_HE_Summary_File_Writer.h"


Summary_File_Writer* summary_builder(const string& pCK,	int nb_workers, Summary_configuration *summary, 
					const map<string, vector<string>>& map_scenario_names, 
					const map<string, vector<string>>& map_index_shock_names, Dispatch_Manager* dispatch_manager)
{
		Summary_File_Writer* summary_file_writer;

		//if (pCK == "DEU_IDB01" ||pCK == "DEU_IDB02" ||pCK == "DEU_IAB01" ||pCK == "DEU_IAB02" ||pCK == "DEU_IDB03" ||pCK == "DEU_IAB03" || pCK == "DEU_IDB04" ||pCK == "DEU_IAB04"||pCK == "DEU_IAB05"||pCK == "DEU_IDB05")
		//{
		//	summary_file_writer = new Germany_Summary_File_Writer(nb_workers, summary,
		//	map_scenario_names, map_index_shock_names, dispatch_manager);
		//}
		/*else if (pCK == "AGD_UK_WDB01")
		{
			summary_file_writer = new AGD_UK_WB_Summary_File_Writer(nb_workers, summary, 
				map_scenario_names, map_index_shock_names, dispatch_manager);
		}
		else if (pCK == "AGD_UK_ADB01")
		{
			summary_file_writer = new AGD_UK_AB_Summary_File_Writer(nb_workers, summary, 
				map_scenario_names, map_index_shock_names, dispatch_manager);
		}*/
		//else if (pCK == "AUS_WB01")//ml ajout output
		//{
		//	summary_file_writer = new AUS_WB_Summary_File_Writer(nb_workers, summary,
		//	map_scenario_names, map_index_shock_names, dispatch_manager);
		//}
		//else
		//{
			summary_file_writer = new Summary_File_Writer(nb_workers, summary,
														  map_scenario_names, map_index_shock_names, dispatch_manager);
		//}
		return summary_file_writer;
}

HE_Summary_File_Writer * he_summary_builder (const string& pCK)
{

	HE_Summary_File_Writer* he_summary_file_writer;

	//if (pCK == "DEU_IDB01" ||pCK == "DEU_IDB02" ||pCK == "DEU_IAB01" ||pCK == "DEU_IAB02")
	//{
	//	// Add here the constructor of germany he summary
	//	he_summary_file_writer = new Germany_HE_Summary_File_Writer();
	//}
	//else if(pCK == "DEU_IDB03" ||pCK == "DEU_IAB03" || pCK == "DEU_IDB04" ||pCK == "DEU_IAB04"|| pCK == "DEU_IDB05" ||pCK == "DEU_IAB05")
	//{
	//	he_summary_file_writer = new Germany_HE_Summary_File_Writer_05();	
	//}
	//else
	//{
		he_summary_file_writer = new HE_Summary_File_Writer();
	//}
	return  he_summary_file_writer;

}

HE_Writter* build_he_writter(const string& pCK)
{
	//if (pCK == "FRC_WB01" )
	//{
	//	return new HE_Writter_FRC();
	//}
	//if (pCK == "DEU_IDB01" ||pCK == "DEU_IDB02" ||pCK == "DEU_IAB01" ||pCK == "DEU_IAB02" )
	//{
	//	return new HE_Writter_DEU();
	//}
	//else if(pCK == "DEU_IDB03" || pCK == "DEU_IAB03" || pCK == "DEU_IDB04" ||pCK == "DEU_IAB04"||pCK == "DEU_IDB05"||pCK == "DEU_IAB05")
	//{
	//	return new HE_Writter_DEU_05();
	//}
	//else
	//{
		return new HE_Writter();
	//}
}