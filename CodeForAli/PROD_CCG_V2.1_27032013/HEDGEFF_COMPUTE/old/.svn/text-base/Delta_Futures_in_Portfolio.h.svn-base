#pragma once
#ifndef DELTA_FUTURES_IN_PORTFOLIO_H
#define DELTA_FUTURES_IN_PORTFOLIO_H
#include "Nbr_Futures.h"
#include "Delta_Futures.h"
#include "Assumptions.h"
/**
* Class for Deltas due to futures in porfolio <BR>
* <CODE>tab[i][j]=delta_futures[i][j]*nbr_futures[i-1][j]+ tab[i][j-1]</CODE>
*/
class Delta_Futures_In_Portfolio
{
public:
	struct Objects_Needed
	{
		Nbr_Futures* nbr_futures;
		Delta_Futures* delta_futures;
		Assumptions* assumptions;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_futures_contract;
		double sell_at_time_to_expiry;
		double reb_freq_gamma;
	} characteristics;*/
	double ** tab;
	void init_tab(int i, int j);
	Delta_Futures_In_Portfolio(Assumptions* assumptions,Delta_Futures* delta_futures);
	~Delta_Futures_In_Portfolio(void);
};
#endif