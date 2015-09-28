#include "Sv_Base.h"
#include "Nbr_Swaps.h"
#include "Assumptions.h"
#include "Trading_Swaps.h"

Trading_Swaps::Trading_Swaps(Assumptions* assumptions,
		Sv_Base* sv_base,
		Nbr_Swaps* nbr_swaps)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.sv_base=sv_base;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->init_tab();
}
void Trading_Swaps::init_tab()
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
				this->objects_needed.nbr_swaps->tab[i][j]
				*this->objects_needed.sv_base->tab[i][j];
			else this->tab[i][j]=
				(this->objects_needed.nbr_swaps->tab[i][j]-this->objects_needed.nbr_swaps->tab[i-1][j])
				*this->objects_needed.sv_base->tab[i][j];
			this->trading[i]+=this->tab[i][j];
		}
		

	}
}

Trading_Swaps::~Trading_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
	delete[] trading;
}
