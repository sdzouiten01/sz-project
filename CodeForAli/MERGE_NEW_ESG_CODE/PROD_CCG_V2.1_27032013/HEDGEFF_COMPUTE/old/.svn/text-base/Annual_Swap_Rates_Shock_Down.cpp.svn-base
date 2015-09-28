#include "Annual_Swap_Rates_Shock_Down.h"
#include "Rw_Scn.h"
#include "Assumptions.h"
#include "Math_Tools.h"
Annual_Swap_Rates_Shock_Down::Annual_Swap_Rates_Shock_Down(int swap_no,Rw_Scn* rw_scn,Assumptions* assumptions)
{
	this->objects_needed.rw_scn=rw_scn;
	this->objects_needed.assumptions=assumptions;
	this->dim_x=this->objects_needed.rw_scn->model_period;
	this->dim_y=this->objects_needed.rw_scn->
		maturities[this->objects_needed.rw_scn->nb_of_mty-1]+1;
	this->maturities=this->objects_needed.rw_scn->maturities;
	this->mty_to_hedge=(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no];
	this->swap_no=swap_no;
	this->shock_to_apply=this->objects_needed.assumptions->shocks_to_apply[this->swap_no];
	/*this->tst.open("Annual_Swap_Rates_Shock_Down.txt");
	this->tst<<"dim_x= "<<this->dim_x<<endl;
	this->tst<<"dim_y= "<<this->dim_y<<endl;
	for(int i=0;i<this->dim_x;++i)
	{
		for(int j=0;j<this->dim_y;++j)
		{
			tst<<3<<endl;
			cout<<this->objects_needed.rw_scn->tab_0[i][j]<<" ";
		}
	tst<<endl;
	cout<<endl;
	}*/
	this->init_tab();
	/*tst<<"après interpol"<<endl;

	for(int i=0;i<this->dim_x;++i)
	{
	for(int j=0;j<this->dim_y;++j)
	{
		tst<<this->tab[i][j]<<" ";
	}
	tst<<endl;
	}
	*/
	this->shock_down();
/*
	tst.open("Annual_Swap_Rates_Shock_Down.txt");
	tst<<"dim_x= "<<this->dim_x<<endl;
	tst<<"dim_y= "<<this->dim_y<<endl;

	T=new double*[this->dim_x];
	for(int i=0;i<this->dim_x;++i)
	{
		T[i]=new double[this->dim_y];
	}
	double swap_rate=0;
	double zero_coupon_prec=0;

	for (int i =0;i<this->dim_x;++i)
	{
		for (int j =0;j<this->dim_y;++j)
		{
				T[i][j]=10+0*Math_Tools::swaption_call_price(this->objects_needed.zero_coupon_rates->tab_0,
														11,
														21,//(int)this->objects_needed.assumptions->maturities_to_hedge[swap_no]*this->objects_needed.assumptions->nb_steps_per_year+j,
														1,//i,
														swap_rate,
														zero_coupon_prec,
														this->objects_needed.assumptions->compounding_frequencies[swap_no],
														this->objects_needed.assumptions->nb_steps_per_year,
														false,
														false,
							  0.0486,//double strike,
							  4.91/100,//double sigma,
							  0,//this->objects_needed.zero_coupon_rates->tab_0[i][10],//double rate,
							  10//double time_to_maturity
							  );
				this->tab_0[i][j]=10;
		}
	}
	tst.close();
*/
    /*tst<<"après shock"<<endl;

	for(int i=0;i<this->dim_x;++i)
	{
	for(int j=0;j<this->dim_y;++j)
	{
		tst<<this->tab[i][j]<<" ";
	}
	tst<<endl;
	}
	this->tst.close();*/
}
/*
double Annual_Swap_Rates_Shock_Down::interpolated_rate(double mty);
{
	double res=0;
	double product=1;
	for (int i=0;i<this->objects_needed.rw_scn->model_period;i++)
	{
		for (int j=0;j<this->objects_needed.rw_scn->nb_of_mty-1;j++)
		{
			if(i!=j) product*=(mty-this->objects_needed.rw_scn.maturities[i])
					          /(this->objects_needed.rw_scn.maturities[j]-this->objects_needed.rw_scn.maturities[i]);
		}
		res+=product*this->objects_needed.rw_scn.tab_0[this->objects_needed.rw_scn.model_period-1][i];
	}
}
*/
void Annual_Swap_Rates_Shock_Down::init_tab()
{
	
	
	this->tab=new double*[this->dim_x];
	int deb, fin, l;
	double pente;
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i]=new double[this->dim_y];
	}
	this->maturities=new int[this->dim_y];
	this->maturities[0]=0;
	for(int j = 0; j< this->dim_x; ++j)
	{
		deb=this->objects_needed.rw_scn->maturities[0];
		//this->tab[j][0]=12;
		for(int k=0;k<this->objects_needed.rw_scn->nb_of_mty-1;++k)
		{
			//tst<<"j= "<<j<<" k= "<<k<<endl;
			fin=this->objects_needed.rw_scn->maturities[k+1];
			//tst<<fin<<endl;
			pente=(this->objects_needed.rw_scn->tab[j][k+1]-this->objects_needed.rw_scn->tab[j][k])/
				  (this->objects_needed.rw_scn->maturities[k+1]-this->objects_needed.rw_scn->maturities[k]);
			tst<<pente<<endl;
			for(int l=deb;l<=fin;++l)
			{
				this->maturities[l]=this->maturities[l-1]+1;
				this->tab[j][l]=this->objects_needed.rw_scn->tab[j][k]+(this->maturities[l]-this->objects_needed.rw_scn->maturities[k])*pente;
			}
			deb=fin+1;

		}
		l=this->objects_needed.rw_scn->maturities[0]-1;
		while(l>=0)
		{
			this->tab[j][l]=2*this->tab[j][l+1]-this->tab[j][l+2];
			l--;
		}
	}
	for(int i=0;i<this->dim_x;++i)
	{
		for(int j=0;j<this->dim_y;++j)
		{
		
			this->tab[i][j]=this->objects_needed.assumptions->rates_freq_ann
				*this->tab[i][j]/100+
				(1-this->objects_needed.assumptions->rates_freq_ann)*
				(pow((1+0.5*this->tab[i][j]/100),2)-1);
		}
	}
}


double Annual_Swap_Rates_Shock_Down::shock(int start_rate, int key_rate, int end_rate, int mty)
{
	if (start_rate==0)
	{
		if (mty<=key_rate) return this->shock_to_apply/100;
		else if (mty>=end_rate) return 0.;
		else return (end_rate - mty)*this->shock_to_apply/100
					 /(end_rate-key_rate);
	}
	else if (end_rate==100)
	{
		if (mty<=start_rate) return 0.;
		else if (mty>=end_rate) return this->shock_to_apply/100;
		else return (mty - start_rate)*this->shock_to_apply/100
					 /(key_rate-start_rate);
	}
	else 
	{
		if (mty<=start_rate || mty>=end_rate) return 0.;
		else if (mty>start_rate && mty<=key_rate) return (mty - start_rate)*this->shock_to_apply/100
														  /(key_rate-start_rate);
		else return (end_rate - mty)*this->shock_to_apply/100
					 /(end_rate-key_rate);
	}
}


void Annual_Swap_Rates_Shock_Down::shock_down()
{

	for (int i =0;i<this->dim_x;++i)
	{
		for (int j =0;j<this->dim_y;++j)
		{	
			//if(this->swap_no==0)
			//classic shock down
			/*{this->tab[i][j]=this->tab[i][j]-shock(0,
														this->mty_to_hedge,
														(int)this->objects_needed.assumptions->maturities_to_hedge[this->swap_no+1],
														j);*/

//chth221209-2
			
						/*{*/this->tab[i][j]=this->tab[i][j]-shock(int(this->objects_needed.assumptions->start_mty[this->swap_no]),
														this->mty_to_hedge,
														int(this->objects_needed.assumptions->end_mty[this->swap_no]),
														j);
			

			//}
			/*else
			{
				if(this->swap_no==this->objects_needed.assumptions->nb_maturities_to_hedge-1)

					this->tab[i][j]=this->tab[i][j]-shock((int)this->objects_needed.assumptions->maturities_to_hedge[this->swap_no-1],
														this->mty_to_hedge,100,j);
				else
					this->tab[i][j]=this->tab[i][j]-shock((int)this->objects_needed.assumptions->maturities_to_hedge[this->swap_no-1],
														this->mty_to_hedge,
														(int)this->objects_needed.assumptions->maturities_to_hedge[this->swap_no+1]
														,j);
			}*/
		}
	}
}


Annual_Swap_Rates_Shock_Down::~Annual_Swap_Rates_Shock_Down(void)
{
	for(int i=0;i<this->dim_x;++i)
	{
		delete [] this->tab[i]; 
	}
	delete [] this->tab;
	delete this->maturities;
}