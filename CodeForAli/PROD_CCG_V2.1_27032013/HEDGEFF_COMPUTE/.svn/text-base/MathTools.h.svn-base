/******************************************************************************
* namespace used for some mathematics functions
*		
* 23-05-2010
/******************************************************************************/
#pragma once
#include <cmath>

namespace AXAHedgeEffective
{

#define		PI 3.14159265
#define		ONE_OVER_SQRT_2PI 0.398942280401433

	// Classic d0 function used in BS formula
	inline double d0(double t,double x,double y,double sigma)
	{
		double sigmaSqrtT = sigma*sqrt(t);
		return ( log(x/y)/sigmaSqrtT - sigmaSqrtT/2.0 );
	}
	// Classic d1 function used in BS formula
	inline double d1(double t,double x,double y,double sigma)
	{
		double sigmaSqrtT = sigma*sqrt(t);
		return ( log(x/y)/sigmaSqrtT + sigmaSqrtT/2.0 );
	}

	inline double f(double x)
	{
		double y = 1./(1+0.33267*x);
		double y2 = y*y;
		double a=0.4361836;
		double b=-0.1201676;
		double c=0.937298;
		return (1-ONE_OVER_SQRT_2PI*exp(-x*x*0.5)*(a*y+b*y2+c*y*y2));
	}
	// Cumulative distribution of normal law N(0,1)
	inline double N(double x)
	{
		return  ( x>=0 ? f(x): (1-f(-x)) );
	}
	// Density of N(0,1)
	inline double DiffN(double x)
	{
		return ONE_OVER_SQRT_2PI*exp(-x*x*0.5);
	}
	// delta of a put in a BS model
	inline double delta_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma)
	{
		return -N(
		d0(
		time_to_maturity,
		strike,
		underlying*exp(rate*time_to_maturity),		
		sigma)
		);
	}
	// Gamma of a put in a BS model
	inline double gamma_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma)
	{
		return DiffN(
		d1(
		time_to_maturity,
		underlying*exp(rate*time_to_maturity),
		strike,
		sigma)
		)/(underlying*sigma*sqrt(time_to_maturity));
	}
	// rho of a put in a BS model
	inline double rho_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma)
	{
		return -time_to_maturity*strike*exp(-rate*time_to_maturity)*N(
		d1(
		time_to_maturity,
		strike,
		underlying*exp(rate*time_to_maturity),		
		sigma)
		);
	}
	// Price of a put in a BS model
	inline double put_price_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma)
	{
		return
		strike*exp(-rate*time_to_maturity)*
		N(d1(time_to_maturity,strike,underlying*exp(rate*time_to_maturity),sigma))
		-underlying*N(d0(time_to_maturity,strike,underlying*exp(rate*time_to_maturity),sigma));
	}

	extern double g_math_swap_price_spread;
	extern double* g_math_swap_price_trans_cost;

	double swap_price(double** annual_zero_coupons_rates,
							  int start_time,
							  int end_time,
							  int current_time,
							  double& g_swap_rate,
							  int frequency,
							  int nb_steps_per_year,
							  double& coupon,
							  bool isCalcSwapRate = true,
							  bool isShocked = false,
							  double zero_coupon_prec = 0,
							  double zero_coupon_next = 0,
							  bool isReceiver = true // added 10/01/2011
							  );
	
	double diff_swap_price(double** zero_coupons_rates,
							  double** zero_shocked_up_coupons_rates,
							  double** zero_shocked_down_coupons_rates,
							  int start_time,
							  int end_time,
							  int current_time,
							  double& swap_rate,
							  int frequency,
							  int nb_steps_per_year,
							  bool isReceiver = true // added 10/01/2011
							  );

	//double swap_price(double** annual_zero_coupons_rates,
	//						  int start_time,
	//						  int end_time,
	//						  int current_time,
	//						  double& g_swap_rate,
	//						  int frequency,
	//						  int nb_steps_per_year,
	//						  double& coupon,
	//						  bool isCalcSwapRate = true,
	//						  bool isShocked = false,
	//						  double zero_coupon_prec = 0,
	//						  double zero_coupon_next = 0
	//						  );

	//double diff_swap_price(double** zero_coupons_rates,
	//						  double** zero_shocked_up_coupons_rates,
	//						  double** zero_shocked_down_coupons_rates,
	//						  int start_time,
	//						  int end_time,
	//						  int current_time,
	//						  double& swap_rate,
	//						  int frequency,
	//						  int nb_steps_per_year);
	/*
	double swap_price2(double** annual_zero_coupons_rates,
							  int start_time,
							  int end_time,
							  int current_time,
							  double& swap_rate,
							  double& zero_coupon_prec,
							  double& zero_coupon_next,
							  double frequency,
							  int nb_steps_per_year,
							  bool force_swap_rate,
							  bool force_zero_coupon_prec
							  );
	 */
}
// Return the maximum between a and b
	/*
	inline double max(double a, double b)
	{
		return (a>b) ? a:b;
	}
	// Return the minimum between a and b
	inline double min(double a, double b)
	{
		return (a>b) ? b:a;
	}
	*/