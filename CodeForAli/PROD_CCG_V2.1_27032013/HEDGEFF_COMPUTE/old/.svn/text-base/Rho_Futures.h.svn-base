#pragma once
#ifndef RHO_FUTURES_H
#define RHO_FUTURES_H
class Assumptions;
class Tte_Futures;
class Rf_Futures_Shock_Up;
class Rf_Futures_Shock_Down;
class Underlying;
/**
* Class for deltas of futures used in hedging portfolio
*/
class Rho_Futures
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Tte_Futures* tte_futures;
		Rf_Futures_Shock_Up* rf_futures_shock_up;
		Rf_Futures_Shock_Down* rf_futures_shock_down;
		Underlying* underlying;
	}objects_needed;
	double** tab;
	Rho_Futures(
		int swap_no,
		Assumptions* assumptions,
		Tte_Futures* tte_futures,
		Rf_Futures_Shock_Up* rf_futures_shock_up,
		Rf_Futures_Shock_Down* rf_futures_shock_down,
		Underlying* underlying
		);
	int swap_no;
	void init_tab();
	~Rho_Futures(void);
};
#endif