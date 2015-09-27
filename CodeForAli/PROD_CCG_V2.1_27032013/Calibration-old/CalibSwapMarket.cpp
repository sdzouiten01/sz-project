#include "CalibSwapMarket.h" 

void CalibSwapMarket::InitMrktStruct(marketStruct *SwapMrktStruct,double **SwapMatrix,double **WeightsMatrix,double *SwapExp,double *SwapTenor,double LiborMat,
	int nsExp,int nsTenor,int omega,double *dateCurve,double *zeroCurve,int nCurve,int interpolation_type,int extrapolation_type_start,int extrapolation_type_end,double K, double* swap_curve, bool new_interpolation, bool boot_sec)
{
	int i,j;
	int count=0;
	for (i=0; i<nsExp; i++) {
		for (j=0; j<nsTenor; j++) {
			SwapMrktStruct->MarketData[0][count] = SwapMatrix[i][j];
			SwapMrktStruct->MarketData[1][count] = 1.;
			SwapMrktStruct->MarketData[2][count] = 0.;
			SwapMrktStruct->MarketData[3][count] = SwapExp[i];
			SwapMrktStruct->MarketData[4][count] = SwapExp[i] + SwapTenor[j];
			SwapMrktStruct->MarketData[5][count] = LiborMat;
			SwapMrktStruct->MarketData[6][count] = WeightsMatrix[i][j];
			count++;
		}
	}
	double P1,P2,T1,T2,t,sum,x,Pt;
	bool strike=1;
	if(K<1) strike=0;
	for(int k=0;k<nCurve;k++){
		SwapMrktStruct->dateCurve[k]= dateCurve[k];
		SwapMrktStruct->zeroCurve[k]= zeroCurve[k];
	}
	
    SwapMrktStruct->TMax = SwapMrktStruct->nMax/12;
	double *initial_fwd = new double [SwapMrktStruct->nMax];
	
	if(new_interpolation == true){
		int frequence = int(1/LiborMat);
		Interpolation::set_forward_rate(SwapMrktStruct->nMax/12,SwapMrktStruct->nCurve,SwapMrktStruct->dateCurve,SwapMrktStruct->zeroCurve,
										interpolation_type,extrapolation_type_start,extrapolation_type_end,initial_fwd,frequence,swap_curve, boot_sec);
	}
	else{
		double frequence_old = 1/LiborMat;
		Interpolation::set_forward_rate(SwapMrktStruct->nMax,SwapMrktStruct->nCurve,SwapMrktStruct->dateCurve,SwapMrktStruct->zeroCurve,
									initial_fwd,frequence_old, boot_sec);
	}

	Interpolation::set_discount_factor(SwapMrktStruct->nMax,initial_fwd,SwapMrktStruct->discCurve);
	
	for (i=0; i<SwapMrktStruct->mD; i++) {
		T1 = SwapMrktStruct->MarketData[3][i];
		T2 = SwapMrktStruct->MarketData[4][i];
		if(strike){
			x = (T1/SwapMrktStruct->TMax)*SwapMrktStruct->nMax; j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
			P1 = SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
			x = (T2/SwapMrktStruct->TMax)*SwapMrktStruct->nMax;  j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
			P2 =SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
			sum = 0.;
			t = T1 + LiborMat;
			while (t<=T2*1.000001) {
				x = (t/SwapMrktStruct->TMax)*SwapMrktStruct->nMax;  j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
				Pt = SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
				sum += Pt;
				t = t + LiborMat;
			}
			K = (P1-P2)/(sum*LiborMat);
		}
		SwapMrktStruct->MarketData[2][i] = K;
		SwapMrktStruct->MarketData[7][i] = G1analytics::BlackES(SwapMrktStruct->MarketData[0][i],K,omega,T1,T2,
					LiborMat,SwapMrktStruct->discCurve,SwapMrktStruct->TMax,SwapMrktStruct->nMax);
		//13/05/09
		LiborMat=0.5;
		if(strike){
			x = (T1/SwapMrktStruct->TMax)*SwapMrktStruct->nMax; j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
			P1 = SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
			x = (T2/SwapMrktStruct->TMax)*SwapMrktStruct->nMax;  j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
			P2 =SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
			sum = 0.;
			t = T1 + LiborMat;
			while (t<=T2*1.000001) {
				x = (t/SwapMrktStruct->TMax)*SwapMrktStruct->nMax;  j = min((int)x, SwapMrktStruct->nMax-1); x = x-j;
				Pt = SwapMrktStruct->discCurve[j] + x*(SwapMrktStruct->discCurve[j+1] - SwapMrktStruct->discCurve[j]);
				sum += Pt;
				t = t + LiborMat;
			}
			K = (P1-P2)/(sum*LiborMat);
		}
		SwapMrktStruct->MarketData[2][i] = K;
		SwapMrktStruct->MarketData[5][i] = LiborMat;
		
	}
	delete [] initial_fwd;
}

void CalibSwapMarket::PostProcess(
	double *MarketFit,
	double **CalibSwap,
	double **SwapMatrix,
	int nsExp,
	int nsTenor
	)
{
	int k=0;
	for (int i=0; i<nsExp; i++) {
		for (int j=0; j<nsTenor; j++) {
			if (SwapMatrix[i][j]>0.) {
				CalibSwap[i][j] = SwapMatrix[i][j] + MarketFit[k];
				k = k + 1;
			}
		}
	}
}
