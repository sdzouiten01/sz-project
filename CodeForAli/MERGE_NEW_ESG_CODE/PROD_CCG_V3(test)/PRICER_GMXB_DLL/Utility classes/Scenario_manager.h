#pragma once
#include "../Utility classes/stdafx.h"
//#include "Utility classes\GMXBLogger.h"
#include "head.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include "GMXBException.h"


using namespace std;

extern ofstream trace;

class scenario_manager
{
public	:
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
		
		//// Variables for Hedge Effectivness
		//double *** swap_table;
		//int nb_rate;

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

		
		
		
		
		/** Get the total number of scenario to run*/
		int get_nb_scenarios();		
		
		double ** get_monthly_rates(int it_number);		
		
		
		
		
		
		
		



		
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
		void set_columns(vector<int>& scen_fund_col_vector, int yield_column_for_pvs_index, int lapse_dyn_treas_col_index, int rates_indic_index);

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
	
		
		



		/*
		* Destructor of the class. (Delete ann_from_monthly_rates rates table)
		*/
		~scenario_manager();

		/*
		* Constructor when the file mode is used
		*/
		scenario_manager();
	
		scenario_manager(int number_of_scen, vector<int> scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int& t_high_shorter, const string& file_name, int time_step);

		/*
		* Constructor when the esg mode is used
		*/
		scenario_manager(double ***va_scen, int number_of_scen, vector<int> scen_fund_col_vector, int yield_column_for_pvs_index, 
			int lapse_dyn_treas_col_index, int rates_indic_index, int index_num, int t_high_shorter, int time_step);
	
		void init_scen_vector(double ***va_scen);
		
		
		
		
		/*
		* Calculate the annual rates from a given scenario file name
 		*/
		void init_scenario_manager(const string& file_name, int reset_value);

		/*
		* Calculate the annual rates from the va_scenario
 		*/
		void init_scenario_manager(int reset_value);
		//void init_scenario_manager_he(const string& file_name, int reset_value, double* swap_table, int nb_date);

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


				
};


