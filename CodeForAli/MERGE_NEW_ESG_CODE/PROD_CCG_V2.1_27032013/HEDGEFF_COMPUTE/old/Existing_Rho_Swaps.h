#pragma once
#ifndef EXISTING_RHO_SWAPS_H
#define EXISTING_RHO_SWAPS_H
#include "Assumptions.h"
class Nbr_Swaps;
class Rho_Swaps;
/**
* Class to know existing rho in portfolio due to futures contracts only
*/
class Existing_Rho_Swaps
{
public:
	struct Objects_Needed
	{
		Rho_Swaps* rho_swaps;
		Assumptions* assumptions;
		Nbr_Swaps* nbr_swaps;
	}objects_needed;
	struct Characteristics
	{
		double sigma;
		double q;
		double length_of_swaps_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	double * existing_sv;
	double ** tab;
	int swap_no;
	void init_tab(int i, int j);
	double return_existing_sv(int i);
	Existing_Rho_Swaps(int swap_no,Assumptions* assumptions, Rho_Swaps* rho_swaps );
	~Existing_Rho_Swaps(void);
};
#endif