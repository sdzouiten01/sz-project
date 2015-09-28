#pragma once
#ifndef ASSETPURCHASE_H
#define ASSETPURCHASE_H
class Assumptions;
class Gamma_Asset;
class Hedgeff;
class Existing_Gamma;
class Gamma_Puts_In_Portfolio;
class Gamma_Puts_In_Portfolio_2;
/**
* Intermediary Class to used by  <CODE> Nbr_Puts <\CODE> to determine the number of new option to sale/purchase
* @see Nbr_Puts
*/
class Option_Trade
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Gamma_Asset* gamma_asset;
		Hedgeff* hedgeff;
		Existing_Gamma* existing_gamma;
		Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio;
		Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2;
	}objects_needed;
	struct Gamma_Bounds
	{
		double net_gamma_lower_bound;
		double net_gamma_upper_bound;
		double liab_gamma_lower_bound;
		double liab_gamma_upper_bound;
	} gamma_bounds;
	//struct Characteristics
	//{
	//	//struct Gamma_Bounds gamma_bounds;	
	//	//double shock_size;
	//	double length_of_option_contract;
	//	double sell_at_time_to_expiry;
	//} characteristics;
	typedef enum {Gal,Out,Ex_Gam_Neg,Ex_Gam_Pos,Nb_Cols_In_Tab} Tab_Cols;
	typedef enum {Sup_Up_Bound,In_Tol_Area,Inf_Low_Bound,Nb_Rows_In_Tab} Tab_Rows;
	struct Option_Trade_Struct
	{
		int time;/**< Time step considered*/
		bool hedging;/**< TRUE if and only if the user wants to hedge gamma*/
		//int ** tab;/**< Array to test value of gamma for liability and asset to take decisions*/
		double ex_gamma;/**< Value of the existing gamma at tiem considered*/
		double gamma_put;/**< Gamma of  put at time considered*/
		double gamma_liability;/**< Gamma of liability at time considered*/
		double net_gamma;/**< Net Gamma value */
	//	double req_gamma_liability;
	//	double req_net_gamma;
		double short_existing_positions;/**< =TRUE if and only if have to reduce positions (or increase)=> no new asset engaged*/
		double contracts;/** number of new contracts to sell/buy */
		double new_net_gamma;/** new net gamma after closing some positions*/
	} * tab_option_trade_struct;
	
		void init_gamma_bounds();
		void init_characteristics();
		void init_tab_asset_purchase_struct(int i);
		void init_time(int i);
		void init_hedging(int i);
	//	void init_tab(int i);
		void init_ex_gamma(int i);
		void init_gamma_put(int i);
		void init_gamma_liability(int i);
		void init_net_gamma(int i);
	//	void init_req_gamma_liability(int i);
	//	void init_req_net_gamma(int i);
		void init_short_existing_positions(int i);
		void init_contracts(int i);
		void init_new_net_gamma(int i);
		
	/*Option_Trade(Assumptions* assumptions, Gamma_Asset* gamma_asset, 
		Hedgeff* hedgeff,Existing_Gamma* existing_gamma,
		Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio);*/
		Option_Trade(Assumptions* assumptions,Gamma_Asset* gamma_asset,
		Hedgeff* hedgeff);
	~Option_Trade(void);
};
#endif