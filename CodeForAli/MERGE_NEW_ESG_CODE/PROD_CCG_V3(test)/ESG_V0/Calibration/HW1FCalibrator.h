#ifndef HW1FCALIBRATOR_H
#define HW1FCALIBRATOR_H

#include "../Instruments/YieldCurve.h"
#include "../Instruments/Analytics.h"
#include <vector>
#include "../Instruments/1DRootFinder.h"
#include "../Instruments/Minimisation.h"
#include <math.h>
#define  UNREFERENCED_PARAMETER(P) (P)
//#define EPSILON 1.E-6


class HW1FSwaptionPricer
{
public:
	int number_of_parameters_to_calibrate;
	string calibration_on;
	string calibration_Type;
	double *parameters_input;
	double *parameters_min_input;
	double *parameters_max_input;
	double *parameters_output;
	double *parameters_output_mapped;

	HW1FSwaptionPricer();
	HW1FSwaptionPricer(string calib_type,string calib_param,double* &param_input,double* &param_min_input,double* &param_max_input);
	~HW1FSwaptionPricer();

	// function to map between physical and calibration parameters
	void to_mappped_parameters();
	// function to map between physical and calibration parameters
	void to_unmappped_parameters();

	void setParameters( double& MRV, double& Volatility);
	void setMappedParameters(double *parameters_mapped);

	double getMRV();
	double getsigma() ;

	double swaption( const double& T, const double& P_T,const double& fwd_M_Expiry, const std::vector<double>& Ci, const std::vector<double>& Cr,
		const std::vector<double>& Price, const double& Nominal/*,const double& strike*/) ;

	double implied_vol( const double& fwd, const double& K,const double& price, const double& mat, const double& df,std::string& opt,const double PVO1) ;
};

class HW1FCalibrator
{
	public:
		class locSwaptionDescription
		{
				public:
					// inputs
					double myTenor;
					double myExpiry;
					double myMktVol;
					double myWeight;
					string mySwaptionType;
					// Intermediary results
					double myMktPrice;
					std::vector<double> myCi;
					std::vector<double> myCr;
					double  mySwap_Rate;
					std::vector<double> myDatePayement_Price;
					double myT;
					double myP_T;
					double myFwd_M_Expiry;
					double  myStrikeWeight;
					double myPVO1;

					//constructor
					locSwaptionDescription(){}
					locSwaptionDescription(double tenor, double expiry, double vol, double weight,double price,std::vector<double> ci, std::vector<double> cr,
						double swaprate, std::vector<double> datepay,double t,double p_t,double fwd_m_expiry,string SwaptionType,double StrikeWeight, double PVO1)
						:myTenor(tenor),myExpiry(expiry),myMktVol(vol),myWeight(weight),myMktPrice(price),myCi(ci),myCr(cr),mySwap_Rate(swaprate),
						myDatePayement_Price(datepay),myT(t),myP_T(p_t),myFwd_M_Expiry(fwd_m_expiry),mySwaptionType(SwaptionType),myStrikeWeight(StrikeWeight),myPVO1(PVO1) {}
		
		};

		class functorHW1FMimnimisator
		{
				public:
					HW1FSwaptionPricer *myModel;
					std::vector<locSwaptionDescription> myData;
					functorHW1FMimnimisator(){}
					functorHW1FMimnimisator(HW1FSwaptionPricer* &model, std::vector<locSwaptionDescription> &data) :myModel(model),myData(data) {}
					~functorHW1FMimnimisator(){}
					
					void derivative(std::vector<double>& x, std::vector<double>& deriv);
					double operator()(std::vector<double>& modelparameters);

					void operator() (double& x,         //!< the value for which the function is evaluated
									std::vector<double>& a, //!< the parameters to value the function (the one that we are trying to find)
									double& y,               //!< result of the function
									std::vector<double>& dyda);
					void operator() (double *x, double *fev);
			
		};


		// to be created in the constructor
		std::vector<locSwaptionDescription> myMktData;

		YC *myYield_Curve;

		int myCalibrate_Y_N;
		bool myDebug_info_Calib;
		string myDebug_address_Calib;


		void initMktData(const std::vector<double>& expiries,const std::vector<double>& tenors,
					 const std::vector<double>& mktvol,const std::vector<double>& mWeights, double strike_weight, string SwaptionType);
		
		HW1FCalibrator(InterfaceESG0* &iEsg,  const YC &mYieldCurve);
		void calibrate(HW1FSwaptionPricer* &model,double& error, MinimisationMethod MethodMin);

		~HW1FCalibrator(){ delete myYield_Curve;}
		void Write_Debug_File(string DbgFilePath, int CurrentScen,HW1FSwaptionPricer* &model,double error);
};


#endif