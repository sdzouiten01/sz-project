#ifndef MATH_TOOLS_H
#define MATH_TOOLS_H
#include <cmath>
#define PI 3.14159265
#define UN_OVER_SQRT_2PI 0.398942280401433
/**
* namespace used for some mathematics functions
*/

//using namespace std;

namespace Math_Tools
{
	/** Returns the maximum between a and b*/
	double maxim(double a, double b);
	/** Returns the minimum between a and b*/
	double minim(double a, double b);
	/** Classic d0 function used in BS formula)*/
	double d0(double t,double x,double y,double sigma);
	/** Classic d1 function used in BS formula)*/
	double d1(double t,double x,double y,double sigma);
	/** Cumulative distribution of normal law N(0,1)*/
	double N(double x);
	/** Density of N(0,1)*/
	double DiffN(double x);
	/** Gamma of a put in a BS model*/
	double gamma_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma);
	/** delta of a put in a BS model*/
	double delta_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma);
	/** rho of a put in a BS model*/
	double rho_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma);
	/** Price of a put in a BS model*/
	double put_price_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma);
	//double convert_rate(double frequency,double annual_rate);

	double swap_price(double** annual_zero_coupons_rates,
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
}

#endif