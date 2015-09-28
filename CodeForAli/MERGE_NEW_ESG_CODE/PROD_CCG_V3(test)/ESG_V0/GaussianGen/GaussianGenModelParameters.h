#ifndef GAUSSIANGENMODELPARAMETERS_H
#define GAUSSIANGENMODELPARAMETERS_H

#include "../Calibration/HW1FCalibrator.h"
#include <string>
#include "../Instruments/InterfaceESG0.h"
#include "../UniformGen/Mersenne_Twister.h"
using namespace std;


class GaussianGenModelParameters{
public:

	string Uniform_Method;
	string Gaussian_Method;
	int Martingale;
	int Antithetic;
	int Nb_Step;
	int Number_Scen_Antithetic;
	int NB_equity;
	int Nb_rate;
	int Nb_bond;
	int Number_Scen;
	unsigned long ** ListSeed;
	double ** Inverse_Cholesky_Correlation;
	
	int myDebug_info_ScenGen;
	string myDebug_address_ESG;

	GaussianGenModelParameters(){};
	~GaussianGenModelParameters();
	int read_seed(ifstream &file2,unsigned long * seed);
	void Cholesky(double** input, int dimension);
	GaussianGenModelParameters(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model );

};

#endif