/******************************************************************************
* Class Portfolio
*		- This class represent our portefolio = liability + Assets;
*			note: Assets = index_i + puts of index_i +futures of index_i + Swaps + Swaptions
*		- It does all the  hedges: Gamma, Delta, Rho, Rho-convexity
*		- Now, this class handle only one scenarios
* 15-06: add support for severel scenarios
*
* 23-05-2010
******************************************************************************/
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "OptionPut.h"
#include "Future.h"
#include "Swap.h"
#include "SwapRate.h"

using std::ofstream;

class Portfolio{
	friend class SwapRate;
public:
	~Portfolio();
	void loadConfig(std::string config, std::string liabOutPut, std::string rw_scn);
	void hedging(std::string liabOutPut, std::string rw_scn, int beg, int end, std::string rlt, ofstream& log_strm);

	// Variables for hedging
	bool _isHedgeDelta, _isHedgeGamma, _isHedgeRho, _isHedgeRhoConv;
	Configuration _config;

private:
	void allocateMemory();
	void loadDataHeader(std::string liabOutPut);
	void loadScenData(int scen_no);

	void hedging(ofstream& log_strm);			// hedge for current scenario
	void gamma_neutral(int i, int idxId);		// hedge for the i-th OptionPut of av_idxId, _index_to_hedge[i] = av_index
	void delta_neutral(int i, int idxId);		// hedge for the i-th Future of av_idxId, _index_to_hedge[i] = av_index
	void rho_neutral();

	void calc_rho_fut_and_put();
	void analyse();

	void dumpRltHeader(std::string rlt);
	void dumpCurScenRlt();

	SwapRate* p_swap_rate;
	// Variables for data
	double*	_liab_ov;
	double*	_claims;
	double*	_charges;
	double*	_av;											// av = sum(av_index_i)
	double**	_av_indexes;
	double** _indexes;									// return rate*1000
	double** _delta_index_;
	double** _gamma_index_;
	double*	_delta_lbs;
	double*	_gamma_lbs;
	double*	_rho_0_lbs;
	double** _rho_i_lbs;								// [nb_ma][nb_time]

	double*	_total_pl_future;
	double*	_total_pl_option;
	double*	_total_pl_unrealized_swap,  *_total_pl_realized_swap;
	double*	_total_trans_cost_future;
	double*	_total_trans_cost_option;
	double*	_total_trans_cost_swap;
	double*	_cash;

	// Assets
	std::vector<OptionPut>				_options_puts;
	std::vector<Future>					_futures;
	std::vector<std::vector<double> >	_rho_futures;
	std::vector<std::vector<double> >	_rho_puts;
	std::vector<Swap>					_swaps;

	int _scen_beg,  _scen_end;
	int _cur_scen;

	std::ifstream _liab_file;
	std::ofstream _rlt_file;
	
	// Configuration
	int g_model_period;
	int g_nb_av;
	int* g_indexes_to_hedge;
	int g_nb_index_to_hedge;
	int g_nb_steps_per_year;
	int g_nb_lbs_maturities;
	int* g_lbs_maturities;
	double g_index_base;

	int g_reb_freq_delta;
	int g_reb_freq_gamma;
	int g_reb_freq_rho;
	double g_net_gamma_lower_bound;
	double g_net_gamma_upper_bound;
	double g_swap_trade_upper_seuil, g_swap_trade_lower_seuil;
	bool g_swap_trade_NO_upper_seuil; // added 11/01/2011
	bool g_unwind_swap;
};