#include "Existing_Gamma.h"
#include "Nbr_Puts.h"
#include "Gamma_Puts.h"
Existing_Gamma::Existing_Gamma(Assumptions* assumptions, Gamma_Puts* gamma_puts)
{
	this->objects_needed.gamma_puts=gamma_puts;
	this->objects_needed.assumptions=assumptions;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->existing_ov=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		
	}
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->existing_ov[i]=0;
		for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{
			this->tab[i][j]=0;
		}
	}
}

void Existing_Gamma::init_tab(int i, int j)
{
	if(i+1<this->objects_needed.assumptions->model_period)
	this->tab[i][j]=this->objects_needed.nbr_puts->tab[i][j]
		*this->objects_needed.gamma_puts->tab[i+1][j];
		this->existing_ov[i]+=this->tab[i][j];
}

Existing_Gamma::~Existing_Gamma(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete this->tab[i];
	}
	delete this->tab;
	delete this->existing_ov;
}