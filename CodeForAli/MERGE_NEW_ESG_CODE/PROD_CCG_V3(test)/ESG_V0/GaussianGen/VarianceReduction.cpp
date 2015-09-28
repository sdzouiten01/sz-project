#include "VarianceReduction.h"
#include <math.h>


// num_vol : nb of time steps for which a volatility is given as input (size of vol[])
// vol[k] : volatilities for duration vol_time[k] (vol_time[k] is expressed in months if BI.delta_t = 12)
// num_step : date (in months if BI.delta_t = 12) for which we want to calculate the volatility (the first month corresponds to num_step = 0)

// Interpolates the volatility at time num_step given volatilities at fixed time steps
double VarianceReduction::Interpole_Volatility(int index_position,int step_position){
	double output=0;
	bool finished = false; // Indicates whethter the correct interpolation interval has already been found
	if( Vol_Time_Size ==1) output=Vol_Equity[index_position][0];
	if(Vol_Time_Size>1){
		if(step_position>=Vol_Time[Vol_Time_Size-1]-1){
			// step by step extrapolation
			output=Vol_Equity[index_position][Vol_Time_Size-1];
			finished = true;
		}
		for(int j=2;j<=Vol_Time_Size&&finished==false;j++){
			if(step_position>=Vol_Time[Vol_Time_Size-j]-1){
				// step by step interpolation, matches Moses (takes the vol given for the next maturity)
				if(step_position==Vol_Time[Vol_Time_Size-j]-1){
					output=Vol_Equity[index_position][Vol_Time_Size-j];
				}
				else{
					output=Vol_Equity[index_position][Vol_Time_Size-j+1];
				}
				finished = true;
			}
		}
		if(finished==false){
			// if vol_time[0]>1
			output=Vol_Equity[index_position][0];
		}
	}
	return output;
}

//Calculate the black scholes scenario using the input random number table and the volatility of the index, the scenaio is saved on the ouput table
void VarianceReduction::Black_Scholes_Formula(double* random_input_tab,double* scen_ouput_tmp_tab, double vol){
	double A = exp(-vol*vol*Time_Step_t/2.0);
	double b = sqrt(Time_Step_t)*vol;
	double c=1;
	for(int j=0;j<NB_Scen_Antithetic;j++){
		c = exp(b*random_input_tab[j]);
		scen_ouput_tmp_tab[j]=A*c;		
		if(Antithetic==2)	scen_ouput_tmp_tab[j+NB_Scen_Antithetic]=A/c;	
	}
}

// Tests if the product over time X1*...*Xt of the returns of the equity is satisfactory (for equities).
// Xi = exp(-voli*voli*delta_t/2+voli*sqrt(delta_t)*Wi) where Wi are Gaussian independent variables Wi ~ N(0,1) 
bool VarianceReduction::Test_Martingale(double* scen_input_tmp_tab,double* cumul_scen_ouput_tmp_tab, int index_position, int step_position){
	bool control=1;
	double mean=0.;
	for(int j=0;j<NB_Scen;j++){ // tab is an array containing the input Xt
		cumul_scen_ouput_tmp_tab[j]*=scen_input_tmp_tab[j]; // traj is an array which memorizes the product  X1*...*Xt-1 (starts initialised at 1)
		mean+=cumul_scen_ouput_tmp_tab[j]; // E[X1*...*Xt] = 1;
	}
	mean=mean/NB_Scen;
	mean=abs(mean-1);
	if(mean>Error_table[index_position][step_position]*10.){control=0;}
	return control;
}
bool VarianceReduction::Test_beta(double* scen_input_tmp_tab,double* cumul_scen_ouput_tmp_tab, int index_position, int step_position){
	bool control=1;
	double mean=0.;
	for(int j=0;j<NB_Scen;j++){ // tab is an array containing the input Xt
		cumul_scen_ouput_tmp_tab[j]*=scen_input_tmp_tab[j]; // traj is an array which memorizes the product  X1*...*Xt-1 (starts initialised at 1)
		mean+=1./cumul_scen_ouput_tmp_tab[j]; // E[X1*...*Xt] = 1;
	}
	mean=mean/NB_Scen;
	mean=abs(mean-1);
	if(mean>Error_table[index_position][step_position]){control=0;}
	return control;
}
// Checks various martingality conditions and calls the equity model (Black Scholes)  to fill the scenario array.
bool VarianceReduction::Test_Equity(double** random_index_input_table, int index_position){
	
	bool control=1;

	double *scen_tab=new double [NB_Scen];
	double *tmp=new double [NB_Scen];
	for(int j=0;j<NB_Scen;j++) { tmp[j]=1.; scen_tab[j]=0.;}

	double vol=0.;

	for(int i=0;i<NB_Time_Step;i++)
	{
		vol=Interpole_Volatility(index_position,i);
		// Calls the Black and Scholes model.

		Black_Scholes_Formula(random_index_input_table[i],scen_tab,vol);
	// Notice that the call is made with 1/BI.delta_t() because BI.delta_t is a frequency, not a duration. Ex : BI.delta_t = 12 for monthly simulation.

		control=Test_Martingale(scen_tab,tmp, index_position, i);
		if(control==0) 
		{
			delete[] tmp;
			delete[] scen_tab;
			return 0;
		}
	}
	
	delete[] tmp;
	delete[] scen_tab;
	// tmp are used to test martingality conditions on a sum X1+...+Xt or a product Y1*...*Yt over time
	// tmp1[1] and tmp2[1] memorize the value of tmp at the previous time step, calculations are done on tmp1[0] ad tmp2[0].	
	return control;
}

// Fills the second half of the scenario array with antithetic variables.
void VarianceReduction::ANTITHETIC(double* random_input_tab){
	if(Antithetic==2){
		for(int j=0;j<NB_Scen_Antithetic;j++) random_input_tab[j+NB_Scen_Antithetic]=-random_input_tab[j];
	}
}

void VarianceReduction::Hull_White_Formula( double* random_IR_gaussian1_input_table, double* random_IR_gaussian2_input_table,double* input_integral_gaussian,double* scen_ouput_tmp_tab,int step_position){
	double t=step_position*Time_Step_t;
	double a_t=HW_a*t;
	double a_delta_t=HW_a*Time_Step_t;
	double et=exp(-a_t);
	double ed=exp(-a_delta_t);
	double const_1 = 1./et*sqrt((exp(2.*a_delta_t)-1.)/(2.*HW_a));
	double const_1_1 = sqrt(HW_sigma*HW_sigma/2./HW_a*(1.-ed*ed));
	double const_2 = HW_sigma*HW_sigma/(HW_a*HW_a)*(0.5*Time_Step_t-(0.25*et*et*(ed*ed-1)-et*(ed-1))/HW_a);
	double const_2_2 = HW_sigma*HW_sigma/(HW_a*HW_a)*0.5*(Time_Step_t-(1.-ed*ed)*et*et/HW_a*0.5);
	double const_3 = HW_sigma/HW_a;
	double const_4 = sqrt(Time_Step_t);
	double const_6_6 = HW_sigma*HW_sigma/2./HW_a/HW_a*(1.-ed)*(1.-ed*et*et);
	double previous_value=0.;
	double const_5 = 1;
	for(int i=0;i<Antithetic;i++){
		if(i==1) const_5 = -1;
		for(int j=0;j<NB_Scen_Antithetic;j++){
			int i_n_j = i*NB_Scen_Antithetic+j;
			previous_value=input_integral_gaussian[i_n_j];
			//input_integral_gaussian[i_n_j]=input_integral_gaussian[i_n_j]+const_1*const_5*random_IR_gaussian2_input_table[j];
			input_integral_gaussian[i_n_j]=ed*input_integral_gaussian[i_n_j]-const_1_1*const_5*random_IR_gaussian2_input_table[j]-const_6_6;

			//scen_ouput_tmp_tab[i_n_j]=exp(const_2+const_3*(const_5*const_4*random_IR_gaussian1_input_table[j]-et*(ed*input_integral_gaussian[i_n_j]-previous_value)));
			scen_ouput_tmp_tab[i_n_j]=exp(const_2_2+ const_3*const_5*const_4*random_IR_gaussian1_input_table[j] + (input_integral_gaussian[i_n_j]-previous_value)/HW_a );
		}
	}
	
}
// Checks various martingality conditions and calls the rate model (Hull and White)  to fill the scenario array.
bool VarianceReduction::Test_Rate(double** random_IR_gaussian1_input_table,double** random_IR_gaussian2_input_table    /* Scenario *s,Basic_Information &BI,int position_index,int position_step,double* tmp, double e*/){
	
	bool control=1;

	double *scen_tab=new double [NB_Scen];
	double *integral_guassian1_tab=new double [NB_Scen];
	double *tmp=new double [NB_Scen];
	for(int j=0;j<NB_Scen;j++) { tmp[j]=1.; scen_tab[j]=0.; integral_guassian1_tab[j]=0.;}

	for(int i=0;i<NB_Time_Step;i++)
	{
		// Needs to call antithetic now because the variables in scenario[3+position_index+BI.nb_rate()] will be
		// replaced one by one during the calculation of short_rate(...) and aren't also given as input in random.
		//ANTITHETIC(random_IR_gaussian2_input_table[i]);
		// Calls the Hull and White model.
		Hull_White_Formula(random_IR_gaussian1_input_table[i],random_IR_gaussian2_input_table[i], integral_guassian1_tab, scen_tab, i);
		// Tests if B(0,t)/beta(t) is worth 1 in average (for rates).
		// s->scenario[3+BI.nb_rate()] contains the second gaussian variable when flag==1.
		// Notice that the call is made with 1/BI.delta_t() because BI.delta_t is a frequency, not a duration. Ex : BI.delta_t = 12 for monthly simulation.
		control=Test_beta(scen_tab,tmp, 0, i);
		if(control==0) 
		{
			delete[] tmp;
			delete[] scen_tab;
			delete[] integral_guassian1_tab;
			return 0;
		}
	}
	
	delete[] tmp;
	delete[] scen_tab;
	delete[] integral_guassian1_tab;
	// tmp are used to test martingality conditions on a sum X1+...+Xt or a product Y1*...*Yt over time
	// tmp1[1] and tmp2[1] memorize the value of tmp at the previous time step, calculations are done on tmp1[0] ad tmp2[0].	
	return control;
}

VarianceReduction::~VarianceReduction(){
	for(int i=0; i<NB_index+1; i++) delete [] Error_table[i];
	for(int i=0; i<NB_index+1; i++) delete [] Martingale_level_try_tab[i];
	for(int i=0; i<NB_index; i++) delete [] Vol_Equity[i];
	delete [] Error_table;
	delete [] Martingale_level_try_tab;
	delete [] Vol_Equity;
	delete [] Vol_Time;
}
VarianceReduction::VarianceReduction(InterfaceESG0 * &interface_Esg)
{
	int *vol_Time_loc=new int[interface_Esg->Nb_vol];
		double **input_vol=new double *[interface_Esg->Nb_equity]; 
	for(int i=0;i<interface_Esg->Nb_equity;i++){input_vol[i]=new double [interface_Esg->Nb_vol];}

	for(int j=0;j<interface_Esg->Nb_vol;j++) vol_Time_loc[j]=(int)interface_Esg->Eq_Volatility_Matrix[0][j];
	for(int i=0;i<interface_Esg->Nb_equity;i++){
		for(int j=0;j<interface_Esg->Nb_vol;j++) input_vol[i][j]=interface_Esg->Eq_Volatility_Matrix[i+1][j];
	}
	
	double** error_table_local=new double *[interface_Esg->Nb_equity+1];
	for(int i=0;i<interface_Esg->Nb_equity+1;i++){
		error_table_local[i]=new double [interface_Esg->Number_Year*interface_Esg->Time_Step];
		for (int j=0;j<interface_Esg->Number_Year*interface_Esg->Time_Step;j++){
			error_table_local[i][j]=interface_Esg->Error_tab[i];
		}	
	}
	double **martingale_level_try_tab_local=new double *[interface_Esg->Nb_equity+1];
	for(int i=0;i<interface_Esg->Nb_equity+1;i++){
		martingale_level_try_tab_local[i]=new double [2];
		for (int j=0;j<2;j++){
			martingale_level_try_tab_local[i][0]=interface_Esg->Add_error_tab[i];
			martingale_level_try_tab_local[i][1]=interface_Esg->Max_try_tab[i];
		}
	}

	Init_VarianceReduction(interface_Esg->Nb_equity,interface_Esg->Number_Scenario,interface_Esg->Antithetic,interface_Esg->Number_Year*interface_Esg->Time_Step,
		interface_Esg->Time_Step,vol_Time_loc,interface_Esg->Nb_vol, input_vol,error_table_local,martingale_level_try_tab_local);

	delete [] vol_Time_loc;
	for(int i=0; i<interface_Esg->Nb_equity; i++){
		delete [] input_vol[i];
	}
	delete [] input_vol;

	for(int i=0; i<interface_Esg->Nb_equity+1; i++){
		delete [] martingale_level_try_tab_local[i];
		delete [] error_table_local[i];
	}
	delete [] martingale_level_try_tab_local;
	delete [] error_table_local;

}
void VarianceReduction::Init_VarianceReduction( int nB_index,int nB_Scen,int antithetic,int nB_Time_Step,int delta_t,int* vol_Time,int vol_Time_Size,	
									 double** vol_Equity,double** error_table,double **martingale_level_try_tab)
{
	NB_index=nB_index;
	NB_Scen=nB_Scen;
	Antithetic=antithetic;	
	NB_Scen_Antithetic=nB_Scen/antithetic;
	NB_Time_Step=nB_Time_Step;
	Time_Step_t=1./delta_t;

	Vol_Time_Size=vol_Time_Size;
	
	Vol_Time=new int [vol_Time_Size];
	for(int i=0; i<vol_Time_Size; i++) Vol_Time[i]=vol_Time[i];
	
	Vol_Equity=new double* [nB_index];
	for(int i=0; i<nB_index; i++){
		Vol_Equity[i]=new double [vol_Time_Size];
		for(int j=0; j<vol_Time_Size; j++)
			Vol_Equity[i][j]=vol_Equity[i][j];
	}

	Error_table=new double* [nB_index+1];
	for(int i=0; i<nB_index+1; i++){
		Error_table[i]=new double [nB_Time_Step];
		for(int j=0; j<nB_Time_Step; j++)
			Error_table[i][j]=error_table[i][j];
	}
	Martingale_level_try_tab=new double* [nB_index+1];
	for(int i=0; i<nB_index+1; i++){
		Martingale_level_try_tab[i]=new double [2];
		for(int j=0; j<2; j++)
			Martingale_level_try_tab[i][j]=martingale_level_try_tab[i][j];
	}

}
void VarianceReduction::Init_Calib_parrameters(double hW_a,double hW_sigma)
{
	HW_a=hW_a;
	HW_sigma=hW_sigma;
}
void VarianceReduction::Adjust_Error_table()
{
	double const1=0.;
	for(int i=0; i<NB_index+1; i++){
		const1=(Error_table[i][NB_Time_Step-1]-Error_table[i][0])/NB_Time_Step;
		if(const1==0.){
			for(int j=0; j<NB_Time_Step; j++)
				Error_table[i][j]=Error_table[i][j]+Martingale_level_try_tab[i][0];
			}
		else{
			for(int j=0; j<NB_Time_Step; j++)
				Error_table[i][j]=Error_table[i][j]+Martingale_level_try_tab[i][0]*(i+1)/NB_Time_Step;
		}
	}
}










