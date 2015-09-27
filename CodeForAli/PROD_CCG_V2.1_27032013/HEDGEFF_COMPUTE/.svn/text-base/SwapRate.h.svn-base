/******************************************************************************
* Class SwapRate
*		- This class contains  swap-rates of different maturities
*		- It provides Annual_Swap_Rate, Zero_Coupon_Swap_Rate, Short_Annual_Swap_Rate(RF)
*
* 24-05-2010
******************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Portfolio.h"
#include "Assumptions.h"

class SwapRate{
public:
	SwapRate(Configuration* cfg);
	~SwapRate();

	void loadDataHeader(std::string rw_scn);
	void loadScenData(int scen_no, Portfolio* p);
	void initial();
	//void calc_zero_coupon(); // modified 28/11/2010 -- commented as it's not used
	void shock();
	void dump();

	double**		_zero_coupon_rates;
	double***	_zero_coupon_shock_up_rates;
	double***	_zero_coupon_shock_down_rates;

	double**		_shocks;
	double**		_rates;
	double**		_annual_swap_rates;

	int* _maturities;
	int dim_x;
	int dim_y;
	
	bool _is_shocked;
	int _cur_scen;
	std::ifstream _scn_file;
	//
	Configuration* _config;
	int g_model_period;
	int g_rates_freq_ann;
	int g_nb_swap_maturities;
	int g_nb_lbs_maturities;
	int g_nb_av;
	int* g_swap_maturities;
private:
	void allocateMemory();
};