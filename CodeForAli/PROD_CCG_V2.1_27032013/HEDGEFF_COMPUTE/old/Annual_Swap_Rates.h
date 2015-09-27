#ifndef ANNUAL_SWAPS_RATES_H
#define ANNUAL_SWAPS_RATES_H
#pragma once
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class Assumptions;
class Rw_Scn;
/**
* Class to compute piecewise linear interpolation
*/
class Annual_Swap_Rates
{
public:
	struct Objects_Needed
	{
		Rw_Scn* rw_scn;
		Assumptions* assumptions;
	}objects_needed;
	ofstream a;
	int dim_x;
	int dim_y;
	ofstream tst;
	int * maturities;
	double** tab;
	void init_tab();
	Annual_Swap_Rates(Rw_Scn* rw_scn,Assumptions* assumptions);
	~Annual_Swap_Rates(void);
};
#endif