#pragma once
#ifndef UNDERLYING_H
#define UNDERLYING_H
#include "Hedgeff.h"
#include "Assumptions.h"
/**
* Class for underlying value
*/
class Underlying
{
public:
	struct Objects_Needed
	{
		Hedgeff* hedgeff;
		Assumptions* assumptions;
	}objects_needed;
	Underlying(Hedgeff* hedgeff,
		Assumptions* assumptions);
	~Underlying(void);
	double** tab;
	void init_tab();
};
#endif