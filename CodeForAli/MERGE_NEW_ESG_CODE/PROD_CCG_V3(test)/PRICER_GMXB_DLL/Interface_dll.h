
// Launch calibration standalone with the DLL (changed)
_declspec (dllimport) int run_calibration_standalone(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);

// ESG stand alone ( previous)
_declspec (dllimport) int run_esg_standalone(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);

// ESG stand alone (Baloise)
_declspec (dllimport) int TEST_NEW_CALIB(string& config, ofstream& log_strm, int nbWorker, int idWorker, string& s);
	
//Launch ESG and projection ( Baloise)
_declspec (dllimport) int ALI_BL_esg_projection_V2(string& config_Projection, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);

//Launch projection with a set of scenario file
_declspec (dllimport) int run_HA_file_mode(string& config, ofstream& log_strm, int nb_of_workers, int worker_id, string& s);




