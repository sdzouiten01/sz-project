#pragma once
#ifndef SV_BASE_H
#define SV_BASE_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Tte_Swaps;
class Zero_Coupon_Rates;
class Sv_Base
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Tte_Swaps* tte_swaps;
		Zero_Coupon_Rates* zero_coupon_rates;
	}objects_needed;
	double** tab;
	void init_tab();
	int swap_no;
	ofstream tt;
	Sv_Base(int swap_no,Assumptions* assumptions,
		Tte_Swaps* tte_swaps,
		Zero_Coupon_Rates* zero_coupon_rates);
	~Sv_Base(void);
};
#endif