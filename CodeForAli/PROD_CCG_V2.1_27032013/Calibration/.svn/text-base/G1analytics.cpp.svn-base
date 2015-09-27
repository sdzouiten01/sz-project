#include "G1analytics.h"
#include "ProbaUtilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <string.h>

#define MIN_RSTAR -5

#undef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))

#undef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))

/**********************************************************************/
// G1:  One factor Gaussian short rate model
/**********************************************************************/


double G1analytics::G1ESwaption(
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
	)
{


    double PM0T1,V0T1,PM0ti,VT1ti,V0ti,sum1,rstarnew,sumf,sumfp,dsum,rstar,Xi;
	int Nt;
	Nt = (int)((T2-T1)/dT + 0.00001);
	double *ti=new double [Nt];
	double *ci=new double [Nt];
	double *AT1ti=new double [Nt];
	double *BT1ti=new double [Nt];
    int i,iter,nt;
	double dt, xt, ft;

	//10/04/09
	//double fwd,PM0T1_tmp;
	double B0T1;
	B0T1=(1.-exp(-kappa1*T1))/kappa1;
    
    for (i=0; i<Nt; i++) {ti[i] = T1 + (i+1)*dT; ci[i] = K*dT;} ci[Nt-1] += 1.;


	dt = TMax/(double)nMax;
	xt = T1/dt;	nt = min((int)xt, nMax-1);  ft = xt-nt;
	PM0T1 = ft*discMRKT[nt+1] + (1-ft)*discMRKT[nt];

	//10/04/09
	/*xt = (T1+dt)/dt;	nt = min((int)xt, nMax-1);  ft = xt-nt;
	PM0T1_tmp = ft*discMRKT[nt+1] + (1-ft)*discMRKT[nt];
	fwd=-(log(PM0T1_tmp)-log(PM0T1))/dt;*/

    V0T1 =  G1VtTfunc(0,T1,sigma1,kappa1);
    for (i=0; i<Nt; i++) {
        BT1ti[i] = (1.-exp(-kappa1*(ti[i]-T1)))/kappa1;

		xt = ti[i]/dt;	nt = min((int)xt, nMax-1);  ft = xt-nt;
		PM0ti = ft*discMRKT[nt+1] + (1-ft)*discMRKT[nt];

        VT1ti = G1VtTfunc(T1,ti[i],sigma1,kappa1);
        V0ti  = G1VtTfunc(0,ti[i],sigma1,kappa1);
        AT1ti[i] = (PM0ti/PM0T1)*exp(0.5*(VT1ti - V0ti + V0T1));
		//AT1ti[i] = (PM0ti/PM0T1)*exp((fwd+0.5*sigma1*sigma1*B0T1*B0T1)*BT1ti[i]+0.5*(VT1ti - V0ti + V0T1));
    }

    rstar = 0.;
    for (iter=1; iter<=100; iter++) {
        sumf = -1.;
        sumfp = 0.;
        for (i=0; i<Nt; i++) {
            dsum = ci[i]*AT1ti[i]*exp(-BT1ti[i]*rstar);
            sumf = sumf + dsum;
            sumfp = sumfp - dsum*BT1ti[i];
        }
        rstarnew = rstar - sumf/sumfp;
        if (fabs(rstarnew-rstar)<1.E-8*fabs(rstar)) break;
        rstar = max(MIN_RSTAR, rstarnew);
	}

    sum1 = 0.;
    for (i=0; i<Nt; i++) {
        Xi = AT1ti[i]*exp(-BT1ti[i]*rstar);
        sum1 = sum1 + ci[i]*G1BondOpt(0.,0.,T1,ti[i],Xi,-omega,sigma1,kappa1,discMRKT,TMax,nMax);
    }
	delete [] ti;
	delete [] ci;
	delete [] AT1ti;
	delete [] BT1ti;
    return sum1;

}

double G1analytics::G1BondOpt(
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
	)
{


	double SIGMA,P1,P2,h,BT1T2,BtT1,zbo;

    BT1T2 = (1. - exp(-kappa1*(T2-T1)))/kappa1;
    BtT1 = (1. - exp(-2*kappa1*(T1-t)))/(2.*kappa1);

	SIGMA = sigma1*BT1T2*sqrt(BtT1);
	P1 = G1ZCB(x1,t,T1,sigma1,kappa1,discMRKT,TMax,nMax);
	P2 = G1ZCB(x1,t,T2,sigma1,kappa1,discMRKT,TMax,nMax);
	h = log(P2/(P1*K))/SIGMA + 0.5*SIGMA;
	zbo = omega*(P2*CumNorm(omega*h) - K*P1*CumNorm(omega*(h-SIGMA)));

	return zbo;
}

// Not used
double G1analytics::G1SwapRate(
	double x1,
	double t,
	double *T,
	int	nT,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	)
{
	
	double sum1, PtA, PtB;
	int i;

    sum1 = 0.;
    for (i=1; i<=nT; i++) {
        sum1 = sum1 + (T[i]-T[i-1])*G1ZCB(x1,t,T[i],sigma1,kappa1,discMRKT,TMax,nMax);
    }
    PtA = G1ZCB(x1,t,T[0],sigma1,kappa1,discMRKT,TMax,nMax);
	PtB = G1ZCB(x1,t,T[nT],sigma1,kappa1,discMRKT,TMax,nMax);
	sum1=(PtA-PtB)/sum1;
	return sum1;
}


	
double G1analytics::G1ZCB(
	double x1,
	double t,
	double T,
	double sigma1,
	double kappa1,
	double *discMRKT,
	double TMax,
	int nMax	
	)
{

    double V0T,P0T,V0t,e1tT,VtT,P0t,ZB,A, dt, xT, fT,xt, ft;
	int nT,nt;

	dt = TMax/(double)nMax;
	e1tT = exp(-kappa1*(T-t));

    V0T = G1VtTfunc(0.,T,sigma1,kappa1);
    
	xT = T/dt;  nT = min((int)xT, nMax-1);  fT = xT-nT;	
	P0T = fT*discMRKT[nT+1] + (1-fT)*discMRKT[nT];
    if (t==0.) {
        V0t = 0.;
        VtT = V0T;
        P0t = 1.;
    } else {
		V0t = G1VtTfunc(0.,t,sigma1,kappa1);
        VtT = G1VtTfunc(t,T,sigma1,kappa1);
		xt = t/dt;	nt = min((int)xt, nMax-1);  ft = xt-nt;
        P0t = ft*discMRKT[nt+1] + (1-ft)*discMRKT[nt];	
	}
    
    A = 0.5*(VtT - V0T + V0t) - ((1-e1tT)/kappa1)*x1;
    ZB = (P0T/P0t)*exp(A);
    
    return ZB;
}


	
double G1analytics::G1VtTfunc(
	double t,
	double T,
	double sigma1,
	double kappa1
	)
{
	double c1,e1tT,d1tT;
	
    c1 = (sigma1/kappa1);
	e1tT = exp(-kappa1*(T-t));
	d1tT = c1*c1*((T-t) + 2/kappa1*e1tT - 0.5/kappa1*e1tT*e1tT - 1.5/kappa1);
    return d1tT;
}


double G1analytics::BlackES(
	double sigma,
    double K,
	double omega,
	double T1,
	double T2,
    double dT,
	double *discCurve,
    double TMax,
    int nMax
    )
{
	int nreset,j,i;
	double P1,P2,sum,s12,F,v,d1,d2,ES,t,x,Pt;

	nreset = (int)((T2-T1)/dT);
    x = (T1/TMax)*nMax;  i = min((int)x, nMax-1); x = x-i;
    P1 = discCurve[i] + x*(discCurve[i+1] - discCurve[i]);
    x = (T2/TMax)*nMax;  i = min((int)x, nMax-1); x = x-i;
    P2 = discCurve[i] + x*(discCurve[i+1] - discCurve[i]);

    sum = 0.;
	for (j=1; j<=nreset; j++) {
		t = T1 + j*dT;
        x = (t/TMax)*nMax;  i = min((int)x, nMax-1); x = x-i;
        Pt = discCurve[i] + x*(discCurve[i+1] - discCurve[i]);
		sum = sum + Pt;
	}
	s12 = (P1-P2)/(sum*dT);

	F = s12;
	v = sigma*sqrt(T1);
	d1 = (log(F/K) + 0.5*v*v)/v;
	d2 = d1 - v;
	ES = (sum*dT) * omega*(F*CumNorm(omega*d1) - K*CumNorm(omega*d2));

	return ES;
}

int G1analytics::impvolBlack(
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
	)
{
	int it,maxit,errcode;
	double Vlow,Vhi,Vnew,dsig,siglow,sighi,swap,signew;
	double sig1,sig2,errV,errsig;

	sig1 = 0.005;
	sig2 = 1.;
	errV = 1.e-08;
	errsig = 1.e-08;
	maxit = 100;

	int count = 0;

	Vlow = G1analytics::BlackES(sig1,K,omega,T1,T2,dT,discCurve,TMax,nMax) - market;
	Vhi  = G1analytics::BlackES(sig2,K,omega,T1,T2,dT,discCurve,TMax,nMax) - market;
	while((Vlow*Vhi>0||fabs(Vlow)<errV || fabs(Vhi)<errV||fabs(Vlow-Vhi) < errV)&&count<20){
		sig1*=0.5;
		sig2*=2;
		Vlow = G1analytics::BlackES(sig1,K,omega,T1,T2,dT,discCurve,TMax,nMax) - market;
		Vhi  = G1analytics::BlackES(sig2,K,omega,T1,T2,dT,discCurve,TMax,nMax) - market;
		count++;
	}
	if(Vlow*Vhi>0||fabs(Vlow)<errV || fabs(Vhi)<errV||fabs(Vlow-Vhi) < errV){
		*sigimp = 0.;
        errcode = 101;  
		return errcode;
	}

	if (Vlow < 0) {
		siglow = sig1;
		sighi = sig2;
	}
	else {
		siglow = sig2;
		sighi = sig1;
		swap = Vhi;
		Vhi = Vlow;
		Vlow = swap;
	}

	it = 0;
	dsig = sighi-siglow;
	signew = 0.5*(siglow+sighi);
	while ((fabs(dsig)>errsig) && (it<=maxit)) {
		it++;

		signew = 0.5*(siglow+sighi);
		Vnew = BlackES(signew,K,omega,T1,T2,dT,discCurve,TMax,nMax) - market;

		if (Vnew<0) {
			siglow = signew;
			Vlow = Vnew;
		}
		else {
			sighi = signew;
			Vhi = Vnew;
		}
		dsig = sighi - siglow;
	}

	if (it<=maxit) {

		*sigimp = signew;
		errcode = 0;
		return errcode;
	}
	else { 
		errcode = 104;
		*sigimp = 0.;
		return errcode;
	}
}


// Not used
double G1analytics::funcB(double a,double t,double T)
{   
	return 1/a *(1 - exp(-a*(T-t)));
}

// Not used
double G1analytics::taux_court_Fwd(double *discMRKT,double t,double TMax,int nMax)
{
	double res = 0.;
	double epsilon = 1E-6;
	int nt,nT;
	double dt,T,disct,discT,xt,xT,ft,fT;
			
	dt = TMax/(double)nMax;
	T = t + epsilon;
	xt = t/dt;	nt = min((int)xt, nMax-1);  ft = xt-nt;
	xT = T/dt;  nT = min((int)xT, nMax-1);  fT = xT-nT;
	disct = ft*discMRKT[nt+1] + (1-ft)*discMRKT[nt];
	discT = fT*discMRKT[nT+1] + (1-fT)*discMRKT[nT];
    
	res = -(log(discT)-log(disct))/epsilon;
	return res;
}

// Not used
double G1analytics::func_phi(double *discMRKT,double t,double TMax,int nMax,double kappa1,double sigma1)
{
     double fwd_rate = taux_court_Fwd(discMRKT,t,TMax,nMax);
     double res = fwd_rate + sigma1*sigma1/2.*pow(G1analytics::funcB(kappa1,0,t),2);		        
     return res ;
}
