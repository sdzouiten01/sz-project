#pragma once
#ifndef LAUNCHER_H
#define LAUNCHER_H
//#include "Rates_Reader.h"
#include "Delta_Gamma_Hedge.h"
#include "Rho_Hedge.h"
//#include "Display.h"
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
#include <math.h>


class Gamma;
class Gamma_Asset;
class Option_Trade;
class Debug;
/**
* Class to sum up all hedging results for every scenario and every asset <BR>
* An object of this class contains pointers to objects of all other classes
*/
class Launcher
{
public:
	//ofstream debug_file;
	double* new_gamma_liab;
	double* new_gamma_asset;
	double* new_opt1_not;
	double* new_option_pursh;
	double* new_option_sales;
	double* new_mkt_value;
	int nb_of_scen;
	int nb_index_to_cover;
	int nb_maturities_to_hedge;

	//chth221209
	//int my_chth;

	/*Delta_Gamma_Hedge *** tab_of_delta_gamma_hedge;*/
	/*Rho_Hedge*** tab_of_rho_hedge;*/
	Launcher(
		int start_scn,
		int end_scn,
		int nb_of_scen,
		int nb_index_to_cover,
		string hedgeff_path,
		string rw_scn_path,
		int model_period,
		string output_path,
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
		double rho_transaction_cost,
		double option_transaction_cost,
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
		double* index_bases,
		double* index_to_hedge,
		double* scen_to_hedge,

		//chth221209
		//int my_chth
		//chth221209-2
		double* start_mty,
		double* end_mty
	);
	int error;
	// Comments by Y & Y
	//double return_total_pnl(int scen);
	// Comments by Y & Y
	//double return_total_fut_trans_costs(int scen);
	// Comments by Y & Y
	//double return_total_opt_trans_costs(int scen);
	// Comments by Y & Y
	//double return_total_pnl_opts_glob(int scen);
	//double return_total_pnl_futs_glob(int scen);
	// Comments by Y & Y
	//double return_total_liab_ov_change(int scen);
	// Comments by Y & Y
	//double return_total_cash(int scen);
	double* extract_portfolio_pnl(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge);
	double* extract_total_pnl(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge ** tab_of_rho_hedge);
	double return_total_trans_costs(int scen);
	double* extract_fut_opt_swp_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,Rho_Hedge **tab_of_rho_hedge);
	double* extract_pnl_opt_fut_swp_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,Rho_Hedge **tab_of_rho_hedge);
	double* extract_pnl_opt_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge);
	double* extract_opt_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge);
	double* extract_fut_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge);
	double* extract_pnl_eq_fut_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge);
	double* extract_swp_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge);
	double* extract_pnl_rt_swp_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge);
	double* extract_settle_cont_glob(int scen);
	double* extract_int_cont_glob(int scen);
	double* liab_ov_change(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge);
	double* extract_cash_part(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,Rho_Hedge ** tab_of_rho_hedge);
	void display_report1(string ouputs_path);
	
	//Comments by Yuan & Yann
	//void display_report1_bis(string ouputs_path);
	
	void display_one_dim_tab(double*  one_dim_tab,string name,string out_path,int scen_no,int asset_no);
	void display_two_dim_tab(double** two_dim_tab,string name,string out_path,int scen_no,int asset_no,int row_nb,int col_b);
	void display_gamma_asset(Gamma_Asset* gamma_asset,string name,string out_path,int scen_no,int asset_no,Delta_Gamma_Hedge** tab_of_delta_gamma_hedge);
	void display_asset_purchase(Option_Trade* option_trade,string name,string out_path,int scen_no,int asset_no);
	void display_futures_purchase(Future_Trade* future_trade,string name,string out_path,int scen_no,int asset_no,Delta_Gamma_Hedge** tab_of_delta_gamma_hedge);
	void display_delta_asset(Delta_Asset* delta_asset,string name,string out_path,int scen_no,int asset_no,Delta_Gamma_Hedge** tab_of_delta_gamma_hedge);
	void display_report1_bis(string outputs_path, 
								   Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,
								   Rho_Hedge ** tab_of_rho_hedge,
								   int nb_of_scenarios, int start_scn, int end_scn);
	void display_header(string outputs_path, Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, int start_scn, int end_scn);

	~Launcher(void);
};
#endif;