#pragma once
#ifndef FUTURES_PURCHASE_H
#define FUTURES_PURCHASE_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;
class Assumptions;
class Delta_Asset;
class Hedgeff;
class Existing_Delta_Futures;
class Existing_Delta;
class Delta_Futures_In_Portfolio;
class Delta_Futures_In_Portfolio_2;
/**
* Intermediary Class to used by  <CODE> Nbr_Puts <\CODE> to determine the number of new futures to sale/purchase
* @see Nbr_Futures
*/
class Future_Trade
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Delta_Asset* delta_asset;
		Hedgeff* hedgeff;
		Existing_Delta_Futures* existing_delta_futures;
		Existing_Delta* existing_delta;
		Delta_Futures_In_Portfolio* delta_futures_in_portfolio;
		Delta_Futures_In_Portfolio_2* delta_futures_in_portfolio_2;
	}objects_needed;
	struct Delta_Bounds
	{
		double net_delta_lower_bound;
		double net_delta_upper_bound;
		double liab_delta_lower_bound;
		double liab_delta_upper_bound;
	} delta_bounds;
	struct Characteristics
	{
		//struct Delta_Bounds delta_bounds;	
		//double shock_size;
		double length_of_futures_contract;
		double sell_at_time_to_expiry;
	} characteristics;
	typedef enum {Ex_Delta_Fut_Pos,Ex_Delta_Fut_Neg,Nb_Cols_In_Tab} Tab_Cols;
	typedef enum {Net_Delta_Pos,Net_Delta_Neg,Nb_Rows_In_Tab} Tab_Rows;
	struct Futures_Purchase_Struct
	{
		int time;
		bool hedging;
		int ** tab;
		double ex_delta;
		double ex_delta_futures;
		double delta_asset;
		double delta_liability;
		double net_delta;
	//	double req_delta_liability;
	//	double req_net_delta;
		double short_existing_positions;
		double contracts;
		double new_net_delta;
		
	} * tab_futures_purchase_struct;
		ofstream debug;
		int current_future;
		void init_delta_bounds();
		void init_characteristics();
		void init_tab_futures_purchase_struct(int i);
		void init_time(int i);
		void init_hedging(int i);
		void init_tab(int i);
		void init_ex_delta(int i);
		void init_ex_delta_futures(int i);
		void init_delta_asset(int i);
		void init_delta_liability(int i);
		void init_net_delta(int i);
	//	void init_req_delta_liability(int i);
	//	void init_req_net_delta(int i);
		void init_short_existing_positions(int i);
		void init_contracts(int i);
		void init_new_net_delta(int i);
		
	/*Future_Trade(Assumptions* assumptions, Delta_Asset* delta_asset, 
		Hedgeff* hedgeff,Existing_Delta_Futures* existing_delta,
		Delta_Futures_In_Portfolio* delta_futures_in_portfolio);*/
		Future_Trade(Assumptions* assumptions,Delta_Asset* delta_asset,
		Hedgeff* hedgeff);
	~Future_Trade(void);
};
#endif