#include "Rho.h"
#include "Rho_In_Portfolio.h"
#include "Hedgeff.h"
#include "Assumptions.h"
#include "Swaps_Trade.h"
#include "Nbr_Swaps.h"
#include "Market_Value_Swaps.h"
#include "Trading_Swaps.h"
#include "Math_Tools.h"
#include "New_Swaps_Notional.h"


Rho::Rho(int swap_no, Rho_In_Portfolio* rho_in_portfolio, Hedgeff* hedgeff, Assumptions* assumptions,
			Swaps_Trade* swaps_trade, Nbr_Swaps* nbr_swaps,
			 Market_Value_Swaps* market_value_swaps, Trading_Swaps* trading_swaps, New_Swaps_Notional* new_swaps_notional)
{

	this->objects_needed.assumptions=assumptions;
	this->objects_needed.hedgeff=hedgeff;
	this->objects_needed.rho_in_portfolio = rho_in_portfolio;

	this->objects_needed.swaps_trade=swaps_trade;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->objects_needed.market_value_swaps=market_value_swaps;
	this->objects_needed.trading_swaps=trading_swaps;
	this->objects_needed.new_swaps_notional=new_swaps_notional;
	this->swap_no=swap_no;
	init_rho_liab();
	init_asset_rho_before_swaps();
	init_asset_rho_after_swaps();
	init_net_rho_before_futures();
	init_net_rho_after_swaps();
	init_mkt_value();
	init_swap_pursh_sales();
	init_swap_pursh();
	init_swap_sales();
	init_rt_swp_1_ct();
	init_total_notional();
	init_pnl_rt_swaps();
	init_trans_costs();
}


void Rho::init_swap_pursh_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->swap_pursh_sales=new double[size];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->swap_pursh_sales[i]=this->objects_needed.trading_swaps->trading[i];
	}
}
void Rho::init_swap_pursh()
{
	int size=this->objects_needed.assumptions->model_period;
	this->swap_pursh=new double[size];
	for(int i=0;i<size;++i)
	{
		this->swap_pursh[i]=
			Math_Tools::maxim(this->swap_pursh_sales[i],0);
	}

}
void Rho::init_swap_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->swap_sales=new double[size];
	for(int i=0;i<size;++i)
	{
		this->swap_sales[i]=
			-Math_Tools::minim(this->swap_pursh_sales[i],0);
	}
}
void Rho::init_mkt_value()
{
	int size=this->objects_needed.assumptions->model_period;
	this->mkt_value=new double[size];
	for(int i=0;i<size;++i)
	{
		this->mkt_value[i]=this->objects_needed.market_value_swaps->mkt_val[i];
	}
}
void Rho::init_rho_liab()
{
	this->liability_rho.rho_liab=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->liability_rho.rho_liab[i]=this->objects_needed.hedgeff->rho_liab[swap_no][i];
	}
}
void Rho::init_asset_rho_after_swaps()
{
	this->asset_rho.asset_rho_after_swaps=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_rho.asset_rho_after_swaps[i]=this->objects_needed.rho_in_portfolio->asset_rho[i];
	}
}
void Rho::init_net_rho_after_swaps()
{
	this->net_rho_after_swaps=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_rho_after_swaps[i]=this->asset_rho.asset_rho_after_swaps[i]-this->liability_rho.rho_liab[i];
	}	
}
void Rho::init_asset_rho_before_swaps()
{
	this->asset_rho.asset_rho_before_swaps=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_rho.asset_rho_before_swaps[i]=
			this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho+
			this->liability_rho.rho_liab[i];
	}
}
void Rho::init_net_rho_before_futures()
{
	this->net_rho_before_swaps=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_rho_before_swaps[i]=this->asset_rho.asset_rho_before_swaps[i]-this->liability_rho.rho_liab[i];
	}	
}
//void Rho::init_delta_new_eq_fut1()
//{
//	this->swap_rho.delta_new_eq_fut_1=new double[this->objects_needed.assumptions->model_period];
//	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
//	{
//		this->swap_rho.delta_new_eq_fut_1[i]=exp(this->int_rate[i]);
//	}
//}
void Rho::init_rt_swp_1_ct()
{
	this->nb_of_swaps.rt_swp_1_ct=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->nb_of_swaps.rt_swp_1_ct[i]=this->objects_needed.nbr_swaps->numbers_of_swaps[i];
	}
}
void Rho::init_total_notional()
{
	this->total_notional=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->total_notional[i]=this->nb_of_swaps.rt_swp_1_ct[i]*1;
	}
}
void Rho::init_int_cont()
{
	this->int_cont=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->int_cont[i]=-this->total_notional[i]*this->int_rate[i];
	}
}
void Rho::init_div_cont()
{
	this->div_cont=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->div_cont[i]=0;
	}
}
void Rho::init_settle_cont()
{
	//this->settle_cont=new double[this->objects_needed.assumptions->model_period];
	//this->settle_cont[0]=this->objects_needed.assumptions->delta_per_cont*
	//	this->int_rate[0]*(this->inputs.index[1]-this->inputs.index[0]);
	//for (int i=1;i<this->objects_needed.assumptions->model_period;++i)
	//{
	//	this->settle_cont[i]=this->objects_needed.assumptions->delta_per_cont*
	//		this->nb_of_swaps.rt_swp_1_ct[i-1]*(
	//		this->inputs.index[i]-this->inputs.index[i-1]);
	//}
}
void Rho::init_pnl_rt_swaps()
{
	int size=this->objects_needed.assumptions->model_period;
	this->pnl_rt_swp=new double[size];
	this->total_pnl=0;
	for(int i=0;i<size;++i)
	{
		if(i==0)
			this->pnl_rt_swp[i]=
				this->mkt_value[i]
				-this->swap_pursh[i]
				+this->swap_sales[i];
		else
			this->pnl_rt_swp[i]=
				this->mkt_value[i]
				-this->mkt_value[i-1]
				-this->swap_pursh[i]
				+this->swap_sales[i];
		
	}
	this->total_pnl=this->pnl_rt_swp[size-1];
}

void Rho::init_trans_costs()
{
	int size=this->objects_needed.assumptions->model_period;
	this->trans_cost=new double[size];
	this->trans_cost[0]=0;
	this->total_trans_costs=0;
	for(int i=0;i<size;++i)
	{
			this->trans_cost[i]=abs(this->objects_needed.new_swaps_notional->swap_not[i]
			*this->objects_needed.assumptions->rho_transaction_cost/10000*(double)this->objects_needed.assumptions->maturities_to_hedge[this->swap_no]/objects_needed.assumptions->nb_steps_per_year);
		this->total_trans_costs+=this->trans_cost[i];
	}
}
Rho::~Rho(void)
{
	delete this->asset_rho.asset_rho_after_swaps;
	delete this->asset_rho.asset_rho_before_swaps;


	delete this->trans_cost;
	delete this->nb_of_swaps.rt_swp_1_ct;
	delete this->liability_rho.rho_liab;
	delete this->mkt_value;
	delete this->swap_sales;
	delete this->swap_pursh;
	delete this->swap_pursh_sales;

	//delete[] this->div_cont;

	//delete this->div_rate;
	//delete [] this->int_rate;
	//delete [] this->int_cont;
	delete [] this->net_rho_before_swaps;
	delete [] this->net_rho_after_swaps;
	delete [] this->pnl_rt_swp;
	//delete [] this->settle_cont;
	delete [] this->total_notional;
}
