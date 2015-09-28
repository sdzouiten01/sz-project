#pragma once
#ifndef EXISTING_RHO_H
#define EXISTING_RHO_H
#include "Assumptions.h"
class Nbr_Swaps;
class Nbr_Futures;
class Rho_Swaps;
class Rho_Puts;
class Nbr_Puts;
class Rho_Futures;
/**
* Class used to know existing rho genreated by both futures and options in portfolio at a time step
*/
class Existing_Rho
{
public:
	struct Objects_Needed
	{
		Rho_Swaps* rho_swaps;
		Assumptions* assumptions;
		Nbr_Swaps* nbr_swaps;
		Nbr_Futures** nbr_futures_glob;
		Nbr_Puts** nbr_puts_glob;
		Rho_Puts** rho_puts_glob;
		Rho_Futures** rho_futures_glob;
	}objects_needed;
	int nb_index;
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
	Existing_Rho(int swap_no,Assumptions* assumptions, Rho_Swaps* rho_swaps ,Rho_Puts** rho_puts_glob,Nbr_Puts** nbr_puts_glob,Nbr_Futures** nbr_futures_glob,Nbr_Swaps* nbr_swaps,Rho_Futures** rho_futures_glob,int nb_index);
	~Existing_Rho(void);
};
#endif