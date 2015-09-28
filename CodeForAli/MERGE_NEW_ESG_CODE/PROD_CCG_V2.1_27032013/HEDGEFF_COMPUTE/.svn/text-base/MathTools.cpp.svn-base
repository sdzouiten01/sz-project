#include "MathTools.h"
//#include "MathTable.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

namespace AXAHedgeEffective
{
	// return previous payment
	int return_prev_payment_date(int start_time,int current_time,int frequency)
	{
		int duration = current_time - start_time;
		int duration_frac = duration%frequency;
		int cur_time_int = current_time - duration_frac;

		if( current_time==start_time || cur_time_int == 0 || cur_time_int < frequency )
			return start_time;
		else
		{
			// current_time > start_time
			if ( duration_frac == 0 )
				return current_time - frequency;
			else
				return cur_time_int;
		}
	}

	inline int return_next_payment_date(int start_time,int current_time,int frequency)
	{
		return return_prev_payment_date(start_time,current_time,frequency)+frequency;
	}

	int return_nb_payment_dates(int start_time,int current_time,int end_time,int frequency)
	{
		int cur_dur = current_time-start_time;
		int res=(int)(((end_time-start_time)-(cur_dur - cur_dur%frequency))/frequency);
		if ( cur_dur%frequency==0 && current_time!=start_time)
			++res;
		return res;
	}

	void calc_zero_coupon(double** annual_zero_coupons_rates,
						  int start_time,
						  int end_time,
						  int current_time,
						  int frequency,
						  int nb_steps_per_year,
						  double& swap_rate,
						  double& zero_coupon_prec,
						  double& zero_coupon_next,
						  double & sumDF) // added 14/01/2011: to save the sum discount factor		  
	{
		int prev_payment_date = return_prev_payment_date(start_time,current_time,frequency);
		int next_payment_date;
		if((current_time-start_time)%frequency==0 && current_time!=start_time)
			next_payment_date = prev_payment_date + frequency;
		else
			next_payment_date=prev_payment_date;

		double freq = frequency/(double)nb_steps_per_year;
		int inf_bound = (int)ceil(freq)-1;
		zero_coupon_prec = annual_zero_coupons_rates[prev_payment_date][inf_bound]
				+(freq-inf_bound)*(annual_zero_coupons_rates[prev_payment_date][inf_bound+1] - annual_zero_coupons_rates[prev_payment_date][inf_bound]);

		zero_coupon_next=annual_zero_coupons_rates[next_payment_date][inf_bound]
				+(freq-inf_bound)*(annual_zero_coupons_rates[next_payment_date][inf_bound+1] - annual_zero_coupons_rates[next_payment_date][inf_bound]);

		zero_coupon_prec=1/pow(1+zero_coupon_prec, freq);
		zero_coupon_next=1/pow(1+zero_coupon_next, freq);
		
		// swap_rate here is stored as the fixed rate for swap
		// excluded the transaction cost (the spread)
		if(current_time == start_time) // only if at the trading day => have to calculate the fixed rate
		{
			next_payment_date = prev_payment_date + frequency;
			int nb_payment_dates = return_nb_payment_dates(start_time,current_time,end_time,frequency);
			int time_until_next_payment;
			int ann_ZC_rates_lower_bound;
			double coef_linear_interp;

			double sum, Bij;
			sum = Bij = 0;
			for ( int i = 0; i < nb_payment_dates; ++i, next_payment_date += frequency )
			{
				time_until_next_payment = next_payment_date-current_time;
				ann_ZC_rates_lower_bound = max((int)ceil((double)time_until_next_payment/(double)nb_steps_per_year)-1, 0);

				coef_linear_interp = time_until_next_payment/(double)nb_steps_per_year - ann_ZC_rates_lower_bound;
				Bij = annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]
							+coef_linear_interp*(annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound+1]-annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]); 

				Bij = 1.0/pow((1+Bij), time_until_next_payment/(double)nb_steps_per_year);
				sum += Bij;
			}
			swap_rate = (1-Bij)/sum*nb_steps_per_year/(double)frequency;

			// added 14/01/2011
			sumDF = sum/nb_steps_per_year*(double)frequency;
		}
	}

	// Modified 10/01/2011
	// to calculate rho of a swap => for an OLD version, it's used ONLY for a RECEIVER swap!
	//--------------------------------------------------------------------------------------
	// What's new:
	// 1. the function will return rho of a RECEIVER swap or -rho of a PAYER swap ( as a factor of shock amplitude!!!)
	// 2. For RECEIVER swap: fixed rate = swap rate - spread
	//    For PAYER swap:    fixed rate = swap rate + spread
	// 3. Still keeping the positive notional represent for a RECEIVER swap, negative for a PAYER one!
	double diff_swap_price(double** zero_coupons_rates,
						   double** zero_coupons_shocked_up_rates,
						   double** zero_coupons_shocked_down_rates,
						   int start_time,
						   int end_time,
						   int current_time,
						   double& swap_rate,
						   int frequency,
						   int nb_steps_per_year,
						   bool isReceiver // added 10/01/2011
						   )
	{
		double zero_coupon_prec, zero_coupon_next, sumDF; // modified 14/01/2011

		// swap_rate here is stored as the fixed rate for swap (WITHOUT transaction cost)

		calc_zero_coupon(zero_coupons_rates, start_time, end_time, current_time, frequency, nb_steps_per_year, swap_rate, zero_coupon_prec, zero_coupon_next,sumDF);
		
		// added 10/01/2011
		// swap rate with transaction cost included
		if (current_time == start_time) //only in the trading day
		{
			if (isReceiver) // RECEIVER swap
			{
				swap_rate -= g_math_swap_price_spread;
			}
			else // PAYER swap
			{
				swap_rate += g_math_swap_price_spread;
			}
		}
		
		//--TODO: re-put p1, p2 inside the 'else' loop
		double coupon;
		double p1 = swap_price(zero_coupons_shocked_up_rates, start_time, end_time, current_time, swap_rate, frequency, nb_steps_per_year, coupon,  false, true, zero_coupon_prec, zero_coupon_next, isReceiver);
		double p2 = swap_price(zero_coupons_shocked_down_rates, start_time, end_time, current_time, swap_rate, frequency, nb_steps_per_year, coupon, false, true, zero_coupon_prec, zero_coupon_next, isReceiver);

		// modified 14/01/2011
		if (current_time == start_time) // at trading time
		{
			
			return -sumDF;
		}
		else // at later time
		{
			return p1 - p2;
		}
	}

	double g_math_swap_price_spread;
	
	double* g_math_swap_price_trans_cost;

	// Modified 10/01/2011
	// to calculate present value of a swap => for an OLD version, it's used ONLY for a RECEIVER swap!
	//------------------------------------------------------------------------------------------------
	// What's new:
	// 1. the function will return PV of a RECEIVER swap or -PV of a PAYER swap
	// 2. For RECEIVER swap: fixed rate = swap rate - spread
	//    For PAYER swap:    fixed rate = swap rate + spread
	// 3. Still keeping the positive notional represent for a RECEIVER swap, negative for a PAYER one!
	double swap_price(double** annual_zero_coupons_rates,
					  int start_time,
					  int end_time,
					  int current_time,
					  double& swap_rate,
					  int frequency,
					  int nb_steps_per_year,
					  double& coupon,
					  bool isCalcSwapRate, /*= true*/
					  bool isShocked, /* = false */
					  double zero_coupon_prec, /* = 0 */
					  double zero_coupon_next,/* = 0 */
					  bool isReceiver /* = true */ // added 10/01/2011
					  )
	{
		int prev_payment_date = return_prev_payment_date(start_time,current_time,frequency);
		int next_payment_date = prev_payment_date + frequency;
		int nb_payment_dates = return_nb_payment_dates(start_time,current_time,end_time,frequency);
		int time_until_next_payment;
		int ann_ZC_rates_lower_bound;
		double coef_linear_interp;

		double sum, Bij;
		sum = Bij = 0.0;

		double zero_coupons_0, zero_coupons_1;
		zero_coupons_0 = zero_coupons_1 = 0;
		int after_date=current_time;
		double t_next_pay;

		double zero_coupon_rate_lb;
		double* cur_zero_rate = annual_zero_coupons_rates[current_time];
		for ( int i = 0; i < nb_payment_dates; ++i, next_payment_date += frequency )
		{
			time_until_next_payment = next_payment_date-current_time;
			t_next_pay = time_until_next_payment/(double)nb_steps_per_year;

			ann_ZC_rates_lower_bound = (int)ceil(t_next_pay)-1; 
			coef_linear_interp = t_next_pay - ann_ZC_rates_lower_bound;
			//MBT 
			/*res[i]=convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound])
					+coef_linear_interp
					*(convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound+1])
					  -convert_rate(frequency/nb_steps_per_year,annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound])
					  );*/

			zero_coupon_rate_lb = cur_zero_rate[ann_ZC_rates_lower_bound];
			Bij = zero_coupon_rate_lb
						+coef_linear_interp*(cur_zero_rate[ann_ZC_rates_lower_bound+1]-zero_coupon_rate_lb); 

			Bij = pow((1+Bij), -t_next_pay);
			/*
			double tmp = Bij;
			double Bij2 = 1/power((1+tmp), time_until_next_payment);
			double dif = Bij - Bij2;
			if ( abs(dif) > 1e-5 )
			{
				cout << "Warning: Error > 1e-6 " << dif << endl;
				Bij2 = 1/power((1+tmp), time_until_next_payment);
				exit(1);
			}
			*/
			sum += Bij;

			if ( i < 2 )
			{
				if ( i == 0 )
					zero_coupons_0 = Bij;
				if ( i == 1 )
					zero_coupons_1 = Bij;
			}
		}
		
		// modified 10/01/2011: 
		// fixed rate is different depend on PAYER or RECEIVER position
		//-------------------------------------------------------------
		if( current_time == start_time && isCalcSwapRate ) // if it's trading date and it's flagged to re-calculate the fixed rate
		{
			if (isReceiver) // receiver swap
			{
				swap_rate = (1.0-Bij)/sum*nb_steps_per_year/(double)frequency - g_math_swap_price_spread;
			}
			else // payer swap
			{
				swap_rate = (1.0-Bij)/sum*nb_steps_per_year/(double)frequency + g_math_swap_price_spread;
			}

			*g_math_swap_price_trans_cost = g_math_swap_price_spread*sum*(double)frequency/nb_steps_per_year;
		}

		double floating_leg = 0.0;
		coupon = 0.0;
		if (isShocked)
		{
			if (((current_time - start_time)%frequency==0) && current_time < end_time && start_time!=current_time)
			{
				floating_leg =  (1.0/zero_coupon_prec-1)*zero_coupons_0 + zero_coupons_1/zero_coupon_next;
				coupon = swap_rate*(double)frequency/nb_steps_per_year - (1.0/zero_coupon_prec-1)*zero_coupons_0;
			}
			else
				floating_leg = zero_coupons_0 / zero_coupon_prec;
		}
		else
		{
			int inf_bound = (int)ceil((double)frequency/nb_steps_per_year)-1;
			zero_coupon_prec = annual_zero_coupons_rates[prev_payment_date][inf_bound]
													+(frequency/(double)nb_steps_per_year-inf_bound)
													*(annual_zero_coupons_rates[prev_payment_date][inf_bound+1] - annual_zero_coupons_rates[prev_payment_date][inf_bound]);

			zero_coupon_prec=1/pow(1+zero_coupon_prec,frequency/(double)nb_steps_per_year);
			floating_leg = zero_coupons_0 / zero_coupon_prec;
			
			if (((current_time - start_time)%frequency==0) && current_time <= end_time && start_time!=current_time)
			{
				coupon = swap_rate*(double)frequency/nb_steps_per_year - (1.0/zero_coupon_prec-1)*zero_coupons_0;
			}
		}
		double fix_leg = sum*swap_rate*frequency/(double)nb_steps_per_year + Bij;

		// Yaokun: 29-06-2010
		//  swap value[t] do not contain coupon[t]
		double res = fix_leg - floating_leg - coupon; //10/01/2011: will give PV for a receiver swap and -PV for a payer swap!

		if (abs(res) < 1e-12)
			res=0.0;

		return res;
	}

/******************************************************************************
* old version codes
******************************************************************************/
/*
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
double* extract_zero_coupons(double** annual_zero_coupons_rates,int start_time,int current_time,int end_time,int frequency,int nb_steps_per_year,int& nb_payment_dates,double& zero_coupon_prec,double& zero_coupon_next,bool force_zero_coupon_prec)
{
	int next_payment_date,after_date,prev_payment_date,ann_ZC_rates_lower_bound,time_until_next_payment;
	
	double coef_linear_interp;
	nb_payment_dates=return_nb_payment_dates(start_time,current_time,end_time,frequency);
	double* res = new double[nb_payment_dates];
	after_date=current_time;
	for (int i=0;i<nb_payment_dates;++i)
	{
		next_payment_date=return_next_payment_date(start_time,after_date,frequency);

		//sFout2 << "- " << i << ", " << next_payment_date << ": ";

		after_date=next_payment_date+1;
		time_until_next_payment=next_payment_date-current_time;
		ann_ZC_rates_lower_bound=max((int)ceil((double)time_until_next_payment/(double)nb_steps_per_year)-1,0);
		coef_linear_interp=(double)time_until_next_payment/nb_steps_per_year-ann_ZC_rates_lower_bound;

		res[i]=annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]
				+coef_linear_interp
				*(annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound+1]
				  -annual_zero_coupons_rates[current_time][ann_ZC_rates_lower_bound]);
				  
		//MBT 
		res[i]=1/pow((1+res[i]),(double)time_until_next_payment/(double)nb_steps_per_year);
		//sFout2 << res[i] << endl;
	}

	prev_payment_date=return_prev_payment_date(start_time,current_time,frequency);
	if( (current_time-start_time)%frequency==0 && current_time!=start_time)
		next_payment_date=return_next_payment_date(start_time,current_time,frequency);
	else
		next_payment_date=prev_payment_date;
	int inf_bound=(int)ceil(frequency/(double)nb_steps_per_year)-1;
	//MBT (ajout de If)
	if(! force_zero_coupon_prec)
	{
		zero_coupon_prec=annual_zero_coupons_rates[prev_payment_date][inf_bound]
									+(frequency/(double)nb_steps_per_year-inf_bound)
										*(annual_zero_coupons_rates[prev_payment_date][inf_bound+1] - annual_zero_coupons_rates[prev_payment_date][inf_bound]);

		zero_coupon_next=annual_zero_coupons_rates[next_payment_date][inf_bound]
									+(frequency/(double)nb_steps_per_year-inf_bound)
										*(annual_zero_coupons_rates[next_payment_date][inf_bound+1] - annual_zero_coupons_rates[next_payment_date][inf_bound]);
				  
		//MBT 
		zero_coupon_prec=1/pow(1+zero_coupon_prec,frequency/(double)nb_steps_per_year);
		zero_coupon_next=1/pow(1+zero_coupon_next,frequency/(double)nb_steps_per_year);
	}
	return res;
}

double compute_fix_leg(double* zero_coupons,double swap_rate,int size,int frequency,int nb_steps_per_year)
{
	double res=0;
	res=sum_tab(zero_coupons,0,size-1);
	res*=swap_rate*frequency/(double)nb_steps_per_year;
	res+=zero_coupons[size-1];
	return res;
}

double compute_floating_leg(
						double* zero_coupons,
						double zero_coupon_prec,
						double zero_coupon_next,
						bool coupon)
{
	if (! coupon)
		return (1+1./zero_coupon_prec-1)*zero_coupons[0];
	else
	{
		//assert (zero_coupons[0] != 1);
		//sFout2 << "calc floating leg: " << zero_coupon_prec << ", " << zero_coupon_next << endl;
		//sFout << "- " << zero_coupons[0] << ", " << zero_coupons[1] << endl;
		return (1./zero_coupon_prec-1)*zero_coupons[0]+(1+1./zero_coupon_next-1)*zero_coupons[1];
	}
}
double compute_swap_rate(double* zero_coupons,int size)
{
	double somme=sum_tab(zero_coupons,0,size-1);
	//sFout2 << "Calc Swap Rate: " << zero_coupons[size-1] << ", " << somme;
	double tmp = (1-zero_coupons[size-1])/sum_tab(zero_coupons,0,size-1);
	return tmp;
}
double swap_price2(double** annual_zero_coupons_rates,
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
	//sFout2 << start_time << ", " << end_time << ", " << current_time << ": rate= " << swap_rate << ", " << zero_coupon_prec << ", " << zero_coupon_next << endl;
	double floating_leg;
	double fix_leg;
	double res;
	int size;
	bool coupon=false;
	double* zero_coupons=extract_zero_coupons(annual_zero_coupons_rates,start_time,current_time,end_time,frequency,nb_steps_per_year, size,zero_coupon_prec,zero_coupon_next, force_zero_coupon_prec);
	if (((current_time - start_time)%  (int)(frequency)==0) && current_time<end_time && start_time!=current_time)
		coupon=true;
	
	if(current_time==start_time && !force_swap_rate)
	{
		swap_rate=compute_swap_rate(zero_coupons,size)*(double)nb_steps_per_year/frequency;
		//sFout2 << ", " << swap_rate << endl;
	}

	floating_leg=compute_floating_leg(zero_coupons,zero_coupon_prec,zero_coupon_next,coupon);
	fix_leg=compute_fix_leg(zero_coupons,swap_rate,size,frequency,nb_steps_per_year);
	res= fix_leg-floating_leg;
	if (abs(res)<0.000000000001)
		res=0;

	//sFout2 << "fix/float: " << fix_leg << ", " << floating_leg << ", " << res << endl << endl;
	//chth
	delete zero_coupons;

	return res;
}
*/
}