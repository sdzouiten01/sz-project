#pragma once
#ifndef TTE_FUTURES_H
#define TTE_FUTURES_H
#include "Assumptions.h"
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
/**
* Class to know time to expiry for futures contracts
*/
class Tte_Futures
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
	} objects_needed;
	Tte_Futures(Assumptions* assumptions);
	~Tte_Futures(void);
	double** tab;
	bool isNullPrevTte(int i, int j);
	bool isMaturityReached(int i, int j);
	void init_tab_shift();
	void init_tab_sheduled();
};
#endif