#include "Option_Notional.h"
#include "Underlying.h"
#include "Nbr_Puts.h"
#include "Assumptions.h"

Option_Notional::Option_Notional(Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Puts* nbr_puts)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.underlying=underlying;
	this->objects_needed.nbr_puts=nbr_puts;
	this->init_tab();
}
void Option_Notional::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->opt_not=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->opt_not[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			this->tab[i][j]=
				this->objects_needed.nbr_puts->tab[i][j]*
				this->objects_needed.underlying->tab[i][j];
			this->opt_not[i]+=this->tab[i][j];
		}

	}
}
Option_Notional::~Option_Notional(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->opt_not;
}