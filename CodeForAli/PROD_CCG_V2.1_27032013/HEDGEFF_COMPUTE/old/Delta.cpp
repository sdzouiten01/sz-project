#include "Delta.h"
#include "Rf.h"
#include "Delta_In_Portfolio.h"
#include "Hedgeff.h"
#include "Assumptions.h"
#include "Option_Trade.h"
#include "Future_Trade.h"
#include "Nbr_Futures.h"
#include "Market_Value_Futures.h"
#include "Trading_Futures.h"
#include "Math_Tools.h"
#include "New_Futures_Notional.h"
Delta::Delta(Delta_In_Portfolio* delta_in_portfolio,Hedgeff* hedgeff,Assumptions* assumptions,
			 Option_Trade* option_trade,Future_Trade* future_trade,Nbr_Futures* nbr_futures,Rf* rf,
			 Market_Value_Futures* market_value_futures,Trading_Futures* trading_futures,New_Futures_Notional* new_futures_notional)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.hedgeff=hedgeff;
	this->objects_needed.delta_in_portfolio=delta_in_portfolio;
	this->objects_needed.option_trade=option_trade;
	this->objects_needed.future_trade=future_trade;
	this->objects_needed.nbr_futures=nbr_futures;
	this->objects_needed.market_value_futures=market_value_futures;
	this->objects_needed.rf=rf;
	this->objects_needed.trading_futures=trading_futures;
	this->objects_needed.new_futures_notional=new_futures_notional;
	init_index();
	init_int_rate();
	init_delta_liab();
	init_asset_delta_before_futures();
	init_asset_delta_after_futures();
	init_net_delta_before_futures();
	init_net_delta_after_futures();
//	init_delta_new_eq_fut1();
	init_mkt_value();
	init_future_pursh_sales();
	init_future_pursh();
	init_future_sales();
	init_eq_fut_1_ct();
	init_total_notional();
	init_int_cont();
	init_div_cont();
	init_settle_cont();
	init_pnl_eq_futures();
	init_trans_costs();
}
void Delta::init_index()
{
	this->inputs.index=new double[this->objects_needed.assumptions->model_period];
	this->inputs.index=this->objects_needed.hedgeff->index;
}
void Delta::init_int_rate()
{
	this->int_rate=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->int_rate[i]=this->objects_needed.rf->tab[i][i];
	}
	
}
void Delta::init_future_pursh_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->future_pursh_sales=new double[size];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->future_pursh_sales[i]=this->objects_needed.trading_futures->trading[i];
	}
}
void Delta::init_future_pursh()
{
	int size=this->objects_needed.assumptions->model_period;
	this->future_pursh=new double[size];
	for(int i=0;i<size;++i)
	{
		this->future_pursh[i]=
			Math_Tools::maxim(this->future_pursh_sales[i],0);
	}

}
void Delta::init_future_sales()
{
	int size=this->objects_needed.assumptions->model_period;
	this->future_sales=new double[size];
	for(int i=0;i<size;++i)
	{
		this->future_sales[i]=
			-Math_Tools::minim(this->future_pursh_sales[i],0);
	}
}
void Delta::init_mkt_value()
{
	int size=this->objects_needed.assumptions->model_period;
	this->mkt_value=new double[size];
	for(int i=0;i<size;++i)
	{
		this->mkt_value[i]=this->objects_needed.market_value_futures->mkt_val[i];
	}
}
void Delta::init_delta_liab()
{
	this->liability_delta.delta_liab=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->liability_delta.delta_liab[i]=this->objects_needed.hedgeff->delta_liab[i];
	}
}
void Delta::init_asset_delta_after_futures()
{
	this->asset_delta.asset_delta_after_futures=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_delta.asset_delta_after_futures[i]=this->objects_needed.delta_in_portfolio->asset_delta[i];
	}
}
void Delta::init_net_delta_after_futures()
{
	this->net_delta_after_futures=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_delta_after_futures[i]=this->asset_delta.asset_delta_after_futures[i]-this->liability_delta.delta_liab[i];
	}	
}
void Delta::init_asset_delta_before_futures()
{
	this->asset_delta.asset_delta_before_futures=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_delta.asset_delta_before_futures[i]=
			this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta+
			this->liability_delta.delta_liab[i];
	}
}
void Delta::init_net_delta_before_futures()
{
	this->net_delta_before_futures=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->net_delta_before_futures[i]=this->asset_delta.asset_delta_before_futures[i]-this->liability_delta.delta_liab[i];
	}	
}
//void Delta::init_delta_new_eq_fut1()
//{
//	this->future_delta.delta_new_eq_fut_1=new double[this->objects_needed.assumptions->model_period];
//	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
//	{
//		this->future_delta.delta_new_eq_fut_1[i]=exp(this->int_rate[i]);
//	}
//}
void Delta::init_eq_fut_1_ct()
{
	this->nb_of_futures.eq_fut_1_ct=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->nb_of_futures.eq_fut_1_ct[i]=this->objects_needed.nbr_futures->numbers_of_futures[i];
	}
}
void Delta::init_total_notional()
{
	this->total_notional=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->total_notional[i]=this->nb_of_futures.eq_fut_1_ct[i]*this->inputs.index[i];
	}
}
void Delta::init_int_cont()
{
	this->int_cont=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->int_cont[i]=-this->total_notional[i]*this->int_rate[i];
	}
}
void Delta::init_div_cont()
{
	this->div_cont=new double[this->objects_needed.assumptions->model_period];
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->div_cont[i]=0;
	}
}
void Delta::init_settle_cont()
{
	this->settle_cont=new double[this->objects_needed.assumptions->model_period];
	this->settle_cont[0]=this->objects_needed.assumptions->delta_per_cont*
		this->int_rate[0]*(this->inputs.index[1]-this->inputs.index[0]);
	for (int i=1;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->settle_cont[i]=this->objects_needed.assumptions->delta_per_cont*
			this->nb_of_futures.eq_fut_1_ct[i-1]*(
			this->inputs.index[i]-this->inputs.index[i-1]);
	}
}
void Delta::init_pnl_eq_futures()
{
	int size=this->objects_needed.assumptions->model_period;
	this->pnl_eq_fut=new double[size];
	this->total_pnl=0;
	for(int i=0;i<size;++i)
	{
		if(i==0)
			this->pnl_eq_fut[i]=
				this->mkt_value[i]
				-this->future_pursh[i]
				+this->future_sales[i];
		else
			this->pnl_eq_fut[i]=
				this->mkt_value[i]
				-this->mkt_value[i-1]
				-this->future_pursh[i]
				+this->future_sales[i];
		
	}
	this->total_pnl=this->pnl_eq_fut[size-1];
}

void Delta::init_trans_costs()
{
	int size=this->objects_needed.assumptions->model_period;
	this->trans_cost=new double[size];
	this->trans_cost[0]=0;
	this->total_trans_costs=0;
	for(int i=0;i<size;++i)
	{
			this->trans_cost[i]=abs(this->objects_needed.new_futures_notional->fut_not[i]
			*this->objects_needed.assumptions->delta_transaction_cost/10000);
		this->total_trans_costs+=this->trans_cost[i];
	}
}
Delta::~Delta(void)
{
	delete [] this->inputs.index;
	//delete this->asset_delta;
	delete[] this->div_cont;
	//delete this->div_rate;
	delete [] this->int_rate;
	delete [] this->trans_cost;
	delete [] this->int_cont;
	delete [] this->net_delta_before_futures;
	delete [] this->net_delta_after_futures;
	delete [] this->pnl_eq_fut;
	delete [] this->settle_cont;
	delete [] this->total_notional;
	//chth
	delete [] this->nb_of_futures.eq_fut_1_ct;
	delete [] this->asset_delta.asset_delta_before_futures;
	delete [] this->asset_delta.asset_delta_after_futures;
	delete [] this->liability_delta.delta_liab;
	delete [] this->mkt_value;
	delete [] this->future_sales;
	delete [] this->future_pursh;
	delete [] this->future_pursh_sales;
//	delete [] this->inputs.index;
}
