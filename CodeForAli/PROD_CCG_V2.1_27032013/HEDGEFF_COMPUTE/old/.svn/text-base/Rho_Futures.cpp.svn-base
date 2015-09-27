#include "Rho_Futures.h"
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Und_Base.h"
#include "Underlying.h"
#include "Tte_Futures.h"
#include "Rf_Futures_Shock_Up.h"
#include "Rf_Futures_Shock_Down.h"
Rho_Futures::Rho_Futures(
		int swap_no,
		Assumptions* assumptions,
		Tte_Futures* tte_futures,
		Rf_Futures_Shock_Up* rf_futures_shock_up,
		Rf_Futures_Shock_Down* rf_futures_shock_down,
		Underlying* underlying)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rf_futures_shock_up=rf_futures_shock_up;
	this->objects_needed.rf_futures_shock_down=rf_futures_shock_down;
	this->objects_needed.tte_futures=tte_futures;
	this->objects_needed.underlying=underlying;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rho_Futures::init_tab()
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
			if(this->objects_needed.tte_futures->isMaturityReached(i,j)||
				i<j)
				this->tab[i][j]=0;
			else
			this->tab[i][j]=(
								this->objects_needed.underlying->tab[i][j]*
									exp(this->objects_needed.rf_futures_shock_up->tab[i][j]*this->objects_needed.tte_futures->tab[i][j])
								- 
								this->objects_needed.underlying->tab[i][j]*
									exp(this->objects_needed.rf_futures_shock_down->tab[i][j]*this->objects_needed.tte_futures->tab[i][j])
							)/(2*this->objects_needed.assumptions->shocks_to_apply[this->swap_no]/100);
		}
	}
}
Rho_Futures::~Rho_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
