#include "ScenarioGen.h"



ScenarioGen::~ScenarioGen()
{

	for(int i=0;i<Gaus_Model_Param->NB_equity+1;i++) 
		delete [] Eq_Drift_Matrix[i];
	delete [] Eq_Drift_Matrix;

	delete [] Equity_name_tab;
	delete [] Rate_Mat_tab;
	delete [] Rate_type_tab;
	delete [] Rate_Period_tab;
	delete [] Bond_Mat_tab;
	delete [] Bond_coupon_tab;
	delete [] Bond_Freq_tab;
	
	for(int i=0;i<this->Num_Index;i++)
	{
		for(int j=0;j<Gaus_Model_Param->Nb_Step+1;j++) delete[] Scenario_vector[i][j];
		delete[] Scenario_vector[i];
	}
	delete[] Scenario_vector;

	delete Yield_curve_vector;
	delete Test;
	delete Gaus_Model_Param;
	delete Gaussian;
	delete Diffusion_Model;


}

ScenarioGen::ScenarioGen(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model , YC* &mYieldCurve,string Model_rand,string Model_guassian,string Model_diff, int scn_path_number, int number_scn_gen_stepbystep)
{
	Model_diffusion_Name=Model_diff; 
	Model_random_Name=Model_rand; 
	Model_guassian_Name=Model_guassian; 	
	
	this->Test= new VarianceReduction(interface_Esg);
	this->Test->Init_Calib_parrameters(model->getMRV(),model->getsigma());
	this->Gaus_Model_Param=new GaussianGenModelParameters(interface_Esg,model);

	this->Gaus_Model_Param->Number_Scen=number_scn_gen_stepbystep;
	this->Gaus_Model_Param->Number_Scen_Antithetic=this->Gaus_Model_Param->Number_Scen/this->Gaus_Model_Param->Antithetic;;

	this->Gaussian=new GaussianGen(Gaus_Model_Param);

	Yield_curve_vector=new YC(*mYieldCurve);


	if(Model_diffusion_Name=="BS_HW1F")
		this->Diffusion_Model= new BS_HW1F(interface_Esg);

	this->Diffusion_Model->Initialize_Calib_parameters(model->getMRV(),model->getsigma());
		
	this->Num_Index=4+Gaus_Model_Param->NB_equity+Gaus_Model_Param->Nb_bond+Gaus_Model_Param->Nb_rate;	
	Scenario_vector= new double** [Num_Index];			
	for(int i=0; i<Num_Index; i++){				
		Scenario_vector[i]= new double* [Gaus_Model_Param->Nb_Step+1];			
		for(int j=0;j<Gaus_Model_Param->Nb_Step+1;j++) Scenario_vector[i][j]=new double [Gaus_Model_Param->Number_Scen];			
	}			
	//Initialize			
	for(int j=0;j<Gaus_Model_Param->Number_Scen;j++){				
		for(int i=0;i<Gaus_Model_Param->Nb_Step+1;i++){				
			Scenario_vector[0][i][j]=j+1;				
			Scenario_vector[1][i][j]=i;			
		}			
		Scenario_vector[2][0][j]=0;			
		Scenario_vector[3+Gaus_Model_Param->Nb_rate][0][j]=1;				
		for(int i=0;i<Gaus_Model_Param->NB_equity;i++){				
			Scenario_vector[4+Gaus_Model_Param->Nb_rate+i][0][j]=1;			
		}		
	}

	If_Generate_Scenario=interface_Esg->If_Generate_Scenario; //From InterfaceESG Class
	Generate_All_Scenarios_In_One_Step=interface_Esg->Generate_All_Scenarios_In_One_Step; //From InterfaceESG Class
	If_Write_scenario=interface_Esg->Write_scenario; //From InterfaceESG Class
	If_Write_gaussian=interface_Esg->Write_gaussian; //From InterfaceESG Class
	Precision=interface_Esg->Precision; //From InterfaceESG Class
	File_scenario_address=interface_Esg->File_scenario_address; //From InterfaceESG Class
	If_use_rate_table=interface_Esg->If_use_rate_table; //From InterfaceESG Class
	If_use_bond_table=interface_Esg->If_use_bond_table; //From InterfaceESG Class
	if(If_use_rate_table==1){
		Rate_Mat_tab=new double [Gaus_Model_Param->Nb_rate];
		Rate_type_tab=new int [Gaus_Model_Param->Nb_rate];
		Rate_Period_tab=new double [Gaus_Model_Param->Nb_rate];	
		for(int i=0;i<Gaus_Model_Param->Nb_rate;i++){
			Rate_Mat_tab[i]=interface_Esg->Rate_Mat_tab[i];
			Rate_type_tab[i]= interface_Esg->Rate_type_tab[i];
			Rate_Period_tab[i]=interface_Esg->Rate_Period_tab[i];
		}
	}
	else{
		Rate_Mat_tab=new double [Gaus_Model_Param->Nb_rate+1];
		Rate_type_tab=new int [Gaus_Model_Param->Nb_rate+1];
		Rate_Period_tab=new double [Gaus_Model_Param->Nb_rate+1];	
	}

	if(If_use_bond_table==1){
		Bond_Mat_tab=new double [Gaus_Model_Param->Nb_bond];
		Bond_coupon_tab=new double [Gaus_Model_Param->Nb_bond];
		Bond_Freq_tab=new double [Gaus_Model_Param->Nb_bond];	
		for(int i=0;i<Gaus_Model_Param->Nb_rate;i++){
			Bond_Mat_tab[i]=interface_Esg->Bond_Mat_tab[i];
			Bond_coupon_tab[i]= interface_Esg->Bond_coupon_tab[i];
			Bond_Freq_tab[i]=interface_Esg->Bond_Freq_tab[i];
		}
	}
	else{
		Bond_Mat_tab=new double [Gaus_Model_Param->Nb_bond+1];
		Bond_coupon_tab=new double [Gaus_Model_Param->Nb_bond+1];
		Bond_Freq_tab=new double [Gaus_Model_Param->Nb_bond+1];	
	}

	Equity_name_tab=new string [Gaus_Model_Param->NB_equity];
	for(int i=0;i<Gaus_Model_Param->NB_equity;i++)
			Equity_name_tab[i]=interface_Esg->Equity_name_tab[i];
	
	Scenario_path_number=scn_path_number;
	Scenario_shock_number=0;

	Nb_dates_drift_table=interface_Esg->Nb_dates_drift_table;
	Use_Equity_Drift_table=interface_Esg->Use_Equity_Drift_table;
	Equity_Drift_With_IR_Model=interface_Esg->Use_Equity_Drift_table;
	
	Eq_Drift_Matrix=new double *[Gaus_Model_Param->NB_equity+1];
	for(int i=0;i<Gaus_Model_Param->NB_equity+1;i++){
		Eq_Drift_Matrix[i]=new double [Nb_dates_drift_table];
		for(int j=0;j<Nb_dates_drift_table;j++) Eq_Drift_Matrix[i][j]=interface_Esg->Eq_Drift_Matrix[i][j];
	}


}
ScenarioGen::ScenarioGen(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model , YC* &mYieldCurve,string Model_rand,string Model_guassian,string Model_diff, int scn_path_number)
{
	Model_diffusion_Name=Model_diff; 
	Model_random_Name=Model_rand; 
	Model_guassian_Name=Model_guassian; 	
	
	this->Test= new VarianceReduction(interface_Esg);
	this->Test->Init_Calib_parrameters(model->getMRV(),model->getsigma());
	this->Gaus_Model_Param=new GaussianGenModelParameters(interface_Esg,model);
	this->Gaussian=new GaussianGen(Gaus_Model_Param);

	Yield_curve_vector=new YC(*mYieldCurve);


	if(Model_diffusion_Name=="BS_HW1F")
		this->Diffusion_Model= new BS_HW1F(interface_Esg);

	this->Diffusion_Model->Initialize_Calib_parameters(model->getMRV(),model->getsigma());
		
	this->Num_Index=4+Gaus_Model_Param->NB_equity+Gaus_Model_Param->Nb_bond+Gaus_Model_Param->Nb_rate;	
	Scenario_vector= new double** [Num_Index];			
	for(int i=0; i<Num_Index; i++){				
		Scenario_vector[i]= new double* [Gaus_Model_Param->Nb_Step+1];			
		for(int j=0;j<Gaus_Model_Param->Nb_Step+1;j++) Scenario_vector[i][j]=new double [Gaus_Model_Param->Number_Scen];			
	}			
	//Initialize			
	for(int j=0;j<Gaus_Model_Param->Number_Scen;j++){				
		for(int i=0;i<Gaus_Model_Param->Nb_Step+1;i++){				
			Scenario_vector[0][i][j]=j+1;				
			Scenario_vector[1][i][j]=i;			
		}			
		Scenario_vector[2][0][j]=0;			
		Scenario_vector[3+Gaus_Model_Param->Nb_rate][0][j]=1;				
		for(int i=0;i<Gaus_Model_Param->NB_equity;i++){				
			Scenario_vector[4+Gaus_Model_Param->Nb_rate+i][0][j]=1;			
		}		
	}

	If_Generate_Scenario=interface_Esg->If_Generate_Scenario; //From InterfaceESG Class
	Generate_All_Scenarios_In_One_Step=interface_Esg->Generate_All_Scenarios_In_One_Step; //From InterfaceESG Class
	If_Write_scenario=interface_Esg->Write_scenario; //From InterfaceESG Class
	If_Write_gaussian=interface_Esg->Write_gaussian; //From InterfaceESG Class
	Precision=interface_Esg->Precision; //From InterfaceESG Class
	File_scenario_address=interface_Esg->File_scenario_address; //From InterfaceESG Class
	If_use_rate_table=interface_Esg->If_use_rate_table; //From InterfaceESG Class
	If_use_bond_table=interface_Esg->If_use_bond_table; //From InterfaceESG Class
	if(If_use_rate_table==1){
		Rate_Mat_tab=new double [Gaus_Model_Param->Nb_rate];
		Rate_type_tab=new int [Gaus_Model_Param->Nb_rate];
		Rate_Period_tab=new double [Gaus_Model_Param->Nb_rate];	
		for(int i=0;i<Gaus_Model_Param->Nb_rate;i++){
			Rate_Mat_tab[i]=interface_Esg->Rate_Mat_tab[i];
			Rate_type_tab[i]= interface_Esg->Rate_type_tab[i];
			Rate_Period_tab[i]=interface_Esg->Rate_Period_tab[i];
		}
	}
	else{
		Rate_Mat_tab=new double [Gaus_Model_Param->Nb_rate+1];
		Rate_type_tab=new int [Gaus_Model_Param->Nb_rate+1];
		Rate_Period_tab=new double [Gaus_Model_Param->Nb_rate+1];	
	}

	if(If_use_bond_table==1){
		Bond_Mat_tab=new double [Gaus_Model_Param->Nb_bond];
		Bond_coupon_tab=new double [Gaus_Model_Param->Nb_bond];
		Bond_Freq_tab=new double [Gaus_Model_Param->Nb_bond];	
		for(int i=0;i<Gaus_Model_Param->Nb_rate;i++){
			Bond_Mat_tab[i]=interface_Esg->Bond_Mat_tab[i];
			Bond_coupon_tab[i]= interface_Esg->Bond_coupon_tab[i];
			Bond_Freq_tab[i]=interface_Esg->Bond_Freq_tab[i];
		}
	}
	else{
		Bond_Mat_tab=new double [Gaus_Model_Param->Nb_bond+1];
		Bond_coupon_tab=new double [Gaus_Model_Param->Nb_bond+1];
		Bond_Freq_tab=new double [Gaus_Model_Param->Nb_bond+1];	
	}

	Equity_name_tab=new string [Gaus_Model_Param->NB_equity];
	for(int i=0;i<Gaus_Model_Param->NB_equity;i++)
			Equity_name_tab[i]=interface_Esg->Equity_name_tab[i];
	
	Scenario_path_number=scn_path_number;
	Scenario_shock_number=0;

	Nb_dates_drift_table=interface_Esg->Nb_dates_drift_table;
	Use_Equity_Drift_table=interface_Esg->Use_Equity_Drift_table;
	
	Eq_Drift_Matrix=new double *[Gaus_Model_Param->NB_equity+1];
	for(int i=0;i<Gaus_Model_Param->NB_equity+1;i++){
		Eq_Drift_Matrix[i]=new double [Nb_dates_drift_table];
		for(int j=0;j<Nb_dates_drift_table;j++) Eq_Drift_Matrix[i][j]=interface_Esg->Eq_Drift_Matrix[i][j];
	}


}
void ScenarioGen::write_seed(ofstream &file1,unsigned long * seed){
	file1<<seed[N+1]<<",";
	for(int i=0;i<N+1;i++){
		file1<<seed[i]<<",";
	}
	file1<<endl;
}

int ScenarioGen::read_seed(ifstream &file2,unsigned long * seed){
	string row;
	getline(file2,row);
	char* retPtr = new char[row.length()+1];
	strcpy_s(retPtr, row.length()+1, row.c_str());
	char* context = NULL;
	int martingale = atoi(strtok_s(retPtr, ",",&context));
	if(context[0]==0){
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in Seed File");
		throw new GMXBException(message);
	}
	for (int i=0; i<N+1; i++){
		if(context[0]==0){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error in Seed File");
			throw new GMXBException(message);
		}
		seed[i] = strtoul(strtok_s(NULL, ",",&context),NULL,10);
	}
	delete [] retPtr;
	return martingale;
}
void ScenarioGen::Reset_Scenario_With_IR_Shocks(HW1FSwaptionPricer* &model, double startDate,double keyDate, double endDate,double shockSize,string& shockType)
{
	//this->Yield_curve_vector->Reset_YC();
	this->Test->Init_Calib_parrameters(model->getMRV(),model->getsigma());
	this->Diffusion_Model->Initialize_Calib_parameters(model->getMRV(),model->getsigma());
	this->Yield_curve_vector->ShockSwap(startDate,keyDate,endDate,shockSize,shockType);
	this->Gaussian->Uniform_Generator->Initialisation_seed0();
				
	for(int i=3; i<Num_Index; i++)				
		for(int j=1;j<Gaus_Model_Param->Nb_Step+1;j++) 
			for(int k=0;k<Gaus_Model_Param->Number_Scen;k++) 
				Scenario_vector[i][j][k]=0.;			
	
	Scenario_shock_number+=1;
}
void ScenarioGen::Generate_Scenario()
{
	if(If_Generate_Scenario==1){

		this->Gaussian->GenAll(this->Test);			
		if(If_Write_gaussian)
			this->Gaussian->Write_Debug_File(File_scenario_address,0);
			
		Diffusion_Model->Initialize_Class_Parameters(this->Gaussian->Gaussian_vector[0],this->Gaussian->Gaussian_vector[1]);

		Diffusion_Model->Calculate_Short_rate(Yield_curve_vector->Forward_YC,Scenario_vector[2]);
		Diffusion_Model->Calculate_Beta(Yield_curve_vector->INV_ZCB_Yield,Scenario_vector[3+Gaus_Model_Param->Nb_rate]);
		
		for (int i=0;i<Gaus_Model_Param->NB_equity;i++){
			if(Use_Equity_Drift_table=="Y"){
				YC *Yield_curve_Drift_temp;
				Yield_curve_Drift_temp=new YC(*Yield_curve_vector);
				string shock="Drift_shock_index_"+to_string(i);
				Yield_curve_Drift_temp->ShockSwap_Drift(Eq_Drift_Matrix[0],Eq_Drift_Matrix[i+1],Nb_dates_drift_table,shock);
				if(Equity_Drift_With_IR_Model=="Y")
					Diffusion_Model->Calculate_Equity_Return_With_Drift(i,this->Gaussian->Gaussian_vector[2+i],Scenario_vector[3+Gaus_Model_Param->Nb_rate],
							Scenario_vector[4+Gaus_Model_Param->Nb_rate+i],Yield_curve_vector->INV_ZCB_Yield,Yield_curve_Drift_temp->INV_ZCB_Yield);
				else
					Diffusion_Model->Calculate_Equity_Return_With_Drift_without_IR_Model(i,this->Gaussian->Gaussian_vector[2+i],Scenario_vector[4+Gaus_Model_Param->Nb_rate+i],
												Yield_curve_Drift_temp->INV_ZCB_Yield);
				delete Yield_curve_Drift_temp;
			}
			else
				Diffusion_Model->Calculate_Equity_Return(i,this->Gaussian->Gaussian_vector[2+i],Scenario_vector[3+Gaus_Model_Param->Nb_rate],Scenario_vector[4+Gaus_Model_Param->Nb_rate+i]);
		
		}
		

		if(If_use_rate_table==1){	
			for(int k=0;k<Gaus_Model_Param->Nb_rate;k++){
				if(Rate_type_tab[k]==1) Diffusion_Model->Calculate_Notional_Rate(Rate_Mat_tab[k],Yield_curve_vector->Log_ZCB_Yield,Scenario_vector[3+k]);
				if(Rate_type_tab[k]==2) Diffusion_Model->Calculate_Swap_Rate(Rate_Mat_tab[k],Yield_curve_vector->Log_ZCB_Yield,Scenario_vector[3+k], Rate_Period_tab[k]);
			}
		}

		if(If_use_bond_table==1){
			for(int k=0;k<Gaus_Model_Param->Nb_bond;k++){
				if(Bond_coupon_tab[k]<0.000001)
					Diffusion_Model->Calculate_Bond_Return(Bond_Mat_tab[k], Yield_curve_vector->Log_ZCB_Yield, Scenario_vector[4+Gaus_Model_Param->Nb_rate+Gaus_Model_Param->NB_equity+k]);
				else
					Diffusion_Model->Calculate_Bond_Coupon_Return(Bond_Mat_tab[k],Yield_curve_vector->Log_ZCB_Yield,
					Scenario_vector[4+Gaus_Model_Param->Nb_rate+Gaus_Model_Param->NB_equity+k], Bond_coupon_tab[k], Bond_Freq_tab[k]);
			}
		}
		if(If_Write_scenario)
			Write_Scenario();

	}		
	/*
	HW1FSwaptionPricer *model=new HW1FSwaptionPricer(this->Interface_Data->Calib_Error_Function_Type,this->Interface_Data->Calibration_On,this->Interface_Data->Param_initial,this->Interface_Data->Param_min,this->Interface_Data->Param_max);

	double error=0.;
	MinimisationMethod MethodMin=LM_NEW;

	this->Calibrator->calibrate(model,error,MethodMin);

	this->Diffusion_Model->Initialize_Calib_parameters(model->getMRV(),model->getsigma());
	this->Test->Init_Calib_parrameters(model->getMRV(),model->getsigma());*/

}
void ScenarioGen::Write_Scenario()
{
	ofstream scen;
	string ss;
	ss = File_scenario_address + "scenario_" + to_string(Scenario_path_number)+ "_" + to_string(Scenario_shock_number) + ".csv" ;
	scen.open(ss.c_str());

	scen<<"Iteration"<<","<<"Period"<<","<<"Short_Rate"<<",";
	for(int i=0;i<Gaus_Model_Param->Nb_rate;i++) scen<<"Nominal_Rate_"<<Rate_Mat_tab[i]<<"Yr"<<",";
	scen<<"Beta"<<",";
	for(int i=0;i<Gaus_Model_Param->NB_equity;i++) scen<< Equity_name_tab[i]<<"_Return"<<",";
	for(int i=0;i<Gaus_Model_Param->Nb_bond;i++) scen<<"Bond_Return_T"<<Bond_Mat_tab[i]<<"C"<<Bond_coupon_tab[i]<<",";
	scen<<endl;
	for(int i=0;i<Gaus_Model_Param->Number_Scen;i++){
		for(int j=0;j<Gaus_Model_Param->Nb_Step+1;j++){
			for(int k=0;k<Num_Index;k++){
				scen<<setprecision(Precision)<<Scenario_vector[k][j][i]<<",";
			}
			scen<<endl;
		}
	}
	scen.close();
			
}


	