#pragma once
#ifndef RF_FUTURES_SHOCK_UP_H
#define RF_FUTURES_SHOCK_UP_H
class Zero_Coupon_Rates_Shock_Up;
class Assumptions;
class Tte_Futures;
/**
* Class to comute short annual_swap_rates
*/
class Rf_Futures_Shock_Up
{
public:
	struct Objects_Needed
	{
			Assumptions* assumptions;
			Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up;
			Tte_Futures* tte_futures;
	}objects_needed;
	int swap_no;
	Rf_Futures_Shock_Up(int swap_no, Assumptions* assumptions,Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up,Tte_Futures* tte_futures);
	~Rf_Futures_Shock_Up(void);
	double** tab;
	void init_tab();

};
#endif