#ifndef TOOLS_H
#define TOOLS_H

#include <math.h>
#include "GMXBParameters.h"
#include <iostream>
using namespace std;

#pragma once

template<typename T>
class SmartPointer
{
protected:
	T* myPtr;
	unsigned long* myCounter;

	// This function is needed as it's not possible to make all SmartPointer<T> friends of each other
	friend unsigned long* refCount(const SmartPointer<T>& sp) { return sp.myCounter; }

    void setupFromRawPointer(T *ptr);
    void setupFromSmartPointerContent(T *ptr, unsigned long* myCounter);

	void dropReference();

public:
    explicit SmartPointer(T *p = 0) { setupFromRawPointer(p); }
    template<typename T2> SmartPointer(T2 *p ) { setupFromRawPointer(p); }
	SmartPointer(const SmartPointer<T>& rhs) { setupFromSmartPointerContent(rhs.ptr(), refCount(rhs)); }
    
	~SmartPointer() {  dropReference(); }


	T* ptr() const { return myPtr; }

	T& operator*() const { return *myPtr; }
    T* operator->() const { return myPtr; }
	T& operator[](int i){return myPtr[i];}

	SmartPointer<T>& operator=(const SmartPointer<T>& other)
	{
		if (myCounter != other.myCounter)
		{
			dropReference();

			myPtr = other.myPtr;
			myCounter = other.myCounter;
			++*myCounter;
		}

		return *this;
	}

    template<typename T2> SmartPointer<T>& equalsDynamicCast(const SmartPointer<T2>& rhs)
    {
        if (myCounter != refCount(rhs))
        {
            dropReference();
            // If cast fails, reset using default constructor via placement new
            // This is ok as we've already called dropReference to clean up
            T *castPtr = dynamic_cast<T *>(rhs.ptr());
            if (castPtr != 0) setupFromSmartPointerContent(castPtr, refCount(rhs));
            else setupFromRawPointer(0);
        }
        return *this;
    }


    bool isNull() const { return myPtr == 0; }

	friend bool operator==(const SmartPointer<T>& lhs, const SmartPointer<T>& rhs) { return lhs.myPtr == rhs.myPtr; }
	friend bool operator!=(const SmartPointer<T>& lhs, const SmartPointer<T>& rhs) { return lhs.myPtr != rhs.myPtr; }
	friend bool operator<(const SmartPointer<T>& lhs, const SmartPointer<T>& rhs)  { return lhs.myPtr < rhs.myPtr; }

};

template<typename T>
void SmartPointer<T>::dropReference()
{
    --*myCounter;

    if (*myCounter == 0)
    {
        delete myPtr;
        delete myCounter;
    }

    myPtr = 0;
    myCounter = 0;
}


template<typename T>
void SmartPointer<T>::setupFromRawPointer(T *ptr)
{
    myPtr = ptr;
    myCounter = new unsigned long;
    *myCounter = 1;
}


template<typename T>
void SmartPointer<T>::setupFromSmartPointerContent(T *ptr, unsigned long* counter)
{
    myPtr = ptr;
    myCounter = counter;
    ++*myCounter;
}

template<class T1, class T2> SmartPointer<T1> smart_dynamic_cast(const SmartPointer<T2>& rhs)
{
    SmartPointer<T1> sp;
    sp.equalsDynamicCast(rhs);
    return sp;
}

void Cholesky(double** input, int dimension);

double interpole(int num_vol,int num_step,SmartPointer<int> vol_time, double* vol);

double linear_interp(double* arrayT, double* arrayY, int num, double term);


class Basic_Information{
private:int Nb_Scens;
	    int Antithetic;
		int Nb_Equity;
		int Nb_Bond;
		int Nb_Rate;
		int Nb_Step;
		int Nb_Initial_Rate;
		int Nb_Vol;
		int Nb_Random;
		double A;
		double Sigma;
		int Delta_T;
		SmartPointer <double> Error;
		SmartPointer <double> Add_Error;
		SmartPointer <int> Max_Try;
		SmartPointer <int> Vol_Time;
		double** Vol_Equity;
		SmartPointer <double> Maturity_Bond;
		SmartPointer <double> Bond_Coupon;
		SmartPointer <double> Bond_Freq;
		SmartPointer <double> Maturity_Rate;
		SmartPointer <int> Rate_Type;
		SmartPointer <double> Rate_Period;
		SmartPointer <double> Forward_Rate;
		SmartPointer <double> Forward_Rate1;
		double** Correlation;

public:Basic_Information(){}

	   Basic_Information(int nb_scen,int ant,int nb_eq,int nb_bond,int nb_rate,
	   int nb_step,int nb_initial,int nb_vol,double a,double sigma,int delta_t,double *error,double *add_error, int * max_try,double **vol,
	   double *m_bond,double *bond_coupon,double *bond_freq,double *m_rate,int *rate_type,double *rate_period,double *rate,double **cor/*,string* equity*/){
		   Nb_Scens=nb_scen;
		   Antithetic=ant;
		   Nb_Equity=nb_eq;
		   Nb_Bond=nb_bond;
		   Nb_Rate=nb_rate;
		   Nb_Step=nb_step;
		   Nb_Initial_Rate=nb_initial;
		   Nb_Vol=nb_vol;
		   A=a;
		   Sigma=sigma;
		   Delta_T=delta_t;
           //Error=SmartPointer <double>(error);
		   double *error_copy=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   error_copy[i]=error[i];
		   }
		   Error=SmartPointer<double>(error_copy);

		   double *add_error_copy=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   add_error_copy[i]=add_error[i];
		   }
		   Add_Error=SmartPointer<double>(add_error_copy);

		   int *max_try_copy=new int [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   max_try_copy[i]=max_try[i];
		   }
		   Max_Try=SmartPointer<int>(max_try_copy);

		   Vol_Equity=new double *[Nb_Equity];
		   for(int i=0;i<Nb_Equity;i++){
			   Vol_Equity[i]=new double [Nb_Vol];
			   for(int j=0;j<Nb_Vol;j++){
				   Vol_Equity[i][j]=vol[i+1][j];
			   }
		   }

		   int *Vol_T=new int [Nb_Vol];
		   for(int i=0;i<Nb_Vol;i++){
			   Vol_T[i]=(int)vol[0][i];
		   }
		   Vol_Time=SmartPointer<int> (Vol_T);

		   double *m_bond_copy=new double [Nb_Bond];
		   double *bond_coupon_copy=new double [Nb_Bond];
		   double *bond_freq_copy=new double [Nb_Bond];
		   for(int i=0;i<Nb_Bond;i++){
			   m_bond_copy[i]=m_bond[i];
			   bond_coupon_copy[i]=bond_coupon[i];
			   bond_freq_copy[i]=bond_freq[i];
		   }
		   Maturity_Bond=SmartPointer<double>(m_bond_copy);
		   Bond_Coupon=SmartPointer<double>(bond_coupon_copy);
		   Bond_Freq=SmartPointer<double>(bond_freq_copy);
		   
		   double *m_rate_copy=new double [Nb_Rate];
		   int *rate_type_copy=new int [Nb_Rate];
		   double * rate_period_copy=new double [Nb_Rate];
		   for(int i=0;i<Nb_Rate;i++){
			   m_rate_copy[i]=m_rate[i];
			   rate_type_copy[i]=rate_type[i];
			   rate_period_copy[i]=rate_period[i];
		   }
		   Maturity_Rate=SmartPointer<double>(m_rate_copy);
		   Rate_Type=SmartPointer<int>(rate_type_copy);
		   Rate_Period=SmartPointer<double>(rate_period_copy);

		   double* forward_rate=new double [Nb_Initial_Rate];
		   double *forward_rate1=new double [Nb_Initial_Rate+1];
		   forward_rate1[0]=0;
		   for(int i=0;i<Nb_Initial_Rate;i++){
			   forward_rate1[i+1]=(i+1)/Delta_T*log(1+rate[i]/100);
			   if(i==0) forward_rate[i]=forward_rate1[i+1]*Delta_T;
			   if(i>0) forward_rate[i]=(forward_rate1[i+1]-forward_rate1[i])*Delta_T;
		   }
		   Forward_Rate=SmartPointer<double>(forward_rate);
		   Forward_Rate1=SmartPointer<double>(forward_rate1);
		  
		   Nb_Random=2+Nb_Equity;
		   Correlation=new double *[Nb_Random];
		   for(int i=0;i<Nb_Random;i++){
			   Correlation[i]=new double [Nb_Random];
		   }
		   Correlation[0][0]=cor[0][0];
		   Correlation[1][1]=1.0;
		   for(int i=2;i<Nb_Random;i++){
			   Correlation[0][i]=cor[0][i-1];
			   Correlation[i][0]=cor[i-1][0];
		   }
		   for(int i=2;i<Nb_Random;i++){
			   for(int j=2;j<Nb_Random;j++){
				   Correlation[i][j]=cor[i-1][j-1];
			   }
		   }
		   double correl=(exp(A/Delta_T)-1.0)/A*sqrt(2.0*A/(1.0/Delta_T*(exp(2.0*A/Delta_T)-1)));
		   Correlation[0][1]=correl;
		   Correlation[1][0]=correl;
		   for(int i=2;i<Nb_Random;i++){
			   Correlation[1][i]=Correlation[0][i]*correl;
			   Correlation[i][1]=Correlation[0][i]*correl;
		   }

		   Cholesky(Correlation,Nb_Random);
	   }

	  
	   //! TO DO add const 
	   Basic_Information(Basic_Information &BI)
	   {
		   Nb_Scens=BI.Nb_Scens;
		   Antithetic=BI.Antithetic;
		   Nb_Equity=BI.Nb_Equity;
		   Nb_Bond=BI.Nb_Bond;
		   Nb_Rate=BI.Nb_Rate;
		   Nb_Step=BI.Nb_Step;
		   Nb_Initial_Rate=BI.Nb_Initial_Rate;
		   Nb_Vol=BI.Nb_Vol;
		   Nb_Random=BI.Nb_Random;
		   A=BI.A;
		   Sigma=BI.Sigma;
		   Delta_T=BI.Delta_T;
		   
		   double *error=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   error[i]=BI.Error[i];
		   }
		   Error=SmartPointer<double>(error);

		   double *add_error=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   add_error[i]=BI.Add_Error[i];
		   }
		   Add_Error=SmartPointer<double>(add_error);
		   
		   int *max_try=new int [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   max_try[i]=BI.Max_Try[i];
		   }
		   Max_Try=SmartPointer<int>(max_try);

		   Vol_Equity=new double *[Nb_Equity];
		   for(int i=0;i<Nb_Equity;i++){
			   Vol_Equity[i]=new double [Nb_Vol];
			   for(int j=0;j<Nb_Vol;j++){
				   Vol_Equity[i][j]=BI.Vol_Equity[i][j];
			   }
		   }

		   int *vol_t=new int [Nb_Vol];
		   for(int i=0;i<Nb_Vol;i++){
			   vol_t[i]=BI.Vol_Time[i];
		   }
		   Vol_Time=SmartPointer<int>(vol_t);

		   double *m_bond=new double [Nb_Bond];
		   double *bond_coupon=new double [Nb_Bond];
		   double *bond_freq=new double [Nb_Bond];
		   for(int i=0;i<Nb_Bond;i++){
			   m_bond[i]=BI.Maturity_Bond[i];
			   bond_coupon[i]=BI.Bond_Coupon[i];
			   bond_freq[i]=BI.Bond_Freq[i];
		   }
		   Maturity_Bond=SmartPointer<double>(m_bond);
		   Bond_Coupon=SmartPointer<double>(bond_coupon);
		   Bond_Freq=SmartPointer<double>(bond_freq);

		   double *m_rate=new double [Nb_Rate];
		   int *rate_type=new int [Nb_Rate];
		   double *rate_period=new double[Nb_Rate];
		   for(int i=0;i<Nb_Rate;i++){
			   m_rate[i]=BI.Maturity_Rate[i];
			   rate_type[i]=BI.Rate_Type[i];
			   rate_period[i]=BI.Rate_Period[i];
		   }
		   Maturity_Rate=SmartPointer<double>(m_rate);
		   Rate_Type=SmartPointer<int>(rate_type);
		   Rate_Period=SmartPointer<double>(rate_period);
		   
		   double *forward=new double [Nb_Initial_Rate];
		   double *forward1=new double [Nb_Initial_Rate+1];
		   for(int i=0;i<Nb_Initial_Rate;i++){
			   forward[i]=BI.Forward_Rate[i];
			   forward1[i]=BI.Forward_Rate1[i];
		   }
		   forward1[Nb_Initial_Rate]=BI.Forward_Rate1[Nb_Initial_Rate];

		   Forward_Rate=SmartPointer<double>(forward);
		   Forward_Rate1=SmartPointer<double>(forward1);

		   Correlation=new double* [Nb_Random];
		   for(int i=0;i<Nb_Random;i++){
			   Correlation[i]=new double [Nb_Random];
			   for(int j=0;j<Nb_Random;j++){
				   Correlation[i][j]=BI.Correlation[i][j];
			   }
		   }
	   }

	   //! --Yuan  ajouter les tailles de table* dans les parametres
	   void copy(int nb_scen,int ant,int nb_eq,int nb_bond,int nb_rate,
								int nb_step,int nb_initial,int nb_vol,double a,double sigma,int delta_t,
								double *error_in,   int error_size,
								double *add_error_in,  int add_error_size,
								int *max_try_in,  int max_try_size,
								double **vol,       // d'ont need copy
								double *m_bond_in,  int m_bond_size,
								double *bond_coupon_in, int bond_coupon_size,
								double *bond_freq_in, int bond_freq_size,
								double *m_rate_in,  int m_rate_size,
								int *rate_type_in, int rate_type_size,
								double *rate_period_in, int rate_period_size,
								double *rate_in,    int rate_size,
								double **cor_in,    int cor_size/*,
								string *equity_in,  int equity_size*/)
	   {
		    //! Yuan Added copy (necessary or not necessary)
          	double *error  = new double[error_size];
			for(int i=0; i<error_size; i++){
				error[i] = error_in[i];
			}

			double *add_error  = new double[add_error_size];
			for(int i=0; i<add_error_size; i++){
				add_error[i] = add_error_in[i];
			}

			int *max_try  = new int[max_try_size];
			for(int i=0; i<max_try_size; i++){
				max_try[i] = max_try_in[i];
			}

			double *m_bond = new double[m_bond_size];
            for(int i=0; i<m_bond_size; i++)
				 m_bond[i] = m_bond_in[i];
			
			double *bond_coupon = new double[bond_coupon_size];
			for(int i=0;i<bond_coupon_size;i++){
				bond_coupon[i]=bond_coupon_in[i];
			}
			double *bond_freq = new double[bond_freq_size];
			for(int i=0;i<bond_freq_size;i++){
				bond_freq[i]=bond_freq_in[i];
			}

			double *m_rate = new double[m_rate_size];
            for(int i=0; i<m_rate_size; i++)
				 m_rate[i] = m_rate_in[i];

			int *rate_type=new int[rate_type_size];
			for(int i=0;i<rate_type_size;i++)
				rate_type[i]=rate_type_in[i];

			double *rate_period=new double[rate_period_size];
			for(int i=0;i<rate_period_size;i++)
				rate_period[i]=rate_period_in[i];

			double *rate   = new double[rate_size];
			for(int i=0; i<rate_size; i++)
				 rate[i] = rate_in[i];
			
		   Nb_Scens=nb_scen;
		   Antithetic=ant;
		   Nb_Equity=nb_eq;
		   Nb_Bond=nb_bond;
		   Nb_Rate=nb_rate;
		   Nb_Step=nb_step;
		   Nb_Initial_Rate=nb_initial;
		   Nb_Vol=nb_vol;
		   A=a;
		   Sigma=sigma;
		   Delta_T=delta_t;
		   Error=SmartPointer <double>(error);
		   Add_Error=SmartPointer <double>(add_error);
		   Max_Try=SmartPointer <int>(max_try);

		   Vol_Equity=new double *[Nb_Equity];
		   for(int i=0;i<Nb_Equity;i++){
			   Vol_Equity[i]=new double [Nb_Vol];
			   for(int j=0;j<Nb_Vol;j++){
				   Vol_Equity[i][j]=vol[i+1][j];
			   }
		   }

		   int *Vol_T=new int [Nb_Vol];
		   for(int i=0;i<Nb_Vol;i++){
			   Vol_T[i]=(int)vol[0][i];
		   }
		   Vol_Time=SmartPointer<int> (Vol_T);

		   Maturity_Bond=SmartPointer<double> (m_bond);
		   Bond_Coupon=SmartPointer<double>(bond_coupon);
		   Bond_Freq=SmartPointer<double>(bond_freq);
		   Maturity_Rate=SmartPointer<double>(m_rate);
		   Rate_Type=SmartPointer<int>(rate_type);
		   Rate_Period=SmartPointer<double>(rate_period);

		   double* forward_rate=new double [Nb_Initial_Rate];
		   double *forward_rate1=new double [Nb_Initial_Rate+1];
		   forward_rate1[0]=0;
		   for(int i=0;i<Nb_Initial_Rate;i++){
			   forward_rate1[i+1]=(i+1.0)/Delta_T*log(1+rate[i]/100);
			   if(i==0) forward_rate[i]=forward_rate1[i+1]*Delta_T;
			   if(i>0) forward_rate[i]=(forward_rate1[i+1]-forward_rate1[i])*Delta_T;
		   }
		   Forward_Rate=SmartPointer<double>(forward_rate);
		   Forward_Rate1=SmartPointer<double>(forward_rate1);

		   Nb_Random=cor_size;
		   Correlation=new double *[Nb_Random];
		   for(int i=0;i<Nb_Random;i++){
			   Correlation[i]=new double [Nb_Random];
		   }
		   Correlation[0][0]=cor_in[0][0];
		   Correlation[1][1]=1.0;
		   for(int i=2;i<Nb_Random;i++){
			   Correlation[0][i]=cor_in[0][i-1];
			   Correlation[i][0]=cor_in[i-1][0];
		   }
		   for(int i=2;i<Nb_Random;i++){
			   for(int j=2;j<Nb_Random;j++){
				   Correlation[i][j]=cor_in[i-1][j-1];
			   }
		   }
		   double correl=(exp(A/Delta_T)-1.0)/A*sqrt(2.0*A/(1.0/Delta_T*(exp(2.0*A/Delta_T)-1)));
		   if(correl > 1-(1e-10)){
			   correl = 1-(1e-10);
		   }
		   Correlation[0][1]=correl;
		   Correlation[1][0]=correl;
		   for(int i=2;i<Nb_Random;i++){
			   Correlation[1][i]=Correlation[0][i]*correl;
			   Correlation[i][1]=Correlation[0][i]*correl;
		   }
		   Cholesky(Correlation,Nb_Random);
		   delete[] rate;	   
	   }


	   void copy(Basic_Information BI){
		   Nb_Scens=BI.Nb_Scens;
		   Antithetic=BI.Antithetic;
		   Nb_Equity=BI.Nb_Equity;
		   Nb_Bond=BI.Nb_Bond;
		   Nb_Rate=BI.Nb_Rate;
		   Nb_Step=BI.Nb_Step;
		   Nb_Initial_Rate=BI.Nb_Initial_Rate;
		   Nb_Vol=BI.Nb_Vol;
		   Nb_Random=BI.Nb_Random;
		   A=BI.A;
		   Sigma=BI.Sigma;
		   Delta_T=BI.Delta_T;
		   double *error=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   error[i]=BI.Error[i];
		   }
		   Error=SmartPointer<double>(error);

		   double *add_error=new double [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   add_error[i]=BI.Add_Error[i];
		   }
		   Add_Error=SmartPointer<double>(add_error);

		   int *max_try=new int [Nb_Equity+1];
		   for(int i=0;i<Nb_Equity+1;i++){
			   max_try[i]=BI.Max_Try[i];
		   }
		   Max_Try=SmartPointer<int>(max_try);
		   
		   Vol_Equity=new double *[Nb_Equity];
		   for(int i=0;i<Nb_Equity;i++){
			   Vol_Equity[i]=new double [Nb_Vol];
			   for(int j=0;j<Nb_Vol;j++){
				   Vol_Equity[i][j]=BI.Vol_Equity[i][j];
			   }
		   }

		   int *vol_t=new int [Nb_Vol];
		   for(int i=0;i<Nb_Vol;i++){
			   vol_t[i]=BI.Vol_Time[i];
		   }
		   Vol_Time=SmartPointer<int>(vol_t);

		   double *m_bond=new double [Nb_Bond];
		   double *bond_coupon=new double [Nb_Bond];
		   double *bond_freq=new double [Nb_Bond];
		   for(int i=0;i<Nb_Bond;i++){
			   m_bond[i]=BI.Maturity_Bond[i];
			   bond_coupon[i]=BI.Bond_Coupon[i];
			   bond_freq[i]=BI.Bond_Freq[i];
		   }
		   Maturity_Bond=SmartPointer<double>(m_bond);
		   Bond_Coupon=SmartPointer<double>(bond_coupon);
		   Bond_Freq=SmartPointer<double>(bond_freq);

		   double *m_rate=new double [Nb_Rate];
		   int *rate_type=new int [Nb_Rate];
		   double *rate_period=new double [Nb_Rate];
		   for(int i=0;i<Nb_Rate;i++){
			   m_rate[i]=BI.Maturity_Rate[i];
			   rate_type[i]=BI.Rate_Type[i];
			   rate_period[i]=BI.Rate_Period[i];
		   }
		   Maturity_Rate=SmartPointer<double>(m_rate);
		   Rate_Type=SmartPointer<int>(rate_type);
		   Rate_Period=SmartPointer<double>(rate_period);

		   double *forward=new double [Nb_Initial_Rate];
		   double *forward1=new double [Nb_Initial_Rate+1];
		   for(int i=0;i<Nb_Initial_Rate;i++){
			   forward[i]=BI.Forward_Rate[i];
			   forward1[i]=BI.Forward_Rate1[i];
		   }
		   forward1[Nb_Initial_Rate]=BI.Forward_Rate1[Nb_Initial_Rate];

		   Forward_Rate=SmartPointer<double>(forward);
		   Forward_Rate1=SmartPointer<double>(forward1);

		   Correlation=new double* [Nb_Random];
		   for(int i=0;i<Nb_Random;i++){
			   Correlation[i]=new double [Nb_Random];
			   for(int j=0;j<Nb_Random;j++){
				   Correlation[i][j]=BI.Correlation[i][j];
			   }
		   }
	   }

	   ~Basic_Information(){
		  if(this->Correlation!= 0){
				for(int i = 0; i <Nb_Random; i++){
					if(this->Correlation[i]!= 0) delete[] this->Correlation[i];
				}
				delete [] Correlation;
		   }
		   if(this->Vol_Equity!= 0){
				for(int i = 0; i <Nb_Equity; i++){
					if(this->Vol_Equity[i]!= 0) delete[] this->Vol_Equity[i];
				}
				delete [] Vol_Equity;
		   }
	   }
	   
	   int nb_scens(){return Nb_Scens;}
	   int antithetic(){return Antithetic;}
	   int nb_eq(){return Nb_Equity;}
	   int nb_bond(){return Nb_Bond;}
	   int nb_rate(){return Nb_Rate;}
	   int nb_step(){return Nb_Step;}
	   int nb_vol(){return Nb_Vol;}
	   double a(){return A;}
	   double sigma(){return Sigma;}
	   int delta_t(){return Delta_T;}
	   double rate_period(int i){return Rate_Period[i];}
	   double error(int i){return Error[i];}
	   SmartPointer<int> vol_t(){return Vol_Time;}
	   double* vol_eq(int i){return Vol_Equity[i];}
	   double m_bond(int i){return Maturity_Bond[i];}
	   double m_rate(int i){return Maturity_Rate[i];}
	   double rate_type(int i){return Rate_Type[i];}
	   double bond_coupon(int i){return Bond_Coupon[i];}
	   double bond_freq(int i){return Bond_Freq[i];}
	   double forward_rate(int i){return Forward_Rate[i];}
	   double forward_rate1(int i){return Forward_Rate1[i];}
	   SmartPointer<double> forward_rate(){return Forward_Rate;}
	   SmartPointer<double> forward_rate1(){return Forward_Rate1;}
	   double cor(int i,int j){return Correlation[i][j];}
	   void increase_martingality(int i){Error[i]=Error[i]+Add_Error[i];}
	   int max_try(int i){return Max_Try[i];}
};

#endif