#include "Calibration.h"
#include "GMXBParameters.h"
#include "GMXBException.h"
#include <math.h>
#include <string.h>

using namespace std;

void Calibration::Optimisation_LM(double *dateCurve,
								  double *zeroCurve,
								  int interpolation_type,
								  int extrapolation_type_start,
								  int extrapolation_type_end,
								  double *SwapExp,
								  double *SwapTenor,
								  double **SwapMatrix,
								  double **WeightsMatrix,
								  int n_dates,
								  int nExp,
								  int nTenor,
								  double K,
								  double *pGuess,
								  double* pMin,
								  double *pMax,
								  double *&x,
								  double** VolSwaption,
								  double *&PriceSwaption,
								  double *&ZC_Interpol,
								  string &Calib_param,
								  string &Error_Type,
								  int omega,
								  double &LiborMat,
								  double *RMSFit_out,
								  int HE_mode_calibration,
								  double* swap_curve,
								  int last_model_date,
								  bool new_interpolation,
								  int delta_t,
								  bool boot_sec)
{
	int nPar = 2;

	if(Calib_param == "a and sigma") {
	    nPar = 2;
    }
    else{
		if( Calib_param == "sigma" || Calib_param == "a" ){
			nPar = 1; 
		}
    }

	int nMax = last_model_date*12;
	
/****************    initialisation de la structure de marché  *************************/ 
	marketStruct SwapMrktStruct(n_dates,nMax,nExp*nTenor);
	CalibSwapMarket::InitMrktStruct(&SwapMrktStruct,SwapMatrix,WeightsMatrix,SwapExp,SwapTenor,LiborMat,nExp,
									nTenor,omega,dateCurve,zeroCurve,n_dates,interpolation_type,
									extrapolation_type_start,extrapolation_type_end,K,swap_curve,new_interpolation, boot_sec);
	for (int i=0; i<nMax; i++)
	{
		ZC_Interpol[i] = (pow(SwapMrktStruct.discCurve[i+1],-12./(i+1))-1)*100.;
    }
	if(HE_mode_calibration==0 && delta_t!=12){
		int nMax_bis = last_model_date*delta_t;
		double *initial_fwd = new double [nMax_bis];
		int frequence = int(1/LiborMat);
		Interpolation::set_forward_rate(last_model_date,n_dates,dateCurve,zeroCurve,
										interpolation_type,extrapolation_type_start,extrapolation_type_end,initial_fwd,frequence,swap_curve,boot_sec,delta_t);
		
		double *discCurve = new double[nMax_bis+1] ;
		Interpolation::set_discount_factor(nMax_bis,initial_fwd,discCurve,delta_t);
		for (int i=0; i<nMax_bis; i++)
		{
			ZC_Interpol[i] = (pow(discCurve[i+1],-delta_t/(i+1.))-1)*100.;
		}
		delete[] initial_fwd;
		delete[] discCurve;
	}
	if(HE_mode_calibration==0)
	{
		return;	
	}
/****************    initialisation du model deux facteur *************************/ 
	modelStruct ModelStruct;
	double epsilon = 0.01 ; 
	for ( int i = 0 ; i< 2 ; i++)
	{
	     pMin[i] = pMin[i]*(1.- epsilon);
         pMax[i] = pMax[i]*(1+epsilon);
	}
	Calibration::InitModelStruct(&ModelStruct,pGuess,pMin,pMax,nPar);
	for(int k=0;k<nPar;k++){
        if ( Calib_param == "a and sigma" || Calib_param == "sigma") 
		{
			x[k] = pGuess[k];
	    }
	    else {  
			 if( Calib_param == "a"){  // nPar = 1
	         x[k] = pGuess[1];
		     }
		}	
	}

/**************************************************************************************/  
/**************          calibration du model un facteur				 **************/ 
/**************************************************************************************/
	Mapping(x,&ModelStruct,1);
	int type;
	if (Error_Type == "volatility") 
	{
	   type = 0 ;
	}
	else if (Error_Type == "price") 
	{
	  type = 1 ;
	}
	else if (Error_Type == "relative price") 
	{
	  type = 2 ;
	}
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error_Type value does not exist");
		throw new GMXBException(message);
	}

	// decalage d'indice de pGuess pour respecter la convention de lmdif
	pGuess[2]=pGuess[1];
	pGuess[1]=pGuess[0];
	pGuess[0]=0.;

	int maxit =1000;
	int info=0;
	int nfev=0;
	double ftol = 1E-16;
	double xtol = 1E-16;
	double *res = new double[nExp*nTenor];
	double *fvec =new double[nExp*nTenor+1];
	double *work = new double[(nPar+2)*(nExp*nTenor+7)];
	for(int i=0; i<nExp*nTenor; i++){
		res[i]=0.0;
	}
	for(int i=0; i<nExp*nTenor+1; i++){
		fvec[i]=0.0;
	}
	for(int i=0; i<(nPar+2)*(nExp*nTenor+7); i++){
		work[i]=0.0;
	}

	LMnumerics::lmdif(Calibration::G1Model,
					  &ModelStruct,
					  &SwapMrktStruct,
					  nExp*nTenor,
					  type,
					  Calib_param,
					  omega,
					  nPar,
					  pGuess,
					  x,
					  fvec,
					  ftol,
					  xtol,
					  maxit,
					  info,
					  &nfev,
					  work);
	
	for(int i =0; i<nPar;i++){
		x[i]= x[i+1];
	}
	x[nPar]=0.;

	Calibration::Mapping(x,&ModelStruct,1);
	Calibration::G1Model(res,nExp*nTenor,0,Calib_param,omega,pGuess,x,&ModelStruct,&SwapMrktStruct);
	Calibration::Mapping(x,&ModelStruct,-1);

	// VolSwaption contient les vol de swaptions avec les paramètres optimaux
	CalibSwapMarket::PostProcess(res,VolSwaption,SwapMatrix,nExp,nTenor);

	// PriceSwaption contient les prix de swaptions avec les paramètres optimaux
	Calibration::Mapping(x,&ModelStruct,1);		
	Calibration::G1Model(PriceSwaption,nExp*nTenor,3,Calib_param,omega,pGuess,x,&ModelStruct,&SwapMrktStruct);
	Calibration::Mapping(x,&ModelStruct,-1);

	// Calcul de l'erreur sur les prix
	Calibration::Mapping(x,&ModelStruct,1);		
	Calibration::G1Model(res,nExp*nTenor,1,Calib_param,omega,pGuess,x,&ModelStruct,&SwapMrktStruct);
	Calibration::Mapping(x,&ModelStruct,-1);
	double e=pow(LMnumerics::enorm(nExp*nTenor,res),2);
	*RMSFit_out=e;

	for(int i =0; i<2;i++){
		pGuess[i]= pGuess[i+1];
	}
	if ( Calib_param == "sigma" ){ 
	   x[1] = pGuess[1];
	}
    if ( Calib_param == "a" ){ 
	   x[1] = x[0];
	   x[0] = pGuess[0];
	}
	for ( int i = 0 ; i< 2 ; i++)
	{
	     pMin[i] = pMin[i]/(1.- epsilon);
         pMax[i] = pMax[i]/(1+epsilon);
	}

	delete [] res;
	delete [] fvec;
	delete [] work;

	if(delta_t!=12){
		int nMax_bis = last_model_date*delta_t;
		double *initial_fwd = new double [nMax_bis];
		int frequence = int(1/LiborMat);
		Interpolation::set_forward_rate(last_model_date,n_dates,dateCurve,zeroCurve,
										interpolation_type,extrapolation_type_start,extrapolation_type_end,initial_fwd,frequence,swap_curve,boot_sec,delta_t);
		double *discCurve = new double[nMax_bis+1] ;
		Interpolation::set_discount_factor(nMax_bis,initial_fwd,discCurve,delta_t);
		for (int i=0; i<nMax_bis; i++)
		{
			ZC_Interpol[i] = (pow(discCurve[i+1],-delta_t/(i+1.))-1)*100.;
		}
		delete[] initial_fwd;
		delete[] discCurve;
	}
}


// function to initialize G1 model struct
   void Calibration::InitModelStruct(
	modelStruct *G1ModelStruct,
    double *pGuess,
	double *pMin,
	double *pMax,
	int nPar
	)
{
	for(int i=0;i<4;i++){
		G1ModelStruct->pMin[i] = pMin[i];
		G1ModelStruct->pMax[i] = pMax[i];
	}
    G1ModelStruct->nPar =nPar;

    pGuess[0] = max(0.99*pMin[0]+0.01*pMax[0], min(0.99*pMax[0]+0.01*pMin[0], pGuess[0]));
    pGuess[1] = max(0.99*pMin[1]+0.01*pMax[1], min(0.99*pMax[1]+0.01*pMin[1], pGuess[1]));
}

/**********************************************************************/
/**************************   Mapping     *****************************/
/**********************************************************************/

// function to map between physical and calibration parameters
void Calibration::Mapping(
    double *x,
    modelStruct *ModStruct,
    int direction
    )
{
	double *pMin,*pMax,y;
    int i,nPar;
    nPar = ModStruct->nPar;
	pMin=new double [nPar];
	pMax=new double [nPar];
	for(int i=0;i<nPar;i++){
		pMin[i]=ModStruct->pMin[i];
		pMax[i]=ModStruct->pMax[i];
	}
		

    // backward direction uses tanh transformation
    // maps -Inf <= x <= Inf to pMin <= x <= pMax

	if (direction==-1) {
        for (i=0; i<=nPar-1; i++) {
	        x[i] = 0.5*(pMin[i]+pMax[i]) + 0.5*(pMax[i]-pMin[i])*tanh(x[i]);
        }
    // forward direction atanh (doesn't exist in C, so use logs)
    // maps pMin <= x <= pMax to -Inf <= x <= Inf
    }else{
		double epsilon = 0.01;
		double z1,z2 ; 
        for (i=0; i<=nPar-1; i++) {
			double lim_inf = pMin[i]/(1-epsilon); 
			double lim_sup = pMax[i]/(1+epsilon);
            y = (x[i] - 0.5*(pMin[i]+pMax[i]))/(0.5*(pMax[i]-pMin[i]));    
			z1 = (lim_inf - 0.5*(pMin[i]+pMax[i]))/(0.5*(pMax[i]-pMin[i]));
			z2 = (lim_sup - 0.5*(pMin[i]+pMax[i]))/(0.5*(pMax[i]-pMin[i]));
			double y2=y;
			y = min(max(y,z1),z2);
			if(y2!=y){cout << "danger : bounds for calibration parameters were reached" << endl;}
            x[i] = 0.5*log((1.+y)/(1.-y));
        }
    }
	delete [] pMin;
	delete [] pMax;
}


/**********************************************************************/
/********************     G1Model    ***************************/
/**********************************************************************/
// G1 model valuation function; calls G1ModelSingle
void Calibration::G1Model(
	double *results,
	int m,
	int n,
	string Calib_Param,
	int omega,
	double *pGuess,
	double *x,
	modelStruct *ModStruct,
	marketStruct *MrktStruct)
{
	for(int i=0; i<m; i++){
		results[i] = G1ModelSingle(i,n,Calib_Param,omega,pGuess,x,ModStruct,MrktStruct);
	}
}

/**********************************************************************/
/********************     G1ModelSingle     ***************************/
/**********************************************************************/


double Calibration::G1ModelSingle(
	int i,
	int n,
	string Calib_Param,
	int omega,
	double *pGuess,
	double *x,
	modelStruct *ModStruct,
	marketStruct *MrktStruct
	)
{
	int flag,nMax;
	double model,sigma1,kappa1,K,T1,T2,dT,V,mrktVol,wt,TMax;
	
    mrktVol = MrktStruct->MarketData[0][i];
    K  = MrktStruct->MarketData[2][i];
	T1 = MrktStruct->MarketData[3][i];
	T2 = MrktStruct->MarketData[4][i];
	dT = MrktStruct->MarketData[5][i];
	wt = MrktStruct->MarketData[6][i];
    nMax = MrktStruct->nMax;
    TMax = MrktStruct->TMax;

	if (n!=3&&wt==0.) return 0.;
	
	Mapping(x,ModStruct,-1);
	
	if (Calib_Param == "a and sigma") 
	{
		sigma1 = x[0];
		kappa1 = x[1];
	}
	else { 
		if(Calib_Param == "sigma"){
			sigma1 = x[0];
			kappa1 = pGuess[2];  // indice a été décalé pour respecter la convention de lmdif
		}
		else if(Calib_Param == "a"){
			sigma1 = pGuess[1];  // indice a été décalé pour respecter la convention de lmdif
			kappa1 = x[0];
		}
		else{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "List of Calibration Parameters incorrect");
			throw new GMXBException(message);	
		}
	}
	
    Mapping(x,ModStruct,1);
	
	V = G1analytics::G1ESwaption(T1,T2,dT,K,omega,sigma1,kappa1,MrktStruct->discCurve,TMax,nMax);

    if(n == 0)  // différence des volatilités des swaptions
	{
		// Black's implied vol
		flag = G1analytics::impvolBlack(omega,&model,V,T1,T2,dT,K,MrktStruct->discCurve,TMax,nMax);
		return (model - mrktVol)*wt;
	}
    if(n == 1)   // différence des prix des swaptions
	{
		double MarketPrice = MrktStruct->MarketData[7][i];
		return (V-MarketPrice)*wt;
	}
	if(n == 2)   // différence relative des prix des swaptions
	{
		double MarketPrice = MrktStruct->MarketData[7][i];
		return (V-MarketPrice)*wt/MarketPrice;
	}
	return V;
}


