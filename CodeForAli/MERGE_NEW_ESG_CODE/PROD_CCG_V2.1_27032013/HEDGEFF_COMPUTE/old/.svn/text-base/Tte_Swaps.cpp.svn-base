#include "Tte_Swaps.h"

Tte_Swaps::Tte_Swaps(int swap_no,Assumptions* assumptions)
{
	this->objects_needed.assumptions=assumptions;
	this->swap_no=swap_no;
	if(this->objects_needed.assumptions->sheduled_futures)
		this->init_tab_sheduled();
	else
		this->init_tab_shift();
}
void Tte_Swaps::init_tab_shift()
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
			if (i>this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j||i<j)
				this->tab[i][j]=0;
			else this->tab[i][j]=(double)(this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+
				j-i)/this->objects_needed.assumptions->nb_steps_per_year;

		}
	}
}
bool Tte_Swaps::isNullPrevTte(int i, int j)
{
	if (i-1>=0)
	{
		if(this->tab[i-1][j]==0 && i>j)
			return true;
		else return false;
	}
	return false;
}
bool Tte_Swaps::isMaturityReached(int i, int j)
{
	if(this->tab[i][j]==0 && i>=j)
		return true;
	else return false;
}
void Tte_Swaps::init_tab_sheduled()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	ofstream debug_file("DEBUGGG.txt");
	double maturity=0;
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		if(j%this->objects_needed.assumptions->length_of_futures_contract==0)
		{
			maturity+=this->objects_needed.assumptions->length_of_futures_contract;
		}
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if (i<j||i>maturity)
				this->tab[i][j]=0;
			else 
			{
				debug_file<<"i="<<i<<",j="<<j<<endl;
				debug_file<<"	maturity="<<maturity<<endl;
				this->tab[i][j]=(double)(maturity
				-i)/this->objects_needed.assumptions->nb_steps_per_year;
			}

		}
	}
}
Tte_Swaps::~Tte_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] tab;
}
