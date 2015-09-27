#include "Gamma.h"
#include "Hedgeff.h"
#include "Assumptions.h"
#include "Gamma_Asset.h"
#include "Market_Value.h"
#include "New_Option_Notional.h"
#include "Trading.h"
#include "Option_Notional.h"
#include "Nbr_Puts.h"
#include "Gamma_Puts_In_Portfolio_2.h"
#include "Option_Trade.h"

Gamma::Gamma(Hedgeff* hedgeff,Assumptions* assumptions,Gamma_Asset* gamma_asset,
		Option_Notional* option_notional,New_Option_Notional* new_option_notional,
		Trading* trading,Market_Value* market_value,Nbr_Puts* nbr_puts,Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2,Option_Trade* option_trade)
{
	this->objects_needed.hedgeff=hedgeff;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.gamma_asset=gamma_asset;
	this->objects_needed.option_notional=option_notional;
	this->objects_needed.new_option_notional=new_option_notional;
	this->objects_needed.trading=trading;
	this->objects_needed.market_value=market_value;
	this->objects_needed.nbr_puts=nbr_puts;
	this->objects_needed.gamma_puts_in_portfolio_2=gamma_puts_in_portfolio_2;
	this->objects_needed.option_trade=option_trade;
	this->init_fields();
}

Gamma::~Gamma(void)
{
	
	delete [] this->hand_calculate.mkt_value;
	delete [] this->hand_calculate.opt1_not;
	delete [] this->hand_calculate.option_pursh;
	delete [] this->hand_calculate.option_sales;
	delete [] this->hand_calculate.option_pursh_sales;
	delete [] this->hand_calculate.pnl_opts;
	//chth
	delete [] this->net_gamma_before_options;
	delete [] this->asset_gamma_before_options;
	delete [] this->asset_gamma_after_options;
	delete [] this->hand_calculate.trans_costs;
	delete [] this->hand_calculate.eq_put_1_ct;
	delete [] this->net_gamma_after_options;
}
void Gamma::init_fields()
{
//init_period();
 //init_gamma_1_lb();
 //init_gamma_new_asset();
 init_asset_gamma_before_options();
 init_asset_gamma_after_options();
 init_net_gamma_before_options();
 init_net_gamma_after_options();
//init_stock_return();
//init_underlying();
 init_opt1_not();
 init_eq_put_1_ct();
 init_option_pursh_sales();
 init_option_pursh();
 init_option_sales();
 init_mkt_value();
 init_pnl_opts();
 init_trans_costs();
}
//void Gamma::init_period()
//{
//	int size=this->objects_needed.assumptions->model_period;
//	this->hand_calculate.period=new double[size];
//	for(int i=0;i<size;++i)
//	{
//		this->hand_calculate.period[i]=i;
//	}
//}

//void Gamma::init_gamma_new_asset()
//{
//	int size=this->objects_needed.assumptions->model_period;
//	this->hand_calculate.gamma_new_asset=new double[size];
//	for(int i=0;i<size;++i)
//	{
//		this->hand_calculate.gamma_new_asset[i]=
//			this->objects_needed.gamma_asset->tab_of_gamma_asset_struct[i].gamma;
//	}
//}

//void Gamma::init_stock_return()
//{
//	int size=this->objects_needed.assumptions->model_period;
//	this->hand_calculate.stock_return=new double[size];
//	for(int i=1;i<size;++i)
//	{
//		this->hand_calculate.stock_return[i]=
//			this->objects_needed.hedgeff->index[i]/this->objects_needed.hedgeff->index[i-1]-1;
//	}
//}
//void Gamma::init_underlying()
//{
//	int size=this->objects_needed.assumptions->model_period;
//	this->hand_calculate.underlying=new double[size];
//	for(int i=1;i<size;++i)
//	{
//		this->hand_calculate.underlying[i]=
//			this->objects_needed.hedgeff->index[i-1]*(1+this->hand_calculate.stock_return[i]);
//	}
//}
void Gamma::init_eq_put_1_ct()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.eq_put_1_ct=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.eq_put_1_ct[i]=this->objects_needed.nbr_puts->numbers_of_puts[i];
	}
}
void Gamma::init_opt1_not()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.opt1_not=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.opt1_not[i]=
			this->objects_needed.option_notional->opt_not[i];
	}
}
void Gamma::init_option_pursh_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.option_pursh_sales=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.option_pursh_sales[i]=this->objects_needed.trading->trading[i];
	}
}
void Gamma::init_option_pursh()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.option_pursh=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.option_pursh[i]=
			Math_Tools::maxim(this->hand_calculate.option_pursh_sales[i],0);
	}

}
void Gamma::init_option_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.option_sales=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.option_sales[i]=
			-Math_Tools::minim(this->hand_calculate.option_pursh_sales[i],0);
	}
}
void Gamma::init_mkt_value()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.mkt_value=new double[size];
	for(int i=0;i<size;++i)
	{
		this->hand_calculate.mkt_value[i]=this->objects_needed.market_value->mkt_val[i];
	}
}
void Gamma::init_pnl_opts()
{
	//ofstream plopt("pnlopts.txt");
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.pnl_opts=new double[size];
	total_pnl=0;
	for(int i=0;i<size;++i)
	{
		if(i==0)
			this->hand_calculate.pnl_opts[i]=
			this->hand_calculate.mkt_value[i]
			-this->hand_calculate.option_pursh[i]
			+this->hand_calculate.option_sales[i];
		else
			this->hand_calculate.pnl_opts[i]=
				this->hand_calculate.mkt_value[i]
			-this->hand_calculate.mkt_value[i-1]
			-this->hand_calculate.option_pursh[i]
			+this->hand_calculate.option_sales[i];
		
	}
	total_pnl=this->hand_calculate.pnl_opts[size-1];
	
}
void Gamma::init_trans_costs()
{
	int size=this->objects_needed.assumptions->model_period;
	this->hand_calculate.trans_costs=new double[size];
	this->hand_calculate.trans_costs[0]=0;
	this->total_trans_costs=0;
	for(int i=1;i<size;++i)
	{
		this->hand_calculate.trans_costs[i]=abs(
			this->objects_needed.new_option_notional->opt_not[i]*
			this->objects_needed.assumptions->option_transaction_cost/10000);	
		this->total_trans_costs+=this->hand_calculate.trans_costs[i];
	}
}

void Gamma::init_asset_gamma_after_options()
{
	this->asset_gamma_after_options=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_gamma_after_options[i]=this->objects_needed.gamma_puts_in_portfolio_2->tab[i][i];
	}
}
void Gamma::init_net_gamma_after_options()
{
	this->net_gamma_after_options=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_gamma_after_options[i]=this->asset_gamma_after_options[i]-this->objects_needed.hedgeff->gamma_liab[i];
	}	
}
void Gamma::init_asset_gamma_before_options()
{
	this->asset_gamma_before_options=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_gamma_before_options[i]=
			this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma+
			this->objects_needed.hedgeff->gamma_liab[i];
	}
}
void Gamma::init_net_gamma_before_options()
{
	this->net_gamma_before_options=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_gamma_before_options[i]=this->asset_gamma_before_options[i]-this->objects_needed.hedgeff->gamma_liab[i];
	}	
}