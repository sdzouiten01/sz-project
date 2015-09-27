#include "Existing_Rho.h"
#include "Nbr_Swaps.h"
#include "Nbr_Futures.h"
#include "Rho_Swaps.h"
#include "Rho_Puts.h"
#include "Rho_Futures.h"
#include "Nbr_Puts.h"
Existing_Rho::Existing_Rho(int swap_no,
						   Assumptions* assumptions, 
						   Rho_Swaps* rho_swaps ,
						   Rho_Puts** rho_puts_glob
						   ,Nbr_Puts** nbr_puts_glob,
						   Nbr_Futures** nbr_futures_glob,
						   Nbr_Swaps* nbr_swaps,
						   Rho_Futures** rho_futures_glob,
						   int nb_index)
{
	this->objects_needed.rho_swaps=rho_swaps;
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.nbr_puts_glob=nbr_puts_glob;
	this->objects_needed.nbr_futures_glob=nbr_futures_glob;
	this->objects_needed.rho_futures_glob=rho_futures_glob;
	this->objects_needed.rho_puts_glob=rho_puts_glob;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->swap_no=swap_no;
	this->nb_index=nb_index;
		this->tab=new double*[this->objects_needed.assumptions->model_period];
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			this->tab[i]=new double[this->objects_needed.assumptions->model_period];
		}
	
	this->existing_sv=new double[this->objects_needed.assumptions->model_period];


	
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			this->existing_sv[i]=0;
			for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
			{
				this->tab[i][j]=0;
			}			
		}
	
}

void Existing_Rho::init_tab( int i, int j)
{

	if(i+1<this->objects_needed.assumptions->model_period)	
	{
		for (int index=0;index<this->nb_index;++index)
		{
			this->tab[i][j]+=this->objects_needed.nbr_puts_glob[index]->tab[i+1][j]
								*this->objects_needed.rho_puts_glob[index]->tab[i+1][j];
			this->tab[i][j]+=this->objects_needed.nbr_futures_glob[index]->tab[i+1][j]
								*this->objects_needed.rho_futures_glob[index]->tab[i+1][j];
		}
		this->tab[i][j]+=this->objects_needed.nbr_swaps->tab[i][j]
							*this->objects_needed.rho_swaps->tab[i+1][j];
	}
			//this->tab[i][j]=this->objects_needed.nbr_swaps->tab[i][j]
			//		*this->objects_needed.rho_swaps->tab[i+1][j]+
			//		this->objects_needed.nbr_puts->tab[i+1][j]
			//		*this->objects_needed.rho_puts->tab[i+1][j]+
			//		this->objects_needed.nbr_futures->tab[i+1][j]
			//		*this->objects_needed.rho_futures->tab[i+1][j];
	
	this->existing_sv[i]+=this->tab[i][j];
}
double Existing_Rho::return_existing_sv(int i)
{
	if(i==-1) 
	{
		double res=0;
		for (int index=0;index<this->nb_index;++index)
		{
			res+=this->objects_needed.nbr_puts_glob[index]->tab[0][0]
					*this->objects_needed.rho_puts_glob[index]->tab[0][0];
			res+=this->objects_needed.nbr_futures_glob[index]->tab[0][0]
					*this->objects_needed.rho_futures_glob[index]->tab[0][0];
		}
		return res;
	}
	//return this->objects_needed.nbr_puts->tab[0][0]
	//				*this->objects_needed.rho_puts->tab[0][0]+
	//					this->objects_needed.nbr_futures->tab[0][0]
	//				*this->objects_needed.rho_futures->tab[0][0];
	else return this->existing_sv[i];
}

Existing_Rho::~Existing_Rho(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->existing_sv;
}