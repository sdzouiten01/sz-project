#ifndef ANNUAL_SWAPS_RATES_SHOCK_UP_H
#define ANNUAL_SWAPS_RATES_SHOCK_Up_H
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
class Annual_Swap_Rates_Shock_Up
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
	double** tab;//linearization and parellel shifted rate curve

	void init_tab();
	void shock_up();
	double shock(int start_rate, int key_rate, int end_rate, int mty);
	Annual_Swap_Rates_Shock_Up(int swap_no,Rw_Scn* rw_scn,Assumptions* assumptions);
	~Annual_Swap_Rates_Shock_Up(void);
};
#endif