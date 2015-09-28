#include "Assumptions.h"
#include "Underlying.h"
#include "Nbr_Futures.h"
#include "Tte_Futures.h"
#include "New_Futures_Notional.h"

New_Futures_Notional::New_Futures_Notional(Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Futures* nbr_futures,
		Tte_Futures* tte_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.underlying=underlying;
	this->objects_needed.nbr_futures=nbr_futures;
	this->objects_needed.tte_futures=tte_futures;
	this->init_tab();

}
void New_Futures_Notional::init_tab()
{
	this->fut_not=new double[this->objects_needed.assumptions->model_period];
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->fut_not[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(this->objects_needed.tte_futures->isMaturityReached(i,j))
				this->tab[i][j]=0;
			else
			{
				if(i==0)
					this->tab[i][j]=
					this->objects_needed.nbr_futures->tab[i][j]
					*this->objects_needed.underlying->tab[i][j];
				else this->tab[i][j]=
					(this->objects_needed.nbr_futures->tab[i][j]-this->objects_needed.nbr_futures->tab[i-1][j])
					*this->objects_needed.underlying->tab[i][j];
			}
			fut_not[i]+=this->tab[i][j];
		}

	}
}
New_Futures_Notional::~New_Futures_Notional(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
	delete [] this->fut_not;
}
