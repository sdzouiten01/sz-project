#include "Assumptions.h"
#include "Math_Tools.h"
#include "Underlying.h"
#include "Tte_Futures.h"
#include "Rf_Futures.h"
#include "Fv_Base.h"

Fv_Base::Fv_Base(Assumptions* assumptions,
		Underlying* underlying,
		Tte_Futures* tte_futures,
	
		Rf_Futures* rf_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rf_futures=rf_futures;
	this->objects_needed.underlying=underlying;
	this->objects_needed.tte_futures=tte_futures;
	this->init_tab();
}
void Fv_Base::init_tab()
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
			if(i>this->objects_needed.assumptions->length_of_futures_contract+j||
				this->objects_needed.underlying->tab[i][j]==0)
				this->tab[i][j]=0;
			else
				this->tab[i][j]=this->objects_needed.underlying->tab[i][j]*
									exp(this->objects_needed.rf_futures->tab[i][j]*
									this->objects_needed.tte_futures->tab[i][j]);
		}
	}
}
Fv_Base::~Fv_Base(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
}
