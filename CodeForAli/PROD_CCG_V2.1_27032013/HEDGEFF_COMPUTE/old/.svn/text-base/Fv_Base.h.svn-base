#pragma once
#ifndef FV_BASE_H
#define FV_BASE_H
class Assumptions;
class Underlying;
class Tte_Futures;
class Rf_Futures;
class Fv_Base
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Underlying* underlying;
		Tte_Futures* tte_futures;
		Rf_Futures* rf_futures;
	}objects_needed;
	double** tab;
	void init_tab();
	Fv_Base(Assumptions* assumptions,
		Underlying* underlying,
		Tte_Futures* tte_futures,
		Rf_Futures* rf_futures);
	~Fv_Base(void);
};
#endif