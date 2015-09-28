#include "Assumptions.h"
#include "Underlying.h"
#include "Nbr_Swaps.h"
#include "Tte_Swaps.h"
#include "New_Swaps_Notional.h"

New_Swaps_Notional::New_Swaps_Notional(Assumptions* assumptions,
		Underlying* underlying,
		Nbr_Swaps* nbr_swaps,
		Tte_Swaps* tte_swaps)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.underlying=underlying;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->objects_needed.tte_swaps=tte_swaps;
	this->init_tab();

}
void New_Swaps_Notional::init_tab()
{
	this->swap_not=new double[this->objects_needed.assumptions->model_period];
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->swap_not[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(this->objects_needed.tte_swaps->isMaturityReached(i,j))
				this->tab[i][j]=0;
			else
			{
				if(i==0)
					this->tab[i][j]=
					this->objects_needed.nbr_swaps->tab[i][j]
					*1;
				else this->tab[i][j]=
					(this->objects_needed.nbr_swaps->tab[i][j]-this->objects_needed.nbr_swaps->tab[i-1][j])
					*1;
			}
			swap_not[i]+=this->tab[i][j];
		}

	}
}
New_Swaps_Notional::~New_Swaps_Notional(void)
{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
	delete [] this->swap_not;
}
