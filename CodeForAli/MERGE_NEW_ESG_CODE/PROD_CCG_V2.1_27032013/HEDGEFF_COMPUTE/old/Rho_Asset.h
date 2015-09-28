#pragma once
#ifndef RHO_ASSET_H
#define RHO_ASSET_H
#include "Math_Tools.h"

class Assumptions;
class Rho_Swaps;
/**
* Class for deltas of new futures
*/
class Rho_Asset
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Rho_Swaps* rho_swaps;
	}objects_needed;
	double* tab;
	void init_tab();
	void init_time(int i);
	int swap_no;
	Rho_Asset(int swap_no,Rho_Swaps* rho_swaps);
	~Rho_Asset(void);
};
#endif