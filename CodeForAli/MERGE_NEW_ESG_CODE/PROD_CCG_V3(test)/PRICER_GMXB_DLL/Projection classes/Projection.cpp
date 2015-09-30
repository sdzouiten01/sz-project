#include "Projection.h"
#define  UNREFERENCED_PARAMETER(P) (P)
int Projection::compt_sen=0;

Assumption0::Assumption0(){}
Assumption0::Assumption0(const char * tables)
{	
	// Assumptions tables------------------------------------------------
	map<string, GMXB_Assumption_Table*> sheet_tables;
	assump = parse_sheets(string(tables), sheet_tables, string("assump") );
		
	Ratchet_Y_N = atoi(this->assump->search("Ratchet_Y_N").c_str());
	Perf_Bonus_Y_N = atoi(this->assump->search("Perf_Bonus_Y_N").c_str());
	Percentage_Perf=atof(this->assump->search("Percentage_Perf").c_str());
	Use_Lapse_Y_N = atoi(this->assump->search("Use_Lapse_Y_N").c_str());
	Base_Lapse=atof(this->assump->search("Base_Lapse").c_str());
	Base_Lapse_p_m=1-exp(log(1-Base_Lapse)/12.);
	Dynamic_Lapse_Y_N = atoi(this->assump->search("Dynamic_Lapse_Y_N").c_str());
	X_1=atof(this->assump->search("X_1").c_str());
	X_2=atof(this->assump->search("X_2").c_str());
	X_3=atof(this->assump->search("X_3").c_str());
	X_4=atof(this->assump->search("X_4").c_str());
	Y_1=atof(this->assump->search("Y_1").c_str());
	Y_2=atof(this->assump->search("Y_2").c_str());
	Y_3=atof(this->assump->search("Y_3").c_str());
	Y_4=atof(this->assump->search("Y_4").c_str());
	Guarantee_Charge_Before=atof(this->assump->search("Guarantee_Charge").c_str());
	Guarantee_Charge_After_04_2015=atof(this->assump->search("Guarantee_Charge_After_04_2015").c_str());
	Guarantee_Charge_Before_p_m=Guarantee_Charge_Before/12.;
	Guarantee_Charge_After_04_2015_p_m=Guarantee_Charge_After_04_2015/12.;
	Guarantee_Charge=0.0;
	Guarantee_Charge_p_m=0.0;

	Fixed_Charge=atof(this->assump->search("Fixed_Charge").c_str());
	Fixed_Charge_p_m=Fixed_Charge/12.;
	Bonus_rate=atof(this->assump->search("Bonus_rate").c_str());
	Retrocessions_Fund=atof(this->assump->search("Retrocessions_Fund").c_str());
}
double Assumption0::Get_Dynamic_Lapse_Value(double moneyness)
{
	if(Use_Lapse_Y_N==0)
		return 0.;
	else if(Dynamic_Lapse_Y_N==0)
		return Base_Lapse_p_m*(moneyness!=0.);
	else
	{
		double a,b,u,v,plf;
				
		if(moneyness<X_1){a=X_1;b=X_1;}
		else if(moneyness<X_2){a=X_1;b=X_2;}
		else if(moneyness<X_3){a=X_2;b=X_3;}
		else if(moneyness<X_4){a=X_3;b=X_4;}
		else{a=X_4;b=X_4;}
					
		if(a==X_1) u=Y_1;
		else if(a==X_2) u=Y_2;
		else if(a==X_3) u=Y_3;
		else u=Y_4;
				
		if(b==X_1) v=Y_1;
		else if(b==X_2) v=Y_2;
		else if(b==X_3) v=Y_3;
		else v=Y_4;
				
		if(b==a) plf=u;
		else plf= u+(v-u)/(b-a)*(moneyness-a);

		return plf*Base_Lapse_p_m;
	}
}

Projection::Projection(const vector<vector <int>>& i_s_a, const char * tables, bool generate_dbg_files, const string& debug_file_path)
{
	UNREFERENCED_PARAMETER(tables);

	this->myDbgFilePath  = debug_file_path;
	this->myGenerateDebugFiles = generate_dbg_files;
	this->ShockSize = i_s_a.size();
	if (this->ShockSize > 0)
	{		
		this->ShockArray = new int* [this->ShockSize];
		for(int i = 0; i < this->ShockSize; i++)
		{
			this->ShockArray[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
			for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
				this->ShockArray[i][j] = i_s_a[i][j];
		}
	}

	this->set_DebugVector_to_zero();
	this->set_MainProjArrayVector_to_zero();
	this->set_ConstVector_to_zero();

	myConsVector_size=TOTAL_CONSTVECTOR_COLS;
	myMainProjArrayVector_size=MAIN_PROJ_NUM_COLS;
	myDebugVector_size=DEBUG_NUM_COLS;	

	set_Local_Parameters_to_zero();

	this->myCurrentScen=1;
	this->myShockNumber=0;
	this->total_number_scn=0;
	this->myCalc_Prorata=0.;
	this->myCalc_Remaining_Months=0;	
	this->myPosition_MP_in_File=0;
	//ofstream debug_file;
}
Projection::~Projection()
{
	// delete the shock array
	for(int i = 0; i <this->ShockSize; i++){
		if(this->ShockArray[i] != 0) delete[] this->ShockArray[i];
	}
	if (this->ShockSize > 0)
		delete[] this->ShockArray;		
}
void Projection::mySet_Debug_File_Header(){
	this->debug_file << "TIME,DURATION_MONTH_BL_CH,";
	this->debug_file << endl;																																			
}
void Projection::projection_base_contract(int curr_shock_number){UNREFERENCED_PARAMETER(curr_shock_number);}
void Projection::PresentValueCalculation(int curr_shock_number,bool delta)
{
	UNREFERENCED_PARAMETER(curr_shock_number);
	UNREFERENCED_PARAMETER(delta);
}
void Projection::Calculate_ConstVector_timing(){}
void Projection::Calculate_ConstVector_guarantees(){}
void Projection::Calculate_ConstVector_decrement(){}
void Projection::initialise_mp_calculation(){}
void Projection::set_current_model_point(Model_Point *mp)
{
	this->myModel_Point = mp;

	this->initialise_mp_calculation();
	myCalc_Remaining_Months=this->myModel_Point->Calc_Remaining_Months;
	myPosition_MP_in_File=this->myModel_Point->mp_position_in_file;
	myCalc_Prorata=this->myModel_Point->Calc_Prorata;
	this->InitProjection();
}
void Projection::set_Local_Parameters_to_zero(){set_Local_Parameters_to_zero0();}
	

void Projection::set_value_on_debug_file ( double value, int row, int t){DebugVector[row][t]=value;}
void Projection::set_DebugVector_to_zero(){memset(this->DebugVector, 0, sizeof(this->DebugVector));}
void Projection::set_ConstVector_to_zero(){memset(this->ConstVector, 0, sizeof(this->ConstVector));}
void Projection::set_MainProjArrayVector_to_zero(){memset(this->MainProjArrayVector, 0, sizeof(this->MainProjArrayVector));}
void Projection::set_Local_Parameters_to_zero0()
{
	account_value=0.;
	survival_index=0.;
	survival_index_by_step=0.;
	claims_wb_t=0.;
	claims_db_t=0.;
	GMDB_level=0.;
	basis_capital=0.;
	annuity_per_freq=0.;	
	add_coupon_from_ratchet=0.;
	Performance_bonus_valuation=0.;
	bonus_value=0.;	
	fund_fees=0.;
	lapse=0.;
	itm=0.;
	lapse_val=0.;
	account_value_1=0.;
	hedge_cost_fees=0.;
	fixed_charges=0.;
	mortality_rate=0.;
	sumprod_dynamic_lapse=0.;
	dynamic_lapse_ratio=0.;
}	
void Projection::set_total_number_scn( int scn_numb){ total_number_scn=scn_numb;}
void Projection::set_index_shock_array(const vector<vector <int>>& i_s_a)
{
	// Deletes the old shock array
	for(int i = 0; i <this->ShockSize; i++){
		if(this->ShockArray[i] != 0) delete[] this->ShockArray[i];
	}
	if (this->ShockSize > 0)
		delete[] this->ShockArray;

	// Initilize the new shock array
	this->ShockSize = i_s_a.size();
	if (this->ShockSize > 0){
		this->ShockArray = new int* [this->ShockSize];
		for(int i = 0; i < this->ShockSize; i++){
			this->ShockArray[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
			for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
				this->ShockArray[i][j] = i_s_a[i][j];
		}
	}
}
void Projection::set_monthly_rates(double ** monthly_rates, int scen_num){
	this->myMonthly_Return_table = monthly_rates;
	this->myCurrentScen= scen_num;
}
void Projection::init_market_data()
{	
	//initialize discount factor at t=0
	MainProjArrayVector[DISCOUNT_FACTOR][0] = 1.;
	//initialize fund return at t=0
	MainProjArrayVector[FUND_RETURN][0] = 0.;
	//initialize market data for each time step		
	for( int t=1; t <= myCalc_Remaining_Months ; t++){
		MainProjArrayVector[DISCOUNT_FACTOR][t] = this->myMonthly_Return_table[t][MAXIMUM_NUMBER_OF_INDEXES + 1]; 
		//MainProjArrayVector[FUND_RETURN][t] = exp(log(1.+this->myMonthly_Return_table[t][0])/12.)-1.;
		MainProjArrayVector[FUND_RETURN][t] = this->myMonthly_Return_table[t][0];
	}
	MainProjArrayVector[FUND_RETURN][1]*=myCalc_Prorata;
}
void Projection::InitProjection()
{
	if(myGenerateDebugFiles)
		set_DebugVector_to_zero();
	set_ConstVector_to_zero();
	Calculate_ConstVector_timing();
	Calculate_ConstVector_guarantees();
	Calculate_ConstVector_decrement();
}
void Projection::myWrite_Debug_File()
{
	char debug_file_name[MAX_LENGTH_FILE_NAME];
	sprintf_s(debug_file_name, "%smain_array_mp%d_scn%d.csv" ,this->myDbgFilePath.c_str(), myPosition_MP_in_File, this->myCurrentScen);
	this->debug_file.open(debug_file_name);		
	mySet_Debug_File_Header();
	for ( int t=0; t<= myCalc_Remaining_Months; t++)
	{
		this->debug_file <<setprecision(15)<< t << ",";
		for(int col = 0; col < myConsVector_size ; col++)
			this->debug_file <<setprecision(15)<< this->ConstVector[col][t] << ",";
		for(int col = 0; col < myMainProjArrayVector_size ; col++)
			this->debug_file << setprecision(15)<< this->MainProjArrayVector[col][t] << ",";
		for(int col = 0; col < myDebugVector_size ; col++)
			this->debug_file << setprecision(15)<< this->DebugVector[col][t] << ",";
		this->debug_file << endl;
	}
	this->debug_file.close();
}
void Projection::init_Count_scen_to_zero(){this->myModel_Point->Calc_Count_Scen=0;}
int Projection::Count_scen(){return this->myModel_Point->Calc_Count_Scen;}
void Projection::Increase_Count_scen(){this->myModel_Point->Calc_Count_Scen +=1;}
void Projection::project()
{
	Increase_Count_scen();
	//this->myModel_Point->Calc_Count_Scen +=1;
	//Projection::compt_sen +=1;		
	// Loop over the indexes shocks
	set_MainProjArrayVector_to_zero();
	init_market_data();
	for(int sh_n = 0; sh_n < this->ShockSize; sh_n++)
	{	
		this->myShockNumber = sh_n;
		
		this->projection_base_contract(this->myShockNumber);

		if(myGenerateDebugFiles)
			this->myWrite_Debug_File();
		
		//if(Projection::compt_sen==this->total_number_scn)
		if(Count_scen()==this->total_number_scn)
			this->PresentValueCalculation(this->myShockNumber,true);
	}
		
}


	
