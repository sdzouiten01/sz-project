#include "Annual_Swap_Rates_Shock_Up.h"
#include "Zero_Coupon_Rates_Shock_Up.h"
Zero_Coupon_Rates_Shock_Up::Zero_Coupon_Rates_Shock_Up(int swap_no , Annual_Swap_Rates_Shock_Up*annual_swap_rates_shock_up)
{
	this->swap_no=swap_no;
	this->objects_needed.annual_swap_rates_shock_up=annual_swap_rates_shock_up;
	this->dim_x=this->objects_needed.annual_swap_rates_shock_up->dim_x;
	this->dim_y=this->objects_needed.annual_swap_rates_shock_up->dim_y;
	
	this->init_tab();

}
void Zero_Coupon_Rates_Shock_Up::init_tab()
{
	this->tab=new double*[this->dim_x];
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i]=new double[this->dim_y];
	}
	
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i][0]=this->objects_needed.annual_swap_rates_shock_up->tab[i][0];
		this->tab[i][1]=this->objects_needed.annual_swap_rates_shock_up->tab[i][1];
		for(int j=2;j<this->dim_y;++j)
		{
		this->tab[i][j]=pow((
					(1+this->objects_needed.annual_swap_rates_shock_up->tab[i][j])/
					(1-(this->objects_needed.annual_swap_rates_shock_up->tab[i][j]/this->objects_needed.annual_swap_rates_shock_up->tab[i][j-1])*(1-pow((1+this->tab[i][j-1]),1-j)))
					),(1./(double)j))-1;
		}
	}
	
}
Zero_Coupon_Rates_Shock_Up::~Zero_Coupon_Rates_Shock_Up(void)
{                                                              
	for(int i=0;i<this->dim_x;++i)
	{
		delete []  this->tab[i];
	}
	delete [] this->tab;
}
