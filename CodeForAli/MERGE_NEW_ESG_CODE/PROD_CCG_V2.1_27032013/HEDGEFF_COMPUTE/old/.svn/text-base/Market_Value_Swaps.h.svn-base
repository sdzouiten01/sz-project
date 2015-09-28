#pragma once
#ifndef MARKET_VALUE_SWAPS_H
#define MARKET_VALUE_SWAPS_H

class Assumptions;
class Sv_Base;
class Nbr_Swaps;
class Market_Value_Swaps
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Sv_Base* sv_base;
		Nbr_Swaps* nbr_swaps;
	} objects_needed;
	double** tab;
	double* mkt_val;
	void init_tab();
	Market_Value_Swaps(Assumptions* assumptions,
		Sv_Base* sv_base,
		Nbr_Swaps* nbr_swaps);
	~Market_Value_Swaps(void);
};
#endif