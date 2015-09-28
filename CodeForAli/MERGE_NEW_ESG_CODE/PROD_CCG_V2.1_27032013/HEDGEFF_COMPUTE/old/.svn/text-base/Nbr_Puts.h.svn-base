#pragma once
#ifndef NBRPUTS_H
#define NBRPUTS_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Existing_Gamma;
class Option_Trade;
class Gamma_Puts_In_Portfolio;
class Gamma_Puts_In_Portfolio_2;
/**
/**
* Class for numbers of futures in portfolio using our delta hedging Strategy: 
* Gamma Neutral strategy, and preference to use existing options contracts when it is possible.
*/
class Nbr_Puts
{
public:
	struct Objects_Needed
	{
		Option_Trade* option_trade;
		Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio;
		Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2;
		Existing_Gamma* existing_gamma;
		Assumptions* assumptions;
	}objects_needed;
	/*struct Characteristics
	{
		double sigma;
		double q;
		double length_of_option_contract;
		double sell_at_time_to_expiry;
	} characteristics;*/
	ofstream debug_file;
	double * numbers_of_puts;
	double ** tab;
	void init_fields();
	void init_sigma();
	void init_q();
	void init_length_of_option_contract();
	void init_sell_at_time_to_expiry();
	void init_tab();
	void init_tab(int i, int j);
	void init_numbers_of_puts();
	Nbr_Puts(Option_Trade* option_trade,
		Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio,
		Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2,
		Existing_Gamma* existing_gamma,
		Assumptions* assumptions);

	~Nbr_Puts(void);
};
#endif