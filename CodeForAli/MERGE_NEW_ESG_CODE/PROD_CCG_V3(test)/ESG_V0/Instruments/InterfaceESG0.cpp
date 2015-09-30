#ifndef _DLL_FILE_
#define _DLL_FILE_
#endif
#include "InterfaceESG0.h"
//#include "../../AHS_PARSER/WildcardManager.h"
#include "WildcardManager.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

InterfaceESG0::InterfaceESG0(const string& config)
{


	Config_file = config;
	GMXBParameters * Assumptions_local = new GMXBParameters(config.c_str(), "generator_assumptions");	//Linked to the "generator_assumption" tab

	string Use_Correct_Date_local=Assumptions_local->search("Use_Correct_Date").c_str();
	if(Use_Correct_Date_local=="Y") Use_Correct_Date=1;
	else if(Use_Correct_Date_local=="N") Use_Correct_Date=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Use_Correct_Date : Yes ""Y"" or No ""N"" ??");
		throw new GMXBException(message);
	}
		
	string Use_reported_date_Date_local=Assumptions_local->search("Use_reported_date_Date").c_str();
	if(Use_reported_date_Date_local=="Y") Use_reported_date_Date=1;
	else if(Use_reported_date_Date_local=="N") Use_reported_date_Date=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Use_reported_date_Date : Yes ""Y"" or No ""N"" ??");
		throw new GMXBException(message);
	}

	int nmber_date_in_month[12]= {31,28,31,30,31,30,31,31,30,31,30,31};
	//int *nmber_date_in_month=new int[12];
	//nmber_date_in_month[0]=31; nmber_date_in_month[1]=31;nmber_date_in_month[2]=31;nmber_date_in_month[3]=31;nmber_date_in_month[4]=31;nmber_date_in_month[5]=31;
	char dateStr [9];
	char timeStr [9];
	_strdate_s( dateStr);
	_strtime_s( timeStr );
	char *next_token1 = NULL;
	//char *next_token2 = NULL;
	string month1 = strtok_s(dateStr,"/",&next_token1);
	string today1 = strtok_s(NULL, "/",&next_token1) ;
	//string year1 = "20" + (string)strtok_s(NULL, "/",&next_token1) ;
	int valn_day = atoi(today1.c_str());
	int valn_month = atoi(month1.c_str());
	//int valn_year = atoi(year1.c_str());
	if(valn_month<1)
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "ERROR with valn_month in the code");
		throw new GMXBException(message);
	}
	if(Use_Correct_Date==1) Prorata=(nmber_date_in_month[valn_month-1]-valn_day+1.)/nmber_date_in_month[valn_month-1];
	else if(Use_reported_date_Date==1)
	{
		int day2=atoi(Assumptions_local->search("Reported_day").c_str());
		int month2=atoi(Assumptions_local->search("Reported_month").c_str());
		if(month2<1 || ((nmber_date_in_month[month2-1]-day2+1.)<0))
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "ERROR with Reported_month and Reported_day ");
			throw new GMXBException(message);
		}
		Prorata=(nmber_date_in_month[month2-1]-day2+1.)/nmber_date_in_month[month2-1];
	}
	else Prorata=1.;
	/*delete nmber_date_in_month;*/

	GMXB_Assumption_Table * Swap_rate_table_local = new GMXB_Assumption_Table(config.c_str(), "swap_rate_table");
	int nb_total_date = (int) Swap_rate_table_local->get_row_high_index();
	Nb_date = 0;	
	int* weightsCurve = new int[nb_total_date];
	for(int i=0;i<nb_total_date;i++){
		weightsCurve[i] = (int) Swap_rate_table_local->search(i+1,2);
		if(weightsCurve[i]!=0){this->Nb_date++;}
	}
	SwapRateVector= new double* [2];
	SwapRateVector[0]= new double [Nb_date];
	SwapRateVector[1]= new double [Nb_date];

	int count = 0;
	for(int i=0;i<nb_total_date;i++){
		if(weightsCurve[i]!=0){
			SwapRateVector[0][count] = Swap_rate_table_local->search(i+1,0);
			SwapRateVector[1][count] = Swap_rate_table_local->search(i+1,1);
			count++;
		}
	}
	delete [] weightsCurve;
	delete Swap_rate_table_local;

	Interpolation_type=Assumptions_local->search("Interpolation_Type").c_str();
	Extrapolation_type_start=Assumptions_local->search("Extrapolation_Start_Curve").c_str();
	Extrapolation_type_end=Assumptions_local->search("Extrapolation_End_Curve").c_str();
	Svensson_Extrapolation_End_option=Assumptions_local->search("Svensson_Extrapolation_End_option").c_str();
	Coupon_period=Assumptions_local->search("Coupon_Period").c_str();
	string bootstrapping_security_local=Assumptions_local->search("Bootstrapping_Security").c_str();
	if(bootstrapping_security_local=="Y") Bootstrapping_security=true;
	else if(bootstrapping_security_local=="N") Bootstrapping_security=false;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Bootstrapping_Security value does not exist");
		throw new GMXBException(message);
	}
	string Calibrate_Y_N_local=Assumptions_local->search("Calibrate").c_str();
	if(Calibrate_Y_N_local=="Y") Calibrate_Y_N=1;
	else if(Calibrate_Y_N_local=="N") Calibrate_Y_N=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Calibrate : Yes or No ??");
		throw new GMXBException(message);
	}	
	string debug_info_local=Assumptions_local->search("Debug_Calibration_Info").c_str();
	if(debug_info_local=="Y") Debug_info_Calib=1;
	else if(debug_info_local=="N") Debug_info_Calib=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Debug_Calibration_Info value does not exist");
		throw new GMXBException(message);
	}
	
	WildcardManager wm( config, "wildcards" );
	Debug_address_Calib=wm.getPath(Assumptions_local->search("Debug_Calibration_Address"));




	GMXB_Assumption_Table * Swaption_vol_table_local = new GMXB_Assumption_Table(config.c_str(), "swaption_vol_table");
	GMXB_Assumption_Table * Weight_table_local = new GMXB_Assumption_Table(config.c_str(), "weight_table");
	Nb_tenor_Swaption = (int) (Swaption_vol_table_local->get_column_high_index() - Swaption_vol_table_local->get_column_low_index());
	Nb_maturity_Swaption = (int) (Swaption_vol_table_local->get_row_high_index() - Swaption_vol_table_local->get_row_low_index());
	double *Swaption_tenor_local=new double [Nb_tenor_Swaption];
	double *Swaption_maturity_local=new double [Nb_maturity_Swaption];
	double **Swaption_vol_local=new double *[Nb_maturity_Swaption];
	double **Swaption_Weight_local=new double *[Nb_maturity_Swaption];

	for(int i=0;i<Nb_tenor_Swaption;i++) Swaption_tenor_local[i]=Swaption_vol_table_local->search(0,i+1);

	for(int i=0; i < Nb_maturity_Swaption; i++){
		Swaption_maturity_local[i] = Swaption_vol_table_local->search(i+2,0);
		Swaption_vol_local[i] = new double [Nb_tenor_Swaption];
		Swaption_Weight_local[i] = new double [Nb_tenor_Swaption];
		for(int j=0;j<Nb_tenor_Swaption;j++){
			Swaption_vol_local[i][j] = Swaption_vol_table_local->search(i+2,j+1);
			Swaption_Weight_local[i][j] = Weight_table_local->search(i+2,j+1);
			if(Swaption_Weight_local[i][j]>=0) Swaption_Weight_local[i][j]=sqrt(Swaption_Weight_local[i][j]);
			else{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Negative weights for swaptions");
				throw new GMXBException(message);
			}
		}
	}
	int count2=0;
	for(int i=0; i < Nb_maturity_Swaption; i++){
		for(int j=0;j<Nb_tenor_Swaption;j++){
			if(Swaption_Weight_local[i][j]>0)  count2++;
		}
	}

	Calc_Swaption_tenor.resize(count2);
	Calc_Swaption_maturity.resize(count2);
	Calc_Swaption_vol.resize(count2);
	Calc_Swaption_Weight.resize(count2);
	
	count2=0;
	for(int i=0; i < Nb_maturity_Swaption; i++){
		for(int j=0;j<Nb_tenor_Swaption;j++){
			if(Swaption_Weight_local[i][j]>0){
				Calc_Swaption_tenor[count2]=Swaption_tenor_local[j];
				Calc_Swaption_maturity[count2]=Swaption_maturity_local[i];
				Calc_Swaption_vol[count2]=Swaption_vol_local[i][j];
				Calc_Swaption_Weight[count2]=Swaption_Weight_local[i][j];
				count2++;
			}
		}
	}
	delete []  Swaption_tenor_local;
	delete []  Swaption_maturity_local;
	for(int i=0;i<Nb_maturity_Swaption;i++){
			delete [] Swaption_vol_local[i];
			delete [] Swaption_Weight_local[i];
	}
	delete [] Swaption_vol_local;
	delete [] Swaption_Weight_local;
	delete Swaption_vol_table_local;
	delete Weight_table_local;
	Calibration_On=Assumptions_local->search("Calibration_On").c_str();
	Param_initial = new double [4];
	Param_min    = new double [4];
	Param_max    = new double [4];
	for(int i=0;i<4;i++){
		Param_initial[i]=0;
		Param_min[i]=0;
		Param_max[i]=0;
	}
	Param_initial[1]=atof(Assumptions_local->search("sigma_Initial").c_str());
	Param_initial[0]=atof(Assumptions_local->search("a_Initial").c_str());
	Param_min[1]=atof(Assumptions_local->search("sigma_Min").c_str());
	Param_min[0]=atof(Assumptions_local->search("a_Min").c_str());
	Param_max[1]=atof(Assumptions_local->search("sigma_Max").c_str());
	Param_max[0]=atof(Assumptions_local->search("a_Max").c_str());
	string swaption_type_local=Assumptions_local->search("Swaption_Type").c_str();
	if(swaption_type_local=="payer") Swaption_type="PAY";
	else if(swaption_type_local=="receiver") Swaption_type="REC";
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Swaption_Type value does not exist");
		throw new GMXBException(message);
	}	
	Strike=atof(Assumptions_local->search("Swaption_Strike").c_str());
	Calib_Error_Function_Type=Assumptions_local->search("Error_Function_Type").c_str();
	A_without_calib=atof(Assumptions_local->search("a").c_str());
	Sigma_without_calib=atof(Assumptions_local->search("sigma").c_str());



	string Generate_Scenario_local=Assumptions_local->search("Generate_Scenario").c_str();
	if(Generate_Scenario_local=="Y") If_Generate_Scenario=1;
	else if(Generate_Scenario_local=="N") If_Generate_Scenario=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Bootstrapping_Security value does not exist");
		throw new GMXBException(message);
	}
	
	Time_Step = atoi(Assumptions_local->search("Time_Step").c_str());
	Number_Year=atoi(Assumptions_local->search("Number_Year").c_str());
	Number_Scenario=atoi(Assumptions_local->search("Number_Scenario").c_str());
	Local_Number_Scenario = Number_Scenario;
	
	
	string antithetic_local=Assumptions_local->search("Antithetic_Option").c_str();
	if(antithetic_local=="Y") Antithetic=2;
	else if (antithetic_local=="N") Antithetic=1;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Antithetic_Option value does not exist");
		throw new GMXBException(message);	
	}
	if(Antithetic==2 && Number_Scenario%2==1){
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Antithetic_Option activated : Must have an even number of paths");
		throw new GMXBException(message);
	}

	string test_martingale_local=Assumptions_local->search("Martingale_Test").c_str();
	if(test_martingale_local=="Y") Test_martingale=1;
	else if(test_martingale_local=="Use_Seed_File") Test_martingale=2;
	else if(test_martingale_local=="Adj_Test_Level") Test_martingale=3;
	else if (test_martingale_local=="N") Test_martingale=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Martingale_Test value does not exist");
		throw new GMXBException(message);	
	}
	Test_beta=Test_martingale;		
	string debug_info_local2=Assumptions_local->search("Debug_Generation_Info").c_str();
	if(debug_info_local2=="Y") Debug_Scenario_Generator_info=1;
	else if(debug_info_local2=="N") Debug_Scenario_Generator_info=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Debug_Martingality_Info value does not exist");
		throw new GMXBException(message);	
		}

	Debug_ESG_address=wm.getPath(Assumptions_local->search("Debug_Generation_Address"));
	string Generate_all_scn_in_ones_local=Assumptions_local->search("Generate_All_Scenarios_In_One_Step").c_str();
	if(Generate_all_scn_in_ones_local=="Y") Generate_All_Scenarios_In_One_Step=1;
	else if(Generate_all_scn_in_ones_local=="N") Generate_All_Scenarios_In_One_Step=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Generate_All_Scenarios_In_One_Step : Yes ""Y"" or No ""N"" ??");
		throw new GMXBException(message);
	}
	if(Generate_All_Scenarios_In_One_Step==0) Number_Scenario=2;

	GMXBParameters * Equity_table_local = new GMXBParameters(config.c_str(), "equity_table");
	GMXB_Assumption_Table * Vol_table_local         = new GMXB_Assumption_Table(config.c_str(), "vol_table"); 
	Nb_equity=(int)Vol_table_local->get_column_high_index();
	Nb_vol=(int)Vol_table_local->get_row_high_index(); 
	Eq_Volatility_Matrix=new double *[Nb_equity+1];
	for(int i=0;i<Nb_equity+1;i++){
		Eq_Volatility_Matrix[i]=new double [Nb_vol];
		for(int j=0;j<Nb_vol;j++) Eq_Volatility_Matrix[i][j]=Vol_table_local->search(j+1,i);
	}
	Equity_name_tab=new string [Nb_equity];
	for(int i=0;i<Nb_equity;i++){
		stringstream ss;
		ss<<(i+1);
		string equity_number = "equity " + ss.str();
		Equity_name_tab[i]=Equity_table_local->search(equity_number);
	}
	delete Equity_table_local;
	delete Vol_table_local;


	Use_Equity_Drift_table=Assumptions_local->search("Use_Equity_Drift_table").c_str();
	Equity_Drift_With_IR_Model=Assumptions_local->search("Equity_Drift_With_IR_Model").c_str();
	GMXB_Assumption_Table * equity_drift_table_local = new GMXB_Assumption_Table(config.c_str(), "equity_drift_table");
	Nb_dates_drift_table=(int)equity_drift_table_local->get_row_high_index(); 
	Eq_Drift_Matrix=new double *[Nb_equity+1];
	for(int i=0;i<Nb_equity+1;i++){
		Eq_Drift_Matrix[i]=new double [Nb_dates_drift_table];
		for(int j=0;j<Nb_dates_drift_table;j++) Eq_Drift_Matrix[i][j]=equity_drift_table_local->search(j+1,i);
	}
	delete equity_drift_table_local;


	string vol_HW_local=Assumptions_local->search("Treat_Vol_Extract_HW").c_str();
	if(vol_HW_local=="Y") vol_HW=true;
	else if(vol_HW_local=="N") vol_HW=false;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Treat_Vol_Extract_HW value does not exist");
		throw new GMXBException(message);
	}

	string vol_implied_to_loc_local=Assumptions_local->search("Treat_Vol_Implied_To_Loc").c_str();
	if(vol_implied_to_loc_local=="Y") Vol_implied_to_loc=true;
	else if(vol_implied_to_loc_local=="N") Vol_implied_to_loc=false;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Treat_Vol_Implied_To_Loc value does not exist");
		throw new GMXBException(message);
	}		



	string if_use_rate_table_local=Assumptions_local->search("Generate_Rates").c_str();
	if(if_use_rate_table_local=="Y") If_use_rate_table=1;
	else if(if_use_rate_table_local=="N") If_use_rate_table=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Generate_Rates value does not exist");
		throw new GMXBException(message);	
	}

	if(If_use_rate_table ==1){
		GMXB_Assumption_Table * Rate_table_local= new GMXB_Assumption_Table(config.c_str(), "rate_table");
		Nb_rate=(int)Rate_table_local->get_row_high_index();
		Rate_Mat_tab=new double [Nb_rate];
		Rate_type_tab=new int [Nb_rate];
		Rate_Period_tab=new double [Nb_rate];
		for(int i=0;i<Nb_rate;i++){
			Rate_Mat_tab[i]=Rate_table_local->search(i+1,0);
			Rate_type_tab[i]= (int) Rate_table_local->search(i+1,1);
			Rate_Period_tab[i]=Rate_table_local->search(i+1,2);
		}
		delete Rate_table_local;
	}     
	else{
		Nb_rate = 0;
		Rate_Mat_tab=new double [Nb_rate+1];
		Rate_type_tab=new int [Nb_rate+1];
		Rate_Period_tab=new double [Nb_rate+1];	
	}   

	string if_use_bond_table_local=Assumptions_local->search("Generate_Bonds").c_str();
	if(if_use_bond_table_local=="Y") If_use_bond_table=1;
	else if(if_use_bond_table_local=="N") If_use_bond_table=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Generate_Bonds value does not exist");
		throw new GMXBException(message);	
	}

	if(If_use_bond_table ==1){
		GMXB_Assumption_Table * Bond_table_local= new GMXB_Assumption_Table(config.c_str(), "bond_table"); 
		Nb_bond=(int)Bond_table_local->get_row_high_index();
		Bond_Mat_tab=new double [Nb_bond];
		Bond_coupon_tab=new double [Nb_bond];
		Bond_Freq_tab=new double [Nb_bond];
		for(int i=0;i<Nb_bond;i++){
			Bond_Mat_tab[i]=Bond_table_local->search(i+1,0);
			Bond_coupon_tab[i]=Bond_table_local->search(i+1,1);
			Bond_Freq_tab[i]=Bond_table_local->search(i+1,2);
		}
		delete Bond_table_local;
	}     
	else{
		Nb_bond = 0;
		Bond_Mat_tab=new double [Nb_bond+1];
		Bond_coupon_tab=new double [Nb_bond+1];
		Bond_Freq_tab=new double [Nb_bond+1];
	
	}    

	GMXB_Assumption_Table * Correlation_table_local = new GMXB_Assumption_Table(config.c_str(), "correlation_table"); 
	Correlation_matrix=new double *[Nb_equity+1];
	for(int i=0;i<Nb_equity+1;i++){
		Correlation_matrix[i]=new double [Nb_equity+1];
		for(int j=0;j<Nb_equity+1;j++)	Correlation_matrix[i][j]=Correlation_table_local->search(i+1,j);
	}
	delete Correlation_table_local;

	GMXB_Assumption_Table *Error_table_local = new GMXB_Assumption_Table(config.c_str(), "error_table");                   
	Error_tab=new double [Nb_equity+1];
	Add_error_tab=new double [Nb_equity+1];
	Max_try_tab=new int [Nb_equity+1];
	for(int i=0;i<Nb_equity+1;i++){
		Error_tab[i]=Error_table_local->search(i+1,0);
		Add_error_tab[i]=Error_table_local->search(i+1,1);
		Max_try_tab[i]= (int) Error_table_local->search(i+1,2);
	}
	delete Error_table_local;

	string output_scenario_local=Assumptions_local->search("Write_Scenario").c_str();
	if(output_scenario_local=="Y") Write_scenario=1;
	else if(output_scenario_local=="N") Write_scenario=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Write_Scenario value does not exist");
		throw new GMXBException(message);	
	}
	string output_gaussian_local=Assumptions_local->search("Write_Gaussian").c_str();
	if(output_gaussian_local=="N")  Write_gaussian=0;
	else if((output_gaussian_local=="Y") && (output_scenario_local=="Y")) Write_gaussian=1;
	else if((output_gaussian_local=="Y") && (output_scenario_local=="N")) Write_gaussian=0;
	else{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Write_Scenario value does not exist");
		throw new GMXBException(message);	
	}
	Precision=atoi(Assumptions_local->search("Output_Precision").c_str());
	File_scenario_address = wm.getPath(Assumptions_local->search("Scenario_File_Name"));	

	delete Assumptions_local;
}
InterfaceESG0::~InterfaceESG0(void)
{
	Calc_Swaption_tenor.clear();
	Calc_Swaption_maturity.clear();
	Calc_Swaption_vol.clear();
	Calc_Swaption_Weight.clear();

	delete []  Param_initial;
	delete []  Param_min;
	delete []  Param_max;
	delete []  SwapRateVector[0];
	delete  [] SwapRateVector[1];
	delete  [] SwapRateVector;

	delete []  Equity_name_tab;
	delete []  Bond_Mat_tab;
	delete []  Bond_coupon_tab;
	delete []  Bond_Freq_tab;
	delete []  Rate_Mat_tab;
	delete []  Rate_type_tab;
	delete []  Rate_Period_tab;
	delete []  Error_tab;
	delete []  Add_error_tab;
	delete []  Max_try_tab;

	for(int i=0;i<Nb_equity+1;i++){
		delete [] Correlation_matrix[i];
		delete [] Eq_Volatility_Matrix[i];
		delete [] Eq_Drift_Matrix[i];
	}
	delete [] Correlation_matrix;
	delete [] Eq_Volatility_Matrix;
	delete [] Eq_Drift_Matrix;

}