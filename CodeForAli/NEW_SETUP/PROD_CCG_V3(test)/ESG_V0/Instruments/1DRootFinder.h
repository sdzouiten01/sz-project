#ifndef _1DROOTFINDER_H
#define _1DROOTFINDER_H

//#include "Analytics.h"
#define EPSILON 1.E-6
#define _ERROR -1000000.0

enum RootFindingMethod
{
	BISSECTRICE,
	ROOT_NEWTON,
	LEVENBERG,
};

template<typename OBJECTIVEFUNCTION>
class _1DRootFinder
{
protected:
	RootFindingMethod   myMethod;		//! Method for finding the solution to the system of equations
	long   myMaxEval;		//! Maximum number of function evaluations
	double myLowBound;
	double myHighBound;		//! Lower and upper bound for solution
	double myAccuracy;		//! Sets the accuracy for the root finding, i.e., x is considered a zero if  |f(x)| < accuracy). 
	long   myIterations;	//! Number of iterations required to get the solution.

public:
	_1DRootFinder(RootFindingMethod m = BISSECTRICE) : myMaxEval(50), myAccuracy(EPSILON / 100.), myMethod(m){}

	double solve(const OBJECTIVEFUNCTION& f,double& guess,double xMin,double xMax);
	double solve2(double r);

private:
	double rtbis(const OBJECTIVEFUNCTION& func, const double xMin, const double xMax, const double xAcc);
	double rtnewt(const OBJECTIVEFUNCTION& func, const double xMin, const double xMax, const double xAcc);
	bool zbrac(const OBJECTIVEFUNCTION& func, double& x1, double& x2);

};


template<typename OBJECTIVEFUNCTION>
double _1DRootFinder<OBJECTIVEFUNCTION>::solve2(double r)
{
		return r;
}

template<typename OBJECTIVEFUNCTION>
double _1DRootFinder<OBJECTIVEFUNCTION>::solve(const OBJECTIVEFUNCTION& f, double& guess,double xMin, double xMax)
{
	// Bissection by default
	if(myMethod == BISSECTRICE)
	{
		bool success = zbrac(f, xMin, xMax);
		if(!success)
		{
			//continue_with_msg("Solution not bracketed");
			return guess;
		}
		return rtbis(f,xMin,xMax,myAccuracy);
	}
	else if(myMethod == ROOT_NEWTON)
	{
		return rtnewt(f,xMin,xMax,myAccuracy);
	}
 	else return _ERROR;
}
template<typename OBJECTIVEFUNCTION>
double _1DRootFinder<OBJECTIVEFUNCTION>::rtnewt(const OBJECTIVEFUNCTION& func, const double x1, const double x2, const double xAcc) 
{
	int JMAX = 20;
	int j;
	double df,dx,f,rtn;
	rtn = 0.5*(x1+x2); 
	for ( j = 1 ; j <= JMAX ; j++) 
	{
		f = func(rtn);
		df = func.derivate(rtn);
		dx=f/df;
		rtn -= dx;
		if ((x1-rtn)*(rtn-x2) < 0.0)
		Analytics::terminate_with_msg("Jumped out of brackets in rtnewt");
		if (fabs(dx) < xAcc) return rtn; 
	}
	Analytics::terminate_with_msg("Maximum number of iterations exceeded in rtnewt");
	return 0.0; 
}

template<typename OBJECTIVEFUNCTION>
bool _1DRootFinder<OBJECTIVEFUNCTION>::zbrac(const OBJECTIVEFUNCTION& func, double& x1, double& x2)
{
	const int NTRY=50;
	const double FACTOR = 1.6;
	int j;
	double f1,f2;

	if (x1 == x2) Analytics::terminate_with_msg("Bad initial range in zbrac");

	f1=func(x1);
	f2=func(x2);
	for (j=0;j<NTRY;j++) 
	{
		if (f1*f2 < 0.0) return true;
		if (fabs(f1) < fabs(f2))
			f1=func(x1 += FACTOR*(x1-x2));
		else
			f2=func(x2 += FACTOR*(x2-x1));
	}
	return false;
}

template<typename OBJECTIVEFUNCTION>
double _1DRootFinder<OBJECTIVEFUNCTION>::rtbis(const OBJECTIVEFUNCTION& func, const double x1, const double x2, const double xAcc) 
{
	const int JMAX=myMaxEval;
	int j;
	double dx,f,fmid,xmid,rtb;

	f=func(x1);
	fmid=func(x2);
	if (f*fmid >= 0.0) // THROW("Root must be bracketed for bisection in rtbis")
		Analytics::terminate_with_msg("Root must be bracketed for bisection in rtbis");
	rtb = f < 0.0 ? (dx=x2-x1,x1) : (dx=x1-x2,x2);
	for (j=0;j<JMAX;j++) {
		fmid=func(xmid=rtb+(dx *= 0.5));
		if (fmid <= 0.0) rtb=xmid;
		if (fabs(dx) < xAcc || fmid == 0.0)
		{
			myIterations=j;
			return rtb;
		}
	}
	//THROW("Too many bisections in rtbis");
	Analytics::terminate_with_msg("Too many bisections in rtbis");
	return _ERROR;
}
#endif


