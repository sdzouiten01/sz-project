#include "Rho_Swaps.h"
#include "Rho_Asset.h"
#include "Assumptions.h"
Rho_Asset::Rho_Asset(int swap_no,Rho_Swaps* rho_swaps)
{
		this->objects_needed.rho_swaps=rho_swaps;
		this->swap_no=swap_no;
		this->init_tab();
}

void  Rho_Asset::init_tab()
{
	this->tab=new double[this->objects_needed.rho_swaps->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.rho_swaps->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=this->objects_needed.rho_swaps->tab[i][i];
	}				
}

Rho_Asset::~Rho_Asset(void)
{
	delete tab;
}

