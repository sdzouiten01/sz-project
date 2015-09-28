
#include "Rho_Hedge.h"

Rho_Hedge::Rho_Hedge(int swap_no,Delta_Gamma_Hedge** tab_of_delta_gamma_hedge,int nb_index,int scen_no)
{
	this->tab_of_delta_gamma_hedge=tab_of_delta_gamma_hedge;
	this->assumptions=tab_of_delta_gamma_hedge[0]->assumptions;
	this->hedgeff=tab_of_delta_gamma_hedge[0]->hedgeff;
	this->rw_scn=tab_of_delta_gamma_hedge[0]->rw_scn;
	this->tte=tab_of_delta_gamma_hedge[0]->tte;
	this->tte_futures=tab_of_delta_gamma_hedge[0]->tte_futures;
	this->nb_index=nb_index;
	this->zero_coupon_rates=tab_of_delta_gamma_hedge[0]->zero_coupon_rates;
	this->scen_no=scen_no-1;
	/*Tte* tte;*./
	Tte_Futures* tte_futures;
	Und_Base* und_base;
	Underlying* underlying;
	Zero_Coupon_Rates* zero_coupon_rates;
	Nbr_Puts* nbr_puts;
	Nbr_Futures* nbr_futures;*/
	
	this->annual_swap_rates_shock_down=new Annual_Swap_Rates_Shock_Down(swap_no,rw_scn,assumptions);
	this->annual_swap_rates_shock_up=new Annual_Swap_Rates_Shock_Up(swap_no,rw_scn,assumptions);
	this->zero_coupon_rates_shock_down=new Zero_Coupon_Rates_Shock_Down(swap_no,annual_swap_rates_shock_down);
	this->zero_coupon_rates_shock_up=new Zero_Coupon_Rates_Shock_Up(swap_no,annual_swap_rates_shock_up);	
	this->rf_shock_down=new Rf_Shock_Down(swap_no,assumptions,zero_coupon_rates_shock_down,tte);
	this->rf_shock_up=new Rf_Shock_Up(swap_no,assumptions,zero_coupon_rates_shock_up,tte);
	this->rf_futures_shock_down=new Rf_Futures_Shock_Down(swap_no,assumptions,zero_coupon_rates_shock_down,tte_futures);
	this->rf_futures_shock_up=new Rf_Futures_Shock_Up(swap_no,assumptions,zero_coupon_rates_shock_up,tte_futures);


	this->underlying_glob=new Underlying*[this->nb_index];


	//for(int i=0;i<= this->nb_index; ++1)
	//{
	//		delete [] this->underlying_glob[i];
	//}

	this->und_base_glob=new Und_Base*[this->nb_index];
	this->rho_puts_glob=new Rho_Puts*[this->nb_index];
	this->rho_futures_glob=new Rho_Futures*[this->nb_index];
	this->nbr_futures_glob=new Nbr_Futures*[this->nb_index];
	this->nbr_puts_glob=new Nbr_Puts*[this->nb_index];
	for (int index=0;index<this->nb_index;++index)
	{
		this->underlying_glob[index]=tab_of_delta_gamma_hedge[index]->underlying;
		this->und_base_glob[index]=tab_of_delta_gamma_hedge[index]->und_base;
		this->nbr_futures_glob[index]=tab_of_delta_gamma_hedge[index]->nbr_futures;
		this->nbr_puts_glob[index]=tab_of_delta_gamma_hedge[index]->nbr_puts;
		this->rho_puts_glob[index]=new Rho_Puts(swap_no,assumptions,und_base_glob[index],underlying_glob[index],tte,rf_shock_up,rf_shock_down);
		this->rho_futures_glob[index]=new Rho_Futures(swap_no, assumptions,tte_futures,rf_futures_shock_up,rf_futures_shock_down,underlying_glob[index]);
	}
	//this->rho_puts=new Rho_Puts(swap_no,assumptions,und_base,underlying,tte,rf_shock_up,rf_shock_down);
	//this->rho_futures=new Rho_Futures(swap_no, assumptions,tte_futures,rf_futures_shock_up,rf_futures_shock_down,underlying);

	this->tte_swaps=new Tte_Swaps(swap_no,assumptions);
	this->rho_swaps=new Rho_Swaps(swap_no,assumptions,tte_swaps,zero_coupon_rates_shock_up,zero_coupon_rates_shock_down,zero_coupon_rates);
	this->rho_asset=new Rho_Asset(swap_no,rho_swaps);
	this->rho_swaps_in_portfolio=new Rho_Swaps_In_Portfolio(swap_no,assumptions,rho_swaps);
	this->rho_swaps_in_portfolio_2=new Rho_Swaps_In_Portfolio_2(swap_no,assumptions,rho_swaps);
	this->swaps_trade=new Swaps_Trade(swap_no,assumptions,rho_asset,hedgeff);
	this->existing_rho=new Existing_Rho(swap_no,assumptions,rho_swaps,rho_puts_glob,nbr_puts_glob,nbr_futures_glob,nbr_swaps,rho_futures_glob,this->nb_index);
	this->existing_rho_swaps=new Existing_Rho_Swaps(swap_no,assumptions,rho_swaps);
	//
	this->nbr_swaps=new Nbr_Swaps(swap_no,swaps_trade,rho_swaps_in_portfolio,rho_swaps_in_portfolio_2,existing_rho,existing_rho_swaps,assumptions,tte_swaps);
	this->sv_base=new Sv_Base(swap_no,assumptions,tte_swaps,zero_coupon_rates);

	this->trading_swaps=new Trading_Swaps(assumptions,sv_base,nbr_swaps);
	this->market_value_swaps= new Market_Value_Swaps(assumptions,sv_base,nbr_swaps);
	this->rho_in_portfolio=new Rho_In_Portfolio(assumptions,rho_puts_glob,nbr_puts_glob,nbr_futures_glob,rho_futures_glob,nbr_swaps,rho_swaps,this->nb_index);

	this->new_swaps_notional=new New_Swaps_Notional(assumptions,underlying,nbr_swaps,tte_swaps);
	this->rho=new Rho(swap_no,rho_in_portfolio,hedgeff,assumptions,swaps_trade,nbr_swaps,market_value_swaps,trading_swaps,new_swaps_notional);
}

Rho_Hedge::~Rho_Hedge(void)
{
	
		delete this->market_value_swaps;


	//chth
	delete [] this->underlying_glob;
	delete this->und_base_glob;
	for (int index=0;index<this->nb_index;++index)
	{
		delete  this->rho_puts_glob[index];
		delete  this->rho_futures_glob[index];
	}
	delete [] this->rho_puts_glob;
	delete  [] this->rho_futures_glob;
	delete this->nbr_futures_glob;
	delete this->nbr_puts_glob;
	delete this->annual_swap_rates_shock_down;
	delete  this->annual_swap_rates_shock_up;
	delete  this->zero_coupon_rates_shock_down;
	delete   this->zero_coupon_rates_shock_up;	
	delete this->rf_shock_down;
	delete  this->rf_shock_up;
	delete  this->rf_futures_shock_down;
	delete  this->rf_futures_shock_up;
	delete this->rho_asset;
	delete this->tte_swaps;

	delete this->rho_swaps;

	delete this->sv_base;

	delete this->rho_swaps_in_portfolio;
	delete this->rho_swaps_in_portfolio_2;
	delete this->swaps_trade;
	delete this->existing_rho;
	delete this->existing_rho_swaps;

	delete this->nbr_swaps;

	

	delete this->trading_swaps;

	delete this->rho_in_portfolio;
	delete this->new_swaps_notional;
	delete this->rho;

}
