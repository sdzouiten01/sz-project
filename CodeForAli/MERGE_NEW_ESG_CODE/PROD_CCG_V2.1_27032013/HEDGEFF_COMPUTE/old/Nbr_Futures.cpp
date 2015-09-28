#include "Nbr_Futures.h"
#include "Future_Trade.h"
#include "Existing_Delta.h"
#include "Existing_Delta_Futures.h"
#include "Assumptions.h"
#include "Delta_Futures_In_Portfolio.h"
#include "Delta_Futures_In_Portfolio_2.h"
#include "Tte_Futures.h"
Nbr_Futures::Nbr_Futures(Future_Trade* future_trade,
				   Delta_Futures_In_Portfolio* delta_futures_in_portfolio,
				   Delta_Futures_In_Portfolio_2* delta_futures_in_portfolio_2,
				   Existing_Delta* existing_delta,Existing_Delta_Futures* existing_delta_futures,
				   Assumptions* assumptions,Tte_Futures* tte_futures)
{
	//this->debug_file.open("DEBUG_NBR_FUT.txt");
	this->current_future=0;
	this->objects_needed.future_trade=future_trade;
	this->objects_needed.delta_futures_in_portfolio=delta_futures_in_portfolio;
	this->objects_needed.delta_futures_in_portfolio_2=delta_futures_in_portfolio_2;
	this->objects_needed.existing_delta=existing_delta;
	this->objects_needed.existing_delta_futures=existing_delta_futures;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.tte_futures=tte_futures;
	this->init_fields();
}
void Nbr_Futures::init_fields()
{

	init_tab();
}

void Nbr_Futures::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];

	this->numbers_of_futures=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->numbers_of_futures[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	
	//this->current_future=0;
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->objects_needed.future_trade->objects_needed.existing_delta=this->objects_needed.existing_delta;
		this->objects_needed.future_trade->objects_needed.existing_delta_futures=this->objects_needed.existing_delta_futures;
		this->objects_needed.future_trade->current_future=this->current_future;
		this->objects_needed.future_trade->objects_needed.delta_futures_in_portfolio=this->objects_needed.delta_futures_in_portfolio;
		this->objects_needed.future_trade->objects_needed.delta_futures_in_portfolio_2=this->objects_needed.delta_futures_in_portfolio_2;
		this->objects_needed.future_trade->init_tab_futures_purchase_struct(i);
		this->current_future=-1;
		for (int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{			
			
			this->objects_needed.delta_futures_in_portfolio->objects_needed.nbr_futures=this;
			this->objects_needed.delta_futures_in_portfolio->init_tab(i,j);			
			//this->objects_needed.delta_futures_in_portfolio->init_tab(i,j-1);
			this->current_col=j;
			init_tab(i,j);

		/*	if (this->tab[i][j]!=0) 
			{
				this->current_future++;
				
			}*/
			this->objects_needed.existing_delta->objects_needed.nbr_futures=this;
			this->objects_needed.existing_delta->init_tab(i,j);
			this->objects_needed.existing_delta_futures->objects_needed.nbr_futures=this;
			this->objects_needed.existing_delta_futures->init_tab(i,j);
			this->objects_needed.delta_futures_in_portfolio_2->objects_needed.nbr_futures=this;
			this->objects_needed.delta_futures_in_portfolio_2->init_tab(i,j);
			this->numbers_of_futures[i]+=this->tab[i][j];
		}	
		
	}
	
}
void Nbr_Futures::init_tab(int i,int j)
{
	/*this->objects_needed.existing_delta->objects_needed.nbr_futures=this;
	this->objects_needed.existing_delta->init_tab(i,j);
	this->objects_needed.delta_futures_in_portfolio->objects_needed.nbr_futures=this;
	this->objects_needed.delta_futures_in_portfolio->init_tab(i,j);*/
	if (i<j || this->objects_needed.tte_futures->isMaturityReached(i,j))
	{
		//debug_file<<"  i<j OR i>=length+j"<<endl;
		//debug_file<<"    nbr_futures=0:"<<endl;
		this->tab[i][j]=0;
	}
	else if(i==j)
	{
		
		//this->objects_needed.future_trade->init_tab_futures_purchase_struct(i);
		//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
		//debug_file<<"  i=j"<<endl;
		this->objects_needed.future_trade->init_new_net_delta(i);
			this->objects_needed.future_trade->init_contracts(i);
			//debug_file<<"    nbr_futures=contracts"<<endl;
			//debug_file<<"      contracts="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].contracts<<endl;
			//debug_file<<"          because"<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut>0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[0][0]<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut<0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[0][1]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut>0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[1][0]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut<0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[1][1]<<endl;
			//debug_file<<"                because"<<endl;
			//debug_file<<"                Net Delta="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta<<endl;
			//debug_file<<"                New Net Delta="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].new_net_delta<<endl;
			//debug_file<<"                Ex Delta Fut="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].ex_delta_futures<<endl;
			//debug_file<<"                delta Asset="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].delta_asset<<endl;
		  
		this->tab[i][j]=
			
			this->objects_needed.future_trade->tab_futures_purchase_struct[i].contracts;
	}
	else 
	{
		//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
		/*this->objects_needed.future_trade->objects_needed.existing_delta=this->objects_needed.existing_delta;
		this->objects_needed.future_trade->objects_needed.delta_futures_in_portfolio=this->objects_needed.delta_futures_in_portfolio;
		this->objects_needed.future_trade->init_tab_futures_purchase_struct(i);*/
		if(this->objects_needed.future_trade->tab_futures_purchase_struct[i].short_existing_positions==0 ||
			j%(int)this->objects_needed.assumptions->reb_freq_delta!=0)
		{
			//debug_file<<"  indicator=0"<<endl;
			//debug_file<<"          because"<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut>0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[0][0]<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut<0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[0][1]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut>0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[1][0]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut<0)="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].tab[1][1]<<endl;
			//debug_file<<"     keep same number of futures"<<endl;
			if(i==0)
			{
				//debug_file<<"   i=0 so number of futures=0"<<endl;
				this->tab[i][j]=0;
			}
			else 
			{
				//debug_file<<"    so number of futures="<<this->tab[i-1][j]<<endl;
				this->tab[i][j]=this->tab[i-1][j];
			}
		}
		else 
		{
			//debug_file<<"    indicator=1"<<endl;
			/*this->objects_needed.delta_futures_in_portfolio->objects_needed.nbr_futures=this;
			this->objects_needed.delta_futures_in_portfolio->init_tab(i,j);*/
			//this->objects_needed.future_trade->init_tab_futures_purchase_struct(i);
			if(abs(this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta)>
				abs(this->objects_needed.delta_futures_in_portfolio->tab[i][j]))
			{
				//debug_file<<"       NetDelta>DeltaFutInPortfolio"<<endl;
				//debug_file<<"           so number of futures=0"<<endl;
				//debug_file<<"           because"<<endl;
				//debug_file<<"           NetDelta="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta<<endl;
				//debug_file<<"           DeltaFutInPortfolio="<<this->objects_needed.delta_futures_in_portfolio->tab[i][j]<<endl;
				this->tab[i][j]=0;
			}
			else if(j==0)
			{
				//debug_file<<"       NetDelta<DeltaFutInPortfolio AND j=0"<<endl;
				//debug_file<<"         NbFut(i,j)=NbFut(i-1,j)*(1-NetDelta/ExistingDelta)"<<endl;
				
				//this->objects_needed.existing_delta->objects_needed.nbr_futures=this;
				//this->objects_needed.existing_delta->init_tab(i-1,j);
				if(this->tab[i-1][j]==0)
					this->tab[i][j]=0;
				else
					this->tab[i][j]=
						this->tab[i-1][j]*(1-this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta/
						this->objects_needed.existing_delta_futures->tab[i-1][j]);
				//debug_file<<"           so NbFut(i,j)="<<this->tab[i][j]<<endl;
				//debug_file<<"           because"<<endl;
				//debug_file<<"           NetDelta="<<this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta<<endl;
				//debug_file<<"           DeltaFutInPortfolio="<<this->objects_needed.delta_futures_in_portfolio->tab[i][j]<<endl;
				//debug_file<<"           ExistingDelta="<<this->objects_needed.existing_delta->tab[i-1][j]<<endl;
				//debug_file<<"			NbFut(i-1,j)="<<this->tab[i-1][j]<<endl;
			}
			else 
			{
				this->objects_needed.delta_futures_in_portfolio->init_tab(i,j-1);
				//this->objects_needed.delta_futures_in_portfolio->init_tab(i,j-1);
				if(abs(this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta)>
					abs(this->objects_needed.delta_futures_in_portfolio->tab[i][j-1]))
				{
					//this->objects_needed.existing_delta->init_tab(i-1,j);
					//this->objects_needed.future_trade->init_tab_futures_purchase_struct(i);
					if(this->tab[i-1][j]==0)
						this->tab[i][j]=0;
					else
						this->tab[i][j]=
							this->tab[i-1][j]*
							(1-(this->objects_needed.future_trade->tab_futures_purchase_struct[i].net_delta
							-this->objects_needed.delta_futures_in_portfolio->tab[i][j-1])/
							this->objects_needed.existing_delta_futures->tab[i-1][j]);
				}
				else

				{
					if(i==0)this->tab[i][j]=0;
					else this->tab[i][j]=this->tab[i-1][j];
				}

			}
		}
	}
	
	
//	this->objects_needed.existing_delta->init_existing_ov(i);
}

Nbr_Futures::~Nbr_Futures(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->numbers_of_futures;
}
