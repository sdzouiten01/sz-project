#include "Delta_Puts.h"
#include "Nbr_Puts.h"
#include "Assumptions.h"
#include "Nbr_Futures.h"
#include "Delta_Futures.h"
#include "Delta_In_Portfolio.h"
Delta_In_Portfolio::Delta_In_Portfolio(
		Assumptions* assumptions,
		Delta_Puts* delta_puts,
		Nbr_Puts* nbr_puts,
		Nbr_Futures* nbr_futures,
		Delta_Futures* delta_futures
		)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.delta_puts=delta_puts;
	this->objects_needed.nbr_puts=nbr_puts;
	this->objects_needed.nbr_futures=nbr_futures;
	this->objects_needed.delta_futures=delta_futures;
	this->init_tab();
}
void Delta_In_Portfolio::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->asset_delta=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_delta[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			this->tab[i][j]=
				this->objects_needed.nbr_puts->tab[i][j]*
				this->objects_needed.delta_puts->tab[i][j]+
				this->objects_needed.delta_futures->tab[i][j]*
				this->objects_needed.nbr_futures->tab[i][j];
			this->asset_delta[i]+=this->tab[i][j];
		}

	}
}
Delta_In_Portfolio::~Delta_In_Portfolio(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->asset_delta;
}