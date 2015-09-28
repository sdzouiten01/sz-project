#pragma once
#ifndef EXISTING_DELTA_FUTURES_H
#define EXISTING_DELTA_FUTURES_H
#include "Assumptions.h"
class Nbr_Futures;
class Delta_Futures;
/**
* Class to know existing delta in portfolio due to futures contracts only
*/
class Existing_Delta_Futures
{
public:
	struct Objects_Needed
	{
		Delta_Futures* delta_futures;
		Assumptions* assumptions;
		Nbr_Futures* nbr_futures;
	}objects_needed;
	struct Characteristics
	{
		double sigma;
		double q;
		double length_of_futures_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	double * existing_fv;
	double ** tab;
	void init_tab(int i, int j);
	double return_existing_fv(int i);
	Existing_Delta_Futures(Assumptions* assumptions, Delta_Futures* delta_futures );
	~Existing_Delta_Futures(void);
};
#endif