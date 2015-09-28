/******************************************************************************
* Class Swap
*		- This class contains  swap
*
* 1-05-2010
******************************************************************************/
#pragma once
#include "BandMatrix.h"
#include "Assumptions.h"
#include "TradeInfo.h"
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>

using std::vector;
class SwapRate;
class Swap{
	friend class Portfolio;
public:
	Swap():_is_initialed(false){}

	void initial(int ma_id, SwapRate* r, Configuration* cfg);
	~Swap();
	void init_sv_bases();
	void init_rho_swaps();
	double trading( int i);
	double market_value( int i);

	void buy_sell_new_swap(int i, double rho_net);	// decide how much swap we should buy/sell at time i, in order to neutralise rho_net;
	void realise_exist_swap(int i);										// realised all the swap existed when Rho/AV < seuil
	void init_rho_bef(int i, double& rho_bef);				// calcule rho of this swap (maturity[ma_id]) before rho-hedge at time i
	void init_rho_af(int i);

	double tte_swap(int i, int j);
	void dumpInfo();
	void dumpPurchaseInfo();
private:
	SwapRate* _swap_rate;
	Configuration* _config;

	vector<std::deque<TradeInfo> >		_nb_swap; // stored the trade time & notional

	vector<double>		_rho_bef;
	vector<double>		_rho_af;
	vector<double>		_rho_net;
	vector<double>		_coupon;

	int						_ma_id;						// maturity index
	int						_swap_maturity;
	bool					_is_initialed;

	BandMatrix				_sv_bases; // swaps (RECEIVER) values
	BandMatrix				_coupons;  // coupon correspondent
	std::vector<BandMatrix>	_rho_swaps;// rho swaps correspondent


	// added 10/01/2011 : using to distinguish payer / receiver swaps
	// when taking into account the transaction cost directly in the fixed rate of swaps
	//-------------------------------------------------------
	// "TEMPORARY SOLUTION", may need to review after that!!!
	//-------------------------------------------------------
	//bool isReceiverSwap;
	BandMatrix		        _sv_bases_4payer; // swaps (PAYER) values
	BandMatrix		        _coupons_4payer;  // coupon correspondent
	std::vector<BandMatrix>	_rho_swaps_4payer;// rho swaps correspondent


	double*		_eq_swap;
	double*		_mkt_value;
	double*		_trading;
	double*		_pl_unrealized, *_pl_realized;
	//
	double*		_trans_cost;
	//double		_total_pl, _total_trans_cost;

	// Configuration
	int g_model_period;
	int g_length_of_future_contract;
	int g_nb_steps_per_year;
};