#include "Delta_Futures_In_Portfolio_2.h"

Delta_Futures_In_Portfolio_2::Delta_Futures_In_Portfolio_2(Assumptions* assumptions,Delta_Futures* delta_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.delta_futures=delta_futures;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		this->tab[i][0]=0;
	}
}
void Delta_Futures_In_Portfolio_2::init_tab(int i, int j)
{
		if(j==0)
		this->tab[i][j]=this->objects_needed.delta_futures->tab[i][j]*
			this->objects_needed.nbr_futures->tab[i][j];
		else 
			this->tab[i][j]=this->objects_needed.delta_futures->tab[i][j]*
			this->objects_needed.nbr_futures->tab[i][j]+this->tab[i][j-1];
	
}
Delta_Futures_In_Portfolio_2::~Delta_Futures_In_Portfolio_2(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
