#ifndef INTERFACEESG0_H
#define INTERFACEESG0_H
#include "GMXB_Assumption_Table.h"
#include "GMXBParameters.h"
#include <string>
using namespace std;


class InterfaceESG0{
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Global  Interface parameters //////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//Address of configuration file (interface of ESG)
	string Config_file;
	//Use correct date means : the system date: the today date.
	int Use_Correct_Date;
	//Use reported date means : the date mentioned in the config file.
	int Use_reported_date_Date;	
	//Calculated prorata for the first month: from the reference date to the end of the current month
	double Prorata;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Calibration Interface parameters //////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Bootstrapping indicator: Yes or No?, check if it's really used!!!!!
	bool Bootstrapping_security;
	//Interpolation and extrapolation Types: "SPLINE","CONSTANT","LINEAR","LOGLINEAR",
	string Interpolation_type;
	string Extrapolation_type_start;
	string Extrapolation_type_end;
	string Svensson_Extrapolation_End_option;
	// Coupon period for swap curve and swaptions matrix:"SEMI_ANNUAL", "ANNUAL"
	string Coupon_period;
	//Swap rate curve vector : [0]= dates, [1]= swap rates ( *100, ex: 2.3%--> 2.3)
	double **SwapRateVector;
	//Number date used in the swap yield curve , SwapRateVector size(2)
	int Nb_date;
	//If Calibration Yes or No?
	int Calibrate_Y_N;
	//Output calibration information
	bool Debug_info_Calib;
	//Output path address for calibration
	string Debug_address_Calib;

	//////////////////////////////////////////////////////////////////////////////////IF CALIB==Yes
	
	//If Calib "Y", Number Tenors, vector of Calc_Swaption_tenor size
	int Nb_tenor_Swaption; 	
	//If Calib "Y", swaption Tenor vector
	std::vector<double> Calc_Swaption_tenor;
	//If Calib "Y", Number maturities, vector of Calc_Swaption_maturity size
	int Nb_maturity_Swaption;	
	//If Calib "Y", swaption maturity vector
	std::vector<double> Calc_Swaption_maturity;
	//If Calib "Y", swaption vector used for the calibration. The constructor for this vector is : for Maturity{ for Tenor}
	std::vector<double> Calc_Swaption_vol;
	//If Calib "Y", weights swaption vector used for the calibration. The constructor for this vector is : for Maturity{ for Tenor}
	std::vector<double> Calc_Swaption_Weight;
	//Calibration on "a", "sigma", or "a and sigma"
	string Calibration_On;
	//If Calib "Y", vector 4 parameters (initial parameters: a[0],sigma[1],0.0[2],0.0[3])
	double *Param_initial;
	//If Calib "Y", vector 4 parameters (initial min parameters: a min[0], sigma min[1],0.0[2],0.0[3])
	double *Param_min;
	//If Calib "Y", vector 4 parameters (initial max parameters: a max [0],sigma max[1],0.0[2],0.0[3])
	double *Param_max;
	//If Calib "Y", swaption type : "payer" or "receiver"
	string Swaption_type;
	//If Calib "Y", strike of swaptions matrix: 100% for at the money
	double Strike;
	//If Calib "Y", Error function : "price", "relative price", "volatility"
	string Calib_Error_Function_Type;

	//////////////////////////////////////////////////////////////////////////////////IF CALIB==No
	
	//If Calib "N", parameter "a" used in the projection
	double A_without_calib;
	//If Calib "N", parameter "sigma" used in the projection
	double Sigma_without_calib;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////Generator Interface parameters //////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
	// Generate Scenarios : Yes=1, No=0
	int If_Generate_Scenario;
	// Very important: if we want to generate scenario by scenario ( generate two scenarios and project ...) put 0, otherwise 1(generate all)
	int Generate_All_Scenarios_In_One_Step;
	//Number step : 12 for monthly, 1 yearly(not tested), 256 for daily ( not tested)
	int Time_Step; 
	//Number years for the projection
	int Number_Year;
	//Number of projected scenarios
	int Number_Scenario;
	//Local Number of projected scenarios
	int Local_Number_Scenario;
	//Variance reduction option 1: antithetic 
	int Antithetic;
	//Variance reduction option 2: martingality and beta test (the same meaning, test beta for IR)
	int Test_beta;
	//Variance reduction option 2: martingality and beta test (the same meaning, test beta for IR)
	int Test_martingale;
	//Outputing the debug information : concerning the martingality test, but also the Implied volatility if it's activated
	int Debug_Scenario_Generator_info;
	//Address path for debuging information
	string Debug_ESG_address;
	//Equities name vector
	string *Equity_name_tab;
	//Matrix of Equities volatility: Eq_Volatility_Matrix[Number equities(Nb_equity) +1 (due to maturities values)][Number volatilities (Nb_vol)]
	double **Eq_Volatility_Matrix;
	//Number of equities
	int Nb_equity;
	//Number of volatilities in the vol table ( number of points in the volatility curve for the equity indexes)
	int Nb_vol;
	//These parameters : vol_HW and Vol_implied_to_loc are used to convert equity implied volatilities to BSHW volatilities
	bool vol_HW;
	//These parameters : vol_HW and Vol_implied_to_loc are used to convert equity implied volatilities to BSHW volatilities
	bool Vol_implied_to_loc;		


	//Generate Rates : Yes or No ?
	int If_use_rate_table;
	//Number of rates indexes we need to generate 
	int Nb_rate;
	//Vector of  maturities of rates indexes (years)
	double *Rate_Mat_tab;
	//Vector of  rate type ( swap rate or Zero coupon rate) of rates indexes
	int *Rate_type_tab;
	//Vector of  rate period ( in case of swap rate) of rates indexes: ANNUAL or SEMI ANNUAL, 1 or 0.5
	double * Rate_Period_tab;	

	//Generate Bonds : Yes or No ?
	int If_use_bond_table;
	//Number of Bonds indexes we need to generate 
	int Nb_bond;
	//Vector of  maturities of Bonds indexes (years)
	double *Bond_Mat_tab;
	//Vector of  Bond coupon ( if there is coupon, otherwise 0) of Bonds indexes
	double *Bond_coupon_tab;
	//Vector of  Bond coupon frequency of Bonds indexes
	double *Bond_Freq_tab;


	//Correlation matrix: size=(Nb_equity+1)*(Nb_equity+1)
	double **Correlation_matrix;

	//Error Vector: size = Nb_equity+1
	double *Error_tab;
	//incremental values for error vector : size = Nb_equity+1
	double *Add_error_tab;
	//Maximum try before incrementing the error values in error vector : size = Nb_equity+1
	int* Max_try_tab;

	//Write scenario : Yes or No?
	bool Write_scenario;
	bool Write_gaussian;
	//Number digit for double values in the scenario file
	int Precision;	
	//adress path for writing the scenarios file
	string File_scenario_address;
	
	int Nb_dates_drift_table;
	double **Eq_Drift_Matrix;
	string Use_Equity_Drift_table;

	//Constructor
	InterfaceESG0(const string& config);
	//Destructor
	virtual ~InterfaceESG0();
};

#endif