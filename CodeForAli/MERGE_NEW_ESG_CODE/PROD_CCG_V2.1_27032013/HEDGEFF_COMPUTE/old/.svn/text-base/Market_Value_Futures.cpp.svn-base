#include "Assumptions.h"
#include "Fv_Base.h"
#include "Nbr_Futures.h"
#include "Market_Value_Futures.h"

Market_Value_Futures::Market_Value_Futures(Assumptions* assumptions,
		Fv_Base* fv_base,
		Nbr_Futures* nbr_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.fv_base=fv_base;
	this->objects_needed.nbr_futures=nbr_futures;
	this->init_tab();
}
void Market_Value_Futures::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->mkt_val=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->mkt_val[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			this->tab[i][j]=
				this->objects_needed.nbr_futures->tab[i][j]*
				this->objects_needed.fv_base->tab[i][j];
			this->mkt_val[i]+=this->tab[i][j];
		}
	}
}
Market_Value_Futures::~Market_Value_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
	delete[] mkt_val;
}
