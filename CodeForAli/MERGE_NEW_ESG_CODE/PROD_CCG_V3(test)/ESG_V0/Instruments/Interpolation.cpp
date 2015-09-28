#include "Interpolation.h"
#include "GMXB_Assumption_Table.h"


void Interpolate::Total_Interpolate(double *input_YC_dates,double *input_YC_values,int input_YC_size,double *interpol_YC_dates,double *interpol_YC_values,
									  int interpol_YC_size,int interpolation_Type,int extrapolation_Type_Start,int extrapolation_Type_End )
{
	double epsilon = 0.0001;

	double *h_coefficient = new double [input_YC_size];
	double *z_coefficient = new double [input_YC_size];
	if(interpolation_Type==4 || extrapolation_Type_Start==4 || extrapolation_Type_End==4){
		Spline_Coefficients(input_YC_dates,input_YC_values,input_YC_size,h_coefficient,z_coefficient);
	}

	for (int i = 0; i < interpol_YC_size; i++){
		
		if(interpol_YC_dates[i]<input_YC_dates[0]-epsilon)
			interpol_YC_values[i] = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates[i],extrapolation_Type_Start,h_coefficient,z_coefficient);

		else if(interpol_YC_dates[i]<(input_YC_dates[input_YC_size-1]+epsilon))
			interpol_YC_values[i] = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates[i],interpolation_Type,h_coefficient,z_coefficient);

		else
			interpol_YC_values[i] = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates[i],extrapolation_Type_End,h_coefficient,z_coefficient);
	}

	delete[] h_coefficient;
	delete[] z_coefficient;

}
double Interpolate::Total_Interpolate(double *input_YC_dates,double *input_YC_values,int input_YC_size,double interpol_YC_dates,int interpolation_Type,int extrapolation_Type_Start,int extrapolation_Type_End )
{
	double interpol_YC_values;
	double epsilon = 0.0001;
	double *h_coefficient = new double [input_YC_size];
	double *z_coefficient = new double [input_YC_size];
	if(interpolation_Type==4 || extrapolation_Type_Start==4 || extrapolation_Type_End==4){
		Spline_Coefficients(input_YC_dates,input_YC_values,input_YC_size,h_coefficient,z_coefficient);
	}

	if(interpol_YC_dates<input_YC_dates[0]-epsilon)
		interpol_YC_values = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates,extrapolation_Type_Start,h_coefficient,z_coefficient);
	else if(interpol_YC_dates<(input_YC_dates[input_YC_size-1]+epsilon))
		interpol_YC_values = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates,interpolation_Type,h_coefficient,z_coefficient);
	else
		interpol_YC_values = Interpol(input_YC_dates,input_YC_values,input_YC_size,interpol_YC_dates,extrapolation_Type_End,h_coefficient,z_coefficient);

	delete[] h_coefficient;
	delete[] z_coefficient;

	return interpol_YC_values;

}
void Interpolate::Spline_Coefficients(double* &arrayT, double* &arrayY, int num,double* &h_coefficient, double* &z_coefficient/*, double slope_end*/)
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
	z_coefficient[num - 1] = 0.0/*slope_end*/;
	
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
double Interpolate::Interpol(double* input_YC_dates,double* input_YC_values,int input_YC_size,double interpol_date,
							   int interpolation_type,double* h_coefficient,double* z_coefficient)
{
	if(interpolation_type==1){
		return Loglinear_Interp(input_YC_dates, input_YC_values, input_YC_size, interpol_date);
	}
	else if(interpolation_type==2){
		return Linear_Interp(input_YC_dates, input_YC_values, input_YC_size, interpol_date);
	}
	else if(interpolation_type==3){
		return Const_Interp(input_YC_dates, input_YC_values, input_YC_size, interpol_date);
	}
	else if(interpolation_type==4){
		return Spline_Interp(input_YC_dates, input_YC_values, input_YC_size, interpol_date, h_coefficient, z_coefficient);
	}
	else
	{
		return 0.;
		/*char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Interpolation value does not exist");
		throw new GMXBException(message);*/	
	}
}
double Interpolate::Loglinear_Interp(double *arrayT,double *arrayY, int num, double term)
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
	if(arrayY[index_low]<=0. || arrayY[index_high]<=0.)
	{
	    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "loglinear interpolation: negative values !");
		throw new GMXBException(message);
	}
	double ZC_rate = exp(a * log(arrayY[index_low]) + b * log(arrayY[index_high]));
	return ZC_rate;
}
double Interpolate::Linear_Interp(double *arrayT,double *arrayY, int num, double term)
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
double Interpolate::Const_Interp(double *arrayT,double *arrayY, int num, double term)
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
double Interpolate::Spline_Interp(double* &arrayT, double* &arrayY, int num, double term,double* &h_coefficient, double* &z_coefficient)
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

