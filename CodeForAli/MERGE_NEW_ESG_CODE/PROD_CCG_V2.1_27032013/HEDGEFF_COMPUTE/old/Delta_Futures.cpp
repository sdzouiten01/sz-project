#include "Delta_Futures.h"
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Und_Base.h"
#include "Underlying.h"
#include "Tte_Futures.h"
#include "Rf_Futures.h"
Delta_Futures::Delta_Futures(
		Assumptions* assumptions,
		Tte_Futures* tte_futures,
		Rf_Futures* rf_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rf_futures=rf_futures;
	this->objects_needed.tte_futures=tte_futures;
	this->init_tab();
}
void Delta_Futures::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(this->objects_needed.tte_futures->isMaturityReached(i,j)||
				i<j)
				this->tab[i][j]=0;
			else
			this->tab[i][j]=exp(this->objects_needed.rf_futures->tab[i][j]*this->objects_needed.tte_futures->tab[i][j]);
		}
	}
}
Delta_Futures::~Delta_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
