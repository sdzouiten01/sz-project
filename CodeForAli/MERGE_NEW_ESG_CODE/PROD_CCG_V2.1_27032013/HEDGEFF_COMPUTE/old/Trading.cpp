#include "Trading.h"
#include "Ov_Base.h"
#include "Nbr_Puts.h"
#include "Assumptions.h"

Trading::Trading(Assumptions* assumptions,
		Ov_Base* ov_base,
		Nbr_Puts* nbr_puts)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.ov_base=ov_base;
	this->objects_needed.nbr_puts=nbr_puts;
	this->init_tab();
}
void Trading::init_tab()
{	
	this->trading=new double[this->objects_needed.assumptions->model_period];
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->trading[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			if(i==0)
				this->tab[i][j]=
				this->objects_needed.nbr_puts->tab[i][j]
				*this->objects_needed.ov_base->tab[i][j];
			else this->tab[i][j]=
				(this->objects_needed.nbr_puts->tab[i][j]-this->objects_needed.nbr_puts->tab[i-1][j])
				*this->objects_needed.ov_base->tab[i][j];
			this->trading[i]+=this->tab[i][j];
		}
		

	}
	//ofstream bla("ogog.txt");
	//for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	//{
	//	//bla<<j<<endl;
	//	bla<<this->trading[j]<<endl;
	//}
}

Trading::~Trading(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->trading;
}
