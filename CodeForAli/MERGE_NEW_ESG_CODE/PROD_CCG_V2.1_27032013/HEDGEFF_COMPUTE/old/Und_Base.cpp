#include "Und_Base.h"
#include "Underlying.h"
#include "Assumptions.h"
Und_Base::Und_Base(Underlying* underlying,
		Assumptions* assumptions)
{
	this->objects_needed.underlying=underlying;
	this->objects_needed.assumptions=assumptions;
	this->init_tab();
}
void Und_Base::init_tab()


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
			if (i<j) this->tab[i][j]=0;
			else if (i==j) this->tab[i][j]=this->objects_needed.underlying->tab[i][j]*1;
			else this->tab[i][j]=this->tab[i-1][j];
			////cout<<"bbbbbbbbb "<<this->tab[i][j]<<endl;
		}
	}
}
Und_Base::~Und_Base(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete this->tab[i];
	}
	delete this->tab;
}
