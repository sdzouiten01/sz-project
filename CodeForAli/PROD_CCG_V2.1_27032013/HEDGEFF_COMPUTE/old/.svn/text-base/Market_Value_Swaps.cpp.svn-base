#include "Assumptions.h"
#include "Sv_Base.h"
#include "Nbr_Swaps.h"
#include "Market_Value_Swaps.h"

Market_Value_Swaps::Market_Value_Swaps(Assumptions* assumptions,
		Sv_Base* sv_base,
		Nbr_Swaps* nbr_swaps)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.sv_base=sv_base;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->init_tab();
}
void Market_Value_Swaps::init_tab()
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
				this->objects_needed.nbr_swaps->tab[i][j]*
				this->objects_needed.sv_base->tab[i][j];
			this->mkt_val[i]+=this->tab[i][j];
		}
	}
}
Market_Value_Swaps::~Market_Value_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
	delete[] mkt_val;
}
