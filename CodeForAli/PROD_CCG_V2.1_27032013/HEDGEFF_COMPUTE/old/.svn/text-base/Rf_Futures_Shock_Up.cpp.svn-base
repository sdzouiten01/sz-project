#include "Rf_Futures_Shock_Up.h"
#include "Zero_Coupon_Rates_Shock_Up.h"
#include "Assumptions.h"
#include "Tte_Futures.h"
Rf_Futures_Shock_Up::Rf_Futures_Shock_Up(int swap_no, Assumptions* assumptions,Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up,Tte_Futures* tte_futures)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates_shock_up=zero_coupon_rates_shock_up;
	this->objects_needed.tte_futures=tte_futures;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rf_Futures_Shock_Up::init_tab()
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
					this->objects_needed.zero_coupon_rates_shock_up->tab[i][(int)(this->objects_needed.tte_futures->tab[i][j])]+
					(this->objects_needed.tte_futures->tab[i][j]-(int)this->objects_needed.tte_futures->tab[i][j])*
					this->objects_needed.zero_coupon_rates_shock_up->tab[i][(int)(this->objects_needed.tte_futures->tab[i][j])+1];
			}
		}
	}
	

}
Rf_Futures_Shock_Up::~Rf_Futures_Shock_Up(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
