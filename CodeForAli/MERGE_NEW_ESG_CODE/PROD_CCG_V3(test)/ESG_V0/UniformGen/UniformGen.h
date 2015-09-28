#ifndef UNIFORMGEN_H
#define UNIFORMGEN_H

//#include <iostream>
//#include <fstream>
//using namespace std;

class UniformGen{
public:

	unsigned long * Seed;
	int  SeedSize;
	int mti;

	UniformGen();
	UniformGen(unsigned int size);
	UniformGen(unsigned long * seed,int size,int m);
	virtual ~UniformGen();
	virtual void Inputseed(unsigned long * input);
	virtual void Outputseed(unsigned long * output);
	virtual void Initialisation_seed0(){};
	virtual double Genrand(){ return 0.;};
};

#endif