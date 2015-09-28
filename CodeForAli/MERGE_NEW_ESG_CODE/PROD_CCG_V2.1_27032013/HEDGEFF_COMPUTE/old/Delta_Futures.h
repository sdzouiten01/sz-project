#pragma once
#ifndef DELTA_FUTURES_H
#define DELTA_FUTURES_H
class Assumptions;
class Tte_Futures;
class Rf_Futures;
/**
* Class for deltas of futures used in hedging portfolio
*/
class Delta_Futures
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Tte_Futures* tte_futures;
		Rf_Futures* rf_futures;
	}objects_needed;
	double** tab;
	Delta_Futures(
		Assumptions* assumptions,
		Tte_Futures* tte_futures,
		Rf_Futures* rf_futures
		);
	void init_tab();
	~Delta_Futures(void);
};
#endif