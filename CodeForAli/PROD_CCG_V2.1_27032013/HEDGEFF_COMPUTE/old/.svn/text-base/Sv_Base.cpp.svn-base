#include "Assumptions.h"
#include "Math_Tools.h"
#include "Tte_Swaps.h"
#include "Zero_Coupon_Rates.h"
#include "Sv_Base.h"

Sv_Base::Sv_Base(int swap_no,Assumptions* assumptions,
		Tte_Swaps* tte_swaps,
	
		Zero_Coupon_Rates* zero_coupon_rates)
{
	this->swap_no=swap_no;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.zero_coupon_rates=zero_coupon_rates;
	this->objects_needed.tte_swaps=tte_swaps;
	this->init_tab();
}
void Sv_Base::init_tab()
{
	double swap_rate=0;
	double zero_coupon_prec=0;
	double zero_coupon_next=0;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	this->tt.open("DEBUG_SV_BASE.txt");
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{		
		
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			
			
			if(i>this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j||i<j)
				this->tab[i][j]=0;
			else
			{
				tt<<"i="<<i<<", j="<<j<<endl;
				tt<<"	start_time="<<j<<endl;
				tt<<"	end_time="<<this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j<<endl;
				tt<<"	current_time="<<i<<endl;
				double tst=this->objects_needed.zero_coupon_rates->tab[0][0];
				double** tst2 = this->objects_needed.zero_coupon_rates->tab;
				this->tab[i][j]=Math_Tools::swap_price(this->objects_needed.zero_coupon_rates->tab,
														j,
														(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j,
														i,
														swap_rate,
														zero_coupon_prec,
														zero_coupon_next,
														this->objects_needed.assumptions->compounding_frequencies[swap_no],
														this->objects_needed.assumptions->nb_steps_per_year,
														false,false);
				tt<<"		swap_rate="<<swap_rate<<endl;
			}
			
			
		}
	}
	tt.close();
}
Sv_Base::~Sv_Base(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] tab[i];
	}
	delete[] tab;
}
