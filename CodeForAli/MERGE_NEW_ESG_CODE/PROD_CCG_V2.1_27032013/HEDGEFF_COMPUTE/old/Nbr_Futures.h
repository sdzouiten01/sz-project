#pragma once
#ifndef NBR_FUTURES_H
#define NBR_FUTURES_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Existing_Delta;
class Existing_Delta_Futures;
class Future_Trade;
class Delta_Futures_In_Portfolio;
class Delta_Futures_In_Portfolio_2;
class Tte_Futures;
/**
* Class for numbers of futures in portfolio using our delta hedging Strategy: 
* Delta Neutral strategy, and preference to use existing futures contracts when it is possible.
*/
class Nbr_Futures
{
public:
	struct Objects_Needed
	{
		Future_Trade* future_trade;
		Delta_Futures_In_Portfolio* delta_futures_in_portfolio;
		Delta_Futures_In_Portfolio_2* delta_futures_in_portfolio_2;
		Existing_Delta* existing_delta;
		Existing_Delta_Futures* existing_delta_futures;
		Assumptions* assumptions;
		Tte_Futures* tte_futures;
	}objects_needed;
	
	int current_future;
	ofstream debug_file;
	int current_col;
	double * numbers_of_futures;
	double ** tab;
	void init_fields();
	void init_sigma();
	void init_q();
	void init_length_of_futures_contract();
	void init_sell_at_time_to_expiry();
	void init_tab();
	void init_tab(int i, int j);
	Nbr_Futures(Future_Trade* future_trade,
		Delta_Futures_In_Portfolio* delta_futures_in_portfolio,
		Delta_Futures_In_Portfolio_2* delta_futures_in_portfolio_2,
		Existing_Delta* existing_delta,Existing_Delta_Futures* existing_delta_futures,
		Assumptions* assumptions,Tte_Futures* tte_futures);

	~Nbr_Futures(void);
};
#endif