#pragma once
#ifndef GAMMA_PUTS_IN_PORTFOLIO_H
#define GAMMA_PUTS_IN_PORTFOLIO_H
#include "Nbr_Puts.h"
#include "Gamma_Puts.h"
#include "Assumptions.h"
/**
* Class for Deltas due to futures in porfolio <BR>
* <CODE>tab[i][j]=delta_puts[i][j]*nbr_puts[i-1][j]+ tab[i][j-1]</CODE>
*/
class Gamma_Puts_In_Portfolio
{
public:
	struct Objects_Needed
	{
		Nbr_Puts* nbr_puts;
		Gamma_Puts* gamma_puts;
		Assumptions* assumptions;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_option_contract;
		double sell_at_time_to_expiry;
		double reb_freq_gamma;
	} characteristics;*/
	double ** tab;
	void init_tab(int i, int j);
	Gamma_Puts_In_Portfolio(Assumptions* assumptions,Gamma_Puts* gamma_puts);
private:
private:
private:
private:

private:
	~Gamma_Puts_In_Portfolio(void);
};
#endif