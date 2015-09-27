#include "Delta_Futures_In_Portfolio.h"

Delta_Futures_In_Portfolio::Delta_Futures_In_Portfolio(Assumptions* assumptions,Delta_Futures* delta_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.delta_futures=delta_futures;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{

		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
}
void Delta_Futures_In_Portfolio::init_tab(int i, int j)
{
	if(i==0) this->tab[i][j]=0;
	else if(j==0)
	{
		//this->objects_needed.delta_futures->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.delta_futures->tab[i][j]*
			this->objects_needed.nbr_futures->tab[i-1][j];
	}
	 
	else
	{
		//this->objects_needed.delta_futures->init_tab(i,j);
		this->tab[i][j]=this->objects_needed.delta_futures->tab[i][j]*
			this->objects_needed.nbr_futures->tab[i-1][j]+
			this->tab[i][j-1];
	}
}
Delta_Futures_In_Portfolio::~Delta_Futures_In_Portfolio(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
