#include "Market_Value.h"
#include "Ov_Base.h"
#include "Nbr_Puts.h"
#include "Assumptions.h"

Market_Value::Market_Value(Assumptions* assumptions,
		Ov_Base* ov_base,
		Nbr_Puts* nbr_puts)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.ov_base=ov_base;
	this->objects_needed.nbr_puts=nbr_puts;
	this->init_tab();
}
void Market_Value::init_tab()
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
				this->objects_needed.nbr_puts->tab[i][j]*
				this->objects_needed.ov_base->tab[i][j];
			this->mkt_val[i]+=this->tab[i][j];
		}
	}
}
Market_Value::~Market_Value(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->mkt_val;
}
