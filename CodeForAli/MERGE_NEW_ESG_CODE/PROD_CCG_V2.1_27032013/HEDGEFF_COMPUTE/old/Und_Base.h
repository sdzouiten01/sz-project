#pragma once
#ifndef UND_BASE_H
#define UND_BASE_H
class Underlying;
class Assumptions;
class Underlying;
/**
* Class for underlying base
*/
class Und_Base
{
public:
	struct Objects_Needed
	{
		Underlying* underlying;
		Assumptions* assumptions;
	}objects_needed;
	Und_Base(Underlying* underlying,
		Assumptions* assumptions);
	~Und_Base(void);
	double** tab;
	void init_tab();
};
#endif