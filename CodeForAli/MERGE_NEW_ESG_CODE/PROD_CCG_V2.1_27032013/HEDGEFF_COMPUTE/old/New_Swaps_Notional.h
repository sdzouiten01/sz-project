#pragma once
#ifndef NEW_SWAPS_NOTIONAL_H
#define NEW_SWAPS_NOTIONAL_H
class Assumptions;
class Underlying;
class Nbr_Swaps;
class Tte_Swaps;
class New_Swaps_Notional
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Underlying* underlying;
		Nbr_Swaps* nbr_swaps;
		Tte_Swaps* tte_swaps;
	} objects_needed;

	New_Swaps_Notional(
		Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Swaps* nbr_swaps,
		Tte_Swaps* tte_swaps);
	double* swap_not;
	double** tab;
	void init_tab();
	~New_Swaps_Notional(void);
};
#endif