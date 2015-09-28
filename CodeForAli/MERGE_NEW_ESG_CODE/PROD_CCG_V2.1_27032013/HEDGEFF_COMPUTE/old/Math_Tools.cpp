#include "Math_Tools.h"
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
using namespace std;

double Math_Tools::maxim(double a, double b)
{
	if(a>b) return a;
	else return b;
}
double Math_Tools::minim(double a, double b)
{
	if(a>b) return b;
	else return a;
}

double Math_Tools::d0(double t,double x,double y,double sigma)
{
	return (1./(sigma*pow(t,0.5))*log(x/y)-0.5*sigma*pow(t,0.5));
}
double Math_Tools::d1(double t,double x,double y,double sigma)
{
	return (1./(sigma*pow(t,0.5))*log(x/y)+0.5*sigma*pow(t,0.5));
}
double f(double x)
{
	double y,a,b,c;
	y=1./(1+0.33267*x);
	a=0.4361836;
	b=-0.1201676;
	c=0.937298;
	return (1-UN_OVER_SQRT_2PI*exp(-x*x*0.5)*(a*y+b*y*y+c*y*y*y));
}
double Math_Tools::N(double x)
{
	
	if(x>=0)
		return f(x);
	else return 1-f(-x);	
}
double Math_Tools::DiffN(double x)
{

	return UN_OVER_SQRT_2PI*exp(-x*x*0.5);
}

double Math_Tools::delta_BS(double underlying,double strike,
							double time_to_maturity,double rate,double div,double sigma)
{
	return -N(
		d0(
		time_to_maturity,
		strike,
		underlying*exp(rate*time_to_maturity),		
		sigma)
		);
}

double Math_Tools::rho_BS(double underlying,double strike,
							double time_to_maturity,double rate,double div,double sigma)
{
	return -time_to_maturity*strike*exp(-rate*time_to_maturity)*N(
		d1(
		time_to_maturity,
		strike,
		underlying*exp(rate*time_to_maturity),		
		sigma)
		);
}

double Math_Tools::gamma_BS(double underlying,double strike,
							double time_to_maturity,double rate,double div,double sigma)
{
	return DiffN(
		d1(
		time_to_maturity,
		underlying*exp(rate*time_to_maturity),
		strike,
		sigma)
		)/(underlying*sigma*pow(time_to_maturity,0.5));
}
double convert_rate(double frequency,double annual_rate)
{
	return 1/frequency*(pow(1+annual_rate,frequency)-1);
}
double sum_tab(double *tab,int index_start,int index_end)
{
	double res=0;
	for(int i=index_start;i<=index_end;++i)
	{
		res+=tab[i];
	}
	return res;
}
int return_prev_payment_date(int start_time,int current_time,double frequency)
{
	int tmp;
	tmp=(current_time)-(current_time-start_time)% (int)(frequency);
	if(current_time==start_time)
		return start_time;
	else if ((current_time - start_time)%  (int)(frequency)==0)
		return current_time-(int)frequency;
	else if (tmp<frequency)
		return start_time;
	else 
		return tmp;
}
int return_next_payment_date(int start_time,int current_time,double frequency)
{
	 return return_prev_payment_date(start_time,current_time,frequency)+(int)frequency;
}
int return_nb_payment_dates(int start_time,int current_time,int end_time,double frequency)
{
	int res=(int)(((end_time-start_time)-((current_time-start_time)-(current_time-start_time)%(int)frequency))/frequency);
	if ((current_time-start_time)%(int)frequency==0 && current_time!=start_time) ++res;
	return res;
}

double* extract_zero_coupons(double** annual_zero_coupons_rates,int start_time,int current_time,int end_time,double frequency,int nb_steps_per_year,int& nb_payment_dates,double& zero_coupon_prec,double& zero_coupon_next,bool force_zero_coupon_prec)
{
	int next_payment_date,after_date,prev_payment_date,ann_ZC_rates_lower_bound,time_until_next_payment;
	
	double coef_linear_interp;
	nb_payment_dates=return_nb_payment_dates(start_time,current_time,end_time,frequency);
	double* res = new double[nb_payment_dates];
	after_date=current_time;
	for (int i=0;i<nb_payment_dates;++i)
	{
		next_payment_date=return_next_payment_date(start_time,after_date,frequency);
		after_date=next_payment_date+1;
		time_until_next_payment=next_payment_date-current_time;
		ann_ZC_rates_lower_bound=max((int)ceil((double)time_until_next_payment/nb_steps_per_year)-1,0);
		coef_linear_interp=(double)time_until_next_payment/nb_steps_per_year-ann_ZC_rates_lower_bound;
		//MBT 
		/*res[i]=convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound])
				+coef_linear_interp
				*(convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound+1])
				  -convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound])
				  );*/
		res[i]=annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]
				+coef_linear_interp
				*(annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound+1]
				  -annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]);
				  
		//MBT 
		//res[i]=exp(-res[i]*(double)time_until_next_payment/nb_steps_per_year);
		res[i]=1/pow((1+res[i]),(double)time_until_next_payment/nb_steps_per_year);
	}
	prev_payment_date=return_prev_payment_date(start_time,current_time,frequency);
	if((current_time-start_time)%(int)frequency==0 && current_time!=start_time)
		next_payment_date=return_next_payment_date(start_time,current_time,frequency);
	else
		next_payment_date=prev_payment_date;
	int inf_bound=(int)ceil(frequency/nb_steps_per_year)-1;
	//MBT (ajout de If)
	if(! force_zero_coupon_prec)
	{zero_coupon_prec=annual_zero_coupons_rates[prev_payment_date][inf_bound]
			+(frequency/nb_steps_per_year-inf_bound)
			*(annual_zero_coupons_rates[prev_payment_date][inf_bound+1]
			  -annual_zero_coupons_rates[prev_payment_date][inf_bound]);
				 
	zero_coupon_next=annual_zero_coupons_rates[next_payment_date][inf_bound]
			+(frequency/nb_steps_per_year-inf_bound)
			*(annual_zero_coupons_rates[next_payment_date][inf_bound+1]
			  -annual_zero_coupons_rates[next_payment_date][inf_bound]);
				  
	//MBT 
	//zero_coupon_prec=exp(-zero_coupon_prec*frequency/nb_steps_per_year);
	zero_coupon_prec=1/pow(1+zero_coupon_prec,frequency/nb_steps_per_year);
	zero_coupon_next=1/pow(1+zero_coupon_next,frequency/nb_steps_per_year);
	}
	return res;
}

//double* extract_zero_coupons(double** annual_zero_coupons_rates,int start_time,int current_time,int end_time,double frequency,int nb_steps_per_year,int& out_size,double& zero_coupon_prec)
//{
//	int swap_current_time=current_time-start_time;
//	int swap_end_time=end_time-start_time;
//	int inf_bound;
//	//int sup_bound;
//	int size=(int)((swap_end_time-(swap_current_time-swap_current_time%(int)frequency))/frequency);
//	if ((swap_current_time%(int)frequency==0) &&(swap_current_time!=0))
//		size++;
//	out_size=size;
//	int prev_payment_date;
//	double* res= new double[size];
//	int* payment_dates=new int[size];
//	payment_dates[0]=swap_current_time-swap_current_time%(int)frequency+(int)frequency;
//	if ((swap_current_time%(int)frequency==0) && (swap_current_time!=0))
//		payment_dates[0]-=(int)frequency;
//	double a;
//	int* next_payment_period=new int[size];
//	double coeff;
//	for(int i=1;i<size;++i)
//	{
//		payment_dates[i]=payment_dates[i-1]+(int)frequency;
//	}
//	for(int i=0;i<size;++i)
//	{
//		next_payment_period[i]=payment_dates[i]-swap_current_time;
//	}
//	for(int i=0;i<size;++i)
//	{
//		a=(double)next_payment_period[i]/nb_steps_per_year;
//
//		inf_bound=(int)ceil(a)-1;
//		coeff=(double)next_payment_period[i]/nb_steps_per_year;
//		res[i]=convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][inf_bound])
//				+coeff
//				*(convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][inf_bound+1])
//				  -convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][inf_bound])
//				  );
//		res[i]=exp(-res[i]*a);
//
//		
//	}	
//	a=(double)next_payment_period[0]/nb_steps_per_year;
//	inf_bound=(int)ceil(a)-1;
//	prev_payment_date=return_prev_payment_date(start_time,current_time,frequency);
//	zero_coupon_prec=convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[prev_payment_date][inf_bound])
//			+frequency/nb_steps_per_year
//			*(convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[prev_payment_date][inf_bound+1])
//			  -convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[prev_payment_date][inf_bound])
//			  );
//	
//	zero_coupon_prec=exp(-zero_coupon_prec*frequency/nb_steps_per_year);
//
//		
//	
//	return res;
//}
double compute_fix_leg(double* zero_coupons,double swap_rate,int size,double frequency,int nb_steps_per_year)
{
	double res=0;
	//double zero_coupon_prec;
//	double* zero_coupons=extract_zero_coupons(annual_zero_coupons_rates,start_time,current_time,end_time,frequency,nb_steps_per_year, size,zero_coupon_prec);
	res=sum_tab(zero_coupons,0,size-1);
	res*=swap_rate*frequency/nb_steps_per_year;
	res+=zero_coupons[size-1];
	return res;
}

double compute_floating_leg(
						double* zero_coupons,
						double zero_coupon_prec,
						double zero_coupon_next,
						bool coupon)
{
	//int size;
//	ofstream tt("compute_floating_leg.txt");
//	tt<<"extracting zero coupons..."<<endl;
//	double* zero_coupons=extract_zero_coupons(annual_zero_coupons_rates,start_time,current_time,end_time,frequency,nb_steps_per_year,size,zero_coupon_prec);
//	tt<<"size="<<size<<endl;
	if (! coupon)
		return (1+1./zero_coupon_prec-1)*zero_coupons[0];
	else
		return (1./zero_coupon_prec-1)*zero_coupons[0]+(1+1./zero_coupon_next-1)*zero_coupons[1];
	
}
double compute_swap_rate(double* zero_coupons,int size)
{
//	int size;
//	double zero_coupon_prec;
//	double tst=annual_zero_coupons_rates[0][0];
//	double* zero_coupons=extract_zero_coupons(annual_zero_coupons_rates,start_time,start_time,end_time,frequency,nb_steps_per_year, size,zero_coupon_prec);
	double somme=sum_tab(zero_coupons,0,size-1);
	return (1-zero_coupons[size-1])/sum_tab(zero_coupons,0,size-1);
}

double Math_Tools::swap_price(double** annual_zero_coupons_rates,
							  int start_time,
							  int end_time,
							  int current_time,
							  double& swap_rate,
							  double& zero_coupon_prec,
							  double& zero_coupon_next,
							  double frequency,
							  int nb_steps_per_year,
							  bool force_swap_rate,
							  bool force_zero_coupon_prec
							  )
{
	double floating_leg;
	double fix_leg;
	double res;
	int size;
	bool coupon=false;
	double* zero_coupons=extract_zero_coupons(annual_zero_coupons_rates,start_time,current_time,end_time,frequency,nb_steps_per_year, size,zero_coupon_prec,zero_coupon_next, force_zero_coupon_prec);
	//double tst=annual_zero_coupons_rates[0][0];
	if (((current_time - start_time)%  (int)(frequency)==0) && current_time<end_time && start_time!=current_time)
		coupon=true;
	
	if(current_time==start_time && !force_swap_rate)
		swap_rate=compute_swap_rate(zero_coupons,size)*(double)nb_steps_per_year/frequency;	
	floating_leg=compute_floating_leg(zero_coupons,zero_coupon_prec,zero_coupon_next,coupon);
	fix_leg=compute_fix_leg(zero_coupons,swap_rate,size,frequency,nb_steps_per_year);
	res= fix_leg-floating_leg;
	if (abs(res)<0.000000000001)
		res=0;

	//chth
	delete zero_coupons;

	return res;
}


double Math_Tools::put_price_BS(double underlying,double strike,
		double time_to_maturity,double rate,double div,double sigma)
{
	return
		strike*exp(-rate*time_to_maturity)*
		N(d1(time_to_maturity,strike,underlying*exp(rate*time_to_maturity),sigma))
		-underlying*N(d0(time_to_maturity,strike,underlying*exp(rate*time_to_maturity),sigma));
}	