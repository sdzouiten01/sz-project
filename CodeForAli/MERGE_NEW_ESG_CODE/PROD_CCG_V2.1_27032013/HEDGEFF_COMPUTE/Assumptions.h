#pragma once
#include <string>

struct Configuration{
	Configuration() :g_model_period(0){}
	//declaration of global variables
	int g_model_period;				// Number of time steps used
	int g_nb_steps_per_year;						// number of time steps per year (=12 if monthly hedge etc)

	double g_moneyness;
	double g_index_base;
	int g_length_of_option_contract;			// length of an option contract
	int g_length_of_future_contract;			// length of a future contract
	int g_sell_at_time_to_expiry;				// sell at time to expiry
	bool g_rates_freq_ann;							// =1 if semi annual annual_swap_rates frequency =0 if annual 
	bool g_sheduled_futures;						// .......

	double g_volatility;								// volatility of asset
	int* g_lbs_maturities;
	int* g_start_ma;
	int* g_end_ma;
	double* g_shocks;
	int* g_compounding_frequencies;

	int* g_swap_maturities;

	int g_nb_lbs_maturities;
	int g_nb_swap_maturities;
	int g_nb_av;
	int g_nb_index_to_hedge;
	int* g_indexes_to_hedge;		// which index will be hedged

	int g_reb_freq_gamma;								// rebalancing frequency of gamma hedge
	int g_reb_freq_delta;									// rebalancing frequency of delta hedge
	int g_reb_freq_rho;										// rebalancing frequency of rho hedge
	
	double g_net_gamma_lower_bound;		// lower bound of the tolerance interval for net gamma
	double g_net_gamma_upper_bound;		// upper bound of the tolerance interval for net gamma
	double g_liab_gamma_lower_bound;		// lower bound of the tolerance interval for liability gamma
	double g_liab_gamma_upper_bound;		// upper bound of the tolerance interval for liability gamma
	
	double g_swap_trade_upper_seuil, g_swap_trade_lower_seuil;						// seuil for swap trade
	bool g_unwind_swap;								// unwind = true, use the intervalle in rho-hedge
	bool g_swap_trade_NO_upper_seuil;               // added 11/01/2011 // if we don't use upper seuil

	double g_delta_trans_cost;						// transaction cost to hedge delta, buy/sell futures
	double g_gamma_trans_cost;					// transaction cost to hedge gamma, buy/sell option put
	double* g_swap_spread;		// transaction cost when sell/buy option (bps of notional)

	bool _isDumpFutureNb, _isDumpOptionNb, _isDumpSwapNb, _isDumpSwapRho;
	
	// format of liability input file
	int g_nb_column_skip_between_claims_and_charges;
	int g_nb_column_skip_between_charges_and_av;
	
	void loadConfig(std::string config, std::string liab, std::string rw_scn);
	void init_rw_maturities(std::string rw_file);

	~Configuration();
};