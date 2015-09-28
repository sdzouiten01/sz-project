#ifndef INTERFACE_AXA_GEN_H
#define INTERFACE_AXA_GEN_H

#ifdef AXAGEN_DLL
class __declspec (dllexport) InterfaceAxaGen
#else
class __declspec (dllimport) InterfaceAxaGen
#endif
{
public:
	// "table" is the name of the CONFIGRATION FILE
	InterfaceAxaGen( const char* table );

	~InterfaceAxaGen( void );

	// generate the seed;
	void genSeed( unsigned long* seed, const unsigned int nbSeed,
				  int startSeed, int serialNumber, const int stepLen=10 );

	// output results to a file designated in the CONFIGRATION FILE
	// using seeds in the CONFIGRATION FILE
	void Gen( void );

	// output results to a array
	// using seeds in the CONFIGRATION FILE
	void Gen( double*** output );

	// output results to a file designated in the CONFIGRATION FILE
	// using user input seeds 
	void Gen( unsigned long* seed );

	// output results to a array
	// using user input seeds 
	void Gen( double*** output, unsigned long* seed );

public:
	int num_equity;
	int num_bond;
	int num_rate;
	int num_initial;
	int num_vol;
	int *equity;
	double *bond;
	double *rate;
	double *initial_spot_rate;
	double **correlation;
	double **vol;
	double *error;
	double delta_t;
	int year;
	int scenario;
	int uniform;
	int gaussian;
	int antithetic;
	int short_rate;
	int equity_model;
	double a;
	double sigma;
	char* f_scenario;
	char* random;
	int g;
	bool test_lognormal_mean;
	bool test_beta;
	double test_variance;
	double test_covariance;
	bool test_martingale;
	unsigned long* seed;
};

#endif