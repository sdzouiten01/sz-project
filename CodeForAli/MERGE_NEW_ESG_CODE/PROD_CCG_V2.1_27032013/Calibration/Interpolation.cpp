#include "Interpolation.h"
#include "Matrice.h"
#include <iostream>
#include "GMXBException.h"

using namespace std;

#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))

double Interpolation::loglinear_interp(double *arrayT,double *arrayY, int num, double term)
{ 	
	// Given the array of terms arrayT and the array of yields arrayY, and given a value 
	// of "term" this routine returns a loglinearly interpolated value "yield"
	
	int index_low = 0;
	int index_high = num - 1;
	int index = 0;
		
	// Finds the right place in the table by means of bisection
	while (index_high - index_low > 1)
	{
		index = (int)(index_low + index_high)/2;
		if (term > arrayT[index])
			index_low = index;
		else 
			index_high = index;
	}

	double h = arrayT[index_high] - arrayT[index_low];
	
	double a = (arrayT[index_high] - term) / h;
	double b = (term - arrayT[index_low]) / h;

	double ZC_rate = exp(a * log(arrayY[index_low]) + b * log(arrayY[index_high]));
	return ZC_rate;
}

double Interpolation::linear_interp(double *arrayT,double *arrayY, int num, double term)
{
	// Given the array of terms arrayT and the array of yields arrayY, and given a value 
	// of "term" this routine returns a linearly interpolated value "yield"

	int index_low = 0;
	int index_high = num - 1;
	int index = 0;
		
	// Finds the right place in the table by means of bisection
	while (index_high - index_low > 1)
	{
		index = (int)(index_low + index_high)/2;
		if (term > arrayT[index])
			index_low = index;
		else 
			index_high = index;
	}

	double h = arrayT[index_high] - arrayT[index_low];
	
	double a = (arrayT[index_high] - term) / h;
	double b = (term - arrayT[index_low]) / h;

	double ZC_rate = a * (arrayY[index_low]) + b * (arrayY[index_high]);
	return ZC_rate;
}

double Interpolation::const_interp(double *arrayT,double *arrayY, int num, double term)
{
	// Given the array of terms arrayT and the array of yields arrayY, and given a value 
	// of "term" this routine returns a linearly interpolated value "yield"
	double epsilon = 0.0001;
	int index_low = 0;
	int index_high = num - 1;
	int index = 0;
		
	// Finds the right place in the table by means of bisection
	while (index_high - index_low > 1)
	{
		index = (int)(index_low + index_high)/2;
		if (term-epsilon > arrayT[index])
			index_low = index;
		else 
			index_high = index;
	}

	if(term-epsilon > arrayT[index_low]){
		return arrayY[index_high];
	}
	else{
		return arrayY[index_low];
	}
}



void Interpolation::spline_coefficients(double* &arrayT, double* &arrayY, int num,
								double* &h_coefficient, double* &z_coefficient)
{
    
	int i = 0;
	
	Matrice Coeff(num -2);
	Matrice RHS(num-2,1);
	// h is defined as the time steps set in arrayT
	for (i = 0; i <= num - 2; i++){
		h_coefficient[i] = arrayT[i+1] - arrayT[i];
	}
	
	// Z(0) and Z(N) = 0.0	
	z_coefficient[0] = 0.0;			
	z_coefficient[num - 1] = 0.0;
	
	if (num == 3) // There is no set of linear equations to be solved
	{
		z_coefficient[1] = (6.0 * ((arrayY[2] - arrayY[1])/h_coefficient[1] - (arrayY[1] - arrayY[0])/h_coefficient[0]))
						 	/ (2.0*(h_coefficient[0] + h_coefficient[1]));
	}
	else if (num > 3) // Solve the set of linear equations
	{
		// Equations for Z(1)
		Coeff._matrice[0][0] = 2.0*(h_coefficient[0] + h_coefficient[1]);
		Coeff._matrice[0][1] = h_coefficient[1];
		RHS._matrice[0][0] = 6.0 * ((arrayY[2] - arrayY[1])/h_coefficient[1] - (arrayY[1] - arrayY[0])/h_coefficient[0]);

		// Equations for Z(N-1)
		Coeff._matrice[num-3][num-4] = h_coefficient[num-3];
		Coeff._matrice[num-3][num-3] = 2.0*(h_coefficient[num-3] + h_coefficient[num-2]);
		RHS._matrice[num-3][0] = 6.0 * ((arrayY[num-1] - arrayY[num-2])/h_coefficient[num-2] - (arrayY[num-2] - arrayY[num-3])/h_coefficient[num-3]);
	
		// Equations for Z(2) to Z(N-2)
		int j = 2;
		for (i = 1; i <= num - 4; i++)
		{
			Coeff._matrice[i][i-1] = h_coefficient[j-1];
			Coeff._matrice[i][i] = 2.0 * (h_coefficient[j-1] + h_coefficient[j]);
			Coeff._matrice[i][i+1] = h_coefficient[j];
			RHS._matrice[i][0] = 6*((arrayY[j+1] - arrayY[j])/h_coefficient[j] - (arrayY[j] - arrayY[j-1])/h_coefficient[j-1]);
			j++;
		}
		Coeff.Inverse2();
		Coeff.product(RHS);
		for (i = 1; i <= num-2; i ++){
			z_coefficient[i] = Coeff._matrice[i-1][0];
		}
		
	}
}



double Interpolation::spline_interp(double* &arrayT, double* &arrayY, int num, double term,
									double* &h_coefficient, double* &z_coefficient)
{
	// Takes in terms within arrayT and yields within arrayY which both have num elements and returns
	// the cubic spline interpolated value at term
	
	int index_low = 0;
	int index_high = num - 1;
	int index = 0;

	while (index_high - index_low > 1)
	{
		index = (int)(index_low + index_high)/2;
		if (term > arrayT[index])
			index_low = index;
		else 
			index_high = index;
	}
	// Interpolation using cubic spline is defined as
	// y = (z(i_high) * (x - x_low)^3 + z(i_low) * (x_high - x)^3) / (6 * h(i_low))
	// 		+ (y(i_high) / h(i_low) - h(i_low) / 6 * z(i_high)) * (x - x_low)
	//		+ (y(i_low) / h(i_low) - h(i_low) / 6 * z(i_low)) * (x_high - x)
	double yield = (z_coefficient[index_high] * pow((term - arrayT[index_low]),3.0) + z_coefficient[index_low]
						* pow((arrayT[index_high] - term),3.0)) / (6.0 * h_coefficient[index_low])
				+ ((arrayY[index_high]/h_coefficient[index_low]) - (h_coefficient[index_low]/6.0)*z_coefficient[index_high])
						* (term - arrayT[index_low]) 
				+ ((arrayY[index_low]/h_coefficient[index_low]) - (h_coefficient[index_low]/6.0)
						*z_coefficient[index_low])*(arrayT[index_high] - term);
	
	return yield;
	
}

void Interpolation::set_forward_rate(int last_term,int number_yields, double* &terms , double* &terms_yields,
									 int interpolation_type, int extrapolation_type_start, int extrapolation_type_end,
									 double* &initial_fwd, int coupon_frequency, double* swap_curve, bool boot_sec, int delta_t)
{
	int nb_interpol_swap_dates = (last_term-1)*coupon_frequency+12;
	double* interpol_swap_dates = new double[nb_interpol_swap_dates];
	double* interpol_swap_curve = new double[nb_interpol_swap_dates];
	interpol_swap_dates[0]=0;
	for (int i = 0; i < nb_interpol_swap_dates; i++){
		if(i<12){interpol_swap_dates[i]=(i+1)/12.0;}
		else{interpol_swap_dates[i]=interpol_swap_dates[i-1]+1.0/coupon_frequency;}
	}

	total_interpolation(number_yields,terms,terms_yields,nb_interpol_swap_dates,interpol_swap_dates,interpol_swap_curve,
				extrapolation_type_start,interpolation_type,extrapolation_type_end);

	for (int i = 0; i < nb_interpol_swap_dates; i++){
		swap_curve[i]=interpol_swap_curve[i];
	}

	strip_coupon(last_term, coupon_frequency, 0, interpol_swap_curve, interpol_swap_dates, swap_curve, boot_sec);


	int nb_interpol_zc_dates = last_term*delta_t+1;
	double* interpol_zc_dates = new double[nb_interpol_zc_dates];
	double* interpol_zc_curve = new double[nb_interpol_zc_dates];
	interpol_zc_dates[0]=0.0/delta_t;
	for (int i = 1; i < nb_interpol_zc_dates; i++){
		interpol_zc_dates[i]=interpol_zc_dates[i-1]+1.0/delta_t;
	}

	total_interpolation(nb_interpol_swap_dates,interpol_swap_dates,interpol_swap_curve,nb_interpol_zc_dates,interpol_zc_dates,interpol_zc_curve,
				extrapolation_type_start,interpolation_type,extrapolation_type_end);

	
	convert_to_forward(interpol_zc_curve,initial_fwd,nb_interpol_zc_dates-1);

	delete[] interpol_swap_dates;
	delete[] interpol_swap_curve;
	delete[] interpol_zc_dates;
	delete[] interpol_zc_curve;
	
}

void Interpolation::total_interpolation(int number_yields,double * terms,double * terms_yields,int nb_interpol_dates,double * interpol_dates,double* interpol_curve,int extrapolation_type_start,int interpolation_type,int extrapolation_type_end){
	double epsilon = 0.0001;

	double *h_coefficient = new double [number_yields];
	double *z_coefficient = new double [number_yields];
	if(interpolation_type==4 || extrapolation_type_start==4 || extrapolation_type_end==4){
		spline_coefficients(terms, terms_yields, number_yields,h_coefficient,z_coefficient);
	}

	for (int i = 0; i < nb_interpol_dates; i++){
		
		if(interpol_dates[i]<terms[0]-epsilon){
		//	interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],interpol_curve,extrapolation_type_start,h_coefficient,z_coefficient);
		interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],extrapolation_type_start,h_coefficient,z_coefficient);
		}

		else if(interpol_dates[i]<(terms[number_yields-1]+epsilon)){
			//interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],interpol_curve,interpolation_type,h_coefficient,z_coefficient);
		interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],interpolation_type,h_coefficient,z_coefficient);
		}

		else{
		//	interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],interpol_curve,extrapolation_type_end,h_coefficient,z_coefficient);
		interpol_curve[i] = interpol(number_yields,terms,terms_yields,interpol_dates[i],extrapolation_type_end,h_coefficient,z_coefficient);
		}
	}

	delete[] h_coefficient;
	delete[] z_coefficient;
}

double Interpolation::interpol(int number_yields,double* terms,double* terms_yields,double interpol_date,int interpolation_type,double* h_coefficient,double* z_coefficient){
	if(interpolation_type==1){
		return loglinear_interp(terms, terms_yields, number_yields, interpol_date);
	}
	else if(interpolation_type==2){
		return linear_interp(terms, terms_yields, number_yields, interpol_date);
	}
	else if(interpolation_type==3){
		return const_interp(terms, terms_yields, number_yields, interpol_date);
	}
	else if(interpolation_type==4){
		return spline_interp(terms, terms_yields, number_yields, interpol_date, h_coefficient, z_coefficient);
	}
	else
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Interpolation value does not exist");
		throw new GMXBException(message);	
	}
}


void Interpolation::strip_coupon(int last_term, int coupon_frequency, double gilt_adj, double* &terms_yields, double* init_terms, double* init_yields, bool boot_sec)
{
	double accum_value=0.0;
	double strip_coupon1=0.0;
	for (int i = 0; i < (last_term-1)*coupon_frequency+12; i++)
    {
		if(i==5 && coupon_frequency==2){
			strip_coupon1 = 1.0/(1+init_terms[i]*init_yields[i]/100.0);
		}
		else if(i>=11){
    		double yield_rate = init_yields[i]/100.0;
    		//The price of a ZCB
			double loc_coupon = (1.0 - yield_rate / int (coupon_frequency) * accum_value)
    							/(1.0 + yield_rate / int (coupon_frequency));
			if(loc_coupon>0){
				strip_coupon1 = loc_coupon;
			}
			if(loc_coupon<=0 && boot_sec == false){
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Impossible to bootstrap the swap curve");
				throw new GMXBException(message);
			}
		}

    	//Used by previous statement
		if((i==5 && coupon_frequency==2) || i>=11){
			accum_value += strip_coupon1;
    		double zero_yield = pow(strip_coupon1, -1.0/init_terms[i]) - 1.0;
    		double adj_zero_yield = zero_yield + gilt_adj / 10000.0;
			if(i!=5){terms_yields[i] = adj_zero_yield * 100.0;}
		}
	}

}


void Interpolation::convert_to_forward(double* &terms_yields,double* &initial_fwd, int last_term)
{
	//Converts the spot rates to forward rates
	initial_fwd[0] = log(1 + terms_yields[1] / 100.0);
	for (int i = 1; i < last_term; i++)
	{		
		double rate_start = log(1.0 + terms_yields[i] / 100.0) * i;
		double rate_end = log(1.0 + terms_yields[i + 1] / 100.0) * (i + 1);
		initial_fwd[i] = rate_end - rate_start;
	}
}


void Interpolation::set_discount_factor(int nmaturities, double* &initial_fwd,double* &initial_zcb, int delta_t)
{
	// Calculate initial zero coupon bond prices
	initial_zcb[0] = 1.0;
	for (int i = 0; i < nmaturities; i++)
	{
		initial_zcb[i+1] = initial_zcb[i] * exp(-initial_fwd[i]/(delta_t*1.0));
	}
}



// Moses interpolation method
void Interpolation::set_forward_rate(int last_term,int number_yields, double* &terms , double* &terms_yields,
									 double* &initial_fwd, double coupon_frequency, bool boot_sec)
{
	
	// last_term - Last term of the initial forward curve. In projection period
	//Identify last coupon payment period
	int last_given_term = (int) terms[number_yields - 1]; //last observed maturity in years
	    // Interpolate yields for coupon payment periods
	double *tmp_terms=new double [(int) (last_given_term*coupon_frequency+1)];
	double *tmp_yields=new double [(int) (last_given_term*coupon_frequency+1)];
	double *term=new double [last_term+1];
	double *yields=new double [last_term+1];
	
	Interpolation::fill_in_yields_old((int) (last_given_term*coupon_frequency),coupon_frequency, number_yields,terms, terms_yields,tmp_terms,tmp_yields);
	    // Convert the BEY's to spot rates
	double gilt_adj = 0. ; 
	
	Interpolation::strip_coupon(last_given_term, (int) coupon_frequency, gilt_adj, tmp_yields, boot_sec);
	// Interpolate yields between coupon payment periods
	
	Interpolation::fill_in_yields_old(last_term, 12.0/(double)coupon_frequency,(int) (last_given_term*coupon_frequency+1), tmp_terms, tmp_yields,term,yields);
    // Convert yields to forward rates
	Interpolation::convert_to_forward(yields,initial_fwd,last_term);
	delete [] tmp_terms;
	delete [] tmp_yields;
	delete [] term;
	delete [] yields;
}

void Interpolation::strip_coupon(int last_term, int coupon_frequency, double gilt_adj, double* &terms_yields, bool boot_sec)
{

	double* accum_value = new double[last_term * coupon_frequency+1];
	accum_value[0]=0.0;
	double strip_coupon1 = 0.0;

	for (int i = 1; i <= last_term * coupon_frequency; i++)
    {
    	double yield_rate = terms_yields[i]/100.0;
    
    	//The price of a ZCB
		double loc_coupon = (1.0 - yield_rate / int (coupon_frequency) * accum_value[i - 1])
    						 /(1.0 + yield_rate / int (coupon_frequency));
	    if(loc_coupon>0){
			strip_coupon1 = loc_coupon;
		}
		if(loc_coupon<=0 && boot_sec == false){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Impossible to bootstrap the swap curve");
			throw new GMXBException(message);
		}
    	//Used by previous statement
    	accum_value[i] = accum_value[i - 1] + strip_coupon1;
    	double zero_yield = pow(strip_coupon1, -double(coupon_frequency)/ i ) - 1.0;
    	double adj_zero_yield = zero_yield + gilt_adj / 10000.0;
    	terms_yields[i] = adj_zero_yield * 100.0;
	}
	delete [] accum_value;
}

void Interpolation::fill_in_yields_old(int last_period, double frequency, int number_yields ,
								   double* &terms,double* &terms_yields, double* &temp_terms,double* &temp_yields)
{
	int last_term = (int) (terms[number_yields - 1] * frequency);
	for (int i = 0; i < number_yields; i++)
	{
		terms[i] = frequency * terms[i];
	}
	double *h_coefficient = new double [number_yields];
    double *z_coefficient = new double [number_yields];
	spline_coefficients(terms, terms_yields, number_yields,h_coefficient,z_coefficient);
	for (int i = 0; i <= min(last_term, last_period); i++) // Interpolation
	{
		temp_terms[i] = i;
	    temp_yields[i] = spline_interp(terms, terms_yields, number_yields, i,h_coefficient,z_coefficient);	
	}
	for (int i = (last_term+1); i <= last_period; i++)    // Extrapolation
	{
		temp_yields[i]= loglinear_interp(temp_terms, temp_yields, last_term + 1, i);
	}
	delete [] h_coefficient;
    delete [] z_coefficient;	
}