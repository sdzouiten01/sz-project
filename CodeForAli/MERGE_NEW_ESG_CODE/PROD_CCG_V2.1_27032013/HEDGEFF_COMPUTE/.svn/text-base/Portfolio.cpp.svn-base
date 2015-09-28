#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cassert>
#include "Portfolio.h"
#include "Assumptions.h"
//#include "MathTable.h"
using namespace std;

void Portfolio::loadConfig(string config, string liabOutPut, string rw_scn)
{
	_config.loadConfig(config, liabOutPut, rw_scn);
	
	g_model_period = _config.g_model_period;
	g_nb_av = _config.g_nb_av;
	g_nb_index_to_hedge = _config.g_nb_index_to_hedge;
	g_nb_steps_per_year = _config.g_nb_steps_per_year;
	g_nb_lbs_maturities = _config.g_nb_lbs_maturities;
	g_lbs_maturities = _config.g_lbs_maturities;
	g_indexes_to_hedge = _config.g_indexes_to_hedge;
	g_index_base =  _config.g_index_base;

	g_reb_freq_delta = _config.g_reb_freq_delta;
	g_reb_freq_gamma = _config.g_reb_freq_gamma;
	g_net_gamma_lower_bound = _config.g_net_gamma_lower_bound;
	g_net_gamma_upper_bound = _config.g_net_gamma_upper_bound;

	g_reb_freq_rho = _config.g_reb_freq_rho;
	g_swap_trade_upper_seuil = _config.g_swap_trade_upper_seuil;
	g_swap_trade_lower_seuil = _config.g_swap_trade_lower_seuil;
	g_swap_trade_NO_upper_seuil = _config.g_swap_trade_NO_upper_seuil; // added 11/01/2011
	g_unwind_swap = _config.g_unwind_swap;

	allocateMemory();
	p_swap_rate = new SwapRate(&_config);
}
void Portfolio::allocateMemory()
{
	// memery allocate
	_total_pl_future = NULL;
	_av = new double[g_model_period];
	_charges = new double[g_model_period];
	_claims = new double[g_model_period];
	_liab_ov = new double[g_model_period];
	_delta_lbs = new double[g_model_period];
	_gamma_lbs = new double[g_model_period];
	_rho_0_lbs = new double[g_model_period];

	_av_indexes = new double*[g_nb_av];
	_indexes = new double*[g_nb_av];
	_delta_index_ = new double*[g_nb_av];
	_gamma_index_ = new double*[g_nb_av];
	_rho_i_lbs = new double*[g_nb_lbs_maturities];

	for (int i = 0; i < g_nb_av; ++i)
	{
		_av_indexes[i] = new double [g_model_period];
		_indexes[i] = new double[g_model_period];
		_delta_index_[i] = new double[g_model_period];
		_gamma_index_[i] = new double[g_model_period];
	}
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		_rho_i_lbs[i] = new double[g_model_period];
	}

	_options_puts.resize(g_nb_index_to_hedge);
	if (_isHedgeDelta)
	{
		_futures.resize(g_nb_index_to_hedge);
	}
	if (_isHedgeRho)
	{
		_swaps.resize(g_nb_lbs_maturities);
	}
}
void Portfolio::loadDataHeader(string liabOutPut)
{
	_liab_file.open(liabOutPut.c_str());
	if ( !_liab_file )
	{
		cout << "Error: Liability files do not found- " << liabOutPut << endl;
		exit(1);
	}
	string line;
	getline(_liab_file, line);			// skip the first line of the input file, we can add check file-format here.
}

// modified 17/11/2010
// load data from the liability file
// need to well set-up the format in the congifuration file: nb_index_total, nb_columns to skip, model_period, etc.
void Portfolio::loadScenData(int scen_no)
{
	string stmp;
	// go to the start line of the current scenario
	_cur_scen = INT_MIN;
	_liab_file >> _cur_scen;
	while ( _cur_scen != scen_no && !_liab_file.eof() )
	{
		getline(_liab_file, stmp);
		_liab_file >> _cur_scen;
	}

	// begin load data
	int tmp; char ch; double db_tmp;
	for ( int i = 0; i < g_model_period; ++i)
	{
		// read scen_no
		if ( i != 0 )
		{
			_liab_file >> _cur_scen;
		}
		if ( _cur_scen != scen_no )	// has ready finished current scenario
		{
			break;
		}
		_claims[i] = 0;
		_liab_ov[i] = 0;
		_charges[i] = 0;
		_av[i] = 0;

		_liab_file >> ch;				// read ','
		assert( ch == ',' );
		_liab_file >> tmp;				// read period
		_liab_file >> ch;
		_liab_file >> _liab_ov[i];

		// --------------------------------------------------------------------------------------------
		// -- depend on format of the liability file, we have to skip several columns (if necessary) --
		// ------------ for instant: between CLAIMS and CHARGES and/or CHARGES and AV -----------------
		// ------------ need to change directly in the configuration file -----------------------------
		// --------------------------------------------------------------------------------------------
		// claims
		_liab_file >> ch;               // skip ','
		_liab_file >> _claims[i];

		// skip columns between column of CLAIMS and CHARGES
		for (int j = 0; j < _config.g_nb_column_skip_between_claims_and_charges; ++j)
		{
			_liab_file >> ch;
			_liab_file >> db_tmp;
		}

		// charges
		_liab_file >> ch;               // skip ','
		_liab_file >> _charges[i];

		// skip columns between column of CHARGES and AV
		for (int j = 0; j < _config.g_nb_column_skip_between_charges_and_av; ++j)
		{
			_liab_file >> ch;           // skip ','
			_liab_file >> db_tmp;
		}

		// total av
		_liab_file >> ch;               // skip ','
		_liab_file >> _av[i];

		for (int j = 0; j < g_nb_av; ++j)
		{
			_liab_file >> ch;		    // skip ','
			_av_indexes[j][i] = 0;
			_liab_file >> _av_indexes[j][i];
		}
		for (int j = g_nb_av; j < 12; ++j)
		{
			_liab_file >> ch;				// skip ','
			_liab_file >> db_tmp;
		}
		for (int j = 0; j < g_nb_av; ++j)
		{
			_liab_file >> ch;				// skip ','
			_delta_index_[j][i] = 0;
			_liab_file >> _delta_index_[j][i];
		}
		_liab_file >> ch;				// skip ','
		_liab_file >> _delta_lbs[i];
		for (int j = 0; j < g_nb_av; ++j)
		{
			_liab_file >> ch;				// skip ','
			_gamma_index_[j][i] = 0;
			_liab_file >> _gamma_index_[j][i];
		}
		_liab_file >> ch;				// skip ','
		_gamma_lbs[i] = 0;
		_liab_file >> _gamma_lbs[i];
		_liab_file >> ch;
		_rho_0_lbs[i] = 0;
		_liab_file >> _rho_0_lbs[i];    // global rho liability

		//-------------------------------------------
		// added 30/11/2010
		////// to skip the rho 1Y
		////_liab_file >> ch;				// skip ','
		////_liab_file >> db_tmp;

		if (g_lbs_maturities[0] > 1)        // skip the rho 1Y
		{
			_liab_file >> ch;				// skip ','
			_liab_file >> db_tmp;
		}
		//-------------------------------------------


		for (int j = 0; j < g_nb_lbs_maturities; ++j)
		{
			_liab_file >> ch;				// skip ','
			_rho_i_lbs[j][i] = 0;
			_liab_file >> _rho_i_lbs[j][i];
		}
		/*
		for (int j = 0; j < g_nb_av; ++j)
		{
			_liab_file >> ch;				// skip ','
			_liab_file >> _indexes[j][i];
		}
		*/
		if ( _av[i] == 0 && _liab_file.fail())
		{
			_liab_file.clear();
		}
		getline(_liab_file, stmp);						// have read all the datas we care, skip the rest of this line
	}

	p_swap_rate->loadScenData(scen_no, this);
	p_swap_rate->initial();

	double index_base = g_index_base;
	for (int i = 0; i < g_nb_av; ++i)
	{
		_indexes[i][0] = index_base;
	}
	for (  int t = 1; t < g_model_period; ++t )
	{
		for (int i = 0; i < g_nb_av; ++i)
		{
			_indexes[i][t] = _indexes[i][t-1]*(1+_indexes[i][t]/100.0);
		}
	}

	double factor;
	for (  int t =0; t < g_model_period; ++t)
	{
		for (int i = 0; i < g_nb_av; ++i)
		{
			factor = _av_indexes[i][t]/_indexes[i][t];
			double delta = _delta_index_[i][t];
			double gamma = _gamma_index_[i][t];
			_delta_index_[i][t] *= factor;
			_gamma_index_[i][t] *= (factor*factor);
		}
	}
}
Portfolio::~Portfolio()
{
	delete [] _av;
	delete [] _charges;
	delete [] _claims;
	delete [] _liab_ov;
	delete [] _delta_lbs;
	delete [] _gamma_lbs;
	delete [] _rho_0_lbs;

	for ( int i  = 0; i < g_nb_av; ++i)
	{
		delete [] _av_indexes[i];
		delete [] _indexes[i];
		delete [] _delta_index_[i];
		delete [] _gamma_index_[i];
	}
	delete [] _av_indexes;
	delete [] _indexes;
	delete [] _delta_index_;
	delete [] _gamma_index_;

	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		delete [] _rho_i_lbs[i];
	}
	delete [] _rho_i_lbs;

	if ( _total_pl_future != NULL )
	{
		delete [] _total_pl_future;
		delete [] _total_pl_option;
		delete [] _total_pl_unrealized_swap;
		delete [] _total_pl_realized_swap;

		delete [] _total_trans_cost_future;
		delete [] _total_trans_cost_option;
		delete [] _total_trans_cost_swap;

		delete [] _cash;
	}
}
// Functions for hedging

void Portfolio::hedging(string liabOutPut, string rw_scn, int beg, int end, string rlt, ofstream& log_strm )
{
	//log_strm << " hedge: " << 1 << endl;
	// skip file head
	loadDataHeader(liabOutPut);
	p_swap_rate->loadDataHeader(rw_scn);
	
	// add suffix to the intermediate files
	char buf[10];
	string output_name = rlt+"_";
	itoa(beg, buf, 10);
	output_name.append(buf);
	output_name.append("_");
	itoa(end, buf, 10);
	output_name.append(buf);
	output_name.append(".csv"); // file format csv
	dumpRltHeader(output_name);

	// begin hedging for each scenario
	for ( int scen = beg; scen <= end; ++scen )
	{
		// load data for this scenario
		loadScenData(scen);

		// begin hedge
		hedging(log_strm);

		// output result for this scenario
		dumpCurScenRlt();
	}

	// finished, close file
	_liab_file.clear();
	_liab_file.close();
	_rlt_file.clear();
	_rlt_file.close();
	//log_strm << " hedge end "  << endl;
}
void Portfolio::hedging(ofstream& log_strm)
{
	//log_strm << " Hedge Gamma" << endl;
	if (_isHedgeGamma)
	{
		int index_i_pos;
		for (  int i = 0; i < g_nb_index_to_hedge; ++i )
		{
			index_i_pos = g_indexes_to_hedge[i]-1;						// index_i (i>=1)saved in _av_indexes[i-1]
			_options_puts[i].init_underlying(g_indexes_to_hedge[i], _indexes[index_i_pos], p_swap_rate, &_config);
			//_options_puts[i].dumpInfo();
		}

		for ( int i = 0; i < g_nb_index_to_hedge; ++i)
		{
			index_i_pos = g_indexes_to_hedge[i]-1;
			gamma_neutral(i, index_i_pos);
			_options_puts[i].analyse();

			if ( _config._isDumpOptionNb )
				_options_puts[i].dumpPutInfo();
		}
	}
	//log_strm << " Hedge Delta" << endl;
	if (_isHedgeDelta)
	{
		int index_i_pos;
		for (  int i = 0; i < g_nb_index_to_hedge; ++i )
		{
			if (_isHedgeGamma)
				_options_puts[i].init_delta();
			
			index_i_pos = g_indexes_to_hedge[i]-1;						// index_i (i>=1)saved in _av_indexes[i-1]
			_futures[i].init_underlying(g_indexes_to_hedge[i], _indexes[index_i_pos], p_swap_rate, &_config);
			//_futures[i].dumpInfo();
		}

		for ( int i = 0; i < g_nb_index_to_hedge; ++i)
		{
			index_i_pos = g_indexes_to_hedge[i]-1;
			delta_neutral(i, index_i_pos);
			_futures[i].analyse();

			if (  _config._isDumpFutureNb )
				_futures[i].dumpPurchaseInfo();
		}
	}

	//log_strm << " Hedge Rho" << endl;
	if (_isHedgeRho)
	{
		p_swap_rate->shock();
		
		// for debugging purpose
		if ( _config._isDumpSwapRho )
		{
			p_swap_rate->dump();
		}

		for ( int i = 0; i < g_nb_lbs_maturities; ++i )
		{
			_swaps[i].initial(i, p_swap_rate, &_config);
			_swaps[i].init_rho_swaps();
		}

		calc_rho_fut_and_put();
		rho_neutral();

		for ( int i = 0; i < _swaps.size(); ++i )
		{	
			if ( _config._isDumpSwapRho )
				_swaps[i].dumpInfo();

			if ( _config._isDumpSwapNb )
				_swaps[i].dumpPurchaseInfo();
		}
	}
	//log_strm << " Hedge Finish" << endl;
	//g_swap_rate.dump();
}
void Portfolio::gamma_neutral(int put_id, int idx_id)
{
	double gamma_net, gamma_port;
	bool is_short_exist_position = false;
	for ( int i = 0; i < g_model_period; ++i)
	{
		gamma_port = _options_puts[put_id].init_gamma_bef(i);
		
		gamma_net = gamma_port - _gamma_index_[idx_id][i];		// gamma_net[i]: net gamma before hedge at t = i
		_options_puts[put_id]._gamma_net[i] = gamma_net;
		// 1. Decision of  the sitution of hedge
		//	-check whether t_i is the time we should do hedging
		//  case 1: do not need to hedge in this period
		if ( (i+1)%g_reb_freq_gamma != 0)
			continue;

		// ratio = gamma_net/_gamma_index_[idx_id][i];
		// case 2: net_gamma is in our bearable interval, doesn't need hedge
		if ( g_net_gamma_lower_bound < gamma_net  && gamma_net < g_net_gamma_upper_bound )
			continue;

		is_short_exist_position = false;
		// case 3/4: decide whether we need to buy/sell existing put or not (instead of buy/sell new put)
		if ( (gamma_net > g_net_gamma_upper_bound && gamma_port > 0) 
			|| (gamma_net < g_net_gamma_lower_bound && gamma_port < 0) )
		{
			is_short_exist_position = true;
		}

		// 2. Beginning of hedge from here!
		if ( is_short_exist_position )
		{
			_options_puts[put_id].buy_sell_exist_put(i, _gamma_index_[idx_id][i]);
		}
		else	// is_short_exist_position = false: vende nouveau ou liquidation existante
		{
			_options_puts[put_id].buy_sell_new_put(i, _gamma_index_[idx_id][i]);
		}
		_options_puts[put_id].init_gamma_af(i);
	}
}
void Portfolio::delta_neutral(int i_pos, int idx_id)
{
	double delta_net, delta_future, delta_port;
	bool is_short_exist_position = false;
	for ( int i = 0; i < g_model_period; ++i)
	{
		delta_future = _futures[i_pos].init_delta_bef(i); 
		delta_port = delta_future;
		if ( _isHedgeGamma  )
		{
			delta_port += _options_puts[i_pos]._delta[i];
		}

		delta_net = delta_port - _delta_index_[idx_id][i];		// delta_net[i]: net delta before hedge at t = i
		_futures[i_pos]._delta_net[i] = delta_net;
		// 1. Decision of  the sitution of hedge
		//	-check whether t_i is the time we should do hedging
		//  case 1: do not need to hedge this period
		if ( (i+1)%g_reb_freq_delta != 0)
			continue;

		is_short_exist_position = false;
		// case 2/3: decide whether we need to buy/sell existing future or not (instead of buy/sell new future)
		if ( (delta_net > 0 && delta_future > 0)  || (delta_net < 0 && delta_future < 0) )
		{
			is_short_exist_position = true;
		}

		// 2. Beginning of hedge from here!
		if ( is_short_exist_position )
		{
			_futures[i_pos].buy_sell_exist_future(i);
		}
		else	// is_short_exist_position = false: vende nouveau ou liquidation existante
		{
			_futures[i_pos].buy_sell_new_future(i, _delta_index_[idx_id][i]);
		}
		_futures[i_pos].init_delta_af(i);
	}
}

// Modified 10/01/2011
// using the convention: positive notional => RECEIVER, negative notional => PAYER
//--------------------------------------------------------------------------------
void Portfolio::rho_neutral()
{
	double ratio;
	double rho_net = 0, rho_port = 0;
	double rho_exist_asset = 0;
	// hedge for each period
	for ( int i = 0; i < g_model_period; ++i)
	{
		if ( (i+1)%g_reb_freq_rho != 0)
				break;

		// if don't support dividing by zero
		if ( _av[i] == 0 )
		{
			ratio  = INT_MAX;
		}
		else
			ratio = _rho_0_lbs[i]/_av[i];
		

		// Modified 11/01/2011
		// -- in case we don't need an upper seuil => just add MAX in the upper seuil of the run configuration
		//if ( g_unwind_swap && (g_swap_trade_lower_seuil < ratio ) )//&& ratio < g_swap_trade_upper_seuil) )
		//{
		//	if ( g_swap_trade_NO_upper_seuil || (!g_swap_trade_NO_upper_seuil && ratio < g_swap_trade_upper_seuil) )
		//	{
		//		// Yaokun: To be completed....
		//		// sell all the swap at this time, calculate the new PL realised
		//		for ( int i_swap = 0; i_swap < _swaps.size(); ++i_swap )
		//		{
		//			_swaps[i_swap].realise_exist_swap(i);
		//		}
		//	}
		//}
		
		// --TODO--: check the formula
		// Modified 13/01/2011
		// -- always calculate the asset rho, net rho before
		// -- change the condition to execute the unwind swaps
		if ( g_unwind_swap && (g_swap_trade_lower_seuil < ratio ) 
			 && (g_swap_trade_NO_upper_seuil || (!g_swap_trade_NO_upper_seuil && ratio < g_swap_trade_upper_seuil)) ) // then unwind swaps
		{
			// calcualte the asset rho before
			for (int i_ma = g_nb_lbs_maturities-1; i_ma >= 0; --i_ma)
			{
				rho_port = 0;

				// asset rho before trade (including what we have just traded)
				_swaps[i_ma].init_rho_bef(i, rho_port);

				rho_port += (_rho_futures[i][i_ma] + _rho_puts[i][i_ma]); // plus rho from future or options if any
				rho_net = rho_port - _rho_i_lbs[ i_ma ][i];

				_swaps[i_ma]._rho_net[i] = rho_net;
			}

			// sell all the swap at this time, calculate the new PL realised
			for ( int i_swap = 0; i_swap < _swaps.size(); ++i_swap )
			{
				_swaps[i_swap].realise_exist_swap(i);
			}

		}
		else // then swap trade normally
		{
			// rho hedge from the largest maturities: 50 ->40 -> ... -> 1
			for (int i_ma = g_nb_lbs_maturities-1; i_ma >= 0; --i_ma)
			{
				rho_port = 0;
				// to calculate the rho of NEWLY swaps for bucket of maturity i_ma
				for ( int j_ma = i_ma+1; j_ma < g_nb_lbs_maturities; ++j_ma )
				{
					for (deque<TradeInfo>::iterator iter = _swaps[j_ma]._nb_swap[i].begin(); iter != _swaps[j_ma]._nb_swap[i].end(); ++iter)
					{
						if (iter->_amount > 0) // receiver
						{
							rho_port += iter->_amount*_swaps[j_ma]._rho_swaps[i_ma](i, iter->_time);
						}
						else // payer
						{
							rho_port += iter->_amount*_swaps[j_ma]._rho_swaps_4payer[i_ma](i, iter->_time);
						}
					}
				}
				
				// asset rho before trade (including what we have just traded)
				_swaps[i_ma].init_rho_bef(i, rho_port);

				rho_port += (_rho_futures[i][i_ma] + _rho_puts[i][i_ma]); // plus rho from future or options if any
				rho_net = rho_port - _rho_i_lbs[ i_ma ][i];

				_swaps[i_ma]._rho_net[i] = rho_net;
				_swaps[i_ma].buy_sell_new_swap(i, rho_net);
			}
		}
		
		// compute the asset rho after trade
		for ( int i_ma = 0; i_ma < g_nb_lbs_maturities; ++i_ma )
		{
			_swaps[i_ma].init_rho_af(i);
		}
	}
}

void Portfolio::calc_rho_fut_and_put()
{
	_rho_puts.resize(g_model_period);
	_rho_futures.resize(g_model_period);

	for (  int i = 0; i < g_model_period; ++i )
	{
		_rho_puts[i].resize(g_nb_lbs_maturities);
		_rho_futures[i].resize(g_nb_lbs_maturities);

		for ( int i_ma = 0; i_ma < g_nb_lbs_maturities; ++i_ma )
		{
			_rho_puts[i][i_ma] = 0;
			_rho_futures[i][i_ma] = 0;
			if (_isHedgeGamma)
			{
				for (  int j = 0; j < _options_puts.size();  ++j )
				{
					_rho_puts[i][i_ma] += _options_puts[j].rho_total(i, i_ma);
				}
			}
			if (_isHedgeDelta)
			{
				for (  int j = 0; j < _futures.size();  ++j )
				{
					_rho_futures[i][i_ma] += _futures[j].rho_total(i, i_ma);
				}
			}
		}
	}
}
void Portfolio::dumpRltHeader(string rlt)
{
	// display Header
	_rlt_file.open(rlt.c_str());
	_rlt_file<<"Scenario,Period";
	_rlt_file<<",Option Value";
	for (  int i=1; i<= g_nb_index_to_hedge; ++i )
	{
		_rlt_file<<",Delta Liab "<<i;
		if (_isHedgeDelta)
		{
			_rlt_file<<",Asset Delta Before Futures"<<i;
			_rlt_file<<",Net Delta Before Futures"<<i;
			_rlt_file<<",Asset Delta After Futures"<<i;
			_rlt_file<<",Net Delta After Futures"<<i;
			_rlt_file<<",Eq Fut Ct "<<i;
			_rlt_file<<",Future Pursh "<<i;
			_rlt_file<<",Future Sales "<<i;
			_rlt_file<<",Market Value (Fut) "<<i;
		}
		if (_isHedgeGamma)
		{
	//	_rlt_file<<",Total Notional "<<i;
			_rlt_file<<",Gamma Liab "<<i;	
			_rlt_file<<",Asset Gamma Before Options"<<i;
			_rlt_file<<",Net Gamma Before Options"<<i;
			_rlt_file<<",Asset Gamma After Options"<<i;
			_rlt_file<<",Net Gamma After Options"<<i;
			_rlt_file<<",Opt Not "<<i;
			_rlt_file<<",Eq Put Ct "<<i;
			_rlt_file<<",Option Pursh "<<i;
			_rlt_file<<",Option Sales "<<i;
			_rlt_file<<",Market Value (Opt)"<<i;
		}
	}
	if (_isHedgeRho)
	{
		for ( int i = 0; i < g_nb_lbs_maturities; ++i )
		{
			int maturity = g_lbs_maturities[i];
			_rlt_file << ",Rho Liab " << maturity;
			_rlt_file << ",Asset Rho Before Swaps" << maturity;
			_rlt_file << ",Net Rho Before Swaps" << maturity;
			_rlt_file << ",Asset Rho After Swaps" << maturity;
			_rlt_file << ",Net Rho After Swaps" << maturity;
			_rlt_file << ",Rt Swp Ct " << maturity;
			_rlt_file << ",Swap Pursh " << maturity;
			_rlt_file << ",Swap Sales " << maturity;
			_rlt_file << ",Market Value (Swp) " << maturity;
			_rlt_file << ",Coupon" << maturity;
		}
	}

	//_rlt_file<<",Int Cont";
	//_rlt_file<<",Settle Cont";

	_rlt_file<<",Pnl Eq Fututres";
	_rlt_file<<",Fut Trans Costs";
	_rlt_file<<",Pnl Opt";
	_rlt_file<<",Opt Trans Costs";
	_rlt_file<<",Pnl unrealized Swaps";
	_rlt_file<<",Pnl realized Swaps";
	_rlt_file<<",Swp Trans Costs";
	_rlt_file<<",Trans Costs Opt + Fut + Swp";	
	_rlt_file<<",Pnl Opt + Fut + Swp";	
	_rlt_file<<",Cash";
	_rlt_file<<",Total Coupon";
	_rlt_file<<",Option value Change";
	_rlt_file<<",Total_charges";
	_rlt_file<<",claims";
	_rlt_file<<",Porftolio Pnl";
	_rlt_file<<",TOTAL PNL";

	_rlt_file<<endl;
}
void Portfolio::dumpCurScenRlt()
{
	analyse();
	// display contents
	double trading;
	double trans_cost;
	double pl;
	double ov_change;
	double port_pl;
	double coupon;
	for( int k=0;k < g_model_period;++k)
	{
		coupon = 0;
		_rlt_file << _cur_scen << ',' << k << ',' << _liab_ov[k];

		for (  int i=0; i < g_nb_index_to_hedge; ++i )
		{
			_rlt_file << ',' << _delta_index_[i][k];
			if (_isHedgeDelta)
			{
				double delta_bef = _futures[i]._delta_bef[k];
				double delta_net_bef = _futures[i]._delta_bef[k] - _delta_index_[i][k];
				double delta_af = _futures[i]._delta_af[k];
				double delta_net_af = _futures[i]._delta_af[k] - _delta_index_[i][k];

				if (_isHedgeGamma)
				{
					delta_bef += _options_puts[i]._delta[k];
					delta_net_bef += _options_puts[i]._delta[k];
					delta_af += _options_puts[i]._delta[k];
					delta_net_af += _options_puts[i]._delta[k];
				}				
				_rlt_file << ',' << delta_bef;
				_rlt_file << ',' << delta_net_bef;
				_rlt_file << ',' <<  delta_af;
				_rlt_file << ',' << delta_net_af;
				_rlt_file << ',' << _futures[i]._eq_future[k];

				trading = _futures[i]._trading[k];
				// Option Purchase & Option Sales
				if (trading > 0)
				{
					_rlt_file << ',' <<trading;
					_rlt_file << ',' << 0;
				}
				else
				{
					_rlt_file << ',' << 0;
					_rlt_file << ',' << -trading;
				}
				_rlt_file << ',' << _futures[i]._mkt_value[k];
			}
			if (_isHedgeGamma)
			{
				//	_rlt_file<<","<<tab_of_delta_gamma_hedge[j][i]->delta->total_notional[k];
				_rlt_file <<","<<_gamma_index_[i][k];
				_rlt_file <<","<<_options_puts[i]._gamma_bef[k];
				// net_gamma_before_options
				_rlt_file<<","<< (_options_puts[i]._gamma_bef[k] - _gamma_index_[i][k]);
				_rlt_file<<","<<_options_puts[i]._gamma_af[k];
				// net_gamma_after_options[k]
				_rlt_file<<","<< (_options_puts[i]._gamma_af[k] - _gamma_index_[i][k]);
				
				_rlt_file<<","<<_options_puts[i]._opt_not[k];
				_rlt_file<<","<<_options_puts[i]._eq_put[k];

				trading = _options_puts[i]._trading[k];
				if (trading > 0)
				{
					_rlt_file<<","<<trading;
					_rlt_file<<","<< 0;
				}
				else
				{
					_rlt_file<<","<< 0;
					_rlt_file<<","<< -trading;
				}
				_rlt_file<<","<<_options_puts[i]._mkt_value[k];
			}
		}

		if (_isHedgeRho)
		{
			for (int i = 0; i < g_nb_lbs_maturities;++i)
			{
				double rho_bef = _swaps[i]._rho_bef[k];
				double rho_net_bef = _swaps[i]._rho_bef[k] -  _rho_i_lbs[i][k];
				double rho_af = _swaps[i]._rho_af[k];
				double rho_net_af = _swaps[i]._rho_af[k] -  _rho_i_lbs[i][k];

				if (_isHedgeGamma)
				{
					rho_bef += _rho_puts[k][i];
					rho_net_bef += _rho_puts[k][i];
					rho_af += _rho_puts[k][i];
					rho_net_af += _rho_puts[k][i];
				}
				if (_isHedgeDelta)
				{
					rho_bef += _rho_futures[k][i];
					rho_net_bef += _rho_futures[k][i];
					rho_af += _rho_futures[k][i];
					rho_net_af += _rho_futures[k][i];
				}
				_rlt_file << "," <<  _rho_i_lbs[i][k];
				_rlt_file << "," <<  rho_bef;
				_rlt_file << "," <<  rho_net_bef;
				_rlt_file << "," <<  rho_af;
				_rlt_file << "," <<  rho_net_af;
				_rlt_file << "," <<  _swaps[i]._eq_swap[k];

				trading = _swaps[i]._trading[k];
				if (trading > 0)
				{
					_rlt_file<<","<<trading;
					_rlt_file<<","<< 0;
				}
				else
				{
					_rlt_file<<","<< 0;
					_rlt_file<<","<< -trading;
				}
				_rlt_file << "," <<  _swaps[i]._mkt_value[k];
				_rlt_file << "," <<  _swaps[i]._coupon[k];
				coupon += _swaps[i]._coupon[k];
			}
		}
		_rlt_file<<","<<_total_pl_future[k];
		_rlt_file<<","<<_total_trans_cost_future[k];
		_rlt_file<<","<<_total_pl_option[k];
		_rlt_file<<","<<_total_trans_cost_option[k];
		_rlt_file<<","<<_total_pl_unrealized_swap[k];
		_rlt_file<<","<<_total_pl_realized_swap[k];
		_rlt_file<<","<<_total_trans_cost_swap[k];
		
		trans_cost = _total_trans_cost_future[k]+_total_trans_cost_option[k]+_total_trans_cost_swap[k];
		_rlt_file<<","<< trans_cost;
		pl = _total_pl_future[k]+_total_pl_option[k]+_total_pl_unrealized_swap[k]+_total_pl_realized_swap[k];;
		_rlt_file<<","<< pl;
		_rlt_file<<","<< _cash[k];
		_rlt_file<<","<< coupon;

		ov_change = 0;
		if ( k > 0 )
		{
			ov_change = _liab_ov[k]-_liab_ov[k-1];
			_rlt_file<<","<< ov_change;
		}
		else
			_rlt_file<<","<< ov_change;
		
		_rlt_file<<","<< _charges[k];
		_rlt_file<<","<< _claims[k];
		//portfolio_pnl
		port_pl = pl + _cash[k] - trans_cost;
		_rlt_file<<","<< port_pl;
		// total_pnl
		_rlt_file<<","<< port_pl - ov_change + _charges[k] - _claims[k];
		_rlt_file<<endl;
	}
}
void Portfolio::analyse()
{
	if (_total_pl_future == NULL)		// has not allocated memory before
	{
		_total_pl_future = new double[g_model_period];
		_total_pl_option = new double[g_model_period];
		_total_pl_unrealized_swap = new double[g_model_period];
		_total_pl_realized_swap = new double[g_model_period];

		_total_trans_cost_future = new double[g_model_period];
		_total_trans_cost_option = new double[g_model_period];
		_total_trans_cost_swap = new double[g_model_period];

		_cash = new double[g_model_period];
	}

	double ratio = 0;
	for (  int i = 0; i < g_model_period; ++i )
	{
		_total_pl_option[i] = 0;
		_total_trans_cost_option[i] = 0;
		_cash[i] = 0;
		if (_isHedgeGamma)
		{
			for (  int j = 0; j < _options_puts.size(); ++j )
			{
				_total_pl_option[i] += _options_puts[j]._pl[i];
				_total_trans_cost_option[i] += _options_puts[j]._trans_cost[i];

				if (i > 0)
					_cash[i] += _options_puts[j]._mkt_value[i-1];
			}
		}
		_total_pl_future[i] = 0;
		_total_trans_cost_future[i] = 0;
		if (_isHedgeDelta)
		{
			for (  int j = 0; j < _futures.size(); ++j )
			{
				_total_pl_future[i] += _futures[j]._pl[i];
				_total_trans_cost_future[i] += _futures[j]._trans_cost[i];

				/*
				// Yaokun: 09-07-2010
				// cash do not include market value of futures, commented by Mawaheb.
				if (i > 0)
					_cash[i] += _futures[j]._mkt_value[i-1];
				*/
			}
		}

		_total_pl_unrealized_swap[i] = 0;
		_total_pl_realized_swap[i] = 0;
		_total_trans_cost_swap[i] = 0;
		//double total;
		if (_isHedgeRho)
		{
		//	total  = 0;
			for (  int j = 0; j < _swaps.size(); ++j )
			{
				_total_pl_unrealized_swap[i] += _swaps[j]._pl_unrealized[i];
				_total_pl_realized_swap[i] += _swaps[j]._pl_realized[i];
				_total_trans_cost_swap[i] += _swaps[j]._trans_cost[i];

				if (i > 0)
					_cash[i] += _swaps[j]._mkt_value[i-1];
			}
		}
		if (i > 0)
		{
			ratio = 1.0/g_nb_steps_per_year*p_swap_rate->_zero_coupon_rates[i][1] + (1-1.0/g_nb_steps_per_year)*p_swap_rate->_zero_coupon_rates[i][0];
			_cash[i] = (_liab_ov[i-1]-_cash[i])*ratio/(double)g_nb_steps_per_year;
		}
	}
}