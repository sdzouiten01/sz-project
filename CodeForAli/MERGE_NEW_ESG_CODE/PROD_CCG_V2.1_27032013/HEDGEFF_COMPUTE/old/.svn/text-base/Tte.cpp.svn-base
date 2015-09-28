#include "Tte.h"

Tte::Tte(Assumptions* assumptions)
{
	this->objects_needed.assumptions=assumptions;
	this->init_tab();
}
void Tte::init_tab()
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
			if (i<j||i>this->objects_needed.assumptions->length_of_option_contract+j
				-this->objects_needed.assumptions->sell_at_time_to_expiry)
				this->tab[i][j]=0;
			else this->tab[i][j]=(double)(this->objects_needed.assumptions->length_of_option_contract+
				j-i)/this->objects_needed.assumptions->nb_steps_per_year;

		}
	}
}
Tte::~Tte(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete this->tab[i];
	}
	delete tab;
}
