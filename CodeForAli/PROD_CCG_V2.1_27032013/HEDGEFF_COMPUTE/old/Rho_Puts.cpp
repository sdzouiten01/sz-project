#include "Rho_Puts.h"

Rho_Puts::Rho_Puts(int swap_no,Assumptions* assumptions,
		Und_Base* und_base,
		Underlying* underlying,
		Tte* tte,
		Rf_Shock_Up* rf_shock_up,
		Rf_Shock_Down* rf_shock_down)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.und_base=und_base;
	this->objects_needed.underlying=underlying;
	this->objects_needed.tte=tte;
	this->objects_needed.rf_shock_down=rf_shock_down;
	this->objects_needed.rf_shock_up=rf_shock_up;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rho_Puts::init_tab()
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
				
				if(i>=this->objects_needed.assumptions->length_of_option_contract
					+j-this->objects_needed.assumptions->sell_at_time_to_expiry||
					i<j)
					this->tab[i][j]=0;
				else
				this->tab[i][j]=(Math_Tools::put_price_BS(
					this->objects_needed.underlying->tab[i][j],
					this->objects_needed.und_base->tab[i][j],
					this->objects_needed.tte->tab[i][j],
					this->objects_needed.rf_shock_up->tab[i][j],
					0,
					this->objects_needed.assumptions->volatility)-
					Math_Tools::put_price_BS(
					this->objects_needed.underlying->tab[i][j],
					this->objects_needed.und_base->tab[i][j],
					this->objects_needed.tte->tab[i][j],
					this->objects_needed.rf_shock_down->tab[i][j],
					0,
					this->objects_needed.assumptions->volatility))/(
					2*this->objects_needed.assumptions->shocks_to_apply[this->swap_no]/100);
			}
		}
	
}
Rho_Puts::~Rho_Puts(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete[] this->tab;
}
