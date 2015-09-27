#pragma once
#ifndef RF_SHOCK_DOWN_H
#define RF_SHOCK_DOWN_H
class Zero_Coupon_Rates_Shock_Down;
class Assumptions;
class Tte;
/**
* Class to comute short annual_swap_rates
*/
class Rf_Shock_Down
{
public:
	struct Objects_Needed
	{
			Assumptions* assumptions;
			Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down;
			Tte* tte;
	}objects_needed;
	int swap_no;
	Rf_Shock_Down(int swap_no, Assumptions* assumptions,Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,Tte* tte);
	~Rf_Shock_Down(void);
	double** tab;
	void init_tab();

};
#endif