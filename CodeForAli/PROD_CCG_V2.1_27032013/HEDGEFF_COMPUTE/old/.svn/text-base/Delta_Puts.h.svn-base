#pragma once
#ifndef DELTA_PUTS_H
#define DELTA_PUTS_H
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Und_Base.h"
#include "Underlying.h"
#include "Tte.h"
#include "Rf.h"
/**
* Class for deltas of puts
*/
class Delta_Puts
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Und_Base* und_base;
		Underlying* underlying;
		Tte* tte;
		Rf* rf;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_option_contract;
		double sell_at_time_to_expiry;
		double reb_freq_gamma;
	} characteristics;*/
	double* asset_data;
	double ** tab;
	void init_tab();
	Delta_Puts(Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf* rf);
	~Delta_Puts(void);
};
#endif