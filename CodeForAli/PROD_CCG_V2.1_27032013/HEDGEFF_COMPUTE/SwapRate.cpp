#include "SwapRate.h"
#include "Assumptions.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cassert>
using namespace std;

SwapRate::SwapRate(Configuration* cfg):_is_shocked(true), _config(cfg)
{
	g_model_period = _config->g_model_period;
	g_rates_freq_ann = _config->g_rates_freq_ann;
	g_nb_swap_maturities = _config->g_nb_swap_maturities;
	g_nb_lbs_maturities = _config->g_nb_lbs_maturities;
	g_swap_maturities = _config->g_swap_maturities;
	g_nb_av = _config->g_nb_av;

	allocateMemory();
}
void SwapRate::loadDataHeader(string rw_scn)
{
	_scn_file.open(rw_scn.c_str());
	if ( !_scn_file )
	{
		cout << "Error: Rate files do not found - " << rw_scn << endl;
		exit(1);
	}
	string line;
	getline(_scn_file, line);			// skip the first line of the input file
}
void SwapRate::allocateMemory()
{
	_rates = new double*[g_model_period];

	for ( int i = 0; i < g_model_period; ++i)
	{
		_rates[i] = new double[g_nb_swap_maturities];
	}

	//
	dim_x = g_model_period;
	 dim_y = g_swap_maturities[g_nb_swap_maturities-1]+1;
	 _annual_swap_rates=new double*[dim_x];
	 for(int i=0;i<this->dim_x;++i)
	 {
		 _annual_swap_rates[i]=new double[dim_y];
	 }
	 _maturities=new int[dim_y];

	 _zero_coupon_rates=new double*[dim_x];
	 for(int i=0;i<dim_x;++i)
	 {
		 _zero_coupon_rates[i]=new double[dim_y];
	 }

	 // allocate memories
	_zero_coupon_shock_up_rates = new double**[g_nb_lbs_maturities];
	_zero_coupon_shock_down_rates = new double**[g_nb_lbs_maturities];
	_shocks = new double*[g_nb_lbs_maturities];
	for (int i = 0; i < g_nb_lbs_maturities; ++i)
	{
		_zero_coupon_shock_up_rates[i] = new double*[dim_x];
		_zero_coupon_shock_down_rates[i] = new double*[dim_x];

		_shocks[i] = new double[dim_y];
		//_shocks[i] = new double[dim_x];
		for(int j = 0; j < dim_x; ++j )
		{
			_zero_coupon_shock_up_rates[i][j] = new double[dim_y];
			_zero_coupon_shock_down_rates[i][j] = new double[dim_y];
			/*for(int k = 0; k < dim_y; ++k )
			{
				_zero_coupon_shock_up_rates[i][j][k]=0.;
				_zero_coupon_shock_down_rates[i][j][k]=0.;
			}*/
		}
	}
}
void SwapRate::loadScenData(int scen_no, Portfolio* pt)
{
	string stmp;
	// go to the start line of the current scenario
	_cur_scen = INT_MIN;
	_scn_file >> _cur_scen;
	while ( _cur_scen != scen_no && !_scn_file.eof() )
	{
		getline(_scn_file, stmp);
		_scn_file >> _cur_scen;
	}

	int tmp; char ch; double d_tmp;
	for ( int i = 0; i < g_model_period; ++i)
	{
		if ( i != 0 )
		{
			_scn_file >> _cur_scen;
		}
		if ( _cur_scen != scen_no )	// has ready finished current scenario
		{
			break;
		}
		_scn_file >> ch;							// skip ','
		_scn_file >> tmp;						// read period

		for ( int j = 0; j < g_nb_swap_maturities; ++j )
		{
			_scn_file >> ch;						// skip ','
			_scn_file >> _rates[i][j];
		}
		_scn_file >> ch;
		_scn_file >> d_tmp;				// skip Beta
		for (int j = 0; j < g_nb_av; ++j)
		{
			_scn_file >> ch;				// skip ','
			_scn_file >> pt->_indexes[j][i];
		}
		getline(_scn_file, stmp);			// have read all the data we care, skip this line	
	}
}

void SwapRate::initial()
{
	// linearly interpolated the swap rates
	 _maturities[0] = 0;
	 for( int j = 0; j < g_model_period; ++j)
	 {
		 int deb = g_swap_maturities[0];

		 for(int k = 0; k < g_nb_swap_maturities - 1; ++k)
		 {
			 int fin = g_swap_maturities[k+1];
			 double pente = (_rates[j][k+1]-_rates[j][k]) / (g_swap_maturities[k+1] - g_swap_maturities[k]);
			 for(int t = deb; t <= fin; ++t)
			 {
				 _maturities[t] = _maturities[t-1] + 1;
				 _annual_swap_rates[j][t] = _rates[j][k] + (_maturities[t] - g_swap_maturities[k]) * pente;
			 }
			 deb = fin + 1;
		 }
		 int t = g_swap_maturities[0] - 1;
		 while(t >= 0)
		 {
			 _annual_swap_rates[j][t] = 2 * _annual_swap_rates[j][t+1] - _annual_swap_rates[j][t+2];
			 t--;
		 }
	 }

	 // convert swap rate input to be annual if it is not (ONLY work for semi-annual!!!)
	 // convert also the swap rate into its absolute value
	 // --TODO: may change this to be clear from the input (annual, semi-annual or quartely swaps as input)
	 for(int i = 0; i < this->dim_x; ++i)
	 {
		 for(int j = 0; j < this->dim_y; ++j)
		 {
			 _annual_swap_rates[i][j] = g_rates_freq_ann * _annual_swap_rates[i][j]/100.0 +
										(1 - g_rates_freq_ann) * (pow((1 + 0.5 * _annual_swap_rates[i][j]/100.0), 2) - 1);
		 }
	 }

	 // convert to zero_coupon_rates
	 for(int i = 0; i < dim_x; ++i)
	 {
		 _zero_coupon_rates[i][0] = _annual_swap_rates[i][0];
		 _zero_coupon_rates[i][1] = _annual_swap_rates[i][1];
		 for(int j = 2; j < dim_y; ++j)
		 {
			 _zero_coupon_rates[i][j] = 
				 pow(
					 ( (1+_annual_swap_rates[i][j]) /
						 (1-(_annual_swap_rates[i][j]/_annual_swap_rates[i][j-1])*(1-pow((1+_zero_coupon_rates[i][j-1]), 1-j))) )
					 , (1./(double)j)
					) 
					- 1.0;
		 }
	 }
 }

// modified 28/11/2010
// commented as it's not used
//void SwapRate::calc_zero_coupon()
//{
//	 _maturities[0] = 0;
//
//	 // convert swap rate input to be annual if it is not (ONLY work for semi-annual!!!)
//	 // convert also the swap rate into its absolute value
//	 // --TODO: may change this to be clear from the input (annual, semi-annual or quartely swaps as input)
//	 for(int i = 0; i < this->dim_x; ++i)
//	 {
//		 for(int j = 0; j < this->dim_y; ++j)
//		 {
//			 _annual_swap_rates[i][j] = g_rates_freq_ann * _annual_swap_rates[i][j]/100.0 +
//										(1-g_rates_freq_ann)*(pow((1+0.5*_annual_swap_rates[i][j]/100.0),2)-1);
//		 }
//	 }
//
//	 // Zero_Swap_Coupon
//	 for(int i = 0; i < dim_x; ++i)
//	 {
//		 _zero_coupon_rates[i][0] = _annual_swap_rates[i][0];
//		 _zero_coupon_rates[i][1] = _annual_swap_rates[i][1];
//		 for(int j = 2; j < dim_y; ++j)
//		 {
//			 //_zero_coupon_rates[i][j]=pow((
//				// (1+_annual_swap_rates[i][j])/
//				// (1-(_annual_swap_rates[i][j]/_annual_swap_rates[i][j-1])*(1-pow((1+_zero_coupon_rates[i][j-1]),1-j)))
//				// ),(1./(double)j))-1;
//			 _zero_coupon_rates[i][j] = 
//				 pow(
//					 ( (1+_annual_swap_rates[i][j]) /
//						 (1-(_annual_swap_rates[i][j]/_annual_swap_rates[i][j-1])*(1-pow((1+_zero_coupon_rates[i][j-1]), 1-j))) )
//					 , (1./(double)j)
//					) 
//					- 1.0;
//		 }
//	 }
// }

SwapRate::~SwapRate()
{
	 for(int i=0;i<this->dim_x;++i)
	 {
		 delete [] _annual_swap_rates[i];
		 delete [] _zero_coupon_rates[i];
		 delete [] _rates[i];
	 }

	 delete [] _rates;
	 delete [] _annual_swap_rates;
	 delete [] _zero_coupon_rates;
	 delete [] _maturities;
	 if (_is_shocked)
	 {
		 //for (int i = 0; i < _config->_nb_swap_maturities; ++i)
		for (int i = 0; i < g_nb_lbs_maturities/*_config->_nb_lbs_maturities*/; ++i)
		 {
			 for(int j = 0; j < dim_x; ++j )
			 {
				 delete [] _zero_coupon_shock_up_rates[i][j];
				 delete [] _zero_coupon_shock_down_rates[i][j];
			 }
			 delete [] _zero_coupon_shock_up_rates[i];
			 delete [] _zero_coupon_shock_down_rates[i];
			 delete [] _shocks[i];
		 }
		 delete [] _zero_coupon_shock_up_rates;
		 delete [] _zero_coupon_shock_down_rates;
		 delete [] _shocks;
	 }
	 _scn_file.close();
}
void SwapRate::dump()
 {
	 ofstream fout("../04_for_debug/rate_mine.csv");
	 fout << "Rate" << endl;
	 fout.precision(20);
	 for ( int i = 0; i < g_model_period; ++i)
	 {
		 for ( int j = 0; j < g_nb_swap_maturities; ++j )
		 {
			 fout << _rates[i][j] << ",";
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 fout.open("../04_for_debug/annual_swap_rate_mine.csv");
	 fout << "Annual Swap Rate" << endl;
	 fout.precision(15);
	 for (int i = 0; i < dim_x; ++i)
	 {
		 for (int j = 0; j < dim_y; ++j)
		 {
			 fout << _annual_swap_rates[i][j] << ",";
		 }
		 fout << endl;
	 }
	 fout << endl;
	 fout.clear();
	 fout.close();
	 
	 fout.open("../04_for_debug/zero_coupon_rate_mine.csv");
	 fout << "_zero_coupon_rates" << endl;
	 fout.precision(20);
	 for (int i = 0; i < dim_x; ++i)
	 {
		 for (int j = 0; j < dim_y; ++j)
		 {
			 fout << _zero_coupon_rates[i][j] << ",";
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 fout.open("../04_for_debug/shock_rate_mine.csv");
	 fout.precision(20);
	 for (int i = 0; i < g_nb_lbs_maturities; ++i)
	 {
		 for ( int j = 0; j < dim_y; ++j )
		 {
			 fout << _shocks[i][j] << ',';
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();

	 fout.open("../04_for_debug/zero_coupon_shock_up_rate_mine.csv");
	 fout.precision(20);
	 for (int i_ma = 0; i_ma < g_nb_lbs_maturities; ++i_ma)
	 {
		 fout << _config->g_lbs_maturities[i_ma] << endl;
		 for (int i = 0; i < dim_x; ++i)
		 {
			 for (int j = 0; j < dim_y; ++j)
			 {
				 fout << _zero_coupon_shock_up_rates[i_ma][i][j] << ",";
			 }
			 fout << endl;
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();
	 fout.open("../04_for_debug/zero_coupon_shock_down_rate_mine.csv");
	 fout.precision(20);
	 for (int i_ma = 0; i_ma < g_nb_lbs_maturities; ++i_ma)
	 {
		 fout << _config->g_lbs_maturities[i_ma] << endl;
		 for (int i = 0; i < dim_x; ++i)
		 {
			 for (int j = 0; j < dim_y; ++j)
			 {
				 fout << _zero_coupon_shock_down_rates[i_ma][i][j] << ",";
			 }
			 fout << endl;
		 }
		 fout << endl;
	 }
	 fout.clear();
	 fout.close();
 }

void SwapRate::shock()
{
	#pragma region Initial shocks
	double* g_shocks      = _config->g_shocks;
	int* g_lbs_maturities = _config->g_lbs_maturities;
	int* g_start_ma       = _config->g_start_ma;  
	int* g_end_ma         = _config->g_end_ma;

	double shock_val, step;

	for ( int i = 0; i < g_nb_lbs_maturities; ++i )
	{
		for ( int j = 0; j < g_start_ma[i]; ++j )
		{
			_shocks[i][j] = 0.0;						// zero shock at the beginning
		}

		if (g_start_ma[i] > 0)
		{
			shock_val = 0.0;
			step      = (g_shocks[i]/100.0) / (g_lbs_maturities[i]-g_start_ma[i]);
			for ( int j = g_start_ma[i]; j < g_lbs_maturities[i]; ++j )
			{
				_shocks[i][j] = shock_val;
				shock_val    += step;                   // linearly shift upward starting from zero
			}
		}
		else // parallel shift at the beginning
		{
			for ( int j = 0; j < g_lbs_maturities[i]; ++j )
			{
				_shocks[i][j] = (g_shocks[i]/100.0);
			}
		}

		if (g_end_ma[i] < dim_y)
		{
			shock_val = g_shocks[i]/100.0;
			step      = shock_val/(g_end_ma[i]-g_lbs_maturities[i]);
			for ( int j = g_lbs_maturities[i]; j < min(g_end_ma[i], dim_y); ++j )
			{
				_shocks[i][j] = shock_val;
				shock_val    -= step;                   // linearly shift downward to zero
			}
		}
		else // parallel shift at the end 
		{
			for ( int j = g_lbs_maturities[i]; j < min(g_end_ma[i], dim_y); ++j )
			{
				_shocks[i][j] = g_shocks[i]/100.0;
			}
		}

		for ( int j = g_end_ma[i]; j < dim_y; ++j )
		{
			_shocks[i][j] = 0.0;                        // zero shock afterward
		}
	}
	#pragma endregion
	
	#pragma region calculate shock_up/down on zero_coupon_rate
	int maturity;
	double annual_shock_up, annual_shock_up_prev, annual_shock_down, annual_shock_down_prev;
	for (int i_ma = 0; i_ma < g_nb_lbs_maturities; ++i_ma)
	{
		maturity = g_lbs_maturities[i_ma];
		for (int i = 0; i < dim_x; ++i)
		{
			_zero_coupon_shock_up_rates[i_ma][i][0]   = _annual_swap_rates[i][0] + _shocks[i_ma][0];
			_zero_coupon_shock_down_rates[i_ma][i][0] = _annual_swap_rates[i][0] - _shocks[i_ma][0];

			_zero_coupon_shock_up_rates[i_ma][i][1]   = _annual_swap_rates[i][1] + _shocks[i_ma][1];
			_zero_coupon_shock_down_rates[i_ma][i][1] = _annual_swap_rates[i][1] - _shocks[i_ma][1];

			for( int j = 2; j < dim_y; ++j )
			{
				annual_shock_down_prev = _annual_swap_rates[i][j-1] - _shocks[i_ma][j-1];
				annual_shock_down      = _annual_swap_rates[i][j]   - _shocks[i_ma][j];
				_zero_coupon_shock_down_rates[i_ma][i][j] = 
					pow(
					  ( (1+annual_shock_down) /
						  (1-(annual_shock_down/annual_shock_down_prev)*(1-pow((1+_zero_coupon_shock_down_rates[i_ma][i][j-1]), 1-j))) )
					  , (1./(double)j)
					  )
					  -1.0;

				annual_shock_up_prev = _annual_swap_rates[i][j-1] + _shocks[i_ma][j-1];
				annual_shock_up      = _annual_swap_rates[i][j]   + _shocks[i_ma][j];
				_zero_coupon_shock_up_rates[i_ma][i][j] = 
					pow(
					  ( (1+annual_shock_up)/
					     (1-(annual_shock_up/annual_shock_up_prev)*(1-pow((1+_zero_coupon_shock_up_rates[i_ma][i][j-1]), 1-j))) )
					  , (1./(double)j))
					  -1.0;
			}
		}
	}
	#pragma endregion
}