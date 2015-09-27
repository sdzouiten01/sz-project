#pragma once
#ifndef TTE_SWAPS_H
#define TTE_SWAPS_H
#include "Assumptions.h"
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
/**
* Class to know time to expiry for futures contracts
*/
class Tte_Swaps
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
	} objects_needed;
	Tte_Swaps(int swap_no,Assumptions* assumptions);
	~Tte_Swaps(void);
	int swap_no;
	double** tab;
	bool isNullPrevTte(int i, int j);
	bool isMaturityReached(int i, int j);
	void init_tab_shift();
	void init_tab_sheduled();
};
#endif