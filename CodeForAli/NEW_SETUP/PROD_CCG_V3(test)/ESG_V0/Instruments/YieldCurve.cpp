#include "YieldCurve.h"

#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))


YC::YC(InterfaceESG0* &interface_Esg)
{
	myDebug_info_Calib=interface_Esg->Debug_info_Calib;
	myDebug_address_Calib=interface_Esg->Debug_address_Calib;
	indicator_scn=0;

	Init_parameters(interface_Esg->Number_Year*interface_Esg->Time_Step,interface_Esg->Time_Step,interface_Esg->SwapRateVector,interface_Esg->Coupon_period,interface_Esg->Interpolation_type,
				interface_Esg->Extrapolation_type_start,interface_Esg->Extrapolation_type_end,interface_Esg->Svensson_Extrapolation_End_option,interface_Esg->Nb_date,interface_Esg->Bootstrapping_security,interface_Esg->Prorata);


}
void YC::Init_parameters(int nb_Step,int time_step,double** &input_Swap_YC,string coupon_Period_Type,
						 string interpolation_Type,string extrapolation_Type_Start,string extrapolation_Type_End,string Svensson_Extrapolation_End_option,
						 int input_Swap_YC_Size,bool bootstrapping_Security, double prorata)
{
	Nb_Step=nb_Step;
	Delta_t=time_step;

	if(coupon_Period_Type=="ANNUAL") Coupon_Period=1;
	else if(coupon_Period_Type=="SEMI_ANNUAL") Coupon_Period=2;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Coupon_Period value does not exist");
		throw new GMXBException(message);
	}

	if(interpolation_Type=="LOGLINEAR") Interpolation_Type=1;
	else if(interpolation_Type=="LINEAR") Interpolation_Type=2;
	else if(interpolation_Type=="CONSTANT") Interpolation_Type=3;
	else if(interpolation_Type=="SPLINE") Interpolation_Type=4;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Interpolation_Type value does not exist");
		throw new GMXBException(message);
	}

	if(extrapolation_Type_Start=="LOGLINEAR") Extrapolation_Type_Start=1;
	else if(extrapolation_Type_Start=="LINEAR") Extrapolation_Type_Start=2;
	else if(extrapolation_Type_Start=="CONSTANT") Extrapolation_Type_Start=3;
	else if(extrapolation_Type_Start=="SPLINE") Extrapolation_Type_Start=4;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Extrapolation_Start_Curve value does not exist");
		throw new GMXBException(message);
	}

	if(extrapolation_Type_End=="LOGLINEAR") Extrapolation_Type_End=1;
	else if(extrapolation_Type_End=="LINEAR") Extrapolation_Type_End=2;
	else if(extrapolation_Type_End=="CONSTANT") Extrapolation_Type_End=3;
	else if(extrapolation_Type_End=="SPLINE") Extrapolation_Type_End=4;
	//else if(extrapolation_Type_End=="SVENSSON") Extrapolation_Type_End=5;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Extrapolation_End_Curve value does not exist");
		throw new GMXBException(message);
	}
	if(Svensson_Extrapolation_End_option=="Y") Svensson_option=1;
	else Svensson_option=0;
	Prorata=prorata;
	Bootstrapping_Security=bootstrapping_Security;
	ShockType="baseline";

	Inintialize_YC(input_Swap_YC,input_Swap_YC_Size);

}
void YC::Reset_YC()
{
	ShockType="baseline";

	for (int i = 0; i < Ouput_Swap_YC_Size; i++){
		Ouput_Swap_YC[0][i]=Ouput_Swap_Initial_YC[0][i];
		Ouput_Swap_YC[1][i]=Ouput_Swap_Initial_YC[1][i];
	}
	Inintialize_YC();
	
}
void YC::Inintialize_YC(double ** &input_Swap,int input_Swap_Size)
{
	ShockType="baseline";
	int Number_Total_Date_Interpol_tmp=100;
	Ouput_Swap_YC=new double * [2];
	/*int*/ Ouput_Swap_YC_Size = (Number_Total_Date_Interpol_tmp-1)*Coupon_Period+12;
	Ouput_Swap_YC[0]=new double  [Ouput_Swap_YC_Size];
	Ouput_Swap_YC[1]=new double  [Ouput_Swap_YC_Size];
	for (int i = 0; i < Ouput_Swap_YC_Size; i++){
		if(i<12){Ouput_Swap_YC[0][i]=(i+1)/12.0;}
		else{Ouput_Swap_YC[0][i]=Ouput_Swap_YC[0][i-1]+1.0/Coupon_Period;}
	}

	Interpolate::Total_Interpolate(input_Swap[0],input_Swap[1],input_Swap_Size,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
		Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
	

	Number_Total_Date = Number_Total_Date_Interpol_tmp*Delta_t+1;
	Dates = new double[Number_Total_Date];
	ZCR_YC = new double[Number_Total_Date];	
	Forward_YC = new double[Number_Total_Date-1];
	Fwd_ZCB_YC = new double[Number_Total_Date-1];
	ZCB_Yield = new double[Number_Total_Date];
	Log_ZCB_Yield = new double[Number_Total_Date];
	INV_ZCB_Yield= new double[Number_Total_Date];

	Inintialize_YC();
	
	if (Svensson_option==1){
		Apply_Svensson_Extrapolation(input_Swap,input_Swap_Size);
	}
	
	Ouput_Swap_Initial_YC=new double * [2];
	Ouput_Swap_Initial_YC[0]=new double  [Ouput_Swap_YC_Size];
	Ouput_Swap_Initial_YC[1]=new double  [Ouput_Swap_YC_Size];
	for (int i = 0; i < Ouput_Swap_YC_Size; i++){
		Ouput_Swap_Initial_YC[0][i]=Ouput_Swap_YC[0][i];
		Ouput_Swap_Initial_YC[1][i]=Ouput_Swap_YC[1][i];
	}	
}
void YC::Apply_Svensson_Extrapolation(double ** &input_Swap,int input_Swap_Size)
{
	double lambda_1_temp=1.265;
	double lambda_2_temp=0.9;

	double dates_temp[4];
	for (int i=0;i<4;i++) dates_temp[i]=input_Swap[0][input_Swap_Size-4+i];

	double ZCR_temp[4];
	for (int i=0;i<4;i++) ZCR_temp[i]=GetZeroCouponRate(dates_temp[i]);

	double Ai_temp[4];
	for (int i=0;i<4;i++) Ai_temp[i]=(1.-exp(-dates_temp[i]/lambda_1_temp))/dates_temp[i]*lambda_1_temp;
	double Bi_temp[4];
	for (int i=0;i<4;i++) Bi_temp[i]=Ai_temp[i] - exp(-dates_temp[i]/lambda_1_temp) ;
	double Ci_temp[4];
	for (int i=0;i<4;i++) Ci_temp[i]=(1.-exp(-dates_temp[i]/lambda_2_temp))/dates_temp[i]*lambda_2_temp - exp(-dates_temp[i]/lambda_2_temp) ;
		
	double ratio_1_temp[3];
	for (int i=0;i<3;i++) ratio_1_temp[i]=(Bi_temp[i+1]-Bi_temp[i])/(Ai_temp[i+1]-Ai_temp[i]) ;
	double ratio_2_temp[3];
	for (int i=0;i<3;i++) ratio_2_temp[i]=(Ci_temp[i+1]-Ci_temp[i])/(Ai_temp[i+1]-Ai_temp[i]) ;
	double ratio_3_temp[3];
	for (int i=0;i<3;i++) ratio_3_temp[i]=(ZCR_temp[i+1]-ZCR_temp[i])/(Ai_temp[i+1]-Ai_temp[i]) ;

	double Beta_i_temp[4];	
	Beta_i_temp[3]=( (ratio_3_temp[2]-ratio_3_temp[0])/(ratio_1_temp[2]-ratio_1_temp[0])  - (ratio_3_temp[1]-ratio_3_temp[0])/(ratio_1_temp[1]-ratio_1_temp[0])    )
		/ ( (ratio_2_temp[2]-ratio_2_temp[0])/(ratio_1_temp[2]-ratio_1_temp[0])  - (ratio_2_temp[1]-ratio_2_temp[0])/(ratio_1_temp[1]-ratio_1_temp[0])    );
	
	Beta_i_temp[2]= (ratio_3_temp[1]-ratio_3_temp[0])/(ratio_1_temp[1]-ratio_1_temp[0])  -(ratio_2_temp[1]-ratio_2_temp[0])/(ratio_1_temp[1]-ratio_1_temp[0]) *Beta_i_temp[3];
	Beta_i_temp[1]= ratio_3_temp[0]-ratio_2_temp[0]* Beta_i_temp[3]- ratio_1_temp[0]* Beta_i_temp[2];
	Beta_i_temp[0]= ZCR_temp[0] -Ci_temp[0]* Beta_i_temp[3]- Bi_temp[0]* Beta_i_temp[2]- Ai_temp[0]* Beta_i_temp[1];
		

	double epsilon = 0.0001;
	int input_Swap_Size_new=input_Swap_Size;
	int add_dates[4];
	add_dates[0]=30;
	add_dates[1]=40;
	add_dates[2]=50;
	add_dates[3]=100;
	if ( dates_temp[3] <= add_dates[0] -epsilon ) input_Swap_Size_new +=4;
	else if ( dates_temp[3] <= add_dates[1] -epsilon ) input_Swap_Size_new +=3;
	else if ( dates_temp[3] <= add_dates[2] -epsilon ) input_Swap_Size_new +=2;
	else if ( dates_temp[3] <= add_dates[3] -epsilon ) input_Swap_Size_new +=1;

	double **Input_Swap_YC_new;
	Input_Swap_YC_new=new double * [2];
	Input_Swap_YC_new[0]=new double  [input_Swap_Size_new];
	Input_Swap_YC_new[1]=new double  [input_Swap_Size_new];		
	for (int i=0;i<input_Swap_Size;i++){
		Input_Swap_YC_new[0][i]= input_Swap[0][i];
		Input_Swap_YC_new[1][i]= input_Swap[1][i];
	}		
	for (int i=0;i<(input_Swap_Size_new-input_Swap_Size);i++){
		Input_Swap_YC_new[0][input_Swap_Size_new -i-1]= add_dates[3-i];
	}		
		
	double *ZCB_Yield_new;
	ZCB_Yield_new=new double[Ouput_Swap_YC_Size];
	for (int i=0;i<Ouput_Swap_YC_Size;i++){
		if (Ouput_Swap_YC[0][i]<=dates_temp[3] ) ZCB_Yield_new[i]= GetDiscountFactor(Ouput_Swap_YC[0][i]);
		else ZCB_Yield_new[i]=1./exp(log(1.+ Beta_i_temp[0]+ Beta_i_temp[1]*(1. - exp(-Ouput_Swap_YC[0][i]/lambda_1_temp) )/ Ouput_Swap_YC[0][i]*lambda_1_temp +
			Beta_i_temp[2]*((1. - exp(-Ouput_Swap_YC[0][i]/lambda_1_temp) )/ Ouput_Swap_YC[0][i]*lambda_1_temp -exp(-Ouput_Swap_YC[0][i]/lambda_1_temp) )     +
			Beta_i_temp[3]*((1. - exp(-Ouput_Swap_YC[0][i]/lambda_2_temp) )/ Ouput_Swap_YC[0][i]*lambda_2_temp -exp(-Ouput_Swap_YC[0][i]/lambda_2_temp) ))*Ouput_Swap_YC[0][i]) ;
	}		
	double sum=ZCB_Yield_new[5]*(Coupon_Period==2)+ZCB_Yield_new[11];
	for (int i=12;i<Ouput_Swap_YC_Size;i++){
		sum+=ZCB_Yield_new[i];
		Ouput_Swap_YC[1][i]=(1.- ZCB_Yield_new[i] )/(1./Coupon_Period*sum);			
	}	
		
	for (int i=0;i<(input_Swap_Size_new-input_Swap_Size);i++){
		Input_Swap_YC_new[1][input_Swap_Size_new -i-1]= Ouput_Swap_YC[1][int(add_dates[3-i]-1)*Coupon_Period+12-1];
	}		

	Interpolate::Total_Interpolate(Input_Swap_YC_new[0],Input_Swap_YC_new[1],input_Swap_Size_new,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
		Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
	Inintialize_YC();

	delete [] Input_Swap_YC_new[0];
	delete [] Input_Swap_YC_new[1];
	delete [] Input_Swap_YC_new;
	delete [] ZCB_Yield_new;
}

void YC::Inintialize_YC()
{

	double* Bootstrap_ZC_rate_tmp=new double  [Ouput_Swap_YC_Size];
	for (int i = 0; i < Ouput_Swap_YC_Size; i++){  Bootstrap_ZC_rate_tmp[i]=Ouput_Swap_YC[1][i];  }
	Bootstrap_ZCR(Ouput_Swap_YC_Size,Ouput_Swap_YC,Bootstrap_ZC_rate_tmp,Coupon_Period,0.,Bootstrapping_Security);

	Dates[0]=0.0/Delta_t;
	if(Prorata==0) Dates[1]=1.0/Delta_t;
	else Dates[1]=1.0/Delta_t*Prorata;
	for (int i = 2; i < Number_Total_Date; i++){  Dates[i]=Dates[i-1]+1.0/Delta_t;  } 
	Interpolate::Total_Interpolate(Ouput_Swap_YC[0],Bootstrap_ZC_rate_tmp,Ouput_Swap_YC_Size,Dates,ZCR_YC,Number_Total_Date,
		Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);

	Calculate_FWd();
	Calculate_DF();
	
	delete [] Bootstrap_ZC_rate_tmp;
	if(myDebug_info_Calib)
		Write_Debug_File(myDebug_address_Calib, indicator_scn);

}
void YC::Bootstrap_ZCR(int tab_size,double** &interpolate_swap, double* &Bootstrap_ZCR_output, int coupon_frequency, double gilt_adj,  bool boot_sec)
{
	double accum_value=0.0;
	double strip_coupon1=0.0;
	for (int i = 0; i < tab_size; i++)
    {
		if(i==5 && coupon_frequency==2){
			strip_coupon1 = 1.0/(1+interpolate_swap[0][i]*interpolate_swap[1][i]/*/100.0*/);
		}
		else if(i>=11){
    		double yield_rate = interpolate_swap[1][i]/*/100.0*/;
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
    		double zero_yield = pow(strip_coupon1, -1.0/interpolate_swap[0][i]) - 1.0;
    		double adj_zero_yield = zero_yield + gilt_adj / 100/* 10000.0*/;
			if(i!=5){Bootstrap_ZCR_output[i] = adj_zero_yield /** 100.0*/;}
		}
	}

}
void YC::Calculate_FWd()
{
	//Converts the spot rates to forward rates
	Forward_YC[0] = log(1 + ZCR_YC[1] /*/ 100.0*/)*Dates[1]*Delta_t;
	Fwd_ZCB_YC[0] =exp(Forward_YC[0]/Delta_t);
	for (int i = 1; i < Number_Total_Date-1; i++)
	{		
		double rate_start = log(1.0 + ZCR_YC[i] /*/ 100.0*/) * Dates[i]*Delta_t;
		double rate_end = log(1.0 + ZCR_YC[i + 1] /*/ 100.0*/) * Dates[i+1]*Delta_t;
		Forward_YC[i] = rate_end - rate_start;
		Fwd_ZCB_YC[i] =exp(Forward_YC[i]/Delta_t);
	}
}
void YC::Calculate_DF()
{
	Log_ZCB_Yield[0]=0.0;
	INV_ZCB_Yield[0]=1.;
	ZCB_Yield[0] = 1.;
	for (int i = 1; i < Number_Total_Date; i++)	{
		Log_ZCB_Yield[i] =Dates[i]*log(1.0 + ZCR_YC[i] /*/ 100.0*/);
		ZCB_Yield[i] = exp(-Log_ZCB_Yield[i]);
		INV_ZCB_Yield[i] = 1./ZCB_Yield[i];
		 
	}
}
void YC::ShockSwap_Drift( double* &Input_Drift_Date,double* &Input_Drift_values,int Input_Drift_table_size, string& shockType)
{
	ShockType=shockType;

	double *Ouput_Swap_Drift_table_temp;
	Ouput_Swap_Drift_table_temp=new double [Ouput_Swap_YC_Size];
	for (int i=0;i<Ouput_Swap_YC_Size;i++) Ouput_Swap_Drift_table_temp[i]=0.;
	Interpolate::Total_Interpolate(Input_Drift_Date,Input_Drift_values,Input_Drift_table_size,Ouput_Swap_YC[0],Ouput_Swap_Drift_table_temp,Ouput_Swap_YC_Size,2,3,3);
	 
	for (int i=0; i<Ouput_Swap_YC_Size; i++)  Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i]+Ouput_Swap_Drift_table_temp[i];

	delete [] Ouput_Swap_Drift_table_temp;

	Inintialize_YC();
}void YC::ShockSwap( double startDate,double keyDate, double endDate,double shockSize,string& shockType)
{
	ShockType=shockType;
	if(shockType == "bucket_shock"){
		for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]>startDate && Ouput_Swap_YC[0][i]<endDate){
				if (Ouput_Swap_YC[0][i]<=keyDate)
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i]+ ((Ouput_Swap_YC[0][i]-startDate)/(keyDate-startDate))*shockSize;
				else
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + ((endDate-Ouput_Swap_YC[0][i])/(endDate-keyDate))*shockSize;
			}
			else
				Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i];
		}
	}
	else if(shockType == "parallel_shift"){
	    for (int i=0; i<Ouput_Swap_YC_Size; i++){
             //Ouput_Swap_YC[1][i] = max(0.05,Ouput_Swap_YC[1][i] + shockSize);
			 Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + shockSize;
		}
	}
	else if(shockType == "baseline"){
	    for (int i=0; i<Ouput_Swap_YC_Size; i++)
             Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i];
	}
	else if(shockType == "bucket_first"){
	    for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]<endDate){
				if (Ouput_Swap_YC[0][i]<=keyDate)
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + shockSize;
				else
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + ((endDate-Ouput_Swap_YC[0][i])/(endDate-keyDate))*shockSize;
			}
			else
				Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i];
		}
	}
	else if(shockType == "bucket_last"){
	   for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]>startDate){
				if (Ouput_Swap_YC[0][i]<=keyDate)
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i]+ ((Ouput_Swap_YC[0][i]-startDate)/(keyDate-startDate))*shockSize;
				else
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + shockSize;
			}
			else
				Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i];
	   }
	}
	else if(shockType == "new_bucket_last")
	{
		int temp_size=0;
		for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]>startDate){
				if(Ouput_Swap_YC[0][i]>=keyDate){
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + shockSize;
					temp_size++;
				}
			}
			else
				temp_size++;
		}
	   double* Ouput_Swap_YC_DATES_TEMP=new double  [temp_size];
	   double* Ouput_Swap_YC_VALUES_TEMP=new double  [temp_size];
	   int temp_size2=0;
	   for (int i=0; i<Ouput_Swap_YC_Size; i++){
		   if (Ouput_Swap_YC[0][i]>startDate){
			   if(Ouput_Swap_YC[0][i]>=keyDate){
					Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
					Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
					temp_size2++;
				}
			}
			else{
				Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
				Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_Initial_YC[1][i];
				temp_size2++;
			}
	   }

		Interpolate::Total_Interpolate(Ouput_Swap_YC_DATES_TEMP,Ouput_Swap_YC_VALUES_TEMP,temp_size,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
			Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
		delete []  Ouput_Swap_YC_DATES_TEMP;
		delete []  Ouput_Swap_YC_VALUES_TEMP;
	}
	else if(shockType == "new_bucket_first")
	{
		int temp_size=0;
		for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]<endDate){
				if (Ouput_Swap_YC[0][i]<=keyDate){
					Ouput_Swap_YC[1][i] = Ouput_Swap_Initial_YC[1][i] + shockSize;
					temp_size++;
				}
			}
			else
				temp_size++;
		}
	   double* Ouput_Swap_YC_DATES_TEMP=new double  [temp_size];
	   double* Ouput_Swap_YC_VALUES_TEMP=new double  [temp_size];
	   int temp_size2=0;
		for (int i=0; i<Ouput_Swap_YC_Size; i++){
			if (Ouput_Swap_YC[0][i]<endDate){
				if (Ouput_Swap_YC[0][i]<=keyDate){
					Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
					Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
					temp_size2++;
				}
			}
			else{
				Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
				Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_Initial_YC[1][i];
				temp_size2++;
			}
		}
	   	Interpolate::Total_Interpolate(Ouput_Swap_YC_DATES_TEMP,Ouput_Swap_YC_VALUES_TEMP,temp_size,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
			Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
		delete [] Ouput_Swap_YC_DATES_TEMP;
		delete [] Ouput_Swap_YC_VALUES_TEMP;
	}

	else
	{
	    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "shockType does not exist, error for bucket type !");
		throw new GMXBException(message);
	}
	Inintialize_YC();
}
//void YC::ShockSwap( double startDate,double keyDate, double endDate,double shockSize,string& shockType)
//{
//	ShockType=shockType;
//	if(shockType == "bucket_shock"){
//		for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]>startDate && Ouput_Swap_YC[0][i]<endDate){
//				if (Ouput_Swap_YC[0][i]<=keyDate)
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i]+ ((Ouput_Swap_YC[0][i]-startDate)/(keyDate-startDate))*shockSize;
//				else
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + ((endDate-Ouput_Swap_YC[0][i])/(endDate-keyDate))*shockSize;
//			}
//			else
//				Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i];
//		}
//	}
//	else if(shockType == "parallel_shift"){
//	    for (int i=0; i<Ouput_Swap_YC_Size; i++){
//             //Ouput_Swap_YC[1][i] = max(0.05,Ouput_Swap_YC[1][i] + shockSize);
//			 Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + shockSize;
//		}
//	}
//	else if(shockType == "baseline"){
//	    for (int i=0; i<Ouput_Swap_YC_Size; i++)
//             Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i];
//	}
//	else if(shockType == "bucket_first"){
//	    for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]<endDate){
//				if (Ouput_Swap_YC[0][i]<=keyDate)
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + shockSize;
//				else
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + ((endDate-Ouput_Swap_YC[0][i])/(endDate-keyDate))*shockSize;
//			}
//			else
//				Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i];
//		}
//	}
//	else if(shockType == "bucket_last"){
//	   for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]>startDate){
//				if (Ouput_Swap_YC[0][i]<=keyDate)
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i]+ ((Ouput_Swap_YC[0][i]-startDate)/(keyDate-startDate))*shockSize;
//				else
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + shockSize;
//			}
//			else
//				Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i];
//	   }
//	}
//	else if(shockType == "new_bucket_last")
//	{
//		int temp_size=0;
//		for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]>startDate){
//				if(Ouput_Swap_YC[0][i]>=keyDate){
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + shockSize;
//					temp_size++;
//				}
//			}
//			else
//				temp_size++;
//		}
//	   double* Ouput_Swap_YC_DATES_TEMP=new double  [temp_size];
//	   double* Ouput_Swap_YC_VALUES_TEMP=new double  [temp_size];
//	   int temp_size2=0;
//	   for (int i=0; i<Ouput_Swap_YC_Size; i++){
//		   if (Ouput_Swap_YC[0][i]>startDate){
//			   if(Ouput_Swap_YC[0][i]>=keyDate){
//					Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
//					Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
//					temp_size2++;
//				}
//			}
//			else{
//				Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
//				Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
//				temp_size2++;
//			}
//	   }
//
//		Interpolate::Total_Interpolate(Ouput_Swap_YC_DATES_TEMP,Ouput_Swap_YC_VALUES_TEMP,temp_size,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
//			Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
//		delete []  Ouput_Swap_YC_DATES_TEMP;
//		delete []  Ouput_Swap_YC_VALUES_TEMP;
//	}
//	else if(shockType == "new_bucket_first")
//	{
//		int temp_size=0;
//		for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]<endDate){
//				if (Ouput_Swap_YC[0][i]<=keyDate){
//					Ouput_Swap_YC[1][i] = Ouput_Swap_YC[1][i] + shockSize;
//					temp_size++;
//				}
//			}
//			else
//				temp_size++;
//		}
//	   double* Ouput_Swap_YC_DATES_TEMP=new double  [temp_size];
//	   double* Ouput_Swap_YC_VALUES_TEMP=new double  [temp_size];
//	   int temp_size2=0;
//		for (int i=0; i<Ouput_Swap_YC_Size; i++){
//			if (Ouput_Swap_YC[0][i]<endDate){
//				if (Ouput_Swap_YC[0][i]<=keyDate){
//					Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
//					Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
//					temp_size2++;
//				}
//			}
//			else{
//				Ouput_Swap_YC_DATES_TEMP[temp_size2]=Ouput_Swap_YC[0][i];
//				Ouput_Swap_YC_VALUES_TEMP[temp_size2]=Ouput_Swap_YC[1][i];
//				temp_size2++;
//			}
//		}
//	   	Interpolate::Total_Interpolate(Ouput_Swap_YC_DATES_TEMP,Ouput_Swap_YC_VALUES_TEMP,temp_size,Ouput_Swap_YC[0],Ouput_Swap_YC[1],Ouput_Swap_YC_Size,
//			Interpolation_Type,Extrapolation_Type_Start,Extrapolation_Type_End);
//		delete [] Ouput_Swap_YC_DATES_TEMP;
//		delete [] Ouput_Swap_YC_VALUES_TEMP;
//	}
//
//	else
//	{
//	    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
//		sprintf_s(message, "shockType does not exist, error for bucket type !");
//		throw new GMXBException(message);
//	}
//	Inintialize_YC();
//}
YC::~YC()
{
	delete [] Dates;	
	delete [] ZCR_YC;
	delete [] Forward_YC;
	delete [] ZCB_Yield;
	delete [] Log_ZCB_Yield;
	delete [] Fwd_ZCB_YC;
	delete [] Ouput_Swap_YC[0];
	delete [] Ouput_Swap_YC[1];
	delete [] Ouput_Swap_YC;
	delete [] INV_ZCB_Yield;
	delete [] Ouput_Swap_Initial_YC[0];
	delete [] Ouput_Swap_Initial_YC[1];
	delete [] Ouput_Swap_Initial_YC;
	

}
double YC::GetDiscountFactor(double dat)
{
	return Interpolate::Total_Interpolate(Dates,ZCB_Yield,Number_Total_Date,dat,2,2,2);
}
double YC::GetZeroCouponRate(double dat)
{
	return Interpolate::Total_Interpolate(Dates,ZCR_YC,Number_Total_Date,dat,2,2,2);
}
double YC::GetFwd(double dat)
{
	return Interpolate::Total_Interpolate(Dates,Forward_YC,Number_Total_Date-1,dat,2,2,2);
}
double YC::GetSwapRate(double date_start, double date_end)
{

	double PV01=GetPV01(date_start,date_end);
	double Discount_factor_start_temp= GetDiscountFactor(date_start);
	double Discount_factor_end_temp= GetDiscountFactor(date_end);  
	double numerator=Discount_factor_start_temp-Discount_factor_end_temp;
	double result = numerator / PV01 / Discount_factor_start_temp;
	return result;
}
double YC::GetPV01(double date_start, double date_end)
{
	double epsilon = 0.0001;
	double PV01=0.;
	int indic=0;
	double time_temp=date_start+1./Coupon_Period;
	double Discount_factor_temp=0.;
	while(indic==0)
	{
		Discount_factor_temp = GetDiscountFactor(time_temp); 
		PV01+=1./Coupon_Period*Discount_factor_temp;
		time_temp+=1./Coupon_Period;
		if(abs(time_temp-date_end)<=epsilon){
			Discount_factor_temp =GetDiscountFactor(time_temp);   
			PV01+=1./Coupon_Period*Discount_factor_temp;
			indic=1;
		}
		else if(time_temp > date_end+epsilon){
			Discount_factor_temp =GetDiscountFactor(date_end);   
			PV01+=(1./Coupon_Period-(time_temp-date_end))*Discount_factor_temp;
			indic=1;			
		}
	}
	PV01/=GetDiscountFactor(date_start);  

	return PV01;	
}
void YC::Write_Debug_File(string DbgFilePath, int CurrentScen)
{
		ofstream output;
		string ss;
		//char debug_file_name[1000];
		ss = DbgFilePath + "Output_Yield_Curves_scn" + to_string(CurrentScen) + "_"+ShockType + ".csv";
		output.open(ss.c_str());
		output<<"TIME"<<";"<<"Swap_output"<<";"<<"TIME"<<";"<< "ZCB_Yield"<<";"<<"Log_ZCB_Yield"<<";"<<"INV_ZCB_Yield"<<";"<<"TIME"<<";"<<"ZCR_YC"<<";"<<"Forward_YC"<<";"<<"Fwd_ZCB_YC"<< endl;
		for(int i=0;i< Number_Total_Date ;i++){
			if(i< Ouput_Swap_YC_Size){
				output<<setprecision(10)<<Ouput_Swap_YC[0][i]<<";"<<setprecision(10)<<Ouput_Swap_YC[1][i]<<";";
				output<<setprecision(10)<<Dates[i]<<";"<<setprecision(10)<<ZCB_Yield[i]<<";"<<setprecision(10)<<Log_ZCB_Yield[i]<<";"<<setprecision(10)<<INV_ZCB_Yield[i] << ";";
				output<<setprecision(10)<<Dates[i]<<";"<<setprecision(10)<<ZCR_YC[i]<<";"<<setprecision(10)<<Forward_YC[i]<<";"<<setprecision(10)<<Fwd_ZCB_YC[i] <<endl;
			}
				else if(i < Number_Total_Date-1){
					output<<""<<";"<<""<<";";
					output<<setprecision(10)<<Dates[i]<<";"<<setprecision(10)<<ZCB_Yield[i]<<";"<<setprecision(10)<<Log_ZCB_Yield[i]<<";"<<setprecision(10)<<INV_ZCB_Yield[i] << ";";
					output<<setprecision(10)<<Dates[i]<<";"<<setprecision(10)<<ZCR_YC[i]<<";"<<setprecision(10)<<Forward_YC[i]<<";"<<setprecision(10)<<Fwd_ZCB_YC[i] <<endl;
				}
				else if(i == Number_Total_Date-1){
					output<<""<<";"<<""<<";";
					output<<""<<";"<<""<<";"<<""<<";"<<"" << ";";
					output<<setprecision(10)<<Dates[i]<<";"<<setprecision(10)<<ZCR_YC[i]<<";"<<setprecision(10)<<Forward_YC[i]<<";"<<setprecision(10)<<Fwd_ZCB_YC[i] <<endl;
				}
			}			
		output.close();

}	
YC::YC(const YC& myc)
{
	this->Nb_Step=myc.Nb_Step;
	this->Bootstrapping_Security=myc.Bootstrapping_Security;
	this->Delta_t=myc.Delta_t;
	this->Coupon_Period=myc.Coupon_Period;
	this->Interpolation_Type=myc.Interpolation_Type;
	this->Extrapolation_Type_Start=myc.Extrapolation_Type_Start;
	this->Extrapolation_Type_End=myc.Extrapolation_Type_End;
	this->Svensson_option=myc.Svensson_option;
	this->ShockType=myc.ShockType;
	this->Prorata=myc.Prorata;
	this->Ouput_Swap_YC_Size=myc.Ouput_Swap_YC_Size;
	this->Number_Total_Date=myc.Number_Total_Date;
	this->myDebug_info_Calib=myc.myDebug_info_Calib;
	this->myDebug_address_Calib=myc.myDebug_address_Calib;
	this->indicator_scn=myc.indicator_scn;


	this->Ouput_Swap_YC=new double * [2];
	this->Ouput_Swap_YC[0]=new double  [Ouput_Swap_YC_Size];
	this->Ouput_Swap_YC[1]=new double  [Ouput_Swap_YC_Size];
	this->Ouput_Swap_Initial_YC=new double * [2];
	this->Ouput_Swap_Initial_YC[0]=new double  [Ouput_Swap_YC_Size];
	this->Ouput_Swap_Initial_YC[1]=new double  [Ouput_Swap_YC_Size];
	for (int i = 0; i < Ouput_Swap_YC_Size; i++){
		this->Ouput_Swap_YC[0][i]=myc.Ouput_Swap_YC[0][i];
		this->Ouput_Swap_YC[1][i]=myc.Ouput_Swap_YC[1][i];
		this->Ouput_Swap_Initial_YC[0][i]=myc.Ouput_Swap_Initial_YC[0][i];
		this->Ouput_Swap_Initial_YC[1][i]=myc.Ouput_Swap_Initial_YC[1][i];

	}

	this->Dates = new double[Number_Total_Date];
	this->ZCR_YC = new double[Number_Total_Date];	
	this->ZCB_Yield = new double[Number_Total_Date];
	this->Log_ZCB_Yield = new double[Number_Total_Date];
	this->INV_ZCB_Yield= new double[Number_Total_Date];	
	for (int i = 0; i < Number_Total_Date; i++){
		this->Dates[i]=myc.Dates[i];
		this->ZCR_YC[i]=myc.ZCR_YC[i];
		this->ZCB_Yield[i]=myc.ZCB_Yield[i];
		this->Log_ZCB_Yield[i]=myc.Log_ZCB_Yield[i];
		this->INV_ZCB_Yield[i]=myc.INV_ZCB_Yield[i];
	}

	this->Forward_YC = new double[Number_Total_Date-1];
	this->Fwd_ZCB_YC = new double[Number_Total_Date-1];
	for (int i = 0; i < Number_Total_Date-1; i++){
		this->Forward_YC[i]=myc.Forward_YC[i];
		this->Fwd_ZCB_YC[i]=myc.Fwd_ZCB_YC[i];
	}

}