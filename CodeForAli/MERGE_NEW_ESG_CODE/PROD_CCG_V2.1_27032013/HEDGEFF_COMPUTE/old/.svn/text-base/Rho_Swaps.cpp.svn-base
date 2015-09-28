#include "Rho_Swaps.h"
#include "Assumptions.h"
#include "Math_Tools.h"
#include "Tte_Swaps.h"
#include "Zero_Coupon_Rates_Shock_Up.h"
#include "Zero_Coupon_Rates.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
Rho_Swaps::Rho_Swaps(
		int swap_no,
		Assumptions* assumptions,
		Tte_Swaps* tte_swaps,
		Zero_Coupon_Rates_Shock_Up* zero_coupon_rates_shock_up,
		Zero_Coupon_Rates_Shock_Down* zero_coupon_rates_shock_down,
		Zero_Coupon_Rates* zero_coupon_rates)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates_shock_up=zero_coupon_rates_shock_up;
	this->objects_needed.zero_coupon_rates_shock_down=zero_coupon_rates_shock_down;
	this->objects_needed.zero_coupon_rates=zero_coupon_rates;
	this->objects_needed.tte_swaps=tte_swaps;
	this->swap_no=swap_no;
	this->init_tab();
}
void Rho_Swaps::init_tab()
{
	double swap_rate=0;
	double zero_coupon_prec=0;
	double zero_coupon_next=0;
	//double zero_coupon_prec=0;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			
			if(this->objects_needed.tte_swaps->isMaturityReached(i,j)||
				i<j)
				this->tab[i][j]=0;
			else
			{
				//calcul du taux swap
				Math_Tools::swap_price(this->objects_needed.zero_coupon_rates->tab,
														j,
														(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j,
														i,
														swap_rate,
														zero_coupon_prec,
														zero_coupon_next,
														this->objects_needed.assumptions->compounding_frequencies[swap_no],
														this->objects_needed.assumptions->nb_steps_per_year,
														false,
														false);
				this->tab[i][j]=(Math_Tools::swap_price(this->objects_needed.zero_coupon_rates_shock_up->tab,
														j,
														(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j,
														i,
														swap_rate,
														zero_coupon_prec,
														zero_coupon_next,
														this->objects_needed.assumptions->compounding_frequencies[swap_no],
														this->objects_needed.assumptions->nb_steps_per_year,
														true,
														true)//true=>on ne change pas le taux swap
								-
								Math_Tools::swap_price(this->objects_needed.zero_coupon_rates_shock_down->tab,
													j,
													(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j,
													i,
													swap_rate,
													zero_coupon_prec,
													zero_coupon_next,
													this->objects_needed.assumptions->compounding_frequencies[swap_no],
													this->objects_needed.assumptions->nb_steps_per_year,
													true,
													true)//true=>on ne change pas le taux swap
								)
								/
								(2* this->objects_needed.assumptions->shocks_to_apply[this->swap_no]/100);
			}
		}
	}
}
Rho_Swaps::~Rho_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
}
