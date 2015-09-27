#pragma once

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "tinyxml.h"
#include "tinystr.h"
#include <vector>
#include <time.h>
#include "model_point_head.h"
#include "GMXBParameters.h"
#include "GMXB_Assumption_Table.h"
#include "GMXBLogger.h"
#include "GMXBException.h"
#include "Utility classes/Fund.h"
#include "Utility classes/Fund_Manager.h"
#include "Utility classes/Scenario_configuration.h"
#include "Utility classes/Product_configuration.h"
using namespace std;

extern ofstream trace;

class Model_Point
{
	/*
	* Structure used when we load the 
	* last columns of the inforce containing the 
	* the fund characteristics. This specific structure
	* is dedicated to statinforce
	*/
public:
	struct model_point_fund_stat{
		string fund_indicator;  
  		double nb_share;  
		string fund_name;   
		double fund_value; 
		double total_value;

		void stat_fund_in_vector(model_point_fund_stat& m)
		{
			this->nb_share    += m.nb_share;
			this->total_value += m.total_value;
		}

		bool operator ==(const Model_Point::model_point_fund_stat& m) const 
		{
			if(this->fund_name== m.fund_name)
				return true;
			else 
				return false;
		}
		/*
		*	static bool fund_name_preindicate(model_point_fund_stat& m1, model_point_fund_stat& m2)
		*	{
		*		if(m1.fund_name == m2.fund_name)
		*			return true;
		*		else
		*			return false;
		*	}
		*
		*	void print()
		*	{
		*		cout << "fund_indicator = " << fund_indicator<< endl;
		*		cout << "fund_name  = "     << fund_name<< endl;
		*		cout << "nb_share = "       << nb_share << endl;
		*		cout << "fund_value  = "    << fund_value<< endl;
		*	    cout << "total_value  = "   << total_value<< endl;
		*	}
		*/
	};

protected:
	/*
	* Structure used when we load the 
	* last columns of the inforce containing the 
	* the fund characteristics
	*/
	struct model_point_fund{
		Fund fund;
		double alpha_t[SUBACCT_MAX_PERIOD];
		double return_fund_t[SUBACCT_MAX_PERIOD];
		double number_share;
		double target_weight;
		string fund_type;
		bool is_short_sellable_fund;
	};

protected :
	/*
	* Add or update a fund in the list of funds of this model point
	* For REBALANCE model point, we have 2 indicators A and T
	* For DRIFT model point, we have only 1 indicator which is A
	*/	
	void add_or_update_fund(const string& fund_name, const string& fund_type, double nb_share, 
		double fund_value, Product_configuration& product_config)
	{
		// Check if the fund name
		// exist
		bool is_short_sellable;
		
		Fund f;
		try
		{
			f = product_config.get_fund_by_name(fund_name);
			is_short_sellable = f.check_group_shortsell_id(product_config.get_shortsellable_group());
		}
		catch (GMXBException* e)
		{
			delete e;
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, " The fund name %s for policy holder : %s is unknown", fund_name.c_str(),this->policy_id.c_str());
			throw new GMXBException(message);			
		}

		if (fund_type!="A" && fund_type!="T")
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, " The fund name %s is missing A or T indicator found (%s) (policy id : %s)", fund_name.c_str(), fund_type.c_str(), this->policy_id.c_str());
			throw new GMXBException(message);								
		}

		// Check if the fund value
		// in the inforce is consistent with 
		// the fund value in the param file
		if (abs(f.get_fund_value() - fund_value) > 0.01)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, " Inconsistency: The fund value in the in force file for policy holder: %s  and fund value %s is %f. the value loaded in the param file is %f", 
				this->policy_id.c_str(), fund_name.c_str(), fund_value, f.get_fund_value());
			throw new GMXBException(message);									
		}
		
		// if (this->av_rebal_ind == "REBALANCE")
		if (this->av_rebal_ind == Model_Point::REBALANCE)
		{
			// Find the fund in list_of_funds 
			bool  found = false;
			unsigned int mp_f;
			for (mp_f = 0; mp_f < this->list_of_funds.size(); mp_f++)
			{
				if (this->list_of_funds[mp_f].fund.alh_fund_id == fund_name)
				{
					found = true;
					break;
				}
			}
			// model point fund doesn't exist in list_of_funds
			// Add it
			if (!found)
			{
				model_point_fund mp_fund;
				mp_fund.fund = f;
				mp_fund.number_share = 0;
				mp_fund.target_weight = 0;
				mp_fund.is_short_sellable_fund = is_short_sellable;
				mp_fund.fund_type = fund_type;
				if (fund_type == "A")
					mp_fund.number_share = nb_share;
				else
				{
					mp_fund.target_weight = nb_share;
				}
				
				memset(mp_fund.alpha_t, 0, sizeof(mp_fund.alpha_t));
				memset(mp_fund.return_fund_t, 0, sizeof(mp_fund.return_fund_t));
				
				this->list_of_funds.push_back(mp_fund);
			}
			// model point fund exist in list_of_funds
			// Update it
			else
			{
				if (fund_type == "A")
					this->list_of_funds[mp_f].number_share = nb_share;
				else
				{
					this->list_of_funds[mp_f].target_weight = nb_share;
				}
				this->list_of_funds[mp_f].fund_type = "T";
			}
		}

		// else if (this->av_rebal_ind == "DRIFT")
		else if (this->av_rebal_ind == Model_Point::DRIFT)
		{
			// Raise an exception if the 
			// fund_type is not of type A
			if (fund_type != "A")
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, " The fund type allowed for DRIFT model point is A (found %s) (fund name : %s) (model point id %s)", 
							fund_type.c_str(), fund_name.c_str(), policy_id.c_str());
				throw new GMXBException(message);								
			}

			model_point_fund mp_fund;
			mp_fund.fund = f;
			mp_fund.fund_type = "A";
			mp_fund.number_share = nb_share;
			mp_fund.is_short_sellable_fund = is_short_sellable;
			memset(mp_fund.alpha_t, 0, sizeof(mp_fund.alpha_t));
			memset(mp_fund.return_fund_t, 0, sizeof(mp_fund.return_fund_t));
			this->list_of_funds.push_back(mp_fund);
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, " The rebalance indicator should be either REBALANCE or DRIFT ");
			throw new GMXBException(message);								
		}
	}

	/*
	*	This function parse the inforce file line 
	*	that contains informations about the funds
	*   the pattern is : fund_id1; Nb_share_fund1; Nb_share_fund1*Value_fund1
	*/
	void parse_fund_line(const string& fund_line, Product_configuration& product_config)
	{
		if (this->load_model_point_funds)
		{
			this->total_fund_values = 0;
	 		if (fund_line.length() > 0)
			{
				istringstream all_model_point_funds;
				all_model_point_funds.str(fund_line);
				string one_fund;
				// Loop on all the funds
				while (getline(all_model_point_funds, one_fund, '*'))
				{

					istringstream fund_attributes;
					fund_attributes.str(one_fund);
					string fund_name, nb_share_str, fund_value_str, fund_indicator, total_value_str;
					getline(fund_attributes, fund_name, ';');
					getline(fund_attributes, nb_share_str, ';');
					getline(fund_attributes, fund_value_str, ';');
					getline(fund_attributes, total_value_str, ';');
					getline(fund_attributes, fund_indicator, ';');

					this->add_or_update_fund(fund_name, fund_indicator, atof(nb_share_str.c_str()), 
						atof(fund_value_str.c_str()), product_config);

				}

				for (unsigned int i = 0; i < this->list_of_funds.size(); i++)
				{
					this->total_fund_values += this->list_of_funds[i].number_share * this->list_of_funds[i].fund.get_fund_value();
				}
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "No information about funds for policy holder : %s ", this->policy_id.c_str());
				throw new GMXBException(message);			
			}

			// Compute alphas at t=0 for 
			// all the funds
			for (unsigned int i = 0; i < this->list_of_funds.size(); i++)
			{
				this->list_of_funds[i].alpha_t[0] =
								(this->list_of_funds[i].number_share 
											* this->list_of_funds[i].fund.get_fund_value()/this->total_fund_values);
			}
		}
	}

	void parse_fund_line_stat(const string& fund_line)
	{
			this->stat_list_of_funds.clear();

	 		if (fund_line.length() > 0)
			{
				istringstream all_model_point_funds;
				all_model_point_funds.str(fund_line);
				string one_fund;
				// Loop on all the funds
				while (getline(all_model_point_funds, one_fund, '*'))
				{

					istringstream fund_attributes;
					fund_attributes.str(one_fund);
					string fund_name, nb_share_str, fund_value_str, fund_indicator, total_value_str;
					getline(fund_attributes, fund_name, ';');
					getline(fund_attributes, nb_share_str, ';');
					getline(fund_attributes, fund_value_str, ';');
					getline(fund_attributes, total_value_str, ';');
					getline(fund_attributes, fund_indicator, ';');

					model_point_fund_stat static_fund;

					static_fund.fund_indicator  = fund_indicator;
					static_fund.fund_name		= fund_name;
					static_fund.nb_share		= atof(nb_share_str.c_str());
					static_fund.fund_value		= atof(fund_value_str.c_str());
					static_fund.total_value		= static_fund.fund_value * static_fund.nb_share;

					this->stat_list_of_funds.push_back(static_fund);
				}
			}
			/*else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "No information about funds for policy holder : %s ", this->policy_id.c_str());
				throw new GMXBException(message);			
			}*/
		}
public:
	// table of present values
		double ** pv_lapse;
		double ** pv_mortality;
		double ** pv_paidup;
		double ** pv_longevity;
		double ** pv_election;
		int lapse_size;
		int mortality_size;
		int paidup_size; //AJOUT
		int longevity_size;
		int election_size;
		double **index_shock_pvs;
		double **fund_shock_pvs;
		int index_shock_size;
		int fund_shock_size;
		string product;
		string group;
		string prod_code_key;
		string policy_id;
		double age_valn;
		double duration;
		string sex; 			
		double m_sex_perc; 		
		double gmab_period; 
		//string av_rebal_ind;
		int av_rebal_ind;
		double gmxb_a_c;
		double gmab_rollup_valn;
		double gmab_ratchet_valn;
		double gmab_rollup_rate;
		int gmab_av_period;
		double gmab_maturity_freq;
		double gmxb_ehc_charge;
		double gmdb_ratchet_valn;
		double gmdb_rollup_valn;
		int gmdb_av_period;
		double gmwb_ratchet_valn;
		double gmwb_rollup_valn;
		double gmib_ratchet_valn;
		double gmib_rollup_valn;
		double gmwb_ratchet_period;
		double gmwb_rollup_rate;
		double gmxb_g_c;
		char * date;
		int day;
		int month;
		int year;
		/*stat_inforces*/
		double realized_RRC;
		double admin_charges;

		/* MODIFICATION - 
		   Variable gmxb_type2 added for countries that have two differents tables in assumption */
		string gmxb_type2;
		double admin_fee_dollar;
		double gmxb_deferral_period;
		double prem_paid;
		double payt_freq;
		int gmsb_av_period;
		double gmsb_period;
		double gmxb_m_c;
		double mgt_fee_perc;
		double axa_roll_r;
		double year_prem;
		double av_if_array[MAXIMUM_NUMBER_OF_INDEXES];
		double av_split_prop_array[MAXIMUM_NUMBER_OF_INDEXES];
		int prem_durr;
		double fpw_accum_total;

		double gmwb_payt_yr;
		double gmwb_wdl_cum_dyn_lapse;
		double gmwb_wdl_rate;

		// NA - calculated fields
		double age_exact_issue; 
		int elapsed_months;
		string prem_type;
		double prem_curr;
		int valn_day;
		int valn_month;
		int valn_year;
		int elapsed_mths_in_valn_yr;
		int elapsed_years;
		double age_election_ddf_calculated;

		double fee_rp;
		double up_front_g_c;

		int mp_position_in_file;
		bool load_longstaff;

		bool load_model_point_funds;
		double av_split_target[MAXIMUM_NUMBER_OF_INDEXES];

		vector<model_point_fund> list_of_funds;
		vector<int> list_of_shortsellable_fund_ids;

		double total_fund_values;

		double total_av;
		
		double    Acqu_ch_t0;
		double    Acqu_ch_t1;
		double    Acqu_ch_t2;
		double    Acqu_ch_t3;
		double    Acqu_ch_t4;
		double    Acqu_ch_t5;
		double    Acqu_ch_t6;
		double    Acqu_ch_t7;
		double    Acqu_ch_t8;
		double    Acqu_ch_t9;
		double    Commission_charges;
		int		  Pay_months;
		int       Pay_status;
		int       Prem_elapsed_months;
		
		// Extraction date to use as a valuation date
		char * extraction_date;
		int birthday;
		int extraction_day;
		int extraction_month;
		int extraction_year;



		const static int REBALANCE = 1;
		const static int DRIFT = 2;
		vector<model_point_fund_stat> stat_list_of_funds;
	/*
	* Default constructor of the 
	* class.
	*/
	Model_Point()
	{
		this->get_timestamp();
		this->pv_lapse = 0;
		this->pv_mortality = 0;
		this->pv_paidup = 0;
		this->pv_longevity = 0;
		this->pv_election = 0;
		this->lapse_size = 0;
		this->mortality_size = 0;
		this->paidup_size = 0; //AJOUT
		this->election_size = 0;
		this->longevity_size = 0;
		this->fund_shock_pvs = 0;
		this->fund_shock_size = 0;
		this->index_shock_size = 0;
		this->index_shock_pvs = 0;

	}
	/*
	* Copy constructor of Model_Point class
	*/
	Model_Point(const Model_Point& mp)
	{
		this->index_shock_size = mp.index_shock_size;
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = mp.index_shock_pvs[i][j];
		}

		this->lapse_size = mp.lapse_size;

		this->pv_lapse = new double* [this->lapse_size];
		// initialize the pv table
		for (int i = 0; i < this->lapse_size; i++)
		{
			this->pv_lapse[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_lapse[i][j] = mp.pv_lapse[i][j];
		}
		
		// Initialise pv for mortality shocks
		this->mortality_size = mp.mortality_size;

		this->pv_mortality = new double* [this->mortality_size];
		// initialize the pv table
		for (int i = 0; i < this->mortality_size; i++)
		{
			this->pv_mortality[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_mortality[i][j] = mp.pv_mortality[i][j];
		}

		// Initialise pv for longevity shocks
		this->longevity_size = 1;

		this->pv_longevity = new double* [this->longevity_size];
		// initialize the pv table
		for (int i = 0; i < this->longevity_size; i++)
		{
			this->pv_longevity[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_longevity[i][j] = mp.pv_longevity[i][j];
		}
		// Initialise pv for election shocks
		this->election_size = 1;

		this->pv_election = new double* [this->election_size];
		// initialize the pv table
		for (int i = 0; i < this->election_size; i++)
		{
			this->pv_election[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_election[i][j] = mp.pv_election[i][j];
		}
		// Initialise pv for fund shocks

		this->fund_shock_size = mp.fund_shock_size;
		this->fund_shock_pvs = new double*[this->fund_shock_size];

		// initialize the fund pv table
		for (int i = 0; i < this->fund_shock_size; i++)
		{
			this->fund_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->fund_shock_pvs[i][j] = mp.fund_shock_pvs[i][j];
		}

	}

	/* TO DO : put this function in the AUS class and splitting code for initialize pvs in functions
	* New Constructor of the class to be used in MOdel_point_AUS to add pv_column 
	* vector<string>& row : vector of string corresponding to 1 line of the inforce file
	* int rank : rank of this model point in the whole inforce file
	* Product_configuration& product_conf : contain some information to build the model point object
	* Scenario_configuration& scenario_config : contain some information to build the model point object(size of the tables)
	* pv_nb : number of PV column
	*/
	Model_Point(vector<string>& row, int rank, Product_configuration& product_conf, 
					Scenario_configuration& scenario_config, int pv_nb)
	{	
		// Initialization of total account value
		this->total_av = 0.0;
		
		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->index_shock_pvs[i][j] = 0.;
		}

		// Initialise pv for lapse shocks
		this->lapse_size = scenario_config.get_lapse_mult().size();

		this->pv_lapse = new double* [this->lapse_size];
		// initialize the pv table
		for (int i = 0; i < this->lapse_size; i++)
		{
			this->pv_lapse[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->pv_lapse[i][j] = 0.;
		}
		
		// Initialise pv for mortality shocks
		this->mortality_size = scenario_config.get_mortality_mult().size();

		this->pv_mortality = new double* [this->mortality_size];
		// initialize the pv table
		for (int i = 0; i < this->mortality_size; i++)
		{
			this->pv_mortality[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->pv_mortality[i][j] = 0.;
		}

		// Initialise pv for paidup shocks
		this->paidup_size = scenario_config.get_paidup_mult().size();

		this->pv_paidup = new double* [this->paidup_size];
		// initialize the pv table
		for (int i = 0; i < this->paidup_size; i++)
		{
			this->pv_paidup[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->pv_paidup[i][j] = 0.;
		}
		// Initialise pv for longevity shocks
		this->longevity_size = 1;

		this->pv_longevity = new double* [this->longevity_size];
		// initialize the pv table
		for (int i = 0; i < this->longevity_size; i++)
		{
			this->pv_longevity[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->pv_longevity[i][j] = 0.;
		}
		// Initialise pv for election shocks
		this->election_size = 1;

		this->pv_election = new double* [this->election_size];
		// initialize the pv table
		for (int i = 0; i < this->election_size; i++)
		{
			this->pv_election[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->pv_election[i][j] = 0.;
		}

		// Initialise pv for fund shocks

		this->fund_shock_size = scenario_config.get_fund_shock_array().size();
		this->fund_shock_pvs = new double*[this->fund_shock_size];

		// initialize the fund pv table
		for (int i = 0; i < this->fund_shock_size; i++)
		{
			this->fund_shock_pvs[i] = new double[pv_nb];
			for (int j=0 ; j < pv_nb; j++)
				this->fund_shock_pvs[i][j] = 0.;
		}

		memset(this->av_if_array, 0, sizeof(this->av_if_array));
		memset(this->av_split_prop_array, 0, sizeof(this->av_split_prop_array));
		this->get_timestamp();
		initialise_mp(row);
		this->mp_position_in_file = rank;
		this->load_longstaff = product_conf.is_longstaff();
		this->load_model_point_funds = product_conf.is_projection_by_fund();
		this->parse_fund_line(row[row.size() - 1], product_conf);
		this->parse_fund_line_stat(row[row.size() - 1]);
	}


	/*
	* Assignment operator for Model_Point class
	*/
	Model_Point & operator = (const Model_Point& mp)
	{

		this->index_shock_size = mp.index_shock_size;
		this->index_shock_pvs = new double* [this->index_shock_size];
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = mp.index_shock_pvs[i][j];
		}

		this->lapse_size = mp.lapse_size;

		this->pv_lapse = new double* [this->lapse_size];
		// initialize the pv table
		for (int i = 0; i < this->lapse_size; i++)
		{
			this->pv_lapse[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_lapse[i][j] = mp.pv_lapse[i][j];
		}
		
		// Initialise pv for mortality shocks
		this->mortality_size = mp.mortality_size;

		this->pv_mortality = new double* [this->mortality_size];
		// initialize the pv table
		for (int i = 0; i < this->mortality_size; i++)
		{
			this->pv_mortality[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_mortality[i][j] = mp.pv_mortality[i][j];
		}

		// Initialise pv for paidup shocks
		this->paidup_size = mp.paidup_size;

		this->pv_paidup = new double* [this->paidup_size];
		// initialize the pv table
		for (int i = 0; i < this->paidup_size; i++)
		{
			this->pv_paidup[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_paidup[i][j] = mp.pv_paidup[i][j];
		}
		// Initialise pv for longevity shocks
		this->longevity_size = 1;

		this->pv_longevity = new double* [this->longevity_size];
		// initialize the pv table
		for (int i = 0; i < this->longevity_size; i++)
		{
			this->pv_longevity[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_longevity[i][j] = mp.pv_longevity[i][j];
		}
		// Initialise pv for election shocks
		this->election_size = 1;

		this->pv_election = new double* [this->election_size];
		// initialize the pv table
		for (int i = 0; i < this->election_size; i++)
		{
			this->pv_election[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->pv_election[i][j] = mp.pv_election[i][j];
		}

		// Initialise pv for fund shocks

		this->fund_shock_size = mp.fund_shock_size;
		this->fund_shock_pvs = new double*[this->fund_shock_size];

		// initialize the fund pv table
		for (int i = 0; i < this->fund_shock_size; i++)
		{
			this->fund_shock_pvs[i] = new double[this->get_total_pv_columns_number()];
			for (int j=0 ; j < this->get_total_pv_columns_number(); j++)
				this->fund_shock_pvs[i][j] = mp.fund_shock_pvs[i][j];
		}
	}
	
	/*
	* Destructor of the class 
	*		delete all the pvs tables (i.e indexes, funds, lapse, mortality, longevity)
	*/
	virtual ~Model_Point()
	{
		// delete of pv indexes
		for (int i = 0; i < this->index_shock_size; i++)
		{
			if (this->index_shock_pvs[i]!=0)
				delete[] this->index_shock_pvs[i];
		}
		if(this->index_shock_pvs!=0)
			delete[] this->index_shock_pvs;

		// delete of pv funds
		for (int i = 0; i < this->fund_shock_size; i++)
		{
			if (this->fund_shock_pvs[i]!=0)
				delete[] this->fund_shock_pvs[i];
		}

		if(this->fund_shock_pvs!=0)
			delete[] this->fund_shock_pvs;

		// delete of pv lapse
		for (int i = 0; i < this->lapse_size; i++)
		{
			if (this->pv_lapse[i]!=0)
				delete[] this->pv_lapse[i];
		}
		if(this->pv_lapse!=0)
			delete[] this->pv_lapse;

		// delete of pv mortality
		for (int i = 0; i < this->mortality_size; i++)
		{
			if (this->pv_mortality[i]!=0)
				delete[] this->pv_mortality[i];
		}
		if(this->pv_mortality!=0)
			delete[] this->pv_mortality;
		// delete of pv paidup
		//for (int i = 0; i < this->paidup_size; i++)
		//{
		//	if (this->pv_paidup[i]!=0)
		//		delete[] this->pv_paidup[i];
		//}
		//if(this->pv_paidup!=0)
		//	delete[] this->pv_paidup;

		if(this->pv_paidup!=0)
		{
			for (int i = 0; i < this->paidup_size; i++)
			{
				if (this->pv_paidup[i]!=0)
					delete[] this->pv_paidup[i];
			}
			delete[] this->pv_paidup;
		}

		// delete of pv longevity
		for (int i = 0; i < this->longevity_size; i++)
		{
			if (this->pv_longevity[i]!=0)
				delete[] this->pv_longevity[i];
		}
		if(this->pv_longevity!=0)
			delete[] this->pv_longevity;

		// delete of pv election
		//for (int i = 0; i < this->election_size; i++)
		//{
		//	if (this->pv_election[i]!=0)
		//		delete[] this->pv_election[i];
		//}
		//if(this->pv_election!=0)
		//	delete[] this->pv_election;

		if(this->pv_election!=0)
		{
			for (int i = 0; i < this->election_size; i++)
			{
				if (this->pv_election[i]!=0)
					delete[] this->pv_election[i];
			}
			delete[] this->pv_election;
		}

	}

	/*
	* Constructor of the class
	* vector<string>& row : vector of string corresponding to 1 line of the inforce file
	* int rank : rank of this model point in the whole inforce file
	* Product_configuration& product_conf : contain some information to build the model point object
	* Scenario_configuration& scenario_config : contain some information to build the model point object(size of the tables)
	*/
	Model_Point(vector<string>& row, int rank, Product_configuration& product_conf, 
					Scenario_configuration& scenario_config)
	{	
		// Initialization of total account value
		this->total_av = 0.0;
		
		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->index_shock_pvs[i][j] = 0.;
		}

		// Initialise pv for lapse shocks
		this->lapse_size = scenario_config.get_lapse_mult().size();

		this->pv_lapse = new double* [this->lapse_size];
		// initialize the pv table
		for (int i = 0; i < this->lapse_size; i++)
		{
			this->pv_lapse[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->pv_lapse[i][j] = 0.;
		}
		
		// Initialise pv for mortality shocks
		this->mortality_size = scenario_config.get_mortality_mult().size();

		this->pv_mortality = new double* [this->mortality_size];
		// initialize the pv table
		for (int i = 0; i < this->mortality_size; i++)
		{
			this->pv_mortality[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->pv_mortality[i][j] = 0.;
		}

		// Initialise pv for paidup shocks
		this->paidup_size = scenario_config.get_paidup_mult().size();

		this->pv_paidup = new double* [this->paidup_size];
		// initialize the pv table
		for (int i = 0; i < this->paidup_size; i++)
		{
			this->pv_paidup[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->pv_paidup[i][j] = 0.;
		}
		// Initialise pv for longevity shocks
		this->longevity_size = 1;

		this->pv_longevity = new double* [this->longevity_size];
		// initialize the pv table
		for (int i = 0; i < this->longevity_size; i++)
		{
			this->pv_longevity[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->pv_longevity[i][j] = 0.;
		}
		// Initialise pv for election shocks
		this->election_size = 1;

		this->pv_election = new double* [this->election_size];
		// initialize the pv table
		for (int i = 0; i < this->election_size; i++)
		{
			this->pv_election[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->pv_election[i][j] = 0.;
		}

		// Initialise pv for fund shocks

		this->fund_shock_size = scenario_config.get_fund_shock_array().size();
		this->fund_shock_pvs = new double*[this->fund_shock_size];

		// initialize the fund pv table
		for (int i = 0; i < this->fund_shock_size; i++)
		{
			this->fund_shock_pvs[i] = new double[TOTAL_OUTPUT_COLUMNS];
			for (int j=0 ; j < TOTAL_OUTPUT_COLUMNS; j++)
				this->fund_shock_pvs[i][j] = 0.;
		}

		memset(this->av_if_array, 0, sizeof(this->av_if_array));
		memset(this->av_split_prop_array, 0, sizeof(this->av_split_prop_array));
		this->get_timestamp();
		initialise_mp(row);
		this->mp_position_in_file = rank;
		this->load_longstaff = product_conf.is_longstaff();
		this->load_model_point_funds = product_conf.is_projection_by_fund();
		this->parse_fund_line(row[row.size() - 1], product_conf);
		this->parse_fund_line_stat(row[row.size() - 1]);
	}

	/*
	* Copy constructor (used for the hedge effectiveness)
	*/
	Model_Point(Model_Point * mp, int index_shock_size_in, int fund_shock_size_in)
	{
		this->total_av = 0.0;
		this->pv_lapse = 0;
		this->pv_mortality = 0;
		this->pv_longevity = 0;
		this->pv_election = 0;
		this->lapse_size = 0;
		this->mortality_size = 0;
		this->longevity_size = 0;
		this->fund_shock_pvs = 0;
		this->fund_shock_size = 0;

		this->index_shock_size = 0;
		this->index_shock_pvs = 0;
		this->pv_paidup = 0;
		this->paidup_size = 0;
		this->election_size = 0;

		memset(this->av_if_array, 0, sizeof(this->av_if_array));
		memset(this->av_split_prop_array, 0, sizeof(this->av_split_prop_array));
		this->get_timestamp();
		this->initialise_mp(mp);
		this->mp_position_in_file = mp->mp_position_in_file;
		this->load_model_point_funds = 0;
	}


	/*
	* Get the actual date 
	* valn_month : actual month
	* valn_year : actual year
	*/
	void get_timestamp()
	{
		char dateStr [9];
		char timeStr [9];
		_strdate_s( dateStr);
		_strtime_s( timeStr );
		char* next_token;
		string month = strtok_s(dateStr,"/",&next_token);
		string today = strtok_s(NULL, "/",&next_token) ;
		string year = "20" + (string)strtok_s(NULL, "/",&next_token) ;

		//string month = strtok(dateStr,"/");
		//string today = strtok(NULL, "/") ;
		//string year = "20" + (string)strtok(NULL, "/") ;
		valn_day = atoi(today.c_str());
		valn_month = atoi(month.c_str());
		valn_year = atoi(year.c_str());	
		//delete [] next_token;
		//if (next_token!=0) delete [] next_token;
	}


	/*
	* eliminate blank characters (in the beginning and in the end of teh string) from 
	* a string given in parameter
	*/
	void trim(string& str)   
	{   
		size_t startpos = str.find_first_not_of(" \t"); 
		size_t endpos = str.find_last_not_of(" \t"); 
		if(( string::npos == startpos ) || ( string::npos == endpos))   
		{   
			 str = "";   
		}   
		else  
			str = str.substr( startpos, endpos-startpos+1 );
	}

	/*
	* Initialize all the model point variable
	*/
	void initialise_mp(vector<string>& row){
		
		product = row[PRODUCT];
		group = row[GROUP];
		prod_code_key = row[PROD_CODE];
		age_valn = atof(row[AGE_AT_VALN].c_str());
		duration = atof(row[DURATION_VALN].c_str());
		sex 			= row[SEX];
		m_sex_perc 		= atof(row[M_SEX_PERC].c_str());
		policy_id = row[POLICY_ID];
		gmab_period = atof(row[GMXB_WAIT_PERIOD].c_str());
	
		if (row[AV_REBAL_IND] == "REBALANCE")
			av_rebal_ind = Model_Point::REBALANCE;
		else 
			av_rebal_ind = Model_Point::DRIFT;

		fpw_accum_total = atof(row[FPW_ACCUM_TOTAL].c_str());
		gmxb_a_c = atof(row[GMXB_A_C].c_str());
		gmab_rollup_valn = atof(row[GMAB_ROLLUP_VALN].c_str());
		gmab_ratchet_valn = atof(row[GMAB_RATCHET_VALN].c_str());
		gmab_rollup_rate = atof(row[GMAB_ROLLUP_RATE].c_str());
		gmab_av_period = atoi(row[GMXB_AV_PERIOD].c_str());
		gmab_maturity_freq = atof(row[GMXB_WAIT_PERIOD].c_str());
		gmxb_ehc_charge = atof(row[GMXB_EHC_CHG].c_str());
		gmdb_ratchet_valn 	= atof(row[GMDB_RATCHET_VALN].c_str());
		gmdb_rollup_valn 	= atof(row[GMDB_ROLLUP_VALN].c_str());
		gmdb_av_period 		= atoi(row[GMXB_AV_PERIOD].c_str());
		gmwb_ratchet_valn 	= atof(row[GMWB_RATCHET_VALN].c_str()); 
		gmwb_rollup_valn 	= atof(row[GMWB_ROLLUP_VALN].c_str());
		gmib_ratchet_valn 	= atof(row[GMIB_RATCHET_VALN].c_str()); 
		gmib_rollup_valn 	= atof(row[GMIB_ROLLUP_VALN].c_str());
		gmwb_ratchet_period = atof(row[GMXB_AV_PERIOD].c_str()); 
		gmwb_rollup_rate	= atof(row[GMAB_ROLLUP_RATE].c_str());
		date = (char*) (row[GMXB_BIRTHDAY_DATE].c_str());
		day = atoi(strtok(date, "/"));
		month = atoi(strtok(NULL, "/"));
		year = atoi(strtok(NULL, "/"));
		gmxb_g_c 			= atof(row[GMXB_G_C].c_str());
		gmxb_type2			= row[GMXB_TYPE2];
		admin_fee_dollar = atof(row[ADMIN_FEE_DOLLAR].c_str());
		gmxb_deferral_period = atof(row[GMXB_WAIT_PERIOD].c_str());
		prem_paid 		= atof(row[PREM_PAID].c_str());
		payt_freq 		= atof(row[PAYT_FREQ].c_str());
		gmsb_av_period 		= atoi(row[GMXB_AV_PERIOD].c_str());
		gmsb_period			= atof(row[GMXB_WAIT_PERIOD].c_str());
		gmxb_m_c 			= atof(row[GMXB_M_C].c_str());
		mgt_fee_perc 		= atof(row[GMXB_M_C].c_str());
		axa_roll_r   		= atof(row[GMAB_ROLLUP_RATE].c_str());
		year_prem	=  atof(row[YEAR_PREM].c_str());
		prem_curr		= atof(row[PREM_CURR].c_str());
		av_if_array[0] = atof(row[AV_VALN_INDEX0].c_str());
		av_if_array[1] = atof(row[AV_VALN_INDEX1].c_str());
		av_if_array[2] = atof(row[AV_VALN_INDEX2].c_str());
		av_if_array[3] = atof(row[AV_VALN_INDEX3].c_str());
		av_if_array[4] = atof(row[AV_VALN_INDEX4].c_str());
		av_if_array[5] = atof(row[AV_VALN_INDEX5].c_str());
		av_if_array[6] = atof(row[AV_VALN_INDEX6].c_str());
		av_if_array[7] = atof(row[AV_VALN_INDEX7].c_str());
		av_if_array[8] = atof(row[AV_VALN_INDEX8].c_str());
		av_if_array[9] = atof(row[AV_VALN_INDEX9].c_str());
		av_if_array[10] = atof(row[AV_VALN_INDEX10].c_str());
		av_if_array[11] = atof(row[AV_VALN_INDEX11].c_str());
		av_split_prop_array[0] = atof(row[TARGET_AV_SPLIT_VALN_INDEX0].c_str());
		av_split_prop_array[1] = atof(row[TARGET_AV_SPLIT_VALN_INDEX1].c_str());
		av_split_prop_array[2] = atof(row[TARGET_AV_SPLIT_VALN_INDEX2].c_str());
		av_split_prop_array[3] = atof(row[TARGET_AV_SPLIT_VALN_INDEX3].c_str());
		av_split_prop_array[4] = atof(row[TARGET_AV_SPLIT_VALN_INDEX4].c_str());
		av_split_prop_array[5] = atof(row[TARGET_AV_SPLIT_VALN_INDEX5].c_str());
		av_split_prop_array[6] = atof(row[TARGET_AV_SPLIT_VALN_INDEX6].c_str());
		av_split_prop_array[7] = atof(row[TARGET_AV_SPLIT_VALN_INDEX7].c_str());
		av_split_prop_array[8] = atof(row[TARGET_AV_SPLIT_VALN_INDEX8].c_str());
		av_split_prop_array[9] = atof(row[TARGET_AV_SPLIT_VALN_INDEX9].c_str());
		av_split_prop_array[10] = atof(row[TARGET_AV_SPLIT_VALN_INDEX10].c_str());
		av_split_prop_array[11] = atof(row[TARGET_AV_SPLIT_VALN_INDEX11].c_str());
		
		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
				this->total_av += this->av_if_array[i];


		prem_durr = atoi(row[PREM_DURR].c_str());
		gmwb_wdl_cum_dyn_lapse = atof(row[WB_WDL_CUM].c_str());
		gmwb_payt_yr = atof(row[WB_PAYT_YR].c_str());
		gmwb_wdl_rate = atof(row[GMWB_WDL_RATE].c_str());

		fee_rp = atof(row[FEE_RP].c_str());
		up_front_g_c = atof(row[UP_FRONT_G_C].c_str());
		
		extraction_date = (char*) (row[EXTRACTION_DATE].c_str());
		extraction_day = atoi(strtok(extraction_date, "/"));
		extraction_month = atoi(strtok(NULL, "/"));
		extraction_year = atoi(strtok(NULL, "/"));
		birthday = (int)(this->extraction_year + (this->extraction_month-1)/12. + (this->extraction_day-1)/365. - this->age_valn); 
		/*stat inforces*/
		realized_RRC = atof(row[RRC_REALIZED].c_str());
		admin_charges = atof(row[ADMIN_CHARGES].c_str());

		// Compute calculated fields
		age_exact_issue = this->age_valn - this->duration;

		// TO DO remove this (dbf round in Moses)
		if (ceil(age_exact_issue) - (age_exact_issue) <= 0.005)
			age_exact_issue = ceil(age_exact_issue);

		// Modification timing
		elapsed_months = (int)ceil(this->duration*12);

		prem_type		= "R";
		if (this->prem_durr == 1)
			prem_type		= "S";

		// Modification timing 
		elapsed_mths_in_valn_yr = (int)((this->duration - (int)this->duration)*12);
		
		// Modification timing 
		elapsed_years = (int) (this->duration);
		
		age_election_ddf_calculated = this->age_exact_issue + this->gmxb_deferral_period;
		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
			av_split_target[i] = av_split_prop_array[i];
	}

	/*
	* Initialize all the model point variable
	* from a pointer of Model point (used for the hedge effectiveness)
	*/
	virtual void initialise_mp(Model_Point * mp){
		
		product = mp->product;
		group = mp->group;
		prod_code_key = mp->prod_code_key;
		age_valn = mp->age_valn;
		duration = mp->duration;
		sex 			= mp->sex;
		m_sex_perc 		= mp->m_sex_perc;
		policy_id = mp->policy_id;
		gmab_period = mp->gmab_period;
		av_rebal_ind = mp->av_rebal_ind;
		gmxb_a_c = mp->gmxb_a_c;
		gmab_rollup_valn = mp->gmab_rollup_valn;
		gmab_ratchet_valn = mp->gmab_ratchet_valn;
		gmab_rollup_rate = mp->gmab_rollup_rate;
		gmab_av_period = mp->gmab_av_period;
		gmab_maturity_freq = mp->gmab_maturity_freq;
		gmxb_ehc_charge = mp->gmxb_ehc_charge;
		gmdb_ratchet_valn 	= mp->gmdb_ratchet_valn;
		gmdb_rollup_valn 	= mp->gmdb_rollup_valn;
		gmdb_av_period 		= mp->gmdb_av_period;
		gmwb_ratchet_valn 	= mp->gmwb_ratchet_valn;
		gmwb_rollup_valn 	= mp->gmwb_rollup_valn;
		gmwb_ratchet_period = mp->gmwb_ratchet_period;
		gmwb_rollup_rate	= mp->gmwb_rollup_rate;
		gmxb_g_c 			= mp->gmxb_g_c;
		gmxb_type2			= mp->gmxb_type2;
		admin_fee_dollar = mp->admin_fee_dollar;
		gmxb_deferral_period = mp->gmxb_deferral_period;
		prem_paid 		= mp->prem_paid;
		payt_freq 		= mp->payt_freq;
		gmsb_av_period 		= mp->gmsb_av_period;
		gmsb_period			= mp->gmsb_period;
		gmxb_m_c 			= mp->gmxb_m_c;
		mgt_fee_perc 		= mp->mgt_fee_perc;
		axa_roll_r   		= mp->axa_roll_r;
		year_prem	=  mp->year_prem;
		prem_curr		= mp->prem_curr;
		av_if_array[0] = mp->av_if_array[0];
		av_if_array[1] = mp->av_if_array[1];
		av_if_array[2] = mp->av_if_array[2];
		av_if_array[3] = mp->av_if_array[3];
		av_if_array[4] = mp->av_if_array[4];
		av_if_array[5] = mp->av_if_array[5];
		av_if_array[6] = mp->av_if_array[6];
		av_if_array[7] = mp->av_if_array[7];
		av_if_array[8] = mp->av_if_array[8];
		av_if_array[9] = mp->av_if_array[9];
		av_if_array[10] = mp->av_if_array[10];
		av_if_array[11] = mp->av_if_array[11];


		av_split_prop_array[0] = mp->av_split_prop_array[0];
		av_split_prop_array[1] = mp->av_split_prop_array[1];
		av_split_prop_array[2] = mp->av_split_prop_array[2];
		av_split_prop_array[3] = mp->av_split_prop_array[3];
		av_split_prop_array[4] = mp->av_split_prop_array[4];
		av_split_prop_array[5] = mp->av_split_prop_array[5];
		av_split_prop_array[6] = mp->av_split_prop_array[6];
		av_split_prop_array[7] = mp->av_split_prop_array[7];
		av_split_prop_array[8] = mp->av_split_prop_array[8];
		av_split_prop_array[9] = mp->av_split_prop_array[9];
		av_split_prop_array[10] = mp->av_split_prop_array[10];
		av_split_prop_array[11] = mp->av_split_prop_array[11];
		
		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
				this->total_av += this->av_if_array[i];
		}

		prem_durr = mp->prem_durr;
		gmwb_wdl_cum_dyn_lapse = mp->gmwb_wdl_cum_dyn_lapse;
		gmwb_payt_yr = mp->gmwb_payt_yr;
		gmwb_wdl_rate = mp->gmwb_wdl_rate;
		fee_rp = mp->fee_rp;
		//Abdelhadi
		extraction_date = mp->extraction_date;
		extraction_day = mp->extraction_day;
		extraction_month = mp->extraction_month;
		extraction_year = mp->extraction_year;
		birthday = mp->birthday;
		year = mp->year; 
		/*stat inforces*/
		realized_RRC = mp->realized_RRC;
		admin_charges = mp->admin_charges;
		up_front_g_c = mp->up_front_g_c;

		// Compute calculated fields
		age_exact_issue = this->age_valn - this->duration;

		// TO DO remove this (dbf round in Moses)
		if (ceil(age_exact_issue) - (age_exact_issue) <= 0.005)
			age_exact_issue = ceil(age_exact_issue);

		// Modification timing
		elapsed_months = (int)ceil(this->duration*12);

		prem_type = "R";
		if (this->prem_durr == 1)
			prem_type		= "S";

		// Modification timing 
		elapsed_mths_in_valn_yr = (int)((this->duration - (int)this->duration)*12);
		
		// Modification timing 
		elapsed_years = (int) (this->duration);
		
		age_election_ddf_calculated = this->age_exact_issue + this->gmxb_deferral_period;

		for (int i=0; i<MAXIMUM_NUMBER_OF_INDEXES; i++)
			av_split_target[i] = av_split_prop_array[i];
	}

	/*
	* Return the total number of pv columns
	* For the generic products the total number of columns is 10
	* The list is 
	* BEGINING_ACCOUNT_VALUE column number 0
	* PV_GMDB column number 1
	* PV_GMWB column number 2
	* PV_GMAB column number 3
	* PV_GMIB column number 4
	* PV_GMSB column number 5
	* PV_GMXB_REAL_CHG column number 6
	* PV_GMXB_EHC_CHG column number 7
	* DISCOUNT_FACTOR_VALUE column number 8
	* PV_DEP_FEE column number 9
	*/
	virtual int get_total_pv_columns_number()
	{
		return TOTAL_OUTPUT_COLUMNS;
	}

	/*
	* Reset all the alpha_t and return_fund_t arrays (to 0)
	*/
	void init_fund_arrays()
	{
		for (unsigned int i = 0; i < this->list_of_funds.size(); i++)
		{
			memset(this->list_of_funds[i].alpha_t, 0, sizeof(this->list_of_funds[i].alpha_t));
			
			this->list_of_funds[i].alpha_t[0] = (this->list_of_funds[i].number_share 
												* this->list_of_funds[i].fund.get_fund_value()/this->total_fund_values
												);
			memset(this->list_of_funds[i].return_fund_t, 0, sizeof(this->list_of_funds[i].return_fund_t));

		}
	}
	
	/*
	* To add set_pv for all the PV STEC : set_pv_fund_shock_array, set_pv_longevity_shock_array
	* set_pv_mortality_shock_array, set_pv_lapse_shock_array
	*/

	virtual void set_pv_index_shock_array(Scenario_configuration& scenario_config)
	{
		// delete of pv indexes
		for (int i = 0; i < this->index_shock_size; i++)
		{
			if (this->index_shock_pvs[i]!=0)
				delete[] this->index_shock_pvs[i];
		}

		if(this->index_shock_pvs!=0)
			delete[] this->index_shock_pvs;

		this->index_shock_size = scenario_config.get_index_shock_array().size();
		this->index_shock_pvs = new double* [this->index_shock_size];
		// initialize the index pv table
		for (int i = 0; i < this->index_shock_size; i++)
		{
			this->index_shock_pvs[i] = new double[get_total_pv_columns_number()];
			for (int j=0 ; j < get_total_pv_columns_number(); j++)
				this->index_shock_pvs[i][j] = 0.;
		}

	}
	
	virtual void initialize_index_shock_array()
	{
		for (int i = 0; i < this->index_shock_size; i++)
		{
			for (int j=0 ; j < get_total_pv_columns_number() ; j++)
				this->index_shock_pvs[i][j] = 0.;
		}
	}

};