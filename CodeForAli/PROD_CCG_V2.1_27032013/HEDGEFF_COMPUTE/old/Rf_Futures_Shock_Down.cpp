#include "Rf_Futures_Shock_Down.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
#include "Assumptions.h"
#include "Tte_Futures.h"
Rf_Futures_Shock_Down::Rf_Futures_Shock_Down(int swap_no, Assumptions* assumptions,Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,Tte_Futures* tte_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates_shock_down=zero_coupon_rates_shock_down;
	this->objects_needed.tte_futures=tte_futures;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rf_Futures_Shock_Down::init_tab()
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
			if(i<j || this->objects_needed.tte_futures->isNullPrevTte(i,j))
			{
				this->tab[i][j]=0;
			}
			else
			{
				this->tab[i][j]=
					(1-(this->objects_needed.tte_futures->tab[i][j]-(int)(this->objects_needed.tte_futures->tab[i][j])))*
					this->objects_needed.zero_coupon_rates_shock_down->tab[i][(int)(this->objects_needed.tte_futures->tab[i][j])]+
					(this->objects_needed.tte_futures->tab[i][j]-(int)this->objects_needed.tte_futures->tab[i][j])*
					this->objects_needed.zero_coupon_rates_shock_down->tab[i][(int)(this->objects_needed.tte_futures->tab[i][j])+1];
			}
		}
	}
	

}
Rf_Futures_Shock_Down::~Rf_Futures_Shock_Down(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
