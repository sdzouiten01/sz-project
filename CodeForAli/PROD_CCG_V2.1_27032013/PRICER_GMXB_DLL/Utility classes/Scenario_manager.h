#pragma once
#include "stdafx.h"
#include "Utility classes\GMXBLogger.h"
#include "head.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

extern ofstream trace;


using namespace std;
class scenario_manager
{
	private	:
		// Number of scenarios to handle 
		int number_of_scenarios; 
		
		// List of mapping columns with the funds
		// int scen_fund_col[SUBACCT_NUM_FUNDS];
		int *scen_fund_col;
		
		// The columns of teh discount factor
		int yield_column_for_pvs; 
		
		// The columns of the lapse dyn treas
		int lapse_dyn_treas_col;
		
		// 12 if rates in scenario manager are monthly and annualized
		// 1. if rates in scenario manager are monthly and not annualized
		int rates_indic;
		
		// Delimiter of the file (should be a comma)
		char delimiter;
		int period;
		int subacct_max_period;
		int time_step;
		
		// Variables for Hedge Effectivness
		double *** swap_table;
		int nb_rate;

		// The total number of indexes
		int number_of_indexes;

		// Table of annualized rates
		// The dimensions of the table are :
		// 1 - The number of scenarios
		// 2 - Elapsed months (from 1 to 12) 
		// 3 - The period (hard coded vale set tà 61)
		// 4 - The number of funds + rates + discount factor + period
		double **** ann_from_monthly_rates;
		double *** monthly_rates;
		
		// monthly scenarios coming from 
		// the esg or an external dll
		double ***va_scenario;

		/*
		* create the table of annualized rates (initialization of ann_from_monthly_rates)
		*/
		void create_ann_from_monthly_table();

		/*
		* reset the table of annualized rates (set ann_from_monthly_rates to reset_value)
		*/
		void reset_ann_from_monthly_table(int reset_value);

		/*
		* Map the column number for all the equities, bonds, discount_factor 
		*/
		void set_columns(vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
		int lapse_dyn_treas_col_index, int rates_indic_index);

		void init_ann_from_monthly_table(const string& file_name, int reset_value);
		void init_ann_from_monthly_table(int reset_value);

		void init_monthly_rates_table(int reset_value); 
		void init_monthly_rates_table(const string& file_name, int reset_value);

		void init_ann_from_monthly_table_he(const string& file_name, int reset_value, double * swap_table, int nb_date);
		void init_monthly_rates_table_he(const string& file_name, int reset_value, double * swap_table, int nb_date);

		/*
		* Temporary function . Remove it
		*/
		
		void display_ann_from_mthly(char * file_name);
		void reset_monthly_rates_table(int reset_value);
		void display_va_scenario(char * file_name);
		void delete_ann_from_monthly_table();
		void delete_monthly_rates_table();
		void create_monthly_rates_table();
		int init_t_high_shorter(const string& file_name);
	
		
		

	public	: 

		/*
		* Destructor of the class. (Delete ann_from_monthly_rates rates table)
		*/
		~scenario_manager();

		/*
		* Constructor when the file mode is used
		*/
	scenario_manager();
	
		scenario_manager(int number_of_scen, vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int& t_high_shorter, const string& file_name, int time_step);

		/*
		* Constructor when the esg mode is used
		*/
		scenario_manager(double ***va_scen, int number_of_scen, vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int t_high_shorter, int time_step);

		/*
		* Constructor added for hedge effectivness
		*/
		scenario_manager(int number_of_scen, vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int& t_high_shorter, const string& file_name, int time_step, int use_scenario_file_inner,int t_high_local);
		
		/*
		* Calculate the annual rates from a given scenario file name
 		*/
		void init_scenario_manager(const string& file_name, int reset_value);

		/*
		* Calculate the annual rates from the va_scenario
 		*/
		void init_scenario_manager(int reset_value);
		void init_scenario_manager_he(const string& file_name, int reset_value, double* swap_table, int nb_date);

		/*
		* Calculate the annual rates from the va_scenario: for ESG
 		*/
		void init_scenario_manager_ESG(int reset_value, bool if_he_inner_mode);
		void init_ann_from_monthly_table_ESG(int reset_value, bool if_he_inner_mode);
		void init_monthly_rates_table_ESG(int reset_value, bool if_he_inner_mode);

		/*
		* Get the annaulized rates for the given it_number
		*/
		double *** get_ann_from_mthly_rates(int it_number);

		/*
		* Get the total number of scenario to run
		*/
		int get_nb_scenarios();
		void set_va_scenario(double*** va_scen);
		double ** get_monthly_rates(int it_number);
		double ** get_swap_table(int it_number);

				
		/*
		* Actuarial module - Function used to read growth rates given as inputs for projection.
		* These inputs are read and used directly in the function av_growth instead of afmr table
		*/
		//void read_growth_for_aging_policies(const string& file_name, double * growth_by_mp);
		void read_growth_for_aging_policies(const string& file_name, int reset_value, double * growth_by_mp, int nb_of_mp_outers);

};

/*
* Destructor of the class. Delete ann_from_monthly_rates rates table
*/
scenario_manager::~scenario_manager()
{
	if (this->time_step == ANNUAL)
	{
		this->delete_ann_from_monthly_table();
	}
	else 
	{
		this->delete_monthly_rates_table();
	}
	
	if(scen_fund_col != 0)
	delete[] scen_fund_col;

	if (this->nb_rate != 0)
	{
		for(int it = 0; it < this->number_of_scenarios ; it++)
		{
			for(int i = 0; i <= this->subacct_max_period*12; i++)
			{
				delete[] swap_table[it][i];
			}
			delete[] swap_table[it];
		}
		delete[] swap_table;
	}
}


void scenario_manager::delete_ann_from_monthly_table()
{
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for(int i = 0; i < DECALAGE_MAX +1; i++)
		{
			for (int j = 0; j <= this->subacct_max_period; j++)
			{
				delete[] ann_from_monthly_rates[it][i][j];
			}
			delete[] ann_from_monthly_rates[it][i];
		}
		delete[] ann_from_monthly_rates[it];
	}
	delete[] ann_from_monthly_rates;
}

void scenario_manager::delete_monthly_rates_table()
{
	//For actuarial tool
	if(monthly_rates != 0)
	{
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for(int i = 0; i <= this->subacct_max_period*12; i++)
		{
			delete[] monthly_rates[it][i];
		}
		delete[] monthly_rates[it];
	}
	delete[] monthly_rates;
	}
}

/*
* Create the table of annualized table
* Only the number of scenarios is configurable
* The number of funds, the decalage max and the number of funds are constants
*/
void scenario_manager::create_ann_from_monthly_table()
{
	this->ann_from_monthly_rates = new double***[this->number_of_scenarios];
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		this->ann_from_monthly_rates[it] = new double **[DECALAGE_MAX+1];
		for(int i = 0; i < DECALAGE_MAX +1; i++)
		{
			this->ann_from_monthly_rates[it][i] = new double *[this->subacct_max_period  + 1];
			for (int j = 0; j <= this->subacct_max_period; j++)
			{
				// The + 3 is due to period, scenario and short rates columns
				this->ann_from_monthly_rates[it][i][j] = new double[MAXIMUM_NUMBER_OF_INDEXES + 3];
			}
		}
	}
}

void scenario_manager::create_monthly_rates_table()
{
	this->monthly_rates = new double**[this->number_of_scenarios];
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		this->monthly_rates[it] = new double *[this->subacct_max_period*12 + 1];
		for (int i = 0; i <= this->subacct_max_period*12; i++)
		{
			// The + 3 is due to period, scenario and short rates columns
			this->monthly_rates[it][i] = new double[MAXIMUM_NUMBER_OF_INDEXES + 3];
		}
	}
}

/*
*	Reset the annualized table
*/
void scenario_manager::reset_ann_from_monthly_table(int reset_value)
{
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for(int i = 0; i < DECALAGE_MAX +1; i++)
		{
			for (int j = 0; j <= this->subacct_max_period; j++)
			{
				for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES + 3; k++)
				{
					this->ann_from_monthly_rates[it][i][j][k] = reset_value;
				}
			}
		}
	}
}

void scenario_manager::reset_monthly_rates_table(int reset_value)
{
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for (int i = 0; i <= this->subacct_max_period*12; i++)
		{
			for (int j = 0; j < MAXIMUM_NUMBER_OF_INDEXES + 3; j++)
			{
				this->monthly_rates[it][i][j] = reset_value;
			}
		}
	}
}
void scenario_manager::init_scenario_manager(const string& file_name, int reset_value)
{
	if (this->time_step == ANNUAL)
	{
		this->init_ann_from_monthly_table(file_name, reset_value);
	}
	else 
	{
		this->init_monthly_rates_table(file_name, reset_value);
	}
}

void scenario_manager::init_scenario_manager_he(const string& file_name, int reset_value, double* swap_table, int nb_date)
{
	if (this->time_step == ANNUAL)
	{
		this->init_ann_from_monthly_table_he(file_name,reset_value,swap_table, nb_date);
	}
	else 
	{
		this->init_monthly_rates_table_he(file_name,reset_value,swap_table, nb_date);
	}
}


void scenario_manager::init_ann_from_monthly_table(const string& file_name, int reset_value)
{
	this->reset_ann_from_monthly_table(reset_value);
	string row;
	string firstline;
	ifstream file_op(file_name.c_str() , ios::in);
	char *retPtr;
	int iteration = 0;
	int period = 0;
	double *returns_table;
	returns_table = new double[this->number_of_indexes + 3];
	for (int i = 0; i < this->number_of_indexes + 3; i++)
	{
		returns_table[i] = 0.;
	}

	if (file_op)
	{
		// Skip the first line
		getline(file_op, firstline); 
		while(iteration < this->number_of_scenarios && !file_op.eof())
		{
			getline(file_op,row);
			if (row.length() > 0)
			{
				// original version
				retPtr = new char[row.length() + 1];
				strcpy(retPtr, row.c_str());
				retPtr[row.length()] = '\0';
				int col = 0;
				iteration = atoi(strtok(retPtr, ",")) - 1;
				period = atoi(strtok(NULL, ","));
				// Skip the period 0
				if (period == 0)
				{
					delete[] retPtr;
					continue;
				}
				int tt = (int)(floor((period-1)/12.) + 1);
				char * returns;
				int nb_returns = 0;
				while ((returns = strtok(NULL, ",")) != NULL)
				{	
					returns_table[nb_returns] = atof(returns);
					nb_returns++;
				}
				delete[] retPtr;

				// Calculate the fund returns
				double discount_factor_inv = returns_table[this->yield_column_for_pvs - 1];

				int index_col;
				for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
				{
					// fund returns
					index_col = this->scen_fund_col[fund_num] - 1;
					for (int col = 1; col <= DECALAGE_MAX - ((period-1) % 12) ; col++)
					{	
						this->ann_from_monthly_rates[iteration][col][tt][fund_num] =  
								this->ann_from_monthly_rates[iteration][col][tt][fund_num] * (1.0 + returns_table[index_col]/100.0);

						if (discount_factor_inv < SMALL_DOUBLE)
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
						else
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	
				
						// TO DO check this line . Dynamic lapse treas
						if (this->lapse_dyn_treas_col > 0)
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[max(lapse_dyn_treas_col - 1, 0)];
						else
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES] = period;
					}
					
					for (int col = DECALAGE_MAX - ((period-1) % 12) + 1; col <= DECALAGE_MAX ; col++)
					{
						if (tt < this->subacct_max_period)
						{
								this->ann_from_monthly_rates[iteration][col][tt + 1][fund_num] =  
								this->ann_from_monthly_rates[iteration][col][tt + 1][fund_num] * (1.0 + returns_table[index_col]/100.0);
						}
					}
				}
			}
		}
		for (int it = 0; it < number_of_scenarios ; it++)
		{
			for (int tt = 1; tt <= this->subacct_max_period ; tt++)
			{
				for (int col = 1; col <= DECALAGE_MAX ; col++)
				{
					for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
					{					
						this->ann_from_monthly_rates[it][col][tt][fund_num] = 
								pow(this->ann_from_monthly_rates[it][col][tt][fund_num], 1.0 / this->rates_indic) - 1.0;
					}
				}
			}
		}
		file_op.close();

	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The scenario file %s is not found", file_name.c_str());
		throw new GMXBException(message);
	}

	delete[] returns_table;	
}

void scenario_manager::init_monthly_rates_table(const string& file_name, int reset_value)
{
	this->reset_monthly_rates_table(reset_value);
	string row;
	string firstline;
	ifstream file_op(file_name.c_str() , ios::in);
	char *retPtr;
	int iteration = 0;
	int period = 0;
	double *returns_table;
	returns_table = new double[this->number_of_indexes + 3];
	for (int i = 0; i < this->number_of_indexes + 3; i++)
	{
		returns_table[i] = 0.;
	}

	if (file_op)
	{
		// Skip the first line
		getline(file_op, firstline); 
		while(iteration < this->number_of_scenarios && !file_op.eof())
		{
			getline(file_op,row);
			if (row.length() > 0)
			{
				// original version
				retPtr = new char[row.length() + 1];
				strcpy(retPtr, row.c_str());
				retPtr[row.length()] = '\0';
				int col = 0;
				iteration = atoi(strtok(retPtr, ",")) - 1;
				period = atoi(strtok(NULL, ","));
				// Skip the period 0
				if (period == 0)
				{
					delete[] retPtr;
					continue;
				}
				char * returns;
				int nb_returns = 0;
				while ((returns = strtok(NULL, ",")) != NULL)
				{	
					returns_table[nb_returns] = atof(returns);
					nb_returns++;
				}
				delete[] retPtr;

				this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[0];

				double discount_factor_inv = returns_table[this->yield_column_for_pvs - 1];				
				if (discount_factor_inv < SMALL_DOUBLE)
				{
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
				}
				else
				{
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;
				}

				// TO DO check this line . Dynamic lapse treas
				if (this->lapse_dyn_treas_col > 0)
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[max(lapse_dyn_treas_col - 1, 0)];
				else
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = period;

				// Calculate the fund returns
				int index_col;
				for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
				{
					// fund returns
					index_col = this->scen_fund_col[fund_num] - 1;
					this->monthly_rates[iteration][period][fund_num] = returns_table[index_col]/100.0;
				}
			}
		}
		file_op.close();

		//this->print_monthly_rates();
	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The scenario file %s is not found", file_name.c_str());
		throw new GMXBException(message);
	}

	// display_ann_from_mthly("file_mode");
	delete[] returns_table;
}
scenario_manager::scenario_manager(int number_of_scen, vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int& t_high_shorter, const string& file_name, int time_step)
{

	this->number_of_scenarios = number_of_scen;

	this->number_of_indexes = index_num;
	t_high_shorter = init_t_high_shorter(file_name);

	this->period = t_high_shorter*12+1;
	this->subacct_max_period =  t_high_shorter+1;

	this->time_step=time_step;
	if (this->time_step==ANNUAL)
		this->create_ann_from_monthly_table();
	else 
		this->create_monthly_rates_table();

	set_columns(scen_fund_col_vector, yield_column_for_pvs_index, lapse_dyn_treas_col_index, rates_indic_index);

	this->nb_rate=0;
}

scenario_manager::scenario_manager(int number_of_scen, vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int& t_high_shorter, const string& file_name, int time_step, int use_scenario_file_inner,int t_high_local)
{
	
	this->number_of_scenarios = number_of_scen;

	this->number_of_indexes = index_num;

	if(use_scenario_file_inner==1)
		t_high_shorter = init_t_high_shorter(file_name);
	else
		t_high_shorter = t_high_local;

	this->period = t_high_shorter*12+1;

	this->subacct_max_period =  t_high_shorter+1;

	this->time_step = time_step;

	if (this->time_step == ANNUAL)
		this->create_ann_from_monthly_table();
	else 
		this->create_monthly_rates_table();

	set_columns(scen_fund_col_vector, yield_column_for_pvs_index, lapse_dyn_treas_col_index, rates_indic_index);

	this->nb_rate=0;
}
scenario_manager::scenario_manager()
{
	// For actuarial tool
	this->monthly_rates = 0;
	this->ann_from_monthly_rates = 0;
	this->nb_rate=0;
	this->scen_fund_col = 0;

}
/*
* Initialisation of the scenario manager when the esg mode is used
* 
*/
//! Yuan 
scenario_manager::scenario_manager(double ***va_scen,
								   int number_of_scen,
								   vector<int>& scen_fund_col_vector, 
								   int yield_column_for_pvs_index,
								   int lapse_dyn_treas_col_index,
								   int rates_indic_index,
								   int index_num,
								   int t_high_shorter, 
								   int time_step)  
{
	this->subacct_max_period =  t_high_shorter+1; 

	this->va_scenario = va_scen;
	
	this->number_of_scenarios = number_of_scen;
	
	this->period = t_high_shorter*12+1;

	this->number_of_indexes = index_num;

	this->time_step=time_step;
	
	if (this->time_step==ANNUAL)
		this->create_ann_from_monthly_table();
	else 
		this->create_monthly_rates_table();

	set_columns(scen_fund_col_vector, yield_column_for_pvs_index, lapse_dyn_treas_col_index, rates_indic_index);
	this->nb_rate=0;
}


/*
* Set the column number for all the equities, bonds, discount_factor ...
* Convert rates from monthly to annualy
*/
void scenario_manager::set_columns(vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index)
{

	int number_of_funds = scen_fund_col_vector.size();

	this->scen_fund_col = new int[number_of_funds];

	for (int i=0; i <number_of_funds; i++)
			this->scen_fund_col[i] = 0;

	for (int fund_num = 0; fund_num < number_of_funds ; fund_num++)
	{
		this->scen_fund_col[fund_num] = scen_fund_col_vector[fund_num];
	}
	this->yield_column_for_pvs = yield_column_for_pvs_index;
	this->lapse_dyn_treas_col = lapse_dyn_treas_col_index;
	this->rates_indic = rates_indic_index;	
}


void scenario_manager::init_scenario_manager(int reset_value)
{
	if (this->time_step == ANNUAL)
	{
		this->init_ann_from_monthly_table(reset_value);
	}
	else 
	{
		this->init_monthly_rates_table(reset_value);
	}
}
void scenario_manager::init_scenario_manager_ESG(int reset_value, bool if_he_inner_mode)
{
	if (this->time_step == ANNUAL)
	{
		this->init_ann_from_monthly_table_ESG(reset_value,if_he_inner_mode);
	}
	else 
	{
		this->init_monthly_rates_table_ESG(reset_value,if_he_inner_mode);
	}
}

void scenario_manager::init_ann_from_monthly_table(int reset_value)
{
	this->reset_ann_from_monthly_table(reset_value);

	for (int scen_num = 0; scen_num < this->number_of_scenarios;  scen_num++)
	{
		int index_col = 0;
		for (int col = 1; col <= DECALAGE_MAX ; col++)
		{
			for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
			{	
				index_col = this->scen_fund_col[fund_num] - 1;
				double inv_montant_ann_cumul = 1.0;	
				for (int mth_range = 0; mth_range <= 12 - col; mth_range++)
				{
					int scen_time = min(mth_range + 1, this->period - 1);
					double inv_perc_mthly = this->va_scenario[scen_num][scen_time][index_col];
					inv_montant_ann_cumul =  inv_montant_ann_cumul *(1.0 + inv_perc_mthly/100.0);

				}//end for mth range
			this->ann_from_monthly_rates[scen_num][col][1][fund_num] = 
									pow (inv_montant_ann_cumul , 1.0 /this->rates_indic)  - 1.0; 
			 }//end fundnum	

			// Fill in ann_from_mthly_rates[col][1][13] with discount factor cum value
			int row_lookup_dfc = min( 13 - col  , this->period - 1);
			double discount_factor_inv = this->va_scenario[scen_num][row_lookup_dfc][this->yield_column_for_pvs - 1];
			if (discount_factor_inv < SMALL_DOUBLE)
				this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
			else
				this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	
			index_col = max (this->lapse_dyn_treas_col - 1 , 0);
			this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES]= 
										this->va_scenario[scen_num][row_lookup_dfc][index_col];									
		} // End for col

		for (int tt = 2; tt <= this->subacct_max_period; tt++)			
		{
			for (int col = 1; col <= DECALAGE_MAX ; col++)
			{
				for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
				{
					index_col = this->scen_fund_col[fund_num] - 1;
					double inv_montant_ann_cumul = 1.0;
					for (int mth_range = 0; mth_range < DECALAGE_MAX ; mth_range++)
					{	
						int scen_time = min((tt-1)*12 + mth_range + 2 - col,this->period - 1) ;
						double inv_perc_mthly = 0.0;
						inv_perc_mthly = this->va_scenario[scen_num][scen_time][index_col];
						inv_montant_ann_cumul =  inv_montant_ann_cumul *(1.0 + inv_perc_mthly/100.0);
					}
					this->ann_from_monthly_rates[scen_num][col][tt][fund_num] 
						= pow(inv_montant_ann_cumul, 1.0 / this->rates_indic) - 1.0;
				}//end for fundmap...

				int scen_time = min(tt*12 - col + 1 , this->period -1);
				double discount_factor_inv = this->va_scenario[scen_num][scen_time][this->yield_column_for_pvs - 1];

				if (discount_factor_inv < SMALL_DOUBLE)
					this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
				else
					this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 
															1.0 / discount_factor_inv;	
				index_col = max (this->lapse_dyn_treas_col - 1 , 0);
				this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES]// get the government rates for belgian dynamic lapses ITM
					=	this->va_scenario[scen_num][scen_time][index_col];
				}//end for col				
		}//end for tt
	}
}

void scenario_manager::init_monthly_rates_table_ESG(int reset_value, bool if_he_inner_mode)


{
	unsigned int index_table_size = 0;

	if (if_he_inner_mode == true)
		index_table_size = 2;
	else
		index_table_size = MAXIMUM_NUMBER_OF_INDEXES;

	this->reset_monthly_rates_table(reset_value);
	for (int scen_num = 0; scen_num < this->number_of_scenarios;  scen_num++)
	{
		int index_col = 0;
		for (int tt = 1; tt <= (this->subacct_max_period-1)*12; tt++)	//Change : Ali		
		{	
			for (int fund_num = 0; fund_num < index_table_size ; fund_num++)
			{	
				index_col = this->scen_fund_col[fund_num] - 1;
				this->monthly_rates[scen_num][tt][fund_num] = this->va_scenario[index_col+2][tt][scen_num]/100.0;
			}//end fundnum	

			double discount_factor_inv = this->va_scenario[this->yield_column_for_pvs - 1 + 2][tt][scen_num];

			if (discount_factor_inv < SMALL_DOUBLE)
				this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
			else
				this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	

			index_col = max (this->lapse_dyn_treas_col - 1 , 0);
			this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES]// get the government rates for belgian dynamic lapses ITM
			 =	this->va_scenario[index_col+2][tt][scen_num];

		}//end for tt
	}
}

void scenario_manager::init_ann_from_monthly_table_ESG(int reset_value, bool if_he_inner_mode)


{
	unsigned int index_table_size = 0;

	if (if_he_inner_mode == true)
		index_table_size = 2;
	else
		index_table_size = MAXIMUM_NUMBER_OF_INDEXES;

	this->reset_ann_from_monthly_table(reset_value);
	for (int scen_num = 0; scen_num < this->number_of_scenarios;  scen_num++)
	{
		int index_col = 0;
		for (int col = 1; col <= DECALAGE_MAX ; col++)
		{
			for (int fund_num = 0; fund_num < index_table_size ; fund_num++)
			{	
				index_col = this->scen_fund_col[fund_num] - 1;
				double inv_montant_ann_cumul = 1.0;	
				for (int mth_range = 0; mth_range <= 12 - col; mth_range++)
				{
					int scen_time = min(mth_range + 1, this->period - 1);
					double inv_perc_mthly = this->va_scenario[index_col+2][scen_time][scen_num];
					inv_montant_ann_cumul =  inv_montant_ann_cumul *(1.0 + inv_perc_mthly/100.0);	
				}//end for mth range
			this->ann_from_monthly_rates[scen_num][col][1][fund_num] = 
									pow (inv_montant_ann_cumul , 1.0 /this->rates_indic)  - 1.0; 
			 }//end fundnum	

			// Fill in ann_from_mthly_rates[col][1][13] with discount factor cum value
			int row_lookup_dfc = min( 13 - col  , this->period - 1);

			double discount_factor_inv = this->va_scenario[this->yield_column_for_pvs - 1+2][row_lookup_dfc][scen_num];
			if (discount_factor_inv < SMALL_DOUBLE)
				this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
			else
				this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	
			index_col = max (this->lapse_dyn_treas_col - 1 , 0);
			this->ann_from_monthly_rates[scen_num][col][1][MAXIMUM_NUMBER_OF_INDEXES]= 
			this->va_scenario[index_col+2][row_lookup_dfc][scen_num];

		} // End for col

		for (int tt = 2; tt <= this->subacct_max_period; tt++)			
		{
			for (int col = 1; col <= DECALAGE_MAX ; col++)
			{
				for (int fund_num = 0; fund_num < index_table_size; fund_num++)
				{
					index_col = this->scen_fund_col[fund_num] - 1;
					double inv_montant_ann_cumul = 1.0;
					for (int mth_range = 0; mth_range < DECALAGE_MAX ; mth_range++)
					{	
						int scen_time = min((tt-1)*12 + mth_range + 2 - col,this->period - 1) ;
						double inv_perc_mthly = 0.0;
				
						inv_perc_mthly = this->va_scenario[index_col+2][scen_time][scen_num];

						inv_montant_ann_cumul =  inv_montant_ann_cumul *(1.0 + inv_perc_mthly/100.0);
					}

					this->ann_from_monthly_rates[scen_num][col][tt][fund_num] 
						= pow(inv_montant_ann_cumul, 1.0 / this->rates_indic) - 1.0;
				}//end for fundmap...

				int scen_time = min(tt*12 - col + 1 , this->period -1);
	
				double discount_factor_inv = this->va_scenario[this->yield_column_for_pvs - 1 +2][scen_time][scen_num];

				if (discount_factor_inv < SMALL_DOUBLE)
					this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
				else
					this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 
															1.0 / discount_factor_inv;	
				index_col = max (this->lapse_dyn_treas_col - 1 , 0);
				// get the government rates for belgian dynamic lapses ITM
				this->ann_from_monthly_rates[scen_num][col][tt][MAXIMUM_NUMBER_OF_INDEXES] = 
				this->va_scenario[index_col+2][scen_time][scen_num];

			}//end for col				
		}//end for tt
	}
}
void scenario_manager::init_monthly_rates_table(int reset_value)
{

	int index_col;
	this->reset_monthly_rates_table(reset_value);
	for (int scen_num = 0; scen_num < this->number_of_scenarios;  scen_num++)
	{							

		for (int tt = 1; tt <= (this->subacct_max_period-1)*12; tt++)	//Change : Ali		
		{

			for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
			{
				index_col = this->scen_fund_col[fund_num] - 1;
				double inv_montant_ann_cumul = 1.0;
				for (int mth_range = 0; mth_range < DECALAGE_MAX ; mth_range++)
				{	
					double inv_perc_mthly = 0.0;
					inv_perc_mthly = this->va_scenario[index_col+2][tt][scen_num];

					inv_montant_ann_cumul =  inv_montant_ann_cumul *(1.0 + inv_perc_mthly/100.0);
				}
				this->monthly_rates[scen_num][tt][fund_num] = pow(inv_montant_ann_cumul, 1.0 / this->rates_indic) - 1.0;
			}//end for fundmap...

			double discount_factor_inv = this->va_scenario[this->yield_column_for_pvs - 1 + 2][tt][scen_num];

			if (discount_factor_inv < SMALL_DOUBLE)
				this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
			else
				this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	

			index_col = max (this->lapse_dyn_treas_col - 1 , 0);
			this->monthly_rates[scen_num][tt][MAXIMUM_NUMBER_OF_INDEXES]// get the government rates for belgian dynamic lapses ITM
			 =	this->va_scenario[index_col+2][tt][scen_num];
		}//end for tt
	}
}


void scenario_manager::init_ann_from_monthly_table_he(const string& file_name, int reset_value, double * swap_table, int nb_date)
{
	this->reset_ann_from_monthly_table(reset_value);
	string row;
	string firstline;
	ifstream file_op(file_name.c_str() , ios::in);
	char *retPtr;
	int iteration = 0;
	int period = 0;
	double *returns_table;
	returns_table = new double[this->number_of_indexes + 3];
	for (int i = 0; i < this->number_of_indexes + 3; i++)
	{
		returns_table[i] = 0.;
	}

	if (file_op)
	{
		// Skip the first line
		getline(file_op, firstline); 
		while(iteration < this->number_of_scenarios && !file_op.eof())
		{
			getline(file_op,row);
			if (row.length() > 0)
			{
				// original version
				retPtr = new char[row.length() + 1];
				strcpy(retPtr, row.c_str());
				retPtr[row.length()] = '\0';
				int col = 0;
				iteration = atoi(strtok(retPtr, ",")) - 1;
				period = atoi(strtok(NULL, ","));
				
				// Skip the period 0
				if (period == 0)
				{
					delete[] retPtr;
					continue;
				}
				int tt = (int)(floor((period-1)/12.) + 1);
				char * returns;
				int nb_returns = 0;
				while ((returns = strtok(NULL, ",")) != NULL)
				{	
					returns_table[nb_returns] = atof(returns);
					nb_returns++;
				}
				delete[] retPtr;

				// Calculate the fund returns
				double discount_factor_inv = returns_table[this->yield_column_for_pvs - 1];

				int index_col;
				for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
				{
					// fund returns
					index_col = this->scen_fund_col[fund_num] - 1;
					for (int col = 1; col <= DECALAGE_MAX - ((period-1) % 12) ; col++)
					{	
						this->ann_from_monthly_rates[iteration][col][tt][fund_num] =  
							this->ann_from_monthly_rates[iteration][col][tt][fund_num] * (1.0 + returns_table[index_col]/100.0);

						if (discount_factor_inv < SMALL_DOUBLE)
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
						else
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;	

						// TO DO check this line . Dynamic lapse treas
						if (this->lapse_dyn_treas_col > 0)
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[max(lapse_dyn_treas_col - 1, 0)];
						else
							this->ann_from_monthly_rates[iteration][DECALAGE_MAX - (period-1)%12][tt][MAXIMUM_NUMBER_OF_INDEXES] = period;
					}

					for (int col = DECALAGE_MAX - ((period-1) % 12) + 1; col <= DECALAGE_MAX ; col++)
					{
						if (tt < SUBACCT_MAX_PERIOD)
						{
							this->ann_from_monthly_rates[iteration][col][tt + 1][fund_num] =  
								this->ann_from_monthly_rates[iteration][col][tt + 1][fund_num] * (1.0 + returns_table[index_col]/100.0);
						}
					}
				}
			}
		}
		for (int it = 0; it < number_of_scenarios ; it++)
		{
			for (int tt = 1; tt <= SUBACCT_MAX_PERIOD ; tt++)
			{
				for (int col = 1; col <= DECALAGE_MAX ; col++)
				{
					for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
					{					
						this->ann_from_monthly_rates[it][col][tt][fund_num] = 
							pow(this->ann_from_monthly_rates[it][col][tt][fund_num], 1.0 / this->rates_indic) - 1.0;
					}
				}
			}
		}
		file_op.close();

	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The scenario file %s is not found", file_name.c_str());
		throw new GMXBException(message);
	}
	
	delete[] returns_table;

}
void scenario_manager::init_monthly_rates_table_he(const string& file_name, int reset_value, double * swap_table, int nb_date)
{

	int nb_rate=0;
	for (int i=0; i<nb_date; i++)
	{
		if (swap_table[i]!=0)
			nb_rate++;
	}
	this->nb_rate=nb_rate;
	int * rates = new int[nb_rate];
	int index=0;
	for (int i=0; i<nb_date; i++)
	{
		if (swap_table[i]!=0)
		{
			rates[index]=(int)swap_table[i];
			index++;
		}
	}

	this->swap_table = new double**[this->number_of_scenarios];
	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		this->swap_table[it] = new double *[this->subacct_max_period*12 + 1];
		for (int i = 0; i <= this->subacct_max_period*12; i++)
		{
			// The + 3 is due to period, scenario and short rates columns
			this->swap_table[it][i] = new double[nb_rate];
		}
	}


	this->reset_monthly_rates_table(reset_value);
	string row;
	string firstline;
	ifstream file_op(file_name.c_str() , ios::in);
	char *retPtr;
	int iteration = 0;
	int period = 0;
	double *returns_table;
	returns_table = new double[this->number_of_indexes + 3];
	for (int i = 0; i < this->number_of_indexes + 3; i++)
	{
		returns_table[i] = 0.;
	}

	if (file_op)
	{
		// Skip the first line
		getline(file_op, firstline); 
		while(iteration < this->number_of_scenarios && !file_op.eof())
		{
			getline(file_op,row);
			if (row.length() > 0)
			{
				// original version
				retPtr = new char[row.length() + 1];
				strcpy(retPtr, row.c_str());
				retPtr[row.length()] = '\0';
				int col = 0;
				iteration = atoi(strtok(retPtr, ",")) - 1;
				period = atoi(strtok(NULL, ","));
				
				if (iteration >= this->number_of_scenarios)
				{
					delete[] retPtr;
					continue;
				}

				char * returns;
				int nb_returns = 0;
				while ((returns = strtok(NULL, ",")) != NULL)
				{	
					returns_table[nb_returns] = atof(returns);
					nb_returns++;
				}
				delete[] retPtr;

				this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[0];

				double discount_factor_inv = returns_table[this->yield_column_for_pvs - 1];				
				if (discount_factor_inv < SMALL_DOUBLE)
				{
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES + 1]=0.0; 
				}
				else
				{
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES + 1] = 1.0 / discount_factor_inv;
				}

				// TO DO check this line . Dynamic lapse treas
				if (this->lapse_dyn_treas_col > 0)
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = returns_table[max(lapse_dyn_treas_col - 1, 0)];
				else
					this->monthly_rates[iteration][period][MAXIMUM_NUMBER_OF_INDEXES] = period;

				// Calculate the fund returns
				int index_col;
				for (int fund_num = 0; fund_num < MAXIMUM_NUMBER_OF_INDEXES; fund_num++)
				{
					// fund returns
					index_col = this->scen_fund_col[fund_num] - 1;
					this->monthly_rates[iteration][period][fund_num] = returns_table[index_col]/100.0;
				}

				for (int i=0; i<nb_rate; i++)
				{
					//this->swap_table[iteration][period][i]=returns_table[rates[i]-1];
					this->swap_table[iteration][period][i]=returns_table[i];
				}
			}
		}
		file_op.close();
	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The scenario file %s is not found", file_name.c_str());
		throw new GMXBException(message);
	}
	delete[] returns_table;
	delete[] rates;
}

double*** scenario_manager::get_ann_from_mthly_rates(int it_number)
{	
	return this->ann_from_monthly_rates[it_number-1];
}

double** scenario_manager::get_monthly_rates(int it_number)
{	
	return this->monthly_rates[it_number-1];
}


double** scenario_manager::get_swap_table(int it_number)
{	
	return this->swap_table[it_number-1];
}

int scenario_manager::get_nb_scenarios()
{
	return this->number_of_scenarios;
}

void scenario_manager::display_ann_from_mthly(char * file_name)
{
	char name[MAX_LENGTH_MESSAGE_EXCEPTION];
	sprintf(name, "C:\\%s.txt", file_name);
	ofstream log_ann;
	log_ann.open(name);

	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for(int i = 0; i < DECALAGE_MAX +1; i++)
		{
			for (int j = 0; j <= this->subacct_max_period; j++)
			{
				for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES + 3; k++)
				{
					log_ann  << this->ann_from_monthly_rates[it][i][j][k] << ",";				
				}
				log_ann << endl;
			}
		}
	}
}

void scenario_manager::display_va_scenario(char * file_name)
{
	char name[MAX_LENGTH_MESSAGE_EXCEPTION];
	sprintf(name, "C:\\%s.txt", file_name);
	ofstream log_ann;
	log_ann.open(name);

	for(int it = 0; it < this->number_of_scenarios ; it++)
	{
		for (int j = 0; j <= period - 1 ; j++)
		{
			for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES + 3; k++)
			{
				log_ann  << this->va_scenario[it][j][k] << ",";				
			}
			log_ann << endl;
		}
	}
}


int scenario_manager :: init_t_high_shorter(const string& file_name)
{
	string row;
	string firstline;
	ifstream file_op(file_name.c_str() , ios::in);
	char *retPtr;
	int iteration = 0;
	int period = 0;
	double *returns_table;
	returns_table = new double[this->number_of_indexes + 3];

	for (int i = 0; i < this->number_of_indexes + 3; i++)
	{
		returns_table[i] = 0.;
	}

	if (file_op)
	{
		int period_temp = -1;
		// Skip the first line
		getline(file_op, firstline); 
		while(iteration < this->number_of_scenarios && !file_op.eof())
		{
			getline(file_op,row);
			if (row.length() > 0)
			{
				// original version
				retPtr = new char[row.length() + 1];
				strcpy(retPtr, row.c_str());
				retPtr[row.length()] = '\0';
				int col = 0;
				iteration = atoi(strtok(retPtr, ",")) - 1;
				period = atoi(strtok(NULL, ","));
				
				// Skip the period 0
				if (period == 0)
				{
					delete[] retPtr;
					continue;
				}
				if(period_temp < period )
				{
					if(retPtr != 0)
						delete[] retPtr;

					period_temp = period;
				}
				else
				{
					if(retPtr != 0)
						delete[] retPtr;
					break;
				}
			}
		}

		if(period_temp%12!=0 || period_temp > 720)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Error in function in scenario_manager :: init_t_high_shorter, (file_mode) period_temp%12!=0 || period_temp > 720" );
			throw new GMXBException(message);
		}
		else
		{
			file_op.close();
			delete[] returns_table;
			return period_temp/12;
		}
	}
	else 
	{
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf(message, "The scenario file %s is not found", file_name.c_str());
		throw new GMXBException(message);
	}
}
void scenario_manager ::set_va_scenario(double ***va_scen)
{
	this->va_scenario = va_scen;
}

void scenario_manager::read_growth_for_aging_policies(const string& file_name,
													  int reset_value, 
													  double * growth_per_mp, 
													  int nb_of_mp_outers)
{
	string row;
	char *retPtr;
	ifstream file_op(file_name.c_str() , ios::in);

	int i = 0;
	int line = 0;

	while(!file_op.eof() && line < nb_of_mp_outers)
	{
		getline(file_op,row);
		if (row.length() > 0)
		{
			retPtr = new char[row.length() + 1];
			strcpy(retPtr, row.c_str());
			retPtr[row.length()] = '\0';
			growth_per_mp[line] = atof(strtok(retPtr, ","));
			line++;
			delete [] retPtr;
		}
		i++;
	}
}


