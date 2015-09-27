#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include "Swap.h"
#include "MathTools.h"
#include "SwapRate.h"
#include "Assumptions.h"
using namespace std;
using namespace AXAHedgeEffective;


void Swap::initial(int ma_id, SwapRate* r, Configuration* cfg)
{
	_config = cfg;
	_swap_rate = r;
	_ma_id = ma_id;

	g_model_period = _config->g_model_period;
	g_length_of_future_contract = _config->g_length_of_future_contract;
	g_nb_steps_per_year = _config->g_nb_steps_per_year;
	_swap_maturity = _config->g_lbs_maturities[_ma_id]*g_nb_steps_per_year;
	// allocating memories
	if ( !_is_initialed )
	{
		_rho_bef.resize(g_model_period);
		_rho_af.resize(g_model_period);
		_rho_net.resize(g_model_period);
		_nb_swap.resize(g_model_period);
		_coupon.resize(g_model_period);

		_eq_swap = new double[g_model_period];
		_mkt_value = new double[g_model_period];
		_trading = new double[g_model_period];
		_pl_unrealized = new double[ g_model_period ];
		_pl_realized = new double[ g_model_period ];
		_trans_cost = new double[ g_model_period ];
	}
	else
	{
		for (int i = 0; i < g_model_period; ++i)
		{
			_rho_bef[i] = 0;
			_rho_af[i] = 0;
			_rho_net[i] = 0;
			_coupon[i] = 0;

			_eq_swap[i] = 0;
			_mkt_value[i] = 0;
			_trading[i] = 0;
			_pl_unrealized[i] = 0;
			_pl_realized[i] = 0;
			_trans_cost[i] = 0;
		}
		_rho_swaps.clear();
		_rho_swaps_4payer.clear();
		_nb_swap.resize(g_model_period);
	}

	_is_initialed = true;
	init_sv_bases();
}
Swap::~Swap()
{
	if ( _is_initialed )
	{
		// release memories here using delete
		delete [] _eq_swap;
		delete [] _mkt_value;
		delete [] _trading;
		delete [] _pl_unrealized;
		delete [] _pl_realized;
		delete [] _trans_cost;
	}
}

double Swap::tte_swap(int i, int j)
{
	 if ( i < j )
		 return 0;

	 if (_config->g_sheduled_futures)
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
		 int tte_i = _swap_maturity + j - i;
		 if ( tte_i <= 0 )
		 {
			 return 0;
		 }
		 return (double)tte_i/(double)g_nb_steps_per_year;
	 }
}

// Modified 10/01/2011
// taking into account difference between RECEIVER / PAYER swaps
void Swap::init_sv_bases()
{
	int* g_compounding_frequencies = _config->g_compounding_frequencies;

	double fix_rate = 0.0, fix_rate_4payer = 0.0; // added 14/01/2011
	double coupon, coupon_4payer;
	double sv, sv_4payer;

	g_math_swap_price_spread = _config->g_swap_spread[_ma_id];
	
	// for RECEIVER Swaps
	_sv_bases.reset();
	_sv_bases.init(g_model_period,  _swap_maturity + 1);
	_coupons.init(g_model_period, g_model_period);

	//-----------------
	// added 10/01/2011
	// for PAYER Swaps
	_sv_bases_4payer.reset();
	_sv_bases_4payer.init(g_model_period,  _swap_maturity + 1);
	_coupons_4payer.init(g_model_period, g_model_period);
	
	for(int j=0; j < g_model_period;++j)
	{
		g_math_swap_price_trans_cost = &_trans_cost[j];
		for(int i = j; i <= min(j + _swap_maturity, g_model_period-1); ++i)
		{
			{
				// isCalcSwapRate = true => we can keep them in the same loop
				// for RECEIVER => by default
				sv = swap_price( _swap_rate->_zero_coupon_rates, j, j+_swap_maturity, i, fix_rate, g_compounding_frequencies[_ma_id], g_nb_steps_per_year, coupon, true);

				_coupons.set_val(i, j, coupon);
				_sv_bases.set_val(i, j, sv);

				// modified 14/01/2011
				// for PAYER Swaps => isReceiver = false
				sv_4payer = swap_price( _swap_rate->_zero_coupon_rates, j, j+_swap_maturity, i, fix_rate_4payer, g_compounding_frequencies[_ma_id], g_nb_steps_per_year, coupon_4payer, true, false, 0, 0, false);

				_coupons_4payer.set_val(i, j, coupon_4payer);
				_sv_bases_4payer.set_val(i, j, sv_4payer);
			}
		}
	}
}

// Modified 10/01/2011
// taking into account difference between RECEIVER / PAYER swaps
void Swap::init_rho_swaps()
{
	int* g_compounding_frequencies = _config->g_compounding_frequencies;
	double* g_shocks = _config->g_shocks;
	
	g_math_swap_price_spread = _config->g_swap_spread[_ma_id]; // added 10/01/2011
	
	// for RECEIVER Swaps
	_rho_swaps.clear();
	_rho_swaps.resize(_ma_id+1);

	// for PAYER Swaps
	_rho_swaps_4payer.clear();
	_rho_swaps_4payer.resize(_ma_id+1);

	for ( int i_ma = 0; i_ma <= _ma_id; ++i_ma )
	{
		_rho_swaps[i_ma].init(g_model_period, _swap_maturity);
		_rho_swaps_4payer[i_ma].init(g_model_period, _swap_maturity);

		double fix_rate = 0.0, fix_rate_4payer = 0.0;
		double zero_coupon_prec = 0.0;
		double zero_coupon_next = 0.0;
		
		double rho, rho_4payer;
		
		// for RECEIVER
		for(int j = 0; j < g_model_period; ++j)
		{	
			for (int i = j; i < g_model_period; ++i)
			{
				if ( (tte_swap(i, j) == 0 && i >= j))
				{
					_rho_swaps[i_ma].set_val(i, j, 0);
					break;
				}
				else
				{

					rho = diff_swap_price(_swap_rate->_zero_coupon_rates,
										  _swap_rate->_zero_coupon_shock_up_rates[i_ma],
										  _swap_rate->_zero_coupon_shock_down_rates[i_ma],
										  j,
										  j + _swap_maturity,
										  i,
										  fix_rate, // is calculated if necessary (i = j) inside this function
										  g_compounding_frequencies[i_ma],
										  g_nb_steps_per_year,
										  true); // receiver 

					// modified 14/01/2011
					if(i != j ) // current time != trading time
					{
						rho /= (2* g_shocks[i_ma]/100.0);
					}
					else // current time = trading time
					{
						if(i_ma != _ma_id) // if bucket maturity is different from swap's maturity => rho = 0 (for new buying swaps)
						{
							rho = 0.0;
						}
					}
					
					_rho_swaps[i_ma].set_val(i, j, rho);
				}
			}
		}
		
		// for PAYER: need to separate in 02 different loops as fixed rate for RECEIVER & PAYER are different
		for(int j = 0; j < g_model_period; ++j)
		{	
			for (int i = j; i < g_model_period; ++i)
			{
				if ( (tte_swap(i, j) == 0 && i >= j))
				{
					_rho_swaps_4payer[i_ma].set_val(i, j, 0);
					break;
				}
				else
				{
					// for PAYER => isReceiver = false
					rho_4payer = diff_swap_price(_swap_rate->_zero_coupon_rates,
												 _swap_rate->_zero_coupon_shock_up_rates[i_ma],
												 _swap_rate->_zero_coupon_shock_down_rates[i_ma],
												 j,
												 j + _swap_maturity,
												 i,
												 fix_rate_4payer, // is calculated if necessary (i = j) inside this function
												 g_compounding_frequencies[i_ma],
												 g_nb_steps_per_year, 
												 false) ;// payer
					// modified 14/01/2011
					if(i != j ) // current time != trading time
					{
						rho_4payer /= (2* g_shocks[i_ma]/100.0);
					}
					else // current time = trading time
					{
						if(i_ma != _ma_id)
						{
							rho_4payer = 0.0; // if bucket maturity is different from swap's maturity => rho = 0 (for new buying swaps)
						}
					}
					
					_rho_swaps_4payer[i_ma].set_val(i, j, rho_4payer);
				}
			}

		}
	}
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
// unrealised market value do not contains coupons
double Swap::market_value( int i )
{
	double val = 0;	
	for (deque<TradeInfo>::iterator iter = _nb_swap[i].begin(); iter != _nb_swap[i].end(); ++iter)
	{
		// added 10/01/2011
		if (iter->_amount > 0) // receiver
		{
			val += iter->_amount*_sv_bases(i, iter->_time);
		}
		else // payer
		{
			val += iter->_amount*_sv_bases_4payer(i, iter->_time);
		}

	}
	return val;
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
double Swap::trading(int i)
{
	double trading = 0.0;
	if ( i == 0 ) // at time step 0
	{
		if (_nb_swap[0][0]._amount > 0) // receiver
		{
			trading = _nb_swap[0][0]._amount*_sv_bases(0,0);
		}
		else // payer
		{
			trading = _nb_swap[0][0]._amount*_sv_bases_4payer(0,0);
		}
		return trading;
	}

	for (deque<TradeInfo>::iterator iter = _nb_swap[i].begin(); iter != _nb_swap[i].end(); ++iter)
	{
		if (iter->_amount  > 0) // receiver
		{
			trading += iter->_amount * _sv_bases(i, iter->_time);
		}
		else // payer
		{
			trading += iter->_amount * _sv_bases_4payer(i, iter->_time);
		}
	}

	int preI = i-1; // previous trading => TODO: have to check after that (why we need this?)
	for (deque<TradeInfo>::iterator iter = _nb_swap[preI].begin(); iter != _nb_swap[preI].end(); ++iter)
	{
		if (iter->_amount  > 0) // receiver
		{
			trading -= iter->_amount * _sv_bases(i, iter->_time);
		}
		else // payer
		{
			trading -= iter->_amount * _sv_bases_4payer(i, iter->_time);
		}
	}
	return trading;
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
void Swap::init_rho_bef(int i, double& rho_bef)
{
	if ( i == 0 ) // at time step 0
	{
		_rho_bef[0] = rho_bef;
		return;
	}
	// Yaokun: 15-07-2010
	for (deque<TradeInfo>::iterator iter = _nb_swap[i-1].begin(); iter != _nb_swap[i-1].end(); ++iter )
	{
		if (iter->_amount  > 0) // receiver
		{
			rho_bef += iter->_amount * _rho_swaps[_ma_id](i, iter->_time);
		}
		else // payer
		{
			rho_bef += iter->_amount * _rho_swaps_4payer[_ma_id](i, iter->_time);
		}
	}
	_rho_bef[i] = rho_bef;
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
void Swap::init_rho_af(int i)
{
	_rho_af[i] = _rho_bef[i];
	if ( _nb_swap[i].size() != 0 ) // we have swaps in portfolio
	{
		if (_nb_swap[i].back()._amount  > 0) // receiver
		{
			_rho_af[i] += _nb_swap[i].back()._amount * _rho_swaps[_ma_id](i, i);
		}
		else // payer
		{
			_rho_af[i] += _nb_swap[i].back()._amount * _rho_swaps_4payer[_ma_id](i, i);
		}
	}
	// added 13/01/2011
	else // no swaps in portfolio
	{
		_rho_af[i] = 0.0;
	}
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
void Swap::buy_sell_new_swap(int i, double rho_net)
{
	if ( i != 0 )
	{
		_nb_swap[i] = _nb_swap[i-1];
		if ( _nb_swap[i].size() != 0 && _nb_swap[i][0]._time < i - _swap_maturity )
		{
			_nb_swap[i].pop_front(); // remove all expired swaps
		}
	}

	double rho_av; // rho for a swap of notional 1
	if (rho_net > 0) // => need a receiver swap for having a negative rho
	{
		rho_av = _rho_swaps[_ma_id](i, i);
	}
	else // => need a payer swap for having a positive rho
	{
		rho_av = _rho_swaps_4payer[_ma_id](i, i); // ATTENTION: _rho_swaps_4payer stock (-1*rho) of payer swaps => rho_av is always negative
	}

	_nb_swap[i].push_back( TradeInfo( i,  - rho_net/rho_av ) ); //store trading time and notional to be traded (with sign convention)

	// analyse
	_eq_swap[i] = - rho_net/rho_av; //modified 10/01/2011: notional that we've just bought
	_coupon[i] = 0;

	for (deque<TradeInfo>::iterator iter = _nb_swap[i].begin(); iter != _nb_swap[i].end(); ++iter)
	{
		//if ( _coupons(i, iter->_time) != 0 ) // commented 10/01/2011: NO NEED to check this
		//{
		//	_coupon[i] += _coupons(i, iter->_time)*iter->_amount;
		//}

		if (iter->_amount > 0) // receiver
		{
			_coupon[i] += _coupons(i, iter->_time)*iter->_amount;
		}
		else // payer
		{
			_coupon[i] += _coupons_4payer(i, iter->_time)*iter->_amount;
		}

		// _eq_swap[i] += iter->_amount; // commented 10/01/2011: it'll give an accumulated notional which is useless
	}

	_mkt_value[i] = market_value(i);
	_trading[i] = 0;
	if ( i == 0 )
	{
		_trading[0] = trading(0);
		_pl_realized[0] = 0;
		_pl_unrealized[0] = _mkt_value[0] - _trading[0];
	}
	else
	{
		_trading[i] = trading(i);
		_pl_unrealized[i] = _mkt_value[i] - _mkt_value[i-1] - _trading[i];
		_pl_realized[i] = _coupon[i];
	}

	_trans_cost[i] *= abs(_nb_swap[i].back()._amount);
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
//--TODO: Verify again this function
void Swap::realise_exist_swap(int i)
{
	if (i == 0) // should be an error to enter here at the beginning
	{
		return;
	}

	// receive coupon today, then sell all the swap ( coupon do not contained in mkv )
	_nb_swap[i] = _nb_swap[i-1];
	if ( _nb_swap[i].size() != 0 && _nb_swap[i][0]._time < i - _swap_maturity )
	{
		_nb_swap[i].pop_front(); // remove all expired swaps
	}
	
	_coupon[i] = 0;
	for (deque<TradeInfo>::iterator iter = _nb_swap[i].begin(); iter != _nb_swap[i].end(); ++iter)
	{
		//if ( _coupons(i, iter->_time) != 0 ) // commented 10/01/2011: NO NEED to check this
		//{
		//	_coupon[i] += _coupons(i, iter->_time)*iter->_amount;
		//}

		if (iter->_amount > 0) // receiver
		{
			_coupon[i] += _coupons(i, iter->_time)*iter->_amount;
		}
		else // payer
		{
			_coupon[i] += _coupons_4payer(i, iter->_time)*iter->_amount;
		}
	}

	// do not need to buy new swap
	_nb_swap[i].clear();

	// realise all existing swap
	// analyse
	_mkt_value[i] = 0;
	_eq_swap[i] = 0;
	
	// calculate trading
	_trading[i] = trading(i);

	_pl_unrealized[i] = 0;	//_mkt_value[i] - _mkt_value[i-1] - _trading[i];
	_pl_realized[i] = -_mkt_value[i-1] - _trading[i] + _coupon[i];
	_trans_cost[i] = 0;
}

// Modified 10/01/2011
void Swap::dumpInfo()
{
	int* g_lbs_maturities = _config->g_lbs_maturities;
	char buf[4];
	_ltoa_s(_ma_id, buf, 4, 10);
	string s_no(buf);
	s_no.append("-");
	_ltoa_s(g_lbs_maturities[_ma_id], buf, 4, 10);
	s_no.append(buf);

	string file = "../04_for_debug/rho_swap_";
	file.append(s_no);
	file.append(".csv");
	ofstream fout(file.c_str());
	fout.precision(16);
	for ( int i_ma = 0; i_ma <= _ma_id; ++i_ma )
	{
		fout << g_lbs_maturities[i_ma] << endl;
		for ( int i = 0; i < g_model_period; ++i)
		{
			for ( int j = 0; j < g_model_period; ++j)
			{
				fout << _rho_swaps[i_ma](i, j) << ",";
			}
			fout << endl;
		}
		fout << endl;
	}
	fout.clear();
	fout.close();

	file = "../04_for_debug/sv_base_";
	file.append(s_no);
	file.append(".csv");
	fout.open(file.c_str());
	fout.precision(16);
	for ( int i = 0; i < g_model_period; ++i)
	{
		for ( int j = 0; j < g_model_period; ++j)
		{
			fout << _sv_bases(i,j) << ", ";
		}
		fout << endl;
	}
	fout.clear();
	fout.close();

	file = "../04_for_debug/swap_coupon_";
	_ltoa_s(_ma_id, buf, 4, 10);
	s_no.assign(buf);
	s_no.append("-");
	_ltoa_s(g_lbs_maturities[_ma_id], buf, 4, 10);
	s_no.append(buf);
	file.append(s_no);
	file.append(".csv");

	fout.open(file.c_str());
	fout.precision(15);
	for (int i = 0; i < g_model_period; ++i)
	{
		for (int j = 0; j < g_model_period; ++j)
		{
			fout << _coupons(i, j) << ",";
		}
		fout << endl;
	}
	fout.close();


	// added 10/01/2011: for PAYER swaps
	file = "../04_for_debug/rho_swap_PAYER_";
	file.append(s_no);
	file.append(".csv");
	fout.open(file.c_str());
	fout.precision(16);
	for ( int i_ma = 0; i_ma <= _ma_id; ++i_ma )
	{
		fout << g_lbs_maturities[i_ma] << endl;
		for ( int i = 0; i < g_model_period; ++i)
		{
			for ( int j = 0; j < g_model_period; ++j)
			{
				fout << _rho_swaps_4payer[i_ma](i, j) << ",";
			}
			fout << endl;
		}
		fout << endl;
	}
	fout.clear();
	fout.close();

	file = "../04_for_debug/sv_base_PAYER_";
	file.append(s_no);
	file.append(".csv");
	fout.open(file.c_str());
	fout.precision(16);
	for ( int i = 0; i < g_model_period; ++i)
	{
		for ( int j = 0; j < g_model_period; ++j)
		{
			fout << _sv_bases_4payer(i,j) << ", ";
		}
		fout << endl;
	}
	fout.clear();
	fout.close();

	file = "../04_for_debug/swap_coupon_PAYER_";
	_ltoa_s(_ma_id, buf, 4, 10);
	s_no.assign(buf);
	s_no.append("-");
	_ltoa_s(g_lbs_maturities[_ma_id], buf, 4, 10);
	s_no.append(buf);
	file.append(s_no);
	file.append(".csv");

	fout.open(file.c_str());
	fout.precision(15);
	for (int i = 0; i < g_model_period; ++i)
	{
		for (int j = 0; j < g_model_period; ++j)
		{
			fout << _coupons_4payer(i, j) << ",";
		}
		fout << endl;
	}
	fout.close();
}

void Swap::dumpPurchaseInfo()
{
	char buf[4];
	_ltoa_s(_ma_id, buf, 4, 10);
	string s_no(buf);
	s_no.append("-");
	_ltoa_s(_config->g_lbs_maturities[_ma_id], buf, 4, 10);
	s_no.append(buf);

	string file = "../04_for_debug/notional_swap_";
	file.append(s_no);
	file.append(".csv");
	ofstream fout(file.c_str());

	for ( int i = 0; i < g_model_period; ++i)
	 {
		 if (_nb_swap[i].size() == 0)
		 {
			 fout << ',' << endl;
			 continue;
		 }
		 for ( int j = 0; j < _nb_swap[i][0]._time; ++j)
		 {
			 fout << "0,";
		 }
		 fout << _nb_swap[i][0]._amount << ",";
		 for ( int j = 1; j < _nb_swap[i].size(); ++j)
		 {
			 for (  int k = _nb_swap[i][j-1]._time+1; k < _nb_swap[i][j]._time; ++k )
			 {
				 fout << "0,";
			 }
			 fout << _nb_swap[i][j]._amount << ",";
		 }
		 fout << endl;
	 }
	fout.close();
}