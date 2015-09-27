#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <math.h>
#include "InterfaceCalibration.h"
#include "InterfaceEsg.h"
#include "WildcardManager.h"
using namespace std;


extern "C"
{
_declspec (dllimport) int ALI_BL_esg_projection(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);

_declspec (dllimport) int InterfacePriceHE( string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);
_declspec (dllimport) int InterfaceSummarizeHE(string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);

_declspec (dllimport) int InterfacePriceHeAsset(string& config, ofstream& log_strm, int tot_nb_workers, int w_id, string& s);
_declspec (dllimport) int InterfaceSummarizeHeAsset(string& config, ofstream& log_strm, int tot_nb_workers, int w_id, string& s);
_declspec (dllimport) int stat_inforce(string& parameter_file, ofstream& log_strm, int nb_of_workers, int w_id, string& s);

_declspec (dllimport) int run_RA_mode(string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);
_declspec (dllimport) int run_calibration_standalone(string& config, ofstream& log_strm, int nb_of_workers, int idWorker, string& s);

_declspec (dllimport) int run_HA_file_mode(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);
_declspec (dllimport) int run_HA_esg_mode(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);


_declspec (dllimport) int run_esg_standalone(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);


_declspec (dllimport) int run_aging_policies( string& config, ofstream& log_strm, int tot_nb_workers, int w_id, string& s);

_declspec (dllimport) int delete_intermediate_files(string& parameter_file, ofstream& log_strm, int nbnb_of_workers_workers, int w_id, string& s);
_declspec (dllimport) int check_assumptions(string& parameter_file, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);
_declspec (dllimport) int copy_assumptions(string& parameter_file, ofstream& log_strm, int nb_of_workers, int w_id, string& s);

}
int main(int argc, char ** argv)
{
	try
	{		
		//string config = argv[1];
		

		
		string config = "C:\\Users\\b029735\\NoBackupData\\12_MONTHLY_PROCESS\\BALOISE_GMXB_EXECUTABLE.xml";
		
		string s_he_price = config;
		string s_he_sum= "1," + config;

		WildcardManager wm( config, "wildcards" );
		GMXBParameters parameter_main(config, "main_application" );

		int mode = atoi( wm.getPath(parameter_main.search("Mode")).c_str() );
		ofstream log_strm( wm.getPath(parameter_main.search("Log")).c_str() );
		int nbWorker = 1; //atoi( wm.getPath(parameter_main.search("NbWorker")).c_str() );

        int idWorker = 1;
		int scen_pass_number = 0;

		double alpha = -1;
		double sigma = -1;
		double*** va_scen = 0;
		int total_period = 721;
		clock_t start, end;
		start = clock();
		string s = "1,1";

		int run=0;
		if (mode == 1)
		{
			cout << "Mode = " << "DLL (Projection file) mode" << endl;
			run = run_HA_file_mode(config, log_strm, nbWorker, idWorker, s);
		}
		else if (mode == 8)
		{
			cout << "Mode = " << "DLL (ESG and Projection) mode" << endl;
			run = ALI_BL_esg_projection(config, log_strm, nbWorker, idWorker, s);
		}
		else if (mode == 2)
		{
			cout << "Mode = " << "DLL (ESG and Projection) mode" << endl;
			run = run_HA_esg_mode(config, log_strm, nbWorker, idWorker, s);
		}
		else if (mode == 3)
		{
			cout << "Mode = " << "DLL (ESG stand alone) mode" << endl;
			run = run_esg_standalone(config, log_strm, nbWorker, idWorker, s);
		}
		else if (mode == 4)
		{
			cout << "Mode = " << "DLL (Ageing policies) mode" << endl;
			run = run_aging_policies(config, log_strm, nbWorker, idWorker, s);
		}
		else if (mode == 5)
		{
			cout << " Mode = " << "Hedge effectiveness mode : Liability Part" << endl;
			run=InterfacePriceHE( config, log_strm, nbWorker, idWorker, s_he_price);
			run+=InterfaceSummarizeHE( config, log_strm, nbWorker, idWorker, s_he_sum);
			if (run != 2)
				cout << "Error during the run. Check error log file. " << endl;
		}
		else if (mode == 6)
		{
			cout << " Mode = " << "DLL (Risk Appetite) mode" << endl;
			//int c = check_assumptions(config, log_strm, 1, 1, s);
			
			cout << " Pricing and Summarizing for one step..."<<endl;
			int init_run = run_HA_file_mode(config, log_strm, nbWorker, idWorker, s);
			
			if (init_run == 0)
				cout<< " An error occurred during this first step."<<endl;
			
			cout << " Pricing and Summarizing for RA date..." << endl;
			run = run_RA_mode(config, log_strm, nbWorker, idWorker, s);
			if (run == 0)
				cout << "An error occured on pricing. Press a key to quit and check log file. " << endl;
			else
				cout << "Pricing ended correctly. Press any key to quit. " << endl;
		}
		else if (mode == 7)
		{
			cout << " Mode = " << "Hedge effectiveness mode : Asset Part" << endl;
			string s = "";
			run  = InterfacePriceHeAsset( config, log_strm, nbWorker, idWorker, s);
			run += InterfaceSummarizeHeAsset( config, log_strm, nbWorker, idWorker, s);	
			if (run != 2)
				cout << "Error during the run. Check error log file. " << endl;
		}
		else
		{
			cout << "Error : The mode number is wrong. It must be between 1 and 6." << endl;
		}
	
		if (run == 0)
			cout << "Error during the run. Check error log file. " << endl;
		else
			cout << "The global run ended correctly." << endl;
		
		end = clock();
		cout << "Time of run  = " << end - start << " msecond(s) " << endl;
		getchar();
	}
	catch(GMXBException *g)
	{
		cout <<g->get_message() << endl;
		getchar();
	}
	return 1;
}
