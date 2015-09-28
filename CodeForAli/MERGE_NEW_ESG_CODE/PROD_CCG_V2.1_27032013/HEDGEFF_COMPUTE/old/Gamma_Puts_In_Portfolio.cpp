#include "Gamma_Puts_In_Portfolio.h"

Gamma_Puts_In_Portfolio::Gamma_Puts_In_Portfolio(Assumptions* assumptions,Gamma_Puts* gamma_puts)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.gamma_puts=gamma_puts;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
}
void Gamma_Puts_In_Portfolio::init_tab(int i, int j)
{
	
	if(i==0) this->tab[i][j]=0;
	else if(j==0)
	{
		//this->objects_needed.gamma_puts->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.gamma_puts->tab[i][j]*
			this->objects_needed.nbr_puts->tab[i-1][j];
	}
	 
	else
	{
		//this->objects_needed.gamma_puts->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.gamma_puts->tab[i][j]*
			this->objects_needed.nbr_puts->tab[i-1][j]+
			this->tab[i][j-1];
	}
}
Gamma_Puts_In_Portfolio::~Gamma_Puts_In_Portfolio(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
