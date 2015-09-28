#ifndef CALIBRATION_H_
#define CALIBRATION_H_
#include "G1analytics.h"
#include "Interpolation.h"
#include "CalibSwapMarket.h"
#include "Matrice.h"
#include "LMnumerics.h"
#include <string>
#include <iostream>
using namespace std;

#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))


/*extern "C" __declspec(dllexport)  double __stdcall Calibration_1F( SAFEARRAY **Dates,SAFEARRAY **Rates,SAFEARRAY **VSwapExp, 
				SAFEARRAY **VTenor, SAFEARRAY **VSwapMatrix,SAFEARRAY **VWeightsMatrix,
				SAFEARRAY **SA_Param_init,SAFEARRAY **SA_Param_optim,SAFEARRAY **SA_Param_min,
				SAFEARRAY **SA_Param_max,SAFEARRAY **SA_CalibSwaption,
				SAFEARRAY **SA_ZC_Interpol,BSTR calib_type);*/

/**********************************************************************/
/**********************************************************************/

class Calibration{

public:	
	static void InitModelStruct(
	modelStruct *G1ModelStruct,
    double *pGuess,
	double *pMin,
	double *pMax,
	int nPar
	);

static void Mapping(
    double *x,
    modelStruct *ModStruct,
    int direction
    );

static void G1Model(
	double *results,
	int m,
	int n,
	string Calib_Param,
	int omega,
	double *pGuess,
	double *x,
	modelStruct *ModStruct,
	marketStruct *MrktStruct
	);

static double G1ModelSingle(
	int i,
	int n,   
	string Calib_Param,
	int omega,
	double *pGuess,
	double *x,
	modelStruct *ModStruct,
	marketStruct *MrktStruct
	);
/*static void Optimisation_LM(modelStruct *ModelStruct,marketStruct *SwapMrktStruct,double *dateCurve,double *zeroCurve,double *SwapExp,double *SwapTenor,
	double **SwapMatrix,double **WeightsMatrix, int nb_dates,int nExp,int nTenor,double K,double *Param_init,
	double *pMin,double *pMax,double *&Param_optim_out, double **VolSwaption_out,
	double *&PriceSwaption_out,double *&ZC_interpol,string &Calib_param,string &Error_Type,int omega,double &LiborMat,double *RMSFit_out);*/
static void Optimisation_LM(double *dateCurve,double *zeroCurve,int interpolation_type,int extrapolation_type_start,int extrapolation_type_end,
	double *SwapExp,double *SwapTenor,double **SwapMatrix,double **WeightsMatrix, int nb_dates,int nExp,int nTenor,double K,double *Param_init,
	double *pMin,double *pMax,double *&Param_optim_out, double **VolSwaption_out,
	double *&PriceSwaption_out,double *&ZC_interpol,string &Calib_param,string &Error_Type,int omega,double &LiborMat,double *RMSFit_out, int HE_mode_calibration, double* swap_curve,int last_model_date,bool new_interpolation, int delta_t, bool boot_sec);
};
#endif
