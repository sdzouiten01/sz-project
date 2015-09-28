#include "Rho_Puts.h"
#include "Nbr_Puts.h"
#include "Assumptions.h"
#include "Nbr_Futures.h"
#include "Rho_Futures.h"
#include "Nbr_Swaps.h"
#include "Rho_Swaps.h"
#include "Rho_In_Portfolio.h"
Rho_In_Portfolio::Rho_In_Portfolio(
		Assumptions* assumptions,
		Rho_Puts** rho_puts_glob,
		Nbr_Puts** nbr_puts_glob,
		Nbr_Futures** nbr_futures_glob,
		Rho_Futures** rho_futures_glob,
		Nbr_Swaps* nbr_swaps,
		Rho_Swaps* rho_swaps,
		int nb_index
		)
{
	this->objects_needed.assumptions=assumptions;
	this->objects_needed.rho_puts_glob=rho_puts_glob;
	this->objects_needed.nbr_puts_glob=nbr_puts_glob;
	this->objects_needed.nbr_futures_glob=nbr_futures_glob;
	this->objects_needed.rho_futures_glob=rho_futures_glob;
	this->objects_needed.nbr_swaps=nbr_swaps;
	this->objects_needed.rho_swaps=rho_swaps;
	this->nb_index=nb_index;
	this->init_tab();
}


void Rho_In_Portfolio::init_tab()
{
	this->tab=new double*[this->objects_needed.assumptions->model_period];
	this->asset_rho=new double[this->objects_needed.assumptions->model_period];
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		this->asset_rho[i]=0;
		this->tab[i]=new double[this->objects_needed.assumptions->model_period];
        for(int k=0; k<this->objects_needed.assumptions->model_period; k++)
            this->tab[i][k] = 0;
 		//cout << "this->asset_rho[i] =  " << this->asset_rho[i] << " ''''''''''''  " << endl;
	}
	for(int j=0;j<this->objects_needed.assumptions->model_period;++j)
	{
		for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
		{
			for(int k=0;k<this->nb_index;++k)
			{
				this->tab[i][j]+=this->objects_needed.nbr_puts_glob[k]->tab[i][j]*
									this->objects_needed.rho_puts_glob[k]->tab[i][j];
				this->tab[i][j]+=this->objects_needed.nbr_futures_glob[k]->tab[i][j]*
									this->objects_needed.rho_futures_glob[k]->tab[i][j];
			}
			this->tab[i][j]+=this->objects_needed.rho_swaps->tab[i][j]*
								this->objects_needed.nbr_swaps->tab[i][j];
			this->asset_rho[i]+=this->tab[i][j];
		}
	}

}
Rho_In_Portfolio::~Rho_In_Portfolio(void)
{
	for(int i=0;i<this->objects_needed.assumptions->model_period;++i)
	{
		delete[] this->tab[i];
	}
	delete[] this->tab;
	delete[] this->asset_rho;
}