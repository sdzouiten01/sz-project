#pragma once
#ifndef GAMMA_PUTS_H
#define GAMMA_PUTS_H
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Und_Base.h"
#include "Underlying.h"
#include "Tte.h"
#include "Rf.h"
/**
* Class fpr gamma of puts in porfolio
*/
class Gamma_Puts
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
	double ** tab;
	void init_tab();
	Gamma_Puts(Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf* rf);
	~Gamma_Puts(void);
};
#endif