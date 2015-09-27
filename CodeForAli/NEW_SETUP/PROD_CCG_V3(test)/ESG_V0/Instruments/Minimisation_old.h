#ifndef MINIMISATION_H
#define MINIMISATION_H

#include "Analytics.h"
#include <string>
#include <vector>
////#include <iostream>
#define EPSILON 1.E-6
#define _ERROR -1000000.0

#define macheps 1.2e-16
#define tiny  1.0e-38
#define rtiny 3.834e-20
#define rHUGE 1.304e19

enum MinimisationMethod
{
	DFPMIN,
	LM, // Levenberg Macquard
	POWEL,
	NL2SOL,
	BOUNDEDNL2SOL,
	LM_NEW, // Levenberg Macquard new algorithm
};


inline
MinimisationMethod toMinimisationMethod(std::string& s)
{
	if(s == "QN") return DFPMIN;
	if(s == "LM") return LM;
	if(s == "NL2SOL") return NL2SOL;
	if(s == "BNL2SOL") return BOUNDEDNL2SOL;
	if(s == "LM_NEW") return LM_NEW;

	return DFPMIN;
}


template<class T>
inline void SWAP(T &a, T &b) {T dum=a; a=b; b=dum;}




//class Fitmrq 
//{
//public:
//	static const int NDONE=4, ITMAX=1000;
//	int ndat, ma, mfit;
//	const std::vector<double> &x,&y,&sig;
//	double tol;
//	/*OBJECTIVEFUNCTION funcs;*/
//	//void (*funcs)(const double, const std::vector<double> &, double &, std::vector<double> &);
//	std::vector<bool> ia;
//	std::vector<double> a;
//	Matrice covar;
//	Matrice alpha;
//	double chisq;
//
//	Fitmrq(const std::vector<double> &xx, const std::vector<double> &yy, const std::vector<double> &ssig, const std::vector<double> &aa,
//	//OBJECTIVEFUNCTION& funks/*void funks(const double, const std::vector<double> &, double &, std::vector<double> &)*/,
//	const double TOL=1.e-10) : ndat(xx.size()), ma(aa.size()), x(xx), y(yy), sig(ssig),
//	tol(TOL), /*funcs(funks),*/ ia(ma), alpha(ma,ma), a(aa), covar(ma,ma) {
//		
//		for (int i=0;i<ma;i++) ia[i] = true;
//	}
//
//	void hold(const int i, const double val) {ia[i]=false; a[i]=val;}
//	//void free(const int i) {ia[i]=true;}
//	
//	void gaussj(Matrice & a, Matrice& b)
//	{
//		int i,icol,irow,j,k,l,ll,n=a._nbLignes,m=b._nbColonnes;
//		double big,dum,pivinv;
//		std::vector<int> indxc(n),indxr(n),ipiv(n);
//		for (j=0;j<n;j++) ipiv[j]=0;
//		for (i=0;i<n;i++) {
//			big=0.0;
//			for (j=0;j<n;j++)
//				if (ipiv[j] != 1)
//					for (k=0;k<n;k++) {
//						if (ipiv[k] == 0) {
//							if (abs(a._matrice[j][k]) >= big) {
//								big=abs(a._matrice[j][k]);
//								irow=j;
//								icol=k;
//							}
//						}
//					}
//					++(ipiv[icol]);
//					if (irow != icol) {
//						for (l=0;l<n;l++) SWAP(a._matrice[irow][l],a._matrice[icol][l]);
//						for (l=0;l<m;l++) SWAP(b._matrice[irow][l],b._matrice[icol][l]);
//					}
//					indxr[i]=irow;
//					indxc[i]=icol;
//					if (a._matrice[icol][icol] == 0.0) throw("gaussj: Singular Matrix");
//					pivinv=1.0/a._matrice[icol][icol];
//					a._matrice[icol][icol]=1.0;
//					for (l=0;l<n;l++) a._matrice[icol][l] *= pivinv;
//					for (l=0;l<m;l++) b._matrice[icol][l] *= pivinv;
//					for (ll=0;ll<n;ll++)
//						if (ll != icol) {
//							dum=a._matrice[ll][icol];
//							a._matrice[ll][icol]=0.0;
//							for (l=0;l<n;l++) a._matrice[ll][l] -= a._matrice[icol][l]*dum;
//							for (l=0;l<m;l++) b._matrice[ll][l] -= b._matrice[icol][l]*dum;
//						}
//		}
//		for (l=n-1;l>=0;l--) {
//			if (indxr[l] != indxc[l])
//				for (k=0;k<n;k++)
//					SWAP(a._matrice[k][indxr[l]],a._matrice[k][indxc[l]]);
//		}
//	}
//	template<typename OBJECTIVEFUNCTION>
//	void fit(OBJECTIVEFUNCTION& funcs) {
//		int j,k,l,iter,done=0;
//		double alamda=.001,ochisq;
//		std::vector<double> atry(ma),beta(ma),da(ma);
//		mfit=0;
//		for (j=0;j<ma;j++) if (ia[j]) mfit++;
//		Matrice oneda(mfit,1), temp(mfit,mfit);
//		mrqcof(a,alpha,beta,funcs);
//		for (j=0;j<ma;j++) atry[j]=a[j];
//		ochisq=chisq;
//		for (iter=0;iter<ITMAX;iter++) {
//			if (done==NDONE) alamda=0.;
//			for (j=0;j<mfit;j++) {
//				for (k=0;k<mfit;k++) covar._matrice[j][k]=alpha._matrice[j][k];
//				covar._matrice[j][j]=alpha._matrice[j][j]*(1.0+alamda);
//				for (k=0;k<mfit;k++) temp._matrice[j][k]=covar._matrice[j][k];
//				oneda._matrice[j][0]=beta[j];
//			}
//			gaussj(temp,oneda);
//			for (j=0;j<mfit;j++) {
//				for (k=0;k<mfit;k++) covar._matrice[j][k]=temp._matrice[j][k];
//				da[j]=oneda._matrice[j][0];
//			}
//			if (done==NDONE) {
//				covsrt(covar);
//				covsrt(alpha);
//				return;
//			}
//			for (j=0,l=0;l<ma;l++)
//				if (ia[l]) atry[l]=a[l]+da[j++];
//			mrqcof(atry,covar,da,funcs);
//			if (abs(chisq-ochisq) < MAX(tol,tol*chisq)) done++;
//			if (chisq < ochisq) {
//				alamda *= 0.1;
//				ochisq=chisq;
//				for (j=0;j<mfit;j++) {
//					for (k=0;k<mfit;k++) alpha._matrice[j][k]=covar._matrice[j][k];
//						beta[j]=da[j];
//				}
//				for (l=0;l<ma;l++) a[l]=atry[l];
//			} else {
//				alamda *= 10.0;
//				chisq=ochisq;
//			}
//		}
//		throw("Fitmrq too many iterations");
//	}
//
//	template<typename OBJECTIVEFUNCTION>
//	void mrqcof(const std::vector<double> &a, Matrice &alpha, std::vector<double> &beta,OBJECTIVEFUNCTION& funcs) {
//		int i,j,k,l,m;
//		double ymod,wt,sig2i,dy;
//		std::vector<double> dyda(ma);
//		for (j=0;j<mfit;j++) {
//			for (k=0;k<=j;k++) alpha._matrice[j][k]=0.0;
//			beta[j]=0.;
//		}
//		chisq=0.;
//		for (i=0;i<ndat;i++) {
//			funcs(x[i],a,ymod,dyda);
//			sig2i=1.0/(sig[i]*sig[i]);
//			dy=y[i]-ymod;
//			for (j=0,l=0;l<ma;l++) {
//				if (ia[l]) {
//					wt=dyda[l]*sig2i;
//					for (k=0,m=0;m<l+1;m++)
//						if (ia[m]) alpha._matrice[j][k++] += wt*dyda[m];
//					beta[j++] += dy*wt;
//				}
//			}
//			chisq += dy*dy*sig2i;
//		}
//		for (j=1;j<mfit;j++)
//			for (k=0;k<j;k++) alpha._matrice[k][j]=alpha._matrice[j][k];
//	}
//
//	void covsrt(Matrice &covar) {
//		int i,j,k;
//		for (i=mfit;i<ma;i++)
//			for (j=0;j<i+1;j++) covar._matrice[i][j]=covar._matrice[j][i]=0.0;
//		k=mfit-1;
//		for (j=ma-1;j>=0;j--) {
//			if (ia[j]) {
//				for (i=0;i<ma;i++) SWAP(covar._matrice[i][k],covar._matrice[i][j]);
//				for (i=0;i<ma;i++) SWAP(covar._matrice[k][i],covar._matrice[j][i]);
//				k--;
//			}
//		}
//	}
//
//};





template<typename OBJECTIVEFUNCTION>
class Minimisation
{
protected:
	MinimisationMethod   myMethod;
	double				 myGradientTol;
	int					 myTotalIterations;
	int					 myMaxIterations;
	double				 myMaxStepLength;
	double				 myTolerance;

	double				 myCurrentMin;

	// replaces static variables in NR
	std::vector<std::vector<double> > myOneda;
	std::vector<double> myTry;
	std::vector<double> myBeta;
	std::vector<double> myDA;
	int myMfit;
	double myOchisq;
	double myftol; 
	double myxtol;


public:
	//Special parameters for LM_NEW
	int number_parameters;
	int vector_output_size;


	Minimisation(MinimisationMethod m = LM,
				   const double& gradienttol = 1.E-6) : myMethod(m), myGradientTol(gradienttol), myMaxIterations(1000),
				   myMaxStepLength(200.), myTolerance(4. * EPSILON) {}

	double solve(const OBJECTIVEFUNCTION& f, std::vector<double>& guess);

private:
	void dfpmin(const OBJECTIVEFUNCTION& func, std::vector<double>& p, const double gtol, int &iter, double &fret);

	void lnsrch(std::vector<double> &xold, const double fold, std::vector<double> &g,  std::vector<double> &p,
		std::vector<double> &x, double &f, const double stpmax, bool &check, const OBJECTIVEFUNCTION& func);

	void mrqcof(const std::vector<double>& x, const std::vector<double>& y, 
		const std::vector<double>& sig, std::vector<double>& a, 
		std::vector<bool>& ia, std::vector<std::vector<double> >& alpha, 
		std::vector<double>& beta, double& chisq, const OBJECTIVEFUNCTION& funcs);

	void mrqmin(const std::vector<double>& x,
		const std::vector<double>& y,
		const std::vector<double>& sig,
		std::vector<double>& a,
		std::vector<bool>& ia,
		std::vector<std::vector<double> >& covar, 
		std::vector<std::vector<double> >& alpha,
		double& chisq,
		const OBJECTIVEFUNCTION& funcs,
		double& alamda);

	void covsrt( std::vector<std::vector<double> > &covar, std::vector<bool>&ia, const int mfit);

	void gaussj(std::vector<std::vector<double> >& a, std::vector<std::vector<double> >& b);


	void lmdif(OBJECTIVEFUNCTION& funcs,double *guess,double *x,double *fvec);
	void fdjac2(OBJECTIVEFUNCTION& funcs,int m,int n,double *guess,double *x,double *fvec,double *fjac,int *iflag,double epsfcn,double *wa);
	double enorm(int n,double *x);
	void qrfac(int m,int n,double *a,int pivot,int *ipvt,double *rdiag,double *acnorm,double *wa);
	void lmpar(int n,double *r,int ldr,int *ipvt,double *diag,double *qtb,double delta,double *par,double *x,double *sdiag,double *wa1,double *wa2);
	void qrsolv(int n,double *r,int ldr,int *ipvt,double *diag,double *qtb,double *x,double *sdiag,double *wa);


};


template<typename OBJECTIVEFUNCTION>
double Minimisation<OBJECTIVEFUNCTION>::solve(const OBJECTIVEFUNCTION& f, std::vector<double>& guess)
{

	if(myMethod == DFPMIN)
	{
		dfpmin(f, guess, myGradientTol, myTotalIterations, myCurrentMin);
		return myCurrentMin;
	}
	else if(myMethod == LM)
	{
	  //std::vector<double> x(1,0.0); // not used
	  //std::vector<double> y(1, 0.0); // we look to minimise the function
	  //std::vector<double> sig(1,1.0); // assumption that the function is normally distributed

	  //Fitmrq levenberg_m(x,y,sig,guess,1.e-10);
	  //levenberg_m.fit(f);
	
	  

		std::vector<double> x(1,0.0); // not used
		std::vector<double> y(1, 0.0); // we look to minimise the function
		std::vector<double> sig(1,1.0); // assumption that the function is normally distributed
		std::vector<bool>   ia(guess.size(),true); // uses all the parameters
		std::vector<std::vector<double > > covar(guess.size()); // covariance matrix of estimations
		std::vector<std::vector<double> > alpha(guess.size()); // curvature

		int ma = guess.size();
		myOneda.resize(ma);

		for(int i = 0 ; i < ma ; ++i)
		{
			covar[i].resize(ma, 0.);
			alpha[i].resize(ma, 0.);
			myOneda[i].resize(1);
		}

		double chisq = 100.0; // error
		double prevchisq = 0.0; // error
		double diff = 100.0;

		double lambda = -1.0;

		myTry.resize(ma);
		myBeta.resize(ma);
		myDA.resize(ma);
		int iter=0;
		//while( (diff >= 0.0 || diff <= - 0.000001) && chisq > 1.0E-7 )
		while( abs(chisq)> myGradientTol  && iter <=myMaxIterations  )
		{
			prevchisq = chisq;
			mrqmin(x, y, sig, guess, ia, covar, alpha, chisq, f, lambda);
			diff = chisq - prevchisq;
			iter++;
		}
	}
	else if(myMethod == LM_NEW)
	{
		//Special parameters for LM_NEW
	//int number_parameters;    ?????????????
	//int vector_output_size;	????????????????
		//??????????????? should be changed
		double* x=new double [4];
		for(int i=0; i<4; i++){x[i] = 0;}
		double myftol = 1E-16; 
		double myxtol = 1E-16;
		myMaxIterations=1000;

		double *fvec =new double[vector_output_size+1];
		for(int i=0; i<vector_output_size+1; i++){fvec[i]=0.0;}
		double *param_initial = new double [4];
		for(int i=0;i<4;i++){param_initial[i]=guess[i];}
		lmdif(f,param_initial,x,fvec);
	
		delete [] fvec;
		delete [] x;
		delete [] param_initial;
	
	}
	else return _ERROR;
	return _ERROR;
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::dfpmin(const OBJECTIVEFUNCTION& func, std::vector<double>& p, const double gtol, int &iter, double &fret) 
{
  const int ITMAX    = myMaxIterations;
  const double TOLX  = myTolerance;
  const double STPMX = myMaxStepLength;
  bool check;
  int i,its,j;
  double den,fac,fad,fae,fp,stpmax,sum=0.0,sumdg,sumxi,temp,test;
  
  int n=p.size();
  std::vector<double> dg(n),g(n),hdg(n),pnew(n),xi(n);
  std::vector< std::vector<double> > hessin;
  hessin.resize(n);
  for (i=0;i<n;i++) { hessin[i].resize(n);  }
  fp=func(p);
  func.derivative(p,g);
  for (i=0;i<n;i++) {
    for (j=0;j<n;j++) hessin[i][j]=0.0;
    hessin[i][i]=1.0;
    xi[i] = -g[i];
    sum += p[i]*p[i];
  }
  stpmax=STPMX*MAX(sqrt(sum), (double) n);
  for (its=0;its<ITMAX;its++) {
    iter=its;
    lnsrch(p,fp,g,xi,pnew,fret,stpmax,check,func);
    fp=fret;
    for (i=0;i<n;i++) {
      xi[i]=pnew[i]-p[i];
      p[i]=pnew[i];
    }
    test=0.0;
    for (i=0;i<n;i++) {
      temp=fabs(xi[i])/MAX(fabs(p[i]),1.0);
      if (temp > test) test=temp;
    }
    if (test < TOLX)
      return;
    for (i=0;i<n;i++) dg[i]=g[i];
    func.derivative(p,g);
    test=0.0;
    den=MAX(fret,1.0);
    for (i=0;i<n;i++) {
      temp=fabs(g[i])*MAX(fabs(p[i]),1.0)/den;
      if (temp > test) test=temp;
    }
    if (test < gtol)
      return;
    for (i=0;i<n;i++) dg[i]=g[i]-dg[i];
    for (i=0;i<n;i++) {
      hdg[i]=0.0;
      for (j=0;j<n;j++) hdg[i] += hessin[i][j]*dg[j];
    }
    fac=fae=sumdg=sumxi=0.0;
    for (i=0;i<n;i++) {
      fac += dg[i]*xi[i];
      fae += dg[i]*hdg[i];
      sumdg += dg[i] * dg[i];
      sumxi += xi[i] * xi[i];
    }
    if (fac > sqrt(EPSILON*sumdg*sumxi)) {
      fac=1.0/fac;
      fad=1.0/fae;
      for (i=0;i<n;i++) dg[i]=fac*xi[i]-fad*hdg[i];
      for (i=0;i<n;i++) {
        for (j=i;j<n;j++) {
          hessin[i][j] += fac*xi[i]*xi[j]
            -fad*hdg[i]*hdg[j]+fae*dg[i]*dg[j];
          hessin[j][i]=hessin[i][j];
        }
      }
    }
    for (i=0;i<n;i++) {
      xi[i]=0.0;
      for (j=0;j<n;j++) xi[i] -= hessin[i][j]*g[j];
    }
  }
  Analytics::terminate_with_msg("too many iterations in dfpmin");
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::lnsrch(std::vector<double> &xold, const double fold, std::vector<double> &g, std::vector<double>&p,
                            std::vector<double> &x, double &f, const double stpmax, bool &check, const OBJECTIVEFUNCTION&  mycost)
{
  const double ALF=1.0e-4; // As in NR
  const double TOLX=myTolerance;
  int i;
  double a,alam,alam2=0.0,alamin,b,disc,f2=0.0;
  double rhs1,rhs2,slope,sum,temp,test,tmplam;
  
  int n=xold.size();
  check=false;
  sum=0.0;
  for (i=0;i<n;i++) sum += p[i]*p[i];
  sum=sqrt(sum);
  if (sum > stpmax)
    for (i=0;i<n;i++) p[i] *= stpmax/sum;
  
  slope=0.0;
  for (i=0;i<n;i++)
    slope += g[i]*p[i];
  if (slope >= 0.0) Analytics::terminate_with_msg("Roundoff problem in lnsrch.");
    test=0.0;
  for (i=0;i<n;i++) {
    temp=fabs(p[i])/MAX(fabs(xold[i]),1.0);
    if (temp > test) test=temp;
  }
  alamin=TOLX/test;
  alam=1.0;
  x.resize(n);
  for (;;) {
    for (i=0;i<n;i++) x[i]=xold[i]+alam*p[i];
    f=mycost(x);
    if (alam < alamin) {
      for (i=0;i<n;i++) x[i]=xold[i];
      check=true;
      return;
    } else if (f <= fold+ALF*alam*slope) return;
    else {
      if (alam == 1.0)
        tmplam = -slope/(2.0*(f-fold-slope));
      else {
        rhs1=f-fold-alam*slope;
        rhs2=f2-fold-alam2*slope;
        a=(rhs1/(alam*alam)-rhs2/(alam2*alam2))/(alam-alam2);
        b=(-alam2*rhs1/(alam*alam)+alam*rhs2/(alam2*alam2))/(alam-alam2);
        if (a == 0.0) tmplam = -slope/(2.0*b);
        else {
          disc=b*b-3.0*a*slope;
          if (disc < 0.0) tmplam=0.5*alam;
          else if (b <= 0.0) tmplam=(-b+sqrt(disc))/(3.0*a);
          else tmplam=-slope/(b+sqrt(disc));
        }
        if (tmplam>0.5*alam)
          tmplam=0.5*alam;
      }
    }
    alam2=alam;
    f2 = f;
    alam=MAX(tmplam,0.1*alam);
  }
}

template<typename OBJECTIVEFUNCTION>
//const std::vector<double>& x ---> the x-axis values
//const std::vector<double>& y --->   the y-axis values
//const std::vector<double>& sig --->      the standard deviation of each obervation (1 usually)
//std::vector<double>& a --->         the coeeficient to be found
//std::vector<bool>& ia --->        a vector of weights for each parameters. 0 they're not shifted, 1 they are shifted
//std::vector<std::vector<double> >& covar --->  covariance matrix of estimations
//std::vector<std::vector<double> >& alpha --->  curvature
//double& chisq --->           Error
//const OBJECTIVEFUNCTION& funcs ---> User-defined function
//double& alamda --->  Optimisation parameter
void Minimisation<OBJECTIVEFUNCTION>::mrqmin(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& sig, std::vector<double>& a, std::vector<bool>& ia,
                            std::vector<std::vector<double> >& covar, std::vector<std::vector<double> >& alpha, double& chisq, const OBJECTIVEFUNCTION& funcs, double& alamda)          
{
	int j,k,l;

	int ma=a.size();
	if (alamda < 0.0) {
		myMfit=0;
		for (j=0;j<ma;j++)
			if (ia[j]) myMfit++;
		alamda=0.001;
		mrqcof(x,y,sig,a,ia,alpha,myBeta,chisq,funcs);
		myOchisq=chisq;
		for (j=0;j<ma;j++) myTry[j]=a[j];
	}
	std::vector<std::vector<double> > temp(myMfit);
	for (j=0;j<myMfit;j++) {
		temp[j].resize(myMfit);
		for (k=0;k<myMfit;k++) covar[j][k]=alpha[j][k];
		covar[j][j]=alpha[j][j]*(1.0+alamda);
		for (k=0;k<myMfit;k++) temp[j][k]=covar[j][k];
		myOneda[j][0]=myBeta[j];
	}
	gaussj(temp,myOneda);
	for (j=0;j<myMfit;j++) {
		for (k=0;k<myMfit;k++) covar[j][k]=temp[j][k];
		myDA[j]=myOneda[j][0];
	}
	if (alamda == 0.0) {
		covsrt(covar,ia,myMfit);
		covsrt(alpha,ia,myMfit);
		return;
	}
	for (j=0,l=0;l<ma;l++)
		if (ia[l]) myTry[l]=a[l]+myDA[j++];
	mrqcof(x,y,sig,myTry,ia,covar,myDA,chisq,funcs);
	if (chisq < myOchisq) {
		alamda *= 0.1;
		myOchisq=chisq;
		for (j=0;j<myMfit;j++) {
			for (k=0;k<myMfit;k++) alpha[j][k]=covar[j][k];
				myBeta[j]=myDA[j];
		}
		for (l=0;l<ma;l++) a[l]=myTry[l];
	} else {
		alamda *= 10.0;
		chisq=myOchisq;
	}
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::mrqcof(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& sig, std::vector<double>& a, std::vector<bool>& ia, 
											   std::vector<std::vector<double> >& alpha, std::vector<double>& beta, double& chisq, const OBJECTIVEFUNCTION& funcs)
{
	int i,j,k,l,m,mfit=0;
	double ymod,wt,sig2i,dy;

	int ndata=x.size();
	int ma=a.size();
	std::vector<double> dyda(ma);
	for (j=0;j<ma;j++)
		if (ia[j]) mfit++;
	for (j=0;j<mfit;j++) {
		for (k=0;k<=j;k++) alpha[j][k]=0.0;
		beta[j]=0.0;
	}
	chisq=0.0;
	for (i=0;i<ndata;i++) {
		funcs(x[i],a,ymod,dyda);
		sig2i=1.0/(sig[i]*sig[i]);
		dy=y[i]-ymod;
		for (j=0,l=0;l<ma;l++) {
			if (ia[l]) {
				wt=dyda[l]*sig2i;
				for (k=0,m=0;m<l+1;m++)
					if (ia[m]) alpha[j][k++] += wt*dyda[m];
				beta[j++] += dy*wt;
			}
		}
		chisq += dy*dy*sig2i;
	}
	for (j=1;j<mfit;j++)
		for (k=0;k<j;k++) alpha[k][j]=alpha[j][k];
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::covsrt(std::vector<std::vector<double> > &covar, std::vector<bool>&ia, const int mfit)
{
	int i,j,k;

	int ma=ia.size();
	for (i=mfit;i<ma;i++)
		for (j=0;j<i+1;j++) covar[i][j]=covar[j][i]=0.0;
	k=mfit-1;
	for (j=ma-1;j>=0;j--) {
		if (ia[j]) {
			for (i=0;i<ma;i++) SWAP(covar[i][k],covar[i][j]);
			for (i=0;i<ma;i++) SWAP(covar[k][i],covar[j][i]);
			k--;
		}
	}
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::gaussj(std::vector<std::vector<double> >& a, std::vector<std::vector<double> >& b)
{
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv;
	//irow=0;//Ali
	//icol=0;//Ali

	int n=a.size();
	int m=b[0].size();
	std::vector<int> indxc(n),indxr(n),ipiv(n);
	for (j=0;j<n;j++) ipiv[j]=0;
	for (i=0;i<n;i++) {
		big=0.0;
		for (j=0;j<n;j++)
			if (ipiv[j] != 1)
				for (k=0;k<n;k++) {
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big=fabs(a[j][k]);
							irow=j;
							icol=k;
						}
					}
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=0;l<n;l++) SWAP(a[irow][l],a[icol][l]);
			for (l=0;l<m;l++) SWAP(b[irow][l],b[icol][l]);
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (a[icol][icol] == 0.0) Analytics::terminate_with_msg("gaussj: Singular Matrix");
		pivinv=1.0/a[icol][icol];
		a[icol][icol]=1.0;
		for (l=0;l<n;l++) a[icol][l] *= pivinv;
		for (l=0;l<m;l++) b[icol][l] *= pivinv;
		for (ll=0;ll<n;ll++)
			if (ll != icol) {
				dum=a[ll][icol];
				a[ll][icol]=0.0;
				for (l=0;l<n;l++) a[ll][l] -= a[icol][l]*dum;
				for (l=0;l<m;l++) b[ll][l] -= b[icol][l]*dum;
			}
	}
	for (l=n-1;l>=0;l--) {
		if (indxr[l] != indxc[l])
			for (k=0;k<n;k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}
}

template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::lmdif(OBJECTIVEFUNCTION& funcs,double *guess,double *x,double *fvec)
{

       int i,iflag,ij,jj,iter,j,l;
        double actred,delta,dirder,fnorm,fnorm1,gnorm;
        double par,pnorm,prered,ratio;
        double sum,temp,temp1,temp2,temp3,xnorm;
        double *fjac,*diag,*qtf,*wa1,*wa2,*wa3,*wa4,*xpvt;
        int *ipvt;
        double gtol,epsfcn,factor;
        int mode,nprint,ldfjac;
		xnorm=0; // added
		delta=0; // added
        // sub-divide work array
        double ftol=myftol;
        double xtol=myxtol;
		int maxit=myMaxIterations;
		int n=number_parameters;
		int m=vector_output_size;
		int info=0;
		int *nfev=0;
		double *work = new double[(n+2)*(m+7)];
		fjac  = work;
        diag = fjac + (n+1)*(m+1);
        qtf = diag + (n+1);
        xpvt = qtf + (n+1);
        wa1 = xpvt + (n+1);
        wa2 = wa1 + (n+1);
        wa3 = wa2 + (n+1);
        wa4 = wa3 + (n+1);
        ipvt = (int*)xpvt;
        gtol = 1.e-16;
        epsfcn = 1.e-6;
        mode = 1;
        factor = 100.;
        nprint = 0;
        ldfjac = m;
		
        x[0]=0.0;
        ipvt[0]=0;
        diag[0]=0.0;
        qtf[0]=0.0;
        wa1[0]=0.0;
        wa2[0]=0.0;
        wa3[0]=0.0;
        fvec[0]=0.0;
        wa4[0]=0.0;
        fjac[0]=0.0;	
        for(i=1;i<=n;i++){
                x[i-1] = guess[i];
                ipvt[i-1]=ipvt[i];
                diag[i-1]=diag[i];
                qtf[i-1]=qtf[i];
                wa1[i-1]=wa1[i];
                wa2[i-1]=wa2[i];
                wa3[i-1]=wa3[i];
        }
		fvec[m]=0.0;
        for(i=1;i<=m;i++){
                fvec[i-1] = fvec[i];
                wa4[i-1]=wa4[i];
        }
        for(i=1;i<=n*m+m+n+1;i++){
                fjac[i-1] = fjac[i];
        }
        x[n]=0.0;
        ipvt[n]=0;
        diag[n]=0.0;
        qtf[n]=0.0;
        wa1[n]=0.0;
        wa2[n]=0.0;
        wa3[n]=0.0;
		//Calibration::Mapping(x,ModStruct,1);

        fvec[m]=0.0;
        wa4[m]=0.0;
        fjac[(n+1)*(m+1)]=0.0;
        info = 0;
        iflag = 0;
        *nfev = 0;
        if( (n <= 0) || (m < n) || (ldfjac < m) || (ftol < 0.0)
                || (xtol < 0.0) || (gtol < 0.0) || (maxit <= 0)
                || (factor <= 0.0) ){goto L300;}
        if( mode == 2 ){ 
                for( j=0; j<n; j++ ){
                        if( diag[j] <= 0.0 ){goto L300;}
                }
        }
        iflag = 1;
		funcs(guess,x,fvec);
        //fcn(model,m/*,type,Calib_param*//*,omega*/,guess,x,ModStruct/*,MrktStruct*/,fvec);
        *nfev=1;
        if(iflag < 0){goto L300;}
        fnorm = enorm(m,fvec);
        par = 0.0;
        iter = 1;
L30:
        iflag = 2;
		fdjac2(funcs,m,n,guess,x,fvec,fjac,&iflag,epsfcn,wa4);
        //fdjac2(model,m/*,type,Calib_param*//*,omega*/,n,guess,x,ModStruct/*,MrktStruct*/,fvec,fjac,&iflag,epsfcn,wa4);
		*nfev += n;
        if(iflag < 0){goto L300;}
        if( nprint > 0 ){
                iflag = 0;
                if((iter-1%nprint) == 0){

						funcs(guess,x,fvec);
                        //fcn(model,m/*,type,Calib_param*//*,omega*/,guess,x,ModStruct/*,MrktStruct*/,fvec);
                        if(iflag < 0){goto L300;}
                }
        }
        qrfac(m,n,fjac,1,ipvt,wa1,wa2,wa3);    
        if(iter == 1){
                if(mode != 2){
                        for( j=0; j<n; j++ ){
                                diag[j] = wa2[j];
                                if( wa2[j] == 0.0 ){
                                        diag[j] = 1.0;
                                }
                        }
                }
                for( j=0; j<n; j++ ){
                        wa3[j] = diag[j] * x[j];
                }
                xnorm = enorm(n,wa3);
                delta = factor*xnorm;
                if(delta == 0.0)
                delta = factor;
        }
        for( i=0; i<m; i++ ){
                wa4[i] = fvec[i];
        }
        jj = 0;
        for( j=0; j<n; j++){
                temp3 = fjac[jj];
                if(temp3 != 0.0){
                        sum = 0.0;
                        ij = jj;
                        for( i=j; i<m; i++ ){
                                sum = sum +  fjac[ij] * wa4[i];
                                ij = ij + 1;
                        }
                        temp = -sum / temp3;
                        ij = jj;
                        for( i=j; i<m; i++ ){
                                wa4[i] = wa4[i] + fjac[ij] * temp;
                                ij = ij +  1;
                        }
                }
                fjac[jj] = wa1[j];
                jj = jj + m+1;  
                qtf[j] = wa4[j];
        }
         gnorm = 0.0;
        if(fnorm != 0.0){
                jj = 0;
                for( j=0; j<n; j++ ){
                        l = ipvt[j];
                        if(wa2[l] != 0.0){
                                sum = 0.0;
                                ij = jj;
                                for( i=0; i<=j; i++ ){
                                        sum = sum + fjac[ij]*(qtf[i]/fnorm);
                                        ij = ij + 1; 
                                }
                                gnorm = max(gnorm,fabs(sum/wa2[l]));
                        }
                        jj = jj +  m;
                }
        }
        if(gnorm <= gtol){
		info = 4;}
        if( info != 0){goto L300;}
        if(mode != 2){
                for( j=0; j<n; j++ )
                        diag[j] = max(diag[j],wa2[j]);
        }
L200:
        lmpar(n,fjac,ldfjac,ipvt,diag,qtf,delta,&par,wa1,wa2,wa3,wa4);
        for( j=0; j<n; j++ ){
       wa1[j] = -wa1[j];
       wa2[j] = x[j] + wa1[j];
       wa3[j] = diag[j]*wa1[j];
        }
        pnorm = enorm(n,wa3);
        if(iter == 1){ delta = min(delta,pnorm);}
        iflag = 1;
		funcs(guess,wa2,wa4);
        //fcn(model,m/*,type,Calib_param*//*,omega*/,guess,wa2,ModStruct/*,MrktStruct*/,wa4);
        *nfev += 1;
        if(iflag < 0){goto L300;}
        fnorm1 = enorm(m,wa4);
        actred = -1.0;
        if( (0.1*fnorm1) < fnorm){
                temp = fnorm1/fnorm;
                actred = 1.0 - temp * temp;
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                wa3[j] = 0.0;
                l = ipvt[j];
                temp = wa1[l];
                ij = jj;
                for( i=0; i<=j; i++ ){
                        wa3[i] = wa3[i] + fjac[ij]*temp;
                        ij = ij + 1; 
                }
                jj = jj + m;
        }
        temp1 = enorm(n,wa3)/fnorm;
        temp2 = (sqrt(par)*pnorm)/fnorm;
        prered = temp1*temp1 + 2.0*(temp2*temp2);
        dirder = -(temp1*temp1 + temp2*temp2);
        ratio = 0.0;
        if(prered != 0.0){ ratio = actred/prered;}
        if(ratio <= 0.25){
                if(actred >= 0.0){
                        temp = 0.5;
                }else{
                        temp = 0.5*dirder/(dirder + 0.5*actred);
                }
                if( ((0.1*fnorm1) >= fnorm) || (temp < 0.1) ){
                        temp = 0.1;
                }
                delta = temp*min(delta,10.0*pnorm);
                par = par/temp;
        }else {
                if( (par == 0.0) || (ratio >= 0.75) ){
                        delta = 2.0*pnorm;
                        par = 0.5*par;
                }
        }
        if(ratio >= 0.0001){
                for( j=0; j<n; j++ ){
                        x[j] = wa2[j];
                        wa2[j] = diag[j]*x[j];
                }
                for(i=0;i<m;i++){fvec[i] = wa4[i];}
                xnorm = enorm(n,wa2);
                fnorm = fnorm1;
                iter = iter + 1;
        }
        if((fabs(actred) <= ftol) && (prered <= ftol) && (0.5*ratio <= 1.0) ){ 
		info = 1;}
        if(delta <= xtol*xnorm){
		info = 2;}
        if((fabs(actred) <= ftol) && (prered <= ftol) && (0.5*ratio <= 1.0) && ( info == 2) ){
                info = 3;
        }
        if(info != 0){goto L300;}
        if(*nfev >= maxit){info = 5;}
        if((fabs(actred) <= macheps)
                && (prered <= macheps)
                && (0.5*ratio <= 1.0) ){info = 6;}
        if(delta <= macheps*xnorm){info = 7;}
        if(gnorm <= macheps){ info = 8;}
        if(info != 0){goto L300;}
        if(ratio < 0.0001){goto L200;}
        goto L30;
L300:
        if(iflag < 0){info = iflag;}
        iflag = 0;
		if(nprint > 0){		funcs(guess,x,fvec);}//fcn(model,m/*,type,Calib_param*//*,omega*/,guess,x,ModStruct/*,MrktStruct*/,fvec);}

		//Calibration::Mapping(x,ModStruct,-1);

        for(i=1;i<=n;i++){
                j=n+1-i;
                x[j] = x[j-1];
                ipvt[j]=ipvt[j-1];
                diag[j]=diag[j-1];
                qtf[j]=qtf[j-1];
                wa1[j]=wa1[j-1];
                wa2[j]=wa2[j-1];
                wa3[j]=wa3[j-1];
        }
        for(i=1;i<=m;i++){
                j=m+1-i;
                fvec[j] = fvec[j-1];
                wa4[j]=wa4[j-1];
        }
        for(i=1;i<=n*m+m+n+1;i++){
                j=n*m+m+n+1+1-i;
                fjac[j] = fjac[j-1];
        }
        x[0]=0.0;
        ipvt[0]=0;
        diag[0]=0.0;
        qtf[0]=0.0;
        wa1[0]=0.0;
        wa2[0]=0.0;
        wa3[0]=0.0;
        fvec[0]=0.0;
        wa4[0]=0.0;
        fjac[0]=0.0;
		delete [] work;
}
template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::fdjac2(OBJECTIVEFUNCTION& funcs,int m,int n,double *guess,double *x,double *fvec,double *fjac,int *iflag,double epsfcn,double *wa)
{
        int i,j,ij;
        double eps,h,temp;
        temp = max(epsfcn,macheps);
        eps = sqrt(temp);
        ij = 0;
        for( j=0; j<n; j++ ){
                temp = x[j];
                h = eps * fabs(temp);
                if(h == 0.0){h = eps;}
                x[j] = temp + h;
				funcs(guess,x,wa);
                //fcn(model,m/*type,Calib_param*//*,omega*/,guess,x,ModStruct/*,MrktStruct*/,wa);
                if( *iflag < 0){
                        return;
                }
                x[j] = temp;
                for( i=0; i<m; i++ ){
                        fjac[ij] = (wa[i] - fvec[i])/h;
                        ij = ij + 1;    
                }
        }
}
template<typename OBJECTIVEFUNCTION>
double Minimisation<OBJECTIVEFUNCTION>::enorm(int n,double *x)
{
        int i;
        double aHUGE,floatn,s1,s2,s3,xabs,x1max,x3max;
        double ans, temp;

        s1 = 0.0;
        s2 = 0.0;
        s3 = 0.0;
        x1max = 0.0;
        x3max = 0.0;
        floatn = n;
        aHUGE = rHUGE/floatn;

        for( i=0; i<n; i++ ){
                xabs = fabs(x[i]);
                if( (xabs > rtiny) && (xabs < aHUGE)){
                        s2 = s2 + xabs*xabs;
                        continue;
                }
                if(xabs > rtiny){
                        if(xabs > x1max){
                                temp = x1max/xabs;
                                s1 = 1.0 + s1*temp*temp;
                                x1max = xabs;
                        }else{
                                temp = xabs/x1max;
                                s1 = s1 + temp*temp;
                        }
                        continue;
                }
                if(xabs > x3max){
                        temp = x3max/xabs;
                        s3 = 1.0 + s3*temp*temp;
                        x3max = xabs;
                }else{
                        if(xabs != 0.0){
                                temp = xabs/x3max;
                                s3 = s3 + temp*temp;
                        }
                }
        }
        if(s1 != 0.0){
                temp = s1 + (s2/x1max)/x1max;
                ans = x1max*sqrt(temp);
                return ans;
        }
        if(s2 != 0.0){
                if(s2 >= x3max){
                        temp = s2*(1.0+(x3max/s2)*(x3max*s3));
                }else{
                        temp = x3max*((s2/x3max)+(x3max*s3));
                }
                ans = sqrt(temp);
        }else{
                ans = x3max*sqrt(s3);
        }
        return ans;
}
template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::qrfac(int m,int n,double *a,int pivot,int *ipvt,double *rdiag,double *acnorm,double *wa)
{
        int i,ij,jj,j,jp1,k,kmax,minmn;
        double ajnorm,sum,temp; 

        ij = 0;
        for( j=0; j<n; j++ ){

			acnorm[j] = enorm(m,&a[ij]);
                rdiag[j] = acnorm[j];
                wa[j] = rdiag[j];
                if(pivot != 0){ipvt[j] = j;}
                ij = ij + m; 
        }
        minmn = min(m,n);
        for( j=0; j<minmn; j++ ){
                if(pivot == 0){goto L40;}
                kmax = j;
                for( k=j; k<n; k++ ){
                        if(rdiag[k] > rdiag[kmax]){kmax = k;}
                }
                if(kmax == j){goto L40;}
                ij = m * j;
                jj = m * kmax;
                for( i=0; i<m; i++ ){
                        temp = a[ij]; 
                        a[ij] = a[jj]; 
                        a[jj] = temp;
                        ij = ij + 1;
                        jj = jj + 1;
                }

				rdiag[kmax] = rdiag[j];
                wa[kmax] = wa[j];
                k = ipvt[j];
                ipvt[j] = ipvt[kmax];
                ipvt[kmax] = k;
L40:
            jj = j + m*j;
                ajnorm = enorm(m-j,&a[jj]);
                if(ajnorm == 0.0){
                        goto L100;
                }
                if(a[jj] < 0.0){
                        ajnorm = -ajnorm;
                }
                ij = jj;
                for( i=j; i<m; i++ ){
                        a[ij] = a[ij]/ajnorm;
                        ij = ij + 1; 
                }
                a[jj] = a[jj] + 1.0;
                jp1 = j + 1;
                if(jp1 < n ){
                        for( k=jp1; k<n; k++ ){
                                sum = 0.0;
                                ij = j + m*k;
                                jj = j + m*j;
                                for( i=j; i<m; i++ ){
                                        sum = sum + a[jj]*a[ij];
                                        ij = ij + 1; 
                                        jj = jj + 1; 
                                }
                                temp = sum/a[j+m*j];
                                ij = j + m*k;
                                jj = j + m*j;
                                for( i=j; i<m; i++ ){
                                        a[ij] = a[ij] - temp*a[jj];
                                        ij = ij + 1; 
                                        jj = jj + 1; 
                                }
                                if( (pivot != 0) && (rdiag[k] != 0.0) ){
                                        temp = a[j+m*k]/rdiag[k];
                                        temp = max(0.0, 1.0-temp*temp );
                                        rdiag[k] *= sqrt(temp);
                                        temp = rdiag[k]/wa[k];
                                        if( (0.05*temp*temp) <= macheps){
                                                rdiag[k] = enorm(m-j-1,&a[jp1+m*k]);
                                                wa[k] = rdiag[k];
                                        }
                                }
                        }
                }
L100:
        rdiag[j] = -ajnorm;
        }
}
template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::lmpar(int n,double *r,int ldr,int *ipvt,double *diag,double *qtb,double delta,double *par,double *x,double *sdiag,double *wa1,double *wa2)
{
        int i,iter,ij,jj,j,jm1,jp1,k,l,nfsing;
        double dxnorm,fp,gnorm,parc,parl,paru;
        double sum,temp;

        nfsing = n;
        jj = 0;
        for( j=0; j<n; j++ ){
                wa1[j] = qtb[j];
                if( (r[jj] == 0.0) && (nfsing == n) ){nfsing = j;}
                if(nfsing < n){wa1[j] = 0.0;}
                jj = jj + ldr+1; 
        }
        if(nfsing >= 1){
                for( k=0; k<nfsing; k++ ){
                        j = nfsing - k - 1;
                        wa1[j] = wa1[j]/r[j+ldr*j];
                        temp = wa1[j];
                        jm1 = j - 1;
                        if(jm1 >= 0){
                                ij = ldr * j;
                                for( i=0; i<=jm1; i++){
                                        wa1[i] = wa1[i] - r[ij]*temp;
                                        ij = ij + 1;
                                }
                        }
                }
        }
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                x[l] = wa1[j];
        }
        iter = 0;
        for( j=0; j<n; j++ ){
                wa2[j] = diag[j]*x[j];
        }
		dxnorm =enorm(n,wa2);
        fp = dxnorm - delta;
        if(fp <= 0.1*delta){
                goto L220;
        }
        parl = 0.0;
        if(nfsing >= n){
                for( j=0; j<n; j++ ){
                        l = ipvt[j];
                        wa1[j] = diag[l]*(wa2[l]/dxnorm);
                }
                jj = 0;
                for( j=0; j<n; j++ ){
                        sum = 0.0;
                        jm1 = j - 1;
                        if(jm1 >= 0){
                                ij = jj;
                                for( i=0; i<=jm1; i++ ){
                                        sum = sum + r[ij]*wa1[i];
                                        ij = ij + 1;
                                }
                        }
                        wa1[j] = (wa1[j] - sum)/r[j+ldr*j];
                        jj += ldr; 
                }
                temp = enorm(n,wa1);
                parl = ((fp/delta)/temp)/temp;
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                sum = 0.0;
                ij = jj;
                for( i=0; i<=j; i++ ){
                        sum = sum + r[ij]*qtb[i];
                        ij = ij +  1;
                }
                l = ipvt[j];
                wa1[j] = sum/diag[l];
                jj = jj + ldr; 
        }
        gnorm = enorm(n,wa1);
        paru = gnorm/delta;
        if(paru == 0.0){
                paru = tiny/min(delta,0.1);
        }
        *par = max(*par,parl);
        *par = min(*par,paru);
        if(*par == 0.0){*par = gnorm/dxnorm;}
L150:
        iter = iter + 1;
        if(*par == 0.0){*par = max(tiny,0.001*paru);}
        temp = sqrt( *par );
        for( j=0; j<n; j++ ){
                wa1[j] = temp*diag[j];
        }
        qrsolv(n,r,ldr,ipvt,wa1,qtb,x,sdiag,wa2);
        for( j=0; j<n; j++ ){
                wa2[j] = diag[j]*x[j];
        }
        dxnorm = enorm(n,wa2);
        temp = fp;
        fp = dxnorm - delta;
        if( (fabs(fp) <= 0.1*delta)
                || ((parl == 0.0) && (fp <= temp) && (temp < 0.0))
                || (iter == 10) ){goto L220;}

        for( j=0; j<n; j++ ){
                l = ipvt[j];
                wa1[j] = diag[l]*(wa2[l]/dxnorm);
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                wa1[j] = wa1[j]/sdiag[j];
                temp = wa1[j];
                jp1 = j + 1;
                if(jp1 < n){
                        ij = jp1 + jj;
                        for( i=jp1; i<n; i++ ){
                                wa1[i] = wa1[i] - r[ij]*temp;
                                ij = ij + 1; 
                        }
                }
                jj = jj + ldr; 
        }
        temp = enorm(n,wa1);
        parc = ((fp/delta)/temp)/temp;
        if(fp > 0.0){ parl = max(parl, *par);}
        if(fp < 0.0){ paru = min(paru, *par);}
        *par = max(parl, *par + parc);
        goto L150;
L220:
        if(iter == 0){*par = 0.0;}
}
template<typename OBJECTIVEFUNCTION>
void Minimisation<OBJECTIVEFUNCTION>::qrsolv(int n,double *r,int ldr,int *ipvt,double *diag,double *qtb,double *x,double *sdiag,double *wa)
{
        int i,ij,ik,kk,j,jp1,k,kp1,l,nfsing;
        double fcos,fcoftan,qtbpj,fsin,sum,ftan,temp;

        kk = 0;
        for( j=0; j<n; j++ ){
                ij = kk;
                ik = kk;
                for( i=j; i<n; i++ ){
                        r[ij] = r[ik];
                        ij = ij + 1;   
                        ik = ik + ldr; 
                }
                x[j] = r[kk];
                wa[j] = qtb[j];
                kk = kk + ldr+1; 
        }
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                if(diag[l] == 0.0){goto L90;}
                for( k=j; k<n; k++ ){
                        sdiag[k] = 0.0;
                }
                sdiag[j] = diag[l];
                qtbpj = 0.0;
                for( k=j; k<n; k++ ){
                        if(sdiag[k] == 0.0){continue;}
                        kk = k + ldr * k;
                        if(fabs(r[kk]) < fabs(sdiag[k])){
                                fcoftan = r[kk]/sdiag[k];
                                fsin = 0.5/sqrt(0.25+0.25*fcoftan*fcoftan);
                                fcos = fsin*fcoftan;
                        }else{
                                ftan = sdiag[k]/r[kk];
                                fcos = 0.5/sqrt(0.25+0.25*ftan*ftan);
                                fsin = fcos*ftan;
                        }
                        r[kk] = fcos*r[kk] + fsin*sdiag[k];
                        temp = fcos*wa[k] + fsin*qtbpj;
                        qtbpj = fcos*qtbpj-fsin*wa[k];
                        wa[k] = temp;
                        kp1 = k + 1;
                        if( n > kp1 ){
                                ik = kk + 1;
                                for( i=kp1; i<n; i++ ){
                                        temp = fcos*r[ik] + fsin*sdiag[i];
                                        sdiag[i] = fcos*sdiag[i]-fsin*r[ik];
                                        r[ik] = temp;
                                        ik = ik + 1;
                                }
                        }
                }
L90:
                kk = j + ldr*j;
                sdiag[j] = r[kk];
                r[kk] = x[j];
        }
        nfsing = n;
        for( j=0; j<n; j++ ){
                if( (sdiag[j] == 0.0) && (nfsing == n) ){nfsing = j;}
                if(nfsing < n){wa[j] = 0.0;}
        }
        if(nfsing < 1){goto L150;}
        for( k=0; k<nfsing; k++ ){
                j = nfsing - k - 1;
                sum = 0.0;
                jp1 = j + 1;
                if(nfsing > jp1){
                        ij = jp1 + ldr * j;
                        for( i=jp1; i<nfsing; i++ ){
                                sum = sum +  r[ij]*wa[i];
                                ij = ij +  1; 
                        }
                }
                wa[j] = (wa[j] - sum)/sdiag[j];
        }
L150:
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                x[l] = wa[j];
        }
}
#endif