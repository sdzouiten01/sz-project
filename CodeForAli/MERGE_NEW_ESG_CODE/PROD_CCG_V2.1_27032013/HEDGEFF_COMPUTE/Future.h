/******************************************************************************
* Class Future
*		- This class represent 
*		- It does all the  hedges: Gamma, Delta, Rho, Rho-convexity
*
* 27-05-2010
******************************************************************************/
#pragma once
#include <vector>
#include <deque>
#include "TradeInfo.h"
#include "Assumptions.h"

class SwapRate;
class Future{
	friend class Assets;
	friend class Portfolio;
public:
	Future():_is_initialed(false){}
	~Future();
	void init_underlying(int index_no, double* sj, SwapRate* r, Configuration* cfg);

	void buy_sell_new_future(int i, double delta_liab);
	void buy_sell_exist_future(int i);

	double delta_future(int i, int j);
	double rho_future(int i, int j, int maturity_index);
	double rho_total(int i, int maturity_index);
	double fv_base(int i, int j);
	double trading(int i);
	double market_value(int i);

	double init_delta_bef(int i);				// do not count in delta of put existant!!
	double init_delta_af(int i);

	void analyse();
	void dumpInfo();
	void dumpPurchaseInfo();

	double tte_future(int i, int j);
	double rf_future(int i, int j);
	double rf_shock_up_future(int i, int j, int ma_id);
	double rf_shock_down_future(int i, int j, int ma_id);
private:
	SwapRate* _swap_rate;
	double*	_delta_bef;		// delta of future at time i before delta hedgeing
	double*	_delta_af;			// delta of futre at time i after delta hedging
	double*	_delta_net;		// delta of futures and corresponding put at time i

	std::vector<std::vector<double> > _rhos_future;
	std::vector<std::deque<TradeInfo> >	_nb_future;

	double*		_underlying;

	double*		_eq_future;
	double*		_mkt_value;
	double*		_opt_not;							// option notional
	double*		_trading;
	double*		_pl;
	double*		_trans_cost;
	double			_total_pl, _total_trans_cost;

	bool			_is_initialed;
	int			_index;															// index_no of underling

	//
	Configuration* _config;
	int g_model_period;
	int g_length_of_future_contract;
	int g_nb_steps_per_year;
	double g_delta_trans_cost;
	bool g_sheduled_futures;
	double* g_shocks;
};