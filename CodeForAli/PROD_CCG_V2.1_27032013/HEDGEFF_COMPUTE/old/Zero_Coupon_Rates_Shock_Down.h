#pragma once
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class Annual_Swap_Rates_Shock_Down;
/**
* Class to compute bootstrapping
*/
class Zero_Coupon_Rates_Shock_Down
{
public:
	struct Objects_Needed
	{
		Annual_Swap_Rates_Shock_Down* annual_swap_rates_shock_down;
	}objects_needed;
	double** tab;
	int dim_x;
	int dim_y;
	int swap_no;
	ofstream tst;
	Zero_Coupon_Rates_Shock_Down(int swap_no, Annual_Swap_Rates_Shock_Down* annual_swap_rates_shock_down);
	void init_tab();
	~Zero_Coupon_Rates_Shock_Down(void);
};
