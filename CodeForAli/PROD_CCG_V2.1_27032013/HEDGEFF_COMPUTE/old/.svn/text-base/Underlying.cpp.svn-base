#include "Underlying.h"

Underlying::Underlying(Hedgeff* hedgeff,
		Assumptions* assumptions)
{
	this->objects_needed.hedgeff=hedgeff;
	this->objects_needed.assumptions=assumptions;
	this->init_tab();
}
void Underlying::init_tab()
{

	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}

	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(i>=j) this->tab[i][j]=this->objects_needed.hedgeff->index[i];
			else this->tab[i][j]=0;
		}
	}
}
Underlying::~Underlying(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
