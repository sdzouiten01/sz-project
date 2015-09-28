#include "Nbr_Swaps.h"
#include "Swaps_Trade.h"
#include "Existing_Rho.h"
#include "Existing_Rho_Swaps.h"
#include "Assumptions.h"
#include "Rho_Swaps_In_Portfolio.h"
#include "Rho_Swaps_In_Portfolio_2.h"
#include "Tte_Swaps.h"
Nbr_Swaps::Nbr_Swaps(int swap_no,Swaps_Trade* swaps_trade,
				   Rho_Swaps_In_Portfolio* rho_swaps_in_portfolio,
				   Rho_Swaps_In_Portfolio_2* rho_swaps_in_portfolio_2,
				   Existing_Rho* existing_rho,Existing_Rho_Swaps* existing_rho_swaps,
				   Assumptions* assumptions,Tte_Swaps* tte_swaps)
{
	//this->debug_file.open("DEBUG_NBR_FUT.txt");
	this->swap_no=swap_no;
	this->current_future=0;
	this->objects_needed.swaps_trade=swaps_trade;
	this->objects_needed.rho_swaps_in_portfolio=rho_swaps_in_portfolio;
	this->objects_needed.rho_swaps_in_portfolio_2=rho_swaps_in_portfolio_2;
	this->objects_needed.existing_rho=existing_rho;
	this->objects_needed.existing_rho_swaps=existing_rho_swaps;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.tte_swaps=tte_swaps;
	this->init_fields();
}
void Nbr_Swaps::init_fields()
{

	init_tab();
}

void Nbr_Swaps::init_tab()
{
	//ofstream ffff("okm.txt");
//	bool current_future_changed;
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->numbers_of_swaps=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->numbers_of_swaps[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
	}
	
	//this->current_future=0;
	for (int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		//ffff<<i<<":"<<this->current_future<<endl;
		
		this->objects_needed.swaps_trade->objects_needed.existing_rho=this->objects_needed.existing_rho;
		this->objects_needed.swaps_trade->objects_needed.existing_rho_swaps=this->objects_needed.existing_rho_swaps;
		this->objects_needed.swaps_trade->current_future=this->current_future;
		this->objects_needed.swaps_trade->objects_needed.rho_swaps_in_portfolio=this->objects_needed.rho_swaps_in_portfolio;
		this->objects_needed.swaps_trade->objects_needed.rho_swaps_in_portfolio_2=this->objects_needed.rho_swaps_in_portfolio_2;
		
		this->objects_needed.swaps_trade->init_tab_swaps_trade_struct(i);
		
		this->current_future=-1;
		for (int j=0;j<this->objects_needed.assumptions->model_period;++j)
		{			
			
			this->objects_needed.rho_swaps_in_portfolio->objects_needed.nbr_swaps=this;
			
			this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j);			
			//this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j-1);
			this->current_col=j;
			init_tab(i,j);
			
		/*	if (this->tab[i][j]!=0) 
			{
				this->current_future++;
				
			}*/
			this->objects_needed.existing_rho->objects_needed.nbr_swaps=this;
			this->objects_needed.existing_rho->init_tab(i,j);
			this->objects_needed.existing_rho_swaps->objects_needed.nbr_swaps=this;
			this->objects_needed.existing_rho_swaps->init_tab(i,j);
			this->objects_needed.rho_swaps_in_portfolio_2->objects_needed.nbr_swaps=this;
			this->objects_needed.rho_swaps_in_portfolio_2->init_tab(i,j);
			this->numbers_of_swaps[i]+=this->tab[i][j];
		}	
		
	}
	
}
void Nbr_Swaps::init_tab(int i,int j)
{
	/*this->objects_needed.existing_rho->objects_needed.nbr_swaps=this;
	this->objects_needed.existing_rho->init_tab(i,j);
	this->objects_needed.rho_swaps_in_portfolio->objects_needed.nbr_swaps=this;
	this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j);*/
	
	if (i<j || this->objects_needed.tte_swaps->isMaturityReached(i,j))
	{
		//debug_file<<"  i<j OR i>=length+j"<<endl;
		//debug_file<<"    nbr_swaps=0:"<<endl;
		this->tab[i][j]=0;
	}
	else if(i==j)
	{
		
		//this->objects_needed.swaps_trade->init_tab_swaps_trade_struct(i);
		//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
		//debug_file<<"  i=j"<<endl;
		
		this->objects_needed.swaps_trade->init_new_net_rho(i);
		
			this->objects_needed.swaps_trade->init_contracts(i);
			//debug_file<<"    nbr_swaps=contracts"<<endl;
			//debug_file<<"      contracts="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].contracts<<endl;
			//debug_file<<"          because"<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut>0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[0][0]<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut<0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[0][1]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut>0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[1][0]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut<0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[1][1]<<endl;
			//debug_file<<"                because"<<endl;
			//debug_file<<"                Net Delta="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho<<endl;
			//debug_file<<"                New Net Delta="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].new_net_rho<<endl;
			//debug_file<<"                Ex Delta Fut="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].ex_rho_swaps<<endl;
			//debug_file<<"                rho Asset="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].rho_asset<<endl;
		  
		this->tab[i][j]=
			
			this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].contracts;
		
	}
	else 
	{
		//debug_file<<"i="<<i<<","<<"j="<<j<<endl;
		/*this->objects_needed.swaps_trade->objects_needed.existing_rho=this->objects_needed.existing_rho;
		this->objects_needed.swaps_trade->objects_needed.rho_swaps_in_portfolio=this->objects_needed.rho_swaps_in_portfolio;
		this->objects_needed.swaps_trade->init_tab_swaps_trade_struct(i);*/
		if(this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].short_existing_positions==0 ||
			j%(int)this->objects_needed.assumptions->reb_freq_rho!=0)
		{
			//debug_file<<"  indicator=0"<<endl;
			//debug_file<<"          because"<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut>0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[0][0]<<endl;
			//debug_file<<"          (NetDelta>0,ExDeltaFut<0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[0][1]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut>0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[1][0]<<endl;
			//debug_file<<"          (NetDelta<0,ExDeltaFut<0)="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].tab[1][1]<<endl;
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
			/*this->objects_needed.rho_swaps_in_portfolio->objects_needed.nbr_swaps=this;
			this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j);*/
			//this->objects_needed.swaps_trade->init_tab_swaps_trade_struct(i);
			if(abs(this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho)>
				abs(this->objects_needed.rho_swaps_in_portfolio->tab[i][j]))
			{
				//debug_file<<"       NetDelta>DeltaFutInPortfolio"<<endl;
				//debug_file<<"           so number of futures=0"<<endl;
				//debug_file<<"           because"<<endl;
				//debug_file<<"           NetDelta="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho<<endl;
				//debug_file<<"           DeltaFutInPortfolio="<<this->objects_needed.rho_swaps_in_portfolio->tab[i][j]<<endl;
				this->tab[i][j]=0;
			}
			else if(j==0)
			{
				//debug_file<<"       NetDelta<DeltaFutInPortfolio AND j=0"<<endl;
				//debug_file<<"         NbFut(i,j)=NbFut(i-1,j)*(1-NetDelta/ExistingDelta)"<<endl;
				
				//this->objects_needed.existing_rho->objects_needed.nbr_swaps=this;
				//this->objects_needed.existing_rho->init_tab(i-1,j);
				if(this->tab[i-1][j]==0)
					this->tab[i][j]=0;
				else
					this->tab[i][j]=
						this->tab[i-1][j]*(1-this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho/
						this->objects_needed.existing_rho_swaps->tab[i-1][j]);
				//debug_file<<"           so NbFut(i,j)="<<this->tab[i][j]<<endl;
				//debug_file<<"           because"<<endl;
				//debug_file<<"           NetDelta="<<this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho<<endl;
				//debug_file<<"           DeltaFutInPortfolio="<<this->objects_needed.rho_swaps_in_portfolio->tab[i][j]<<endl;
				//debug_file<<"           ExistingDelta="<<this->objects_needed.existing_rho->tab[i-1][j]<<endl;
				//debug_file<<"			NbFut(i-1,j)="<<this->tab[i-1][j]<<endl;
			}
			else 
			{
				this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j-1);
				//this->objects_needed.rho_swaps_in_portfolio->init_tab(i,j-1);
				if(abs(this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho)>
					abs(this->objects_needed.rho_swaps_in_portfolio->tab[i][j-1]))
				{
					//this->objects_needed.existing_rho->init_tab(i-1,j);
					//this->objects_needed.swaps_trade->init_tab_swaps_trade_struct(i);
					if(this->tab[i-1][j]==0)
						this->tab[i][j]=0;
					else
						this->tab[i][j]=
							this->tab[i-1][j]*
							(1-(this->objects_needed.swaps_trade->tab_swaps_trade_struct[i].net_rho
							-this->objects_needed.rho_swaps_in_portfolio->tab[i][j-1])/
							this->objects_needed.existing_rho_swaps->tab[i-1][j]);
				}
				else

				{
					if(i==0)this->tab[i][j]=0;
					else this->tab[i][j]=this->tab[i-1][j];
				}

			}
		}
	}
	
	
//	this->objects_needed.existing_rho->init_existing_ov(i);
}

Nbr_Swaps::~Nbr_Swaps(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->numbers_of_swaps;
}
