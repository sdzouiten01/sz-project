#pragma once
#ifndef MARKET_VALUE_H
#define MARKET_VALUE_H

class Assumptions;
class Ov_Base;
class Nbr_Puts;
/**
* Class to know market value of options in porftolio
*/
class Market_Value
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
	double * mkt_val;
	double ** tab;
	void init_tab();
	Market_Value(Assumptions* assumptions,
		Ov_Base* ov_base,
		Nbr_Puts* nbr_puts);
	~Market_Value(void);
};
#endif