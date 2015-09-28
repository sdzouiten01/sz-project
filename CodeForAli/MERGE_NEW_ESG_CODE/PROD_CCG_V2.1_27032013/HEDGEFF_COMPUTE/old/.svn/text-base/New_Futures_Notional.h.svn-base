#pragma once
#ifndef NEW_FUTURES_NOTIONAL_H
#define NEW_FUTURES_NOTIONAL_H
class Assumptions;
class Underlying;
class Nbr_Futures;
class Tte_Futures;
class New_Futures_Notional
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Underlying* underlying;
		Nbr_Futures* nbr_futures;
		Tte_Futures* tte_futures;
	} objects_needed;

	New_Futures_Notional(
		Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Futures* nbr_futures,
		Tte_Futures* tte_futures);
	double* fut_not;
	double** tab;
	void init_tab();
	~New_Futures_Notional(void);
};
#endif