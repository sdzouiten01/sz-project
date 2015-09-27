#include "Annual_Swap_Rates.h"
#include "Rw_Scn.h"
#include "Assumptions.h"
Annual_Swap_Rates::Annual_Swap_Rates(Rw_Scn* rw_scn,Assumptions* assumptions)
{
	this->objects_needed.rw_scn=rw_scn;
	this->objects_needed.assumptions=assumptions;
	this->dim_x=this->objects_needed.rw_scn->model_period;
	this->dim_y=this->objects_needed.rw_scn->
		maturities[this->objects_needed.rw_scn->nb_of_mty-1]+1;
	/*tst.open("Annual_Swap_Rates.txt");
	tst<<"dim_x= "<<this->dim_x<<endl;
	tst<<"dim_y= "<<this->dim_y<<endl;*/
	/*for(int i=0;i<this->dim_x;++i)
	{
		for(int j=0;j<this->dim_y;++j)
		{
			tst<<3<<endl;
			cout<<this->objects_needed.rw_scn->tab[i][j]<<" ";
		}
	tst<<endl;
	cout<<endl;
	}*/
	this->init_tab();
	//tst<<"après"<<endl;

	//for(int i=0;i<this->dim_x;++i)
	//{
	//for(int j=0;j<this->dim_y;++j)
	//{
	//	tst<<this->tab[i][j]<<" ";
	//}
	//tst<<endl;
	//}
	//tst.close();
}
void Annual_Swap_Rates::init_tab()
{
	this->tab=new double*[this->dim_x];
	for(int i=0;i<this->dim_x;++i)
	{
		this->tab[i]=new double[this->dim_y];
	}
	this->maturities=new int[this->dim_y];
	this->maturities[0]=0;
	for(int j = 0; j< this->objects_needed.rw_scn->model_period; ++j)
	{
		int deb=this->objects_needed.rw_scn->maturities[0];
		//this->tab[j][0]=12;
		for(int k=0;k<this->objects_needed.rw_scn->nb_of_mty-1;++k)
		{
			//tst<<"j= "<<j<<" k= "<<k<<endl;
			int fin=this->objects_needed.rw_scn->maturities[k+1];
			//tst<<fin<<endl;
			double pente=(this->objects_needed.rw_scn->tab[j][k+1]-this->objects_needed.rw_scn->tab[j][k])/
				(this->objects_needed.rw_scn->maturities[k+1]-this->objects_needed.rw_scn->maturities[k]);
			tst<<pente<<endl;
			for(int l=deb;l<=fin;++l)
			{
				this->maturities[l]=this->maturities[l-1]+1;
				this->tab[j][l]=this->objects_needed.rw_scn->tab[j][k]+(this->maturities[l]-this->objects_needed.rw_scn->maturities[k])*pente;
			}
			deb=fin+1;

		}
		int l=this->objects_needed.rw_scn->maturities[0]-1;
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
Annual_Swap_Rates::~Annual_Swap_Rates(void)
{
	for(int i=0;i<this->dim_x;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
	delete [] this->maturities;
}
