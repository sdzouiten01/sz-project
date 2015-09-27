#pragma once
#ifndef RHO_H
#define RHO_H
class Rho_In_Portfolio;
class Hedgeff;
class Assumptions;
class Swaps_Trade;
class Nbr_Swaps;
class Market_Value_Swaps;
class Trading_Swaps;
class New_Swaps_Notional;
/**
* Class to sum up delta results
*/
class Rho
{
public:
	int swap_no;
	struct Objects_Needed
	{
		Swaps_Trade* swaps_trade;
		Rho_In_Portfolio* rho_in_portfolio;
		Hedgeff* hedgeff;
		Assumptions* assumptions;
		Nbr_Swaps* nbr_swaps;
		Market_Value_Swaps* market_value_swaps;
		Trading_Swaps* trading_swaps;
		New_Swaps_Notional* new_swaps_notional;
	}objects_needed;

	struct Inputs
	{
		double * index;
	} inputs;
	double * int_rate;
	double * div_rate;
	struct Liability_Rho
	{
		double * rho_liab;
	} liability_rho;
	struct Asset_Rho
	{
		double* asset_rho_before_swaps;
		double* asset_rho_after_swaps;
	} asset_rho;
	double *  net_rho_before_swaps;
	double *  net_rho_after_swaps;
	struct Swap_Rho
	{
		//double * delta_new_eq_fut_1;
	} swap_rho;
	struct Nb_Of_Swaps
	{
		double * rt_swp_1_ct;
	} nb_of_swaps;	
	double * trans_cost;	
	double * mkt_value;
	double * total_notional;
	double * int_cont;
	double * div_cont;
	double * settle_cont;
	double * swap_pursh_sales;
	double * swap_pursh;
	double * swap_sales;
	double * pnl_rt_swp;
	double total_pnl;
	double total_trans_costs;
	void init_trans_costs();
	void init_index();
	void init_int_rate();
	void init_swap_pursh_sales();
	void init_swap_pursh();
	void init_swap_sales();
	void init_mkt_value();
	void init_rho_liab();
	void init_asset_rho_before_swaps();
	void init_net_rho_before_futures();
	void init_asset_rho_after_swaps();
	void init_net_rho_after_swaps();
	//void init_delta_new_eq_fut1();
	void init_rt_swp_1_ct();
	void init_total_notional();
	void init_int_cont();
	void init_div_cont();
	void init_settle_cont();
	void init_pnl_rt_swaps();

	Rho(int swap_no,Rho_In_Portfolio* rho_in_portfolio,Hedgeff* hedgeff,Assumptions* assumptions,
		 Swaps_Trade* swaps_trade,Nbr_Swaps* nbr_swaps,
		 Market_Value_Swaps* market_value_swaps,Trading_Swaps* trading_swaps,New_Swaps_Notional* new_swaps_notional);
	~Rho(void);
};
#endif 