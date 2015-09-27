#include "Rho_Swaps_In_Portfolio.h"

Rho_Swaps_In_Portfolio::Rho_Swaps_In_Portfolio(int swap_no,Assumptions* assumptions,Rho_Swaps* rho_swaps)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rho_swaps=rho_swaps;
	this->swap_no=swap_no;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{

		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
}
void Rho_Swaps_In_Portfolio::init_tab(int i, int j)
{
	if(i==0) this->tab[i][j]=0;
	else if(j==0)
	{
		//this->objects_needed.rho_swaps->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.rho_swaps->tab[i][j]*
			this->objects_needed.nbr_swaps->tab[i-1][j];
	}
	 
	else
	{
		//this->objects_needed.rho_swaps->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.rho_swaps->tab[i][j]*
			this->objects_needed.nbr_swaps->tab[i-1][j]+
			this->tab[i][j-1];
	}
}
Rho_Swaps_In_Portfolio::~Rho_Swaps_In_Portfolio(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
