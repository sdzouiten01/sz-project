#pragma once
#ifndef DELTA_H
#define DELTA_H
class Delta_In_Portfolio;
class Hedgeff;
class Assumptions;
class Option_Trade;
class Future_Trade;
class Nbr_Futures;
class Market_Value_Futures;
class Trading_Futures;
class New_Futures_Notional;
class Rf;
/**
* Class to sum up delta results
*/
class Delta
{
public:
	struct Objects_Needed
	{
		Future_Trade* future_trade;
		Delta_In_Portfolio* delta_in_portfolio;
		Hedgeff* hedgeff;
		Assumptions* assumptions;
		Option_Trade* option_trade;
		Nbr_Futures* nbr_futures;
		Rf* rf;
		Market_Value_Futures* market_value_futures;
		Trading_Futures* trading_futures;
		New_Futures_Notional* new_futures_notional;
	}objects_needed;
	struct Inputs
	{
		double * index;
	} inputs;
	double * int_rate;
	double * div_rate;
	struct Liability_Delta
	{
		double * delta_liab;
	} liability_delta;
	struct Asset_Delta
	{
		double* asset_delta_before_futures;
		double* asset_delta_after_futures;
	} asset_delta;
	double *  net_delta_before_futures;
	double *  net_delta_after_futures;
	struct Future_Delta
	{
		//double * delta_new_eq_fut_1;
	} future_delta;
	struct Nb_Of_Futures
	{
		double * eq_fut_1_ct;
	} nb_of_futures;	
	double* trans_cost;	
	double* mkt_value;
	double * total_notional;
	double * int_cont;
	double * div_cont;
	double * settle_cont;
	double * future_pursh_sales;
	double * future_pursh;
	double * future_sales;
	double * pnl_eq_fut;
	double total_pnl;
	double total_trans_costs;
	void init_trans_costs();
	void init_index();
	void init_int_rate();
	void init_future_pursh_sales();
	void init_future_pursh();
	void init_future_sales();
	void init_mkt_value();
	void init_delta_liab();
	void init_asset_delta_before_futures();
	void init_net_delta_before_futures();
	void init_asset_delta_after_futures();
	void init_net_delta_after_futures();
	//void init_delta_new_eq_fut1();
	void init_eq_fut_1_ct();
	void init_total_notional();
	void init_int_cont();
	void init_div_cont();
	void init_settle_cont();
	void init_pnl_eq_futures();

	Delta(Delta_In_Portfolio* delta_in_portfolio,Hedgeff* hedgeff,Assumptions* assumptions,
		 Option_Trade* option_trade,Future_Trade* future_trade,Nbr_Futures* nbr_futures,Rf* rf,
		 Market_Value_Futures* market_value_futures,Trading_Futures* trading_futures,New_Futures_Notional* new_futures_notional);
	~Delta(void);
};
#endif 