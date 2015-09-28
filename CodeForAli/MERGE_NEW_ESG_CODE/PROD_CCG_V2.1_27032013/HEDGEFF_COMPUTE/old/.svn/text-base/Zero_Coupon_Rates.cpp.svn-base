#include "Annual_Swap_Rates.h"
#include "Zero_Coupon_Rates.h"
Zero_Coupon_Rates::Zero_Coupon_Rates(Annual_Swap_Rates* annual_swap_rates)
{
	this->objects_needed.annual_swap_rates=annual_swap_rates;
	this->dim_x=this->objects_needed.annual_swap_rates->dim_x;
	this->dim_y=this->objects_needed.annual_swap_rates->dim_y;
	//tst.open("Zero_Coupon_Rates.txt");
	this->init_tab();
	/*for(int i=0;i<this->dim_x;++i)
	{
		for(int j=0;j<this->dim_y;++j)
		{
			tst<<this->tab[i][j]<<" ";
		}
		tst<<endl;
	}
	tst.close();*/
}
void Zero_Coupon_Rates::init_tab()
{
	this->tab=new double*[this->dim_x];
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i]=new double[this->dim_y];
	}
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i][0]=this->objects_needed.annual_swap_rates->tab[i][0];
		this->tab[i][1]=this->objects_needed.annual_swap_rates->tab[i][1];
		for(int j=2;j<this->dim_y;++j)
		{
		this->tab[i][j]=pow((
					(1+this->objects_needed.annual_swap_rates->tab[i][j])/
					(1-(this->objects_needed.annual_swap_rates->tab[i][j]/this->objects_needed.annual_swap_rates->tab[i][j-1])*(1-pow((1+this->tab[i][j-1]),1-j)))
					),(1./(double)j))-1;
		}
	}
}
Zero_Coupon_Rates::~Zero_Coupon_Rates(void)
{
	for(int i=0;i<this->dim_x;++i)
	{
		delete []  this->tab[i];
	}
	delete [] this->tab;
}
