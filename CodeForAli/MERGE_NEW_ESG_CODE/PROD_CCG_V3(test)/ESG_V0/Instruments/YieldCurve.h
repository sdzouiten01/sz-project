#ifndef YC_H
#define YC_H

#include <string>
#include "Interpolation.h"
#include "InterfaceESG0.h"
#include "GMXB_Assumption_Table.h"



using namespace std;

class YC{
public:

	//Number of Steps in th projection (from scenario configuration file), NOT USED into this Class
	int Nb_Step;
	//Bootstrapping indicator : Yes or No ? ---> not used into this class
	bool Bootstrapping_Security;
	//The time step : monthly=12, yearly=1, ....
	int Delta_t;
	// Coupon period for swap instrument: annual=1, semi-annual=0.5
	int Coupon_Period;
	//Interpolation type on the swap curve and the zero coupon curve
	int Interpolation_Type;
	//extrapolation type on the start of the curve (swap and zero coupon)
	int Extrapolation_Type_Start;
	//extrapolation in the end of the curve (swap and zero coupon)
	int Extrapolation_Type_End;

	int Svensson_option;

	//shock type on the swap yield curve : "bucket_shock", "parallel_shift", "baseline", "bucket_first", "bucket_last"....
	string ShockType;
	//Prorata on the current month : from today ( or reporting date) to the end of the current month.
	double Prorata;
	//Interpolated swap curve :Ouput_Swap_YC[0]=dates(with only coupon period), Ouput_Swap_YC[1]=interpolated swap values.
	double **Ouput_Swap_YC;
	
	double ** Ouput_Swap_Initial_YC;

	//size of Ouput_Swap_YC vector : number of dates in the vector Ouput_Swap_YC[0]
	int Ouput_Swap_YC_Size;
	//Final vector of Dates with the time step "1/Delta_t" over around 100 years
	double *Dates;
	// 100 year * Delta_t + 1 : size "Dates" vector
	int Number_Total_Date;
	// Zero coupon yield curve vector
	double * ZCR_YC;
	//Forward ( instantaneous) rates vector
	double *Forward_YC;
	//Discount factor yield curve
	double *ZCB_Yield;
	//Logarithm of the discount factor yield curve
	double *Log_ZCB_Yield;
	//The inverse of the discount factor vector
	double *INV_ZCB_Yield;
	//accumulation of fwd rate investment over one month 
	double *Fwd_ZCB_YC;
	//output debug file
	bool myDebug_info_Calib;
	string myDebug_address_Calib;
	int indicator_scn;

	//Generic constructor
	YC(){};
	//Constructor using a class of interface
	YC(InterfaceESG0 * &interface_Esg);

	//Destructor
	~YC();
	//Function used in the constructor to initialize the class parameters
	void Init_parameters(int nb_Step,int time_step,double ** &input_Swap_YC,string coupon_Period_Type,
		string interpolation_Type,string extrapolation_Type_Start,string extrapolation_Type_End,string Svensson_Extrapolation_End_option,
		int input_Swap_YC_Size,bool bootstrapping_Security, double prorata);
	
	//this function is used to initialize all vector : strating with the interpolation of the initial swap yield curve, then it's calling the "Inintialize_YC()" function
	void Inintialize_YC(double ** &input_Swap,int input_Swap_Size);
	//this function do the bootstrapping, interpolation of the zero coupon curve and the calculus of all other vector ( fwd, ZCB,...)
	void Inintialize_YC();
	//Bootstrapping of the swap yield curve: the output is the zero coupon curve not interpolated yet
	void Bootstrap_ZCR(int tab_size,double** &interpolate_swap, double* &Bootstrap_ZCR_output, int coupon_frequency, double gilt_adj,  bool boot_sec);
	//Calculating  Forward_YC and Fwd_ZCB_YC vector 
	void Calculate_FWd();
	//Calculating  Log_ZCB_Yield,ZCB_Yield, and INV_ZCB_Yield
	void Calculate_DF();
	//This function do a shock on the swap yield curve and calculate then all other curves
	void ShockSwap( double startDate,double keyDate, double endDate,double shockSize,string& shockType);
	//Using the dates "date_start" et "date_end" the function return the swap rate ( using the YC, and all assumptions like coupon period in this class)
	double GetSwapRate(double date_start, double date_end);
	//Using the dates "date_start" et "date_end" the function return the PVO1 value( using the YC, and all assumptions like coupon period in this class)
	double GetPV01(double date_start, double date_end);
	//Calculate Discount factor with maturity "dat" (yearly basis), with linear interpolation
	double GetDiscountFactor(double dat);
	//Calculate forward  value with maturity "dat" (yearly basis), with linear interpolation
	double GetFwd(double dat);
	//Write output file: all yield curves
	void Write_Debug_File(string DbgFilePath, int CurrentScen);
	// Copy constructor of Model_Point class
	YC(const YC& myc);
	void Apply_Svensson_Extrapolation(double ** &input_Swap,int input_Swap_Size);
	double GetZeroCouponRate(double dat);
	void ShockSwap_Drift( double* &Input_Drift_Date,double* &Input_Drift_values,int Input_Drift_table_size, string& shockType);
	void Reset_YC();
};

#endif