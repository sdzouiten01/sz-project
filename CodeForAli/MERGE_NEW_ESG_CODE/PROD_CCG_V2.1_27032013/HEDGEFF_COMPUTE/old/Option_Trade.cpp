#include "Assumptions.h"
#include "Gamma_Asset.h"
#include "Hedgeff.h"
#include "Existing_Gamma.h"
#include "Gamma_Puts_In_Portfolio.h"
#include "Gamma_Puts_In_Portfolio_2.h"
#include "Option_Trade.h"

//Option_Trade::Option_Trade(Assumptions* assumptions, Gamma_Asset* gamma_asset, 
//							   Hedgeff* hedgeff,Existing_Gamma* existing_gamma,
//							   Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio)
//{
//	this->objects_needed.assumptions=assumptions;
//	this->objects_needed.gamma_asset=gamma_asset;
//	this->objects_needed.hedgeff=hedgeff;
//	this->objects_needed.existing_gamma=existing_gamma;
//	this->objects_needed.gamma_puts_in_portfolio=gamma_puts_in_portfolio;
//}
Option_Trade::Option_Trade(Assumptions* assumptions,Gamma_Asset* gamma_asset,
		Hedgeff* hedgeff)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.gamma_asset=gamma_asset;
	this->objects_needed.hedgeff=hedgeff;
	this->tab_option_trade_struct=new Option_Trade_Struct[this->objects_needed.assumptions->model_period];
	this->gamma_bounds.liab_gamma_lower_bound=0;
	this->gamma_bounds.liab_gamma_upper_bound=0;
	this->gamma_bounds.net_gamma_lower_bound=this->objects_needed.assumptions->net_gamma_lower_bound;
	this->gamma_bounds.net_gamma_upper_bound=this->objects_needed.assumptions->net_gamma_upper_bound;
	//this->characteristics.length_of_option_contract

}
void Option_Trade::init_gamma_bounds()
{
	this->gamma_bounds.liab_gamma_lower_bound=0;
	this->gamma_bounds.liab_gamma_upper_bound=0;
	this->gamma_bounds.net_gamma_lower_bound=0;
	this->gamma_bounds.net_gamma_upper_bound=0;
}
//void Option_Trade::init_characteristics()
//{
//	this->characteristics.length_of_option_contract=
//		this->objects_needed.assumptions->length_of_option_contract;
//	this->characteristics.sell_at_time_to_expiry=
//		this->objects_needed.assumptions->sell_at_time_to_expiry;
//	//this->characteristics.shock_size=
//	//	this->objects_needed.assumptions->shock_size;
//}
void Option_Trade::init_tab_asset_purchase_struct(int i)
{
	/*for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{*/
		init_time(i);
				
		init_ex_gamma(i);
		init_gamma_put(i);
		init_gamma_liability(i);
		init_net_gamma(i);
		init_hedging(i);
//		init_req_net_gamma(i);
//		init_req_gamma_liability(i);
		init_new_net_gamma(i);
//		init_tab(i);
		init_short_existing_positions(i);
		init_contracts(i);
	/*}*/
}
void Option_Trade::init_time(int i)
{
	tab_option_trade_struct[i].time=i;
}
void Option_Trade::init_hedging(int i)
{
	if(!this->objects_needed.assumptions->hedge_gamma||
		tab_option_trade_struct[i].time%(int)this->objects_needed.assumptions->reb_freq_gamma!=0)
	{
		this->tab_option_trade_struct[i].hedging=false;
	}
	else if(this->tab_option_trade_struct[i].net_gamma<=
		this->gamma_bounds.net_gamma_upper_bound*abs(this->tab_option_trade_struct[i].gamma_liability) &&
		this->tab_option_trade_struct[i].net_gamma>=
		this->gamma_bounds.net_gamma_lower_bound*abs(this->tab_option_trade_struct[i].gamma_liability))
		this->tab_option_trade_struct[i].hedging=false;
	else
		this->tab_option_trade_struct[i].hedging=true;
}



void Option_Trade::init_ex_gamma(int i)
{
	if(this->tab_option_trade_struct[i].time==0) this->tab_option_trade_struct[i].ex_gamma=0;
	else this->tab_option_trade_struct[i].ex_gamma=
		this->objects_needed.existing_gamma->existing_ov[i-1];
}

void Option_Trade::init_gamma_put(int i)
{
	this->tab_option_trade_struct[i].gamma_put=
		this->objects_needed.gamma_asset->tab_of_gamma_asset_struct[i].gamma;
}
void Option_Trade::init_gamma_liability(int i)
{
	this->tab_option_trade_struct[i].gamma_liability=
		this->objects_needed.hedgeff->gamma_liab[i];
}
void Option_Trade::init_net_gamma(int i)
{
	this->tab_option_trade_struct[i].net_gamma=
		this->tab_option_trade_struct[i].ex_gamma-
		this->tab_option_trade_struct[i].gamma_liability;
}


void Option_Trade::init_short_existing_positions(int i)
{
	if(this->tab_option_trade_struct[i].hedging==false)
		this->tab_option_trade_struct[i].short_existing_positions=0;
	else
	{
	
		if (this->tab_option_trade_struct[i].net_gamma/
			this->tab_option_trade_struct[i].gamma_liability>
			this->gamma_bounds.net_gamma_upper_bound&&
			this->tab_option_trade_struct[i].ex_gamma>0)

			this->tab_option_trade_struct[i].short_existing_positions=1;


		else if (this->tab_option_trade_struct[i].net_gamma/
			this->tab_option_trade_struct[i].gamma_liability>
			this->gamma_bounds.net_gamma_upper_bound&&
			this->tab_option_trade_struct[i].ex_gamma<0)

			this->tab_option_trade_struct[i].short_existing_positions=0;


		else if (this->tab_option_trade_struct[i].net_gamma/
			this->tab_option_trade_struct[i].gamma_liability<
			this->gamma_bounds.net_gamma_lower_bound&&
			this->tab_option_trade_struct[i].ex_gamma>0)

			this->tab_option_trade_struct[i].short_existing_positions=0;


		else if (this->tab_option_trade_struct[i].net_gamma/
			this->tab_option_trade_struct[i].gamma_liability<
			this->gamma_bounds.net_gamma_lower_bound&&	
			this->tab_option_trade_struct[i].ex_gamma<0)

			this->tab_option_trade_struct[i].short_existing_positions=1;

		if(i==0) this->tab_option_trade_struct[i].short_existing_positions=0;	

	}
}

void Option_Trade::init_contracts(int i)
{
	if(this->tab_option_trade_struct[i].hedging)
	{

		if (this->tab_option_trade_struct[i].short_existing_positions==1)
			//if(this->tab_futures_purchase_struct[i].new_net_delta!=0)
				this->tab_option_trade_struct[i].contracts=
				-this->tab_option_trade_struct[i].new_net_gamma/
				this->tab_option_trade_struct[i].gamma_put;
			//else this->tab_futures_purchase_struct[i].contracts=0;
		else 
				this->tab_option_trade_struct[i].contracts=
				-this->tab_option_trade_struct[i].net_gamma/
				this->tab_option_trade_struct[i].gamma_put;
	}
	else this->tab_option_trade_struct[i].contracts=0;
}
void Option_Trade::init_new_net_gamma(int i)
{
	if(i==0) this->tab_option_trade_struct[i].new_net_gamma=
		-this->tab_option_trade_struct[i].gamma_liability;
	else
	this->tab_option_trade_struct[i].new_net_gamma=
		this->objects_needed.gamma_puts_in_portfolio_2->tab[i][i-1]
		-this->tab_option_trade_struct[i].gamma_liability;
		if(abs(this->tab_option_trade_struct[i].new_net_gamma)<=0.000000001)
			this->tab_option_trade_struct[i].new_net_gamma=0;

}
Option_Trade::~Option_Trade(void)
{
	delete[] this->tab_option_trade_struct;
}
