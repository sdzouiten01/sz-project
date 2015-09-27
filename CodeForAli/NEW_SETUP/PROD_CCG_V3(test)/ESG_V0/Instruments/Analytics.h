#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <string>
//#include <vector>
//#include <iostream>

#include "../Instruments/1DRootFinder.h"
//#define PI 3.1415926535 
#define EPS 3.E-9 
#define EPSILON 1.E-6
#define _ERROR -1000000.0
#define MIN(a, b) ( (a) < (b) ? (a) : (b) )
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

namespace Analytics
{

	void terminate_with_msg(const std::string& s);
	void terminate_without_msg(const std::string& s);
	void continue_with_msg(const std::string& s);
	double cnd(const double& x);
	double optionPriceBlack(const double& fwd, const double& K, const double& sigma, const double& mat, const double& df, std::string& opt);
	double fromPricetoVol(const double& fwd, const double& K,const double& price, const double& mat, const double& df, std::string& opt);


};

#endif