#ifndef LMNUMERICS_H_
#define LMNUMERICS_H_

#include <math.h>
#include <string>
#include "CalibSwapMarket.h"
using namespace std;

#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))

#define macheps 1.2e-16
#define tiny  1.0e-38
#define rtiny 3.834e-20
#define rHUGE 1.304e19



class LMnumerics{


public:
static void lmdif(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        int m,
		int type,
		string Calib_param,
		int omega,
        int n,
        double *pGuess,
        double *x,
        double *fvec,
        double ftol,
        double xtol,
        int maxit,
        int info,
        int *nfev,
        double *work
        );

static void lmpar(
        int n,
        double *r,
        int ldr,
        int *ipvt,
        double *diag,
        double *qtb,
        double delta,
        double *par,
        double *x,
        double *sdiag,
        double *wa1,
        double *wa2
        );
static void qrfac(
        int m,
        int n,
        double *a,
        int pivot,
        int *ipvt,
        double *rdiag,
        double *acnorm,
        double *wa
        );
static void qrsolv(
        int n,
        double *r,
        int ldr,
        int *ipvt,
        double *diag,
        double *qtb,
        double *x,
        double *sdiag,
        double *wa
        );
static double enorm(
        int n,
        double *x
        );
static void fdjac2(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        int m,
		int type,
		string Calib_param,
		int omega,
        int n,
		double *pGuess,
        double *x,
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        double *fvec,
        double *fjac,
        int *iflag,
        double epsfcn,
        double *wa
        );
static void fcn(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        int m,
		int type,
		string Calib_param,
		int omega,
		double *pGuess,
        double *x,
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        double *fvec
        );
};
#endif