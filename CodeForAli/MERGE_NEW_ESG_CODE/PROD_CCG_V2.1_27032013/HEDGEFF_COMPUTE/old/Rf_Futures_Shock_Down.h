#pragma once
#ifndef RF_FUTURES_SHOCK_DOWN_H
#define RF_FUTURES_SHOCK_DOWN_H
class Zero_Coupon_Rates_Shock_Down;
class Assumptions;
class Tte_Futures;
/**
* Class to comute short annual_swap_rates
*/
class Rf_Futures_Shock_Down
{
public:
	struct Objects_Needed
	{
			Assumptions* assumptions;
			Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down;
			Tte_Futures* tte_futures;
	}objects_needed;
	int swap_no;
	Rf_Futures_Shock_Down(int swap_no , Assumptions* assumptions,Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,Tte_Futures* tte_futures);
	~Rf_Futures_Shock_Down(void);
	double** tab;
	void init_tab();

};
#endif