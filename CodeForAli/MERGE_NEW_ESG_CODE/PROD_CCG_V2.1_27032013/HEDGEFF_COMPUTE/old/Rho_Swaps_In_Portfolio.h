#pragma once
#ifndef RHO_SWAPS_IN_PORTFOLIO_H
#define RHO_SWAPS_IN_PORTFOLIO_H
#include "Nbr_Swaps.h"
#include "Rho_Swaps.h"
#include "Assumptions.h"
/**
* Class for Deltas due to futures in porfolio <BR>
* <CODE>tab[i][j]=rho_swaps[i][j]*nbr_swaps[i-1][j]+ tab[i][j-1]</CODE>
*/
class Rho_Swaps_In_Portfolio
{
public:
	struct Objects_Needed
	{
		Nbr_Swaps* nbr_swaps;
		Rho_Swaps* rho_swaps;
		Assumptions* assumptions;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_swaps_contract;
		double sell_at_time_to_expiry;
		double reb_freq_gamma;
	} characteristics;*/
	double ** tab;
	int swap_no;
	void init_tab(int i, int j);
	Rho_Swaps_In_Portfolio(int swap_no,Assumptions* assumptions,Rho_Swaps* rho_swaps);
	~Rho_Swaps_In_Portfolio(void);
};
#endif