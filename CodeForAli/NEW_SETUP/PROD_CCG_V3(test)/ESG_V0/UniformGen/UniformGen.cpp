#include "UniformGen.h"
//#include <math.h>
//#include <string>
//#include "GMXBException.h"


void UniformGen::Inputseed(unsigned long * input){
		for (int i=0;i<SeedSize;i++)
			Seed[i]=input[i];
		mti = input[SeedSize];
	
}
void UniformGen::Outputseed(unsigned long * output){
		for (int i=0;i<SeedSize;i++)
			output[i]=Seed[i];
		output[SeedSize]=mti;
	
}
UniformGen::~UniformGen(){
	delete Seed;
}

UniformGen::UniformGen()
{
	SeedSize=1;
	Seed=new unsigned long [SeedSize];
	Seed[0]=0;
	mti=0;
}
UniformGen::UniformGen( unsigned int size)
{
	SeedSize=size;
	Seed=new unsigned long [size];
	for (unsigned int i=0; i<size;i++) Seed[i]=0;
	mti=0;

}
UniformGen::UniformGen(unsigned long * seed,int size,int m){
	SeedSize=size;
	Seed=new unsigned long [SeedSize];
	for (int i=0;i<SeedSize;i++)
			Seed[i]=seed[i];
	mti=m;
}

