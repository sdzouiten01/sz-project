#pragma once
#ifndef RWSCN_H
#define RWSCN_H
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class Assumptions;
/**
* Class to load real world scenarios
*/
class Rw_Scn
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
	}objects_needed;
	int model_period;
	ofstream test;
	int scen_no;
	int index_no;
	int* maturities;
	double** tab;
	int nb_of_mty;
	int row_to_start;
	Rw_Scn(Assumptions* assumptions,int model_period,string file_input_name,int scen_no);
	void init_nb_of_mty(string file_input_name);
	void init_row_to_start(string file_input_name);
	void init_maturities(string file_input_name);
	void init_tab(string file_input_name);
	~Rw_Scn(void);
};
#endif 