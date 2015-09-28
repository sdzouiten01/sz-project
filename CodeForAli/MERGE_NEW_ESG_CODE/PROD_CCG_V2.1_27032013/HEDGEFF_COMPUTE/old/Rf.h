#pragma once
#ifndef RF_H
#define RF_H
class Zero_Coupon_Rates;
class Assumptions;
class Tte;
/**
* Class to comute short annual_swap_rates
*/
class Rf
{
public:
	struct Objects_Needed
	{
			Assumptions* assumptions;
			Zero_Coupon_Rates* zero_coupon_rates;
			Tte* tte;
	}objects_needed;
	Rf(Assumptions* assumptions,Zero_Coupon_Rates* zero_coupon_rates,Tte* tte);
	~Rf(void);
	double** tab;
	void init_tab();

};
#endif