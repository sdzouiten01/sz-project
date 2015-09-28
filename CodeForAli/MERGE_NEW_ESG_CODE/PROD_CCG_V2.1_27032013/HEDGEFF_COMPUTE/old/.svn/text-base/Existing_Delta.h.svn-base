#pragma once
#ifndef EXISTING_DELTA_H
#define EXISTING_DELTA_H
#include "Assumptions.h"
class Nbr_Futures;
class Delta_Futures;
class Delta_Puts;
class Nbr_Puts;
/**
* Class used to know existing delta genreated by both futures and options in portfolio at a time step
*/
class Existing_Delta
{
public:
	struct Objects_Needed
	{
		Delta_Futures* delta_futures;
		Assumptions* assumptions;
		Nbr_Futures* nbr_futures;
		Nbr_Puts* nbr_puts;
		Delta_Puts* delta_puts;
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
	Existing_Delta(Assumptions* assumptions, Delta_Futures* delta_futures ,Delta_Puts* delta_puts,Nbr_Puts* nbr_puts);
	~Existing_Delta(void);
};
#endif