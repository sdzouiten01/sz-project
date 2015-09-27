#pragma once
#ifndef ASSUMPTIONS_H
#define ASSUMPTIONS_H
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
using namespace std;
/**
* Class
*/
class Assumptions
{
public:
	int reb_freq_delta;/** rebalancing frequency of hedging portofolio for delta hedge*/
	int reb_freq_gamma;/** rebalancing frequency of hedging portofolio for gamma hedge*/
	int reb_freq_rho;/** rebalancing frequency of hedging portofolio for rho hedge*/
	double volatility;/** volatility of asset*/
	int length_of_option_contract;/** length of an option contracts*/
	int sell_at_time_to_expiry;/** sell at time to expiry*/
	int length_of_futures_contract;/** lenght of a future contract*/
	int length_of_swaps_contract;/** lenght of a future contract*/
	double gamma_per_cont;
	double delta_per_cont;
	double moneyness;
	int model_period;/** Number of time steps used*/
	double net_gamma_lower_bound;/** lower bound of the tolerance interval for net gamma*/
	double net_gamma_upper_bound;/** upper bound of the tolerance interval for net gamma*/
	double liab_gamma_lower_bound;/** lower bound of the tolerance interval for liability gamma*/
	double liab_gamma_upper_bound;/** upper bound of the tolerance interval for liability gamma*/
	double delta_transaction_cost;/** transaction cost to hedge delta (i.e. when buy/sell futures*/
	double rho_transaction_cost;/** transaction cost to hedge delta (i.e. when buy/sell futures*/
	bool hedge_delta;
	double option_transaction_cost;/** transaction cost when sell/buy option (bps of notional)*/
	int nb_steps_per_year;/** number of time steps per year (=12 if monthly hedge etc)*/
	bool hedge_gamma;/** TRUE if and only if gamma has to be hedged*/
	bool hedge_rho;
	bool rates_freq_ann;/** =1 if semi annual annual_swap_rates frequency =0 if annual */
	bool sheduled_futures;
	int next_time_step_sheduled;

	double* maturities_to_hedge;
	int nb_maturities_to_hedge;
	double* shocks_to_apply;
	double* compounding_frequencies;
	double index_base;

//chth221209-2
double* start_mty;
double* end_mty;

	/** Constructor of Assumptions class*/
	Assumptions(
		int model_period,
		double moneyness,
		int reb_freq_delta,
		int reb_freq_gamma,
		int reb_freq_rho,
		double volatility,
		int length_of_option_contract,
		int sell_at_time_to_expiry,
		int length_of_futures_contract,
		double gamma_per_cont,
		double delta_per_cont,
		double net_gamma_lower_bound,
		double net_gamma_upper_bound,
		double liab_gamma_lower_bound,
		double liab_gamma_upper_bound,
		double delta_transaction_cost,
		double option_transaction_cost,
		double rho_transaction_cost,
		bool  hedge_delta,
		bool  hedge_gamma,
		bool  hedge_rho,
		bool rates_freq_ann,
		bool sheduled_futures,
		int next_time_step_sheduled,
		int nb_steps_per_year,
		int nb_maturities_to_hedge,
		double* maturities_to_hedge,
		double* shocks_to_apply,
		double* compounding_frequencies,
		double index_base,
		//chth221209-2
		double* start_mty,
		double* end_mty
		);
	void init_fields(string file_input_name);
	~Assumptions(void);
};
#endif 