#ifndef ANNUAL_SWAPS_RATES_SHOCK_DOWN_H
#define ANNUAL_SWAPS_RATES_SHOCK_DOWN_H
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
class Annual_Swap_Rates_Shock_Down
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
	int * maturities;
	int mty_to_hedge;
	int swap_no;
	double shock_to_apply;
	ofstream tst;
	double** tab;//linearization and parallel shifted rate curve
	void init_tab();
	void shock_down();   
	double shock(int start_rate, int key_rate, int end_rate, int mty);
	Annual_Swap_Rates_Shock_Down(int swap_no,Rw_Scn* rw_scn,Assumptions* assumptions);
	~Annual_Swap_Rates_Shock_Down(void);
};
#endif