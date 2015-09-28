#pragma once
#ifndef RF_FUTURES_H
#define RF_FUTURES_H
class Zero_Coupon_Rates;
class Assumptions;
class Tte_Futures;
/**
* Class
*/
class Rf_Futures
{
public:
	struct Objects_Needed
	{
			Assumptions* assumptions;
			Zero_Coupon_Rates* zero_coupon_rates;
			Tte_Futures* tte_futures;
	}objects_needed;
	Rf_Futures(Assumptions* assumptions,Zero_Coupon_Rates* zero_coupon_rates,Tte_Futures* tte_futures);
	~Rf_Futures(void);
	double** tab;
	void init_tab();

};
#endif