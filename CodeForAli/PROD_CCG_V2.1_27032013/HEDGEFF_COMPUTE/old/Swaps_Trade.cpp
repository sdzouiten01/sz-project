#include "Assumptions.h"
#include "Rho_Asset.h"
#include "Hedgeff.h"
#include "Existing_Rho_Swaps.h"
#include "Existing_Rho.h"
#include "Rho_Swaps_In_Portfolio.h"
#include "Rho_Swaps_In_Portfolio_2.h"
#include "Swaps_Trade.h"
#include "Rho_Puts.h"

//Swaps_Trade::Swaps_Trade(Assumptions* assumptions, Rho_Asset* rho_asset, 
//							   Hedgeff* hedgeff,Existing_Rho_Swaps* existing_rho,
//							   Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio)
//{
//	this->objects_needed.assumptions=assumptions;
//	this->objects_needed.rho_asset=rho_asset;
//	this->objects_needed.hedgeff=hedgeff;
//	this->objects_needed.existing_rho=existing_rho;
//	this->objects_needed.rho_swaps_in_portfolio=rho_swaps_in_portfolio;
//}
Swaps_Trade::Swaps_Trade(int swap_no,Assumptions* assumptions,Rho_Asset* rho_asset,
		Hedgeff* hedgeff)
{
	//debug.open("Debug_Fut_Pursh.txt");
	this->swap_no=swap_no;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rho_asset=rho_asset;
	this->objects_needed.hedgeff=hedgeff;
	this->tab_swaps_trade_struct=new Swaps_Trade_Struct[this->objects_needed.assumptions->model_period];
	this->rho_bounds.liab_rho_lower_bound=0;
	this->rho_bounds.liab_rho_upper_bound=0;
	this->rho_bounds.net_rho_lower_bound=0;
	this->rho_bounds.net_rho_upper_bound=0;
	
	//this->characteristics.length_of_swaps_contract

}
void Swaps_Trade::init_delta_bounds()
{
	this->rho_bounds.liab_rho_lower_bound=0;
	this->rho_bounds.liab_rho_upper_bound=0;
	this->rho_bounds.net_rho_lower_bound=0;
	this->rho_bounds.net_rho_upper_bound=0;
}
void Swaps_Trade::init_characteristics()
{
	this->characteristics.length_of_swaps_contract=
		this->objects_needed.assumptions->length_of_swaps_contract;
	this->characteristics.sell_at_time_to_expiry=
		this->objects_needed.assumptions->sell_at_time_to_expiry;
	//this->characteristics.shock_size=
	//	this->objects_needed.assumptions->shock_size;
}
void Swaps_Trade::init_tab_swaps_trade_struct(int i)
{
	/*for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{*/
		init_time(i);
		init_hedging(i);	
		
		init_ex_rho(i);
		init_ex_rho_swaps(i);
		
		init_rho_asset(i);
		init_rho_liability(i);
		init_net_rho(i);
//		init_req_net_delta(i);
//		init_req_delta_liability(i);
//		init_new_net_rho(i);
		init_tab(i);
		init_short_existing_positions(i);
	//	init_contracts(i);
	/*}*/
}
void Swaps_Trade::init_time(int i)
{
	tab_swaps_trade_struct[i].time=i;
}
void Swaps_Trade::init_hedging(int i)
{
	if(!this->objects_needed.assumptions->hedge_rho||
		tab_swaps_trade_struct[i].time%(int)this->objects_needed.assumptions->reb_freq_rho!=0)
	{
		this->tab_swaps_trade_struct[i].hedging=false;
	}
	else this->tab_swaps_trade_struct[i].hedging=true;
}
void Swaps_Trade::init_tab(int i)
{
	this->tab_swaps_trade_struct[i].tab=new int*[Nb_Rows_In_Tab];
	for(int j=0;j<Nb_Cols_In_Tab;++j)
	{
		this->tab_swaps_trade_struct[i].tab[j]=new int[Nb_Cols_In_Tab];
	}
	for(int k=0;k<Nb_Rows_In_Tab;++k)
	{
		for(int j=0;j<Nb_Cols_In_Tab;++j)
		{
		this->tab_swaps_trade_struct[i].tab[k][j]=0;
		}
	}
	debug<<this->tab_swaps_trade_struct[i].ex_rho_swaps<<endl;
	if((this->tab_swaps_trade_struct[i].net_rho>=0)&&
		(this->tab_swaps_trade_struct[i].ex_rho_swaps>=0))
		this->tab_swaps_trade_struct[i].tab[Net_Rho_Pos][Ex_Rho_Swp_Pos]=1;

	else if((this->tab_swaps_trade_struct[i].net_rho>=0)&&
		(this->tab_swaps_trade_struct[i].ex_rho_swaps<=0))
		this->tab_swaps_trade_struct[i].tab[Net_Rho_Pos][Ex_Rho_Swp_Neg]=1;

	else if((this->tab_swaps_trade_struct[i].net_rho<=0)&&(
		this->tab_swaps_trade_struct[i].ex_rho_swaps>=0))
		this->tab_swaps_trade_struct[i].tab[Net_Rho_Neg][Ex_Rho_Swp_Pos]=1;

	else if((this->tab_swaps_trade_struct[i].net_rho<=0)&&(
		this->tab_swaps_trade_struct[i].ex_rho_swaps<=0))
		this->tab_swaps_trade_struct[i].tab[Net_Rho_Neg][Ex_Rho_Swp_Neg]=1;


}



void Swaps_Trade::init_ex_rho(int i)
{
	/*if(this->tab_swaps_trade_struct[i].time==0) this->tab_swaps_trade_struct[i].ex_rho=0;
	else this->tab_swaps_trade_struct[i].ex_rho=
		this->objects_needed.existing_rho->return_existing_sv(i-1);*/
	this->tab_swaps_trade_struct[i].ex_rho=
		this->objects_needed.existing_rho->return_existing_sv(i-1);
}
void Swaps_Trade::init_ex_rho_swaps(int i)
{
	/*if(this->tab_swaps_trade_struct[i].time==0) this->tab_swaps_trade_struct[i].ex_rho_swaps=0;
	else this->tab_swaps_trade_struct[i].ex_rho_swaps=
		this->objects_needed.existing_delta_futures->return_existing_sv(i-1);*/
	this->tab_swaps_trade_struct[i].ex_rho_swaps=
		this->objects_needed.existing_rho_swaps->return_existing_sv(i-1);
}

void Swaps_Trade::init_rho_asset(int i)
{
	this->tab_swaps_trade_struct[i].rho_asset=
		this->objects_needed.rho_asset->tab[i];
}
void Swaps_Trade::init_rho_liability(int i)
{
	/*this->tab_swaps_trade_struct[i].rho_liability=
		this->objects_needed.hedgeff->rho_liab[i];*/
	this->tab_swaps_trade_struct[i].rho_liability=this->objects_needed.hedgeff->rho_liab[this->swap_no][i];
}
void Swaps_Trade::init_net_rho(int i)
{
	this->tab_swaps_trade_struct[i].net_rho=
		this->tab_swaps_trade_struct[i].ex_rho-
		this->tab_swaps_trade_struct[i].rho_liability;
}
void Swaps_Trade::init_short_existing_positions(int i)
{
	if(this->tab_swaps_trade_struct[i].hedging==false)
		this->tab_swaps_trade_struct[i].short_existing_positions=0;
	else
	{
		if(this->tab_swaps_trade_struct[i].tab[Net_Rho_Pos][Ex_Rho_Swp_Pos]==1)
			this->tab_swaps_trade_struct[i].short_existing_positions=1;
		else if(this->tab_swaps_trade_struct[i].tab[Net_Rho_Pos][Ex_Rho_Swp_Neg]==1)
			this->tab_swaps_trade_struct[i].short_existing_positions=0;
		else if(this->tab_swaps_trade_struct[i].tab[Net_Rho_Neg][Ex_Rho_Swp_Pos]==1)
			this->tab_swaps_trade_struct[i].short_existing_positions=0;
		else if (this->tab_swaps_trade_struct[i].tab[Net_Rho_Neg][Ex_Rho_Swp_Neg]==1)
			this->tab_swaps_trade_struct[i].short_existing_positions=1;

		if(i==0) this->tab_swaps_trade_struct[i].short_existing_positions=0;	
	}
}
void Swaps_Trade::init_contracts(int i)
{
	if(this->tab_swaps_trade_struct[i].hedging)
	{
		if (this->tab_swaps_trade_struct[i].short_existing_positions==1)
			//if(this->tab_swaps_trade_struct[i].new_net_rho!=0)
				this->tab_swaps_trade_struct[i].contracts=
				-this->tab_swaps_trade_struct[i].new_net_rho/
				this->tab_swaps_trade_struct[i].rho_asset;
			//else this->tab_swaps_trade_struct[i].contracts=0;
		else 
				this->tab_swaps_trade_struct[i].contracts=
				-this->tab_swaps_trade_struct[i].net_rho/
				this->tab_swaps_trade_struct[i].rho_asset;
	}

	
	else this->tab_swaps_trade_struct[i].contracts=0;
	

}
void Swaps_Trade::init_new_net_rho(int i)
{
	if(i==0) this->tab_swaps_trade_struct[i].new_net_rho=
		-this->tab_swaps_trade_struct[i].rho_liability;
	else
	this->tab_swaps_trade_struct[i].new_net_rho=
		this->objects_needed.rho_swaps_in_portfolio_2->tab[i][i-1]
	+this->tab_swaps_trade_struct[i].net_rho-
		this->tab_swaps_trade_struct[i].ex_rho_swaps;
		if(abs(this->tab_swaps_trade_struct[i].new_net_rho)<=0.000000001)
			this->tab_swaps_trade_struct[i].new_net_rho=0;

}
Swaps_Trade::~Swaps_Trade(void)
{
	
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		for(int j=0;j<Nb_Cols_In_Tab;++j)
		{
			delete[] tab_swaps_trade_struct[i].tab[j];
		}
		delete[] tab_swaps_trade_struct[i].tab;
	}	
	delete[] this->tab_swaps_trade_struct;
}
