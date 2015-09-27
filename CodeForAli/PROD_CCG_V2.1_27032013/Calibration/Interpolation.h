#ifndef INTERPOLATION_H_
#define INTERPOLATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define maxKvec 500

class Interpolation{

public:

static double loglinear_interp(double *arrayT,double *arrayY, int num, double term);
static double linear_interp(double *arrayT,double *arrayY, int num, double term);
static double const_interp(double *arrayT,double *arrayY, int num, double term);


static void spline_coefficients(double* &arrayT, double* &arrayY, int num,
								double* &h_coefficient, double* &z_coefficient);

static double spline_interp(double* &arrayT, double* &arrayY, int num, double term,
							double* &h_coefficient, double* &z_coefficient);

static void set_forward_rate(int last_term,int number_yields, double* &terms , double* &terms_yields,
							 int interpolation_type, int extrapolation_type_start, int extrapolation_type_end,
							 double* &initial_fwd, int coupon_frequency, double* swap_curve, bool boot_sec, int delta_t=12);

static void fill_in_yields(int last_period, double frequency, int number_yields ,
								   double* &terms,double* &terms_yields, double* &temp_terms,double* &temp_yields);

static void strip_coupon(int last_term, int coupon_frequency, double gilt_adj, double* &terms_yields, double* init_terms, double* init_yields, bool boot_sec);

static void convert_to_forward(double* &terms_yields,double* &initial_fwd, int last_term );

static void set_discount_factor(int nmaturities, double* &initial_fwd,double* &initial_zcb, int delta_t=12);

static void total_interpolation(int number_yields,double * terms,double * terms_yields,int nb_interpol_dates,double * interpol_dates,double* interpol_curve,int extrapolation_type_start,int interpolation_type,int extrapolation_type_end);
static double interpol(int number_yields,double* terms,double * terms_yields,double interpol_dates,int interpolation_type, double* h_coefficient, double* z_coefficient); 


static void set_forward_rate(int last_term,int number_yields, double* &terms , double* &terms_yields,
							 double* &initial_fwd, double coupon_frequency, bool boot_sec);

static void strip_coupon(int last_term, int coupon_frequency, double gilt_adj, double* &terms_yields, bool boot_sec);

static void fill_in_yields_old(int last_period, double frequency, int number_yields ,
								   double* &terms,double* &terms_yields, double* &temp_terms,double* &temp_yields);

};
#endif
