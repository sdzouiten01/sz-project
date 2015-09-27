#pragma once
#ifndef REPORT_H
#define REPORT_H

#include "Math_Tools.h"

class Hedgeff;
class Assumptions;
class Gamma_Asset;
class Option_Notional;
class Trading;
class Market_Value;
class New_Option_Notional;
class Nbr_Puts;
class Gamma_Puts_In_Portfolio_2;
class Option_Trade;
/**
* Class to be diplayed by launcher functions to get hedging output
*/
class Gamma
{
public:
	struct Objects_Needed
	{
		Hedgeff* hedgeff;
		Assumptions* assumptions;
		Gamma_Asset* gamma_asset;
		Option_Notional* option_notional;
		New_Option_Notional* new_option_notional;
		Trading* trading;
		Market_Value* market_value;
		Nbr_Puts* nbr_puts;
		Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2;
		Option_Trade* option_trade;
	}objects_needed;
	struct Output_In_Moses 
	{

		double * mkt_val;
	} output_in_moses;
	struct Hand_Calculate
	{
		//double * period;
	//	double * gamma_new_asset;
		double * stock_return;
		//double * underlying;
		double * opt1_not;
		double * option_pursh_sales;
		double * option_pursh;
		double * option_sales;
		double * mkt_value;
		double * pnl_opts;
		double * trans_costs;
		double * eq_put_1_ct;
	} hand_calculate;
		double * asset_gamma_before_options;
		double * asset_gamma_after_options;
		double * net_gamma_before_options;
		double * net_gamma_after_options;
		double total_pnl;
		double total_trans_costs;

	Gamma(Hedgeff* hedgeff, Assumptions* assumptions,Gamma_Asset* gamma_asset,
		Option_Notional* option_notional,New_Option_Notional* new_option_notional,
		Trading* trading,Market_Value* market_value,Nbr_Puts* nbr_puts,
		Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2,Option_Trade* option_trade);
	~Gamma(void);
	//void init_period();
	void init_fields();

	void init_asset_gamma_before_options();
	void init_net_gamma_before_options();
	void init_asset_gamma_after_options();
	void init_net_gamma_after_options();



	//void init_gamma_new_asset();
	void init_eq_put_1_ct();
	//void init_stock_return();
	//void init_underlying();
	void init_opt1_not();
	void init_option_pursh_sales();
	void init_option_pursh();
	void init_option_sales();
	void init_mkt_value();
	void init_pnl_opts();
	void init_trans_costs();
};
#endif 