#include <vector>
using namespace std;
#pragma once

class percentile
{
private :
	double percentile_value;
	double val_percent;
	double ind_percent;

public :
	
	percentile()
	{
	};	

	~percentile()
	{
	};	

	percentile(double perc)
	{
		this->percentile_value =perc;
	};
	
	void set_perc(vector<double> vect_0)
	{
		int taille = vect_0.size();
		int rank_perc = percentile_value*taille;
		int i,j,k,ind_min,ind_temp,val_temp,i_max;
		
		vector<int> vect_ind;
		vect_ind.resize(taille);
		for (k=0;k<taille-1;k++)
		{vect_ind[k]=k;}

		i_max=rank_perc+1;
		if (rank_perc+1>taille){i_max=taille;}

		for (i=0;i<min(rank_perc+1,taille);i++)
		{	
			ind_min=i;

			for (j=i+1;j<taille;j++)	
			{
				if (vect_0[j]<vect_0[i]) 
				{

				ind_min=j;
				val_temp=vect_0[i];
				ind_temp=vect_ind[i];
				vect_0[i]=vect_0[ind_min];
				vect_ind[i]=vect_ind[ind_min];
				vect_0[ind_min]=val_temp;
				vect_ind[ind_min]=ind_temp;
				
				}
			}
		}
		
		this->val_percent = vect_0[rank_perc];
		this->ind_percent = vect_ind[rank_perc];

	};

	double get_val_percent()
	{
		return this->val_percent;
	};

	int get_ind_percent()
	{
		return this->ind_percent;
	};

	double moy(vector<double> vect_0)
	{
		int taille = vect_0.size();
		int i,compt;

		compt=0;

		for (i=0;i<taille;i++)	
		{
			compt+=vect_0[i];
		}
		
		return compt/taille;
	};
};
