#include "short rate model.h"
#include <math.h>

void Rate_Model::Hull_White(double** output1, double** output2,double* input,int position_step,double a,double sigma,double delta_t,Basic_Information &BI){
	double t=position_step*delta_t;
	double a_t=a*t;
	double a_delta_t=a*delta_t;
	double et=exp(-a_t);
	double ed=exp(-a_delta_t);
	double const_1 = 1/et*sqrt((exp(2*a_delta_t)-1)/(2*a));
	double const_2 = sigma*sigma/(a*a)*(0.5*delta_t-(0.25*et*et*(ed*ed-1)-et*(ed-1))/a);
	double const_3 = sigma/a;
	double const_4 = sqrt(delta_t);
	int n=BI.nb_scens()/BI.antithetic();
	for(int i=0;i<BI.antithetic();i++){
		for(int j=0;j<n;j++){
			int i_n_j = i*n+j;
			output1[position_step+1][i_n_j]=output1[position_step][i_n_j]+const_1*output2[position_step+1][i_n_j];
			double const_5 = 1;
			if(i%2==1){const_5 = -1;}
			output2[position_step+1][i_n_j]=exp(const_2+const_3*(const_5*const_4*input[j]-et*(ed*output1[position_step+1][i_n_j]-output1[position_step][i_n_j])));
		}
	}
	
	/*
	for(int i=0;i<BI.antithetic();i++){
		for(int j=0;j<n;j++){
			output1[position_step+1][i*n+j]=output1[position_step][i*n+j]+1/et*sqrt((exp(2*a_delta_t)-1)/(2*a))*output2[position_step+1][i*n+j];
			output2[position_step+1][i*n+j]=0;
			output2[position_step+1][i*n+j]+=sigma*sigma/(a*a)*(0.5*delta_t-(0.25*et*et*(ed*ed-1)-et*(ed-1))/a);
			output2[position_step+1][i*n+j]+=sigma/a*(pow(-1,(double) i)*input[j]*sqrt(delta_t)-et*ed*output1[position_step+1][i*n+j]+et*output1[position_step][i*n+j]);		
			output2[position_step+1][i*n+j]=exp(output2[position_step+1][j+i*n]);
		}
	}
	*/
}

double Rate_Model::interest_rate(double maturity,int position_step, double delta_t, double** zero_coupon){
	double rate = (-zero_coupon[position_step+1][(int) (maturity*delta_t/6)]);
	rate*=1./maturity;
	rate=(exp(-rate)-1)*100;
	return rate;
}

double Rate_Model::swap_rate(double maturity, int position_step, double delta_t,double coupon_period, double** zero_coupon){
	int payment= (int) (maturity/coupon_period);
	double discount_factor=1;
	double swap=0;
	int loc_1 = (int) (coupon_period*delta_t/6);
	for(int i=0;i<payment;i++){
		//double ZCB1 = (-zero_coupon[position_step+1][(int) ((i+1)*coupon_period*delta_t/6)]);
		//discount_factor=exp(ZCB1);
		discount_factor = zero_coupon[position_step+1][((i+1)*loc_1)];
		swap+=discount_factor;

	}
	swap=((1-discount_factor)/(coupon_period*swap))*100;
	return swap;
}

double Rate_Model::bond_return(double maturity,double position_step,double delta_t,double** zero_coupon1, double** zero_coupon2){
	double ZCB1 = (-zero_coupon1[(int) position_step+1][1+(int) (((maturity-1.0/delta_t)*delta_t)/6)]);
	double ZCB2 = (-zero_coupon2[(int) (position_step-1+1)][(int) ((maturity)*delta_t/6)]);
	double b_return=exp((ZCB1-ZCB2)*12.0)-1;
	return b_return*100;
}

double Rate_Model::bond_return_coupon(double maturity,double position_step,double delta_t,double** zero_coupon1, double** zero_coupon2, double coupon, double coupon_period){
	double ZCB1 = exp(-zero_coupon1[(int) position_step+1][1+(int) (((maturity-1.0/delta_t)*delta_t)/6)]);
	double ZCB2 = exp(-zero_coupon2[(int) (position_step-1+1)][(int) ((maturity)*delta_t/6)]);
	double mat1 = maturity-coupon_period;
	while(mat1*delta_t>1){
		ZCB1 += (coupon/100*coupon_period)*exp(-zero_coupon1[(int) position_step+1][1+(int) (((mat1-1.0/delta_t)*delta_t)/6)]);
		mat1 -= coupon_period;
	}
	if((int) mat1*delta_t==1){ZCB1 += coupon/100*coupon_period;}
	double mat2 = maturity-coupon_period;
	while(mat2*delta_t>0){
		ZCB2 += (coupon/100*coupon_period)*exp(-zero_coupon2[(int) (position_step-1+1)][(int) ((mat2)*delta_t/6)]);
		mat2 -= coupon_period;
	}
	
	double b_return=exp((log(ZCB1)-log(ZCB2))*12.0)-1;
	//double b_return=exp((ZCB1-ZCB2)*12.0)-1;
	return b_return*100;
}


double Rate_Model::ZCB(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t, double* B1, double* C1){
	//double B=(1-exp(-a*maturity))/a;
	
	int nb_steps = (int) (maturity*delta_t+0.000001);
	double B=B1[nb_steps];
	double C=C1[position_step+1];
	//double A=forward_rate1[position_step+1]-forward_rate1[(int)(position_step+maturity*delta_t+1)]+B*forward_rate[position_step+1]-sigma*sigma*(1-exp(-2*a*(position_step+1)/delta_t))*B*B/4.0/a;
	double A=forward_rate1[position_step+1]-forward_rate1[(position_step+nb_steps+1)]+B*forward_rate[position_step+1]-C*B*B;
	double zcb=A-B*short_rate;
	return zcb;
}

//15/05/09
double Rate_Model::interest_rate(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t, double* B1, double* C1){
	double rate=ZCB(maturity,forward_rate,forward_rate1,short_rate,position_step,delta_t,B1,C1);
	//double rate = (-zero_coupon[position_step+1][(int) (maturity*delta_t/6)]);
	rate*=1./maturity;
	rate=(exp(-rate)-1)*100;
	return rate;
}

double Rate_Model::swap_rate(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate,int position_step, double delta_t,double coupon_period, double* B1, double* C1){
	int payment= (int) (maturity/coupon_period);
	double discount_factor = 1.0;
	double swap=0;
	for(int i=0;i<payment;i++){
		//double ZCB1 = (-zero_coupon[position_step+1][(int) ((i+1)*coupon_period*delta_t/6)]);
		//discount_factor=exp(ZCB1);
		discount_factor=exp(ZCB((i+1)*coupon_period,forward_rate,forward_rate1,short_rate,position_step,delta_t,B1,C1));
		swap+=discount_factor;
	}
	swap=((1-discount_factor)/(coupon_period*swap))*100;
	return swap;
}

double Rate_Model::bond_return(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate1,double short_rate2,int position_step,double delta_t, double* B1, double*C1){
	double ZCB1=ZCB(maturity-1.0/delta_t,forward_rate,forward_rate1,short_rate1,position_step,delta_t,B1,C1);
	double ZCB2=ZCB(maturity,forward_rate,forward_rate1,short_rate2,position_step-1,delta_t,B1,C1);
	//double ZCB1 = (-zero_coupon1[(int) position_step+1][1+(int) (((maturity-1.0/delta_t)*delta_t)/6)]);
	//double ZCB2 = (-zero_coupon2[(int) (position_step-1+1)][(int) ((maturity)*delta_t/6)]);
	double b_return=exp((ZCB1-ZCB2)*delta_t)-1;
	return b_return*100;
}

double Rate_Model::bond_return_coupon(double maturity, SmartPointer<double> forward_rate,SmartPointer<double> forward_rate1,double short_rate1,double short_rate2,int position_step,double delta_t,double coupon, double coupon_period, double*B1, double* C1){
	double ZCB1=exp(ZCB(maturity-1.0/delta_t,forward_rate,forward_rate1,short_rate1,position_step,delta_t,B1,C1));
	double ZCB2=exp(ZCB(maturity,forward_rate,forward_rate1,short_rate2,position_step-1,delta_t,B1,C1));
	//double ZCB1 = (-zero_coupon1[(int) position_step+1][1+(int) (((maturity-1.0/delta_t)*delta_t)/6)]);
	//double ZCB2 = (-zero_coupon2[(int) (position_step-1+1)][(int) ((maturity)*delta_t/6)]);
	double mat1 = maturity-coupon_period;
	while(mat1*delta_t>1){
		ZCB1 += (coupon/100*coupon_period)*exp(ZCB(mat1-1.0/delta_t,forward_rate,forward_rate1,short_rate1,position_step,delta_t,B1,C1));
		mat1 -= coupon_period;
	}
	if((int) mat1*delta_t==1){ZCB1 += coupon/100*coupon_period;}
	double mat2 = maturity-coupon_period;
	while(mat2*delta_t>0){
		ZCB2 += (coupon/100*coupon_period)*exp(ZCB(mat2,forward_rate,forward_rate1,short_rate2,position_step-1,delta_t,B1,C1));
		mat2 -= coupon_period;
	}

	double b_return=exp((log(ZCB1)-log(ZCB2))*delta_t)-1;
	return b_return*100;
}
