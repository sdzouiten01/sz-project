#include "Assumptions.h"
#include "Delta_Asset.h"
#include "Hedgeff.h"
#include "Existing_Delta_Futures.h"
#include "Existing_Delta.h"
#include "Delta_Futures_In_Portfolio.h"
#include "Delta_Futures_In_Portfolio_2.h"
#include "Future_Trade.h"
#include "Delta_Puts.h"

//Future_Trade::Future_Trade(Assumptions* assumptions, Delta_Asset* delta_asset, 
//							   Hedgeff* hedgeff,Existing_Delta_Futures* existing_delta,
//							   Delta_Futures_In_Portfolio* delta_futures_in_portfolio)
//{
//	this->objects_needed.assumptions=assumptions;
//	this->objects_needed.delta_asset=delta_asset;
//	this->objects_needed.hedgeff=hedgeff;
//	this->objects_needed.existing_delta=existing_delta;
//	this->objects_needed.delta_futures_in_portfolio=delta_futures_in_portfolio;
//}
Future_Trade::Future_Trade(Assumptions* assumptions,Delta_Asset* delta_asset,
		Hedgeff* hedgeff)
{
	//debug.open("Debug_Fut_Pursh.txt");
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.delta_asset=delta_asset;
	this->objects_needed.hedgeff=hedgeff;
	this->tab_futures_purchase_struct=new Futures_Purchase_Struct[this->objects_needed.assumptions->model_period];
	this->delta_bounds.liab_delta_lower_bound=0;
	this->delta_bounds.liab_delta_upper_bound=0;
	this->delta_bounds.net_delta_lower_bound=0;
	this->delta_bounds.net_delta_upper_bound=0;
	//this->characteristics.length_of_futures_contract

}
void Future_Trade::init_delta_bounds()
{
	this->delta_bounds.liab_delta_lower_bound=0;
	this->delta_bounds.liab_delta_upper_bound=0;
	this->delta_bounds.net_delta_lower_bound=0;
	this->delta_bounds.net_delta_upper_bound=0;
}
void Future_Trade::init_characteristics()
{
	this->characteristics.length_of_futures_contract=
		this->objects_needed.assumptions->length_of_futures_contract;
	this->characteristics.sell_at_time_to_expiry=
		this->objects_needed.assumptions->sell_at_time_to_expiry;
	//this->characteristics.shock_size=
	//	this->objects_needed.assumptions->shock_size;
}
void Future_Trade::init_tab_futures_purchase_struct(int i)
{
	/*for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{*/
		init_time(i);
		init_hedging(i);		
		init_ex_delta(i);
		init_ex_delta_futures(i);
		init_delta_asset(i);
		init_delta_liability(i);
		init_net_delta(i);
//		init_req_net_delta(i);
//		init_req_delta_liability(i);
		init_new_net_delta(i);
		init_tab(i);
		init_short_existing_positions(i);
	//	init_contracts(i);
	/*}*/
}
void Future_Trade::init_time(int i)
{
	tab_futures_purchase_struct[i].time=i;
}
void Future_Trade::init_hedging(int i)
{
	if(!this->objects_needed.assumptions->hedge_delta||
		tab_futures_purchase_struct[i].time%(int)this->objects_needed.assumptions->reb_freq_delta!=0)
	{
		this->tab_futures_purchase_struct[i].hedging=false;
	}
	else this->tab_futures_purchase_struct[i].hedging=true;
}
void Future_Trade::init_tab(int i)
{
	this->tab_futures_purchase_struct[i].tab=new int*[Nb_Rows_In_Tab];
	for(int j=0;j<Nb_Cols_In_Tab;++j)
	{
		this->tab_futures_purchase_struct[i].tab[j]=new int[Nb_Cols_In_Tab];
	}
	for(int k=0;k<Nb_Rows_In_Tab;++k)
	{
		for(int j=0;j<Nb_Cols_In_Tab;++j)
		{
		this->tab_futures_purchase_struct[i].tab[k][j]=0;
		}
	}
	debug<<this->tab_futures_purchase_struct[i].ex_delta_futures<<endl;
	if((this->tab_futures_purchase_struct[i].net_delta>=0)&&
		(this->tab_futures_purchase_struct[i].ex_delta_futures>=0))
		this->tab_futures_purchase_struct[i].tab[Net_Delta_Pos][Ex_Delta_Fut_Pos]=1;

	else if((this->tab_futures_purchase_struct[i].net_delta>=0)&&
		(this->tab_futures_purchase_struct[i].ex_delta_futures<=0))
		this->tab_futures_purchase_struct[i].tab[Net_Delta_Pos][Ex_Delta_Fut_Neg]=1;

	else if((this->tab_futures_purchase_struct[i].net_delta<=0)&&(
		this->tab_futures_purchase_struct[i].ex_delta_futures>=0))
		this->tab_futures_purchase_struct[i].tab[Net_Delta_Neg][Ex_Delta_Fut_Pos]=1;

	else if((this->tab_futures_purchase_struct[i].net_delta<=0)&&(
		this->tab_futures_purchase_struct[i].ex_delta_futures<=0))
		this->tab_futures_purchase_struct[i].tab[Net_Delta_Neg][Ex_Delta_Fut_Neg]=1;


}



void Future_Trade::init_ex_delta(int i)
{
	/*if(this->tab_futures_purchase_struct[i].time==0) this->tab_futures_purchase_struct[i].ex_delta=0;
	else this->tab_futures_purchase_struct[i].ex_delta=
		this->objects_needed.existing_delta->return_existing_fv(i-1);*/
	this->tab_futures_purchase_struct[i].ex_delta=
		this->objects_needed.existing_delta->return_existing_fv(i-1);
}
void Future_Trade::init_ex_delta_futures(int i)
{
	/*if(this->tab_futures_purchase_struct[i].time==0) this->tab_futures_purchase_struct[i].ex_delta_futures=0;
	else this->tab_futures_purchase_struct[i].ex_delta_futures=
		this->objects_needed.existing_delta_futures->return_existing_fv(i-1);*/
	this->tab_futures_purchase_struct[i].ex_delta_futures=
		this->objects_needed.existing_delta_futures->return_existing_fv(i-1);
}

void Future_Trade::init_delta_asset(int i)
{
	this->tab_futures_purchase_struct[i].delta_asset=
		this->objects_needed.delta_asset->tab_of_delta_asset_struct[i].delta;
}
void Future_Trade::init_delta_liability(int i)
{
	this->tab_futures_purchase_struct[i].delta_liability=
		this->objects_needed.hedgeff->delta_liab[i];
}
void Future_Trade::init_net_delta(int i)
{
	this->tab_futures_purchase_struct[i].net_delta=
		this->tab_futures_purchase_struct[i].ex_delta-
		this->tab_futures_purchase_struct[i].delta_liability;
}
void Future_Trade::init_short_existing_positions(int i)
{
	if(this->tab_futures_purchase_struct[i].hedging==false)
		this->tab_futures_purchase_struct[i].short_existing_positions=0;
	else
	{
		if(this->tab_futures_purchase_struct[i].tab[Net_Delta_Pos][Ex_Delta_Fut_Pos]==1)
			this->tab_futures_purchase_struct[i].short_existing_positions=1;
		else if(this->tab_futures_purchase_struct[i].tab[Net_Delta_Pos][Ex_Delta_Fut_Neg]==1)
			this->tab_futures_purchase_struct[i].short_existing_positions=0;
		else if(this->tab_futures_purchase_struct[i].tab[Net_Delta_Neg][Ex_Delta_Fut_Pos]==1)
			this->tab_futures_purchase_struct[i].short_existing_positions=0;
		else if (this->tab_futures_purchase_struct[i].tab[Net_Delta_Neg][Ex_Delta_Fut_Neg]==1)
			this->tab_futures_purchase_struct[i].short_existing_positions=1;

		if(i==0) this->tab_futures_purchase_struct[i].short_existing_positions=0;	
	}
}
void Future_Trade::init_contracts(int i)
{
	if(this->tab_futures_purchase_struct[i].hedging)
	{
		if (this->tab_futures_purchase_struct[i].short_existing_positions==1)
			//if(this->tab_futures_purchase_struct[i].new_net_delta!=0)
				this->tab_futures_purchase_struct[i].contracts=
				-this->tab_futures_purchase_struct[i].new_net_delta/
				this->tab_futures_purchase_struct[i].delta_asset;
			//else this->tab_futures_purchase_struct[i].contracts=0;
		else 
				this->tab_futures_purchase_struct[i].contracts=
				-this->tab_futures_purchase_struct[i].net_delta/
				this->tab_futures_purchase_struct[i].delta_asset;
	}

	
	else this->tab_futures_purchase_struct[i].contracts=0;
	

}
void Future_Trade::init_new_net_delta(int i)
{
	if(i==0) this->tab_futures_purchase_struct[i].new_net_delta=
		-this->tab_futures_purchase_struct[i].delta_liability;
	else
	this->tab_futures_purchase_struct[i].new_net_delta=
		this->objects_needed.delta_futures_in_portfolio_2->tab[i][i-1]
	+this->tab_futures_purchase_struct[i].net_delta-
		this->tab_futures_purchase_struct[i].ex_delta_futures;
		if(abs(this->tab_futures_purchase_struct[i].new_net_delta)<=0.000000001)
			this->tab_futures_purchase_struct[i].new_net_delta=0;

}
Future_Trade::~Future_Trade(void)
{
	
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		for(int j=0;j<Nb_Cols_In_Tab;++j)
		{
			delete[] tab_futures_purchase_struct[i].tab[j];
		}
		delete[] tab_futures_purchase_struct[i].tab;
	}	
	delete[] this->tab_futures_purchase_struct;
}
