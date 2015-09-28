#include "Rf.h"
#include "Zero_Coupon_Rates.h"
#include "Assumptions.h"
#include "Tte.h"
Rf::Rf(Assumptions* assumptions,Zero_Coupon_Rates* zero_coupon_rates,Tte* tte)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates=zero_coupon_rates;
	this->objects_needed.tte=tte;
	this->init_tab();
}
void Rf::init_tab()
{
	//ofstream tt("klkl.txt");
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
					this->objects_needed.zero_coupon_rates->tab[i][(int)(this->objects_needed.tte->tab[i][j])]+
					((double)((this->objects_needed.assumptions->length_of_option_contract+j-i)%this->objects_needed.assumptions->nb_steps_per_year))/this->objects_needed.assumptions->nb_steps_per_year*
					this->objects_needed.zero_coupon_rates->tab[i][(int)(this->objects_needed.tte->tab[i][j])+1];
			}
		}
	}
}
Rf::~Rf(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
}
