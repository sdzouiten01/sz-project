#include "Analytics.h"
#include <iostream>


void Analytics::terminate_with_msg(const std::string& s)
{
	std::cout << s.c_str() << std::endl ;
	throw("Error. See log window");
}
void Analytics::terminate_without_msg(const std::string& s)
{
	throw("Error: " + s);
}
void Analytics::continue_with_msg(const std::string& s)
{
	std::cout << s.c_str() << std::endl ;
}
double Analytics::cnd(const double& x)
{
	double Epsilon=1.E-6;

	double CDFa=  0.2316419;
	double CDFb=  0.319381530;
	double CDFc= -0.356563782;
	double CDFd=  1.781477937;
	double CDFe= -1.821255978;
	double CDFf=  1.330274429;
	double CDFh=  0.3989422;   //1/sqrt(2PI)

	double k,temp;

	k=1./(1.+CDFa*fabs(x));
	if(x<-5.)
	{
		return 0.;
	}   
	else if(fabs(x)< Epsilon)
	{
		return 0.5;
	}
	else if(x>5.)
	{
		return 1.;
	}
	else
	{
		double g=k*k;
		temp=1.-CDFh*exp(-x*x/2)*(CDFb*k+CDFc*g+CDFd*g*k+CDFe*g*g+CDFf*g*g*k);
		if(x>0.)
		{
			return temp;
		}
		else
		{
			return (1.-temp);
		}
	}
}
double Analytics::optionPriceBlack(const double& fwd, const double& K, const double& sigma, const double& mat, const double& df, std::string& opt)
{

	if(K <= 0 || sigma <= 0.0 || mat <= 0.0) return df * (opt == "CALL" ? MAX(fwd - K,0.) : MAX(K - fwd,0.) );

	double putcall = (opt == "CALL" ? 0.0 : 1.0);

	double vol = sigma * sqrt(mat);
    double h1 = log(fwd / K) / vol + 0.5 * vol;
    double h2 = h1 - vol;
    double n1 = cnd(h1);
    double n2 = cnd(h2);
	double price = fwd * (n1 - putcall) - K * (n2 - putcall) ;
	return price * df;
}

// local functor to solve the volatility root finding issue
class locPriceFunction
{
protected:
	double myFwd;
	double myK;
	double myPrice;
	double myMat;
	double myDF;
	std::string& myOptType;

public:
	locPriceFunction(const double& fwd, const double& K, const double& price,const double& mat, const double& df, std::string& opt) 
		: myFwd(fwd), myK(K), myPrice(price), myMat(mat), myDF(df), myOptType(opt) {}
	
	locPriceFunction & operator=( const locPriceFunction & ) {}
	double operator()(const double& x) const;
	double derivate(const double& s) const;
};

double locPriceFunction::operator()(const double& x) const
{
	double price = Analytics::optionPriceBlack(myFwd, myK, x, myMat, myDF, myOptType);
	return price - myPrice;
}

double locPriceFunction::derivate(const double& s) const
{
	double valueup = (*this)(s+EPSILON);
	double valuedo = (*this)(s-EPSILON);
	return  (valueup - valuedo) / 2. / EPSILON ;
}

double Analytics::fromPricetoVol(const double& fwd, const double& K,const double& price, const double& mat, const double& df,std::string& opt)
{
	locPriceFunction f(fwd, K, price, mat, df, opt);
	_1DRootFinder<locPriceFunction> aRootFinder;
	double guess = 0.2;
	double lowBound = 0.01;
	double highBound = 1.0;
	double result = aRootFinder.solve(f, guess, lowBound, highBound);
	return result;
}