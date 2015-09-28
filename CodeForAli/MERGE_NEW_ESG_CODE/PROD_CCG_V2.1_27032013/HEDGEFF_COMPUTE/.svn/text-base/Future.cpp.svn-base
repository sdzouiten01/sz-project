#include "Future.h"
#include "MathTools.h"
#include "Assumptions.h"
#include "SwapRate.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace AXAHedgeEffective;

Future::~Future()
{
	if (_is_initialed)
	{
		delete [] _delta_bef;
		delete [] _delta_af;
		delete [] _delta_net;
		delete [] _eq_future;
		delete [] _mkt_value;
		delete [] _opt_not;
		delete [] _trading;
		delete [] _pl;
		delete [] _trans_cost;
	}
}
double Future::tte_future(int i, int j)
 {
	 if ( i < j )
		 return 0;

	 if (g_sheduled_futures)
	 {	 
		 int maturity = (j/g_length_of_future_contract+1)*g_length_of_future_contract;
		if ( i >= maturity )
		{
			return 0;
		}
		return (double)(maturity-i)/(double)g_nb_steps_per_year;
	 }
	 else
	 {
		 int tte_i = g_length_of_future_contract + j - i;
		 if ( tte_i <= 0 )
		 {
			 return 0;
		 }
		 return (double)tte_i/(double)g_nb_steps_per_year;
	 }
 }
double Future::rf_future(int i, int j)
{
	// if previous tte is null, rf = 0;
	if ( i >= 1 && i > j && tte_future(i-1, j) == 0 )
	{
		return 0;
	}
	if ( i < j )
		return 0;

	double tte = tte_future(i,j);
	int tte_i = (int)tte;
	double ratio = tte - tte_i;
	 
	return  (1-ratio)*_swap_rate->_zero_coupon_rates[i][tte_i] + ratio*_swap_rate->_zero_coupon_rates[i][tte_i+1];
}
double Future::rf_shock_up_future(int i, int j, int ma_id)
{
	// if previous tte is null, rf = 0;
	if ( i >= 1 && i > j && tte_future(i-1, j) == 0 )
	{
		return 0;
	}
	if ( i < j )
		return 0;

	double tte = tte_future(i,j);
	int tte_i = (int)tte;
	double ratio = tte - tte_i;
	 
	return  (1-ratio)*_swap_rate->_zero_coupon_shock_up_rates[ma_id][i][tte_i] + ratio*_swap_rate->_zero_coupon_shock_up_rates[ma_id][i][tte_i+1];
}
double Future::rf_shock_down_future(int i, int j, int ma_id)
{
	// if previous tte is null, rf = 0;
	if ( i >= 1 && i > j && tte_future(i-1, j) == 0 )
	{
		return 0;
	}
	if ( i < j )
		return 0;

	double tte = tte_future(i,j);
	int tte_i = (int)tte;
	double ratio = tte - tte_i;
	 
	return  (1-ratio)*_swap_rate->_zero_coupon_shock_down_rates[ma_id][i][tte_i] + ratio*_swap_rate->_zero_coupon_shock_down_rates[ma_id][i][tte_i+1];
}
void Future::init_underlying(int index_no, double* sj, SwapRate* r, Configuration* cfg)
{
	_swap_rate = r;
	_index = index_no;
	_underlying = sj;
	_config = cfg;
	g_model_period = _config->g_model_period;
	g_length_of_future_contract = _config->g_length_of_future_contract;
	g_nb_steps_per_year = _config->g_nb_steps_per_year;
	g_sheduled_futures = _config->g_sheduled_futures;
	g_delta_trans_cost = _config->g_delta_trans_cost;
	g_shocks = _config->g_shocks;

	// allocating memories
	if (!_is_initialed)
	{
		_delta_bef = new double[g_model_period];
		_delta_af = new double[g_model_period];
		_delta_net = new double[g_model_period];
		_nb_future.resize(g_model_period);

		_eq_future = new double[g_model_period];
		_mkt_value = new double[g_model_period];
		_opt_not = new double[g_model_period];	
		_trading = new double[g_model_period];
		_pl = new double[g_model_period];
		_trans_cost = new double[g_model_period];
	}
	else
	{
		for (int i = 0; i < g_model_period; ++i)
		{
			_delta_bef[i] = 0;
			_delta_af[i] = 0;
			_delta_net[i] = 0;
			_nb_future[i].clear();

			_eq_future[i] = 0;
			_mkt_value[i] = 0;
			_opt_not[i] = 0;	
			_trading[i] = 0;
			_pl[i] = 0;
			_trans_cost[i] = 0;
		}
	}

	_is_initialed = true;
}
double Future::delta_future(int i, int j)
{
	if ( j > i )
		return 0;
	
	// Maturity is reached
	if( tte_future(i, j) ==0 )   //&& i>=j)
		return 0;

	return exp( rf_future(i, j)*tte_future(i, j));
}
double Future::rho_future(int i, int j, int ma_id)
{
	if ( j > i )
		return 0;
	
	// Maturity is reached
	if( tte_future(i, j) ==0 )   //&& i>=j)
		return 0;

	double underlying = _underlying[i];

	return ( underlying*exp(rf_shock_up_future(i, j, ma_id)*tte_future(i,j))
					- 
				 underlying*exp(rf_shock_down_future(i, j, ma_id)*tte_future(i, j))
				)/(2* g_shocks[ma_id]/100.0);
}
double Future::rho_total(int i, int ma_id)
{
	double rho = 0;
	for (  int j = 0; j < _nb_future[i].size(); ++j )
	{
		rho += _nb_future[i][j]._amount * rho_future(i, _nb_future[i][j]._time, ma_id );
	}
	return rho;
}
double Future::fv_base(int i, int j)
{
	if( j > i || i > g_length_of_future_contract + j )			// j > i => underlying = 0
		return 0;
	else
	{
		double underlying = _underlying[i];
		return underlying*exp(rf_future(i, j)*tte_future(i, j));
	}
}
double Future::trading(int i)
{
	double trading = 0;
	if ( i == 0 )
	{
		assert(_nb_future[0].size() <= 1);
		if ( _nb_future[0][0]._amount != 0 )
		{
			trading = _nb_future[0][0]._amount*fv_base(0, 0);
		}
		return trading;
	}

	for (  int j = 0; j < _nb_future[i].size(); ++j )
	{
		trading += _nb_future[i][j]._amount*fv_base(i, _nb_future[i][j]._time);
	}
	int preI = i-1;
	for (  int j = 0; j < _nb_future[preI].size(); ++j )
	{
		trading -= _nb_future[preI][j]._amount*fv_base(i, _nb_future[preI][j]._time);
	}
	return trading;
}
double Future::market_value(int i)
{
	double val = 0;
	for (  int j = 0; j < _nb_future[i].size(); ++j )
	{
		val += _nb_future[i][j]._amount*fv_base(i, _nb_future[i][j]._time);
	}
	return val;
}
double Future::init_delta_bef(int i)
{
	double delta_bef = 0;
	if ( i == 0)
	{
		_delta_bef[0] = delta_bef;
		return delta_bef;
	}

	for (deque<TradeInfo>::iterator it_n = _nb_future[i-1].begin(); it_n != _nb_future[i-1].end(); ++it_n )
	{
		delta_bef += delta_future(i, it_n->_time) * it_n->_amount;
	}
	_delta_bef[i] = delta_bef;
	return delta_bef;
}
double Future::init_delta_af(int i)
{
	double delta_af = 0;
	for (deque<TradeInfo>::iterator it_n = _nb_future[i].begin(); it_n != _nb_future[i].end(); ++it_n )
	{
		delta_af += delta_future(i, it_n->_time) * it_n->_amount;
	}
	_delta_af[i] = delta_af;
	return delta_af;
}
void Future::buy_sell_new_future(int i, double delta_liab)
{
	double amount;
	if ( i == 0 )
	{
		_delta_af[0] = - delta_liab;
		double tmp = delta_future(0, 0);
		amount = -_delta_net[0]/delta_future(0, 0);
		_nb_future[i].push_back( TradeInfo(0, amount) );
		return;
	}

	_nb_future[i] = _nb_future[i-1];
	// check if the earlies future has been expired
	if ( _nb_future[i].size() != 0 && tte_future(i, _nb_future[i][0]._time ) == 0)
	{
		_nb_future[i].pop_front();
	}
	double tmp = delta_future(i, i);
	amount = -_delta_net[i]/tmp;
	_nb_future[i].push_back( TradeInfo(i, amount) );
}
void Future::buy_sell_exist_future(int i)
{
	assert( i >= 1);
	double delta_rest = _delta_net[i];
	double delta_fut = _delta_bef[i];
	double cur_delta_port;
	double amount;

	if ( abs(delta_rest) >=  abs(delta_fut) )
	{
		// assert( _nb_future[i].size() == 0 );
		_nb_future[i].clear();
		amount = -(delta_rest-delta_fut)/delta_future(i, i);
		_nb_future[i].push_back( TradeInfo(i, amount) );
		return;
	}

	// abs(delta_rest) <  abs(delta_port)
	_nb_future[i] = _nb_future[i-1];
	// check if the earlies future has been expired
	if ( _nb_future[i].size() != 0 && tte_future(i, _nb_future[i][0]._time ) == 0)
	{
		_nb_future[i].pop_front();
	}

	int del_num = 0;
	double delta_future_i_t;
	for (deque<TradeInfo>::iterator it_n = _nb_future[i].begin(); it_n != _nb_future[i].end(); ++it_n)
	{
		delta_future_i_t = delta_future(i, it_n->_time);
		cur_delta_port = delta_future_i_t * it_n->_amount;
		if ( abs(delta_rest) >=  abs(cur_delta_port) )
		{
			delta_rest -= cur_delta_port;
			++del_num;
		}
		else // abs(delta_rest) <  abs(cur_delta_port)
		{
			it_n->_amount -= (delta_rest/delta_future_i_t);		//iter_n->amount must not be zero, because in that case, delta_rest =  cur_delta_port!
			break;
		}
	}
	if (del_num != 0)
	{
		_nb_future[i].erase(_nb_future[i].begin(), _nb_future[i].begin()+del_num);
	}
}
void Future::analyse()
{
	_total_pl = 0;
	_total_trans_cost = 0;
	double underlying;
	for ( int i = 0; i < g_model_period;  ++i)
	{
		_eq_future[i] = 0;
		for (  int j = 0; j < _nb_future[i].size(); ++j )
		{
			_eq_future[i] += _nb_future[i][j]._amount;
		}

		underlying = _underlying[i];
		_opt_not[i] = _eq_future[i]*underlying;

		_mkt_value[i] = market_value(i);
		_trading[i] = trading(i);
		if ( i > 0 )
		{
			// mkt_value[i] - mkt_value[i-1] - purchase[i] + sales[i]
			_pl[i] = _mkt_value[i] - _mkt_value[i-1] - _trading[i];

			_trans_cost[i] = _eq_future[i] - _eq_future[i-1];
			if ( _nb_future[i-1].size() != 0 && tte_future(i, _nb_future[i-1][0]._time) == 0 )
			{
				_trans_cost[i] += _nb_future[i-1][0]._amount;
			}
			_trans_cost[i] = abs(_trans_cost[i])*underlying*g_delta_trans_cost/10000;
		}
		else
		{
			_pl[0] = _mkt_value[0] - _trading[0];
			_trans_cost[0] = abs(_eq_future[0])*underlying*g_delta_trans_cost/10000;
		}
		_total_pl += _pl[i];
		_total_trans_cost += _trans_cost[i];
	}
}
void Future::dumpInfo()
{
	string s_id;
	char buf[4];
	_ltoa_s(_index, buf, 4, 10);
	s_id.append(buf);
	s_id.append("-");

	string file = "../04_for_debug/delta_future_mine_";
	file.append(s_id);
	file.append(".csv");
	ofstream fout(file.c_str());
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << delta_future(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 return;
	 file = "../04_for_debug/fv_base_mine_";
	 file.append(s_id);
	 file.append(".csv");
	 fout.open(file.c_str());

	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << fv_base(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.close();

	 file = "../04_for_debug/rf_future_mine_";
	 file.append(s_id);
	 file.append(".csv");
	 fout.open(file.c_str());

	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_model_period; ++j)
		 {
			 fout << rf_future(i, j) << ",";
		 }
		 fout << endl;
	 }
	 fout.close();
}
void Future::dumpPurchaseInfo()
{
	string file = "../04_for_debug/nbr_future_mine_";
	file.append(string(_index-1, 'A'));
	file.append(".csv");
	ofstream fout(file.c_str());
	for ( int i = 0; i < g_model_period; ++i)
	{
		for ( int j = 0; j < _nb_future[i][0]._time; ++j)
		{
			fout << "0,";
		}
		fout << _nb_future[i][0]._amount << ",";
		for ( int j = 1; j < _nb_future[i].size(); ++j)
		{
			for ( int k = _nb_future[i][j-1]._time+1; k < _nb_future[i][j]._time; ++k )
			{
				fout << "0,";
			}
			fout << _nb_future[i][j]._amount << ",";
		}
		fout << endl;
	}
	fout.close();
}