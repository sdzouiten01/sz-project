#ifndef CALIBSWAPMARKET_H_
#define CALIBSWAPMARKET_H_
#include "Interpolation.h"
#include "G1analytics.h"
#include <iostream>
using namespace std;
#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))




class marketStruct{
public:int mD;
	double **MarketData;
	double *dateCurve;
	double *zeroCurve;
    int nCurve;
	double *discCurve;
    double TMax;
    int nMax;
	marketStruct(){}
	marketStruct(int ncurve,int nmax, int n){
		nCurve=ncurve;
		//cout << nCurve << endl;
		nMax=nmax;
		mD=n;
		MarketData=new double* [8];
		for(int i=0;i<8;i++){
			MarketData[i]=new double [mD];
		}
		dateCurve=new double [ncurve];
		zeroCurve=new double [ncurve];
		discCurve=new double [nmax+1];
	}
	marketStruct(marketStruct &a){
		mD=a.mD;
		nCurve=a.nCurve;
		nMax=a.nMax;
		TMax=a.TMax;
		dateCurve=new double[nCurve];
		zeroCurve=new double[nCurve];
		discCurve=new double[nMax+1];
		for(int i=0;i<nCurve;i++){
			dateCurve[i]=a.dateCurve[i];
			zeroCurve[i]=a.zeroCurve[i];
		}
		for(int i=0;i<nMax+1;i++){
			discCurve[i]=a.discCurve[i];
		}
		MarketData=new double* [8];
		for(int i=0;i<8;i++){
			MarketData[i]=new double [mD];
			for(int j=0;j<mD;j++){
				MarketData[i][j]=a.MarketData[i][j];
			}
		}
	}
	~marketStruct(){
		delete [] dateCurve;
		delete [] zeroCurve;
		delete [] discCurve;
		for(int i=0;i<8;i++){
			delete [] MarketData[i];
		}
		delete []MarketData;
	}
};

class modelStruct{
public:double *pMin;
	double *pMax;
	int nPar;
	modelStruct(){
		pMin=new double [4];
		pMax=new double [4];
		for(int i=0;i<4;i++){
			pMin[i]=0;
			pMax[i]=0;
		}  
	}
	modelStruct(modelStruct & a){
		pMin=new double [4];
		pMax=new double [4];
		for(int i=0;i<4;i++){
			pMin[i]=a.pMin[i];
			pMax[i]=a.pMax[i];
		}
		nPar=a.nPar;
	}
	~modelStruct(){
		delete [] pMin;
		delete [] pMax;	
	}

};


class CalibSwapMarket{


public:
	static void InitMrktStruct(
	marketStruct *SwapMrktStruct,
	double **SwapMatrix,
	double **WeightsMatrix,
	double *SwapExp,
	double *SwapTenor,
	double LiborMat,
	int nsExp,
	int nsTenor,
	int omega,
	double *dateCurve,
	double *zeroCurve,
    int nCurve,
	int interpolation_type,
	int extrapolation_type_start,
	int extrapolation_type_end,
	double K,
	double* swap_curve,
	bool new_interpolation,
	bool boot_sec
	);

static void PostProcess(
	double *MarketFit,
	double **CalibSwap,
	double **SwapMatrix,
	int nsExp,
	int nsTenor
	);

};
#endif

