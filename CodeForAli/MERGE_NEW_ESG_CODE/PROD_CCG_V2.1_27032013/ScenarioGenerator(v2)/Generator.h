#ifndef GENERATOR_H
#define GENERATOR_H

#include "Variance Reduction.h"
#include "tools.h"
#include <iostream>
#include <fstream>
using namespace std;

#define PI acos(-1.0)

class Generator{
public:

static double Polar();

static void Gaussian(Basic_Information &BI,double*** output,unsigned long * seed, int begin_index, int number_indexes, int martingale);

static void Gaussian_reset(Basic_Information &BI,double*** output, int number_indexes,int new_index);

static int Indice_Generator(int position_index, Scenario *s,Variance_Reduction t,Basic_Information BI, unsigned long * seed);

//combinaison lineaire
static void combination(int position_index,int position_step,Basic_Information &BI,Scenario *s);

//static void calcul_scenario(Scenario *s,Basic_Information BI, bool fast);

static void calcul_scenario(Scenario *s,Basic_Information BI, bool closed_formula);
};

#endif