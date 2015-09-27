#pragma once
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class Annual_Swap_Rates;
/**
* Class to compute bootstrapping
*/
class Zero_Coupon_Rates
{
public:
	struct Objects_Needed
	{
		Annual_Swap_Rates* annual_swap_rates;
	}objects_needed;
	double** tab;
	int dim_x;
	int dim_y;
	ofstream tst;
	Zero_Coupon_Rates(Annual_Swap_Rates* annual_swap_rates);
	void init_tab();
	~Zero_Coupon_Rates(void);
};
