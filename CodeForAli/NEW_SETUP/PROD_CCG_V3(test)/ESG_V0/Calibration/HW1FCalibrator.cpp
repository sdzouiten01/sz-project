
#include "HW1FCalibrator.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////HW1FSwaptionPricer//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HW1FSwaptionPricer::HW1FSwaptionPricer(){}	
HW1FSwaptionPricer::~HW1FSwaptionPricer()
{
	delete [] parameters_input;
	delete [] parameters_min_input;
	delete [] parameters_max_input;
	delete [] parameters_output;
	delete [] parameters_output_mapped;	
}	
HW1FSwaptionPricer::HW1FSwaptionPricer(string calib_type,string calib_param,double* &param_input,double* &param_min_input,double* &param_max_input)
{
	calibration_Type=calib_type;
	parameters_input=new double[4];
	parameters_min_input=new double[4];
	parameters_max_input=new double[4];
	parameters_output=new double[4];
	parameters_output_mapped=new double[4];
	for (int i=0; i<4; i++){
		parameters_input[i]=0.;
		parameters_min_input[i]=0.;
		parameters_max_input[i]=0.;
		parameters_output[i]=0.;
		parameters_output_mapped[i]=0.;
	}
	for (int i=0; i<4; i++){
		parameters_input[i]=param_input[i];
		parameters_min_input[i]=param_min_input[i];
		parameters_max_input[i]=param_max_input[i];
		parameters_output[i]=param_input[i];
	}
	calibration_on=calib_param;
	if ( calib_param == "a and sigma"|| calib_param == "sigma" )
		number_of_parameters_to_calibrate=2;
	else if(calib_param == "a")
		number_of_parameters_to_calibrate=1;
	else
		number_of_parameters_to_calibrate=0;
	to_mappped_parameters();
}	
void HW1FSwaptionPricer::to_mappped_parameters()
{
	// forward direction atanh (doesn't exist in C, so use logs)
	// maps pMin <= x <= pMax to -Inf <= x <= Inf
	double epsilon = 0.01;
	double z1,z2 ; 
	double y;
    for (int i=0; i<=number_of_parameters_to_calibrate-1; i++) {
		double lim_inf = parameters_min_input[i]/(1-epsilon); 
		double lim_sup = parameters_max_input[i]/(1+epsilon);
        y = (parameters_output[i] - 0.5*(parameters_min_input[i]+parameters_max_input[i]))/(0.5*(parameters_max_input[i]-parameters_min_input[i]));    
		z1 = (lim_inf - 0.5*(parameters_min_input[i]+parameters_max_input[i]))/(0.5*(parameters_max_input[i]-parameters_min_input[i]));
		z2 = (lim_sup - 0.5*(parameters_min_input[i]+parameters_max_input[i]))/(0.5*(parameters_max_input[i]-parameters_min_input[i]));
		double y2=y;
		y = min(max(y,z1),z2);
		if(y2!=y){cout << "danger : bounds for calibration parameters were reached" << endl;}
        parameters_output_mapped[i] = 0.5*log((1.+y)/(1.-y));
	}
}
void HW1FSwaptionPricer::to_unmappped_parameters()
{
	// backward direction uses tanh transformation
	// maps -Inf <= x <= Inf to pMin <= x <= pMax
    for (int i=0; i<=number_of_parameters_to_calibrate-1; i++) 
		parameters_output[i] = 0.5*(parameters_min_input[i]+parameters_max_input[i]) + 0.5*(parameters_max_input[i]-parameters_min_input[i])*tanh(parameters_output_mapped[i]);	
		
	if ( calibration_on == "a")
		parameters_output[1]=parameters_input[1];
	else if(calibration_on == "sigma")
		parameters_output[0]=parameters_input[0];
	else if(calibration_on ==  "a and sigma"){
		parameters_output[0]=parameters_output[0];
		parameters_output[1]=parameters_output[1];
	}
	else{
		parameters_output[0]=parameters_input[0];
		parameters_output[1]=parameters_input[1];
	}
	//to_mappped_parameters();
}
void HW1FSwaptionPricer::setMappedParameters(double *parameters_mapped) 
{
	for (int i=0; i<4; i++)
		parameters_output_mapped[i]=parameters_mapped[i];	
	to_unmappped_parameters();
}
double HW1FSwaptionPricer::getMRV()  {return parameters_output[0];}
double HW1FSwaptionPricer::getsigma() {return parameters_output[1];}
void HW1FSwaptionPricer::setParameters( double& MRV, double& Volatility)
{
	parameters_output_mapped[0]=MRV;
	parameters_output_mapped[1]=Volatility;
	to_unmappped_parameters();
}
/////////////////////////////////////////////////////////
//////////////////////////HW1FSwaptionPricer::swaption
/////////////////////////////////////////////////////////
class loc_Jamshidian
{
	protected:
		std::vector<double> myA;
		std::vector<double> myB; 
		std::vector<double> myCs;
	public:
		loc_Jamshidian(std::vector<double> A,std::vector<double> B,std::vector<double> Cs):myA(A), myB(B) , myCs(Cs) {}
		double operator()(const double& r) const;
		double derivate(const double& r) const;
};
double loc_Jamshidian::operator()(const double& r) const
{
	int size=myA.size(); int i; double result=0;
	for(i=0;i<size;i++){ result+=myCs[i]*myA[i]*exp(-myB[i]*r); }
	return result-1;
}
double loc_Jamshidian::derivate(const double& r) const
{
	int size=myA.size(); int i; double result=0;
	for(i=0;i<size;i++){ result=result-myCs[i]*myA[i]*myB[i]*exp(-myB[i]*r); }
	return result;
}
double locfunction_B( double a,  double t0 , double t)
{ 
	if ( a>=EPS)
	{
		double result= ( 1.- exp( -a*(t-t0) ) ) / a; 
		return result; }// a#0
	else return t-t0;
}
double HW1FSwaptionPricer::swaption(const double& T, const double& P_T, const double& fwd_M_Expiry, const std::vector<double>& Ci, const std::vector<double>& Cr,
									const std::vector<double>& Price, const double& Nominal/*,const double& strike*/)
{

	int i=0;
	double a=getMRV();
	double sigma=getsigma();
	int size=Ci.size();
	//Calculate of A(T,ti) and B(T,ti)
	std::vector<double> A; A.resize(size); 
	std::vector<double> B; B.resize(size);

	for(i=0;i<size;i++){
		B[i]=locfunction_B(a,0.,Cr[i]);
		A[i]=Price[i]/P_T * exp(B[i]*fwd_M_Expiry-sigma*sigma*locfunction_B(2*a,0.,T)*B[i]*B[i]/2 );
		//cout << "iter ="<< i <<  ",  A[i]="  <<A[i]<<"  ,B[i]= "<<B[i] << "   ,Ci[i]= "<<Ci[i]<<endl;
	}

	//calculate of r*
	double error;
	loc_Jamshidian  functor(A,B,Ci);
	_1DRootFinder<loc_Jamshidian> aRootFinder;
	double r = aRootFinder.solve(functor, error, EPSILON, 1.);

	//calculate of Ki
	std::vector<double> Ki; Ki.resize(size); 
	for(i=0;i<size;i++)
	{ Ki[i]=A[i]*exp(-B[i]*r); }

	//Vector of ZBP(t,T,ti,Ki)
	std::vector<double> ZBP; ZBP.resize(size); 
	for(i=0;i<size;i++)
	{
		double P_S=Price[i];
		double fwd=P_S/P_T;	
		double S;
		S = Cr[i];
		double Vol=sigma*sqrt(locfunction_B(2*a,0.,T)) * locfunction_B(a,0.,S);
		double hi = log(fwd/Ki[i]) /Vol + Vol / 2 ;
		if (Ki[i]>=fwd) {
			ZBP[i]= P_S * Analytics::cnd(hi) - P_T * Analytics::cnd(hi - Vol) * Ki[i];
			ZBP[i]= ZBP[i]-P_S+Ki[i]*P_T;
		}
		else {  ZBP[i]=-P_S * Analytics::cnd(-hi) + P_T* Analytics::cnd(- hi + Vol) * Ki[i];  } 
	}

	// result
	double result=0.;
	for(i=0;i<size;i++)
	{result += Ci[i]*ZBP[i];}
	result=result*Nominal;
	return result;
}
/////////////////////////////////////////////////////////
//////////////////////////HW1FSwaptionPricer::implied_vol
/////////////////////////////////////////////////////////
double value(double fwd_swap_rate, double strike, double sigma, double ZCB_Expiry, double PV01, double expiry, string SwaptionType) 
{
	string opttype = ( SwaptionType == "PAY" ? "CALL" : "PUT" );
	double value = Analytics::optionPriceBlack(fwd_swap_rate, strike, sigma, expiry, 1.0, opttype); 

	value *= PV01;
	value *= ZCB_Expiry;
	return value;

}
class locPriceFunction_
{
	public:
//protected:
	double myFwd;
	double myK;
	double myPrice;
	double myMat;
	double myDF;
	std::string& myOptType;
	double myPVO1;

//public:
	locPriceFunction_(const double& fwd, const double& K, const double& price,const double& mat, const double& df, std::string& opt, double PVO1) 
		: myFwd(fwd), myK(K), myPrice(price), myMat(mat), myDF(df), myOptType(opt), myPVO1(PVO1) {}
	
	locPriceFunction_ & operator=( const locPriceFunction_ & ) {}

	double operator()(const double& x) const;
	double derivate(const double& s) const;
};
double locPriceFunction_::operator()(const double& x) const
{
	double price = Analytics::optionPriceBlack(myFwd, myK, x, myMat, 1., myOptType);
	price*=myPVO1;
	price*=myDF;
	return price - myPrice;
}
double locPriceFunction_::derivate(const double& s) const
{
	double valueup = (*this)(s+EPSILON);
	double valuedo = (*this)(s-EPSILON);
	return  (valueup - valuedo) / 2. / EPSILON ;
}
double HW1FSwaptionPricer::implied_vol( const double& fwd, const double& K,const double& price, const double& mat, const double& df,std::string& opt,const double PVO1)
{
	locPriceFunction_ f(fwd, K, price, mat, df, opt,PVO1);
	_1DRootFinder<locPriceFunction_> aRootFinder;
	double guess = 0.2;
	double lowBound = 0.01;
	double highBound = 1.0;
	double result = aRootFinder.solve(f, guess, lowBound, highBound);
	return result;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////HW1FCalibrator:functorHW1FMimnimisator//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HW1FCalibrator::functorHW1FMimnimisator::derivative(std::vector<double>& x, std::vector<double>& deriv)
{
	deriv.clear();
	deriv.resize(x.size(),0.); 
	double DEL,TEMP,f1;
	std::vector<double> xx(x);
	//double funcAtX = (*this)(x); 
	for(size_t i=0;i<x.size();i++)
	{
		DEL = MAX(fabs(x[i])*EPSILON,EPSILON);
		TEMP = x[i]; 
		xx[i]= x[i] + 1.*DEL; 
		f1 = (*this)(xx);
		xx[i]= TEMP - 1.*DEL;
		deriv[i]=( f1- (*this)(xx))/(2*DEL);
		xx[i]=TEMP;
	}
}
double HW1FCalibrator::functorHW1FMimnimisator::operator()(std::vector<double>& modelparameters)
{
	// Set the parameters in the model
	myModel->setParameters(modelparameters[0],modelparameters[1]);
	double result = 0.;
	double vol_loc=0.;
	for(unsigned int i = 0 ; i < myData.size() ; ++i) {
		double price =myModel->swaption( myData[i].myT, myData[i].myP_T, myData[i].myFwd_M_Expiry, 
		myData[i].myCi,myData[i].myCr, myData[i].myDatePayement_Price,1./*, myData[i].mySwap_Rate*/);

		if (myModel->calibration_Type == "volatility") 
		{
			vol_loc=myModel->implied_vol(myData[i].mySwap_Rate/myData[i].myStrikeWeight, 
						myData[i].mySwap_Rate,price, myData[i].myExpiry, myData[i].myP_T, myData[i].mySwaptionType,myData[i].myPVO1);
								
			result += myData[i].myWeight*myData[i].myWeight*(vol_loc - myData[i].myMktVol)*(vol_loc -myData[i].myMktVol);
		}
		else if (myModel->calibration_Type == "price") 
			result += myData[i].myWeight*myData[i].myWeight*(price - myData[i].myMktPrice)*(price - myData[i].myMktPrice);
		else if (myModel->calibration_Type == "relative price") 
			result += myData[i].myWeight*myData[i].myWeight*(price - myData[i].myMktPrice)*(price - myData[i].myMktPrice)/myData[i].myMktPrice;
		else{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error_Type value does not exist");
			throw new GMXBException(message);
		}
	}
	return sqrt(result);
} 
void HW1FCalibrator::functorHW1FMimnimisator::operator() (double& x,         //!< the value for which the function is evaluated
									std::vector<double>& a, //!< the parameters to value the function (the one that we are trying to find)
									double& y,               //!< result of the function
									std::vector<double>& dyda) //!< vector of derivates to each model parameter
{
	UNREFERENCED_PARAMETER(x);
	y = (*this).operator()(a);
	derivative(a, dyda);
}
void HW1FCalibrator::functorHW1FMimnimisator::operator() (double *x, double *fev)
{
	myModel->setMappedParameters(x);
	double vol_loc=0.;
	//cout << "alpha=" << myModel->getMRV()<<"        , sigma=" << myModel->getsigma() << endl;

	for(unsigned int i=0; i< myData.size(); i++){
		double price =myModel->swaption( myData[i].myT, myData[i].myP_T, myData[i].myFwd_M_Expiry, 
		myData[i].myCi,myData[i].myCr, myData[i].myDatePayement_Price,1./*, myData[i].mySwap_Rate*/);
		if (myModel->calibration_Type == "volatility") 
		{
			vol_loc=myModel->implied_vol(myData[i].mySwap_Rate/myData[i].myStrikeWeight, 
						myData[i].mySwap_Rate,price, myData[i].myExpiry, myData[i].myP_T, myData[i].mySwaptionType,myData[i].myPVO1);
			fev[i] =myData[i].myWeight*(vol_loc - myData[i].myMktVol); //TO DO price should be implied volatility
		}
		else if (myModel->calibration_Type == "price") 
		{
			//cout <<i << "- myTenor=" << myData[i].myTenor<<  "  , myExpiry=" << myData[i].myExpiry <<"   ,price=" << price <<"  ,market price=" <<  myData[i].myMktPrice <<endl;

			fev[i] = myData[i].myWeight*(price - myData[i].myMktPrice);

		}
		else if (myModel->calibration_Type == "relative price") 
			fev[i] = myData[i].myWeight*(price - myData[i].myMktPrice)/myData[i].myMktPrice;
		else{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error_Type value does not exist");
			throw new GMXBException(message);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////HW1FCalibrator//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HW1FCalibrator::initMktData(const std::vector<double>& expiries,const std::vector<double>& tenors,
					 const std::vector<double>& mktvol,const std::vector<double>& mWeights, double strike_weight, string SwaptionType)
{

	int i=0;
	// Create myMktData
	int size= expiries.size();
	myMktData.resize(size);
	for(i = 0 ; i < size ; ++i)
	{
		myMktData[i].myExpiry=expiries[i];
		myMktData[i].myTenor=tenors[i];
		myMktData[i].myMktVol=mktvol[i];
		myMktData[i].myWeight=mWeights[i];
	}
			
	for(i = 0 ; i < size ; ++i)
	{
		myMktData[i].mySwaptionType=( SwaptionType == "PAY" ? "CALL" : "PUT" );
		myMktData[i].myStrikeWeight=strike_weight;
		
		myMktData[i].mySwap_Rate=myYield_Curve->GetSwapRate(myMktData[i].myExpiry, myMktData[i].myExpiry + myMktData[i].myTenor );
		double ZCB=myYield_Curve->GetDiscountFactor(myMktData[i].myExpiry/**myYield_Curve->Delta_t*/);
		double PV01=myYield_Curve->GetPV01(myMktData[i].myExpiry, myMktData[i].myExpiry + myMktData[i].myTenor );
		myMktData[i].myMktPrice =value(myMktData[i].mySwap_Rate, myMktData[i].mySwap_Rate*strike_weight,myMktData[i].myMktVol,ZCB, PV01,myMktData[i].myExpiry,SwaptionType );
		myMktData[i].mySwap_Rate=myMktData[i].mySwap_Rate*strike_weight;
		myMktData[i].myP_T=ZCB;
		myMktData[i].myT=myMktData[i].myExpiry;
		myMktData[i].myFwd_M_Expiry=myYield_Curve->GetFwd(myMktData[i].myExpiry/**myYield_Curve->Delta_t*/);
		int size=0;
		int indicator=0;
		while(indicator==0){
			if(  (size+1.)/myYield_Curve->Coupon_Period <= myMktData[i].myTenor  ) size+=1;
			else indicator=1;
		}
		myMktData[i].myCi.resize(size);
		myMktData[i].myDatePayement_Price.resize(size);
		myMktData[i].myCr.resize(size);
		for (int j=0;j<size; j++)
		{
			myMktData[i].myCr[j]=(j+1.)/myYield_Curve->Coupon_Period;
			myMktData[i].myCi[j]=myMktData[i].mySwap_Rate/myYield_Curve->Coupon_Period;
			myMktData[i].myDatePayement_Price[j]=myYield_Curve->GetDiscountFactor(myMktData[i].myExpiry/**myYield_Curve->Delta_t*/ + (j+1)*1./myYield_Curve->Coupon_Period);
		}
		myMktData[i].myCi[size-1]+=1.;
		myMktData[i].myPVO1=PV01;
	}
}
//////////////////////////////////////////////////////////
HW1FCalibrator::HW1FCalibrator(InterfaceESG0* &iEsg,  const YC &mYieldCurve)
{
	 myYield_Curve=new YC(mYieldCurve);
	initMktData(iEsg->Calc_Swaption_maturity, iEsg->Calc_Swaption_tenor, iEsg->Calc_Swaption_vol,iEsg->Calc_Swaption_Weight,iEsg->Strike,iEsg->Swaption_type);
	myCalibrate_Y_N=iEsg->Calibrate_Y_N;
	myDebug_info_Calib=iEsg->Debug_info_Calib;
	myDebug_address_Calib=iEsg->Debug_address_Calib;
}
////////////////////////////////////////////////////////
void HW1FCalibrator::calibrate(HW1FSwaptionPricer* &model,double& error, MinimisationMethod MethodMin)
{

	functorHW1FMimnimisator functor(model, myMktData);

	Minimisation<functorHW1FMimnimisator> minimisator(model->number_of_parameters_to_calibrate,myMktData.size(),MethodMin,1.E-10);//(functor, DFPMIN or LEVENBERGMAQUARD or OTHER);
	std::vector<double> guess;
	guess.resize(4);
	guess[0]=model->parameters_output_mapped[0];
	guess[1]=model->parameters_output_mapped[1];
	guess[2]=0.;
	guess[3]=0.;

	if(myCalibrate_Y_N==0)
		error=functor(guess);
	else if(MethodMin==DFPMIN)
	{
		error = minimisator.solve(functor,guess);
		guess[0]=model->parameters_output_mapped[0];
		guess[1]=model->parameters_output_mapped[1];
		error=functor(guess);

	}
	else if(MethodMin==LM_NEW)
	{
		error = minimisator.solve(functor,guess);
		guess[0]=model->parameters_output_mapped[0];
		guess[1]=model->parameters_output_mapped[1];
		error=functor(guess);
	}
	else{}

	if(myDebug_info_Calib)
		Write_Debug_File(myDebug_address_Calib,0,model,error);

}
void HW1FCalibrator::Write_Debug_File(string DbgFilePath, int CurrentScen,HW1FSwaptionPricer* &model, double error)
{
	UNREFERENCED_PARAMETER(CurrentScen);
	ofstream output;
	string ss;
	//char debug_file_name[1000];
	ss = DbgFilePath + "Output_Calibration_"  + to_string(CurrentScen)+ ".csv";
	output.open(ss.c_str());

	string separator=";";
	
	output<<"Alpha"<<separator<<setprecision(10)<<model->getMRV()<<endl;
	output<<"Sigma"<<separator<<setprecision(10)<<model->getsigma()<<endl;
	output<<"Error"<<separator<<setprecision(10)<<error<<endl;
	output<<endl<<endl<<endl;
	
	double tenor[8]={1.,2.,5.,7.,10.,15.,20.,30.};
	double maturity[11]={0.25,0.5,1.,2.,3.,4.,5.,10.,15.,20.,30.};
	
	int indic=0;
	output<<"INPUT WEIGHT MATRIX:"<<endl<<""<<separator;
	for (int i=0;i<8;i++){output<<tenor[i]<<separator;}
	output<<endl;
	for (int i=0;i<11;i++)
	{
		output<<maturity[i]<<separator;
		for (int j=0;j<8;j++)
		{
			for (unsigned int k=0;k <myMktData.size();k++)
			{
				if(tenor[j]==myMktData[k].myTenor && maturity[i]==myMktData[k].myExpiry)
				{
					output<<setprecision(10)<<myMktData[k].myWeight<<separator;
					indic=1;
				}
			}
			if(indic==1){indic=0;}
			else output<<setprecision(10)<<0.<<separator;
		}
		output<<endl;
	}
	output<<endl<<endl<<endl;


	indic=0;
	output<<"INPUT VOL MATRIX:"<<endl<<""<<separator;
	for (int i=0;i<8;i++){output<<tenor[i]<<separator;}
	output<<endl;
	for (int i=0;i<11;i++)
	{
		output<<maturity[i]<<separator;
		for (int j=0;j<8;j++)
		{
			for (unsigned int k=0;k<myMktData.size();k++)
			{
				if(tenor[j]==myMktData[k].myTenor && maturity[i]==myMktData[k].myExpiry)
				{
					output<<setprecision(10)<<myMktData[k].myMktVol<<separator;
					indic=1;
				}
			}
			if(indic==1){indic=0;}
			else output<<setprecision(10)<<0.<<separator;
		}
		output<<endl;
	}
	output<<endl<<endl<<endl;
	
	indic=0;
	output<<"OUTPUT VOL MATRIX:"<<endl<<""<<separator;
	for (int i=0;i<8;i++){output<<tenor[i]<<separator;}
	output<<endl;
	for (int i=0;i<11;i++)
	{
		output<<maturity[i]<<separator;
		for (int j=0;j<8;j++)
		{
			for (unsigned int k=0;k<myMktData.size();k++)
			{
				if(tenor[j]==myMktData[k].myTenor && maturity[i]==myMktData[k].myExpiry)
				{
					double price=model->swaption( myMktData[k].myT, myMktData[k].myP_T, myMktData[k].myFwd_M_Expiry, 
								myMktData[k].myCi,myMktData[k].myCr, myMktData[k].myDatePayement_Price,1./*, myData[i].mySwap_Rate*/);
					double vol_loc=model->implied_vol(myMktData[k].mySwap_Rate/myMktData[k].myStrikeWeight, 
									myMktData[k].mySwap_Rate,price, myMktData[k].myExpiry, myMktData[k].myP_T, myMktData[k].mySwaptionType,myMktData[k].myPVO1);

					output<<setprecision(10)<<vol_loc<<separator;
					indic=1;
				}
			}
			if(indic==1){indic=0;}
			else output<<setprecision(10)<<0.<<separator;
		}
		output<<endl;
	}
	output<<endl<<endl<<endl;


	indic=0;
	output<<"INPUT PRICES MATRIX:"<<endl<<""<<separator;
	for (int i=0;i<8;i++){output<<tenor[i]<<separator;}
	output<<endl;
	for (int i=0;i<11;i++)
	{
		output<<maturity[i]<<separator;
		for (int j=0;j<8;j++)
		{
			for (unsigned int k=0;k<myMktData.size();k++)
			{
				if(tenor[j]==myMktData[k].myTenor && maturity[i]==myMktData[k].myExpiry)
				{
					output<<setprecision(10)<<myMktData[k].myMktPrice<<separator;
					indic=1;
				}
			}
			if(indic==1){indic=0;}
			else output<<setprecision(10)<<0.<<separator;
		}
		output<<endl;
	}
	output<<endl<<endl<<endl;



	indic=0;
	output<<"OUTPUT PRICES MATRIX:"<<endl<<""<<separator;
	for (int i=0;i<8;i++){output<<tenor[i]<<separator;}
	output<<endl;
	for (int i=0;i<11;i++)
	{
		output<<maturity[i]<<separator;
		for (int j=0;j<8;j++)
		{
			for (unsigned int k=0;k<myMktData.size();k++)
			{
				if(tenor[j]==myMktData[k].myTenor && maturity[i]==myMktData[k].myExpiry)
				{
					double price=model->swaption( myMktData[k].myT, myMktData[k].myP_T, myMktData[k].myFwd_M_Expiry, 
								myMktData[k].myCi,myMktData[k].myCr, myMktData[k].myDatePayement_Price,1./*, myData[i].mySwap_Rate*/);
					output<<setprecision(10)<<price<<separator;
					indic=1;
				}
			}
			if(indic==1){indic=0;}
			else output<<setprecision(10)<<0.<<separator;
		}
		output<<endl;
	}
	output<<endl<<endl<<endl;


	
	output.close();
}
////////////////////////////////////////////////////////
//void HW1FCalibrator::Write_Debug_File(string DbgFilePath, int CurrentScen)
//{
//	UNREFERENCED_PARAMETER(CurrentScen);
//	ofstream output;
//	string ss;
//	//char debug_file_name[1000];
//	ss = DbgFilePath + "Output_Calibration_Class_Param" + ".csv";
//	output.open(ss.c_str());
//
//	output<<"myTenor"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myTenor<<";";
//	output<<endl;
//	output<<"myExpiry"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myExpiry<<";";
//	output<<endl;
//	output<<"myMktVol"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myMktVol<<";";
//	output<<endl;
//	output<<"myWeight"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myWeight<<";";
//	output<<endl;
//	output<<"myMktPrice"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myMktPrice<<";";
//	output<<endl;
//	output<<"mySwap_Rate"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].mySwap_Rate<<";";
//	output<<endl;
//	output<<"myT"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myT<<";";
//	output<<endl;
//	output<<"myP_T"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myP_T<<";";
//	output<<endl;
//	output<<"myFwd_M_Expiry"<<";";
//	for(unsigned int i = 0 ; i < myMktData.size() ; ++i)
//		output<<setprecision(10)<<myMktData[i].myFwd_M_Expiry<<";";
//	output<<endl;
//
//	for(unsigned int j = 0 ; j <22 ; ++j)
//	{
//		output<<"myCi["<<j<<"]"<<";";
//		for(unsigned int i = 0 ; i < myMktData.size() ; ++i){
//			if(j < myMktData[i].myCi.size())
//				output<<setprecision(10)<<myMktData[i].myCi[j]<<";";
//			else
//				output<<setprecision(10)<<0.<<";";
//		}
//		output<<endl;
//	}
//
//	for(unsigned int j = 0 ; j <22 ; ++j)
//	{
//		output<<"myCr["<<j<<"]"<<";";
//		for(unsigned int i = 0 ; i < myMktData.size() ; ++i){
//			if(j < myMktData[i].myCr.size())
//				output<<setprecision(10)<<myMktData[i].myCr[j]<<";";
//			else
//				output<<setprecision(10)<<0.<<";";
//		}
//		output<<endl;
//	}
//	
//	for(unsigned int j = 0 ; j <22 ; ++j)
//	{
//		output<<"myDatePayement_Price["<<j<<"]"<<";";
//		for(unsigned int i = 0 ; i < myMktData.size() ; ++i){
//			if(j < myMktData[i].myDatePayement_Price.size())
//				output<<setprecision(10)<<myMktData[i].myDatePayement_Price[j]<<";";
//			else
//				output<<setprecision(10)<<0.<<";";
//		}
//		output<<endl;
//	}
//
//		
//		output.close();
//
//}	