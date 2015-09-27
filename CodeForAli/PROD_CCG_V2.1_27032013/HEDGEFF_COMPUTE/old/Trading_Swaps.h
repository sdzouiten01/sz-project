#pragma once
#ifndef TRADING_SWAPS_H
#define TRADING_SWAPS_H
class Assumptions;
class Sv_Base;
class Nbr_Swaps;
class Trading_Swaps
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Sv_Base* sv_base;
		Nbr_Swaps* nbr_swaps;
	} objects_needed;
	double * trading;
	double ** tab;
	void init_tab();
	Trading_Swaps(Assumptions* assumptions,
		Sv_Base* sv_base,
		Nbr_Swaps* nbr_swaps);
	~Trading_Swaps(void);
};
#endif