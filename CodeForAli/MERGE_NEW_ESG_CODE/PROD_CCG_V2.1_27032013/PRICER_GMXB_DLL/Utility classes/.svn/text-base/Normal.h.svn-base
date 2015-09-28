#ifndef _NORMAL_H
#define _NORMAL_H
#include "stdafx.h"
#include <ymath.h>
#include <float.h>
#define LOW 0.02425
#define HIGH 0.97575
#include <math.h>
#include <errno.h>

class Normal
{
private:
	double esp;
	double var;

public :
	Normal(double e, double v)
	{
		this->esp = e;
		this->var = v;

	};

	//Code of cumulative normal distribution function
	//(c) Financial Numerical Recipes in C++, Bernt Arne Ødegaard
	//Abromowitz and Stegun polynomial approximation 
	
	double cumulativeDensity(double z)
	{
		if (z > 6.0)
			return 1.0;
		
		if (z < -6.0)
			return 0.0;

		double b1 = 0.31938153;
		double b2 = -0.356563782;
		double b3 = 1.781477937;
		double b4 = -1.821255978;
		double b5 = 1.330274429;
		double p = 0.2316419;
		double c2 = 0.3989423;

		double a = fabs(z);
		double t = 1.0 / (1.0 + a*p);
		double b = c2 * exp((-z)*(z/2.0));
		double n = ((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t;
		n = 1.0 - b*n;

		if (z < 0.0)
			n = 1.0 - n;
		
		return n;

	};

	double cumulativeDensity_2(const double x)
	{
	  const double b1 =  0.319381530;
	  const double b2 = -0.356563782;
	  const double b3 =  1.781477937;
	  const double b4 = -1.821255978;
	  const double b5 =  1.330274429;
	  const double p  =  0.2316419;
	  const double c  =  0.39894228;

	  if(x >= 0.0) 
	  {
		  double t = 1.0 / ( 1.0 + p * x );
		  return (1.0 - c * exp( -x * x / 2.0 ) * t *( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
	  }
	  else 
	  {
		  double t = 1.0 / ( 1.0 - p * x );
		  return ( c * exp( -x * x / 2.0 ) * t *( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
		}
	};


	double cumulativeDensity_3(double x)
	{
		double Cumnorm;
		double build;
		double resultat;

		double XAbs = fabs(x);

		if (XAbs > 37)
			resultat = 0.0;
		else
		{
			double Exponential = exp(-XAbs*XAbs/ 2);
			if (XAbs < 7.07106781186547)
			{
				build = 0.0352624965998911 * XAbs + 0.700383064443688;
				build = build * XAbs + 6.37396220353165;
				build = build * XAbs + 33.912866078383;
				build = build * XAbs + 112.079291497871;
				build = build * XAbs + 221.213596169931;
				build = build * XAbs + 220.206867912376;
				Cumnorm = Exponential * build;
				build = 8.83883476483184E-02 * XAbs + 1.75566716318264;
				build = build * XAbs + 16.064177579207;
				build = build * XAbs + 86.7807322029461;
				build = build * XAbs + 296.564248779674;
				build = build * XAbs + 637.333633378831;
				build = build * XAbs + 793.826512519948;
				build = build * XAbs + 440.413735824752;
				resultat = Cumnorm / build;
			}
			else
			{
				build = XAbs + 0.65;
				build = XAbs + 4 / build;
				build = XAbs + 3 / build;
				build = XAbs + 2 / build;
				build = XAbs + 1 / build;
				resultat = Exponential / build / 2.506628274631;
			}
		}
		
		if (x > 0)
			return (1 - resultat);
		else
			return resultat;
	};

	double cumulativeDensity_4(double u)
	{
	 const double a[5] = {
	  1.161110663653770e-002,3.951404679838207e-001,2.846603853776254e+001,
	  1.887426188426510e+002,3.209377589138469e+003
	 };
	 const double b[5] = {
	  1.767766952966369e-001,8.344316438579620e+000,1.725514762600375e+002,
	  1.813893686502485e+003,8.044716608901563e+003
	 };
	 const double c[9] = {
	  2.15311535474403846e-8,5.64188496988670089e-1,8.88314979438837594e00,
	  6.61191906371416295e01,2.98635138197400131e02,8.81952221241769090e02,
	  1.71204761263407058e03,2.05107837782607147e03,1.23033935479799725E03
	 };
	 const double d[9] = {
	  1.00000000000000000e00,1.57449261107098347e01,1.17693950891312499e02,
	  5.37181101862009858e02,1.62138957456669019e03,3.29079923573345963e03,
	  4.36261909014324716e03,3.43936767414372164e03,1.23033935480374942e03
	 };
	 const double p[6] = {
	  1.63153871373020978e-2,3.05326634961232344e-1,3.60344899949804439e-1,
	  1.25781726111229246e-1,1.60837851487422766e-2,6.58749161529837803e-4
	 };
	 const double q[6] = {
	  1.00000000000000000e00,2.56852019228982242e00,1.87295284992346047e00,
	  5.27905102951428412e-1,6.05183413124413191e-2,2.33520497626869185e-3
	 };
	 register double y, z;

	 /*if (_isnan(u))
	  return _Nan._D;*/
	 if (!_finite(u))
	  return (u < 0 ? 0.0 : 1.0);
	 y = fabs(u);
		if (y <= 0.46875*1.41421356237309504880) {
	  /* evaluate erf() for |u| <= sqrt(2)*0.46875 */
	  z = y*y;
	  y = u*((((a[0]*z+a[1])*z+a[2])*z+a[3])*z+a[4])
		   /((((b[0]*z+b[1])*z+b[2])*z+b[3])*z+b[4]);
	  return 0.5+y;
	 }
	 z = exp(-y*y/2)/2;
	 if (y <= 4.0) {
	  /* evaluate erfc() for sqrt(2)*0.46875 <= |u| <= sqrt(2)*4.0 */
	  y = y/1.41421356237309504880;
	  y = ((((((((c[0]*y+c[1])*y+c[2])*y+c[3])*y+c[4])*y+c[5])*y+c[6])*y+c[7])*y+c[8])
		/((((((((d[0]*y+d[1])*y+d[2])*y+d[3])*y+d[4])*y+d[5])*y+d[6])*y+d[7])*y+d[8]);

	  y = z*y;
	 } else {
	  /* evaluate erfc() for |u| > sqrt(2)*4.0 */
	  z = z*1.41421356237309504880/y;
	  y = 2/(y*y);
			y = y*(((((p[0]*y+p[1])*y+p[2])*y+p[3])*y+p[4])*y+p[5])
		/(((((q[0]*y+q[1])*y+q[2])*y+q[3])*y+q[4])*y+q[5]);
			y = z*(0.564189583547756286948-y);
	 }
	 return (u < 0.0 ? y : 1-y);
	};

	//double ltqnorm(double p)
	//{
	/* Coefficients in rational approximations. */
	/* const double a[] =
	{
		-3.969683028665376e+01,
		 2.209460984245205e+02,
		-2.759285104469687e+02,
		 1.383577518672690e+02,
		-3.066479806614716e+01,
		 2.506628277459239e+00
	};

	const double b[] =
	{
		-5.447609879822406e+01,
		 1.615858368580409e+02,
		-1.556989798598866e+02,
		 6.680131188771972e+01,
		-1.328068155288572e+01
	};

	const double c[] =
	{
		-7.784894002430293e-03,
		-3.223964580411365e-01,
		-2.400758277161838e+00,
		-2.549732539343734e+00,
		 4.374664141464968e+00,
		 2.938163982698783e+00
	};

	const double d[] =
	{
		7.784695709041462e-03,
		3.224671290700398e-01,
		2.445134137142996e+00,
		3.754408661907416e+00
	};*/
	//	
	//	double q, r;

	//	errno = 0;

	//	if (p < 0 || p > 1)
	//	{
	//		errno = EDOM;
	//		return 0.0;
	//	}
	//	else if (p == 0)
	//	{
	//		errno = ERANGE;
	//		return -HUGE_VAL /* minus "infinity" */;
	//	}
	//	else if (p == 1)
	//	{
	//		errno = ERANGE;
	//		return HUGE_VAL /* "infinity" */;
	//	}
	//	else if (p < LOW)
	//	{
	//		/* Rational approximation for lower region */
	//		q = sqrt(-2*log(p));
	//		return (((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
	//			((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
	//	}
	//	else if (p > HIGH)
	//	{
	//		/* Rational approximation for upper region */
	//		q  = sqrt(-2*log(1-p));
	//		return -(((((c[0]*q+c[1])*q+c[2])*q+c[3])*q+c[4])*q+c[5]) /
	//			((((d[0]*q+d[1])*q+d[2])*q+d[3])*q+1);
	//	}
	//	else
	//	{
	//		/* Rational approximation for central region */
 //   			q = p - 0.5;
 //   			r = q*q;
	//		return (((((a[0]*r+a[1])*r+a[2])*r+a[3])*r+a[4])*r+a[5])*q /
	//			(((((b[0]*r+b[1])*r+b[2])*r+b[3])*r+b[4])*r+1);
	//	}
	//};

	~Normal()
	{
	}

};



#endif