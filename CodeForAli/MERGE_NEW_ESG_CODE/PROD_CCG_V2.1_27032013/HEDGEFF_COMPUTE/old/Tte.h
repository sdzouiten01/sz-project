#pragma once
#ifndef TTE_H
#define TTE_H
#include "Assumptions.h"
/**
* Class to know time to expiry for option contracts
*/
class Tte
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
	} objects_needed;
	Tte(Assumptions* assumptions);
	~Tte(void);
	double** tab;
	void init_tab();
};
#endif