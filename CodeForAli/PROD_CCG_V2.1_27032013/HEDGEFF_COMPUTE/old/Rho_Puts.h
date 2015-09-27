#pragma once
#ifndef RHO_PUTS_H
#define RHO_PUTS_H
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Und_Base.h"
#include "Underlying.h"
#include "Tte.h"
#include "Rf_Shock_Up.h"
#include "Rf_Shock_Down.h"
/**
* Class for deltas of puts
*/
class Rho_Puts
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Und_Base* und_base;
		Underlying* underlying;
		Tte* tte;
		Rf_Shock_Up* rf_shock_up;
		Rf_Shock_Down* rf_shock_down;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_option_contract;
		double sell_at_time_to_expiry;
		double reb_freq_gamma;
	} characteristics;*/
	int swap_no;
	double* asset_data;
	double ** tab;
	void init_tab();
	Rho_Puts(int swap_no,Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf_Shock_Up* rf_shock_up,
		Rf_Shock_Down* rf_shock_down);
	~Rho_Puts(void);
};
#endif