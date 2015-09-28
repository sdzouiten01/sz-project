/******************************************************************************
* Class OptionPut
*
* 24-05-2010
******************************************************************************/
#pragma once
#include <vector>
#include <deque>
#include "TradeInfo.h"
#include "Assumptions.h"

class SwapRate;
class OptionPut{
	friend class Assets;
	friend class Portfolio;
public:
	OptionPut():_is_initialed(false){}
	~OptionPut();
	void init_underlying(int index_no, double* sj, SwapRate* r, Configuration* cfg);

	void buy_sell_new_put(int i, double gamma_liab);
	void buy_sell_exist_put(int i, double gamma_liab);
	void analyse();

	double gamma_put(int i, int j);
	double delta_put(int i, int j);
	double rho_put(int i, int j, int maturity_index);
	double rho_total(int i, int maturity_index);			// total rho of the puts at time i

	double ov_base(int i, int j);
	double trading(int i);
	double market_value(int i);

	double init_gamma_bef(int i);
	double init_gamma_af(int i);

	void init_delta();							// calcul the total delta of option_puts we hold at each time i
	inline bool isExpired(int i, int j)
	{
		return 	 ( (_config->g_length_of_option_contract + j - i) < _config->g_sell_at_time_to_expiry);
	}
	double rf_put(int i, int j);
	double rf_shock_up_put(int i, int j, int ma_id);
	double rf_shock_down_put(int i, int j, int ma_id);

	void dumpInfo();
	void dumpPutInfo();
private:
	SwapRate* _swap_rate;
	Configuration* _config;

	std::vector<double>	_delta;
	double*	_gamma_bef;
	double*	_gamma_af;
	double*	_gamma_net;

	std::vector<std::deque<TradeInfo> >	_nb_put;

	double*		_underlying;

	double*		_eq_put;
	double*		_mkt_value;
	double*		_opt_not;							// option notional
	double*		_trading;
	double*		_pl;
	double*		_trans_cost;
	double			_total_pl, _total_trans_cost;

	bool			_is_initialed;
	int			_index;															// index_no of underling

	// Configuration
	int g_model_period;
	int g_length_of_option_contract;
	int g_sell_at_time_to_expiry;
	int g_nb_steps_per_year;
	double g_moneyness;
	double g_volatility;
	double g_gamma_trans_cost;
	double* g_shocks;
};
