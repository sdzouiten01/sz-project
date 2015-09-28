#pragma once
#ifndef RHO_SWAPS_H
#define RHO_SWAPS_H
class Assumptions;
class Tte_Swaps;
class Zero_Coupon_Rates_Shock_Up;
class Zero_Coupon_Rates_Shock_Down;
class Zero_Coupon_Rates;
/**
* Class for deltas of futures used in hedging portfolio
*/
class Rho_Swaps
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Tte_Swaps* tte_swaps;
		Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up;
		Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down;
		Zero_Coupon_Rates* zero_coupon_rates;
	}objects_needed;
	double** tab;
	Rho_Swaps(
		int swap_no,
		Assumptions* assumptions,
		Tte_Swaps* tte_swaps,
		Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up,
		Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,
		Zero_Coupon_Rates* zero_coupon_rates
		);
	int swap_no;
	void init_tab();
	~Rho_Swaps(void);
};
#endif