#pragma once

#include "../stdafx.h"
#include <vector>
using namespace std;

/*
* This class gather all the information related to a fund 
* string alh_fund_id: The name of the fund 
* double percent_fee_amount: The percentage of fees for this fund
* double fund_value : The fund value
* double current_beta[MAXIMUM_NUMBER_OF_INDEXES]: Array containing the values of the beta for t = 1
* double target_beta[MAXIMUM_NUMBER_OF_INDEXES]: Array containing the values of the targets beta
* int nb_indexes: The total number of indexes for this fund
* int position_in_beta_array[MAXIMUM_NUMBER_OF_INDEXES]: Array containing the id of the non null indexes (for optimization purpose only)
* int fund_id : The id of the fund (The list is fund_list sheet)
*/

class Fund
{

public :

	string alh_fund_id;
	double percent_fee_amount;

	double fund_value;

	double current_beta[MAXIMUM_NUMBER_OF_INDEXES];
	double target_beta[MAXIMUM_NUMBER_OF_INDEXES];

	int nb_indexes;
	int position_in_beta_array[MAXIMUM_NUMBER_OF_INDEXES];

	int fund_id;
	vector<int> list_of_shortsell_group_ids;

	~Fund()
	{
	}

	Fund()
	{
		this->nb_indexes = 0;
		memset(this->current_beta, 0, sizeof(this->current_beta));
		memset(this->target_beta, 0, sizeof(this->target_beta));
		memset(this->position_in_beta_array, 0, sizeof(this->position_in_beta_array));
	}

	Fund(const string& alh_f_id, double perc_fee_amt,
		 const string& list_of_current_betas, const string& list_of_target_betas)
	{

		this->alh_fund_id = alh_f_id;

		this->percent_fee_amount = perc_fee_amt;
		

		this->fill_betas(list_of_target_betas, true);
		this->fill_betas(list_of_current_betas, false);
		memset(this->current_beta, 0, sizeof(this->current_beta));
		memset(this->target_beta, 0, sizeof(this->target_beta));
		memset(this->position_in_beta_array, 0, sizeof(this->position_in_beta_array));

		this->nb_indexes = 0;
	}

	void fill_betas(const string& betas,
		bool is_target_split)
	{
		int l = betas.length();
		char *retPtr = new char[l + 1];
		strcpy(retPtr, betas.c_str());
		retPtr[betas.length()] = '\0';
		double split = atof(strtok(retPtr, ","));
		if (is_target_split)
		{
			if (split != 0)
				this->nb_indexes++;
			
			this->target_beta[0] = split;
		}
		else
			this->current_beta[0] = split;
		for (int i = 1 ; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			split = atof(strtok(NULL, ","));
			if (is_target_split)
			{
				if (split != 0)
					this->nb_indexes++;
				this->target_beta[i] = split;
			}
			else
				this->current_beta[i] = split;
		}
		delete retPtr;
	}

	void optimize_array()
	{
		int pos = 0;
		for (int i = 0 ; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			if (this->target_beta[i] != 0.)
			{
				position_in_beta_array[pos] = i;
				pos++;
			}
		}
	}

	void set_alh_fund_id(const string& fund_name)
	{
		this->alh_fund_id = fund_name;
	}

	void set_percentage_fee_amount(double fee)
	{
		this->percent_fee_amount = fee;
	}
		
	double get_percentage_fee_amount()
	{
		return this->percent_fee_amount;
	}

	void set_fund_value(double valo)
	{
		this->fund_value = valo;
	}

	double get_fund_value()
	{
		return this->fund_value;
	}

	double get_fund_id()
	{
		return this->fund_id;
	}

	void set_fund_id(int id)
	{
		this->fund_id = id;
	}

	void add_shortsell_goup_id(int shortsell_group_id)
	{
		this->list_of_shortsell_group_ids.push_back(shortsell_group_id);
	}

	bool check_group_shortsell_id(int group_id)
	{
		for (int i=0; i < (int)this->list_of_shortsell_group_ids.size(); i++)
		{
			if (this->list_of_shortsell_group_ids[i] == group_id)
				return true;
		}
		return false;
	}

	string get_alh_fund_id()
	{
		return this->alh_fund_id;
	
	}

};