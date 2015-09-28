#include "Assumptions.h"
#include "Hedgeff.h"
#include "Delta_Asset.h"
#include "Rf_Futures.h"
#include "Tte_Futures.h"
Delta_Asset::Delta_Asset(Assumptions* assumptions,Hedgeff* hedgeff,Rf_Futures* rf_futures)
{
		this->objects_needed.assumptions=assumptions;
		this->objects_needed.hedgeff=hedgeff;
		this->objects_needed.rf_futures=rf_futures;
		this->init_tab_of_delta_asset_struct();
}
void Delta_Asset::init_tab_of_delta_asset_struct()
{	
	this->tab_of_delta_asset_struct= new Delta_Asset_Struct[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		init_time(i);
		init_k(i);
		init_r(i);
		init_q(i);
		init_tte(i);
		init_sigma(i);
		init_delta(i);
	}
}

void Delta_Asset::init_time(int i)
{
	this->tab_of_delta_asset_struct[i].time=i;
}
void Delta_Asset::init_k(int i)
{
	this->tab_of_delta_asset_struct[i].k=this->objects_needed.hedgeff->index[i];
}
void Delta_Asset::init_r(int i)
{
	this->tab_of_delta_asset_struct[i].r=this->objects_needed.rf_futures->tab[i][i];
}
void Delta_Asset::init_q(int i)
{
	this->tab_of_delta_asset_struct[i].q=0;
}
void Delta_Asset::init_tte(int i)
{
	this->tab_of_delta_asset_struct[i].tte=this->objects_needed.rf_futures->objects_needed.tte_futures->tab[i][i];
		//=(double)this->objects_needed.assumptions->length_of_futures_contract/this->objects_needed.assumptions->nb_steps_per_year;
}
void Delta_Asset::init_sigma(int i)
{
	this->tab_of_delta_asset_struct[i].sigma
		=this->objects_needed.assumptions->volatility;
}

Delta_Asset::~Delta_Asset(void)
{
	delete[] this->tab_of_delta_asset_struct;
}
void  Delta_Asset::init_delta(int i)
{
			this->tab_of_delta_asset_struct[i].delta=
					exp(this->tab_of_delta_asset_struct[i].r*
					this->tab_of_delta_asset_struct[i].tte);
						
}
