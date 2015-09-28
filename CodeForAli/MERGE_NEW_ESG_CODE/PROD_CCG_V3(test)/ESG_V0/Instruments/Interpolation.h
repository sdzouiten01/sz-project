#ifndef INTERPOLATE_H_
#define INTERPOLATE_H_



#include "Matrice.h"
class Interpolate{

public:
	static void Spline_Coefficients(double* &arrayT, double* &arrayY, int num,double* &h_coefficient, double* &z_coefficient/*, double slope_end*/);

	static void Total_Interpolate(double *input_YC_dates,double *input_YC_values,int input_YC_size,double *interpol_YC_dates,double *interpol_YC_values,
									  int interpol_YC_size,int interpolation_Type,int extrapolation_Type_Start,int extrapolation_Type_End );
	
	static double Total_Interpolate(double *input_YC_dates,double *input_YC_values,int input_YC_size,double interpol_YC_dates,
		int interpolation_Type,int extrapolation_Type_Start,int extrapolation_Type_End );

	static double Interpol(double* input_YC_dates,double* input_YC_values,int input_YC_size,double interpol_date,
							   int interpolation_type,double* h_coefficient,double* z_coefficient);

	static double Loglinear_Interp(double *arrayT,double *arrayY, int num, double term);

	static double Linear_Interp(double *arrayT,double *arrayY, int num, double term);

	static double Const_Interp(double *arrayT,double *arrayY, int num, double term);

	static double Spline_Interp(double* &arrayT, double* &arrayY, int num, double term,double* &h_coefficient, double* &z_coefficient);

};
#endif
