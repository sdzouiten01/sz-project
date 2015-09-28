#pragma once
#ifndef NEW_OPTION_NOTIONAL_H
#define NEW_OPTION_NOTIONAL_H
class Assumptions;
class Underlying;
class Nbr_Puts;
/**
* Class for new option notional: <BR>
* <CODE>tab[i][j]=(nbr_puts[i][j]-nbr_puts[i-1][j])*underlying[i][j]</CODE>
* <CODE>opt_not[i]+=opt_not[i][j]</CODE>
*/
class New_Option_Notional
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Underlying* underlying;
		Nbr_Puts* nbr_puts;
	} objects_needed;
	struct Characteristics
	{
		double length_of_option_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	double * opt_not;
	double ** tab;
	void init_tab();
	New_Option_Notional(Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Puts* nbr_puts);
	~New_Option_Notional(void);
};
#endif