#include "Existing_Rho_Swaps.h"
#include "Nbr_Swaps.h"
#include "Rho_Swaps.h"

Existing_Rho_Swaps::Existing_Rho_Swaps(int swap_no,Assumptions* assumptions, Rho_Swaps* rho_swaps)
{
	this->objects_needed.rho_swaps=rho_swaps;
	this->objects_needed.assumptions=assumptions;
	this->swap_no=swap_no;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->existing_sv=new double[this->objects_needed.assumptions->model_period];
	
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		
	}
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->existing_sv[i]=0;
		for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{
			this->tab[i][j]=0;
		}
	}
}

void Existing_Rho_Swaps::init_tab(int i, int j)
{

	if(i+1<this->objects_needed.assumptions->model_period)		
			this->tab[i][j]=this->objects_needed.nbr_swaps->tab[i][j]
					*this->objects_needed.rho_swaps->tab[i+1][j];
	
	this->existing_sv[i]+=this->tab[i][j];
}
double Existing_Rho_Swaps::return_existing_sv(int i)
{
	if(i==-1) return 0;
	else return this->existing_sv[i];
}

Existing_Rho_Swaps::~Existing_Rho_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->existing_sv;
}