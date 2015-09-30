#include "BS_HW1F.h"
#include "../Instruments/Interpolation.h"
#undef max
#define max( a, b ) ((a) > (b) ? (a) : (b))
#undef min
#define min( a, b ) ((a) < (b) ? (a) : (b))

BS_HW1F::BS_HW1F(InterfaceESG0 * &interface_Esg):DIF_MODEL0()
{

	Time_step=interface_Esg->Time_Step;
	Delta_t=1./Time_step;
	Prorata=interface_Esg->Prorata;
	Antithetic=interface_Esg->Antithetic;
	NB_Time_Step=interface_Esg->Number_Year*interface_Esg->Time_Step;
	NB_index=interface_Esg->Nb_equity;
	NB_Scen=interface_Esg->Number_Scenario;
	NB_Scen_Antithetic=NB_Scen/Antithetic;

	X_t=new double* [NB_Time_Step+1];
	for(int i=0;i<NB_Time_Step+1;i++) X_t[i]=new double [NB_Scen];

	Z_t=new double* [NB_Time_Step+1];
	for(int i=0;i<NB_Time_Step+1;i++) Z_t[i]=new double [NB_Scen];
	
	Integrale_Vol_t=new double* [NB_index];
	for(int i=0;i<NB_index;i++) Integrale_Vol_t[i]=new double [NB_Time_Step];
	Integrale_Var_t=new double* [NB_index];
	for(int i=0;i<NB_index;i++) Integrale_Var_t[i]=new double [NB_Time_Step];

	Y_t=new double [Time_step*100];
	Q_t=new double [Time_step*100];	

	
	Vol_Equity=new double* [NB_index];	
	for(int i=0;i<NB_index;i++) Vol_Equity[i]=new double [NB_Time_Step+1];
	
	double *time_vector_local=new double [NB_Time_Step+1];
	time_vector_local[0]=0.;
	time_vector_local[1]=1./interface_Esg->Time_Step*interface_Esg->Prorata;
	for(int j=2;j<NB_Time_Step+1;j++) time_vector_local[j]= time_vector_local[j-1]+1./interface_Esg->Time_Step;

	double **input_vol=new double *[interface_Esg->Nb_equity]; 
	for(int i=0;i<interface_Esg->Nb_equity;i++){
		input_vol[i]=new double [interface_Esg->Nb_vol];
		for(int j=0;j<interface_Esg->Nb_vol;j++) input_vol[i][j]=interface_Esg->Eq_Volatility_Matrix[i+1][j]*interface_Esg->Eq_Volatility_Matrix[i+1][j];
	}
	double *input_time_vector_local=new double [interface_Esg->Nb_vol];
	for(int j=0;j<interface_Esg->Nb_vol;j++) input_time_vector_local[j]=interface_Esg->Eq_Volatility_Matrix[0][j]/interface_Esg->Time_Step;
	
	for(int i=0;i<interface_Esg->Nb_equity;i++){
		Interpolate::Total_Interpolate(input_time_vector_local,input_vol[i],interface_Esg->Nb_vol,time_vector_local,Vol_Equity[i],NB_Time_Step+1,2,3,3);
	}
	for(int i=0;i<interface_Esg->Nb_equity;i++){
		for(int j=0;j<NB_Time_Step+1;j++) Vol_Equity[i][j]=sqrt(Vol_Equity[i][j]);
	}

	delete [] time_vector_local;
	delete [] input_time_vector_local;
	
	for(int i=0; i<interface_Esg->Nb_equity; i++){
		delete [] input_vol[i];
	}
	delete [] input_vol;

}
BS_HW1F::~BS_HW1F()
{
	delete [] Y_t;
	delete [] Q_t;

	for(int i=0;i<NB_index;i++)   delete[] Vol_Equity[i];
	delete [] Vol_Equity;
	
	for(int i=0;i<NB_Time_Step+1;i++)   delete[] X_t[i];
	delete [] X_t;

	for(int i=0;i<NB_Time_Step+1;i++)   delete[] Z_t[i];
	delete [] Z_t;

	for(int i=0;i<NB_index;i++)   delete[] Integrale_Vol_t[i];
	delete [] Integrale_Vol_t;
	
	for(int i=0;i<NB_index;i++)   delete[] Integrale_Var_t[i];
	delete [] Integrale_Var_t;
}

void BS_HW1F::Initialize_Calib_parameters(double hW_Alpha,double hW_Sigma)
{
	HW_Alpha=hW_Alpha;
	HW_Sigma=hW_Sigma;

}
void BS_HW1F::Initialize_Class_Parameters(double** &input_Gaussian1_ShortRate_tab,double** &input_Gaussian2_ShortRate_tab)
{
	double local0_T1=exp(-HW_Alpha*Delta_t*Prorata);
	double local0_T=exp(-HW_Alpha*Delta_t);
	
	double local1_T1=local0_T1*local0_T1;
	double local1_T=local0_T*local0_T;
	
	double local3=HW_Sigma*HW_Sigma/4./HW_Alpha;

	double local3_T1=local3*(1.-local1_T1);
	//double local3_T=local3*(1.-local1_T);
	
	double local4_T=HW_Sigma/HW_Alpha*sqrt(Delta_t);
	double local4_T1=local4_T*sqrt(Prorata);

	double local5_T1=2.*local3*(1.-local0_T1)*(1.-local0_T1)/HW_Alpha;
	////////////////////////////////////////////////////
	///////////////////////////////////////////////////
	double Q_t_local_inter1=local1_T1;
	Q_t[0]=0.;
	Q_t[1]=local3_T1;
	for(int i=2;i<NB_Time_Step+1;i++){
		Q_t_local_inter1 *=local1_T;
		Q_t[i]=local3*(1.-Q_t_local_inter1);
	}
	////////////////////////////////////////////////////
	///////////////////////////////////////////////////
	for(int j=0;j<NB_Scen;j++) Z_t[0][j]=0.;
	double Z_t_local1_T1=local4_T1*local4_T1/2.;
	double Z_t_local1_T=local4_T*local4_T/2.;
	double Z_t_local_inter1=0.;
	for(int j=0;j<NB_Scen_Antithetic;j++){ 
		Z_t_local_inter1=local4_T1*input_Gaussian1_ShortRate_tab[0][j];
		Z_t[1][j]=Z_t_local1_T1+Z_t_local_inter1;
		if(Antithetic==2) Z_t[1][j + NB_Scen_Antithetic]=Z_t_local1_T1-Z_t_local_inter1;
	}
	for(int i=2;i<NB_Time_Step+1;i++){
		for(int j=0;j<NB_Scen_Antithetic;j++){ 
			Z_t_local_inter1=local4_T*input_Gaussian1_ShortRate_tab[i-1][j];
			Z_t[i][j]=Z_t[i-1][j]+Z_t_local1_T+Z_t_local_inter1;
			if(Antithetic==2) Z_t[i][j + NB_Scen_Antithetic]=Z_t[i-1][j + NB_Scen_Antithetic]+Z_t_local1_T-Z_t_local_inter1;
		}
	}
	////////////////////////////////////////////////////
	///////////////////////////////////////////////////
	double X_t_local1_T1=sqrt(2.*local3*(1.-local1_T1));
	double X_t_local1_T=sqrt(2.*local3*(1.-local1_T));
	for(int j=0;j<NB_Scen;j++) X_t[0][j]=0.;
	double X_t_local_inter0=0.;
	for(int j=0;j<NB_Scen_Antithetic;j++){ 
		X_t_local_inter0=X_t_local1_T1*input_Gaussian2_ShortRate_tab[0][j];
		X_t[1][j]=-local5_T1-X_t_local_inter0;
		if(Antithetic==2) X_t[1][j + NB_Scen_Antithetic]=-local5_T1+X_t_local_inter0;
	}
	double X_t_local2_T=2.*local3*(1.-local0_T)/HW_Alpha;
	double X_t_local_inter1=local1_T1*local0_T;
	double X_t_local_inter2=0.;
	double X_t_local_inter3=0.;
	for(int i=2;i<NB_Time_Step+1;i++){
		X_t_local_inter2=X_t_local2_T*(1.-X_t_local_inter1);
		X_t_local_inter1 *=local1_T;
		for(int j=0;j<NB_Scen_Antithetic;j++){ 
			X_t_local_inter3=X_t_local1_T*input_Gaussian2_ShortRate_tab[i-1][j];
			X_t[i][j]=local0_T*X_t[i-1][j]-X_t_local_inter2-X_t_local_inter3;
			if(Antithetic==2) X_t[i][j + NB_Scen_Antithetic]=local0_T*X_t[i-1][j + NB_Scen_Antithetic]-X_t_local_inter2+X_t_local_inter3;
		}
	}
	////////////////////////////////////////////////////
	///////////////////////////////////////////////////
	
	double const_local=HW_Alpha*Delta_t;
	Y_t_0=(1.-exp(-const_local))/HW_Alpha;
	
	Y_t[0]=(1.-exp(-const_local*Prorata))/HW_Alpha;
	for(int i=1;i<Time_step*100;i++) Y_t[i]=(1.-exp(-const_local*i))/HW_Alpha;
	
	for(int i=0;i<NB_index;i++){
		for(int j=0;j<NB_Time_Step;j++){
			Integrale_Var_t[i][j]=(Vol_Equity[i][j]*Vol_Equity[i][j]+Vol_Equity[i][j+1]*Vol_Equity[i][j+1])*Delta_t/2.;
			Integrale_Vol_t[i][j]=sqrt(Integrale_Var_t[i][j]);
		}
	}
	////////////////////////////////////////////////////
	///////////////////////////////////////////////////
}

void BS_HW1F::Calculate_Short_rate( double* &input_forward,double** &output_shortRate_tab)
{
	double local=0.;
	double local2_T1=Y_t[0]/Delta_t/Prorata;
	double local2_T=Y_t_0*Time_step;
	double local1_T1=Y_t[0]*local2_T1;
	double local1_T=Y_t_0*local2_T;
	for(int j=0;j<NB_Scen;j++) output_shortRate_tab[0][j]=input_forward[0]*100;
	local=input_forward[1]+local1_T1*Q_t[1];
	for(int j=0;j<NB_Scen;j++) output_shortRate_tab[1][j]=(local-local2_T1*X_t[1][j])*100.;
	for(int i=2;i<NB_Time_Step+1;i++){
		local=input_forward[i]+local1_T*Q_t[i];
		for(int j=0;j<NB_Scen;j++) output_shortRate_tab[i][j]=(local-local2_T*X_t[i][j])*100.;
	}
}
void BS_HW1F::Calculate_Beta( double* &input_inverse_ZCB,double** &output_beta_tab)
{
	double local=1./HW_Alpha;	
	double local_2=local*local;	
	double temp1=0.;
	double temp2=0.;
	for(int j=0;j<NB_Scen;j++) output_beta_tab[0][j]=1.;
	for(int i=1;i<NB_Time_Step+1;i++){
		temp1=-local_2*Q_t[i];
		temp2=input_inverse_ZCB[i];
		for(int j=0;j<NB_Scen;j++)
			output_beta_tab[i][j]=temp2*exp(Z_t[i][j]+local*X_t[i][j]+temp1);
	}
}
void BS_HW1F::Calculate_Notional_Rate( double &maturity, double* &input_Log_ZCB,double** &output_rate_tab)
{
	int nb_steps = (int) (maturity*Time_step+0.000001);
	double B=Y_t[nb_steps];
	double C1=-B*B;
	double C=0.;
	double D=0.;
	double cons1=0.;
	if (Prorata==1.) cons1=(exp(-(-input_Log_ZCB[nb_steps])/maturity)-1.)*100.;
	else cons1=(exp(-(input_Log_ZCB[0]-input_Log_ZCB[nb_steps])*Time_step/(nb_steps-1.+Prorata))-1.)*100.;
	for(int j=0;j<NB_Scen;j++) output_rate_tab[0][j]=cons1;

	for(int i=1;i<NB_Time_Step+1;i++){
		C=C1*Q_t[i];
		D=input_Log_ZCB[i]-input_Log_ZCB[i+nb_steps];
		for(int j=0;j<NB_Scen;j++) output_rate_tab[i][j]=(exp(-(D+B*X_t[i][j]+C)/maturity)-1.)*100.;
	}
}
double BS_HW1F::ZCB(double maturity,double* &input_Log_ZCB, int step_position,int scn_position){
	int nb_steps = (int) (maturity*Time_step+0.000001);
	if (step_position==0){
		double cons1=0.;
		if (Prorata==1.) cons1=-input_Log_ZCB[nb_steps];
		else cons1=input_Log_ZCB[0]-input_Log_ZCB[nb_steps];
		return cons1;
	}
	else{
		double B=Y_t[nb_steps];
		double C=-B*B*Q_t[step_position];
		double D=input_Log_ZCB[step_position]-input_Log_ZCB[step_position+nb_steps];
		double zcb=D+B*X_t[step_position][scn_position]+C;
		return zcb;
	}
}
void BS_HW1F::Calculate_Swap_Rate(double &maturity, double* &input_Log_ZCB,double** &output_rate_tab, double &coupon_period){
	int payment= (int) (maturity/coupon_period);
	double discount_factor = 1.0;
	double swap=0.;

	for(int i=0;i<NB_Time_Step+1;i++){
		for(int j=0;j<NB_Scen;j++){
			discount_factor = 1.0;
			swap=0.;
			for(int k=0;k<payment;k++){
				discount_factor=exp(ZCB((k+1)*coupon_period,input_Log_ZCB,i,j));
				swap+=discount_factor;
			}
			swap=((1-discount_factor)/(coupon_period*swap))*100.;
			output_rate_tab[i][j]=swap;
		}
	}
}
void BS_HW1F::Calculate_Bond_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab){
	double ZCB1=0.;
	double ZCB2=0.;
	
	for(int j=0;j<NB_Scen;j++) output_bond_tab[0][j]=0.;
	for(int i=1;i<NB_Time_Step+1;i++){
		for(int j=0;j<NB_Scen;j++){
			ZCB1=ZCB(maturity-Delta_t,input_Log_ZCB,i,j);
			ZCB2=ZCB(maturity,input_Log_ZCB,i-1,j);
			output_bond_tab[i][j]=(exp((ZCB1-ZCB2))-1.);
		}
	}
}
void BS_HW1F::Calculate_Bond_Coupon_Return(double maturity, double* &input_Log_ZCB,double** &output_bond_tab, double coupon,double coupon_period){
	
	double ZCB1=0.;
	double ZCB2=0.;
	double mat1 =0.;
	double mat2 =0.;
	for(int j=0;j<NB_Scen;j++) output_bond_tab[0][j]=0.;

	for(int i=1;i<NB_Time_Step+1;i++)
	{
		for(int j=0;j<NB_Scen;j++)
		{
			ZCB1=exp(ZCB(maturity-Delta_t,input_Log_ZCB,i,j));
			ZCB2=exp(ZCB(maturity,input_Log_ZCB,i-1,j));
			
			mat1 = maturity-coupon_period;
			while(mat1*Time_step>1){
				ZCB1 += (coupon/100.*coupon_period)*exp(ZCB(mat1-Delta_t,input_Log_ZCB,i,j));
				mat1 -= coupon_period;
			}
			if((int) mat1*Time_step==1){ZCB1 += coupon/100.*coupon_period;}
			mat2 = maturity-coupon_period;
			while(mat2*Time_step>0){
				ZCB2 += (coupon/100.*coupon_period)*exp(ZCB(mat2,input_Log_ZCB,i-1,j));
				mat2 -= coupon_period;
			}

			output_bond_tab[i][j]=ZCB1/ZCB2 - 1.;
		}
	}
}
void BS_HW1F::Calculate_Equity_Return(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab){
	double beta_previous=0.;
	double beta=0.;
	double temp1=0.;
	double temp2=0.;
	double conste=0.;
	
	for(int j=0;j<NB_Scen;j++)	output_equity_return_tab[0][j]=0.;
	for(int i=1;i<NB_Time_Step+1;i++)
	{
		temp1=exp(-Integrale_Var_t[index][i-1]/2.);
		conste=Integrale_Vol_t[index][i-1];
		for(int j=0;j<NB_Scen_Antithetic;j++){		
			temp2=exp(conste*input_gaussian[i-1][j]);			
			beta=input_beta[i][j];
			beta_previous=input_beta[i-1][j];
			output_equity_return_tab[i][j]=(beta/beta_previous*temp1*temp2-1.);
			if(Antithetic==2) {
				beta=input_beta[i][j +NB_Scen_Antithetic];
				beta_previous=input_beta[i-1][j +NB_Scen_Antithetic];
				output_equity_return_tab[i][j +NB_Scen_Antithetic]=(beta/beta_previous*temp1/temp2-1.);
			}
		}
	}
}
void BS_HW1F::Calculate_Equity_Return_With_Drift_without_IR_Model(int &index,double** &input_gaussian,double** &output_equity_return_tab,  double* &input_inverse_ZCB_Drift){
	double temp1=0.;
	double temp2=0.;
	double conste=0.;
	double Adjustment_drift=0.;
	
	for(int j=0;j<NB_Scen;j++)	output_equity_return_tab[0][j]=0.;
	for(int i=1;i<NB_Time_Step+1;i++)
	{
		temp1=exp(-Integrale_Var_t[index][i-1]/2.);
		conste=Integrale_Vol_t[index][i-1];
		Adjustment_drift=input_inverse_ZCB_Drift[i-1]*input_inverse_ZCB_Drift[i];
		for(int j=0;j<NB_Scen_Antithetic;j++){		
			temp2=exp(conste*input_gaussian[i-1][j]);			
			output_equity_return_tab[i][j]=(Adjustment_drift*temp1*temp2-1.);
			if(Antithetic==2)  output_equity_return_tab[i][j +NB_Scen_Antithetic]=(Adjustment_drift*temp1/temp2-1.);
		}
	}
}
void BS_HW1F::Calculate_Equity_Return_With_Drift(int &index,double** &input_gaussian, double** &input_beta,double** &output_equity_return_tab, 
												 double* &input_inverse_ZCB, double* &input_inverse_ZCB_Drift){
	double beta_previous=0.;
	double beta=0.;
	double temp1=0.;
	double temp2=0.;
	double conste=0.;
	double Adjustment_drift=0.;
	
	for(int j=0;j<NB_Scen;j++)	output_equity_return_tab[0][j]=0.;
	for(int i=1;i<NB_Time_Step+1;i++)
	{
		temp1=exp(-Integrale_Var_t[index][i-1]/2.);
		conste=Integrale_Vol_t[index][i-1];
		Adjustment_drift=input_inverse_ZCB[i-1]/input_inverse_ZCB_Drift[i-1]*input_inverse_ZCB_Drift[i]/input_inverse_ZCB[i];
		for(int j=0;j<NB_Scen_Antithetic;j++){		
			temp2=exp(conste*input_gaussian[i-1][j]);			
			beta=input_beta[i][j];
			beta_previous=input_beta[i-1][j];
			output_equity_return_tab[i][j]=(beta/beta_previous*Adjustment_drift*temp1*temp2-1.);
			if(Antithetic==2) {
				beta=input_beta[i][j +NB_Scen_Antithetic];
				beta_previous=input_beta[i-1][j +NB_Scen_Antithetic];
				output_equity_return_tab[i][j +NB_Scen_Antithetic]=(beta/beta_previous*Adjustment_drift*temp1/temp2-1.);
			}
		}
	}
}



