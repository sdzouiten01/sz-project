#ifndef SCENARIOGEN_H
#define SCENARIOGEN_H
#include "../Models/Diffusion_Model0.h"
#include "../Models/BS_HW1F.h"
#include "../GaussianGen/GaussianGen.h"
#include "../GaussianGen/VarianceReduction.h"
#include "../Instruments/YieldCurve.h"
//#include "Instruments/InterfaceESG0.h"
#include "../Instruments/InterfaceESG0.h"
#include "../Calibration/HW1FCalibrator.h"
#include <string>

using namespace std;


class ScenarioGen{
public:	
	
	GaussianGenModelParameters *Gaus_Model_Param;
	VarianceReduction *Test;
	GaussianGen *Gaussian;
	DIF_MODEL0 *Diffusion_Model;
	YC *Yield_curve_vector;
	
	int Nb_dates_drift_table;
	double **Eq_Drift_Matrix;
	string Use_Equity_Drift_table;
	string Equity_Drift_With_IR_Model;

	string Model_diffusion_Name; 
	string Model_random_Name; 
	string Model_guassian_Name;	
	
	
	double *** Scenario_vector;
	int Num_Index;

	
	
	int If_Generate_Scenario; //From InterfaceESG Class

	int Generate_All_Scenarios_In_One_Step; //From InterfaceESG Class
	string *Equity_name_tab; //From InterfaceESG Class
	int If_use_rate_table; //From InterfaceESG Class
	double *Rate_Mat_tab; //From InterfaceESG Class
	int *Rate_type_tab; //From InterfaceESG Class
	double * Rate_Period_tab; //From InterfaceESG Class
	int If_use_bond_table; //From InterfaceESG Class
	double *Bond_Mat_tab; //From InterfaceESG Class
	double *Bond_coupon_tab; //From InterfaceESG Class
	double *Bond_Freq_tab; //From InterfaceESG Class
	bool If_Write_scenario; //From InterfaceESG Class
	bool If_Write_gaussian; //From InterfaceESG Class
	int Precision; //From InterfaceESG Class
	string File_scenario_address; //From InterfaceESG Class

	int Scenario_path_number;
	int Scenario_shock_number;
	void Reset_Scenario_With_IR_Shocks(HW1FSwaptionPricer* &model, double startDate,double keyDate, double endDate,double shockSize,string& shockType);
	void Generate_Scenario();

	void Write_Scenario();

	ScenarioGen(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model , YC* &mYieldCurve,string Model_rand,string Model_guassian,string Model_diff, int scn_path_number);
	ScenarioGen(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model , YC* &mYieldCurve,string Model_rand,string Model_guassian,string Model_diff, int scn_path_number, int number_scn_gen_stepbystep);

	void write_seed(ofstream &file1,unsigned long * seed);

	int read_seed(ifstream &file2,unsigned long * seed);

	~ScenarioGen();

};

#endif