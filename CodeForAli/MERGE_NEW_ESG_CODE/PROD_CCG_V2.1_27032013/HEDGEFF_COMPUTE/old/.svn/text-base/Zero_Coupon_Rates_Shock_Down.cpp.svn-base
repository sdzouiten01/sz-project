#include "Annual_Swap_Rates_Shock_Down.h"
#include "Zero_Coupon_Rates_Shock_Down.h"
Zero_Coupon_Rates_Shock_Down::Zero_Coupon_Rates_Shock_Down(int swap_no, Annual_Swap_Rates_Shock_Down* annual_swap_rates_shock_down)
{
	this->swap_no=swap_no;
	this->objects_needed.annual_swap_rates_shock_down=annual_swap_rates_shock_down;
	this->dim_x=this->objects_needed.annual_swap_rates_shock_down->dim_x;
	this->dim_y=this->objects_needed.annual_swap_rates_shock_down->dim_y;
	
	this->init_tab();
}
void Zero_Coupon_Rates_Shock_Down::init_tab()
{
	this->tab=new double*[this->dim_x];
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i]=new double[this->dim_y];
	}
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i][0]=this->objects_needed.annual_swap_rates_shock_down->tab[i][0];
		this->tab[i][1]=this->objects_needed.annual_swap_rates_shock_down->tab[i][1];
		for(int j=2;j<this->dim_y;++j)
		{
		this->tab[i][j]=pow((
					(1+this->objects_needed.annual_swap_rates_shock_down->tab[i][j])/
					(1-(this->objects_needed.annual_swap_rates_shock_down->tab[i][j]/this->objects_needed.annual_swap_rates_shock_down->tab[i][j-1])*(1-pow((1+this->tab[i][j-1]),1-j)))
					),(1./(double)j))-1;
		}
	}
	
}
Zero_Coupon_Rates_Shock_Down::~Zero_Coupon_Rates_Shock_Down(void)
{
	for(int i=0;i<this->dim_x;++i)
	{
		delete []  this->tab[i];
	}
	delete [] this->tab;
}
