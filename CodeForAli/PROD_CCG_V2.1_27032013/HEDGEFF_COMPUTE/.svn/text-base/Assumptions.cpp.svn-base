#include "Assumptions.h"
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

void Configuration::loadConfig(string config, string liab, string rw_scn)
{
	init_rw_maturities(rw_scn);

	string stmp;
	// get model period if it has not been assigned before manuallly.
	if ( g_model_period == 0 )
	{
		ifstream ftmp(liab.c_str());
		if ( !ftmp )
		{
			cout << "Error: do not found " << liab << endl;
			exit(1);
		}
		getline(ftmp, stmp);
		int scen_id, scen_cur;
		ftmp >> scen_id;
		getline(ftmp,stmp);
		ftmp >> scen_cur;
		getline(ftmp,stmp);
		while( scen_id ==  scen_cur && !ftmp.eof() )
		{
			ftmp >> scen_cur;
			getline(ftmp,stmp);
			++g_model_period;
		}
		ftmp.clear();
		ftmp.close();
	}
	
	if (config == "")
		return;

	// follwing code is to read from a config .txt file => --TODO--: TO BE DELETED. Commented for instance.
	// SHOULD NOT GOING HERE
	cout << " PLEASE VERIFY YOUR SET UP! Now the run configuration is all set up in a xml file. \n";
	exit(1);

	/*
	ifstream fin(config.c_str());
	if ( !fin )
	{
		cout << "Error: Configuration files do not found- " << config << endl;
		exit(1);
	}
	fin >> g_nb_steps_per_year;
	getline(fin, stmp);
	fin >> g_nb_av;
	getline(fin, stmp);
	char ch;
	fin >> g_nb_index_to_hedge;
	if ( g_nb_index_to_hedge > g_nb_av )
	{
		cout << "Error: nb_index_to_hedge should less than account number" << endl;
		exit(1);
	}
	g_indexes_to_hedge = new int[g_nb_index_to_hedge];
	fin >> ch;		// skip ':'
	for (int i = 0; i < g_nb_index_to_hedge; ++i)
	{
		fin >> g_indexes_to_hedge[i];
	}
	getline(fin, stmp);
	fin >> g_index_base;
	getline(fin, stmp);
	fin >> g_nb_lbs_maturities;
	fin >> ch;					// skip ':'

	g_lbs_maturities = new int[g_nb_lbs_maturities];
	g_start_ma = new int[g_nb_lbs_maturities];
	g_end_ma = new int[g_nb_lbs_maturities];
	g_shocks = new double[g_nb_lbs_maturities];
	g_compounding_frequencies = new int[g_nb_lbs_maturities];
	g_swap_spread = new double[g_nb_lbs_maturities];

	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		fin >> g_lbs_maturities[i];
	}
	getline(fin, stmp);
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		int tmp;
		fin >> tmp;
		g_start_ma[i] = tmp;
	}
	getline(fin, stmp);
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		int tmp;
		fin >> tmp;
		g_end_ma[i] = tmp;
	}
	getline(fin, stmp);
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		fin >> g_shocks[i];
	}
	getline(fin, stmp);
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		fin >> g_compounding_frequencies[i];
	}
	getline(fin, stmp);
		//
		//fin >> g_nb_swap_maturities;
		//fin >> ch;				// skip ':'
		//g_swap_maturities.resize(g_nb_swap_maturities);
		//for (int i = 0; i < g_nb_swap_maturities; ++i)
		//{
		//	fin >> g_swap_maturities[i];
		//}
		//getline(fin, stmp);
		//
	fin >> g_moneyness;
	getline(fin, stmp);
	fin >> g_length_of_option_contract;
	getline(fin, stmp);
	fin >> g_length_of_future_contract;
	getline(fin, stmp);
	fin >> g_sell_at_time_to_expiry;
	getline(fin, stmp);
	fin >> g_volatility;
	getline(fin, stmp);
	fin >> g_rates_freq_ann;
	getline(fin, stmp);
	fin >> g_sheduled_futures;
	getline(fin, stmp);
	fin >> g_reb_freq_gamma;
	getline(fin, stmp);
	fin >> g_reb_freq_delta;
	getline(fin, stmp);
	fin >> g_reb_freq_rho;
	getline(fin, stmp);
	fin >> g_net_gamma_lower_bound;
	getline(fin, stmp);
	fin >> g_net_gamma_upper_bound;
	getline(fin, stmp);
	fin >> g_liab_gamma_lower_bound;
	getline(fin, stmp);
	fin >> g_liab_gamma_upper_bound;
	getline(fin, stmp);
	g_unwind_swap = false;
	fin >> g_unwind_swap;
	if ( g_unwind_swap )
	{
		fin >> ch;
		fin >> g_swap_trade_lower_seuil;			// read seuil for swap trade
		fin >> g_swap_trade_upper_seuil;
	}
	getline(fin, stmp);
	fin >> g_delta_trans_cost;
	getline(fin, stmp);
	fin >> g_gamma_trans_cost;
	getline(fin, stmp);
	for ( int i = 0; i < g_nb_lbs_maturities; ++i )
	{
		fin >> g_swap_spread[i];
		g_swap_spread[i] /= 100.0;
	}
	getline(fin, stmp);

	_isDumpOptionNb = false;
	_isDumpFutureNb = false;
	_isDumpSwapNb = false;
	_isDumpSwapRho = false;
	fin >> _isDumpOptionNb;
	getline(fin, stmp);
	fin >> _isDumpFutureNb;
	getline(fin, stmp);
	fin >> _isDumpSwapNb;
	getline(fin, stmp);
	fin >> _isDumpSwapRho;
	fin.close();
	*/
}
void Configuration::init_rw_maturities(string rw_file)
{
	ifstream fin(rw_file.c_str());
	if (!fin)
	{
		cout << "Error: file do not found " << rw_file << endl;
		exit(1);
	}
	string row;
	getline(fin, row);
	char * pch;
	char* next_token;
	pch = strtok_s((char*)row.c_str(),",",&next_token);		// scen
	pch = strtok_s(NULL, ",",&next_token);							// period
	pch = strtok_s(NULL, ",",&next_token);

	vector<int> ma;
	ma.reserve(11);
	g_nb_swap_maturities = 0;
	int maturity;
	while ( strcmp(pch, "Beta") != 0 && strcmp(pch, "beta") != 0 )
	{
		maturity = atoi(pch);
		if ( maturity == 0 )
		{
			cout << "Error:  bad format in real world file " << rw_file << endl;
			cout << "  name for maturity in the first line should be nothing but a number!" << endl;
			exit(1);
		}
		ma.push_back( maturity );
		g_nb_swap_maturities++;
		pch = strtok_s(NULL, ",",&next_token);
	}
	g_swap_maturities = new int[g_nb_swap_maturities];
	for ( int i = 0; i < g_nb_swap_maturities; ++i )
	{
		g_swap_maturities[i] = ma[i];
	}
	fin.close();
}
Configuration::~Configuration()
{
	delete [] g_lbs_maturities;
	delete [] g_start_ma;
	delete [] g_end_ma;
	delete [] g_shocks;
	delete [] g_compounding_frequencies;
	delete [] g_swap_maturities;
	delete [] g_indexes_to_hedge;
	delete [] g_swap_spread;
}