#ifndef _INTERFACE_ESG_H_
#define _INTERFACE_ESG_H_

#include <stdio.h> 
#include <string.h> 
#include <Scenario.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "InterfaceCalibration.h"

class InterfaceEsg
{
public:
		std::string config_file;

		GMXBParameters * equity_table;
		GMXB_Assumption_Table * bond_table;
		GMXB_Assumption_Table *	rate_table;
		GMXB_Assumption_Table *	initial;
		GMXB_Assumption_Table * correlation_table;
		GMXB_Assumption_Table * vol_table;
		GMXB_Assumption_Table * error_table;

		GMXBParameters * assumptions;

		int calibration;
		int num_initial;
		double *initial_spot_rate; // = zc, the output of calibration s
		double *initial_spot_rate_dates;
		double a;
		double sigma;
		
		int num_equity;
		int num_bond;
		int num_rate;
		int num_vol;
		string *equity;
		double *bond;
		double *bond_coupon;
		double *bond_freq;
		double *rate;
		int *rate_type;
		double * rate_period;
		
		double **correlation;
		double **vol;
		double *error;
		double *add_error;
		int* max_try;

		int delta_t;
		int year;
		int scenario;
		bool antithetic;
		
	    bool output_scenario;    // 1 = output scenario,  0 = output nothing

		string f_scenario_address;
		
		bool debug_info;
		string debug_address;

		int test_beta;
		int test_martingale;

		// in fact it is bool
		int if_use_rate_table;  
		int if_use_bond_table;
		
		bool closed_formula;
		int precision;
		string if_generate_scenario;
		bool new_interpolation;
		double a_without_calib;
		double sigma_without_calib;
		bool vol_HW;
		bool vol_implied_to_loc;

		Scenario *s;

		InterfaceEsg( const string& config );
		virtual ~InterfaceEsg();
		
		double*** Gen(InterfaceCalibration& iCalib, double& a,double& sigma,int scenario_pass_number,bool if_calib, int bucket_id, int w_id);

		/*
		* The only difference in this function is the way we read the parameters a and sigma
		* When we don't do the calibration, the parameters are read thanks to the attributes of the class
		*/
		double*** Gen_he_mode(InterfaceCalibration& iCalib, double& a, double& sigma, int scenario_pass_number, bool if_calib, int bucket_id=-1);		
		
		/*
		* Function to delete the Scenario class. used for the hedge eefectiveness
		*/
		void delete_s();

		void write_seed(int martingality, ofstream &file1,unsigned long * seed);
		int read_seed(ifstream &file1,unsigned long * seed);

		void set_a_without_calib(double a);
		void set_sigma_without_calib(double sigma);
		void set_initial_spot_rate(double *ZC);
		void read_a_and_sigma(ifstream &file3,double * tab);

		

};

#endif
