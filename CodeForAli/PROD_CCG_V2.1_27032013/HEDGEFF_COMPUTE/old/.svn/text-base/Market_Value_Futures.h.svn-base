#pragma once
#ifndef MARKET_VALUE_FUTURES_H
#define MARKET_VALUE_FUTURES_H

class Assumptions;
class Fv_Base;
class Nbr_Futures;
class Market_Value_Futures
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Fv_Base* fv_base;
		Nbr_Futures* nbr_futures;
	} objects_needed;
	double** tab;
	double* mkt_val;
	void init_tab();
	Market_Value_Futures(Assumptions* assumptions,
		Fv_Base* fv_base,
		Nbr_Futures* nbr_futures);
	~Market_Value_Futures(void);
};
#endif