#pragma once
#ifndef TRADING_H
#define TRADING_H
class Assumptions;
class Ov_Base;
class Nbr_Puts;
/**
* Class to know how to trade options
*/
class Trading
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Ov_Base* ov_base;
		Nbr_Puts* nbr_puts;
	} objects_needed;
	struct Characteristics
	{
		double length_of_option_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	double * trading;
	double ** tab;
	void init_tab();
	Trading(Assumptions* assumptions,
		Ov_Base* ov_base,
		Nbr_Puts* nbr_puts);
	~Trading(void);
};
#endif