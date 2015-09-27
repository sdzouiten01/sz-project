#include "Utility classes/stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <math.h>
#include "GMXB_Assumption_Table.h"
#include "WildcardManager.h"
using namespace std;

extern "C"
{
	_declspec (dllimport) int run_calibration_standalone(string& config, ofstream& log_strm, int nb_of_workers, int idWorker, string& s);
	_declspec (dllimport) int run_esg_standalone(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);
	_declspec (dllimport) int TEST_NEW_CALIB(string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);
	_declspec (dllimport) int ALI_BL_esg_projection_V2(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);
	_declspec (dllimport) int run_HA_file_mode(string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);

}
int main(int argc, char ** argv)
{
	try
	{		
		//string config = argv[1];	
		
		string config = "C:\\Users\\b029735\\NoBackupData\\10_C++ Code New Version2013(working version)\\NEW_SETUP\\BALOISE_GMXB_EXECUTABLE.xml";
		int mode=2;
		int run=0;
		ofstream log_strm;
		int idWorker = 1;
		int nbWorker = 1;
		string s = "1,1";
		if (mode == 101)
		{
			cout << " Mode = " << "Testing New Calibration Mode " << endl;
			string s = "";
			run  = TEST_NEW_CALIB( config, log_strm, nbWorker, idWorker, s);
			if (run == 0)
				cout << "Error during the run. Check error log file. " << endl;
		}
		else if (mode == 2)
		{
			cout << "Mode = " << "DLL (ESG and Projection) mode" << endl;
			run = ALI_BL_esg_projection_V2(config, log_strm, nbWorker, idWorker, s);
			if (run == 0)
				cout << "Error during the run. Check error log file. " << endl;
		}
		if (mode == 1)
		{
			cout << "Mode = " << "DLL (Projection file) mode" << endl;
			run = run_HA_file_mode(config, log_strm, nbWorker, idWorker, s);
			if (run == 0)
				cout << "Error during the run. Check error log file. " << endl;
		}



		/*else if (mode == 3)
		{
			cout << "Mode = " << "DLL (ESG stand alone) mode" << endl;
			run = run_esg_standalone(config, log_strm, nbWorker, idWorker, s);		
		}		
*/
		
		//
		//string s_he_price = config;
		//string s_he_sum= "1," + config;

		//WildcardManager wm( config, "wildcards" );
		//GMXBParameters parameter_main(config, "main_application" );

		//int mode = atoi( wm.getPath(parameter_main.search("Mode")).c_str() );
		//ofstream log_strm( wm.getPath(parameter_main.search("Log")).c_str() );
		//int nbWorker = 1; //atoi( wm.getPath(parameter_main.search("NbWorker")).c_str() );

  //      int idWorker = 1;
		//int scen_pass_number = 0;

		//double alpha = -1;
		//double sigma = -1;
		//double*** va_scen = 0;
		//int total_period = 721;
		//clock_t start, end;
		//start = clock();
		//string s = "1,1";

		//int run=0;
		//if (mode == 1)
		//{
		//	cout << "Mode = " << "DLL (Projection file) mode" << endl;
		//	run = run_HA_file_mode(config, log_strm, nbWorker, idWorker, s);
		//}
		//else if (mode == 8)
		//{
		//	cout << "Mode = " << "DLL (ESG and Projection) mode" << endl;
		//	run = ALI_BL_esg_projection(config, log_strm, nbWorker, idWorker, s);
		//}
		//else if (mode == 2)
		//{
		//	cout << "Mode = " << "DLL (ESG and Projection) mode" << endl;
		//	run = run_HA_esg_mode(config, log_strm, nbWorker, idWorker, s);
		//}
	}
	catch(GMXBException *g)
	{
		cout <<g->get_message() << endl;
		getchar();
	}
	return 1;
}
