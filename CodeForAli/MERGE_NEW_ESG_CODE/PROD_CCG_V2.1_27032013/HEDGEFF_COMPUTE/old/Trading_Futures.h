#pragma once
#ifndef TRADING_FUTURES_H
#define TRADING_FUTURES_H
class Assumptions;
class Fv_Base;
class Nbr_Futures;
class Trading_Futures
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Fv_Base* fv_base;
		Nbr_Futures* nbr_futures;
	} objects_needed;
	double * trading;
	double ** tab;
	void init_tab();
	Trading_Futures(Assumptions* assumptions,
		Fv_Base* fv_base,
		Nbr_Futures* nbr_futures);
	~Trading_Futures(void);
};
#endif