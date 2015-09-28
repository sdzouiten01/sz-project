#ifndef _DLL_FILE_
#define _DLL_FILE_
#endif

#include "GMXB_Assumption_Table.h"
#include "GMXBParameters.h"
#include "WildcardManager.h"
#include "GMXBExcelXMLReader.h"
#include "Generator.h"
#include "InterfaceEsg.h"
#include "InterfaceCalibration.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include <tchar.h>
#include "Mersenne_Twister.h"

using namespace std;

InterfaceEsg::InterfaceEsg( const string& config )
{
		config_file = config;

		assumptions = new GMXBParameters(config.c_str(),"generator_assumptions");
		
		string if_use_rate_table_local=assumptions->search("Generate_Rates").c_str();
		if(if_use_rate_table_local=="Y") if_use_rate_table=1;
		else if(if_use_rate_table_local=="N") if_use_rate_table=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Generate_Rates value does not exist");
			throw new GMXBException(message);	
		}

		string if_use_bond_table_local=assumptions->search("Generate_Bonds").c_str();
		if(if_use_bond_table_local=="Y") if_use_bond_table=1;
		else if(if_use_bond_table_local=="N") if_use_bond_table=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Generate_Bonds value does not exist");
			throw new GMXBException(message);	
		}
		
		equity_table = new GMXBParameters(config.c_str(), "equity_table");

		if(if_use_bond_table ==1)
		bond_table		  = new GMXB_Assumption_Table(config.c_str(), "bond_table");      
		if(if_use_rate_table ==1)
		rate_table		  = new GMXB_Assumption_Table(config.c_str(), "rate_table");

		initial           = new GMXB_Assumption_Table(config.c_str(), "initial_spot_rate");
		correlation_table = new GMXB_Assumption_Table(config.c_str(), "correlation_table"); 
		vol_table         = new GMXB_Assumption_Table(config.c_str(), "vol_table");   
		error_table		  = new GMXB_Assumption_Table(config.c_str(), "error_table");                   
		
		string calibration_local=assumptions->search("Calibrate").c_str();
		if(calibration_local=="Y") calibration=1;
		else if(calibration_local=="N") calibration=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Calibrate value does not exist");
			throw new GMXBException(message);	
		}

		delta_t= atoi(assumptions->search("Time_Step").c_str());
		num_initial=100*max(12,delta_t);
		initial_spot_rate=new double [num_initial];
		initial_spot_rate_dates=new double [num_initial];

		num_equity=(int)vol_table->get_column_high_index();
		if(if_use_bond_table ==1)
		{
			num_bond=(int)bond_table->get_row_high_index();     
		}
		else
		{
		    num_bond = 0;
		}

		if(if_use_rate_table ==1)
		{
			num_rate=(int)rate_table->get_row_high_index();     
		}
		else
		{
		    num_rate = 0;
		}
		     
		num_vol=(int)vol_table->get_row_high_index();      
		equity=new string [num_equity];
		bond=new double [num_bond];
		bond_coupon=new double [num_bond];
		bond_freq=new double [num_bond];
		rate=new double [num_rate];
		rate_type=new int [num_rate];
		rate_period=new double [num_rate];

		correlation=new double *[num_equity+1];
		vol=new double *[num_equity+1];
		error=new double [num_equity+1];
		add_error=new double [num_equity+1];
		max_try=new int [num_equity+1];
		for(int i=0;i<num_equity;i++){
			stringstream ss;
			ss<<(i+1);
			string equity_number = "equity " + ss.str();
			equity[i]=equity_table->search(equity_number);
		}
		
		if(if_use_bond_table ==1)
		{
			for(int i=0;i<num_bond;i++){
				bond[i]=bond_table->search(i+1,0);
				bond_coupon[i]=bond_table->search(i+1,1);
				bond_freq[i]=bond_table->search(i+1,2);
			}
		}
		if(if_use_rate_table ==1)
		{
			for(int i=0;i<num_rate;i++){
				rate[i]=rate_table->search(i+1,0);
				rate_type[i]= (int) rate_table->search(i+1,1);
				rate_period[i]=rate_table->search(i+1,2);
			}
		}
		
		for(int i=0;i<num_equity+1;i++){
			correlation[i]=new double [num_equity+1];
			error[i]=error_table->search(i+1,0);
			add_error[i]=error_table->search(i+1,1);
			max_try[i]= (int) error_table->search(i+1,2);
			for(int j=0;j<num_equity+1;j++){
				correlation[i][j]=correlation_table->search(i+1,j);
			}
		}
		for(int i=0;i<num_equity+1;i++){
			vol[i]=new double [num_vol];
			for(int j=0;j<num_vol;j++){
				vol[i][j]=vol_table->search(j+1,i);
			}
		}
		for(int i=0;i<num_initial;i++){
			initial_spot_rate_dates[i]=initial->search(i+1,0);
		}

		year=atoi(assumptions->search("Number_Year").c_str());
		scenario=atoi(assumptions->search("Number_Scenario").c_str());
		string antithetic_local=assumptions->search("Antithetic_Option").c_str();
		if(antithetic_local=="Y") antithetic=true;
		else if (antithetic_local=="N") antithetic=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Antithetic_Option value does not exist");
			throw new GMXBException(message);	
		}
		if(antithetic==true && scenario%2==1){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Antithetic_Option activated : Must have an even number of paths");
			throw new GMXBException(message);
		}
		
		string output_scenario_local=assumptions->search("Write_Scenario").c_str();
		if(output_scenario_local=="Y") output_scenario=1;
		else if(output_scenario_local=="N") output_scenario=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Write_Scenario value does not exist");
			throw new GMXBException(message);	
		}

		WildcardManager wm( config, "wildcards" );
		f_scenario_address = wm.getPath(assumptions->search("Scenario_File_Name"));

		string debug_info_local=assumptions->search("Debug_Generation_Info").c_str();
		if(debug_info_local=="Y") debug_info=1;
		else if(debug_info_local=="N") debug_info=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Debug_Martingality_Info value does not exist");
			throw new GMXBException(message);	
		}
		debug_address=wm.getPath(assumptions->search("Debug_Generation_Address"));

		string test_martingale_local=assumptions->search("Martingale_Test").c_str();
		if(test_martingale_local=="Y") test_martingale=1;
		else if(test_martingale_local=="Use_Seed_File") test_martingale=2;
		else if(test_martingale_local=="Adj_Test_Level") test_martingale=3;
		else if (test_martingale_local=="N")test_martingale=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Martingale_Test value does not exist");
			throw new GMXBException(message);	
		}
		test_beta=test_martingale;

		string closed_formula_local=assumptions->search("Optimize_Bond_Formula").c_str();
		if(closed_formula_local=="Y") closed_formula=1;
		else if(closed_formula_local=="N") closed_formula=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Optimize_Bond_Formula value does not exist");
			throw new GMXBException(message);	
		}

		precision=atoi(assumptions->search("Output_Precision").c_str());
		if_generate_scenario=assumptions->search("Generate_Scenario").c_str();

		string new_interpolation_local=assumptions->search("New_Interpolation").c_str();
		if(new_interpolation_local=="Y") new_interpolation=true;
		else if(new_interpolation_local=="N") new_interpolation=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "New_Interpolation value does not exist");
			throw new GMXBException(message);
		}

		if(delta_t != 12 && (new_interpolation==false || closed_formula==0))
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "If time step different from 12, New_Interpolation must be Y and Optimize_Bond_Formula must be Y");
			throw new GMXBException(message);	
		}

		string vol_HW_local=assumptions->search("Treat_Vol_Extract_HW").c_str();
		if(vol_HW_local=="Y") vol_HW=true;
		else if(vol_HW_local=="N") vol_HW=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Treat_Vol_Extract_HW value does not exist");
			throw new GMXBException(message);
		}

		string vol_implied_to_loc_local=assumptions->search("Treat_Vol_Implied_To_Loc").c_str();
		if(vol_implied_to_loc_local=="Y") vol_implied_to_loc=true;
		else if(vol_implied_to_loc_local=="N") vol_implied_to_loc=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Treat_Vol_Implied_To_Loc value does not exist");
			throw new GMXBException(message);
		}

		s=0;
}


InterfaceEsg::~InterfaceEsg( void )
{
	    delete assumptions;		
		delete equity_table;	
		if(if_use_bond_table ==1)
			delete bond_table;		
		if(if_use_rate_table ==1)
			delete rate_table;		 
		delete initial;          
		delete correlation_table;
		delete vol_table;        
		delete error_table;		

		delete [] equity;
		delete [] bond;
		delete [] bond_coupon;
		delete [] bond_freq;
		delete [] rate;
		delete [] rate_type;
		delete [] rate_period;
		delete [] initial_spot_rate;
		delete [] initial_spot_rate_dates;
		for(int i=0;i<num_equity+1;i++){
			delete [] correlation[i];
			delete [] vol[i];
		}
		delete [] correlation;
		delete [] vol; 
		delete [] error;
		delete [] add_error;
		delete [] max_try;
		if (s!=0)
			delete s;
}

double*** InterfaceEsg::Gen(InterfaceCalibration& iCalib, 
							double& a,
							double& sigma,
							int scenario_pass_number,
							bool if_calib,
							int bucket_id, int w_id)
{
	//cout << "ESG : scen_pass_num = " << scenario_pass_number << "   bucket_id = " << bucket_id << endl;
	clock_t t0, t4;
	t0 = clock();

	if (s!=0){delete s;}
	// Don't do calibration, read a,sigma,zc from file.

    if(bucket_id == -1)  // STRUCTRUATION!
	{
		if(calibration == false)
		{
			iCalib.HE_mode_calibration=0;
			iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id, w_id);

			string read_a_and_sigma_from_file = assumptions->search("Read_a_and_sigma_from_file").c_str();
			if(read_a_and_sigma_from_file=="Y")
			{
				string calib_debug_address = iCalib.debug_address + "calibration parameters_1.csv"; 
				ifstream file3(calib_debug_address.c_str(), ios::in);
				double * tab = new double[2];
				read_a_and_sigma(file3,tab);
				a = tab[0];
				sigma = tab[1];
				delete [] tab;
			}
			else
			{
					a = atof(assumptions->search("a").c_str());
					sigma = atof(assumptions->search("sigma").c_str());
			}
			cout << "Reading a and sigma from file, without calibration. " << endl;
			cout << "a = " << a << "  " << "\tsigma = " << sigma << endl;
			// To do calibration
			// Scenario pass number is changed by bucket id in PH code
		}
		else
		{
			iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,w_id);
		}
	}
	// HEDGING!
	else                 
	{
		if (if_calib)
		{
			if (calibration)
			{
				iCalib.HE_mode_calibration=1;
				iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,w_id);
			}
			else
			{
				iCalib.HE_mode_calibration=0;
				iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,w_id);
				
				string read_a_and_sigma_from_file = assumptions->search("Read_a_and_sigma_from_file").c_str();
				if (read_a_and_sigma_from_file=="Y")
				{
					stringstream ss;
					ss << scenario_pass_number;
					string	calib_debug_address = iCalib.debug_address + "calibration parameters_" + ss.str() +"_1.csv"; // baseline's bucket_id = 0 
					ifstream file3(calib_debug_address.c_str(), ios::in);
					double * tab = new double[2];
					read_a_and_sigma(file3,tab);
					a = tab[0];
					sigma = tab[1];
					delete [] tab;
				}
				else
				{
					a = atof(assumptions->search("a").c_str());
					sigma = atof(assumptions->search("sigma").c_str());
				}
			}
		}
		else
		{
				iCalib.HE_mode_calibration=0;
				iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,w_id);

		}
	}
	cout << endl;
	
	if (if_generate_scenario=="N")
		return NULL;
	
	if(new_interpolation==false){
		double* tmp = new double [num_initial];
		for(int i=0;i<num_initial;i++){
			tmp[i] = linear_interp(initial_spot_rate_dates, initial_spot_rate, num_initial, (i+1)/12.0);
		}
		for(int i=0;i<num_initial;i++){
			initial_spot_rate[i] = tmp[i];
		}
		delete[] tmp;
	}

	//t1 = clock();

	Mosesrand();
	Variance_Reduction t(antithetic,test_martingale,test_beta);
    stringstream ss;
	ss << scenario_pass_number;
	if(bucket_id != -1)
	{
		ss << "_";
		ss << bucket_id;
	}
	string f_scenario_address_temp = f_scenario_address + "Scenario_" + ss.str() + ".csv";
	s = new Scenario(scenario,
					  antithetic,
					  num_equity,
					  num_bond,
					  num_rate,
				      year*delta_t,
					  num_initial,
					  num_vol,
					  a,
					  sigma,
					  delta_t,
					  error,              num_equity+1,   // error_size
					  add_error,          num_equity+1,   
					  max_try,            num_equity+1,   
					  vol,	              //num_vol,     //vol OK pour SmartPointer
					  bond,               num_bond,       // bond_size
					  bond_coupon,        num_bond,       
					  bond_freq,          num_bond,       
					  rate,               num_rate,       // rate_size,
					  rate_type,          num_rate,
					  rate_period,        num_rate,
					  initial_spot_rate,  num_initial, // initial_spot_rate_size,
					  correlation,        num_equity+2/*,  // correlation_size1, correlation_size2,
					  equity,             num_equity*/);
	
	ofstream filevol;
	if(debug_info==1){filevol.open((debug_address + "vol_" + ss.str() + ".csv").c_str());}
	for(int index=0;index<s->bi().nb_eq();index++){
		if(debug_info==1){filevol<<"INDEX "<<index<<endl<<endl;}
		s->convert_vol(index, a, sigma, s->bi().cor(0,index+2),vol_HW,vol_implied_to_loc,debug_info,filevol);
		if(debug_info==1){filevol<<endl;}
	}
	if(debug_info==1){filevol.close();}
	
	s->initial(s->bi());
	//t2 = clock();
	unsigned long * seed = new unsigned long[N+1];
	unsigned long * seed_bis = new unsigned long[N+1];
	ofstream file1;
	ifstream file2((debug_address + "seed.csv").c_str(), ios::in);
	
	if(test_martingale==2){read_seed(file2,seed);}
	Generator::Gaussian(s->bi(),s->random,seed,0,2+s->bi().nb_eq(),t.mart());
	if(test_martingale==3){Outputseed(seed_bis);}

	if(debug_info==1 && (test_martingale!=2) && w_id ==1){
		file1.open((debug_address + "seed.csv").c_str());
		write_seed(0,file1,seed);
	}
	int* succeed_martingality_test = new int[1+s->bi().nb_eq()];
	for(int i=0;i<s->bi().nb_eq()+1;i++){
		// calculate scenarios
		if(test_martingale==2){
			int adjust=0;
			if(i>0){adjust=1;}
			int martingale = read_seed(file2,seed);
			if(martingale!=1){Generator::Gaussian(s->bi(),s->random,seed,i+adjust,2-adjust,t.mart());}
		}
		succeed_martingality_test[i] = Generator::Indice_Generator(i,s,t,s->bi(),seed);
		if(debug_info==1 && test_martingale<2 && w_id ==1){write_seed(succeed_martingality_test[i],file1,seed);}
		if(test_martingale==3){
			if(succeed_martingality_test[i]==0){
				Generator::Gaussian_reset(s->bi(),s->random,2+s->bi().nb_eq(),i);
				Inputseed(seed_bis);
				s->modif_martingality(i);
				i--;
				cout << endl << endl;
			}
			else if(debug_info==1 && w_id ==1){write_seed(succeed_martingality_test[i],file1,seed);}
			Outputseed(seed_bis);
		}
	}



	if(debug_info==1 && (test_martingale!=2) && w_id ==1){
		file1.close();
	}
	delete[] seed;
	delete[] seed_bis;

	if(debug_info==1 && w_id ==1){
		ofstream martingality;
		martingality.open((debug_address + "martingality_" + ss.str() + ".csv").c_str());
		//martingality.open(debug_address.c_str());
		martingality<<"Index"<<","<<"Level"<<","<<"Number"<<endl;
		martingality<<"Rates"<<","<<s->bi().error(0)<<","<<succeed_martingality_test[0]<<endl;
		for(int i=1;i<s->bi().nb_eq()+1;i++){
			martingality<<equity[i-1]<<","<<s->bi().error(i)<<","<<succeed_martingality_test[i]<<endl;
		}
		martingality.close();
	}
	delete[] succeed_martingality_test;
	//t3 = clock();
	Generator::calcul_scenario(s,s->bi(),closed_formula);
	t4 = clock();
	//cout << "time to calibrate : " << (t1-t0)/1000 << "s" << endl;
	//cout << "time to initialise scenario : " << (t2-t1)/1000 << "s" << endl;
	//cout << "time to generate indexes : " << (t3-t2)/1000 << "s" << endl;
	//cout << "time to calculate scenario : " << (t4-t3)/1000 << "s" << endl;
	cout << "total calculation time : " << (t4-t0)/1000 << "s" << endl;
    
	// affichage
	if(output_scenario == 1 && w_id ==1)
	{
		ofstream scen;
		scen.open(f_scenario_address_temp.c_str());
		s->affiche(scen,precision,equity);
		scen.close();
		//t5 = clock();
	}
	Mosesrand();
	return s->get_scenario();
}

double*** InterfaceEsg::Gen_he_mode(InterfaceCalibration& iCalib,
									double& a,
									double& sigma, 
									int scenario_pass_number, 
									bool if_calib,
									int bucket_id)
{
	if (s!=0){delete s;}

	// Don't do calibration, read a,sigma,zc from file.
	if(calibration == false || if_calib == false){
		a = this->a_without_calib;
		sigma = this->sigma_without_calib;
		
		iCalib.HE_mode_calibration=0;
		//! if need to add bucket_id ???????
		//iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,1);

		cout << "Reading a, sigma and initial spot rates from file, without calibration. " << endl;
		cout << "a = " << a << "  " << "\tsigma = " << sigma << endl;
	}
	// To do calibration
	// Scenario pass number is changed by bucket id in PH code
	else
	{
		iCalib.Calibrate(a,sigma,initial_spot_rate,scenario_pass_number,bucket_id,1);
	}
	cout << endl;
	
	if (if_generate_scenario=="N")
		return NULL;
	
	if(new_interpolation==false){
		double* tmp = new double [num_initial];
		for(int i=0;i<num_initial;i++){
			tmp[i] = linear_interp(initial_spot_rate_dates, initial_spot_rate, num_initial, (i+1)/12.0);
		}
		for(int i=0;i<num_initial;i++){
			initial_spot_rate[i] = tmp[i];
		}
		delete[] tmp;
	}

	Mosesrand();
	Variance_Reduction t(antithetic,test_martingale,test_beta);
    stringstream ss;
	ss << scenario_pass_number;
	if(bucket_id != -1)
	{
		ss << "_";
		ss << bucket_id;
	}
	string f_scenario_address_temp = f_scenario_address + "Scenario_" + ss.str() + ".csv";
	s = new Scenario(scenario,
					  antithetic,
					  num_equity,
					  num_bond,
					  num_rate,
				      year*delta_t,
					  num_initial,
					  num_vol,
					  a,
					  sigma,
					  delta_t,
					  //! Yuan Ajouter les tailles pour les tables, mais c'est mieux de utiliser "vector" que "table*"
					  error,              num_equity+1,   // error_size
					  add_error,          num_equity+1,   
					  max_try,            num_equity+1,   
					  vol,	              //num_vol,     //vol OK pour SmartPointer
					  bond,               num_bond,       // bond_size
					  bond_coupon,        num_bond,       
					  bond_freq,          num_bond,       
					  rate,               num_rate,       // rate_size,
					  rate_type,          num_rate,
					  rate_period,        num_rate,
					  initial_spot_rate,  num_initial, // initial_spot_rate_size,
					  correlation,        num_equity+2/*,  // correlation_size1, correlation_size2,
					  equity,             num_equity*/);

	ofstream filevol;
	if(debug_info==1){filevol.open((debug_address + "vol_" + ss.str() + ".csv").c_str());}
	for(int index=0;index<s->bi().nb_eq();index++){
		if(debug_info==1){filevol<<"INDEX "<<index<<endl<<endl;}
		s->convert_vol(index, a, sigma, s->bi().cor(0,index+2),vol_HW,vol_implied_to_loc,debug_info,filevol);
		if(debug_info==1){filevol<<endl;}
	}
	if(debug_info==1){filevol.close();}

	s->initial(s->bi());
	unsigned long * seed = new unsigned long[N+1];
	unsigned long * seed_bis = new unsigned long[N+1];
	ofstream file1;
	ifstream file2((debug_address + "seed.csv").c_str(), ios::in);
	
	if(test_martingale==2){read_seed(file2,seed);}
	Generator::Gaussian(s->bi(),s->random,seed,0,2+s->bi().nb_eq(),t.mart());
	if(test_martingale==3){Outputseed(seed_bis);}

	if(debug_info==1 && (test_martingale!=2)){
		file1.open((debug_address + "seed.csv").c_str());
		write_seed(0,file1,seed);
	}
	int* succeed_martingality_test = new int[1+s->bi().nb_eq()];
	for(int i=0;i<s->bi().nb_eq()+1;i++){
		// calculate scenarios
		if(test_martingale==2){
			int adjust=0;
			if(i>0){adjust=1;}
			int martingale = read_seed(file2,seed);
			if(martingale!=1){Generator::Gaussian(s->bi(),s->random,seed,i+adjust,2-adjust,t.mart());}
		}
		succeed_martingality_test[i] = Generator::Indice_Generator(i,s,t,s->bi(),seed);
		if(debug_info==1 && test_martingale<2){write_seed(succeed_martingality_test[i],file1,seed);}
		if(test_martingale==3){
			if(succeed_martingality_test[i]==0){
				Generator::Gaussian_reset(s->bi(),s->random,2+s->bi().nb_eq(),i);
				Inputseed(seed_bis);
				s->modif_martingality(i);
				i--;
				cout << endl << endl;
			}
			else if(debug_info==1){write_seed(succeed_martingality_test[i],file1,seed);}
			Outputseed(seed_bis);
		}
	}
	if(debug_info==1 && (test_martingale!=2)){
		file1.close();
	}
	delete[] seed;
	delete[] seed_bis;

	if(debug_info==1){
		ofstream martingality;
		martingality.open((debug_address + "martingality_" + ss.str() + ".csv").c_str());
		//martingality.open(debug_address.c_str());
		martingality<<"Index"<<","<<"Level"<<","<<"Number"<<endl;
		martingality<<"Rates"<<","<<s->bi().error(0)<<","<<succeed_martingality_test[0]<<endl;
		for(int i=1;i<s->bi().nb_eq()+1;i++){
			martingality<<equity[i-1]<<","<<s->bi().error(i)<<","<<succeed_martingality_test[i]<<endl;
		}
		martingality.close();
	}
	delete[] succeed_martingality_test;
	Generator::calcul_scenario(s,s->bi(),closed_formula);
    
	// affichage
	if(output_scenario == 1)
	{
		ofstream scen;
		scen.open(f_scenario_address_temp.c_str());
		s->affiche(scen,precision,equity);
		scen.close();
	}
	Mosesrand();
	return s->get_scenario();
}

void InterfaceEsg::delete_s()
{
	delete s;
	s = 0;
}

void InterfaceEsg::write_seed(int martingality, ofstream &file1,unsigned long * seed){
	file1<<martingality<<",";
	for(int i=0;i<N+1;i++){
		file1<<seed[i]<<",";
	}
	file1<<endl;
}

int InterfaceEsg::read_seed(ifstream &file2,unsigned long * seed){
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

void InterfaceEsg::set_a_without_calib(double a)
{
	a_without_calib=a;
}

void InterfaceEsg::set_sigma_without_calib(double sigma)
{
	sigma_without_calib=sigma;
}

void InterfaceEsg::set_initial_spot_rate(double *ZC)
{
	for(int i=0;i<num_initial;i++)
		initial_spot_rate[i]=ZC[i];
}

void InterfaceEsg::read_a_and_sigma(ifstream &file3,double * tab){
	string row;
	getline(file3,row);
	char* retPtr = new char[row.length()+1];
	strcpy_s(retPtr, row.length()+1, row.c_str());
	char* context = NULL;
	strtok_s(retPtr, ",",&context);
	tab[0] = strtod(strtok_s(NULL, ",",&context),NULL);
	delete [] retPtr;

	getline(file3,row);
	char* retPtr2 = new char[row.length()+1];
	strcpy_s(retPtr2, row.length()+1, row.c_str());
	char* context2 = NULL;
	strtok_s(retPtr2, ",",&context2);
	tab[1] = strtod(strtok_s(NULL, ",",&context2),NULL);
	delete [] retPtr2;
}