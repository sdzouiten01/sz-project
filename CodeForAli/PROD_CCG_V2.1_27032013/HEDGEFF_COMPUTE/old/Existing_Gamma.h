#pragma once
#ifndef EXISTING_GAMMA_H
#define EXISTING_GAMMA_H
#include "Assumptions.h"
class Nbr_Puts;
class Gamma_Puts;
/**
* Class to know exsting gamma in portfolio at a time step
*/
class Existing_Gamma
{
public:
	struct Objects_Needed
	{
		Gamma_Puts* gamma_puts;
		Assumptions* assumptions;
		Nbr_Puts* nbr_puts;
	}objects_needed;
	struct Characteristics
	{
		double sigma;
		double q;
		double length_of_option_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	double * existing_ov;
	double ** tab;
	void init_tab(int i, int j);
	Existing_Gamma(Assumptions* assumptions, Gamma_Puts* gamma_puts);
	~Existing_Gamma(void);
};
#endif