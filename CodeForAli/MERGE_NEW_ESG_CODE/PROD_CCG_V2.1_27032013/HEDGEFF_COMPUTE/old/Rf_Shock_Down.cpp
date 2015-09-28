#include "Rf_Shock_Down.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
#include "Assumptions.h"
#include "Tte.h"
Rf_Shock_Down::Rf_Shock_Down(int swap_no, Assumptions* assumptions,Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,Tte* tte)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates_shock_down=zero_coupon_rates_shock_down;
	this->objects_needed.tte=tte;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rf_Shock_Down::init_tab()
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
			if(i<j || i>this->objects_needed.assumptions->length_of_option_contract+j-
				this->objects_needed.assumptions->sell_at_time_to_expiry)
			{
				this->tab[i][j]=0;
			}			
			else
			{				
				this->tab[i][j]=
					(1-((double)((this->objects_needed.assumptions->length_of_option_contract+j-i)%this->objects_needed.assumptions->nb_steps_per_year))/this->objects_needed.assumptions->nb_steps_per_year)*
					this->objects_needed.zero_coupon_rates_shock_down->tab[i][(int)(this->objects_needed.tte->tab[i][j])]+
					((double)((this->objects_needed.assumptions->length_of_option_contract+j-i)%this->objects_needed.assumptions->nb_steps_per_year))/this->objects_needed.assumptions->nb_steps_per_year*
					this->objects_needed.zero_coupon_rates_shock_down->tab[i][(int)(this->objects_needed.tte->tab[i][j])+1];
			}
		}
	}
	

}
Rf_Shock_Down::~Rf_Shock_Down(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
