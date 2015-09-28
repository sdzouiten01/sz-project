#pragma once
#include "Rho_Puts.h"
#include "Rho_Futures.h"
#include "Tte_Swaps.h"
#include "Rho_Swaps.h"
#include "Rho_Asset.h"
#include "Rho_Swaps_In_Portfolio.h"
#include "Rho_Swaps_In_Portfolio_2.h"
#include "Swaps_Trade.h"
#include "Existing_Rho.h"
#include "Existing_Rho_Swaps.h"
#include "Nbr_Swaps.h"
#include "Sv_Base.h"
#include "Trading_Swaps.h"
#include "Market_Value_Swaps.h"
#include "Rho_In_Portfolio.h"
#include "New_Swaps_Notional.h"
//#include "Rw_Scn_Up.h"
//#include "Rw_Scn_Down.h"
#include "Annual_Swap_Rates_Shock_Down.h"
#include "Annual_Swap_Rates_Shock_Up.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
#include "Zero_Coupon_Rates_Shock_Up.h"
#include "Rf_Shock_Down.h"
#include "Rf_Shock_Up.h"
#include "Rf_Futures_Shock_Down.h"
#include "Rf_Futures_Shock_Up.h"
#include "Rho.h"
//#include "Assumptions.h"
//#include "Zero_Coupon_Rates.h"
//#include "Hedgeff.h"
//#include "Nbr_Puts.h
//#include "Nbr_Futures.h"
//#include "Rw_Scn.h"

#include "Delta_Gamma_Hedge.h"


class Rho_Hedge
{
public:
	Rho_Puts* rho_puts;
	Rho_Puts** rho_puts_glob;
	Rho_Futures* rho_futures;
	Rho_Futures** rho_futures_glob;
	Tte_Swaps* tte_swaps;
	Rho_Swaps* rho_swaps;
	Rho_Asset* rho_asset;
	Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio;
	Rho_Swaps_In_Portfolio_2* rho_swaps_in_portfolio_2;
	Swaps_Trade* swaps_trade;
	Existing_Rho* existing_rho;
	Existing_Rho_Swaps* existing_rho_swaps;
	Nbr_Swaps* nbr_swaps;
	Sv_Base* sv_base;
	Trading_Swaps* trading_swaps;
	Market_Value_Swaps* market_value_swaps;
	Rho_In_Portfolio* rho_in_portfolio;
	New_Swaps_Notional* new_swaps_notional;
	Rho* rho;

	Annual_Swap_Rates_Shock_Down* annual_swap_rates_shock_down;
	Annual_Swap_Rates_Shock_Up* annual_swap_rates_shock_up;
	Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down;
	Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up;
	Rf_Shock_Down* rf_shock_down;
	Rf_Shock_Up* rf_shock_up;
	Rf_Futures_Shock_Down* rf_futures_shock_down;
	Rf_Futures_Shock_Up* rf_futures_shock_up;

	Delta_Gamma_Hedge** tab_of_delta_gamma_hedge;  // !YY
	
	Assumptions* assumptions;
	Hedgeff* hedgeff;
	Rw_Scn* rw_scn;
	Tte* tte;
	Tte_Futures* tte_futures;
	Und_Base* und_base;
	Und_Base** und_base_glob;
	Underlying* underlying;
	Underlying** underlying_glob;
	Zero_Coupon_Rates* zero_coupon_rates;
	Nbr_Puts* nbr_puts;
	Nbr_Puts** nbr_puts_glob;
	Nbr_Futures* nbr_futures;
	Nbr_Futures** nbr_futures_glob;

	int nb_index;
	int scen_no;
	Rho_Hedge(int swap_no,Delta_Gamma_Hedge** tab_of_delta_gamma_hedge,int nb_index,int scen_no);
	~Rho_Hedge(void);
};
