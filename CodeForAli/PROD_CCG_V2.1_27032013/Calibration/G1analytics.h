#ifndef G1ANALYTICS_H_
#define G1ANALYTICS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**********************************************************************/
// G1: One factor Gaussian short rate model
/**********************************************************************/

class G1analytics{
public:
	G1analytics(){};
	~G1analytics();

public:
	
static double  G1ESwaption(
	double T1,
	double T2,
    double dT,
    double K,
    double omega,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	);

static double G1analytics::G1BondOpt(
	double x1,
	double t,
	double T1,
	double T2,
    double K,
    double omega,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	);

static double G1SwapRate(
	double x1,
	double t,
	double *T,
	int	nT,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	);

static double G1ZCB(
	double x1,
	double t,
	double T,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	);
	
static double G1VtTfunc(
	double t,
	double T,
	double sigma1,
	double kappa1
	);

 
static double BlackES(
	double sigma,
    double K,
	double omega,
	double T1,
	double T2,
    double dT,
	double *discCurve,
    double TMax,
    int nMax
    );

public:
	static int impvolBlack(
		int omega,
	double *sigimp,
	double market,
	double T1,
	double T2,
    double dT,
	double K,
	double *discCurve,
    double TMax,
    int nMax
	);


static double funcB(
	double a,
	double t,
	double T
	);

static double taux_court_Fwd(
	double *discMRKT,
	double t,
	double TMax,
	int nMax);

static double func_phi(
	double *discMRKT,
	double t,
	double TMax,
	int nMax,
	double kappa1,
	double sigma1);
};
#endif
