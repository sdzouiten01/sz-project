#include "Delta_Gamma_Hedge.h"

void Delta_Gamma_Hedge::set_error(int error)
{
	this->error=error;
}
Delta_Gamma_Hedge::Delta_Gamma_Hedge(
			string hedgeff_path,
			string rw_scn_path,
			string outputs_path,
			int index_no,
			int scen_no,
			int model_period,
			double moneyness,
			int reb_freq_delta,
			int reb_freq_gamma,
			int reb_freq_rho,
			double volatility,
			int length_of_option_contract,
			int sell_at_time_to_expiry,
			int length_of_futures_contract,
			double gamma_per_cont,
			double delta_per_cont,
			double net_gamma_lower_bound,
			double net_gamma_upper_bound,
			double liab_gamma_lower_bound,
			double liab_gamma_upper_bound,
			double delta_transaction_cost,
			double option_transaction_cost,
			double rho_transaction_cost,
			bool  hedge_delta,
			bool  hedge_gamma,
			bool  hedge_rho,
			bool rates_freq_ann,
            bool sheduled_futures,
            int next_time_step_sheduled,
			int nb_steps_per_year,
			int nb_maturities_to_hedge,
			double* maturities_to_hedge,
			double* shocks_to_apply,
			double* compounding_frequencies,
			double index_base,

			//chth221209-2
			double* start_mty,
			double* end_mty
			)
{
	this->error=0;
	//string assumptions_path;
	//string hedgeff_path;
//	string rw_scn_path;
//	string outputs_path;
	//assumptions_path=inputs_path+"\\Assumptions.csv";
	//hedgeff_path=inputs_path+"\\Hedgeff.csv";
	//rw_scn_path=inputs_path+"\\RWSCN.csv";
	outputs_path=outputs_path;
	//string outputs_path="C:\\Documents and Settings\\l-cohen\\My Documents\\Visual Studio 2008\\Projects\\GMAB_ASSETS_PART\\Outputs\\";
		this->assumptions=
			new Assumptions(
					model_period,
					moneyness,
					reb_freq_delta,
					reb_freq_gamma,
					reb_freq_rho,
					volatility,
					length_of_option_contract,
					sell_at_time_to_expiry,
					length_of_futures_contract,
					gamma_per_cont,
					delta_per_cont,
					net_gamma_lower_bound,
					net_gamma_upper_bound,
					liab_gamma_lower_bound,
					liab_gamma_upper_bound,
					delta_transaction_cost,
					option_transaction_cost,
					rho_transaction_cost,
					hedge_delta,
					hedge_gamma,
					hedge_rho,
					rates_freq_ann,
					sheduled_futures,
					next_time_step_sheduled,
					nb_steps_per_year,
					nb_maturities_to_hedge,
					maturities_to_hedge,
					shocks_to_apply,
					compounding_frequencies,
					index_base,

					//chth2212909-2
					start_mty,
					end_mty
					);
	
	int start=0;
	int end=this->assumptions->model_period;

	this->hedgeff=new Hedgeff(assumptions,
		this->assumptions->model_period,
		hedgeff_path,
		index_no,
		scen_no,
		rw_scn_path);
	
	this->rw_scn=new Rw_Scn(this->assumptions,
		this->assumptions->model_period,
		rw_scn_path,
		scen_no);

	this->tte=new Tte(assumptions);

	this->annual_swap_rates=new Annual_Swap_Rates(rw_scn,assumptions);
	

	this->zero_coupon_rates=new Zero_Coupon_Rates(annual_swap_rates);
	
	this->rf=new Rf(assumptions,zero_coupon_rates,tte);
	

	

	this->underlying=new Underlying(hedgeff,assumptions);

	this->und_base=new Und_Base(underlying,assumptions);

	this->gamma_puts=new Gamma_Puts(assumptions, und_base,underlying,tte,rf);

	this->gamma_asset=new Gamma_Asset(assumptions, hedgeff,rf);

	this->gamma_puts_in_portfolio=new Gamma_Puts_In_Portfolio(assumptions,gamma_puts);
	this->gamma_puts_in_portfolio_2=new Gamma_Puts_In_Portfolio_2(assumptions,gamma_puts);

	this->option_trade=new Option_Trade(assumptions,gamma_asset,hedgeff);

	this->existing_gamma=new Existing_Gamma(assumptions,gamma_puts);
	this->nbr_puts=new Nbr_Puts(option_trade,gamma_puts_in_portfolio,gamma_puts_in_portfolio_2,existing_gamma,assumptions);

//	this->und_base=new Und_Base(underlying,assumptions);
	this->ov_base=new Ov_Base(assumptions,und_base,underlying,tte,rf);
	this->market_value=new Market_Value(assumptions,ov_base,nbr_puts);
	this->trading=new Trading(assumptions, ov_base,nbr_puts);

	this->option_notional=new Option_Notional(assumptions,underlying,nbr_puts);
	this->new_option_notional=new New_Option_Notional(assumptions,underlying,nbr_puts);

	this->delta_puts=new Delta_Puts(assumptions,und_base,underlying,tte,rf);

	this->tte_futures=new Tte_Futures(assumptions);


	this->rf_futures=new Rf_Futures(assumptions,zero_coupon_rates,tte_futures);

	this->delta_futures=new Delta_Futures(assumptions,tte_futures,rf_futures);
	this->delta_asset=new Delta_Asset(assumptions,hedgeff,rf_futures);

	this->delta_futures_in_portfolio=new Delta_Futures_In_Portfolio(assumptions,delta_futures);
	this->delta_futures_in_portfolio_2=new Delta_Futures_In_Portfolio_2(assumptions,delta_futures);
	this->future_trade=new Future_Trade(assumptions,delta_asset,hedgeff);
	this->existing_delta=new Existing_Delta(assumptions,delta_futures,delta_puts,nbr_puts);
	this->existing_delta_futures=new Existing_Delta_Futures(assumptions,delta_futures);




	this->nbr_futures=new Nbr_Futures(future_trade,delta_futures_in_portfolio,delta_futures_in_portfolio_2,existing_delta,existing_delta_futures,assumptions,tte_futures);

	this->delta_in_portfolio=new Delta_In_Portfolio(assumptions,delta_puts,nbr_puts,nbr_futures,delta_futures);
	this->fv_base=new Fv_Base(assumptions,underlying,tte_futures,rf_futures);
	this->market_value_futures=new Market_Value_Futures(assumptions,fv_base,nbr_futures);
	this->trading_futures=new Trading_Futures(assumptions,fv_base,nbr_futures);
	this->new_futures_notional=new New_Futures_Notional(assumptions,underlying,nbr_futures,tte_futures);


	this->gamma=new Gamma(hedgeff,assumptions,gamma_asset,option_notional,new_option_notional,trading,market_value,nbr_puts,gamma_puts_in_portfolio_2,option_trade);
	this->delta=new Delta(delta_in_portfolio,hedgeff,assumptions,option_trade,future_trade,nbr_futures,rf,market_value_futures,trading_futures,new_futures_notional);
	

}

void Delta_Gamma_Hedge::erase_delta()
{
//	delete [] delta->div_cont;
//	delete [] delta->int_rate;
	delete  this->delta;
	/*
	delete [] delta->trans_cost;
	delete [] delta->int_cont;
	delete [] delta->net_delta_before_futures;
	delete [] delta->net_delta_after_futures;
	delete [] delta->asset_delta.asset_delta_before_futures;
	delete [] delta->asset_delta.asset_delta_after_futures;
	delete [] delta->pnl_eq_fut;
	delete [] delta->settle_cont;
	delete [] delta->total_notional;
	delete [] delta->future_pursh;
	delete [] delta->future_pursh_sales;
	delete [] delta->future_sales;*/
}
void Delta_Gamma_Hedge::erase_gamma()
{
	delete  this->gamma;
	/*delete [] gamma->hand_calculate.mkt_value;
	delete [] gamma->hand_calculate.opt1_not;
	delete [] gamma->hand_calculate.option_pursh;
	delete [] gamma->hand_calculate.option_sales;
	delete [] gamma->hand_calculate.option_pursh_sales;
	delete [] gamma->hand_calculate.pnl_opts;*/

}
void Delta_Gamma_Hedge::erase_nbr_swaps()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] nbr_swaps->tab[i];
	}
	delete[] nbr_swaps->tab;
	delete[] nbr_swaps->numbers_of_swaps;*/
}
void Delta_Gamma_Hedge::erase_rho_asset()
{
//delete[] rho_asset->tab_of_rho_asset_struct;
}
void Delta_Gamma_Hedge::erase_rho_futures()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] rho_futures->tab[i];
	}
	delete[] rho_futures->tab;*/
}
void Delta_Gamma_Hedge::erase_rho_puts()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] rho_puts->tab[i];
	}
	delete[] rho_puts->tab;*/
}
void Delta_Gamma_Hedge::erase_rho_swaps()
{
	//for(int i=0;i<assumptions->model_period;++i)
	//{
	//	delete[] rho_swaps->tab[i];
	//}
	//delete[] rho_swaps->tab;
}
void Delta_Gamma_Hedge::erase_rho_swaps_in_portfolio()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] rho_swaps_in_portfolio->tab[i];
	}
	delete[] rho_swaps_in_portfolio->tab;*/
}
void Delta_Gamma_Hedge::erase_rho_swaps_in_portfolio_2()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] rho_swaps_in_portfolio_2->tab[i];
	}
	delete[] rho_swaps_in_portfolio_2->tab;*/
}
void Delta_Gamma_Hedge::erase_swaps_trade()
{
	/*for(int i=0;i<assumptions->model_period;++i)
	{
		for(int j=0;j<swaps_trade->Nb_Cols_In_Tab;++j)
		{
			delete[] swaps_trade->tab_swaps_trade_struct[i].tab[j];
		}
		delete[] swaps_trade->tab_swaps_trade_struct[i].tab;
	}	
	delete[] swaps_trade->tab_swaps_trade_struct;*/
}
void Delta_Gamma_Hedge::erase_trading_futures()
{
	for(int i=0;i< assumptions->model_period;++i)
	{
		delete[] trading_futures->tab[i];
	}
	delete[]  trading_futures->tab;
	delete[]  trading_futures->trading;
}
void Delta_Gamma_Hedge::erase_market_value_futures()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] market_value_futures->tab[i];
	}
	delete[] market_value_futures->tab;
	delete[] market_value_futures->mkt_val;
}

void Delta_Gamma_Hedge::erase_fv_base()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] fv_base->tab[i];
	}
	delete[] fv_base->tab;
}
void Delta_Gamma_Hedge::erase_delta_in_portfolio()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] delta_in_portfolio->tab[i];
	}
	delete[] delta_in_portfolio->tab;
	delete[] delta_in_portfolio->asset_delta;	
}
void Delta_Gamma_Hedge::erase_nbr_futures()
{
	//cout << "we are in the destructor of tab, but we will output tab value " << endl;
	//for(int i=0;i<assumptions->model_period;++i)
	//{
	//	for(int j=0; j<142; j++)
	//	cout << nbr_futures->tab[i][j] << "	 ";
	//}
	//cout << endl;

	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] nbr_futures->tab[i];
	}
	delete[] nbr_futures->tab;    //! YY This line cause : ERROR ! 
	
	delete[] nbr_futures->numbers_of_futures;
}
void Delta_Gamma_Hedge::erase_existing_delta_futures()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] existing_delta_futures->tab[i];
	}
	delete[] existing_delta_futures->tab;
	delete[] existing_delta_futures->existing_fv;
}
void Delta_Gamma_Hedge::erase_existing_delta()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] existing_delta->tab[i];
	}
	delete[] existing_delta->tab;
	delete[] existing_delta->existing_fv;	

}
void Delta_Gamma_Hedge::erase_future_trade()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		for(int j=0;j<future_trade->Nb_Cols_In_Tab;++j)
		{
			delete[] future_trade->tab_futures_purchase_struct[i].tab[j];
		}
		delete[] future_trade->tab_futures_purchase_struct[i].tab;
	}	
	delete[] future_trade->tab_futures_purchase_struct;
}
void Delta_Gamma_Hedge::erase_delta_futures_in_portfolio_2()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] delta_futures_in_portfolio_2->tab[i];
	}
	delete[] delta_futures_in_portfolio_2->tab;
}
void Delta_Gamma_Hedge::erase_delta_futures_in_portfolio()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] delta_futures_in_portfolio->tab[i];
	}
	delete[] delta_futures_in_portfolio->tab;
}
void Delta_Gamma_Hedge::erase_delta_asset()
{
	delete[] delta_asset->tab_of_delta_asset_struct;
}
void Delta_Gamma_Hedge::erase_delta_futures()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] delta_futures->tab[i];
	}
	delete[] delta_futures->tab;
}
void Delta_Gamma_Hedge::erase_rf_futures()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] rf_futures->tab[i];
	}
	delete[] rf_futures->tab;
}
void Delta_Gamma_Hedge::erase_tte_futures()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] tte_futures->tab[i];
	}
	delete[] tte_futures->tab;
}
void Delta_Gamma_Hedge::erase_delta_puts()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] delta_puts->tab[i];
	}
	delete[] delta_puts->tab;
}
void Delta_Gamma_Hedge::erase_new_option_notional()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] new_option_notional->tab[i];
	}
	delete[] new_option_notional->tab;
	delete[] new_option_notional->opt_not;
}
void Delta_Gamma_Hedge::erase_new_futures_notional()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] new_futures_notional->tab[i];
	}
	delete[] new_futures_notional->tab;
	delete[] new_futures_notional->fut_not;
}
void Delta_Gamma_Hedge::erase_option_notional()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] option_notional->tab[i];
	}
	delete[] option_notional->tab;
	delete[] option_notional->opt_not;
}
void Delta_Gamma_Hedge::erase_trading()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] trading->tab[i];
	}
	delete[] trading->tab;
	delete[] trading->trading;
}
void Delta_Gamma_Hedge::erase_market_value()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] market_value->tab[i];
	}
	delete[] market_value->tab;
	delete[] market_value->mkt_val;
}
void Delta_Gamma_Hedge::erase_ov_base()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] ov_base->tab[i];
	}
	delete[] ov_base->tab;
}
void Delta_Gamma_Hedge::erase_und_base()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] und_base->tab[i];
	}
	delete[] und_base->tab;
}
void Delta_Gamma_Hedge::erase_nbr_puts()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] nbr_puts->tab[i];
	}
	delete[] nbr_puts->tab;
	delete[] nbr_puts->numbers_of_puts;
}
void Delta_Gamma_Hedge::erase_existing_gamma()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[]  existing_gamma->tab[i];
	}
	delete[] existing_gamma->tab;
	delete[] existing_gamma->existing_ov;
}
void Delta_Gamma_Hedge::erase_option_trade()
{
	delete[] option_trade->tab_option_trade_struct;
}
void Delta_Gamma_Hedge::erase_gamma_puts_in_portfolio_2()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete [] gamma_puts_in_portfolio_2->tab[i];
	}
	delete [] gamma_puts_in_portfolio_2->tab;

}
void Delta_Gamma_Hedge::erase_gamma_puts_in_portfolio()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete [] gamma_puts_in_portfolio->tab[i];
	}
	delete [] gamma_puts_in_portfolio->tab;	
}
void Delta_Gamma_Hedge::erase_gamma_asset()
{
	delete [] gamma_asset->tab_of_gamma_asset_struct;
}
void Delta_Gamma_Hedge::erase_gamma_puts()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete [] gamma_puts->tab[i];
	}
	delete [] gamma_puts->tab;
}
void Delta_Gamma_Hedge::erase_underlying()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete [] underlying->tab[i];
	}
	delete [] underlying->tab;	
}
void Delta_Gamma_Hedge::erase_rf()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete [] rf->tab[i];
	}
	delete [] rf->tab;
}
void Delta_Gamma_Hedge::erase_zero_coupon_rates()
{
	for(int i=0;i<zero_coupon_rates->dim_x;++i)
	{
		delete []  zero_coupon_rates->tab[i];
	}
	delete [] zero_coupon_rates->tab;
}	
void Delta_Gamma_Hedge::erase_annual_swap_rates()
{
	for(int i=0;i<annual_swap_rates->dim_x;++i)
	{
		delete [] annual_swap_rates->tab[i];
	}
	delete [] annual_swap_rates->tab;
}
void Delta_Gamma_Hedge::erase_annual_swap_rates_shock_down()
{
	/*for(int k=0;k<annual_swap_rates_shock_down->dim_z;++k)
	{
		for (int i=0;i<annual_swap_rates_shock_down->dim_x;++i)
		{
			delete [] annual_swap_rates_shock_down->tab[k][i];
		}
		delete [] annual_swap_rates_shock_down->tab[k];
	}
	delete [] annual_swap_rates_shock_down->tab;*/
}
//void Delta_Gamma_Hedge::erase_annual_swap_rates_shock_up()
//{
//	for(int k=0;k<erase_annual_swap_rates_shock_up->dim_z;++k)
//	{
//		for (int i=0;i<erase_annual_swap_rates_shock_up->dim_x;++i)
//		{
//			delete [] erase_annual_swap_rates_shock_up->tab[k][i];
//		}
//		delete [] erase_annual_swap_rates_shock_up->tab[k];
//	}
//	delete [] erase_annual_swap_rates_shock_up->tab;
//}
void Delta_Gamma_Hedge::erase_tte()
{
	for(int i=0;i<assumptions->model_period;++i)
	{
		delete[] tte->tab[i];
	}
	delete[] tte->tab;
}
void Delta_Gamma_Hedge::erase_rw_scn()
{

	for(int i=0;i<rw_scn->model_period;++i)
	{
		delete [] rw_scn->tab[i];
	}
	delete [] rw_scn->tab;
	//delete [] rw_scn->maturities;
}
void Delta_Gamma_Hedge::erase_hedgeff()
{
 	
	delete this->hedgeff;

	//delete hedgeff->period;
	////delete hedgeff->av_e;
	//delete hedgeff->liab_ov;
	//delete hedgeff->delta_liab;
	//delete hedgeff->gamma_liab;
	//delete hedgeff->discount;
	//delete hedgeff->index;
	////delete hedgeff->gamma_1_as;

}
void Delta_Gamma_Hedge::erase_assumptions()
{

}

void Delta_Gamma_Hedge::erase_objects()
{
	erase_delta();
	erase_gamma();
	erase_new_futures_notional();
	erase_trading_futures();
	erase_market_value_futures();
	erase_fv_base();
	erase_delta_in_portfolio();

	

	erase_nbr_futures();   // -- First ERROR

    //! il exist encore ERROR (9 eme scenario)
	erase_existing_delta_futures();
	erase_existing_delta();
	//
	//// ACTUAL
	erase_future_trade();
	erase_delta_futures_in_portfolio_2();

	erase_delta_futures_in_portfolio();
	erase_delta_asset();
	erase_delta_futures();
	erase_rf_futures();
	

	erase_tte_futures();
	erase_delta_puts();
	erase_new_option_notional();
	erase_option_notional();
	erase_trading();
	erase_market_value();


	erase_ov_base();
	erase_und_base();
	erase_nbr_puts();
	erase_existing_gamma();
	erase_option_trade();
	erase_gamma_puts_in_portfolio_2();
	erase_gamma_puts_in_portfolio();
	erase_gamma_asset();
	erase_gamma_puts();
	erase_underlying();
	erase_rf();
	erase_zero_coupon_rates();	
	erase_annual_swap_rates_shock_down();
	erase_annual_swap_rates();
	erase_tte();
	//erase_rw_scn();
	erase_hedgeff();
	erase_assumptions();
}
Delta_Gamma_Hedge::~Delta_Gamma_Hedge(void)
{
//chth
	//delete  this->delta;
	//delete this->gamma;
	//delete this->trading_futures;
	//delete this->market_value_futures;
	//delete this->fv_base;
	//delete this->delta_in_portfolio;
	//delete this->nbr_futures;
	//delete this->existing_delta_futures;
	//delete this->existing_delta;
	//delete this->future_trade;
	//delete this->delta_futures_in_portfolio_2;
	//delete this->delta_futures_in_portfolio;
	//delete this->delta_asset;
	//delete this->delta_futures;
	//delete this->rf_futures;
	//delete this->tte_futures;
	//delete this->delta_puts;
	//delete this->new_option_notional;
	//delete this->option_notional;
	//delete this->trading;
	//delete this->market_value;
	//delete this->ov_base;
	//delete this->und_base;
	//delete this->nbr_puts;
	//delete this->existing_gamma;
	//delete this->option_trade;
	////delete this->gamma_puts_in_portfolio;
	////delete this->gamma_puts_in_portfolio_2;
	//erase_gamma_puts_in_portfolio_2();
	//erase_gamma_puts_in_portfolio();

	////chth
	//erase_new_futures_notional();


	//delete this->gamma_asset;
	//delete this->gamma_puts;
	////delete this->und_base;
	//delete this->underlying;
	//delete this->rf ;
	//delete this->zero_coupon_rates;	
	//delete this->annual_swap_rates;
	//delete this->tte;
	//delete this->rw_scn;
	//delete this->hedgeff;
	//delete this->assumptions;
	
	

	
	ofstream("DELETE DGH DELTA.txt");
}
