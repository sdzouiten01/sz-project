#pragma once
#ifndef SWAPS_TRADE_H
#define SWAPS_TRADE_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Rho_Asset;
class Hedgeff;
class Existing_Rho_Swaps;
class Existing_Rho;
class Rho_Swaps_In_Portfolio;
class Rho_Swaps_In_Portfolio_2;
/**
* Intermediary Class to used by  <CODE> Nbr_Puts <\CODE> to determine the number of new futures to sale/purchase
* @see Nbr_Swaps
*/
class Swaps_Trade
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Rho_Asset* rho_asset;
		Hedgeff* hedgeff;
		Existing_Rho_Swaps* existing_rho_swaps;
		Existing_Rho* existing_rho;
		Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio;
		Rho_Swaps_In_Portfolio_2* rho_swaps_in_portfolio_2;
	}objects_needed;
	struct Rho_Bounds
	{
		double net_rho_lower_bound;
		double net_rho_upper_bound;
		double liab_rho_lower_bound;
		double liab_rho_upper_bound;
	} rho_bounds;
	struct Characteristics
	{
		//struct Rho_Bounds rho_bounds;	
		//double shock_size;
		double length_of_swaps_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	typedef enum {Ex_Rho_Swp_Pos,Ex_Rho_Swp_Neg,Nb_Cols_In_Tab} Tab_Cols;
	typedef enum {Net_Rho_Pos,Net_Rho_Neg,Nb_Rows_In_Tab} Tab_Rows;
	struct Swaps_Trade_Struct
	{
		int time;
		bool hedging;
		int ** tab;
		double ex_rho;
		double ex_rho_swaps;
		double rho_asset;
		double rho_liability;
		double net_rho;
	//	double req_delta_liability;
	//	double req_net_delta;
		double short_existing_positions;
		double contracts;
		double new_net_rho;
		
	} * tab_swaps_trade_struct;
		ofstream debug;
		int current_future;
		void init_delta_bounds();
		void init_characteristics();
		void init_tab_swaps_trade_struct(int i);
		void init_time(int i);
		void init_hedging(int i);
		void init_tab(int i);
		void init_ex_rho(int i);
		void init_ex_rho_swaps(int i);
		void init_rho_asset(int i);
		void init_rho_liability(int i);
		void init_net_rho(int i);
	//	void init_req_delta_liability(int i);
	//	void init_req_net_delta(int i);
		void init_short_existing_positions(int i);
		void init_contracts(int i);
		void init_new_net_rho(int i);
		
		int swap_no;
	/*Swaps_Trade(Assumptions* assumptions, Rho_Asset* rho_asset, 
		Hedgeff* hedgeff,Existing_Rho_Swaps* existing_rho,
		Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio);*/
		Swaps_Trade(int swap_no,Assumptions* assumptions,Rho_Asset* rho_asset,
		Hedgeff* hedgeff);
	~Swaps_Trade(void);
};
#endif