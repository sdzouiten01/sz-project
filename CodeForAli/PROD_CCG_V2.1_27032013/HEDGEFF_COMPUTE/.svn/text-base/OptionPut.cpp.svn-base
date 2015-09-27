#include "OptionPut.h"
#include "MathTools.h"
#include "Assumptions.h"
#include "SwapRate.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace AXAHedgeEffective;

OptionPut::~OptionPut()
{
	if (_is_initialed)
	{
		delete [] _gamma_bef;
		delete [] _gamma_af;
		delete [] _gamma_net;

		delete [] _eq_put;
		delete [] _mkt_value;
		delete [] _opt_not;	
		delete [] _trading;
		delete [] _pl;
		delete [] _trans_cost;
	}
}
void OptionPut::init_underlying(int index_no, double* sj, SwapRate* r, Configuration* cfg)
{
	_index = index_no;
	_underlying = sj;

	_swap_rate = r;
	_config = cfg;	
	g_model_period = _config->g_model_period;
	g_length_of_option_contract = _config->g_length_of_option_contract;
	g_sell_at_time_to_expiry = _config->g_sell_at_time_to_expiry;
	g_nb_steps_per_year = _config->g_nb_steps_per_year;
	g_moneyness = _config->g_moneyness;
	g_volatility = _config->g_volatility;
	g_gamma_trans_cost = _config->g_gamma_trans_cost;
	g_shocks = _config->g_shocks;

	// allocating memories
	if ( !_is_initialed )
	{
		_gamma_bef = new double[ g_model_period ];
		_gamma_af = new double[ g_model_period ];
		_gamma_net = new double[ g_model_period ];

		_eq_put = new double [ g_model_period ];
		_mkt_value = new double[ g_model_period ];
		_opt_not = new double[ g_model_period ];	
		_trading = new double[ g_model_period ];
		_pl = new double[ g_model_period ];
		_trans_cost = new double[ g_model_period ];

		_nb_put.resize( g_model_period );
	}
	else
	{
		for (int i = 0; i < g_model_period; ++i)
		{
			_gamma_bef[i] = 0;
			_gamma_af[i] = 0;
			_gamma_net[i] = 0;

			_eq_put[i] = 0;
			_mkt_value[i] = 0;
			_opt_not[i] = 0;	
			_trading[i] = 0;
			_pl[i] = 0;
			_trans_cost[i] = 0;
			_nb_put[i].clear();
		}
	}

	_is_initialed = true;
}
double OptionPut::rf_put(int i, int j)
 {
	 if ( j > i)
	 {
		 return 0;
	 }
	 int tte_i = g_length_of_option_contract + j - i;
	 double tte;
	 if ( tte_i < g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		 return 0;
	 else
		 tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	 //rf
	 double ratio = (double)(tte_i%g_nb_steps_per_year)/g_nb_steps_per_year;
	 double rf = (1-ratio)*_swap_rate->_zero_coupon_rates[i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_rates[i][(int)(tte)+1];

	return rf;
 }
double OptionPut::rf_shock_up_put(int i, int j, int ma_id)
 {
	 if ( j > i)
	 {
		 return 0;
	 }
	 int tte_i = g_length_of_option_contract + j - i;
	 double tte;
	 if ( tte_i < g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		 return 0;
	 else
		 tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	 //rf
	 double ratio = (double)(tte_i%g_nb_steps_per_year)/g_nb_steps_per_year;
	 double rf = (1-ratio)*_swap_rate->_zero_coupon_shock_up_rates[ma_id][i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_shock_up_rates[ma_id][i][(int)(tte)+1];

	return rf;
 }
double OptionPut::rf_shock_down_put(int i, int j, int ma_id)
 {
	 if ( j > i)
	 {
		 return 0;
	 }
	 int tte_i = g_length_of_option_contract + j - i;
	 double tte;
	 if ( tte_i < g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		 return 0;
	 else
		 tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	 //rf
	 double ratio = (double)(tte_i%g_nb_steps_per_year)/g_nb_steps_per_year;
	 double rf = (1-ratio)*_swap_rate->_zero_coupon_shock_down_rates[ma_id][i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_shock_down_rates[ma_id][i][(int)(tte)+1];

	return rf;
 }
double OptionPut::gamma_put(int i, int j)
{
	if ( j > i )
		return 0;
	// from here, j <= i
	double underlying = _underlying[i];
	double und_base = _underlying[j] * g_moneyness;

	int sell_at_time_to_expiry = g_sell_at_time_to_expiry;
	int length_of_contract =  g_length_of_option_contract;
	int nb_steps_per_year = g_nb_steps_per_year;
	int tte_i = length_of_contract + j - i;
	double tte;
	if ( tte_i <= sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		return 0;
	else
		tte = (double)(tte_i)/((double)nb_steps_per_year);

	//rf
	double ratio = (double)(tte_i%nb_steps_per_year)/nb_steps_per_year;
	double rf = (1-ratio)*_swap_rate->_zero_coupon_rates[i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_rates[i][(int)(tte)+1];

	return gamma_BS(
				underlying, 
				und_base,
				tte,
				rf,
				0,
				g_volatility);
}
double OptionPut::delta_put(int i, int j)
{
	if( i < j )
		return 0;

	int tte_i = g_length_of_option_contract + j - i;	 
	double tte;
	if ( tte_i <= g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		return 0;
	else
		tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	double underlying = _underlying[i];
	double und_base = _underlying[j] * g_moneyness;
	//rf
	double ratio = (double)(tte_i%g_nb_steps_per_year)/g_nb_steps_per_year;
	double rf = (1-ratio)*_swap_rate->_zero_coupon_rates[i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_rates[i][(int)(tte)+1];
	
	return delta_BS(
			underlying,
			und_base,
			tte,
			rf,
			0,
			g_volatility
		);
}
double OptionPut::rho_put(int i, int j, int ma_idx)
{
	if ( j > i )
		return 0;
	// from here, j <= i
	double underlying = _underlying[i];
	double und_base = _underlying[j] * g_moneyness;

	int tte_i = g_length_of_option_contract + j - i;

	double tte;
	if ( tte_i <= g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		return 0;
	else
		tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	return (put_price_BS(
					underlying,
					und_base,
					tte,
					rf_shock_up_put(i, j, ma_idx),
					0,
					g_volatility)
					-
				put_price_BS(
					underlying,
					und_base,
					tte,
					rf_shock_down_put(i, j, ma_idx),
					0,
					g_volatility)) / (2*g_shocks[ma_idx]/100.0);
}
double OptionPut::rho_total(int i, int ma_id)
{
	if (_nb_put.size() == 0)
	{
		return 0;
	}
	double rho = 0;
	for (  int j = 0; j < _nb_put[i].size(); ++j )
	{
		rho += _nb_put[i][j]._amount * rho_put(i, _nb_put[i][j]._time, ma_id );
	}
	return rho;
}
double OptionPut::ov_base(int i, int j)
{
	if ( j > i )
		return 0;
	// from here, j <= i
	double underlying = _underlying[i];
	double und_base = _underlying[j] * g_moneyness;

	int tte_i = g_length_of_option_contract + j - i;
	double tte;
	if ( tte_i < g_sell_at_time_to_expiry )	// option put too early so that we do not hold at all
		return 0;
	else
		tte = (double)(tte_i)/((double)g_nb_steps_per_year);

	//rf
	double ratio = (double)(tte_i%g_nb_steps_per_year)/g_nb_steps_per_year;
	double rf = (1-ratio)*_swap_rate->_zero_coupon_rates[i][(int)(tte)] 
						+ ratio*_swap_rate->_zero_coupon_rates[i][(int)(tte)+1];

	return put_price_BS(
									underlying,
									und_base,
									tte,
									rf,
									0,
									g_volatility);
}
double OptionPut::trading(int i)
{
	double trading = 0;
	if ( i == 0 )
	{
		assert(_nb_put[0].size() <= 1);
		if ( _nb_put[0][0]._amount != 0 )
		{
			trading = _nb_put[0][0]._amount*ov_base(0, 0);
		}
		return trading;
	}

	for (  int j = 0; j < _nb_put[i].size(); ++j )
	{
		trading += _nb_put[i][j]._amount*ov_base(i, _nb_put[i][j]._time);
	}
	int preI = i-1;
	for (  int j = 0; j < _nb_put[preI].size(); ++j )
	{
		trading -= _nb_put[preI][j]._amount*ov_base(i, _nb_put[preI][j]._time);
	}
	return trading;
}
double OptionPut::market_value(int i)
{
	double val = 0;
	for (  int j = 0; j < _nb_put[i].size(); ++j )
	{
		val += _nb_put[i][j]._amount*ov_base(i, _nb_put[i][j]._time);
	}
	return val;
}
double OptionPut::init_gamma_bef(int i)
{
	double gamma_bef = 0;
	if ( i == 0)
	{
		_gamma_bef[0] = gamma_bef;
		return gamma_bef;
	}
	
	for (deque<TradeInfo>::iterator it_n = _nb_put[i-1].begin(); it_n != _nb_put[i-1].end(); ++it_n )
	{
		gamma_bef += gamma_put(i, it_n->_time) * it_n->_amount;
	}
	_gamma_bef[i] = gamma_bef;
	return gamma_bef;
}
double OptionPut::init_gamma_af(int i)
{
	double gamma_af = 0;
	for (deque<TradeInfo>::iterator it_n = _nb_put[i].begin(); it_n != _nb_put[i].end(); ++it_n )
	{
		gamma_af += gamma_put(i, it_n->_time) * it_n->_amount;
	}
	_gamma_af[i] = gamma_af;
	return gamma_af;
}
void OptionPut::buy_sell_new_put(int i, double gamma_liab)
{
	double amount;
	if ( i == 0 )
	{
		_gamma_af[0] = - gamma_liab;
		double tmp = gamma_put(0, 0);
		amount = -_gamma_net[0]/gamma_put(0, 0);
		_nb_put[i].push_back( TradeInfo(0, amount) );
		return;
	}

	_nb_put[i] = _nb_put[i-1];
	// check if the earlies future has been expired
	if ( _nb_put[i].size() != 0 && isExpired(i, _nb_put[i][0]._time ) )
	{
		_nb_put[i].pop_front();
	}

	double tmp = gamma_put(i, i);
	amount = -_gamma_net[i]/tmp;
	_nb_put[i].push_back( TradeInfo(i, amount) );
}
void OptionPut::buy_sell_exist_put(int i, double gamma_liab)
{
	// (delta_net > 0 && delta_port > 0)  || (delta_net < 0 && delta_port < 0)
	assert( i >= 1);
	double gamma_rest = _gamma_net[i];
	double gamma_port = _gamma_bef[i];
	double cur_gamma_port;
	double amount;

	if ( abs(gamma_rest) >=  abs(gamma_port) )
	{
		_nb_put[i].clear();
		//assert( _nb_put[i].size() == 0 );
		amount = -(gamma_rest-gamma_port)/gamma_put(i, i);
		_nb_put[i].push_back( TradeInfo(i, amount) );
		return;
	}

	// abs(gamma_rest) <  abs(gamma_port)
	_nb_put[i] = _nb_put[i-1];
	// check if the earlies future has been expired
	if ( _nb_put[i].size() != 0 && isExpired(i, _nb_put[i][0]._time ) )
	{
		_nb_put[i].pop_front();
	}
	int del_num = 0;
	double gamma_put_i_t;
	for (deque<TradeInfo>::iterator it_n = _nb_put[i].begin(); it_n != _nb_put[i].end(); ++it_n)
	{
		gamma_put_i_t = gamma_put(i, it_n->_time);
		cur_gamma_port = gamma_put_i_t * it_n->_amount;
		if ( abs(gamma_rest) >=  abs(cur_gamma_port) )
		{
			gamma_rest -= cur_gamma_port;
			++del_num;
		}
		else // abs(gamma_rest) <  abs(cur_gamma_port)
		{
			it_n->_amount -= (gamma_rest/gamma_put_i_t);		//iter_n->amount must not be zero, because in that case, gamma_rest =  cur_gamma_port!
			break;
		}
	}
	if (del_num != 0)
	{
		_nb_put[i].erase(_nb_put[i].begin(), _nb_put[i].begin()+del_num);
	}
}
// This function calcul total delta of the option_put we have in our portfolio
//  Output: _delta[i]  = total delta of puts at time i after gamma hedge
void OptionPut::init_delta()
{
	if ( _nb_put.size() == 0 )
	{
		_delta.assign(g_model_period, 0);
		return;
	}

	_delta.resize( g_model_period );
	double delta = 0;
	for (  int i = 0; i < g_model_period; ++i )
	{
		delta = 0;
		for (  int j = 0; j < _nb_put[i].size(); ++j )
		{
			delta += _nb_put[i][j]._amount * delta_put(i, _nb_put[i][j]._time);
		}
		_delta[i] = delta;
	}
}
void OptionPut::analyse()
{
	double underlying;
	_total_pl = _total_trans_cost = 0;

	for ( int i = 0; i < g_model_period;  ++i)
	{
		_eq_put[i] = 0;
		for (  int j = 0; j < _nb_put[i].size(); ++j )
		{
			_eq_put[i] += _nb_put[i][j]._amount;
		}

		underlying = _underlying[i];
		_opt_not[i] = _eq_put[i]*underlying;

		_mkt_value[i] = market_value(i);
		_trading[i] = trading(i);

		if ( i > 0 )
		{
			// mkt_value[i] - mkt_value[i-1] - purchase[i] + sales[i]
			_pl[i] = _mkt_value[i] - _mkt_value[i-1] - _trading[i];

			_trans_cost[i] = _eq_put[i] - _eq_put[i-1];
			if ( _nb_put[i-1].size() != 0 && isExpired(i, _nb_put[i-1][0]._time) )
			{
				_trans_cost[i] += _nb_put[i-1][0]._amount;
			}
			_trans_cost[i] = abs(_trans_cost[i])*underlying*g_gamma_trans_cost/10000;
		}
		else
		{
			_pl[0] = _mkt_value[0] - _trading[0];
			_trans_cost[0] = abs(_eq_put[0])*underlying*g_gamma_trans_cost/10000;
		}
		_total_pl += _pl[i];
		_total_trans_cost += _trans_cost[i];
	}
}
void OptionPut::dumpInfo()
{
	string file = "../04_for_debug/gamma_put_mine_";
	file.append(string(_index, 'A'));
	file.append(".csv");
	ofstream fout(file.c_str());
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << gamma_put(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 file = "../04_for_debug/delta_put_mine_";
	file.append(string(_index, 'A'));
	file.append(".csv");
	fout.open(file.c_str());
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << delta_put(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 file = "../04_for_debug/ov_base_mine_";
	 file.append(string(_index, 'A'));
	 file.append(".csv");
	 fout.open(file.c_str());
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << ov_base(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.close();

	 fout.open("../04_for_debug/rf_put_mine.csv");
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << rf_put(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.close();
}
void OptionPut::dumpPutInfo()
{
	char buf[4];
	_ltoa_s(_index, buf, 4, 10);
	string s_no(buf);

	string file = "../04_for_debug/nbr_put_"; 
	file.append(s_no);
	 file.append(".csv");
	 ofstream fout(file.c_str());

	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < _nb_put[i][0]._time; ++j)
		 {
			 fout << "0,";
		 }
		 fout << _nb_put[i][0]._amount << ",";
		 for ( int j = 1; j < _nb_put[i].size(); ++j)
		 {
			 for (  int k = _nb_put[i][j-1]._time+1; k < _nb_put[i][j]._time; ++k )
			 {
				 fout << "0,";
			 }
			 fout << _nb_put[i][j]._amount << ",";
		 }
		 fout << endl;
	 }
	 fout.close();
}