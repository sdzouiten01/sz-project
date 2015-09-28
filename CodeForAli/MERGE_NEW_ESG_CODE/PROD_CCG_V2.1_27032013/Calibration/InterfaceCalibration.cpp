#ifndef _DLL_FILE_
#define _DLL_FILE_
#endif

#include "InterfaceCalibration.h"
#include "Calibration.h"
#include "WildcardManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

ofstream spot_rate;
ofstream model_vol;
ofstream model_price;
ofstream swap_rate;
ofstream swap_vol;
ofstream weight_tab;


InterfaceCalibration::InterfaceCalibration( const string& config )
{
	    config_file = config;

		param_initial = new double [4];
		param_min    = new double [4];
		param_max    = new double [4];

		for(int i=0;i<4;i++){
			param_initial[i]=0;
			param_min[i]=0;
			param_max[i]=0;
		}

		swaption_vol_table = new GMXB_Assumption_Table(config.c_str(), "swaption_vol_table");
		weight_table = new GMXB_Assumption_Table(config.c_str(), "weight_table");
		assumptions = new GMXBParameters(config.c_str(), "generator_assumptions");

		nb_tenor = (int) (swaption_vol_table->get_column_high_index() - swaption_vol_table->get_column_low_index());
		nb_maturity = (int) (swaption_vol_table->get_row_high_index() - swaption_vol_table->get_row_low_index());

		delta_t = atoi(assumptions->search("Time_Step").c_str());

		param_initial[0]=atof(assumptions->search("sigma_Initial").c_str());
		param_initial[1]=atof(assumptions->search("a_Initial").c_str());
		param_min[0]=atof(assumptions->search("sigma_Min").c_str());
		param_min[1]=atof(assumptions->search("a_Min").c_str());
		param_max[0]=atof(assumptions->search("sigma_Max").c_str());
		param_max[1]=atof(assumptions->search("a_Max").c_str());

		calib=assumptions->search("Calibration_On").c_str();
		error=assumptions->search("Error_Function_Type").c_str();
		
		string bootstrapping_security_local=assumptions->search("Bootstrapping_Security").c_str();
		if(bootstrapping_security_local=="Y") bootstrapping_security=true;
		else if(bootstrapping_security_local=="N") bootstrapping_security=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Bootstrapping_Security value does not exist");
			throw new GMXBException(message);
		}
		
		string new_interpolation_local=assumptions->search("New_Interpolation").c_str();
		if(new_interpolation_local=="Y") new_interpolation=true;
		else if(new_interpolation_local=="N") new_interpolation=false;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "New_Interpolation value does not exist");
			throw new GMXBException(message);
		}

		string interpolation_type_local=assumptions->search("Interpolation_Type").c_str();
		if(interpolation_type_local=="LOGLINEAR"){interpolation_type=1;}
		else if(interpolation_type_local=="LINEAR"){interpolation_type=2;}
		else if(interpolation_type_local=="CONSTANT"){interpolation_type=3;}
		else if(interpolation_type_local=="SPLINE"){interpolation_type=4;}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Interpolation_Type value does not exist");
			throw new GMXBException(message);	
		}

		string extrapolation_type_start_local=assumptions->search("Extrapolation_Start_Curve").c_str();
		if(extrapolation_type_start_local=="LOGLINEAR"){extrapolation_type_start=1;}
		else if(extrapolation_type_start_local=="LINEAR"){extrapolation_type_start=2;}
		else if(extrapolation_type_start_local=="CONSTANT"){extrapolation_type_start=3;}
		else if(extrapolation_type_start_local=="SPLINE"){extrapolation_type_start=4;}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Extrapolation_Start_Curve value does not exist");
			throw new GMXBException(message);	
		}

		string extrapolation_type_end_local=assumptions->search("Extrapolation_End_Curve").c_str();
		if(extrapolation_type_end_local=="LOGLINEAR"){extrapolation_type_end=1;}
		else if(extrapolation_type_end_local=="LINEAR"){extrapolation_type_end=2;}
		else if(extrapolation_type_end_local=="CONSTANT"){extrapolation_type_end=3;}
		else if(extrapolation_type_end_local=="SPLINE"){extrapolation_type_end=4;}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Extrapolation_End_Curve value does not exist");
			throw new GMXBException(message);	
		}

		string swaption_type_local=assumptions->search("Swaption_Type").c_str();
		if(swaption_type_local=="payer") swaption_type=1;
		else if(swaption_type_local=="receiver") swaption_type=-1;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Swaption_Type value does not exist");
			throw new GMXBException(message);
		}
		
		K=atof(assumptions->search("Swaption_Strike").c_str());

		string coupon_period_local=assumptions->search("Coupon_Period").c_str();
		if(coupon_period_local=="ANNUAL") coupon_period=1.;
		else if(coupon_period_local=="SEMI_ANNUAL")coupon_period=0.5;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Coupon_Period value does not exist");
			throw new GMXBException(message);
		}

		string debug_info_local=assumptions->search("Debug_Calibration_Info").c_str();
		if(debug_info_local=="Y") debug_info=1;
		else if(debug_info_local=="N") debug_info=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Debug_Calibration_Info value does not exist");
			throw new GMXBException(message);
		}
		
		WildcardManager wm( config, "wildcards" );
		debug_address=wm.getPath(assumptions->search("Debug_Calibration_Address"));

		swap_tenor=new double [nb_tenor];
		swap_maturity=new double [nb_maturity];
		swaption_vol=new double *[nb_maturity];
		weight=new double *[nb_maturity];
		
		string if_read_swap_rate_from_excel_local=assumptions->search("Read_Swap_Rate_From_Excel").c_str();
		if(if_read_swap_rate_from_excel_local=="Y") if_read_swap_rate_from_excel=1;
		else if(if_read_swap_rate_from_excel_local=="N") if_read_swap_rate_from_excel=0;
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Read_Swap_Rate_From_Excel value does not exist");
			throw new GMXBException(message);
		}

		swap_rate_table = new GMXB_Assumption_Table(config.c_str(), "swap_rate_table");
		
		if(if_read_swap_rate_from_excel == 1)
		{
			int nb_total_date = (int) swap_rate_table->get_row_high_index();
			this->nb_date = 0;
			
			int* weightsCurve = new int[nb_total_date];
			for(int i=0;i<nb_total_date;i++){
				weightsCurve[i] = (int) swap_rate_table->search(i+1,2);
				if(weightsCurve[i]!=0){this->nb_date++;}
			}
			this->nb_date_initial=this->nb_date;
			this->dateCurve = new double[this->nb_date];
			this->zeroCurve = new double[this->nb_date];
			this->RateCurve = new double[this->nb_date];
			this->dateCurve_initial = new double[this->nb_date_initial];
			this->zeroCurve_initial = new double[this->nb_date_initial];
			this->RateCurve_initial = new double[this->nb_date_initial];
			this->SwapRateTable = new double[this->nb_date];
			int count = 0;
			for(int i=0;i<nb_total_date;i++){
				if(weightsCurve[i]!=0){
					dateCurve[count] = swap_rate_table->search(i+1,0);

					
					zeroCurve[count] = swap_rate_table->search(i+1,1)*100; //HA
					//zeroCurve[count] = swap_rate_table->search(i+1,1); // HE
					
					RateCurve[count] = zeroCurve[count];
					SwapRateTable[count] = zeroCurve[count];
					dateCurve_initial[count]=dateCurve[count];
					zeroCurve_initial[count]=zeroCurve[count];
					RateCurve_initial[count]=RateCurve[count];
					count++;
				}
			}
			delete[] weightsCurve;

		}
		else{
			//// Read the swap rate curve in parameters from Sophis file
			//this->input_swap_rate  = wm.getPath(assumptions->search("Input_Swap_Rate"));
			//string currency = assumptions->search("Currency").c_str();
			//ifstream file_swap_rate( input_swap_rate.c_str() );
			//string row;
			//vector<string> row_vector;
			//istringstream row_streamed;
			//string elementCurrency, elementDate, elementType, elementRate;
			//vector<double> vDateCurve;
			//vector<double> vRateCurve;

			//if (!file_swap_rate.fail())
			//{	
			//	const int CURRENCY_COL = 2+2;
			//	const int DATE_COL = 6+2;          
			//	const int CURRENCY_TYPE_COL = 7+2; 
			//	const int RATE_COL = 10+2;

			//	const char DLIM = ';';
			//	int itr;
			//	
			//	while(!file_swap_rate.eof())
			//	{
			//		getline(file_swap_rate, row);
			//		row_streamed.str(row);
			//		itr = 1;

			//		while(itr++ < CURRENCY_COL){
			//			getline(row_streamed, elementCurrency, DLIM);
			//		}

			//		if(elementCurrency != currency)
			//		{
			//			row_streamed.clear();
			//			continue;
			//		}else{
			//			while(itr++ < DATE_COL+1){
			//				 getline(row_streamed, elementDate, DLIM);
			//			}
			//			while(itr++ < CURRENCY_TYPE_COL+2){
			//				 getline(row_streamed, elementType, DLIM);
			//			}
			//			if(elementType != "Y")
			//			{
			//				row_streamed.clear();
			//				continue;
			//			}else{
			//				while(itr++ < RATE_COL+3){
			//					 getline(row_streamed, elementRate, DLIM);
			//				}
			//					
			//				vDateCurve.push_back(atof(elementDate.c_str()));
			//				vRateCurve.push_back(atof(elementRate.c_str()));
			//				row_streamed.clear();
			//			}
			//		}
			//	}
			//	file_swap_rate.close();



			// Read the swap rate curve in parameters from tbl file
			this->input_swap_rate  = wm.getPath(assumptions->search("Input_Swap_Rate"));
			ifstream file_swap_rate( input_swap_rate.c_str() );
			vector<double> vDateCurve;
			vector<double> vRateCurve;

			int counter=0;
			string s;
			//! 
			file_swap_rate >> s;  //! TIME	
			file_swap_rate >> s;  //! YIELDS
			s = "";

			if (!file_swap_rate.fail())
			{
				
				while(!file_swap_rate.eof())
				{
					counter ++;
					file_swap_rate >> s;
					if(s != "")
					{
						if(counter%2==1)
						{
							vDateCurve.push_back(atof(s.c_str()));
						}else
						{
							vRateCurve.push_back(atof(s.c_str()));
						}
					}else{
					    break;
					}
					s = "";
				}
			}else{
						char message[MAX_LENGTH_MESSAGE_EXCEPTION];
						sprintf_s(message, "The file %s is not found",input_swap_rate.c_str());
						throw new GMXBException(message);	
			}

			this->nb_date = vDateCurve.size();
			this->nb_date_initial=this->nb_date;
			this->dateCurve = new double[this->nb_date];
			this->zeroCurve = new double[this->nb_date];
			this->RateCurve = new double[this->nb_date];
			this->SwapRateTable = new double[this->nb_date];
			this->dateCurve_initial = new double[this->nb_date_initial];
			this->zeroCurve_initial = new double[this->nb_date_initial];
			this->RateCurve_initial = new double[this->nb_date_initial];

			for(int i=0; i<this->nb_date; i++){
				this->dateCurve[i] = vDateCurve[i];
				this->zeroCurve[i] = vRateCurve[i];
				this->RateCurve[i] = vRateCurve[i];
				SwapRateTable[i]   = vRateCurve[i];
				this->dateCurve_initial[i] = dateCurve[i];
			    this->zeroCurve_initial[i] = zeroCurve[i];
			    this->RateCurve_initial[i] = RateCurve[i];
			}
		}

		for(int i=0;i<nb_tenor;i++){
			swap_tenor[i]=swaption_vol_table->search(0,i+1);
		}

		for(int i=0; i < nb_maturity; i++){
			swap_maturity[i] = swaption_vol_table->search(i+2,0);
			swaption_vol[i] = new double [nb_tenor];
			weight[i] = new double [nb_tenor];
			for(int j=0;j<nb_tenor;j++){
				swaption_vol[i][j] = swaption_vol_table->search(i+2,j+1);
				weight[i][j] = weight_table->search(i+2,j+1);
				if(weight[i][j]>=0){
					weight[i][j]=sqrt(weight[i][j]);
				}
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Negative weights for swaptions");
					throw new GMXBException(message);
				}
			}
		}
		HE_mode_calibration=1;
		last_model_date = 100;
		nb_swap_date = (int) (12+(1.0/coupon_period)*(last_model_date-1));
		swap_curve = new double [nb_swap_date];
		date_swap_curve = new double [nb_swap_date];
		for(int i=0;i<12;i++){
			date_swap_curve[i]=(i+1)/12.0;
		}
		for(int i=12;i<nb_swap_date;i++){
			date_swap_curve[i]=date_swap_curve[i-1]+coupon_period;
		}

		if(new_interpolation==true){Interpolate_Swap();}
		
}

InterfaceCalibration ::	~InterfaceCalibration()
{
	    delete swaption_vol_table;
		delete weight_table;
		delete assumptions;
		if(if_read_swap_rate_from_excel == 1){
			delete swap_rate_table;
		}

		delete[] param_initial;
		delete[] param_min;
		delete[] param_max;

		delete [] swap_tenor;
		delete [] swap_maturity;
		for(int i=0;i<nb_maturity;i++){
			delete [] swaption_vol[i];
			delete [] weight[i];
		}
		delete [] swaption_vol;
		delete [] weight;

		delete[] dateCurve;
		delete[] zeroCurve;
		delete[] RateCurve;
		delete[] SwapRateTable;
		delete[] swap_curve;
		delete[] date_swap_curve;
		delete[] zeroCurve_initial;
		delete[] RateCurve_initial;
		delete[] dateCurve_initial;
}


void InterfaceCalibration::Calibrate( double& a, double& sigma, double* zc, int scenario_pass_number, int bucket_id, int w_id)
{
		cout << endl << "          CALIBRATION" << endl << endl;
		double **modelvol=new double *[nb_maturity];
		for(int i=0; i < nb_maturity; i++){
			modelvol[i] = new double[nb_tenor];
		}

		double * modelprice=new double [nb_maturity*nb_tenor];
		double* r = new double;

		double* param_out=new double [4];
		for(int i=0; i<4; i++){
			param_out[i] = 0;
		}

		Calibration::Optimisation_LM(dateCurve,
									 zeroCurve,
									 interpolation_type,
									 extrapolation_type_start,
									 extrapolation_type_end,
									 swap_maturity,
									 swap_tenor, 
									 swaption_vol,
									 weight,
									 nb_date,
									 nb_maturity,
									 nb_tenor,
									 K,
									 param_initial,
									 param_min,
									 param_max,
									 param_out,
									 modelvol,
									 modelprice,
									 zc,
									 calib,
									 error,
									 swaption_type,
									 coupon_period,
									 r,
									 HE_mode_calibration,
									 swap_curve,
									 last_model_date,
									 new_interpolation,
									 delta_t,
									 bootstrapping_security);

		if(HE_mode_calibration==1)
		{
			a = param_out[1];
			sigma = param_out[0];
		}
	
		if(HE_mode_calibration==1)
		{
			cout << "a = " << a << "\tsigma = " << sigma << "\terror : " << *r << endl;
		}

		// Debug
		if(debug_info==1 && w_id ==1)
		{
			stringstream stst;
			stst << scenario_pass_number;

			if(bucket_id != -1)
			{
			   stst << "_" << bucket_id;
			}
			string ss = debug_address + "calibration parameters_" + stst.str() + ".csv";

			if(HE_mode_calibration==1){
				ofstream calibration_parameters;
				calibration_parameters.open(ss.c_str());
				calibration_parameters<<"a"<<","<<setprecision(6)<<a<<endl;
				calibration_parameters<<"sigma"<<","<<setprecision(6)<<sigma<<endl;
				calibration_parameters.close();
			}

			ofstream spot_rate;
			ss = debug_address + "initial spot rates_" + stst.str() + ".csv";
			spot_rate.open(ss.c_str());
			spot_rate<<"TIME"<<","<<"YIELDS"<<endl;
			for(int i=0;i<last_model_date*delta_t;i++){
				spot_rate<<setprecision(4)<<(i+1.)/delta_t <<","<<setprecision(6)<<zc[i]/100.0<<endl;
			}
			spot_rate.close();

			ofstream swap;
			ss = debug_address + "swap curve_" + stst.str() + ".csv";
			swap.open(ss.c_str());
			swap<<"TIME"<<","<<"YIELDS"<<endl;
			
			if (this->new_interpolation)
			{
				for(int i=0;i<nb_swap_date;i++)	
					swap<<setprecision(4)<<date_swap_curve[i]<<","<<setprecision(6)<<swap_curve[i]/100.0<<endl;
			}
			else
			{
				for(int i=0;i<nb_date;i++)	
					swap<<setprecision(4)<<this->dateCurve[i]<<","<<setprecision(6)<<this->zeroCurve[i]/100.0 <<endl;
			}
			swap.close();
			
			if(HE_mode_calibration==1){
				ofstream swaption_volatilities;
				ss = debug_address + "swaption volatilities_" + stst.str() + ".csv";
				swaption_volatilities.open(ss.c_str());
			
				swaption_volatilities<<"ERROR PRICE"<<","<<setprecision(2)<<*r<<endl;
				swaption_volatilities<<endl;

				swaption_volatilities<<"MODEL VOLATILITIES"<<endl;
				swaption_volatilities<<" "<<",";
				for(int i=0;i<nb_tenor;i++){
					swaption_volatilities<<swap_tenor[i]<<",";
				}
				swaption_volatilities<<endl;
				for(int i=0;i<nb_maturity;i++){
					swaption_volatilities<<swap_maturity[i]<<",";
					for(int j=0;j<nb_tenor;j++){
						swaption_volatilities<<setprecision(4)<<modelvol[i][j]*100<<",";
					}
					swaption_volatilities<<endl;
				}
				swaption_volatilities<<endl;

				swaption_volatilities<<"INPUT VOLATILITIES"<<endl;
				swaption_volatilities<<" "<<",";
				for(int i=0;i<nb_tenor;i++){
					swaption_volatilities<<swap_tenor[i]<<",";
				}
				swaption_volatilities<<endl;
				for(int i=0;i<nb_maturity;i++){
					swaption_volatilities<<swap_maturity[i]<<",";
					for(int j=0;j<nb_tenor;j++){
						swaption_volatilities<<setprecision(4)<<swaption_vol[i][j]*100<<",";
					}
					swaption_volatilities<<endl;
				}
				swaption_volatilities<<endl;

				swaption_volatilities<<"ERROR VOLATILITIES"<<endl;
				swaption_volatilities<<" "<<",";
				for(int i=0;i<nb_tenor;i++){
					swaption_volatilities<<swap_tenor[i]<<",";
				}
				swaption_volatilities<<endl;
				for(int i=0;i<nb_maturity;i++){
					swaption_volatilities<<swap_maturity[i]<<",";
					for(int j=0;j<nb_tenor;j++){
						swaption_volatilities<<setprecision(2)<<(swaption_vol[i][j]-modelvol[i][j])*100<<",";
					}
					swaption_volatilities<<endl;
				}
				swaption_volatilities<<endl;
			
				swaption_volatilities<<"MODEL PRICES"<<endl;
				swaption_volatilities<<" "<<",";
				for(int i=0;i<nb_tenor;i++){
					swaption_volatilities<<swap_tenor[i]<<",";
				}
				swaption_volatilities<<endl;
				for(int i=0;i<nb_maturity;i++){
					swaption_volatilities<<swap_maturity[i]<<",";
					for(int j=0;j<nb_tenor;j++){
						swaption_volatilities<<setprecision(2)<<modelprice[i*nb_tenor+j]<<",";
					}
					swaption_volatilities<<endl;
				}
				swaption_volatilities.close();
			}
		}
		// End Debug
		

		delete[] param_out;
		delete[] modelprice; 
		delete r;
		for(int i=0;i<this->nb_maturity;i++){
			delete[] modelvol[i];
		}
		delete modelvol;
}

void InterfaceCalibration::Interpolate_Swap(){
	int coupon_frequency =int (1.0/coupon_period);
	int last_term = 100;
	double *initial_fwd = new double [12*last_term];
	Interpolation::set_forward_rate(last_term, nb_date_initial, dateCurve_initial, zeroCurve_initial,
									 interpolation_type, extrapolation_type_start, extrapolation_type_end,
									 initial_fwd, coupon_frequency, swap_curve, bootstrapping_security);
	delete[] initial_fwd;
	delete[] dateCurve;
	delete[] zeroCurve;
	delete[] RateCurve;

	dateCurve = new double [nb_swap_date];
	zeroCurve = new double [nb_swap_date];
	RateCurve = new double [nb_swap_date];
	for(int i=0;i<nb_swap_date;i++){
		dateCurve[i]=date_swap_curve[i];
		zeroCurve[i]=swap_curve[i];
		RateCurve[i]=swap_curve[i];
	}
	nb_date = nb_swap_date;
}

void InterfaceCalibration::ShockSwap( double startDate,
									  double keyDate,
									  double endDate,
									  double shockSize,
									  string& shockType)
{
	if(shockType == "bucket_shock")
	{
		for (int i=0; i<this->nb_date; i++)
		{
			if (dateCurve[i]>startDate && dateCurve[i]<endDate)
			{
				if (dateCurve[i]<=keyDate)
				{
					this->zeroCurve[i] = this->RateCurve[i]+ ((dateCurve[i]-startDate)/(keyDate-startDate))*shockSize;
				}
				else
				{
				this->zeroCurve[i] = this->RateCurve[i] + ((endDate-dateCurve[i])/(endDate-keyDate))*shockSize;
				}
			}
			else
			{
				this->zeroCurve[i] = this->RateCurve[i];
			}
		}
	}
	else if(shockType == "parallel_shift")
	{
	    for (int i=0; i<this->nb_date; i++)
		{
             this->zeroCurve[i] = max(0.11,this->RateCurve[i] + shockSize);
		}
	}
	else if(shockType == "baseline")
	{
	    for (int i=0; i<this->nb_date; i++)
		{
             this->zeroCurve[i] = this->RateCurve[i];
		}
	}

	else if(shockType == "bucket_first")
	{
	    for (int i=0; i<this->nb_date; i++)
		{
			if (dateCurve[i]<endDate)
			{
				if (dateCurve[i]<=keyDate)
				{
					this->zeroCurve[i] = this->RateCurve[i] + shockSize;
				}
				else
				{
					this->zeroCurve[i] = this->RateCurve[i] + ((endDate-dateCurve[i])/(endDate-keyDate))*shockSize;
				}
			}
			else
			{
				this->zeroCurve[i] = this->RateCurve[i];
			}
		}
	}

	else if(shockType == "bucket_last")
	{
	   for (int i=0; i<this->nb_date; i++)
		{
			if (dateCurve[i]>startDate)
			{
				if (dateCurve[i]<=keyDate)
				{
					this->zeroCurve[i] = this->RateCurve[i]+ ((dateCurve[i]-startDate)/(keyDate-startDate))*shockSize;
				}
				else
				{
					this->zeroCurve[i] = this->RateCurve[i] + shockSize;
				}
			}
			else
			{
				this->zeroCurve[i] = this->RateCurve[i];
			}
		}
	}

	else
	{
	    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "shockType does not exist, error for bucket type !");
		throw new GMXBException(message);	
	}
}

void InterfaceCalibration::UpdateSwapTable(double** scen, int t)
{
	for (int i=0; i< this->nb_date_initial; i++)
	{
		this->RateCurve_initial[i]=scen[t][i];
		this->zeroCurve_initial[i]=this->RateCurve_initial[i];
	}
	Interpolate_Swap();
}

void InterfaceCalibration::InterpolationSwapTable()
{
	double ** linear_interp= new double*[this->nb_date];
	for (int i=0; i<this->nb_date;i++)
	{
		linear_interp[i]=new double[3];
	}
	int index_ref=-1;
	bool test=false;
	for (int i=0; i< this->nb_date; i++)
	{
		if (this->SwapRateTable[i]!=0){
			if (index_ref!=-1){
				for(int j=index_ref;j<=i;j++)
				{
					linear_interp[j][0]=log(this->RateCurve[index_ref]);
					linear_interp[j][1]=this->dateCurve[j]-this->dateCurve[index_ref];
					linear_interp[j][2]=(log(this->RateCurve[i])-log(this->RateCurve[index_ref]))/(this->dateCurve[i]-this->dateCurve[index_ref]);
				}
				if (test==false){
					for(int j=0;j<=index_ref;j++)
					{
						linear_interp[j][0]=log(this->RateCurve[index_ref]);
						linear_interp[j][1]=this->dateCurve[j]-this->dateCurve[index_ref];
						linear_interp[j][2]=linear_interp[index_ref][2];
					}
					test=true;
				}
			}
			index_ref=i;
		}
	}
	for (int i=index_ref; i< this->nb_date; i++)
	{
		linear_interp[i][0]=log(this->RateCurve[index_ref]);
		linear_interp[i][1]=this->dateCurve[i]-this->dateCurve[index_ref];
		linear_interp[i][2]=linear_interp[index_ref][2];
	}

	for (int i=0; i< this->nb_date; i++)
	{
		if (this->SwapRateTable[i]==0){
			this->zeroCurve[i]=exp(linear_interp[i][0]+linear_interp[i][1]*linear_interp[i][2]);
			this->RateCurve[i]=this->zeroCurve[i];
		}
	}

	for (int i=0; i<nb_date; i++)
	{
		delete[] linear_interp[i];
	}
	delete[] linear_interp;
}


