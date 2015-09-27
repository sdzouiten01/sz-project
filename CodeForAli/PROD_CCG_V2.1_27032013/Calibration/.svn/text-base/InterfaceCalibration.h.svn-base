#ifndef _INTERFACE_CALIBRATION_H_
#define _INTERFACE_CALIBRATION_H_
//
#include <string>
#include "GMXB_Assumption_Table.h"
#include "GMXBParameters.h"

using namespace std;

//#ifdef _DLL_FILE_
//class __declspec (dllexport) InterfaceCalibration
//#else
//class __declspec (dllimport) InterfaceCalibration
//#endif
class InterfaceCalibration
{
public:
	InterfaceCalibration( const string& config );
	~InterfaceCalibration( void );
	void ShockSwap( double startDate, double keyDate, double endDate, double shockSize, string& shockType);
	void Calibrate( double& a, double& sigma, double* zc, int scenario_pass_number, int bucket_id, int w_id);
	void UpdateSwapTable(double** scen, int t);
	void InterpolationSwapTable();
	void Interpolate_Swap();

public:
	    string config_file;

		double *param_initial;
		double *param_min;
		double *param_max;
		
		GMXB_Assumption_Table * swaption_vol_table;
		GMXB_Assumption_Table * weight_table;
		GMXB_Assumption_Table *	swap_rate_table;
		GMXB_Assumption_Table * price;
		GMXBParameters * assumptions;
		int nb_tenor;
		int nb_maturity;
		string calib;
		string error;
		bool bootstrapping_security;
		bool new_interpolation;
		int interpolation_type;
		int extrapolation_type_start;
		int extrapolation_type_end;
		int nb_date;
		int nb_date_initial; // HE_YANN
		int swaption_type;
		double coupon_period;
		double K;
		double *swap_tenor;
		double *swap_maturity;
		double **swaption_vol;
		double **weight;
		double *dateCurve;
		double *zeroCurve;   // swap_rate, which will be used for generate scenario, which will be shocked ...
		double *RateCurve;   // swap_rate of the initial value, never will be changed after construction.
		double *zeroCurve_initial; // HE_YANN
		double *RateCurve_initial; //HE_YANN
		double *dateCurve_initial; //HE_YANN
		double *SwapRateTable;
		string input_swap_rate; // Name of the swap rate curve
		int last_model_date;
		int nb_swap_date;
		double* date_swap_curve;
		double* swap_curve;

		int if_read_swap_rate_from_excel;

		bool debug_info;
		string debug_address;
		int HE_mode_calibration;
		int delta_t;
};
#endif
