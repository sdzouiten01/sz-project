#pragma once
#ifndef DELTA_GAMMA_HEDGE_H
#define DELTA_GAMMA_HEDGE_H
#include <fstream>
//#include "PL.h"
#include "Assumptions.h"
#include "Hedgeff.h"
#include "Gamma.h"
#include "Delta.h"
#include "Underlying.h"
#include "Und_Base.h"
#include "Gamma_Puts.h"
#include "Tte.h"
#include "Gamma_Asset.h"
#include "Gamma_Puts_In_Portfolio.h"
#include "Gamma_Puts_In_Portfolio_2.h"
#include "Option_Trade.h"
#include "Existing_Gamma.h"
#include "Nbr_Puts.h"
#include "Ov_Base.h"
#include "Trading.h"
#include "Market_Value.h"
#include "Delta_Puts.h"
#include "Delta_In_Portfolio.h"
#include "Option_Notional.h"
#include "New_Option_Notional.h"
#include "Rw_Scn.h"
#include "Annual_Swap_Rates.h"
#include "Zero_Coupon_Rates.h"
#include "Rf.h"
#include "Delta_Futures.h"
#include "Nbr_Futures.h"
#include "Tte_Futures.h"
#include "Rf_Futures.h"
#include "Delta_Asset.h"
#include "Delta_Futures_In_Portfolio.h"
#include "Delta_Futures_In_Portfolio_2.h"
#include "Future_Trade.h"
#include "Existing_Delta.h"
#include "Existing_Delta_Futures.h"
#include "Nbr_Futures.h"
#include "Fv_Base.h"
#include "Market_Value_Futures.h"
#include "Trading_Futures.h"
#include "New_Futures_Notional.h"
#include "Nbr_Swaps.h"
#include "Rho_Asset.h"
#include "Rho_Futures.h"
#include "Rho_Puts.h"
#include "Rho_Swaps.h"
#include "Rho_Swaps_In_Portfolio.h"
#include "Rho_Swaps_In_Portfolio_2.h"
#include "Swaps_Trade.h"
#include "Annual_Swap_Rates_Shock_Down.h"
#include "Annual_Swap_Rates_Shock_Up.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
#include "Zero_Coupon_Rates_Shock_Up.h"
#include "Rf_Shock_Up.h"
#include "Rf_Shock_Down.h"
#include "Rf_Futures_Shock_Up.h"
#include "Rf_Futures_Shock_Down.h"
#include "Tte_Swaps.h"
#include "Existing_Rho.h"
#include "Existing_Rho_Swaps.h"
#include "Sv_Base.h"
#include "Market_Value_Swaps.h"
#include "Trading_Swaps.h"
#include "Rho_In_Portfolio.h"
#include "New_Swaps_Notional.h"
#include "Rho.h"
#include <stdlib.h>// for MAX_PATH
using namespace std;
/**
* Class to sum up all hedging results for a given scenario and a given asset <BR>
* An object of this class contains pointers to objects of all other classes
*/
class Delta_Gamma_Hedge
{
public:
	
	Assumptions* assumptions;
	//PL* pl;
	Hedgeff* hedgeff;
	Gamma* gamma;
	Delta* delta;

	Underlying* underlying;
	Und_Base* und_base;
	Gamma_Puts* gamma_puts;
	New_Swaps_Notional** new_swaps_notional;
	Tte* tte;
	Gamma_Asset* gamma_asset;
	Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio;
	Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2;
	Option_Trade* option_trade;
	Existing_Gamma* existing_gamma;
	Nbr_Puts* nbr_puts;
	Ov_Base* ov_base;
	Trading* trading;
	Market_Value* market_value;
	Delta_Puts* delta_puts;
	Delta_In_Portfolio* delta_in_portfolio;
	Option_Notional* option_notional;
	New_Option_Notional* new_option_notional;
	Rw_Scn* rw_scn;
	Annual_Swap_Rates* annual_swap_rates;
	Zero_Coupon_Rates* zero_coupon_rates;
	Rf* rf;
	Delta_Futures* delta_futures;
    Nbr_Futures* nbr_futures;
	Tte_Futures* tte_futures;
	Rf_Futures* rf_futures;
	Delta_Asset* delta_asset;
	Delta_Futures_In_Portfolio* delta_futures_in_portfolio;
	Delta_Futures_In_Portfolio_2* delta_futures_in_portfolio_2;
	Future_Trade* future_trade;
	Existing_Delta* existing_delta;
	Existing_Delta_Futures* existing_delta_futures;
	Fv_Base* fv_base;
	Market_Value_Futures* market_value_futures;
	Trading_Futures* trading_futures;
	New_Futures_Notional* new_futures_notional;

	Nbr_Swaps** nbr_swaps;
	Rho_Asset** rho_asset;
	Rho_Futures** rho_futures;
	Rho_Puts** rho_puts;
	Rho_Swaps** rho_swaps;
	Rho_Swaps_In_Portfolio** rho_swaps_in_portfolio;
	Rho_Swaps_In_Portfolio_2** rho_swaps_in_portfolio_2;
	Swaps_Trade** swaps_trade;
	Annual_Swap_Rates_Shock_Down** annual_swap_rates_shock_down;
	Annual_Swap_Rates_Shock_Up** annual_swap_rates_shock_up;
	Zero_Coupon_Rates_Shock_Down** zero_coupon_rates_shock_down;
	Zero_Coupon_Rates_Shock_Up** zero_coupon_rates_shock_up;
	Rf_Shock_Up** rf_shock_up;
	Rf_Shock_Down** rf_shock_down;
	Rf_Futures_Shock_Up** rf_futures_shock_up;
	Rf_Futures_Shock_Down** rf_futures_shock_down;
	Tte_Swaps** tte_swaps;
	Existing_Rho** existing_rho;
	Existing_Rho_Swaps** existing_rho_swaps;
	Sv_Base** sv_base;
	Market_Value_Swaps** market_value_swaps;
	Rho_In_Portfolio** rho_in_portfolio;
	Trading_Swaps** trading_swaps;
	Rho** rho;
	ofstream debug_file;
	bool debug;
	Delta_Gamma_Hedge(
		string hedgeff_path,
		string rw_scn_path,
		string outputs_path,
		int index_no,
		int scen_no,
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
	int error;
	void set_error(int error);
	void erase_delta();
	void erase_gamma();

	void erase_nbr_swaps();
	void erase_rho_asset();
	void erase_rho_futures();
	void erase_rho_puts();
	void erase_rho_swaps();
	void erase_rho_swaps_in_portfolio();
	void erase_rho_swaps_in_portfolio_2();
	void erase_swaps_trade();
	void erase_annual_swap_rates_shock_down();
	void erase_trading_futures();
	void erase_market_value_futures();
	void erase_fv_base();
	void erase_delta_in_portfolio();
	void erase_nbr_futures();
	void erase_existing_delta_futures();
	void erase_existing_delta();
	void erase_future_trade();
	void erase_delta_futures_in_portfolio_2();
	void erase_delta_futures_in_portfolio();
	void erase_delta_asset();
	void erase_delta_futures();
	void erase_rf_futures();
	void erase_tte_futures();
	void erase_delta_puts();
	void erase_new_option_notional();
	void erase_new_futures_notional();
	void erase_option_notional();
	void erase_trading();
	void erase_market_value();
	void erase_ov_base();
	void erase_und_base();
	void erase_nbr_puts();
	void erase_existing_gamma();
	void erase_option_trade();
	void erase_gamma_puts_in_portfolio_2();
	void erase_gamma_puts_in_portfolio();
	void erase_gamma_asset();
	void erase_gamma_puts();
	void erase_underlying();
	void erase_rf();
	void erase_zero_coupon_rates();	
	void erase_annual_swap_rates();
	void erase_tte();
	void erase_rw_scn();
	void erase_hedgeff();
	void erase_assumptions();

	void erase_objects();
	~Delta_Gamma_Hedge(void);
};
#endif 