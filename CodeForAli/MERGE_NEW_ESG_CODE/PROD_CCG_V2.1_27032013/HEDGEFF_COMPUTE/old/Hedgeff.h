#pragma once
#ifndef HEDGEFF_H
#define HEDGEFF_H
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

using namespace std;
class Assumptions;
/**
* Class to load Hedgeff.csv file (output of the liabliliy DLL)
*/
class Hedgeff
{
public:
	
	struct Objects_Needed
	{
		Assumptions* assumptions;
	}objects_needed;

	int model_period;
	int scen_no;
	int index_no;
	double * period;
	double * av_e;
	double * liab_ov;
	double * delta_liab;
	double * gamma_liab;
	double * discount;
	double * index;
	double * av_index;
	double * gamma_1_as;
	double ** rho_liab;
	int row_to_start;

	//! added by M
	double* Total_charges;
	double* claims;



	void init_row_to_start(string file_input_name);
	void init_field(string file_input_name,double * field,string field_name_in_csv, string field_name);
	void init_fields(string file_input_name, string rw_scn_file);
	void affiche(double* one_dim_tab,int start,int end,char* name);
	Hedgeff(Assumptions* assumptions,int model_period,string file_input_name,int index_no,int scen_no,string rw_scn_file);
	~Hedgeff(void);
};
#endif 