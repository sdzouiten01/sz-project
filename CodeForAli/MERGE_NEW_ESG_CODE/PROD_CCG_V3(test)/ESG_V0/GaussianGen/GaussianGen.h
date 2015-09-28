#ifndef GAUSSIANGEN_H
#define GAUSSIANGEN_H

#include "../UniformGen/Mersenne_Twister.h"
#include "VarianceReduction.h"
//#include <iostream>
//#include <fstream>
#include <string>

#include "GaussianGenModelParameters.h"
using namespace std;
#define PI acos(-1.0)
#define MAXGENTRY 1000000

class GaussianGen{
public:
	
	UniformGen *Uniform_Generator;
	string Uniform_Method;
	string Gaussian_Method;

	int Martingale;
	int Antithetic;
	int Nb_Step;
	int Number_Scen_Antithetic;
	int NB_equity;
	int Number_Scen;
	double *** Gaussian_vector;
	unsigned long ** ListSeed;
	double ** Inverse_Cholesky_Correlation;

	GaussianGen(){};
	//void Init_GaussianGen(string uniform_Method,string gaussian_Method,int martingale,int antithetic,int nb_Step,int nB_equity,int number_Scen,unsigned long ** listSeed/*,double ** inverse_Cholesky_Correlation*/);
	GaussianGen(GaussianGenModelParameters * &Input_Gen_Model_Parameters);
	
	//int read_seed(ifstream &file2,unsigned long * seed);
	~GaussianGen();
	double Polar();
	void GenAll(VarianceReduction* &t);
	void GenGaussian_IR();
	void GenGaussian_Equity(int index_position);
	void combination(int index_position);
	void GenGaussian_mod0();
	void GenGaussian_mod1(VarianceReduction* &t);
	void GenGaussian_mod2();
	void GenGaussian_mod3(VarianceReduction* &t);
	void Init_intermediate_tab(int index_position,double ** tmp);
	void combination(int index_position, double ** tmp);
	void Write_Debug_File(string DbgFilePath, int CurrentScen);
	//void Update_inverse_Cholesky_Correlation(double ** vet);
};

#endif