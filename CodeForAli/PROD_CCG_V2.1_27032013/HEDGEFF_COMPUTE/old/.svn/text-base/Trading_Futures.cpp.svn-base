#include "Fv_Base.h"
#include "Nbr_Futures.h"
#include "Assumptions.h"
#include "Trading_Futures.h"

Trading_Futures::Trading_Futures(Assumptions* assumptions,
		Fv_Base* fv_base,
		Nbr_Futures* nbr_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.fv_base=fv_base;
	this->objects_needed.nbr_futures=nbr_futures;
	this->init_tab();
}
void Trading_Futures::init_tab()
{
	this->trading=new double[this->objects_needed.assumptions->model_period];
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->trading[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(i==0)
				this->tab[i][j]=
				this->objects_needed.nbr_futures->tab[i][j]
				*this->objects_needed.fv_base->tab[i][j];
			else this->tab[i][j]=
				(this->objects_needed.nbr_futures->tab[i][j]-this->objects_needed.nbr_futures->tab[i-1][j])
				*this->objects_needed.fv_base->tab[i][j];
			this->trading[i]+=this->tab[i][j];
		}
		

	}
}

Trading_Futures::~Trading_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
	delete[] trading;
}
