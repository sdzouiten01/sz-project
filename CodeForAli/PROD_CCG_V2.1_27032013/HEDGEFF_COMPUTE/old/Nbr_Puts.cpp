#include "Nbr_Puts.h"
#include "Option_Trade.h"
#include "Existing_Gamma.h"
#include "Assumptions.h"
#include "Gamma_Puts_In_Portfolio.h"
#include "Gamma_Puts_In_Portfolio_2.h"
Nbr_Puts::Nbr_Puts(Option_Trade* option_trade,
				   Gamma_Puts_In_Portfolio* gamma_puts_in_portfolio,
				   Gamma_Puts_In_Portfolio_2* gamma_puts_in_portfolio_2,
				   Existing_Gamma* existing_gamma,
				   Assumptions* assumptions)
{
	//this->debug_file.open("DEBUG_NBR_PUTS.txt");
	this->objects_needed.option_trade=option_trade;
	this->objects_needed.gamma_puts_in_portfolio=gamma_puts_in_portfolio;
	this->objects_needed.gamma_puts_in_portfolio_2=gamma_puts_in_portfolio_2;
	this->objects_needed.existing_gamma=existing_gamma;
	this->objects_needed.assumptions=assumptions;
	this->init_fields();
}
void Nbr_Puts::init_fields()
{
	/*init_sigma();
	init_q();
	init_length_of_option_contract();
	init_sell_at_time_to_expiry();*/
	init_tab();
	init_numbers_of_puts();
}
//void Nbr_Puts::init_sigma()
//{
//	this->characteristics.sigma=this->objects_needed.assumptions->volatility;
//}
//void Nbr_Puts::init_q()
//{
//	this->characteristics.q=0;
//}
//void Nbr_Puts::init_length_of_option_contract()
//{
//	this->characteristics.length_of_option_contract=
//		this->objects_needed.assumptions->length_of_option_contract;
//}
//void Nbr_Puts::init_sell_at_time_to_expiry()
//{
//	this->characteristics.sell_at_time_to_expiry=
//		this->objects_needed.assumptions->sell_at_time_to_expiry;
//
//}
void Nbr_Puts::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->numbers_of_puts=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->numbers_of_puts[i]=0;
 		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->objects_needed.option_trade->objects_needed.existing_gamma=this->objects_needed.existing_gamma;
		this->objects_needed.option_trade->objects_needed.gamma_puts_in_portfolio=this->objects_needed.gamma_puts_in_portfolio;
		this->objects_needed.option_trade->objects_needed.gamma_puts_in_portfolio_2=this->objects_needed.gamma_puts_in_portfolio_2;
		this->objects_needed.option_trade->init_tab_asset_purchase_struct(i);
		for (int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{			
			
			this->objects_needed.gamma_puts_in_portfolio->objects_needed.nbr_puts=this;
			this->objects_needed.gamma_puts_in_portfolio->init_tab(i,j);	
			if(j!=0)
				this->objects_needed.gamma_puts_in_portfolio->init_tab(i,j-1);

			init_tab(i,j);

			this->objects_needed.existing_gamma->objects_needed.nbr_puts=this;
			this->objects_needed.existing_gamma->init_tab(i,j);
			this->objects_needed.gamma_puts_in_portfolio_2->objects_needed.nbr_puts=this;
			this->objects_needed.gamma_puts_in_portfolio_2->init_tab(i,j);
			this->numbers_of_puts[i]+=this->tab[i][j];
		}	
		
	}
	/*ofstream ll("glouglou.txt");
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		ll<<this->numbers_of_puts[i]<<endl;
	}*/
}
void Nbr_Puts::init_tab(int i,int j)
{
	/*this->objects_needed.existing_gamma->objects_needed.nbr_puts=this;
	this->objects_needed.existing_gamma->init_tab(i,j);
	this->objects_needed.gamma_puts_in_portfolio->objects_needed.nbr_puts=this;
	this->objects_needed.gamma_puts_in_portfolio->init_tab(i,j);*/
	if (i<j || i>=this->objects_needed.assumptions->length_of_option_contract
		-this->objects_needed.assumptions->sell_at_time_to_expiry+j)
		this->tab[i][j]=0;
	else if(i==j)
	{
		//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
		//this->objects_needed.option_trade->init_tab_asset_purchase_struct(i);
	
		this->objects_needed.option_trade->init_new_net_gamma(i);
			this->objects_needed.option_trade->init_contracts(i);
		this->tab[i][j]=
			
			this->objects_needed.option_trade->tab_option_trade_struct[i].contracts;
		//debug_file<<"	so nbr_puts =contracts"<<endl;
		//debug_file<<"		contracts="<<this->objects_needed.option_trade->tab_option_trade_struct[i].contracts<<endl;
		//debug_file<<"		new net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].new_net_gamma<<endl;
		//debug_file<<"		net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma<<endl;
		//debug_file<<"		gamma liab="<<this->objects_needed.option_trade->tab_option_trade_struct[i].gamma_liability<<endl;
		//debug_file<<"		net/liab="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma/this->objects_needed.option_trade->tab_option_trade_struct[i].gamma_liability;
		//debug_file<<"		UpBound="<<this->objects_needed.option_trade->gamma_bounds.net_gamma_upper_bound;
		//debug_file<<"		InfBound="<<this->objects_needed.option_trade->gamma_bounds.net_gamma_lower_bound;
		//debug_file<<"       gammaputs in portfolio 2="<<this->objects_needed.option_trade->objects_needed.gamma_puts_in_portfolio_2->tab[i][i-1]<<endl;
	}
	else 
	{
		
		/*this->objects_needed.option_trade->objects_needed.existing_gamma=this->objects_needed.existing_gamma;
		this->objects_needed.option_trade->objects_needed.gamma_puts_in_portfolio=this->objects_needed.gamma_puts_in_portfolio;
		this->objects_needed.option_trade->init_tab_asset_purchase_struct(i);*/
		if(this->objects_needed.option_trade->tab_option_trade_struct[i].short_existing_positions==0 ||
			!this->objects_needed.option_trade->tab_option_trade_struct[i].hedging)
		{
			//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
			//debug_file<<"indicator=0"<<endl;
			//debug_file<<"ex gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].ex_gamma<<endl;
			//debug_file<<"net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma<<endl;
			if(i==0)this->tab[i][j]=0;
			else this->tab[i][j]=this->tab[i-1][j];
		}
		else 
		{
			/*this->objects_needed.gamma_puts_in_portfolio->objects_needed.nbr_puts=this;
			this->objects_needed.gamma_puts_in_portfolio->init_tab(i,j);*/
			//this->objects_needed.option_trade->init_tab_asset_purchase_struct(i);
			if(abs(this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma)>
				abs(this->objects_needed.gamma_puts_in_portfolio->tab[i][j]))
			{
				//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
				//debug_file<<"		net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma<<endl;
				//debug_file<<"		gammaliab="<<this->objects_needed.option_trade->tab_option_trade_struct[i].gamma_liability<<endl;
				//debug_file<<"		existing_gamma="<<this->objects_needed.existing_gamma->tab[i-1][j]<<endl;
				//debug_file<<"		gamma puts in portolio i,j="<<this->objects_needed.gamma_puts_in_portfolio->tab[i][j]<<endl;
				this->tab[i][j]=0;
			}
			else if(j==0)
			{
				//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
				//debug_file<<"		net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma<<endl;
				//debug_file<<"		gammaliab="<<this->objects_needed.option_trade->tab_option_trade_struct[i].gamma_liability<<endl;
				//debug_file<<"		existing_gamma="<<this->objects_needed.existing_gamma->tab[i-1][j]<<endl;
				//debug_file<<"		nbrputs(i-1,j)="<<this->tab[i-1][j]<<endl;
				//this->objects_needed.existing_gamma->objects_needed.nbr_puts=this;
				//this->objects_needed.existing_gamma->init_tab(i-1,j);
				if(this->tab[i-1][j]==0)
					this->tab[i][j]=0;
				else
					this->tab[i][j]=
						this->tab[i-1][j]*(1-this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma/
						this->objects_needed.existing_gamma->tab[i-1][j]);
			}
			else 
			{
				//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
				//debug_file<<"		net gamma="<<this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma<<endl;
				//debug_file<<"		gammaliab="<<this->objects_needed.option_trade->tab_option_trade_struct[i].gamma_liability<<endl;
				//debug_file<<"		existing_gamma="<<this->objects_needed.existing_gamma->tab[i-1][j]<<endl;
				//debug_file<<"		gamma puts in portolio i,j-1="<<this->objects_needed.gamma_puts_in_portfolio->tab[i][j-1]<<endl;

				//debug_file<<"		nbrputs(i-1,j)="<<this->tab[i-1][j]<<endl;
				//this->objects_needed.gamma_puts_in_portfolio->init_tab(i,j-1);
				if(abs(this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma)>
					abs(this->objects_needed.gamma_puts_in_portfolio->tab[i][j-1]))
				{
					//this->objects_needed.existing_gamma->init_tab(i-1,j);
					//this->objects_needed.option_trade->init_tab_asset_purchase_struct(i);
					if(this->tab[i-1][j]==0)
						this->tab[i][j]=0;
					else
					this->tab[i][j]=
						this->tab[i-1][j]*
						(1-(this->objects_needed.option_trade->tab_option_trade_struct[i].net_gamma
						-this->objects_needed.gamma_puts_in_portfolio->tab[i][j-1])/
						this->objects_needed.existing_gamma->tab[i-1][j]);
				}
				else

				{
					if(i==0)this->tab[i][j]=0;
					else this->tab[i][j]=this->tab[i-1][j];
				}

			}
		}
	}
	
	
//	this->objects_needed.existing_gamma->init_existing_ov(i);
}
void Nbr_Puts::init_numbers_of_puts()
{

}
Nbr_Puts::~Nbr_Puts(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete [] this->tab[i];
	}
	delete [] this->tab;
	delete this->numbers_of_puts;
}
