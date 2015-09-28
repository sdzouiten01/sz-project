#include "Rho_Swaps_In_Portfolio_2.h"

Rho_Swaps_In_Portfolio_2::Rho_Swaps_In_Portfolio_2(int swap_no,Assumptions* assumptions,Rho_Swaps* rho_swaps)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rho_swaps=rho_swaps;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		this->tab[i][0]=0;
	}
}
void Rho_Swaps_In_Portfolio_2::init_tab(int i, int j)
{
		if(j==0)
		this->tab[i][j]=this->objects_needed.rho_swaps->tab[i][j]*
			this->objects_needed.nbr_swaps->tab[i][j];
		else 
			this->tab[i][j]=this->objects_needed.rho_swaps->tab[i][j]*
			this->objects_needed.nbr_swaps->tab[i][j]+this->tab[i][j-1];
	
}
Rho_Swaps_In_Portfolio_2::~Rho_Swaps_In_Portfolio_2(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
