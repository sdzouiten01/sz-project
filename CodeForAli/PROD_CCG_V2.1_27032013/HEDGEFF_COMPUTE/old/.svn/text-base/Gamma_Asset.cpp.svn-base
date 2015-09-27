#include "Assumptions.h"
#include "Hedgeff.h"
#include "Gamma_Asset.h"
#include "Rf.h"
Gamma_Asset::Gamma_Asset(Assumptions* assumptions,Hedgeff* hedgeff,Rf* rf)
{
		this->objects_needed.assumptions=assumptions;
		this->objects_needed.hedgeff=hedgeff;
		this->objects_needed.rf=rf;
		this->init_tab_of_gamma_asset_struct();
}
void Gamma_Asset::init_tab_of_gamma_asset_struct()
{	
	this->tab_of_gamma_asset_struct= new Gamma_Asset_Struct[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		init_time(i);
		init_k(i);
		init_r(i);
		init_q(i);
		init_tte(i);
		init_sigma(i);
		//init_fund(i);
		init_gamma(i);
	}
}

void Gamma_Asset::init_time(int i)
{
	this->tab_of_gamma_asset_struct[i].time=i;
}
void Gamma_Asset::init_k(int i)
{
	this->tab_of_gamma_asset_struct[i].k=this->objects_needed.hedgeff->index[i];
}
void Gamma_Asset::init_r(int i)
{
	this->tab_of_gamma_asset_struct[i].r=this->objects_needed.rf->tab[i][i];
}
void Gamma_Asset::init_q(int i)
{
	this->tab_of_gamma_asset_struct[i].q=0;
}
void Gamma_Asset::init_tte(int i)
{
	this->tab_of_gamma_asset_struct[i].tte
		=(double)this->objects_needed.assumptions->length_of_option_contract/this->objects_needed.assumptions->nb_steps_per_year;
}
void Gamma_Asset::init_sigma(int i)
{
	this->tab_of_gamma_asset_struct[i].sigma
		=this->objects_needed.assumptions->volatility;
}

Gamma_Asset::~Gamma_Asset(void)
{
	delete [] this->tab_of_gamma_asset_struct;
}
void  Gamma_Asset::init_gamma(int i)
{
	this->tab_of_gamma_asset_struct[i].gamma=
		Math_Tools::gamma_BS(this->tab_of_gamma_asset_struct[i].k,
		this->tab_of_gamma_asset_struct[i].k,
		this->tab_of_gamma_asset_struct[i].tte,
		this->tab_of_gamma_asset_struct[i].r,
		0,
		this->tab_of_gamma_asset_struct[i].sigma);
							
}
void Gamma_Asset::init_fund(int i)
{
	/*this->tab_of_gamma_asset_struct[i].fund=new double*[Nb_Rows_In_Fund];
	for(int j=0;j<Nb_Rows_In_Fund;++j)
	{
		this->tab_of_gamma_asset_struct[i].fund[j]=new double[Nb_Cols_In_Fund];
	}
	this->tab_of_gamma_asset_struct[i].fund[S][Base]=this->tab_of_gamma_asset_struct[i].k;
	this->tab_of_gamma_asset_struct[i].fund[S][Asset_Dwn_1]=
		this->tab_of_gamma_asset_struct[i].fund[S][Base]*
		(1-this->objects_needed.assumptions->shock_size);
	this->tab_of_gamma_asset_struct[i].fund[S][Asset_Up_1]=
		this->tab_of_gamma_asset_struct[i].fund[S][Base]*
		(1+this->objects_needed.assumptions->shock_size);

	for(int j=0;j<Nb_Cols_In_Fund;++j)
	{
		this->tab_of_gamma_asset_struct[i].fund[D1][j]=
			Math_Tools::d0(this->tab_of_gamma_asset_struct[i].tte,
							this->tab_of_gamma_asset_struct[i].fund[S][j],
							this->tab_of_gamma_asset_struct[i].k,
							this->tab_of_gamma_asset_struct[i].sigma);
		this->tab_of_gamma_asset_struct[i].fund[D2][j]=
			Math_Tools::d1(this->tab_of_gamma_asset_struct[i].tte,
							this->tab_of_gamma_asset_struct[i].fund[S][j],
							this->tab_of_gamma_asset_struct[i].k,
							this->tab_of_gamma_asset_struct[i].sigma);
		this->tab_of_gamma_asset_struct[i].fund[N_MD1][j]=
			Math_Tools::N(-this->tab_of_gamma_asset_struct[i].fund[D1][j]);
		this->tab_of_gamma_asset_struct[i].fund[N_MD2][j]=
			Math_Tools::N(-this->tab_of_gamma_asset_struct[i].fund[D2][j]);
		this->tab_of_gamma_asset_struct[i].fund[MvUnit][j]=
			this->tab_of_gamma_asset_struct[i].k*exp(-	this->tab_of_gamma_asset_struct[i].r)
				*this->tab_of_gamma_asset_struct[i].fund[N_MD2][j]
				-this->tab_of_gamma_asset_struct[i].fund[S][j]*
				exp(-this->tab_of_gamma_asset_struct[i].q*this->tab_of_gamma_asset_struct[i].tte)
				*this->tab_of_gamma_asset_struct[i].fund[N_MD1][j];
		this->tab_of_gamma_asset_struct[i].fund[Amount][j]=1;
		this->tab_of_gamma_asset_struct[i].fund[Mv][j]=
			this->tab_of_gamma_asset_struct[i].fund[Amount][j]*
			this->tab_of_gamma_asset_struct[i].fund[MvUnit][j];
		this->tab_of_gamma_asset_struct[i].gamma=
			this->tab_of_gamma_asset_struct[i].fund[Mv][Asset_Dwn_1]+
			this->tab_of_gamma_asset_struct[i].fund[Mv][Asset_Up_1]-
			2*this->tab_of_gamma_asset_struct[i].fund[Mv][Base];
	}*/
	
}