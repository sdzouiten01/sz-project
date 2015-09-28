#pragma once
#ifndef OV_BASE_H
#define OV_BASE_H
class Assumptions;
class Und_Base;
class Underlying;
class Tte;
class Rf;
/**
* Class for option value base
*/
class Ov_Base
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
	Ov_Base(Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf* rf);
	~Ov_Base(void);
};
#endif