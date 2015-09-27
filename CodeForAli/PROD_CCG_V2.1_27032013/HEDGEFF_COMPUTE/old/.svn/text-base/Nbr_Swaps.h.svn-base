#pragma once
#ifndef NBR_SWAPS_H
#define NBR_SWAPS_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Existing_Rho;
class Existing_Rho_Swaps;
class Swaps_Trade;
class Rho_Swaps_In_Portfolio;
class Rho_Swaps_In_Portfolio_2;
class Tte_Swaps;
/**
* Class for numbers of futures in portfolio using our rho hedging Strategy: 
* Delta Neutral strategy, and preference to use existing futures contracts when it is possible.
*/
class Nbr_Swaps
{
public:
	struct Objects_Needed
	{
		Swaps_Trade* swaps_trade;
		Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio;
		Rho_Swaps_In_Portfolio_2* rho_swaps_in_portfolio_2;
		Existing_Rho* existing_rho;
		Existing_Rho_Swaps* existing_rho_swaps;
		Assumptions* assumptions;
		Tte_Swaps* tte_swaps;
	}objects_needed;
	
	int current_future;
	ofstream debug_file;
	int current_col;
	int swap_no;
	double * numbers_of_swaps;
	double ** tab;
	void init_fields();
	void init_sigma();
	void init_q();
	void init_length_of_swaps_contract();
	void init_sell_at_time_to_expiry();
	void init_tab();
	void init_tab(int i, int j);
	Nbr_Swaps(int swap_no,Swaps_Trade* swaps_trade,
		Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio,
		Rho_Swaps_In_Portfolio_2* rho_swaps_in_portfolio_2,
		Existing_Rho* existing_rho,Existing_Rho_Swaps* existing_rho_swaps,
		Assumptions* assumptions,Tte_Swaps* tte_swaps);

	~Nbr_Swaps(void);
};
#endif