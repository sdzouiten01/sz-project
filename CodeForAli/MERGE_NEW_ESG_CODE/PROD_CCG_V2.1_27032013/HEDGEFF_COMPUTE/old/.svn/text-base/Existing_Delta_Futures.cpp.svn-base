#include "Existing_Delta_Futures.h"
#include "Nbr_Futures.h"
#include "Delta_Futures.h"

Existing_Delta_Futures::Existing_Delta_Futures(Assumptions* assumptions, Delta_Futures* delta_futures)
{
	this->objects_needed.delta_futures=delta_futures;
	this->objects_needed.assumptions=assumptions;

	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->existing_fv=new double[this->objects_needed.assumptions->model_period];
	
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		
	}
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->existing_fv[i]=0;
		for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{
			this->tab[i][j]=0;
		}
	}
}

void Existing_Delta_Futures::init_tab(int i, int j)
{

	if(i+1<this->objects_needed.assumptions->model_period)		
			this->tab[i][j]=this->objects_needed.nbr_futures->tab[i][j]
					*this->objects_needed.delta_futures->tab[i+1][j];
	
	this->existing_fv[i]+=this->tab[i][j];
}
double Existing_Delta_Futures::return_existing_fv(int i)
{
	if(i==-1) return 0;
	else return this->existing_fv[i];
}

Existing_Delta_Futures::~Existing_Delta_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->existing_fv;
}