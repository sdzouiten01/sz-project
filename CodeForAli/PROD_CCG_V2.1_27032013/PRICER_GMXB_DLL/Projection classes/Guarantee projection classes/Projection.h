#include <algorithm>
#pragma once
#include "head.h"
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <iomanip>
#include "Model point classes/Model_Point.h"
#include "LSM/Longstaff_Boundary.h"
#include "LSM/ITM_for_LS_Boundary.h"
#include "LSM/Regression.h"

const int T_Max_Monthly = 1000;

using namespace std;

extern ofstream trace;
bool inforce_compare(vector<double> inforce1, vector<double> inforce2)
{
	if (((inforce2[0]/inforce1[0])-1.)>0.000001)
	{
		return 1;
	}
	else if ((((inforce2[0]/inforce1[0])-1.)<0.000001)&&(((inforce2[0]/inforce1[0])-1.)>-0.000001))
	{
		return (inforce1[1]<inforce2[1]);
	}
	else
	{
		return 0;
	}
}

//! for pvs_ScenOutput
class Pvs_scenByscen
{ 
public:
	//int number_of_table;
	int li_nScenPath;

	//! they are of size: [nScenPath][nShock][nPv]
    vector<vector<vector<double>>>  pv_lapse_scen_output;
	vector<vector<vector<double>>>  pv_mortality_scen_output;
	vector<vector<vector<double>>>  pv_paidup_scen_output;
	vector<vector<vector<double>>>  pv_longevity_scen_output;
	vector<vector<vector<double>>>  index_shock_pvs_scen_output;
	vector<vector<vector<double>>>  fund_shock_pvs_scen_output;

	//! they are of size: [nShock][nPv] as normal mp
	vector<vector<double>> pv_lapse_scen_output_temp;
	vector<vector<double>> pv_mortality_scen_output_temp;
	vector<vector<double>> pv_paidup_scen_output_temp;
	vector<vector<double>> pv_longevity_scen_output_temp;
	vector<vector<double>> index_shock_pvs_scen_output_temp;
	vector<vector<double>> fund_shock_pvs_scen_output_temp;

	//! default constructor
	//Pvs_scenByscen():number_of_table(6){};
	Pvs_scenByscen():li_nScenPath(-1),
		     pv_lapse_scen_output(0), pv_mortality_scen_output(0),
			 pv_paidup_scen_output(0),pv_longevity_scen_output(0),
			 index_shock_pvs_scen_output(0),fund_shock_pvs_scen_output(0),
			 pv_lapse_scen_output_temp(0),pv_mortality_scen_output_temp(0),
			 pv_paidup_scen_output_temp(0),pv_longevity_scen_output_temp(0),
			 index_shock_pvs_scen_output_temp(0),fund_shock_pvs_scen_output_temp(0)
	{};
};
class Projection{

public:
	Pvs_scenByscen pvs_scenByscen;
	protected:
	/*
	* Arrays containing all the variable of the 
	* projection such as all the Benefit Base the Account value at different steps of the projections
	* and so on
	* for all the period (from t = 0 to the maturity of the contract)
	* main_proj_array_MTH is filled when the projection is of type monthly
	* main_proj_array_ANN is filled when the projection is of type annually
 	*/
	double main_proj_array_MTH[SUBACCT_MAX_PERIOD_MTH +1][MAIN_PROJ_NUM_COLS];
	double main_proj_array_ANN[SUBACCT_MAX_PERIOD +1][MAIN_PROJ_NUM_COLS];
    double (*main_proj_array)[MAIN_PROJ_NUM_COLS];

	ofstream affich_shock;
	/*
	* Arrays containing all the timing value such as : 
	* POL_MONTH column number 0  
	* POL_YEAR column number 1
	* AGE_LAST column number 2 
	* PRORATA column number 3
	* for all the period (from t = 0 to the maturity of the contract)
	* timing_items_MTH is filled when the projection is of type monthly
	* timing_items_ANN is filled when the projection is of type annually
	*/
	double timing_items_MTH[SUBACCT_MAX_PERIOD_MTH +1][TOTAL_TIMING_ITEMS];
	double timing_items_ANN[SUBACCT_MAX_PERIOD +1][TOTAL_TIMING_ITEMS];
	double (*timing_items)[TOTAL_TIMING_ITEMS];
	
	/*
	* Table containing all the index shocks
	* The first columns is equal the index number which has to shocked (MAXIMUM_NUMBER_OF_INDEXES+1 in case of global shock). 
	*/ 
	int ** index_shock_array;

	/*
	* Table containing all the fund shocks
	* The first columns is equal the fund number which has to shocked. 
	*/ 
	int ** fund_shock_array;

	/*
	* The total number of shock in the index_shock_array
	*/
	int index_shock_size;

	/*
	* The total number of shock in the fund_shock_array
	*/
	int fund_shock_size;

	/*
	* The current number of the index shock
	*/
	int index_shock_number;

	/*
	* The current number of the fund shock
	*/
	int fund_shock_number;

	/*
	* Indicates whether we have to shock the fund or the indexes
	*/
	bool shock_fund;

	/*
	* The current model point to price
	*/
	Model_Point *model_point;

	/*
	* The current scenario used to price the model_point
	*/
	double *** afmr;

	/*
	* monthly rates (hedge effectiveness)
	*/
	double ** mr;
	
	/*
	* The current scenario number
	*/
	int current_scenario_number;

	/*
	* Array which contain the index number 
	* as well as the split of the index
	*/
	map <int, int> index_map;

	/*
	* Array created from index_map that contain only  
	* the indexes with an account value not null (or a non null split)
	*/
	int index_map_2[MAXIMUM_NUMBER_OF_INDEXES];
	
	/*
	* Size of index_map_2
	*/
	int index_map_size;

	
	/*
	* survival month rate cumulated
	* surv_cumul_MTH is filled when the projection is of type monthly
	* surv_cumul_ANN is filled when the projection is of type annually
	*/

	double surv_cumul_MTH[SUBACCT_MAX_PERIOD_MTH +1];
	double surv_cumul_ANN[SUBACCT_MAX_PERIOD +1];
	double * surv_cumul;

	// TO DO variable not linked
	int prem_duration;

	/*
	* Number of years until the maturity 
	* for the current model point to price
 	*/
	int maturity_yr;

	/*
	* 
	*/
	int term_to_omega;

	/*
	* 
	*/ 
	int product_charging_duration;

	
	GMXBParameters  *assump;
	
	/*
	* Female mortality table
	*/
	GMXB_Assumption_Table *mort_table_f;

	/*
	* Male mortality table
	*/
	GMXB_Assumption_Table *mort_table_m;
	
	/*
	* mortality table used in the projection
	* Depending on the policy holder gender, 
	* this table can be either mort_table_f or mort_table_m
	*/
	GMXB_Assumption_Table *mort_table;
	
	/*
	* Lapse rates table
	*/	
	GMXB_Assumption_Table *lapse_rates;

	double lapse_min_pa_perc;
	double lapse_max_pa_perc;
	double mort_adj_f;
	double mort_adj_m;
	double mort_adj;
	double itm_factor;
	double mort_mult_f;
	double mort_mult_m;
	double mort_mult;
	string how_is_scenario_file;
	string av_split_adj;
	string use_av_start_split;
	double mort_age_adj;
	double goalseek_factor;

	int t_high_shorter;
	/*
	* Indicates whether the lapse are dynamic(if set to "Y" ) or static 
	*/
	bool lapse_is_dynamic;

	/*
	*
	*/
	string correct_med_db ;

	/*
	*
	*/
	string projection_type;

	/*
	*
	*/
	double * taxes_new_contract;

	/*
	*
	*/
	double premium_schedular;

	// TODO remove these 2 variables
	int write_subacct_array;
	int write_main_proj_array;

	/*
	* Indicates whether the projection should ouput 
	* the main_proj_array_2 in a seperate file
	*/
	bool generate_debug_files;
	
	/*
	* Used if generate_debug_files is true
	* Path of the debug file
	*/
	string debug_file_path;

	/*
	* Used if generate_debug_files is true
	* Debug file
	*/
	ofstream debug_file;

	/*
	* Arrays which contain all the weights of the indexes during all the projection
	* index_weigths_MTH is filled when the projection is of type monthly
	* index_weigths_ANN is filled when the projection is of type annually
	*/
	double index_weigths_MTH[SUBACCT_MAX_PERIOD_MTH +1][MAXIMUM_NUMBER_OF_INDEXES];
	double index_weigths_ANN[SUBACCT_MAX_PERIOD +1][MAXIMUM_NUMBER_OF_INDEXES];
	double (*index_weigths)[MAXIMUM_NUMBER_OF_INDEXES];

	/*
	* Vector of lapse multiplicator (for lapse shock purpose)
	*/
	vector<GMXB_Assumption_Table *> v_lapse_mult_table;
	
	/*
	* Vector of mortality multiplicator (for mortality shock purpose)
	*/
	vector<GMXB_Assumption_Table *> v_mortality_mult_table;
	/*
	* Vector of paidup multiplicator (for paidup shock purpose)
	*/
	vector<GMXB_Assumption_Table *> v_paidup_mult_table;
	vector<GMXB_Assumption_Table *> v_election_mult_table;
	
	/*
	* Female longevity table
	*/
	GMXB_Assumption_Table *longevity_shock_table_f;

	/*
	* Male longevity table
	*/
	GMXB_Assumption_Table *longevity_shock_table_m;

	/*
	* Longevity table used in the projection
 	*/
	GMXB_Assumption_Table *longevity_shock_table;

	/*
	* Indicates whether we load the longevity table or not (for longevity shocks purpose)
	*/
	bool load_longevity_tables;
	//total number of scenarios
	int total_number_scn;
	/*
	* Longstaff and boundary variables
	*/
	string longstaff_ind;
	string longstaff_method;
	string ITM_choice_type_1;
	string ITM_choice_type_2;
	string ITM_choice_type_3;
	int s0;   
	int t0;
	int n0;
	int p0;
	int first_lapse_year;
	int nb_parameters;
	double length_smoothing;
	double length_smoothing_av;
	double param_bar;
	
	double*** paths;
	double**** explicative_variables;

	double*** abclaims;
	double*** dbclaims;
	double*** sbclaims;
	double*** ibclaims;
	double*** wbclaims;
	double*** premium_charges;
	double*** bonus_charges;
	double*** sum_prem_charges;
	double*** sum_bonus_charges;
	double*** gmxb_charges;
	double*** gmxb_charges_add;

	double*** proba_exercise_traj_by_traj;
	double*** proba_survival;
	double** proba_exercise;

	string longstaff_debug_info;
	string longstaff_proba_info; //mihaela
	string ehc_for_longstaff;
	double*** regression_parameters; 
	double** boundary_value;
	
	/* 
	* Time step of the projection 
	* Allowed values are 1 for annual projection and 12 for monthly projection
	*/
	int time_step;

	// Variables for the monthly projection
	// lapses calculation frequency
	int itm_frequency; 
	
	// fees payment frequency
	int payment_fees_frequency;
	
	// rollup frequency
	int rollup_frequency;
	
	// fund rebalancing frequency
	int rebalance_frequency;
	
	// projection period
	int subacct_max_period;

   /*
	* Risk Apetite
	*/
	string risk_appetite;
	
	/*
	* Risk Apetite
	*/
	double perc_1;
	double perc_2;
	double perc_3;
	int t_RA;
	int ITM_type; //0 AV 1 AV/BB
    int OutputAll; //output all scenario results for each mp
	/*
	* Vector of ITM - Risk Apetite
	*/
	//vector<double> recup_ITM_RA;
	vector<vector<double>> recup_ITM_RA;
	// Used by the actuarial module = (EOM_date - BOM_date)/365
	double duration_age_step; 


	// Actuarial module - multipliers & step
	double mortality_mult_for_ap;
	double lapse_mult_for_ap;
	double charges_mult_for_ap; 

	// avGrowth used for actuarial module
	double avGrowthRate;
	bool is_he;
	bool project_aging_policies;


public:

	/*
	* Default constructor
	*/
	static int compt_sen; //AJOUT
	/*
	*Fichier à créer pour LS
	*/
	ofstream file_stock;
	ofstream file_stock2;
	ifstream file_stock3;
	ifstream file_stock4;
	ofstream file_stock5; 
	Projection(const int t_high_shorter)
	{
		this->t_high_shorter =  t_high_shorter;
	}

	/*
	* Constructor of the class: Initialization of all the tables
	* const vector<vector int>& i_s_a: Matrix of index shock (the first column contain the index id)
	* const vector<vector int>& f_s_a: Matrix of fund shock (the first column contain the fund id)
	* tables : Path to the assumption file
	* generate_dbg_files : Indicates whether the projection should generate debug files or not
	* dbg_file_path : Path of the debug file
	* t_high_shorter : Scenario projection period 
	* l_longevity_tables : Longevity table (used for STEC calculation)
	* t_step : Indicates whether the projection is Annual (1) or Monthly (12)
	*/
	Projection(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a, 
			   const char * tables, bool generate_dbg_files, const string& dbg_file_path,
			   const int t_high_shorter, bool l_longevity_tables, int t_step)
	{
		
		this->t_high_shorter = t_high_shorter;
		
		assump= new GMXBParameters(tables, "assump");
		mort_table_m = new GMXB_Assumption_Table(tables , "mort_rate_m");

		mort_table_f = new GMXB_Assumption_Table(tables, "mort_rate_f");
		lapse_rates = new GMXB_Assumption_Table(tables, "lapse_rate");
		lapse_min_pa_perc = atof(this->assump->search("lapse_min_pa_perc").c_str());
		lapse_max_pa_perc = atof(this->assump->search("lapse_max_pa_perc").c_str());
		itm_factor = atof(this->assump->search("itm_factor").c_str());
		mort_adj_f = atof(this->assump->search("mort_adj_f").c_str());
		mort_adj_m = atof(this->assump->search("mort_adj_m").c_str());
		how_is_scenario_file = this->assump->search("how_is_scenario_file");
		av_split_adj = this->assump->search("av_split_adj");
		use_av_start_split = this->assump->search("use_av_start_split");
		mort_age_adj = atof(this->assump->search("mort_age_adj").c_str());
		goalseek_factor = atof(this->assump->search("goalseek_factor").c_str());
		risk_appetite = this->assump->search("risk_appetite",0);//Risk Appetite
		perc_1 = atof(this->assump->search("perc_val_1",0).c_str());//Risk Appetite
		perc_2 = atof(this->assump->search("perc_val_2",0).c_str());//Risk Appetite
		perc_3 = atof(this->assump->search("perc_val_3",0).c_str());//Risk Appetite
		t_RA = atof(this->assump->search("t_RA",0).c_str());//Risk Appetite
		ITM_type = atoi(this->assump->search("ITM",0).c_str());//Risk Appetite
		OutputAll = atoi(this->assump->search("OutputAll",0).c_str()); //Risk Appetite

		if (this->assump->search("lapse_is_dynamic") == "Y")
			this->lapse_is_dynamic = true;
		else
			this->lapse_is_dynamic = false;

		write_subacct_array = atoi(this->assump->search("write_subacct_array").c_str());
		write_main_proj_array = atoi(this->assump->search("write_main_proj_array").c_str());
		correct_med_db = this->assump->search("correct_med_db");
		mort_mult_f = atof(this->assump->search("mort_mult_f").c_str())/100;
		mort_mult_m = atof(this->assump->search("mort_mult_m").c_str())/100;

		// TO DO - NA - this value should be configurable
		premium_schedular = 1;
		// TO DO - this option must be deleted in the release V13.0
		this->is_he = false;

		this->index_shock_size = i_s_a.size();
		if (this->index_shock_size > 0)
		{
			this->index_shock_array = new int* [this->index_shock_size];
			for(int i = 0; i < this->index_shock_size; i++)
			{
				this->index_shock_array[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
				for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
					this->index_shock_array[i][j] = i_s_a[i][j];
			}
		}

		this->fund_shock_size = f_s_a.size();
		if (this->fund_shock_size > 0)
		{
			int total_nb_funds = f_s_a[0].size();
			this->fund_shock_array = new int*[this->fund_shock_size];
			for(int i = 0; i < this->fund_shock_size; i++)
			{
				this->fund_shock_array[i] = new int [total_nb_funds];
				for(int j = 0; j<total_nb_funds; j++)
					this->fund_shock_array[i][j] = f_s_a[i][j];
			}
		}

		this->generate_debug_files = generate_dbg_files;
		this->debug_file_path = dbg_file_path;

		this->load_longevity_tables = l_longevity_tables;

		if (this->load_longevity_tables)
		{
			longevity_shock_table_m = new GMXB_Assumption_Table(tables , "longevity_m");
			longevity_shock_table_f = new GMXB_Assumption_Table(tables, "longevity_f");
		}

		this->time_step = t_step;
		
		if(this->time_step == ANNUAL)
		{
			main_proj_array = main_proj_array_ANN;
			timing_items    = timing_items_ANN;
			index_weigths   = index_weigths_ANN;
			surv_cumul      = surv_cumul_ANN;

			this->itm_frequency = EACH_T;
			this->payment_fees_frequency = EACH_T;
			this->rollup_frequency = EACH_T;
			this->rebalance_frequency = EACH_T;
			this->subacct_max_period = SUBACCT_MAX_PERIOD;
		}
		else if (this->time_step == MONTHLY)
		{
			main_proj_array = main_proj_array_MTH;
			timing_items    = timing_items_MTH;
			index_weigths   = index_weigths_MTH;
			surv_cumul      = surv_cumul_MTH;

			this->subacct_max_period = SUBACCT_MAX_PERIOD_MTH;
			
			string option = this->assump->search("ITM_frequency");
			if (strcmp(option.c_str(), "Each_Beg_Year") == 0)
			{
				this->itm_frequency = EACH_BEG_YEAR;
			}
			else if (strcmp(option.c_str(), "Each_t")==0)
			{
				this->itm_frequency = EACH_T;
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "ITM_frequency (=%s) is not correct in the assump sheet ", option.c_str());
				throw new GMXBException(message);
			}
			
			option = this->assump->search("payment_fees_frequency");
			if (strcmp(option.c_str(), "Each_t") == 0)
			{
				this->payment_fees_frequency = EACH_T;
			}
			else if (strcmp(option.c_str(), "Each_Beg_Year") == 0)
			{
				this->payment_fees_frequency = EACH_BEG_YEAR;
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "payment_fees_frequency (=%s) is not correct in the assump sheet ", option.c_str());
				throw new GMXBException(message);
			}
			
			option = this->assump->search("rollup_frequency");	
			if (strcmp(option.c_str(), "Each_t") == 0)
			{
				this->rollup_frequency = EACH_T;
			}
			else if (strcmp(option.c_str(), "Each_Beg_Year") == 0)
			{
				this->rollup_frequency = EACH_BEG_YEAR;
			}
			else if (strcmp(option.c_str(), "Each_End_Year") == 0)
			{
				this->rollup_frequency = EACH_END_YEAR;
			}
			else 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "rollup_frequency (=%s) is not correct in the assump sheet ", option.c_str());
				throw new GMXBException(message);
			}
			
			option = this->assump->search("rebalance_frequency");
			if (strcmp(option.c_str(), "Each_t") == 0)
			{
				this->rebalance_frequency = EACH_T;
			}
			else if (strcmp(option.c_str(), "Each_Beg_Year") == 0)
			{
				this->rebalance_frequency = EACH_BEG_YEAR;
			}
			else 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "rebalance_frequency (=%s) is not correct in the assump sheet ", option.c_str());
				throw new GMXBException(message);
			}	
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either be ANNUAL or MONTHLY\n");
			throw new GMXBException(message);
		}

		this->project_aging_policies = false;
	}

	/*
	* Destructor of the class
	* Remove all the tables and pointers 
	* of the class
	*/
	virtual ~Projection()
	{
		if(this->mort_table_m!= 0)
			delete this->mort_table_m;
		if(this->mort_table_f!= 0)
			delete this->mort_table_f;
		if(this->lapse_rates!= 0)
			delete this->lapse_rates;
		if(this->assump != 0)
			delete this->assump;

		// delete the shock array
		for(int i = 0; i <this->index_shock_size; i++)
		{
			if(this->index_shock_array[i] != 0)
				delete[] this->index_shock_array[i];
		}
		if (this->index_shock_size > 0)
			delete[] this->index_shock_array;
		
		// delete the shock array
		for(int i = 0; i <this->fund_shock_size; i++)
		{
			if(this->fund_shock_array[i]!= 0)
				delete[] this->fund_shock_array[i];
		}
		if (this->fund_shock_size > 0)
			delete[] this->fund_shock_array;

		// delete of the longevity arrays
		if (this->load_longevity_tables)
		{
			delete this->longevity_shock_table_f;
			delete this->longevity_shock_table_m;
		
		}
		//delete mortality and lapse shocks tables
		for (unsigned int i = 0; i < this->v_lapse_mult_table.size(); i++)
			delete this->v_lapse_mult_table[i];
		
		for (unsigned int i = 0; i < this->v_mortality_mult_table.size(); i++)
		{
			delete this->v_mortality_mult_table[i];
		}

		for (unsigned int i = 0; i < this->v_paidup_mult_table.size(); i++)
			delete this->v_paidup_mult_table[i];

		for (unsigned int i = 0; i < this->v_election_mult_table.size(); i++)
			delete this->v_election_mult_table[i];
	}


	int get_maturity_yr()
	{
		return this->maturity_yr;
	}

	/*
	* Get the maximum period of the projection (used for hedge effectiveness)
	*/
	virtual int get_subacct_max_period()
	{
		return this->subacct_max_period;
	}

	int get_num_model_point()
	{
		return this->model_point->mp_position_in_file;
	}


	/*
	* TO DO remove this function from the projection
	* Convert from int to string
	*/
	
	inline string nb_to_string(int i){
		string s;
		stringstream sstr;
		sstr << i;
		sstr >> s;
		return s;
	}

	/*
	* TO DO remove this function from the projection
	* Convert from double to string
	*/

	inline string nb_to_string(double d){
		string s;
		stringstream sstr;
		sstr << d;
		sstr >> s;
		return s;
	}

	/*
	*  return the time step (1 or 12) (used for hedge effectiveness)
	*/
	int get_time_step()
	{
		return this->time_step;
	}


	void set_total_number_scn( int scn_numb)
	{
		total_number_scn=scn_numb;
	}

	/*
	* Set the current annual scenario in the projection (see background_final.h)
	*/
	virtual void set_annual_from_monthly(double *** ann_from_mthly, int scen_num)
	{
		this->afmr = ann_from_mthly;
		this->current_scenario_number = scen_num;
	}

	/*
	* Set the current monthly scenario in the projection (see background_final.h)
	*/
	virtual void set_monthly_rates(double ** monthly_rates, int scen_num)
	{
		this->mr = monthly_rates;
		this->current_scenario_number = scen_num;
	}

	/*
	* Actuarial module - Initialize growth rate read in the scenario file used for aging policies
	*/
	virtual void set_monthly_rates_for_aging_policies(double GrowthRate)
	{
	   this->avGrowthRate = GrowthRate;
	}

	/*
	* Get the rates (used for hedge effectiveness)
	*/
	double get_rate(int elapsed_mths_in_valn_yr, int t, int index_num)
	{
		if (this->time_step==ANNUAL)
			return this->afmr[elapsed_mths_in_valn_yr][t][index_num];
		else
			return this->mr[t][index_num];
	}


	/*
	* Get the current model point (used for hedge effectiveness)
	*/
	Model_Point * get_model_point()
	{
		return this->model_point;
	}	
	/*
	* Get a value from the index fund weight array  (used for hedge effectiveness)
	*/

	inline double get_index_weights(int col, int line)
	{
		return this->index_weigths[line][col];
	}
	/*
	* Get a value from the main project array  (used for hedge effectiveness)
	*/
	inline double get_main_proj_array(int col, int line)
	{
		return this->main_proj_array[line][col];
	}

	/*
	* Get a value from the timing items array (used for hedge effectiveness)
	*/

	inline double get_timing_items(int col, int line)
	{
		return this->timing_items[line][col];
	}

	/*
	* set the model point parameters 
	* from the projection out (used for hedge effectiveness)
	*/
	virtual void get_older_policy(Projection* proj_out, int t, string* inforce)
	{
		// Modification timing		
		this->model_point->age_valn = proj_out->get_timing_items(AGE_LAST,t);
		this->model_point->duration = this->model_point->age_valn - proj_out->model_point->age_valn; 
		this->model_point->elapsed_months = (int)ceil(this->model_point->duration*12);
		this->model_point->elapsed_mths_in_valn_yr = (int)((this->model_point->duration - (int)this->model_point->duration)*12);
		this->model_point->elapsed_years = (int) (this->model_point->duration);
		
		// Modif Base
		this->model_point->gmab_rollup_valn		= proj_out->get_main_proj_array(GMAB_ROLLUP_BASE,t-1);
		this->model_point->gmab_ratchet_valn	= proj_out->get_main_proj_array(GMAB_RATCHET_BASE,t-1);
		this->model_point->gmdb_ratchet_valn 	= proj_out->get_main_proj_array(GMDB_AV_IF,t-1);
		this->model_point->gmdb_rollup_valn 	= proj_out->get_main_proj_array(GMDB_PREM_IF,t-1);
		this->model_point->gmwb_ratchet_valn 	= proj_out->get_main_proj_array(GMWB_AV_IF,t-1); 
		this->model_point->gmwb_rollup_valn 	= proj_out->get_main_proj_array(GMWB_PREM_IF,t-1);
		this->model_point->prem_paid			= proj_out->model_point->prem_paid * proj_out->get_main_proj_array(SURV_INDEX,t);
		
		// Modif AV
		this->model_point->total_av = 0;
	
		for (int i=0; i< MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			this->model_point->av_split_prop_array[i] = proj_out->get_index_weights(i,t);
			this->model_point->av_if_array[i] = proj_out->get_main_proj_array(AV_E,t-1)*this->model_point->av_split_prop_array[i]/100;
			this->model_point->total_av += this->model_point->av_if_array[i];
		}

		// Modification timing		
		inforce[AGE_AT_VALN]		= nb_to_string(this->model_point->age_valn );
		inforce[DURATION_VALN]		= nb_to_string(this->model_point->duration) ;
		
		// Modif Base
		inforce[PREM_PAID]			= nb_to_string(this->model_point->prem_paid) ;
		inforce[GMAB_ROLLUP_VALN]	= nb_to_string(this->model_point->gmab_rollup_valn);
		inforce[GMAB_RATCHET_VALN]	= nb_to_string(this->model_point->gmab_ratchet_valn);
		inforce[GMDB_RATCHET_VALN]	= nb_to_string(this->model_point->gmdb_ratchet_valn);
		inforce[GMDB_ROLLUP_VALN]	= nb_to_string(this->model_point->gmdb_rollup_valn);
		inforce[GMWB_RATCHET_VALN]	= nb_to_string(this->model_point->gmwb_ratchet_valn);
		inforce[GMWB_ROLLUP_VALN]	= nb_to_string(this->model_point->gmwb_rollup_valn);
		
		// Modif AV
		for (int i=0; i< MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			inforce[TARGET_AV_SPLIT_VALN_INDEX0 + i] = nb_to_string(this->model_point->av_split_prop_array[i]);
			inforce[AV_VALN_INDEX0 + i]	= nb_to_string(this->model_point->av_if_array[i]);
		}

		//Sheng, Modif: Calcul du cumul de GMWB_PAYMENT
		double wb_wdl_cumul = 0;
		for (int i=0; i<=t; i++)
			wb_wdl_cumul += proj_out->get_main_proj_array(GMWB_PAYMENT,i);
		
		inforce[WB_WDL_CUM] = nb_to_string(wb_wdl_cumul);
		
		double wb_payt_yr = 0;
		
		int first_time_step_of_year = (t-1)/12*12+1;
		
		for (int i=first_time_step_of_year; i<=t; i++)
			wb_payt_yr += proj_out->get_main_proj_array (GMWB_PAYMENT,i);
		
		inforce[WB_PAYT_YR] = nb_to_string(wb_payt_yr);
	}


	virtual vector<Model_Point*> age_policies_from_projection()
	{
		vector<Model_Point*> vector_aged_polices;
		
		for( int tt = 1;  tt <= this->get_subacct_max_period() && 
			this->get_timing_items(POL_YEAR,tt) <= this->get_maturity_yr(); tt++)
		{
			Model_Point * p_mp = new Model_Point(this->model_point, 1, 0);
			p_mp->mp_position_in_file=tt;

			p_mp->age_valn = this->get_timing_items(AGE_LAST,tt);
			p_mp->duration = p_mp->age_valn - this->model_point->age_exact_issue;
			
			p_mp->age_valn = rounding_sup(p_mp->age_valn, 0.000001);
			p_mp->duration = rounding_sup(p_mp->duration, 0.000001);

			p_mp->elapsed_months = (int)(p_mp->duration*12);
			p_mp->elapsed_mths_in_valn_yr = (int)((p_mp->duration - (int)p_mp->duration)*12);
			p_mp->elapsed_years = (int) (p_mp->duration);

			if (tt!=1)
			{				
				//Sheng: superior rounding on age_valn and duration, (10^(-6)) 
				/*p_mp->age_valn = rounding_sup(p_mp->age_valn, 0.000001);
				p_mp->duration = rounding_sup(p_mp->duration, 0.000001);

				p_mp->elapsed_months = (int)(p_mp->duration*12);
				p_mp->elapsed_mths_in_valn_yr = (int)((p_mp->duration - (int)p_mp->duration)*12);
				p_mp->elapsed_years = (int) (p_mp->duration);*/

				// Modif Base
				p_mp->gmab_rollup_valn		= this->get_main_proj_array(GMAB_ROLLUP_BASE,tt-1);
				p_mp->gmab_ratchet_valn	= this->get_main_proj_array(GMAB_RATCHET_BASE,tt-1);
				p_mp->gmdb_ratchet_valn 	= this->get_main_proj_array(GMDB_AV_IF,tt-1);
				p_mp->gmdb_rollup_valn 	= this->get_main_proj_array(GMDB_PREM_IF,tt-1);
				p_mp->gmwb_ratchet_valn 	= this->get_main_proj_array(GMWB_AV_IF,tt-1); 
				p_mp->gmwb_rollup_valn 	= this->get_main_proj_array(GMWB_PREM_IF,tt-1);
				p_mp->prem_paid = this->model_point->prem_paid * this->get_main_proj_array(SURV_INDEX,tt);
				// Modif AV

				p_mp->total_av = 0;
				for (int i=0; i< MAXIMUM_NUMBER_OF_INDEXES; i++)
				{
					p_mp->av_split_prop_array[i] = this->get_index_weights(i,tt);
					p_mp->av_if_array[i] = this->get_main_proj_array(AV_E,tt-1)*p_mp->av_split_prop_array[i]/100;
					p_mp->total_av += p_mp->av_if_array[i];
				}
			}
			vector_aged_polices.push_back(p_mp);
		}
		return vector_aged_polices;

	}

	void set_duration_age_step (double duration_age_step)
	{
		this->duration_age_step = duration_age_step;
	}


	/*
	* This function is used by the actuarial module to increment the parameters 
	* of a vector of policies by (EOM_date-BOM_date)/365
	*/
	virtual vector<Model_Point*> age_policies_from_projection_for_aging_mode()
	{
		vector<Model_Point*> vector_aged_polices;
		Model_Point * p_mp = new Model_Point(this->model_point, 1, 0);
			
		//Only one monthly step of aging for the aging policies mode
		int tt = 2;

		p_mp->mp_position_in_file=tt;
	
		p_mp->age_valn = this->get_timing_items(AGE_LAST,tt-1);

		p_mp->duration = this->model_point->duration + this->duration_age_step;

		p_mp->elapsed_months = (int)ceil(p_mp->duration*12);
		p_mp->elapsed_mths_in_valn_yr = (int)((p_mp->duration - (int)p_mp->duration)*12);
		p_mp->elapsed_years = (int) (p_mp->duration);
		// Modif Base
		p_mp->gmab_rollup_valn		= this->get_main_proj_array(GMAB_ROLLUP_BASE,tt-1);
		p_mp->gmab_ratchet_valn	= this->get_main_proj_array(GMAB_RATCHET_BASE,tt-1);
		p_mp->gmdb_ratchet_valn 	= this->get_main_proj_array(GMDB_AV_IF,tt-1);
		p_mp->gmdb_rollup_valn 	= this->get_main_proj_array(GMDB_PREM_IF,tt-1);
		p_mp->gmwb_ratchet_valn 	= this->get_main_proj_array(GMWB_AV_IF,tt-1); 
		p_mp->gmwb_rollup_valn 	= this->get_main_proj_array(GMWB_PREM_IF,tt-1);
		p_mp->prem_paid = this->model_point->prem_paid;
		// Modif AV

		p_mp->total_av = 0;
		for (int i=0; i< MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			p_mp->av_split_prop_array[i] = this->get_index_weights(i,tt);
			p_mp->av_if_array[i] = this->get_main_proj_array(AV_E,tt-1)*p_mp->av_split_prop_array[i]/100;
			p_mp->total_av += p_mp->av_if_array[i];
		}
		vector_aged_polices.push_back(p_mp);
		return vector_aged_polices;

	}


	/*
	* Set the current monthly scenario in the projection (see background_final.h)
	*/
	void memset_main_proj_array()
	{
   		if(this->time_step == ANNUAL)
		{
		    memset(main_proj_array_ANN, 0, sizeof(main_proj_array_ANN));
		}
		else if(this->time_step == MONTHLY)
		{
		    memset(main_proj_array_MTH, 0, sizeof(main_proj_array_MTH));		
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	}

	/*
	* Set timing items tables to zero
	*/
	void memset_timing_items()
	{
		if(this->time_step == ANNUAL)
		{
		    memset(timing_items_ANN, 0, sizeof(timing_items_ANN));		
		}
		else if(this->time_step == MONTHLY)
		{
		    memset(timing_items_MTH, 0, sizeof(timing_items_MTH));		
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	}

	void memset_index_weigths()
	{
		if(this->time_step == ANNUAL)
		{
		    memset(index_weigths_ANN, 0, sizeof(index_weigths_ANN));		
		}
		else if(this->time_step == MONTHLY)
		{
		    memset(index_weigths_MTH, 0, sizeof(index_weigths_MTH));		
		}
		else
		{
		    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	}

	void memset_surv_cumul()
	{
		if(this->time_step == ANNUAL)
		{
		    memset(surv_cumul_ANN, 0, sizeof(surv_cumul_ANN));
		}
		else if(this->time_step == MONTHLY)
		{
		    memset(surv_cumul_MTH, 0, sizeof(surv_cumul_MTH));		
		}
		else
		{
		    char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	}


	void clear_ITM_RA()
	{
		this->recup_ITM_RA.clear();
	}
		
	/*
	* Set the ITM Vect(AV/Base) (used for Risk Appetite)
	*/

    void sort_ITM_RA()
	{
      sort(this->recup_ITM_RA.begin(),this->recup_ITM_RA.end(), inforce_compare);	
	}


	virtual void set_recup_ITM_RA(int t, int scn_number, int mp_number)
	{
		vector<double> tmp;
		double base;
		
		base=max(this->main_proj_array[t][GMAB_ROLLUP_BASE],this->main_proj_array[t][GMAB_RATCHET_BASE]);
		base=max(base,this->main_proj_array[t][GMWB_AV_IF]);
		base=max(base,this->main_proj_array[t][GMWB_PREM_IF]);
		if (this->get_ITM_type()==1)  //AV/BB may be overrided by different products
		{ tmp.push_back(this->main_proj_array[t][AV_E]/base);} //ITM
		else
		{ tmp.push_back(this->main_proj_array[t][AV_E]);}
		tmp.push_back(this->main_proj_array[t][AV_E]);
        tmp.push_back(this->main_proj_array[t][GMAB_RATCHET_BASE]);
		tmp.push_back(this->main_proj_array[t][GMAB_ROLLUP_BASE]);
		tmp.push_back(this->main_proj_array[t][GMDB_AV_IF]);
		tmp.push_back(this->main_proj_array[t][GMDB_PREM_IF]);
		tmp.push_back(this->main_proj_array[t][GMWB_AV_IF]);
		tmp.push_back(this->main_proj_array[t][GMWB_PREM_IF]);
        tmp.push_back(this->main_proj_array[t][SURV_INDEX]);
		tmp.push_back((double)(scn_number)); //scn number
        tmp.push_back((double)(mp_number)); //mp number


		this->recup_ITM_RA.push_back(tmp);		
	}

	/*
	* Get the ITM Vect(AV/Base) (used for Risk Appetite)
	*/
	vector<vector<double>> get_recup_ITM_RA()
	{
		return this->recup_ITM_RA;		
	}
	
	/*
	* Get risk_appetite parameters
	*/

	virtual int get_ITM_type()
	{
		return this->ITM_type;
	}

    virtual int get_output()
	{
		return this->OutputAll;
	}

	virtual string get_risk_appetite()
	{
		return this->risk_appetite;		
	}

	virtual double get_perc_1()
	{
		return this->perc_1;
	}

	virtual double get_perc_2()
	{
		return this->perc_2;
	}

	virtual double get_perc_3()
	{
		return this->perc_3;
	}
	
	virtual int get_t_RA()
	{
		return this->t_RA;
	}
	
	void initialize_copied_inforce(string** initial_inforce,string** copied_inforce,int nb_col,int nb_mps)
	{
		for(int i = 0; i <nb_mps; i++)
		{
			for(int j=0;j<nb_col;j++){
			
				copied_inforce[i][j] = initial_inforce[i][j];
			}
		}
	}

	virtual void Load_config_by_product(int t,int n_MP, string** inforce,double perc, int mode){}

	/*
	* set the model point parameters 
	* from the projection out (used for hedge effectiveness)
	*/
	virtual void get_older_policy(int t,int n_MP, string** inforce,double perc, int mode)
	{
		int size=this->recup_ITM_RA.size();
		int RA_number;
		char temp[100];
		sprintf(temp,"%s_%2f",inforce[n_MP][GROUP].c_str(),perc);
		
		if (((int)(perc*size))<=0)
		{RA_number= 0;}
		else
		{RA_number=min((int)(perc*size),size)-1;}

		bool ab_rat=((atof(inforce[n_MP][GMAB_RATCHET_VALN].c_str())==(0)) ? (0) : (1));
		bool ab_rol=((atof(inforce[n_MP][GMAB_ROLLUP_VALN].c_str())==(0)) ? (0) : (1));
		bool db_rat=((atof(inforce[n_MP][GMDB_RATCHET_VALN].c_str())==(0)) ? (0) : (1));
		bool db_rol=((atof(inforce[n_MP][GMDB_ROLLUP_VALN].c_str())==(0)) ? (0) : (1));
		bool wb_rat=((atof(inforce[n_MP][GMWB_RATCHET_VALN].c_str())==(0)) ? (0) : (1));
		bool wb_rol=((atof(inforce[n_MP][GMWB_ROLLUP_VALN].c_str())==(0)) ? (0) : (1));

		// Modification timing		
		inforce[n_MP][AGE_AT_VALN]=nb_to_string(this->timing_items[t+1][AGE_LAST]);
		inforce[n_MP][DURATION_VALN]= nb_to_string(this->model_point->duration+t);
        inforce[n_MP][GROUP]=temp;
		//// Modif Base
		inforce[n_MP][PREM_PAID]=nb_to_string(this->model_point->prem_paid) ;
        if (mode==1) //debug mode
		{
			inforce[n_MP][GMAB_RATCHET_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][2]);//GMAB_RATCHET_BASE
			inforce[n_MP][GMAB_ROLLUP_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][3]);//GMAB_ROLLUP_BASE
			inforce[n_MP][GMDB_RATCHET_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][4]);//GMDB_AV_IF (ratchet)
			inforce[n_MP][GMDB_ROLLUP_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][5]);//GMDB_PREM_IF (rollup)
			inforce[n_MP][GMWB_RATCHET_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][6]);//GMWB_AV_IF (ratchet)
			inforce[n_MP][GMWB_ROLLUP_VALN]=nb_to_string(this->recup_ITM_RA[RA_number][7]);//GMWB_PREM_IF (rollup)
		}
		else //generate the inforce for pricing
		{
            inforce[n_MP][GMAB_RATCHET_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][2],this->recup_ITM_RA[RA_number][3])*ab_rat);//Take the MAX
            inforce[n_MP][GMAB_ROLLUP_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][2],this->recup_ITM_RA[RA_number][3])*ab_rol);
            inforce[n_MP][GMDB_RATCHET_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][4],this->recup_ITM_RA[RA_number][5])*db_rat);
			inforce[n_MP][GMDB_ROLLUP_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][4],this->recup_ITM_RA[RA_number][5])*db_rol);
			inforce[n_MP][GMWB_RATCHET_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][6],this->recup_ITM_RA[RA_number][7])*wb_rat);
			inforce[n_MP][GMWB_ROLLUP_VALN]=nb_to_string(max(this->recup_ITM_RA[RA_number][6],this->recup_ITM_RA[RA_number][7])*wb_rol);	
		}
		inforce[n_MP][PREM_PAID]=nb_to_string(this->recup_ITM_RA[RA_number][8]*atof(inforce[n_MP][PREM_PAID].c_str()));// accumulated survival rate
		inforce[n_MP][WB_PAYT_YR]="0";
		//// Modif AV
		for (int i=0; i< MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			inforce[n_MP][TARGET_AV_SPLIT_VALN_INDEX0 + i]=nb_to_string(this->model_point->av_split_prop_array[i]);
			inforce[n_MP][AV_VALN_INDEX0+i]=nb_to_string(this->model_point->av_split_prop_array[i]/100*this->recup_ITM_RA[RA_number][1]);//AV_E
		}

		Load_config_by_product(t,n_MP,inforce,perc,mode);

	}
	
	/*
	* Set the current model point.
	* Load all the tables related to the model points
	* compute all the timing items once (available for all the scenarios)
	*/
	virtual void set_current_model_point(Model_Point *mp)
	{
		this->model_point = mp;
		this->load_tables();
		this->term_to_omega = this->term_to_omega_age_s();
		this->maturity_yr = this->maturity_year_s();
		this->memset_timing_items();
		
		for(int tt =0; tt <= this->subacct_max_period; tt++)
		{
			this->calculate_items_for_timing(tt);
		}
		this->product_charging_duration = this->product_charging_duration_s();

		// calculate funds and index_weights
		if (!this->model_point->load_model_point_funds)
		{
			this->initialize_index_weights();
		}
		else
		{
			this->model_point->init_fund_arrays();
		}
	}

    void initialized_pvstemp_to_Zero()
	{
		//! pvs_..._temps
		for (int i = 0; i < this->model_point->lapse_size; i++)
		{ 
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)
				this->pvs_scenByscen.pv_lapse_scen_output_temp[i][j]=0;
		}

		for (int i = 0; i < this->model_point->mortality_size; i++)
		{
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)
				this->pvs_scenByscen.pv_mortality_scen_output_temp[i][j] = 0;
		}

		for (int i = 0; i < this->model_point->paidup_size; i++)
		{
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)
				this->pvs_scenByscen.pv_paidup_scen_output_temp[i][j] = 0;

		}

		for (int i = 0; i < this->model_point->longevity_size; i++)
		{
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)
				this->pvs_scenByscen.pv_longevity_scen_output_temp[i][j] = 0;
		}

		for (int i = 0; i < this->model_point->index_shock_size; i++)
		{
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)				
				this->pvs_scenByscen.index_shock_pvs_scen_output_temp[i][j] = 0;
		}	

		for (int i = 0; i < this->model_point->fund_shock_size; i++)
		{
			for(int j=0; j<this->model_point->get_total_pv_columns_number(); ++j)
				this->pvs_scenByscen.fund_shock_pvs_scen_output_temp[i][j] = 0;
		}
	}
	/*
	* This function id used for the Hedge effectiveness. It ages the policyholder with 
	* the parameters of the outer projection.
	* proj_out : Projection outer
	* index_shock_size_in : index  shock size
	* fund_shock_size_in : fund shock size
	*/
	virtual void set_current_model_point (Projection* proj_out, int t, string* inforce, 
					int index_shock_size_in, int fund_shock_size_in)
	{	
		this->model_point = new Model_Point(proj_out->model_point, index_shock_size_in, fund_shock_size_in);
		this->model_point->mp_position_in_file=t;

		// use the parameter of the 
		// outer projection to age the model point
		if (t!=1)
			this->get_older_policy(proj_out,t, inforce);

		this->load_tables();
		this->term_to_omega = this->term_to_omega_age_s();
		this->maturity_yr = this->maturity_year_s();
		memset(this->timing_items, 0, sizeof(this->timing_items));
		for(int tt =0; tt <= this->subacct_max_period; tt++)
		{
			this->calculate_items_for_timing(tt);
		}
		
		this->product_charging_duration = this->product_charging_duration_s();
		if (!this->model_point->load_model_point_funds)
		{
			this->initialize_index_weights();
		}
		else
		{
			this->model_point->init_fund_arrays();
		}
	}
	/*
	* Flush the main_proj_array_2 table
	*/
	virtual void display_debug_info(int t)
	{		
		for(int col = 0; col < MAIN_PROJ_NUM_COLS ; col++)
			this->debug_file << setprecision(15)<< this->main_proj_array[t][col] << ",";
		for(int item_col = 0; item_col < TOTAL_TIMING_ITEMS; item_col++)
			this->debug_file << setprecision(15)<< this->timing_items[t][item_col] << ",";

		this->debug_file << endl;
	}
	
	/*
	* Write the debug file header
	*/
	virtual void set_debug_file_header()
	{
		this->debug_file <<  "ADMIN_FEE,ANN_CLAIMS_AV,AV_B,AV_B_BEF,AV_E,AV_E_BEF,AV_GROWTH,AV_GROWTH_AVE,AV_GROWTH_RATE,";
		this->debug_file <<  "AV_REBAL,BOND_RATE_FOR_LAPSES,DISCOUNT_FACTOR,DEATH_CLAIMS_AV,GMXB_CHARGE,AV_E_BEF_CLAIMS,M_AND_E_FEE,";
		this->debug_file <<  "MAT_CLAIMS_AV,PREMIUM_ALLOC,PREMIUM_BONUS,SURR_CLAIMS_AV,SURR_FPW,GMWB_ANN_PAID,";
		this->debug_file <<  "PD_UP_SURR_CHG,AV_B_BEF_BIS,GMWB_PAYMENT_FROM_AV,ANN_CLAIMS_GMIB,ANN_RATE,ANN_RATE_BASE,";
		this->debug_file <<  "GMDB_CLAIMS,DEATH_RATE,FPW_RATE_AV, GMAB_IF,GMAB_CLAIMS,GMAB_NAR,GMDB_AV_IF,";
		this->debug_file <<  "GMDB_AV_IF_BEF,GMDB_IF,GMDB_IF_BEF,GMDB_PAYMENT,GMDB_NAR_BEF,GMDB_PREM_IF,";
		this->debug_file <<  "GMDB_PREM_IF_BEF,GMDB_ROLLUP_CAP_IF,GMDB_ROLLUP_CAP_IF_BEF,GMIB_AX_CURR,GMIB_AX_GUAR,";
		this->debug_file <<  "GMIB_CURR_DEATH_RATE,GMIB_GUAR_DEATH_RATE,GMIB_IF,GMIB_IF_BEF,GMIB_NAR,GMIB_NAR_BEF,";
		this->debug_file <<  "GMIB_RATCHET_IF,GMIB_RATCHET_IF_BEF,GMIB_ROLLUP_CAP_IF,GMIB_ROLLUP_CAP_IF_BEF,";
		this->debug_file <<  "GMIB_ROLLUP_IF,GMIB_ROLLUP_IF_BEF,LAPSE_RATE,SURV_CUM,PREMIUM,PREMIUM_ACCUM,";
		this->debug_file <<  "PREMIUM_ACCUM_BEF,PREMIUM_ANN_IF_B,PREMIUM_ANN_IF_E,SURV_MTH,GMWB_CLAIMS,GMWB_AV_IF,";
		this->debug_file <<  "GMWB_AV_IF_BEF,GMWB_IF,GMWB_IF_BEF,GMWB_PREM_IF,GMWB_PREM_IF_BEF,GMWB_ROLLUP_CAP_IF,";
		this->debug_file <<  "GMWB_ROLLUP_CAP_IF_BEF,GMWB_MAX_WDL_AMT,GMWB_PAYMENT,ONG_PREM_SURR_CHG,HC_PREM,";
		this->debug_file <<  "GMIB_DAX_GUAR,GMSB_PREM_IF,GMSB_GUAR_PAYMENT,GMSB_CLAIMS,GMAB_RATCHET_BASE,";
		this->debug_file <<  "GMAB_ROLLUP_BASE,SURV_INDEX,GMWB_RATCHET_PREV,GMWB_ROLLUP_PREV,ITM,GMSB_IF, GMIB_CLAIMS, TECHNICAL_RATE, POL_MONTH, POL_YEAR, AGE_LAST, PRORATA";
		this->debug_file << endl;
																																				
	}


	/*
	* Initialize the index_map_2 and index_map_size
	* according to the model point characteristics
	*/
	virtual void initialise_index_map()
	{
		int counter = 0;
		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
		{
			if (this->model_point->av_split_prop_array[i] != 0 || this->model_point->av_if_array[i]!=0)
			{ 
				index_map_2[counter] = i+1;
				counter = counter + 1;
			}
		}
		index_map_size = counter;
	}        

	/*
	* Reset the main_proj_array_2 to 0
	*/ 
	inline void set_main_proj_arrays_to_zero()
	{
		this->memset_main_proj_array();
	}

	/*
	* Initialize index weights 
	*/ 
	virtual void initialize_index_weights()
	{

		if (!this->model_point->load_model_point_funds)
		{
			memset_index_weigths();
			
			if (this->model_point->av_rebal_ind == Model_Point::DRIFT)
			{
				for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
					for (int j = 0; j < this->subacct_max_period + 1 ; j++)
					{
						// For checking if the av is 0
						if (this->model_point->total_av < SMALL_DOUBLE)
						    this->index_weigths[j][i]  = 0.;
						else
							this->index_weigths[j][i] = this->model_point->av_if_array[i] * 100.0 / this->model_point->total_av;		
					}
			}
			else if (this->av_split_adj == "Y")
			{
				double av_split_prop_tot = 0.0;
				for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
				{
					av_split_prop_tot += this->model_point->av_split_prop_array[i];
				}
				for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
				{
					for (int j = 0; j < this->subacct_max_period + 1 ; j++)
					{
						if (this->model_point->total_av < SMALL_DOUBLE)
						{
						    this->index_weigths[j][i]  = 0.;
						}
						else
						{
						if (j == 1)
							this->index_weigths[j][i] = this->model_point->av_if_array[i] * 100.0 / this->model_point->total_av;

							else
							{
								this->index_weigths[j][i] = this->model_point->av_split_prop_array[i] * 100.0 / av_split_prop_tot;
								
								// check this line and look why av_split_target # av_split_prop_array for HE on MED AB products
								if (this->is_he == true)
									this->index_weigths[j][i] = this->model_point->av_split_target[i] * 100.0 / av_split_prop_tot;
							}
						}
					}
				}
			}
			else
			{
				for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
					for (int j = 0; j < this->subacct_max_period + 1 ; j++)
					{
						if (this->model_point->total_av < SMALL_DOUBLE)
						{
						    this->index_weigths[j][i]  = 0.;
						}
						else
						{
						if (j == 1)
							this->index_weigths[j][i] = this->model_point->av_if_array[i] * 100.0 / this->model_point->total_av;
						else
							this->index_weigths[j][i] = this->model_point->av_split_prop_array[i];		
						}
					}
			}
		}
	}

	
	/*
	* Update the index weights 
	* from one shock to another
	*/
	void update_index_weights()
	{

		if (this->model_point->av_rebal_ind == Model_Point::DRIFT)
		{
			memset_index_weigths();
		}

		//for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
		for (int i = 0; i < index_map_size; i++)
		{
			int index_num = index_map_2[i];
			if (this->model_point->total_av < SMALL_DOUBLE)
			{
				this->index_weigths[0][index_num - 1] = 0;		
				this->index_weigths[1][index_num - 1] = 0;		
			}
			else
			{
			this->index_weigths[0][index_num - 1] = this->model_point->av_if_array[index_num - 1] * 100.0 / this->model_point->total_av;		
			this->index_weigths[1][index_num - 1] = this->model_point->av_if_array[index_num - 1] * 100.0 / this->model_point->total_av;		
			}
		}
	}

	/*
	* Set the lapse multiplicator in the projection
	* The lapse multiplicator table is read from the configuration 
	* file
	*/
	void set_lapse_mult_table(const vector<GMXB_Assumption_Table*>& v)
	{
		this->v_lapse_mult_table.clear();
		if (v.size() > 0)
		{
			for (unsigned int i =0; i< v.size(); i++ )
			{
				GMXB_Assumption_Table * tab = v[i];
				this->v_lapse_mult_table.push_back(tab);
			}
		}
	}

	/*
	* Set the mortality multiplicator in the projection
	* The mortality multiplicator table is read from the configuration 
	* file
	*/
	void set_mortality_mult_table(const vector<GMXB_Assumption_Table*>& v)
	{
		this->v_mortality_mult_table.clear();
		if (v.size() > 0)
		{
			for (unsigned int i =0; i< v.size(); i++ )
			{
				GMXB_Assumption_Table * tab = v[i];
				this->v_mortality_mult_table.push_back(tab);
			}
		}
	}

	void set_paidup_mult_table(const vector<GMXB_Assumption_Table*>& v)
	{
		this->v_paidup_mult_table.clear();
		if (v.size() > 0)
		{
			for (unsigned int i =0; i< v.size(); i++ )
			{
				GMXB_Assumption_Table * tab = v[i];
				this->v_paidup_mult_table.push_back(tab);
			}
		}
	}

	void set_election_mult_table(const vector<GMXB_Assumption_Table*>& v)
	{
		this->v_election_mult_table.clear();
		if (v.size() > 0)
		{
			for (unsigned int i =0; i< v.size(); i++ )
			{
				GMXB_Assumption_Table * tab = v[i];
				this->v_election_mult_table.push_back(tab);

			}
		}
	}
	//! TODO add if else to use less memory
	virtual void set_pvsScenOutput(Model_Point& mp,  unsigned int nScenPath_ip)
	{
			pvs_scenByscen.li_nScenPath = nScenPath_ip;

			//! scen_by_scen output
			pvs_scenByscen.pv_lapse_scen_output = vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.lapse_size,
																  vector<double>(mp.get_total_pv_columns_number(),0)));

			pvs_scenByscen.pv_mortality_scen_output = vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.mortality_size,
										   vector<double>(mp.get_total_pv_columns_number(),0)));

			pvs_scenByscen.pv_paidup_scen_output = vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.paidup_size,
										   vector<double>(mp.get_total_pv_columns_number(),0)));

			pvs_scenByscen.pv_longevity_scen_output =  vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.longevity_size,
										   vector<double>(mp.get_total_pv_columns_number(),0)));

			pvs_scenByscen.index_shock_pvs_scen_output = vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.index_shock_size,
										   vector<double>(mp.get_total_pv_columns_number(),0)));

			pvs_scenByscen.fund_shock_pvs_scen_output = vector<vector<vector<double>>>(pvs_scenByscen.li_nScenPath,
				                           vector<vector<double>>(mp.fund_shock_size,
										   vector<double>(mp.get_total_pv_columns_number(),0)));

			//! temp
			pvs_scenByscen.pv_lapse_scen_output_temp = vector<vector<double>>(mp.lapse_size,
											vector<double>(mp.get_total_pv_columns_number(),0));

			pvs_scenByscen.pv_mortality_scen_output_temp = vector<vector<double>>(mp.mortality_size,
											vector<double>(mp.get_total_pv_columns_number(),0));

			pvs_scenByscen.pv_paidup_scen_output_temp = vector<vector<double>>(mp.paidup_size,
											vector<double>(mp.get_total_pv_columns_number(),0));

			pvs_scenByscen.pv_longevity_scen_output_temp = vector<vector<double>>(mp.longevity_size,
												vector<double>(mp.get_total_pv_columns_number(),0));

			pvs_scenByscen.index_shock_pvs_scen_output_temp = vector<vector<double>>(mp.index_shock_size,
													vector<double>(mp.get_total_pv_columns_number(),0));
	
			pvs_scenByscen.fund_shock_pvs_scen_output_temp = vector<vector<double>>(mp.fund_shock_size,
													vector<double>(mp.get_total_pv_columns_number(),0));	
	}



	/*
	* List of setters of Actuarial module
	*/
	void set_lapse_mult_for_aging_policies(double lapse_mult)
	{
		this->lapse_mult_for_ap = lapse_mult;
	}

	void set_mortality_mult_for_aging_policies(double mortality_mult)
	{
		this->mortality_mult_for_ap = mortality_mult;
	}
	
	void set_charges_mult_for_aging_policies(double charges_mult)
	{
		this->charges_mult_for_ap = charges_mult;
	}

	void set_duration_age_step_for_aging_policies (double duration_age_step)
	{
		this->duration_age_step = duration_age_step;
	}

protected: 

	/*
	* This function returns the expectation number of year
	* for the policyholder to live
	*/
	int term_to_omega_age_s() 
	{
		int omega_age =  (int)(this->mort_table->get_row_high_index() + this->mort_table->get_column_high_index()-1);
		return omega_age - (int)(floor(this->model_point->age_exact_issue) + this->mort_age_adj);
	}

	/*
	* return the maturity of the contract
	* min (60 years, expectaion number of years to live)
	*/
	virtual int maturity_year_s() 
	{
		if (this->time_step == ANNUAL)
			return min((int)t_high_shorter , (int)(this->term_to_omega - this->model_point->elapsed_years + 1));
			
		else
			return min((int)t_high_shorter *this->time_step, (int)(this->term_to_omega*this->time_step - this->model_point->elapsed_years*this->time_step + 1*this->time_step-this->model_point->elapsed_months ));

	}

	/*
	* Calculates all the timing items
	*/
	
	void calculate_items_for_timing(int t) 
	{
		if (this->time_step == ANNUAL)
			this->calculate_items_for_timing_annual(t);
		else if (this->time_step == MONTHLY)
			this->calculate_items_for_timing_mth(t);
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	}


	/*
	* Calculates all the timing items when the projection is of type ANNUAL
	*/
	virtual void calculate_items_for_timing_annual(int t) 
	{
		if (t == 0 && model_point->elapsed_months < 0)
			throw new GMXBException("Either t = 0 or elapsed_months is negative");

		else if (t == 0 && model_point->elapsed_months >= 0)
		{
			this->timing_items[t][POL_MONTH] = model_point->elapsed_months;
		}
		else 
		{
			this->timing_items[t][POL_MONTH] = model_point->elapsed_months + (t-1) * 12.;
		}

		if (t == 1) 
		{
		//	this->timing_items[POL_YEAR][t] = this->model_point->elapsed_years + 1.0; //changed by sheng
			this->timing_items[t][POL_YEAR] = (int)(this->model_point->duration) + 1.0;
		}
		else {
			this->timing_items[t][POL_YEAR] = this->timing_items[t-1][POL_YEAR] + 1.0;
		}

		if (this->projection_type == "New_Bus" && t == 0)
		{
			this->timing_items[t][AGE_LAST] = 0.;
		}
		else
		{
			this->timing_items[t][AGE_LAST] = (int)(model_point->age_exact_issue) + this->timing_items[t][POL_YEAR] - 1.0;
		}

		if (t==1)
		{
			this->timing_items[t][PRORATA] = (1. - this->model_point->elapsed_mths_in_valn_yr/12. ) ;
		}
		else
		{
			this->timing_items[t][PRORATA] = 1.;
		}
	} 


	/*
	* Calculates all the timing items when the projection is of type ANNUAL
	*/
	virtual void calculate_items_for_timing_mth(int t) 
	{
		if (t == 0 && model_point->elapsed_months < 0)
			throw new GMXBException("Either t = 0 or elapsed_months is negative");

		else if (t == 0 && model_point->elapsed_months >= 0)
		{
			this->timing_items[t][POL_MONTH] = model_point->elapsed_months;
		}
		else 
		{
			this->timing_items[t][POL_MONTH] = this->timing_items[t-1][POL_MONTH]+1;//model_point->elapsed_months + (t-1);
		}
		
		this->timing_items[t][POL_YEAR] =int((this->timing_items[t][POL_MONTH]-1)/12.)+1;

		if (t == 1 && this->project_aging_policies)
		{
			this->timing_items[t][POL_YEAR] = int(model_point->duration + this->duration_age_step)+1;
		}

		if (this->projection_type == "New_Bus" && t == 0)
		{
			this->timing_items[t][AGE_LAST] = 0.;
		}
		else if (t==0)
			this->timing_items[AGE_LAST][t] = model_point->age_valn;
		else
		{
			// HE ALi this->timing_items[t][AGE_LAST] = model_point->age_exact_issue + (this->timing_items[t][POL_MONTH]-1)/12.;
			this->timing_items[t][AGE_LAST] = model_point->age_exact_issue + (this->timing_items[t][POL_MONTH]-1)/12.;
		}

		if (t == 1 && this->project_aging_policies)
		{
			this->timing_items[t][AGE_LAST] = model_point->age_valn + this->duration_age_step;
		}

		this->timing_items[t][PRORATA] = 1.;
	}

	/* 
	* Function to adjust the rate to the time step of the projection
	*/
	inline double adjusted_rate(double rate, int time_step, int method)
	{
		if (this->time_step == ANNUAL)
		{
			return rate;
		}
		else if (this->time_step == MONTHLY)
		{
			if (method == 1)
			{
				return pow((1.0 + rate),1.0/(double)time_step)-1;
			}
			else if (method == -1)
			{
				return 1 - pow((1.0 - rate),1.0/(double)time_step);
			}
			else if (method == 0)
			{
				return (rate /time_step);
			}
			else 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "method (=%d) is not correct in the function adjusted_rate", method);
				throw new GMXBException(message);
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);		
		}
	}



	virtual int product_charging_duration_s()
	{
		if (this->time_step == ANNUAL)
			return this->maturity_yr + this->model_point->elapsed_years;
		else if (this->time_step == MONTHLY)
			return T_Max_Monthly;
		else
			//Actuarial tool
			return this->maturity_yr + (this->model_point->elapsed_years - 1)*this->time_step+(this->model_point->elapsed_months);
}


	/*
	* 
	*/
	virtual double regular_premium_calculation(int t)
	{
		double pol_yr = (int)(this->timing_items[t][POL_YEAR]);
		if (t == 1)
		{
			this->main_proj_array[0][SURV_INDEX] = 1.0;
			this->main_proj_array[t][PREMIUM] = max(0.0, model_point->prem_curr 
						* model_point->payt_freq 
						* this->premium_schedular- model_point->year_prem
					   );
			return this->main_proj_array[t][PREMIUM];
		}
		else 
		{
			if ((int)(this->timing_items[t][POL_YEAR]) <= prem_duration)
			{
				this->main_proj_array[t][PREMIUM] = max(0.0,(model_point->prem_curr 
								* model_point->payt_freq 
								* this->premium_schedular * surv_cumul[t-1]
								)
						   );
				return this->main_proj_array[t][PREMIUM];
			}
			else
			{
				this->main_proj_array[t][PREMIUM] = 0.0;
				return this->main_proj_array[t][PREMIUM];			
			}
		}
	}

	virtual void avBBef(int t)
	{
		double prem = regular_premium_calculation(t); 
		main_proj_array[t][AV_B_BEF] = main_proj_array[t-1][AV_E] + prem;
	}

	virtual	void avBBefBis(int t)
	{
		main_proj_array[t][AV_B_BEF_BIS] = max(0., main_proj_array[t][AV_B_BEF]);
	}

	/*
	* This function performs a shock on the initial Account value
	*/
	virtual void delta_shock(int t)
	{
		// Case of Index projection
		if (!this->model_point->load_model_point_funds)
		{
			if(this->index_shock_array[this->index_shock_number][0]==(MAXIMUM_NUMBER_OF_INDEXES+1))
			{
				main_proj_array[t][AV_B_BEF] *= (this->index_shock_array[this->index_shock_number][1]/100.0+1);
			}
			else if(this->index_shock_array[this->index_shock_number][0]<=MAXIMUM_NUMBER_OF_INDEXES && this->index_shock_array[this->index_shock_number][0]>0)
			{
				int index_num = this->index_shock_array[this->index_shock_number][0];
				
				double premium_alloc = 0.;
				double step = 0.;
				double shock_value = this->index_shock_array[this->index_shock_number][this->index_shock_array[this->index_shock_number][0]]/100.0;
				double prem = regular_premium_calculation(t); 
				premium_alloc = prem * this->index_weigths[t][index_num -1]/100.0;
				step = (this->model_point->av_if_array[index_num - 1] + premium_alloc) * shock_value;
				main_proj_array[t][AV_B_BEF] += step;
				
				// Recompute the indexes weight due to the index shock
				double weighted_shock = 1 + this->index_weigths[t][index_num-1]/100. * shock_value;
				for(int i = 0; i < index_map_size; i++)
				{
					int index_n = index_map_2[i];
					if(index_n == index_num)
					{
						this->index_weigths[t][index_n-1] = this->index_weigths[t][index_n-1]*(1 + shock_value)/weighted_shock;
					}
					else
					{
						this->index_weigths[t][index_n-1] = this->index_weigths[t][index_n-1]/weighted_shock;
					}

					if (this->model_point->av_rebal_ind == Model_Point::DRIFT)
					{
						this->index_weigths[t - 1][index_n-1] = this->index_weigths[t][index_n-1];
					}

				}
			}
		}
		// Case of Fund projection
		else
		{
			// Case of Fund shock
			if (this->shock_fund)
			{
				// Get the shock value from this->fund_shock_array
				double shock_value = this->fund_shock_array[this->fund_shock_number][this->fund_shock_array[this->fund_shock_number][0]]/100.0;


				int shortsellable_fund_id =  this->fund_shock_array[this->fund_shock_number][0];
				// Shock the fund
				for (unsigned int i = 0; i < this->model_point->list_of_funds.size(); i++)
				{
					if (this->model_point->list_of_funds[i].fund.fund_id == shortsellable_fund_id)
					{
						main_proj_array[t][AV_B_BEF] *= (1 + shock_value * this->model_point->list_of_funds[i].alpha_t[0]);
												
						// Recompute the alpha for this fund
						this->model_point->list_of_funds[i].alpha_t[0] =(
											this->model_point->list_of_funds[i].number_share *
											this->model_point->list_of_funds[i].fund.get_fund_value()
											)*(1+shock_value)/main_proj_array[t][AV_B_BEF];


						break;
					}
				}
				// Recompute the alphas for all the other funds
				for (unsigned int i = 0; i < this->model_point->list_of_funds.size(); i++)
				{
					if (this->model_point->list_of_funds[i].fund.fund_id != shortsellable_fund_id)
					{
						this->model_point->list_of_funds[i].alpha_t[0] = (
							this->model_point->list_of_funds[i].number_share*
							this->model_point->list_of_funds[i].fund.get_fund_value()
						)/main_proj_array[t][AV_B_BEF];
					}
				}
			}
			// Case of Index shock
			else
			{
				// Index shock
				if (this->index_shock_array[this->index_shock_number][0]<=MAXIMUM_NUMBER_OF_INDEXES && this->index_shock_array[this->index_shock_number][0]>0)
				{
					double shock_value = this->index_shock_array[this->index_shock_number][this->index_shock_array[this->index_shock_number][0]]/100.0;
					int index_num = this->index_shock_array[this->index_shock_number][0];
										
					double sum_coeff = 0;
					for (unsigned int i = 0; i < this->model_point->list_of_funds.size();i++)
					{
						if (!this->model_point->list_of_funds[i].is_short_sellable_fund)
						{
							sum_coeff += this->model_point->list_of_funds[i].alpha_t[0] * 
											this->model_point->list_of_funds[i].fund.current_beta[index_num-1];
						}
					}

					main_proj_array[t][AV_B_BEF] *= (1 + sum_coeff*shock_value);
					// Recompute the alphas
					for (unsigned int i = 0; i < this->model_point->list_of_funds.size();i++)
					{
						if (!this->model_point->list_of_funds[i].is_short_sellable_fund)
						{
							this->model_point->list_of_funds[i].alpha_t[0] = this->model_point->list_of_funds[i].alpha_t[0]*
														(
															1 + this->model_point->list_of_funds[i].fund.current_beta[index_num-1]
															* shock_value
														 )/ (1 + sum_coeff * shock_value);
						}
						else
						{
							this->model_point->list_of_funds[i].alpha_t[0] = this->model_point->list_of_funds[i].alpha_t[0]/(1 + sum_coeff * shock_value);						
						}
					}
				}
				// Global shock on all the indexes
				else if (this->index_shock_array[this->index_shock_number][0]==(MAXIMUM_NUMBER_OF_INDEXES+1))
				{
					main_proj_array[t][AV_B_BEF] *= (1 + this->index_shock_array[this->index_shock_number][1]/100.0);
				}
			}

		}
	}


	virtual double specific_fees_by_product(int t) 
	{
		if (this->time_step == ANNUAL)
			return (model_point->gmxb_a_c / 100.0) * main_proj_array[t][AV_B_BEF_BIS];

		else if (this->time_step == MONTHLY)
		{
			if (this->payment_fees_frequency == EACH_T)
				return adjusted_rate((model_point->gmxb_a_c / 100.0), this->time_step, -1) * main_proj_array[t][AV_B_BEF_BIS];
			else
				return (model_point->gmxb_a_c / 100.0) * main_proj_array[t][AV_B_BEF_BIS];
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);	
		}

	}


	void mAndEFee(int t)
	{
		if (this->time_step == ANNUAL)
		{
			if (main_proj_array[t][AV_B_BEF_BIS] < SMALL_DOUBLE)
			{
				main_proj_array[t][ADMIN_FEE] = 0.0;
				main_proj_array[t][M_AND_E_FEE] = 0.0;
			}
			else
			{
				main_proj_array[t][ADMIN_FEE] = this->timing_items[t][PRORATA] * model_point->admin_fee_dollar;
				main_proj_array[t][M_AND_E_FEE] = this->timing_items[t][PRORATA] * specific_fees_by_product(t);
			}
		}
		else if (this->time_step == MONTHLY)
		{
			if (main_proj_array[t][AV_B_BEF_BIS] < SMALL_DOUBLE ||(this->payment_fees_frequency == EACH_BEG_YEAR) && 
					((int)this->timing_items[t][POL_MONTH] % 12 != 0) 
				)
			{
				main_proj_array[t][ADMIN_FEE] = 0.0;
				main_proj_array[t][M_AND_E_FEE] = 0.0;
			}
			else
			{

				main_proj_array[t][ADMIN_FEE] = this->timing_items[t][PRORATA] *  model_point->admin_fee_dollar;
				main_proj_array[t][M_AND_E_FEE] = this->timing_items[t][PRORATA] * specific_fees_by_product(t);
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);			
		}
	}

	virtual void gmxbCharge(int t)
	{

		if (this->time_step == ANNUAL)
		{
			if (this->timing_items[t][POL_YEAR] <= this->product_charging_duration)	
			{
					main_proj_array[t][GMXB_CHARGE] = 
													this->timing_items[t][PRORATA] 
												* (model_point->gmxb_g_c / 100.0) 
												* main_proj_array[t][AV_B_BEF_BIS];
			}
			else
			{
					main_proj_array[t][GMXB_CHARGE] = 0.0;
			}
		}
		else if (this->time_step == MONTHLY)
		{
			if ((this->timing_items[t][POL_YEAR] <= this->product_charging_duration) && 
				((this->payment_fees_frequency == EACH_BEG_YEAR) && 
				((int)this->timing_items[t][POL_MONTH] % 12 == 0) || this->payment_fees_frequency == EACH_T)
				)	
			{
				double gmxb_g_c;
			

				if (this->payment_fees_frequency == EACH_BEG_YEAR)
					gmxb_g_c = (model_point->gmxb_g_c / 100.0);
				else
					gmxb_g_c = adjusted_rate((model_point->gmxb_g_c / 100.0), time_step, -1);


				main_proj_array[t][GMXB_CHARGE] = this->timing_items[t][PRORATA] 
												
													* gmxb_g_c
													* main_proj_array[t][AV_B_BEF_BIS];
			}
			else
					main_proj_array[t][GMXB_CHARGE] = 0.0;
		
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);					
		}
	}

	void avB(int t)
	{
		this->mAndEFee(t);
		this->gmxbCharge(t);

		if(this->project_aging_policies)
			main_proj_array[t][GMXB_CHARGE] = main_proj_array[t][GMXB_CHARGE] * this->charges_mult_for_ap;

		main_proj_array[t][AV_B] = main_proj_array[t][AV_B_BEF_BIS]
								 - main_proj_array[t][ADMIN_FEE] 
								 - main_proj_array[t][M_AND_E_FEE]
								 - main_proj_array[t][GMXB_CHARGE];
							
	}

	void avGrowth(int t)
	{

		if (!this->project_aging_policies)
		{
			if (this->time_step == ANNUAL)
			{
				this->avGrowth_annual(t);
			}
			else if (this->time_step == MONTHLY)
			{
				this->avGrowth_monthly(t);
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
				throw new GMXBException(message);
			}
		}
		else
		{
			main_proj_array[t][AV_GROWTH_RATE] = this->avGrowthRate;
			main_proj_array[t][AV_GROWTH] = main_proj_array[t][AV_B] * main_proj_array[t][AV_GROWTH_RATE];
		}
	}

	virtual void avGrowth_monthly(int t)
	{
		int index_num;
		main_proj_array[t][AV_GROWTH]= 0.0;

		if ((this->model_point->av_rebal_ind == Model_Point::REBALANCE && this->rebalance_frequency == EACH_T) || 
					(this->model_point->av_rebal_ind == Model_Point::REBALANCE && this->rebalance_frequency == EACH_BEG_YEAR 
												&& (t == 1 || (int)(this->timing_items[t][POL_MONTH]) % (12) == 0 )))
		{
			double av_growth_t = 0.;
				
			for(int i = 0; i < index_map_size; i++)
			{
				index_num = index_map_2[i];
					
				av_growth_t += 
						(adjusted_rate(this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1, t, index_num - 1),1, 1) 
						- this->timing_items[t][PRORATA] 
						* adjusted_rate((model_point->gmxb_m_c/100.0), this->time_step, -1))  
						* this->index_weigths[t][index_num-1]/100.0;
			}

			main_proj_array[t][AV_GROWTH_RATE] = av_growth_t;
		}
		else if (this->model_point->av_rebal_ind == Model_Point::DRIFT ||  
				(this->model_point->av_rebal_ind == Model_Point::REBALANCE && this->rebalance_frequency == EACH_BEG_YEAR))
		{
			double sum_growth = 0.;
			for(int i = 0; i<index_map_size; i++)
			{
				index_num = index_map_2[i];	
				
				this->index_weigths[t][index_num - 1] = 
					(1.0 +  (adjusted_rate(this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1, t, index_num - 1),1, 1)
					- this->timing_items[t][PRORATA]
					* adjusted_rate(model_point->gmxb_m_c/100.0, this->time_step, -1)))
					* this->index_weigths[t-1][index_num-1];
				
				sum_growth += this->index_weigths[t][index_num - 1];
			}
			for(int i = 0; i<index_map_size; i++)
			{
				index_num = index_map_2[i];
				this->index_weigths[t][index_num-1] = 100 * this->index_weigths[t][index_num - 1]/sum_growth;
			}
			main_proj_array[t][AV_GROWTH_RATE] = sum_growth/100. - 1.;
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "The rebalancing indicator should be either REBALANCE or DRIFT");
			throw new GMXBException(message);
		}
		main_proj_array[t][AV_GROWTH] = main_proj_array[t][AV_B] 
									 * main_proj_array[t][AV_GROWTH_RATE];
	}


	virtual void avGrowth_annual(int t)
	{

		if (!this->model_point->load_model_point_funds)
		{
			int index_num;
			main_proj_array[t][AV_GROWTH]= 0.0;
			if (this->model_point->av_rebal_ind == Model_Point::REBALANCE)
			{
				double av_growth_t = 0.;
				for(int i = 0; i<index_map_size; i++)
				{
					index_num = index_map_2[i];
					av_growth_t += (
									this->afmr[this->model_point->elapsed_mths_in_valn_yr + 1][t][index_num-1]   
									- this->timing_items[t][PRORATA] 
									* (model_point->gmxb_m_c/100.0)) 
									* this->index_weigths[t][index_num-1]/100.0;
				}

				main_proj_array[t][AV_GROWTH_RATE] = av_growth_t;
			}
			else if (this->model_point->av_rebal_ind == Model_Point::DRIFT)
			{
				double sum_growth = 0.;
				for(int i = 0; i<index_map_size; i++)
				{
					index_num = index_map_2[i];	
					this->index_weigths[t][index_num-1] = (1.0 +  (this->afmr[this->model_point->elapsed_mths_in_valn_yr + 1][t][index_num-1]   
															- this->timing_items[t][PRORATA] 
																* model_point->gmxb_m_c/100.0))
																	* this->index_weigths[t-1][index_num-1];
					sum_growth += this->index_weigths[t][index_num-1];
				}
				for(int i = 0; i<index_map_size; i++)
				{
					index_num = index_map_2[i];
					this->index_weigths[t][index_num-1] = 100 * this->index_weigths[t][index_num-1]/sum_growth;
				}
				main_proj_array[t][AV_GROWTH_RATE] = sum_growth/100. - 1.;
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "The rebalancing indicator should be either REBALANCE or DRIFT");
				throw new GMXBException(message);
			}

		}
		else
		{
			for (unsigned int i = 0; i < this->model_point->list_of_funds.size(); i++)
			{
				// Compute the performance of the fund
				for (int j = 0; j < this->model_point->list_of_funds[i].fund.nb_indexes ; j++)
				{
					int index_num_split = this->model_point->list_of_funds[i].fund.position_in_beta_array[j];
					double beta_i_j = 0;
					if (t == 1)
					{
						beta_i_j = this->model_point->list_of_funds[i].fund.current_beta[index_num_split];
					}
					else
					{
						beta_i_j = this->model_point->list_of_funds[i].fund.target_beta[index_num_split];
					}

					this->model_point->list_of_funds[i].return_fund_t[t] += 
						(this->afmr[this->model_point->elapsed_mths_in_valn_yr + 1][t][index_num_split]   
						- this->timing_items[t][PRORATA] 
						* (this->model_point->list_of_funds[i].fund.get_percentage_fee_amount()/100.0))
						* beta_i_j;

				}
				
				if (t > 1)
				{
					// Compute the av fund weights
					if (this->model_point->av_rebal_ind == Model_Point::DRIFT)
					{
						// TO DO check if the denominator is nil
						this->model_point->list_of_funds[i].alpha_t[t-1]= this->model_point->list_of_funds[i].alpha_t[t-2]
									*( 1 + this->model_point->list_of_funds[i].return_fund_t[t-1]) / (1 + main_proj_array[t-1][AV_GROWTH_RATE]);
					}
					else
					{
						this->model_point->list_of_funds[i].alpha_t[t-1] = this->model_point->list_of_funds[i].target_weight;
					}
				}

				main_proj_array[t][AV_GROWTH_RATE] += this->model_point->list_of_funds[i].return_fund_t[t] 
										* this->model_point->list_of_funds[i].alpha_t[t-1] - this->model_point->fee_rp;
			}
		}

		main_proj_array[t][AV_GROWTH] = main_proj_array[t][AV_B] 
									 * main_proj_array[t][AV_GROWTH_RATE];	
	}

	void avEBef(int t)
	{
		this->avGrowth(t);	
		main_proj_array[t][AV_E_BEF]= max(main_proj_array[t][AV_B] + main_proj_array[t][AV_GROWTH], 0.0);
	}


	void annClaimsAv(int t)
	{
		main_proj_array[t][ANN_CLAIMS_AV]= main_proj_array[t][ANN_RATE] * main_proj_array[t][AV_E_BEF];
	}

	virtual double mortality_rate_calculation(int t)
	{
		int mort_table_high_ind = int(this->mort_table->get_row_high_index() - 1);
		int pol_yr = min(int(this->timing_items[t][POL_YEAR]), mort_table_high_ind);
		double table_age = (int)(this->timing_items[t][AGE_LAST]);	
		
		return min(1.0, 
				   this->mort_mult 
				   * this->mort_table->quick_search((int)table_age,1) 
					+ this->mort_adj 
				   );
	}

	double death_rate_calc(int t)
	{
		double death_rate=0.0;
		if ((int)(this->timing_items[t][POL_YEAR]) > this->term_to_omega +1)
			death_rate = 1.0;
		else 
			death_rate = this->timing_items[t][PRORATA] * mortality_rate_calculation(t);
		return death_rate;
	}


	void deathClaimsAv(int t)
	{
		main_proj_array[t][DEATH_CLAIMS_AV] = main_proj_array[t][DEATH_RATE] 
															* main_proj_array[t][AV_E_BEF];
	}
	virtual double dynamic_lapse_rate_axa(int t)
	{
		int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
		double lapse_rate_base = this->lapse_rates->search((int)pol_yr, 1);
		return max(
				   this->lapse_min_pa_perc/100,  
					min (this->lapse_max_pa_perc/100.0,  
					     lapse_rate_base / 100.
						 )
				   );
	}

	virtual double lapse_rate_calc(int t)
	{
		//Actuarial module
		if(!this->project_aging_policies)
		{
		double lapse_rate = 0.0;
		if (main_proj_array[t][AV_E_BEF]>= SMALL_DOUBLE)
		{
			if (this->lapse_is_dynamic)
			{
				lapse_rate = this->timing_items[t][PRORATA] * dynamic_lapse_rate_axa(t);
			}
			else
			{
				int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
				lapse_rate = this->timing_items[t][PRORATA] 
						* min (max(
									this->lapse_min_pa_perc/100, 
									this->lapse_rates->search((int)pol_yr,1)/100
								   ) , 
								   this->lapse_max_pa_perc/100
							   );
				lapse_rate = adjusted_rate(lapse_rate, time_step, -1);
				double short_rate_factor = this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1, t, MAXIMUM_NUMBER_OF_INDEXES) / 100.0;
				main_proj_array[t][BOND_RATE_FOR_LAPSES] = adjusted_rate(short_rate_factor, time_step, 1);

			}
		}
		if (this->longstaff_ind == "Y")
			lapse_rate = 0;
		return lapse_rate;
		}
		else
		{
			double lapse_rate = 0.0;
			if (main_proj_array[t][AV_E_BEF]>= SMALL_DOUBLE)
			{
				if (this->lapse_is_dynamic)
				{
					lapse_rate = this->timing_items[t][PRORATA] * dynamic_lapse_rate_axa(t);
				}
				else
				{
					int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
					lapse_rate = this->timing_items[t][PRORATA] 
							* min (max(
										this->lapse_min_pa_perc/100, 
										this->lapse_rates->search((int)pol_yr,1)/100
									   ) , 
									   this->lapse_max_pa_perc/100
								   );

					lapse_rate=adjusted_rate(lapse_rate,time_step,-1);
				}
			}
			
			return lapse_rate;
		
		}
	}

	void surrClaimsAv(int t)
	{
		main_proj_array[t][SURR_CLAIMS_AV] = main_proj_array[t][LAPSE_RATE] 
										 * (main_proj_array[t][AV_E_BEF] 
			                             - main_proj_array[t][DEATH_CLAIMS_AV]
												 ); 
	}

	void pdUpSurrChg(int t)
	{
		main_proj_array[t][PD_UP_SURR_CHG] = 0;
	}


	virtual void avEBefClaims(int t)
	{
		this->annClaimsAv(t);
		this->deathClaimsAv(t);
		this->surrClaimsAv(t);
		this->pdUpSurrChg(t);

		main_proj_array[t][AV_E_BEF_CLAIMS] = 
								main_proj_array[t][AV_E_BEF]
							  - main_proj_array[t][DEATH_CLAIMS_AV]
							  - main_proj_array[t][SURR_CLAIMS_AV] 
							  - main_proj_array[t][ANN_CLAIMS_AV] 
							  - main_proj_array[t][PD_UP_SURR_CHG];
	}

	virtual double surv_mth_calc(int t)
	{
		double surv_mth = (1.0 - main_proj_array[t][DEATH_RATE]) 
					* (1.0 - main_proj_array[t][LAPSE_RATE]) 
					* (1.0 - this->main_proj_array[t][ANN_RATE]);
		
		surv_cumul[t] = surv_cumul[t-1] * surv_mth;
		return surv_mth;
	}
	virtual void avE (int t)
	{
		main_proj_array[t][AV_E] = max(0.0, main_proj_array[t][AV_E_BEF_CLAIMS]);		
	}

	void finish_t(int t)
	{	
		main_proj_array[t][SURV_INDEX]  =main_proj_array[t-1][SURV_INDEX] * this->surv_mth_calc(t);
		if (this->write_main_proj_array) 
		{
			main_proj_array[t][PREMIUM] = this->regular_premium_calculation(t);
			main_proj_array[t][SURV_MTH] = this->surv_mth_calc(t);
			main_proj_array[t][DEATH_CLAIMS_AV] = main_proj_array[t][DEATH_CLAIMS_AV];
			if (!this->project_aging_policies)
				main_proj_array[t][DISCOUNT_FACTOR] = this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1, t, MAXIMUM_NUMBER_OF_INDEXES + 1);
		}
	}
	
public:

	/*
	* Set a new index shock array in the projection
	* Used for the Hedge effectiveness
	*/
	virtual void set_index_shock_array(const vector<vector <int>>& i_s_a)
	{
		//affich_shock.open("C:\\affich_shock.txt");
		// Deletes the old shock array
		for(int i = 0; i <this->index_shock_size; i++)
		{
			if(this->index_shock_array[i] != 0)
				delete[] this->index_shock_array[i];
		}
		if (this->index_shock_size > 0)
				delete[] this->index_shock_array;
		//affich_shock << "in function set " << "i_s_a.size() = " << i_s_a.size() << endl;
		//cout  << "in function set " << "i_s_a.size() = " << i_s_a.size() << endl;
		this->index_shock_size = i_s_a.size();
		if (this->index_shock_size > 0)
		{
			this->index_shock_array = new int* [this->index_shock_size];
			for(int i = 0; i < this->index_shock_size; i++)
			{
				this->index_shock_array[i] = new int [MAXIMUM_NUMBER_OF_INDEXES+1];
				for(int j = 0; j<MAXIMUM_NUMBER_OF_INDEXES+1; j++)
					this->index_shock_array[i][j] = i_s_a[i][j];
			}
		}
	}
	

		// set_fund_shock_array

	virtual void set_fund_shock_array(const vector<vector <int>>& f_s_a)
	{
		if (f_s_a.size() > 0)
		{
			int total_nb_funds = f_s_a[0].size();
			for(int i = 0; i <this->fund_shock_size; i++)
			{
				if(this->fund_shock_array[i] != 0)
					delete[] this->fund_shock_array[i];
			}
			if (this->fund_shock_size > 0)
					delete[] this->fund_shock_array;

			this->fund_shock_size = f_s_a.size();
			if (this->fund_shock_size > 0)
			{
				this->fund_shock_array = new int* [this->fund_shock_size];
				for(int i = 0; i < this->fund_shock_size; i++)
				{
					this->fund_shock_array[i] = new int [total_nb_funds+1];
					for(int j = 0; j<total_nb_funds+1; j++)
						this->fund_shock_array[i][j] = f_s_a[i][j];
				}
			}
		}
	}


	// Set t_high_shorter

	void set_t_high(int t_h)
	{
		this->t_high_shorter = t_h;
	}

	void set_longevity_table_indic(bool indic)
	{
		this->load_longevity_tables = indic;
	
	}

	/*
	* Calculate the death rate and the lapse rate 
	* and multiply these rates by the shock multiplicator if necessary (for lapse and mortality shocks)
	*/
	void init_rates(int t, double lapse_multiplicator, double mort_multiplicator)
	{
		if (!this->project_aging_policies)
		{
			if (this->time_step == ANNUAL)
			{

       			double death_rate = this->death_rate_calc(t);
				double lapse_rate = this->lapse_rate_calc(t);

				  main_proj_array[t][DEATH_RATE] = min(1.0,death_rate * mort_multiplicator);
				  main_proj_array[t][LAPSE_RATE] = min(1.0,lapse_rate * lapse_multiplicator);
			}
			else if (this->time_step == MONTHLY)
			{
				main_proj_array[t][DEATH_RATE] = adjusted_rate(this->death_rate_calc(t), this->time_step, -1) ;
			
				if (this->itm_frequency == EACH_T || this->timing_items[t][POL_YEAR]!= this->timing_items[t-1][POL_YEAR] || t == 1)
				{
					main_proj_array[t][LAPSE_RATE] = this->lapse_rate_calc(t);
				}
				else
				{
					main_proj_array[t][LAPSE_RATE] = main_proj_array[t-1][LAPSE_RATE];
					main_proj_array[t][ITM] = main_proj_array[t-1][ITM];
					double short_rate_factor = this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1, t ,MAXIMUM_NUMBER_OF_INDEXES) / 100.0;
					main_proj_array[t][BOND_RATE_FOR_LAPSES] = adjusted_rate(short_rate_factor, time_step, 1);
				}
				main_proj_array[t][DEATH_RATE] *=  mort_multiplicator;
				main_proj_array[t][LAPSE_RATE] *=  lapse_multiplicator;
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
				throw new GMXBException(message);	
			}
		}
		else
		{
			main_proj_array[t][DEATH_RATE] = adjusted_rate(this->death_rate_calc(t), this->time_step, -1) ;
			if (this->itm_frequency == EACH_T || this->timing_items[t][POL_YEAR]!=this->timing_items[t-1][POL_YEAR] || t==1){
				main_proj_array[t][LAPSE_RATE] =	this->lapse_rate_calc(t);
			}
			else
			{
				main_proj_array[t][LAPSE_RATE] =	main_proj_array[t-1][LAPSE_RATE];
				main_proj_array[t][ITM] = main_proj_array[t-1][ITM];
			}

			main_proj_array[t][DEATH_RATE] *=  this->mortality_mult_for_ap;
			main_proj_array[t][LAPSE_RATE] *=  this->lapse_mult_for_ap;
		}
	}

	virtual void populate_pvs(int t, double **pvs, int curr_shock_number)
	{
		if (!this->project_aging_policies)
		{
			double discount_factor = 0.;
			if (t == 0)
				discount_factor = 1.0;
			else	 
				discount_factor = this->get_rate(this->model_point->elapsed_mths_in_valn_yr + 1,t,MAXIMUM_NUMBER_OF_INDEXES + 1);  

			if (t==1)
				pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE] += 
						main_proj_array[1][AV_B_BEF];
			
			pvs[curr_shock_number][PV_GMAB] += 
									(main_proj_array[t][GMAB_CLAIMS] * discount_factor);

			pvs[curr_shock_number][PV_GMDB] += 
									(main_proj_array[t][GMDB_CLAIMS]* discount_factor);

			pvs[curr_shock_number][PV_GMWB] += 
									(main_proj_array[t][GMWB_CLAIMS] * discount_factor);

			pvs[curr_shock_number][PV_GMSB] += 
									(main_proj_array[t][GMSB_CLAIMS]* discount_factor);

			pvs[curr_shock_number][PV_GMIB] += (main_proj_array[t][GMIB_CLAIMS]* discount_factor);


			pvs[curr_shock_number][PV_GMXB_REAL_CHG] += 
									(main_proj_array[t][GMXB_CHARGE]* discount_factor);
			
			if (model_point->gmxb_g_c ==0)
			{
				pvs[curr_shock_number][PV_GMXB_EHC_CHG] = 0.;
			}
			else 
				pvs[curr_shock_number][PV_GMXB_EHC_CHG] = 
							(pvs[curr_shock_number][PV_GMXB_REAL_CHG] 
								* adjusted_rate(model_point->gmxb_ehc_charge/100.0, this->time_step,-1) / adjusted_rate(model_point->gmxb_g_c /100.0, this->time_step,-1)
							 );
		
			pvs[curr_shock_number][PV_DEP_FEE] += 
									(main_proj_array[t][HC_PREM]* discount_factor);

			this->main_proj_array[t][DISCOUNT_FACTOR] = discount_factor;
		}
	}

	virtual void project_t(int t, double **pvs, int curr_shock_nb, double lapse_mult, 
							double mortality_mult, bool shock_av) = 0;
	virtual void init_projection()
	{
		this->set_main_proj_arrays_to_zero();

		if (!this->model_point->load_model_point_funds)
		{
			// this->initialize_index_weights();
			this->update_index_weights();
		}
		else
		{
			this->model_point->init_fund_arrays();
		}
	}


	virtual void set_initial_value()
	{
		int index_num;
		for(int i = 0; i<index_map_size; i++)
		{
			index_num = index_map_2[i];	
			main_proj_array[0][AV_E]+= model_point->av_if_array[index_num-1];
		}
		surv_cumul[0] = 1.;
	}

	virtual void project()
	{
		//int maturity_year = this->maturity_yr;
		int maturity_year;
		if (time_step == ANNUAL)
			maturity_year=this->maturity_yr;
		else
			maturity_year=this->maturity_yr * this->time_step - this->model_point->elapsed_months;
		
		// Loop over the indexes shocks
		for(int i_sh_n = 0; i_sh_n < this->index_shock_size; i_sh_n++)
		{
			this->shock_fund = false;
			this->index_shock_number = i_sh_n;
			this->init_projection();
			this->set_initial_value();

			for (int t = 1; t <= maturity_year; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
						break;
				else
				{
					this->project_t(t, this->model_point->index_shock_pvs, this->index_shock_number, 1, 1, true);
				}
			}
			if(this->longstaff_ind=="Y"){
				this->fill_tables_LS(i_sh_n);
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}

		// Loop over the funds shocks
		for (int f_sh_n = 0; f_sh_n < this->fund_shock_size; f_sh_n++)
		{
			this->shock_fund = true;
			this->fund_shock_number = f_sh_n;
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= maturity_year; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
					break;
				else
				{
					this->project_t(t, this->model_point->fund_shock_pvs, this->fund_shock_number, 1, 1, true);
				}
			}

			if(false){this->fill_tables_LS(f_sh_n);}

			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}

		// Loop over the lapse shocks
		int lapse_shock_size = this->v_lapse_mult_table.size();
		for(int l = 0; l < lapse_shock_size; l++)
		{
			GMXB_Assumption_Table * m_lapse_tab = this->v_lapse_mult_table[l];
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= maturity_year; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
						break;
				else
				{
					//int pol_yr = (int)(this->timing_items[POL_YEAR][t]);
					//double lapse_mult = m_lapse_tab->search(int(pol_yr), 1)/100.;
					//this->project_t(t, this->model_point->pv_lapse, l, lapse_mult,1, false);

					if(t==1){
						this->project_t(t, this->model_point->pv_lapse, l, m_lapse_tab->search(t, 1)/100.,1, false);
					}
					else{
						this->project_t(t, this->model_point->pv_lapse, l, (1-this->timing_items[1][PRORATA]) * m_lapse_tab->search(t-1, 1)/100. + this->timing_items[1][PRORATA] * m_lapse_tab->search(t, 1)/100.,1, false);
					}
				}
			}

			if(false){this->fill_tables_LS(l);}

			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}
		
		// Loop over the mortality shocks
		int mortality_shock_size = this->v_mortality_mult_table.size();
		for(int l = 0; l < mortality_shock_size; l++)
		{
			GMXB_Assumption_Table * m_mortality_tab = this->v_mortality_mult_table[l];
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= maturity_year; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
						break;
				else
				{
					//int pol_yr = (int)(this->timing_items[POL_YEAR][t]);
					//double mortality_mult = m_mortality_tab->search(int(pol_yr), 1)/100.;
					//this->project_t(t, this->model_point->pv_mortality, l, 1, mortality_mult, false);

					if(t==1){
						this->project_t(t, this->model_point->pv_mortality, l, 1,m_mortality_tab->search(t, 1)/100., false);
					}
					else{
						this->project_t(t, this->model_point->pv_mortality, l, 1,(1-this->timing_items[1][PRORATA]) * m_mortality_tab->search(t-1, 1)/100. + this->timing_items[1][PRORATA] * m_mortality_tab->search(t, 1)/100., false);
					}
				}
			}

			if(false){this->fill_tables_LS(l);}

			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}

		// perform the longevity shock
		if (this->load_longevity_tables)
		{
			this->init_projection();
			this->set_initial_value();
			for (int t = 1; t <= maturity_year; t++) 
			{
				if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
						break;
				else
				{
					//int pol_yr = (int)(this->timing_items[t][POL_YEAR]);
					int table_age = (int)(this->timing_items[t][AGE_LAST]);
					double mortality_mult = this->longevity_shock_table->search(table_age, t /*pol_yr*/) ;
					this->project_t(t, this->model_point->pv_longevity, 0, 1, mortality_mult, false);
				}
			}
			if (this->generate_debug_files)
			{
				this->write_main_proj();
			}
		}
	}

	void set_aging_policies_indicator(bool indicator)
	{
		this->project_aging_policies = indicator;
	}


	/*
	* This function is used by the actuarial module
	* to age the policies by 1 month
	*/
	virtual void project_for_aging_policies()
	{

		this->index_shock_number = 0;
		this->init_projection();
		this->set_initial_value();

        int maturity_year = 2;
		for (int t = 1; t <= maturity_year; t++) 
		{
			if ( this->timing_items[t][POL_YEAR] > this->product_charging_duration) 
					break;
			else
			{
				this->project_t(t, this->model_point->index_shock_pvs, this->index_shock_number, 1, 1, true);
			}
		}	
		if (this->generate_debug_files)
		{
			this->write_main_proj();
		}
	}


	//! Y
	//! pvs is the usual pv, whitch is the sum of i scenPath
	void update_pvsScenOutput_sp(double **pvs, vector<vector<double>>& pvs_ScenOutput, vector<vector<double>>& pvs_ScenOutput_temp, int curr_shock_number)
	{      
        //! pvs_ScenOutput: t+1
		pvs_ScenOutput[curr_shock_number][BEGINING_ACCOUNT_VALUE] +=
			pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE]-pvs_ScenOutput_temp[curr_shock_number][BEGINING_ACCOUNT_VALUE];
		pvs_ScenOutput[curr_shock_number][PV_GMAB] += 
			pvs[curr_shock_number][PV_GMAB] -pvs_ScenOutput_temp[curr_shock_number][PV_GMAB];
		pvs_ScenOutput[curr_shock_number][PV_GMDB] += 
			pvs[curr_shock_number][PV_GMDB] -pvs_ScenOutput_temp[curr_shock_number][PV_GMDB];
		pvs_ScenOutput[curr_shock_number][PV_GMWB] += 
			pvs[curr_shock_number][PV_GMWB] -pvs_ScenOutput_temp[curr_shock_number][PV_GMWB];
		pvs_ScenOutput[curr_shock_number][PV_GMSB] += 
			pvs[curr_shock_number][PV_GMSB] -pvs_ScenOutput_temp[curr_shock_number][PV_GMSB];
		pvs_ScenOutput[curr_shock_number][PV_GMIB] += 
			pvs[curr_shock_number][PV_GMIB] -pvs_ScenOutput_temp[curr_shock_number][PV_GMIB];
		pvs_ScenOutput[curr_shock_number][PV_GMXB_REAL_CHG] += 
			pvs[curr_shock_number][PV_GMXB_REAL_CHG] - pvs_ScenOutput_temp[curr_shock_number][PV_GMXB_REAL_CHG];
		pvs_ScenOutput[curr_shock_number][PV_DEP_FEE] += 
			pvs[curr_shock_number][PV_DEP_FEE] - pvs_ScenOutput_temp[curr_shock_number][PV_DEP_FEE];
		pvs_ScenOutput[curr_shock_number][PV_GMXB_EHC_CHG] += 
			pvs[curr_shock_number][PV_GMXB_EHC_CHG] - pvs_ScenOutput_temp[curr_shock_number][PV_GMXB_EHC_CHG];

	    //! pvs_ScenOutput_temp: backup of time t
		pvs_ScenOutput_temp[curr_shock_number][BEGINING_ACCOUNT_VALUE] = pvs[curr_shock_number][BEGINING_ACCOUNT_VALUE];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMAB] = pvs[curr_shock_number][PV_GMAB];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMDB] = pvs[curr_shock_number][PV_GMDB];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMWB] = pvs[curr_shock_number][PV_GMWB];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMSB] = pvs[curr_shock_number][PV_GMSB];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMIB] = pvs[curr_shock_number][PV_GMIB];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMXB_REAL_CHG] = pvs[curr_shock_number][PV_GMXB_REAL_CHG];
		pvs_ScenOutput_temp[curr_shock_number][PV_DEP_FEE]       = pvs[curr_shock_number][PV_DEP_FEE];
		pvs_ScenOutput_temp[curr_shock_number][PV_GMXB_EHC_CHG]  = pvs[curr_shock_number][PV_GMXB_EHC_CHG];
		
		//???
		//this->main_proj_array[t][DISCOUNT_FACTOR] = discount_factor;
	}


	void update_pvsScenOutput(int nScenpathId_with_modulo, int nScen_total, int modulo_value)
	{
		    int d = nScen_total%modulo_value;
		    int nScenpathId = (nScenpathId_with_modulo-d)/modulo_value;

			if(nScenpathId == 0)  // for each mp, rinitilize all the variables to 0
			{   
				 initialized_pvstemp_to_Zero();
			}

            //! BEGIN
			for(int i_sh_n = 0; i_sh_n < this->index_shock_size; i_sh_n++)
			{
				update_pvsScenOutput_sp(this->model_point->index_shock_pvs,
					pvs_scenByscen.index_shock_pvs_scen_output[nScenpathId],
					pvs_scenByscen.index_shock_pvs_scen_output_temp,
					i_sh_n);
			}
				
			for (int f_sh_n = 0; f_sh_n < this->fund_shock_size; f_sh_n++)
			{
				update_pvsScenOutput_sp(this->model_point->fund_shock_pvs,
					pvs_scenByscen.fund_shock_pvs_scen_output[nScenpathId], 
					pvs_scenByscen.fund_shock_pvs_scen_output_temp, 
					f_sh_n);
			}

			for(int l = 0; l < this->v_lapse_mult_table.size(); l++)
			{
				update_pvsScenOutput_sp(this->model_point->pv_lapse,
					pvs_scenByscen.pv_lapse_scen_output[nScenpathId], 
					pvs_scenByscen.pv_lapse_scen_output_temp, 
					l);
			}
			
			for(int l = 0; l < this->v_mortality_mult_table.size(); l++)
			{
				update_pvsScenOutput_sp(this->model_point->pv_mortality,
					pvs_scenByscen.pv_mortality_scen_output[nScenpathId], 
					pvs_scenByscen.pv_mortality_scen_output_temp, 
					l);
			}

			// perform the longevity shock
			if (this->load_longevity_tables)
			{
					update_pvsScenOutput_sp(this->model_point->pv_longevity,
					pvs_scenByscen.pv_longevity_scen_output[nScenpathId], 
					pvs_scenByscen.pv_longevity_scen_output_temp, 
					0);
			}    
	}
virtual	void load_tables()
	{
		if(model_point->sex=="F")
		{
				this->mort_table = this->mort_table_f;
				this->mort_mult = this->mort_mult_f;
				this->mort_adj = this->mort_adj_f;
				if (this->load_longevity_tables)
					this->longevity_shock_table = longevity_shock_table_f;
		}
		else if(model_point->sex=="M")
		{
				this->mort_table = this->mort_table_m;
				this->mort_mult = this->mort_mult_m;
				this->mort_adj = this->mort_adj_m;
				if (this->load_longevity_tables)
					this->longevity_shock_table = longevity_shock_table_m;
		}
		else 
		{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Only F or M are allowed for model_point->sex variable");
				throw new GMXBException(message);		
		}
	}

	virtual void write_main_proj()
	{
		this->open_debug_file(this->model_point->mp_position_in_file,this->current_scenario_number);
		this->set_debug_file_header();
		if (this->time_step == ANNUAL)
		{
			for(int tt =0; tt <= this->maturity_yr; tt++)
			{
				this->display_debug_info(tt);
			}
			this->close_debug_file();
		}
		else
		{
			for(int tt =0; (this->timing_items[tt][POL_YEAR] <= this->maturity_yr && tt <= this->subacct_max_period); tt++)
			{
				this->display_debug_info(tt);
			}
			this->close_debug_file();
		}
	}

	double rounding_sup(double value, double epsilon)
	{
		return (ceil(value / epsilon))*epsilon; 
	}
	void set_is_he(bool value)
	{
		this->is_he = value;
	}
protected:
	virtual void open_debug_file(int mp, int scen)
	{
			char debug_file_name[MAX_LENGTH_FILE_NAME];
			sprintf(debug_file_name, "%smain_array_mp%d_scn%d.csv" ,this->debug_file_path.c_str(), mp, scen);
			this->debug_file.open(debug_file_name);
	}

	void close_debug_file()
	{
			this->debug_file.close();
	}
public:

	void init_longstaff_parameters(bool longstaff_indicator)
	{
		
		bool is_longstaff = longstaff_indicator;
		
		if (is_longstaff == true)
		{
			longstaff_ind = "Y";
			longstaff_method = this->assump->search("Pricing_Method");
			nb_parameters = atoi(this->assump->search("Number_parameters").c_str());
			ITM_choice_type_1 = this->assump->search("ITM_choice_1").c_str();
			ITM_choice_type_2 = this->assump->search("ITM_choice_2").c_str();
			ITM_choice_type_3 = this->assump->search("ITM_choice_3").c_str();
			longstaff_debug_info = this->assump->search("Longstaff_debug_info");
			longstaff_proba_info = this->assump->search("Longstaff_proba_info"); 
			ehc_for_longstaff = this->assump->search("EHC_for_Longstaff"); 
			length_smoothing = atof(this->assump->search("Smoothing").c_str());
			param_bar = atof(this->assump->search("Smoothing_bar").c_str());
			first_lapse_year = atoi(this->assump->search("First_lapse_year").c_str());
		}
		else
			longstaff_ind = "N";
	}
	
	virtual string get_longstaff_pricer(){
		return longstaff_ind;
	}
	virtual string get_longstaff_method(){
		return longstaff_method;
	}
	virtual string get_longstaff_debug_info(){
		return longstaff_debug_info;
	}
	virtual string get_longstaff_proba_info(){
		return longstaff_proba_info;
	}
	virtual string get_ehc_for_longstaff(){
		return ehc_for_longstaff;
	}
	virtual int get_end_projection(){
		return min(this->product_charging_duration_s()+1-int(this->model_point->duration),SUBACCT_MAX_PERIOD);
	}
	virtual double * get_mortality(int t){
		int mort_table_high_ind = int(this->mort_table->get_row_high_index() - 1);
		int pol_yr = min(int(this->timing_items[t][POL_YEAR]), mort_table_high_ind);//m
		double table_age = (int)(this->timing_items[t][AGE_LAST]);	//m
		double * survival= new double [t];
		survival[0]=1.;
		if(t<=pol_yr-table_age+mort_table_high_ind){//m
			for (int j=1; j<t; j++){
 				survival[j]=survival[j-1]*(1 - mortality_rate_calculation(j));
			}
		}//m
		else{																//m
			for(int j=pol_yr-table_age+mort_table_high_ind; j<t; j++)		//m
				survival[j] = 0;											//m
		}																	//m
		return survival;
	}
	virtual void initialise_LS(int number_of_scenarios){
		this->size_tables_LS(number_of_scenarios);
		this->create_tables_LS();
		this->set_smoothing_parameter();
	}
	
	virtual void size_tables_LS(int number_of_scenarios){
		s0 = this->index_shock_size;
		t0 = this->get_end_projection();
		
		if (t0<0)
			t0 = 0;
		n0 = number_of_scenarios;
		if(this->longstaff_method == "Boundary"){p0 = 2;}
		else{
			if(nb_parameters == 1){
				p0 = 3;
			}
			else if(nb_parameters == 2){
				p0 = 6;
			}
			else{
				p0 = 10;
			}
		}
	}
	virtual void create_tables_LS(){
		paths = new double** [s0];
		gmxb_charges = new double** [s0];
		abclaims = new double** [s0];
		dbclaims = new double** [s0];
		sbclaims = new double** [s0];
		ibclaims = new double** [s0];
		wbclaims = new double** [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){
			paths[sh_n] = new double* [t0];
			gmxb_charges[sh_n] = new double* [t0];
			abclaims[sh_n] = new double* [t0];
			dbclaims[sh_n] = new double* [t0];
			sbclaims[sh_n] = new double* [t0];
			ibclaims[sh_n] = new double* [t0];
			wbclaims[sh_n] = new double* [t0];
		}
		for(int sh_n=0; sh_n<s0; sh_n++){
			for(int i=0; i<t0; i++){
				paths[sh_n][i] = new double [n0];
				gmxb_charges[sh_n][i] = new double [n0];
				abclaims[sh_n][i] = new double [n0];
				dbclaims[sh_n][i] = new double [n0];
				sbclaims[sh_n][i] = new double [n0];
				ibclaims[sh_n][i] = new double [n0];
				wbclaims[sh_n][i] = new double [n0];
				for(int j=0; j<n0; j++){
					paths[sh_n][i][j] = 0.;
					gmxb_charges[sh_n][i][j] = 0.;
					abclaims[sh_n][i][j] = 0.;
					sbclaims[sh_n][i][j] = 0.;
					ibclaims[sh_n][i][j] = 0.;
					dbclaims[sh_n][i][j] = 0.;
					wbclaims[sh_n][i][j] = 0.;
				}
			}
		}
		explicative_variables = new double*** [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){
			explicative_variables[sh_n] = new double** [t0];
		}
		for(int sh_n=0; sh_n<s0; sh_n++){
			for(int i=0; i<t0; i++){
				explicative_variables[sh_n][i] = new double* [p0];
			}
		}
		for(int sh_n=0; sh_n<s0; sh_n++){
			for(int i=0; i<t0; i++){
				for(int j=0; j<p0; j++){
					explicative_variables[sh_n][i][j] = new double [n0];
					for(int k=0; k<n0; k++){
						explicative_variables[sh_n][i][j][k] = 0;
					}
				}
			}
		}
	
		proba_exercise = new double* [s0];

		for(int sh_n=0; sh_n<s0; sh_n++){
			proba_exercise[sh_n] = new double[t0];
		}
		
		proba_exercise_traj_by_traj = new double** [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){				// m
			proba_exercise_traj_by_traj[sh_n] = new double* [t0];
			for(int i=0; i<t0; i++){
				proba_exercise_traj_by_traj[sh_n][i] = new double [n0];
				for(int j=0; j<n0; j++){
					proba_exercise_traj_by_traj[sh_n][i][j] = 0;
				}
			}
		} // m: proba_exercice_LS initialization here
		proba_survival = new double** [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){
			proba_survival[sh_n] = new double* [t0];
			for(int i=0; i<t0; i++){
				proba_survival[sh_n][i] = new double [n0];
				for(int j=0; j<n0; j++){
					proba_survival[sh_n][i][j] = 0;
				}
			}
		}
		
		regression_parameters = new double** [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){
			
			if (t0==0)
			{
				regression_parameters[sh_n] = new double* [0];
			}
			else
			{
				regression_parameters[sh_n] = new double* [t0-1];
			}
			for(int i=0; i<t0-1; i++){
				regression_parameters[sh_n][i] = new double [p0];
			}
		}
		boundary_value = new double* [s0];
		for(int sh_n=0; sh_n<s0; sh_n++){
			
			if (t0==0)
			{
				boundary_value[sh_n] = new double [0];
			}
			else
			{
				boundary_value[sh_n] = new double [t0-1];
			}
			
		} 
	}

	virtual void set_smoothing_parameter(){
		this->length_smoothing_av = this->length_smoothing * this->model_point->total_av;
	}

	virtual void fill_tables_LS(int sh_n){
		double * survival;
		
		if (t0!=0)
			survival = this->get_mortality(t0);
		for(int i=0; i<t0; i++){
			ITM_for_LS ITM_object(i, this->model_point->prod_code_key, t0,
					this->ITM_choice_type_1, this->ITM_choice_type_2, this->ITM_choice_type_3,
					this->get_main_proj_array(AV_E, i), this->get_main_proj_array(GMAB_IF, i),
					this->get_main_proj_array(GMSB_IF, i), this->get_main_proj_array(GMWB_IF, i),
					this->get_main_proj_array(GMIB_IF, i), 0, 0, 0, this->afmr[this->model_point->elapsed_mths_in_valn_yr + 1][i][MAXIMUM_NUMBER_OF_INDEXES]/100.0,
					survival, this->model_point->gmxb_deferral_period, 0,0,0);

			explicative_variables[sh_n][i][0][current_scenario_number-1] = 1;
			explicative_variables[sh_n][i][1][current_scenario_number-1] = ITM_object.itm_calculation(1);
			
			if(p0>2){
				explicative_variables[sh_n][i][2][current_scenario_number-1] = explicative_variables[sh_n][i][1][current_scenario_number-1] * explicative_variables[sh_n][i][1][current_scenario_number-1];
			}
			if(p0>3){
				explicative_variables[sh_n][i][3][current_scenario_number-1] = ITM_object.itm_calculation(2);
				explicative_variables[sh_n][i][4][current_scenario_number-1] = explicative_variables[sh_n][i][3][current_scenario_number-1] * explicative_variables[sh_n][i][3][current_scenario_number-1];
				explicative_variables[sh_n][i][5][current_scenario_number-1] = explicative_variables[sh_n][i][1][current_scenario_number-1] * explicative_variables[sh_n][i][3][current_scenario_number-1];
			}
			if(p0>6){
				explicative_variables[sh_n][i][6][current_scenario_number-1] = ITM_object.itm_calculation(3);
				explicative_variables[sh_n][i][7][current_scenario_number-1] = explicative_variables[sh_n][i][6][current_scenario_number-1] * explicative_variables[sh_n][i][6][current_scenario_number-1];
				explicative_variables[sh_n][i][8][current_scenario_number-1] = explicative_variables[sh_n][i][1][current_scenario_number-1] * explicative_variables[sh_n][i][6][current_scenario_number-1];
				explicative_variables[sh_n][i][9][current_scenario_number-1] = explicative_variables[sh_n][i][3][current_scenario_number-1] * explicative_variables[sh_n][i][6][current_scenario_number-1];
			}
			
			
			gmxb_charges[sh_n][i][current_scenario_number-1] =  this->get_main_proj_array(GMXB_CHARGE, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			abclaims[sh_n][i][current_scenario_number-1] = this->get_main_proj_array(GMAB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			dbclaims[sh_n][i][current_scenario_number-1] = this->get_main_proj_array(GMDB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			ibclaims[sh_n][i][current_scenario_number-1] = this->get_main_proj_array(GMIB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			sbclaims[sh_n][i][current_scenario_number-1] = max(this->get_main_proj_array(GMSB_PREM_IF, i) - this->get_main_proj_array(AV_E,i),0) *this->get_main_proj_array(DISCOUNT_FACTOR, i);
			wbclaims[sh_n][i][current_scenario_number-1] = this->get_main_proj_array(GMWB_CLAIMS, i) *this->get_main_proj_array(DISCOUNT_FACTOR, i);
			paths[sh_n][i][current_scenario_number-1] += this->get_main_proj_array(GMDB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			paths[sh_n][i][current_scenario_number-1] += this->get_main_proj_array(GMAB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			paths[sh_n][i][current_scenario_number-1] += this->get_main_proj_array(GMIB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			paths[sh_n][i][current_scenario_number-1] += this->get_main_proj_array(GMWB_CLAIMS, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			if(this->get_ehc_for_longstaff()=="Y")
				paths[sh_n][i][current_scenario_number-1] -= (this->model_point->gmxb_ehc_charge/this->model_point->gmxb_g_c)* this->get_main_proj_array(GMXB_CHARGE, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
			else 
				paths[sh_n][i][current_scenario_number-1] -= this->get_main_proj_array(GMXB_CHARGE, i)*this->get_main_proj_array(DISCOUNT_FACTOR, i);
		}
	
		if (t0!=0)
			delete [] survival;
		for(int i=1; i<t0; i++){
			paths[sh_n][i][current_scenario_number-1] += paths[sh_n][i-1][current_scenario_number-1];
		}
		for (int i=1; i<t0; i++){
			if  (i<t0-1){
				paths[sh_n][i][current_scenario_number-1] += max(this->get_main_proj_array(GMSB_PREM_IF, i) - this->get_main_proj_array(AV_E, i) , 0) * this->get_main_proj_array(DISCOUNT_FACTOR, i);  
			}
			else{
				paths[sh_n][i][current_scenario_number-1] += max(this->get_main_proj_array(GMSB_PREM_IF, i) - this->get_main_proj_array(AV_E, i) - this->get_main_proj_array(GMAB_CLAIMS, i) , 0)* this->get_main_proj_array(DISCOUNT_FACTOR, i);	
			}		
		}
	}

	virtual void terminate_LS(int number){		
		for(int sh_n = 0; sh_n < s0; sh_n++){
			this->read(sh_n);
			this->calculate_date_exercise_LS(sh_n);
			this->calculate_survival(sh_n);
			this->calculate_proba_exercise_traj_by_traj(sh_n);
			this->fill_pv_LS(sh_n);
			this->write(sh_n, number);
		}
		this->delete_LS();
	}

	void write (int sh_n, int number){
		if(this->longstaff_debug_info == "Y" || this->longstaff_debug_info == "USE")
			write_proba(sh_n, number);
		if (this->longstaff_debug_info == "Y")
			write_debug(sh_n, number);
		if (this->longstaff_proba_info == "Y")
			write_proba_LS(sh_n, number);
	}

	void read (int sh_n) {
		if (this->longstaff_debug_info == "USE")
			read_debug(sh_n);
		if (this->longstaff_proba_info == "USE")
			read_proba_LS(sh_n);
		if ((this->longstaff_debug_info == "USE")&&(this->longstaff_proba_info == "USE")) {
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "You cannot read both exercise probabilities and frontier/regression parameters values" );
			throw new GMXBException(message);
		}
	}

	void write_debug(int sh_n, int number){
		if(this->longstaff_method == "Boundary" || this->longstaff_method == "LS+Boundary"){
			file_stock2<<(number+1)<<",";
			for (int i=0; i<t0-1; i++){
				file_stock2<<-boundary_value[sh_n][i]<<",";
			}
			file_stock2<<endl;
		}
		if(this->longstaff_method == "LS" || this->longstaff_method == "LS+Boundary"){
			for(int k=0; k<p0; k++){
				file_stock2<<(number+1)<<",";
				for (int i=0; i<t0-1; i++){
					file_stock2<<regression_parameters[sh_n][i][k]<<",";
				}
				file_stock2<<endl;
			}
			file_stock2<<endl;
		}
		
	}

	void write_proba (int sh_n, int number){	
			this->file_stock<<(number+1)<<",";		 
			for (int i=0; i<t0; i++){
				this->file_stock<<proba_exercise[sh_n][i]<<",";
			}
		this->file_stock<<endl;
	}

	void write_proba_LS (int sh_n, int number){
		for(int j=0; j<n0; j++){
			file_stock5<<(number+1)<<",";
			for (int i=0; i<t0-1; i++){
				file_stock5<<proba_exercise_traj_by_traj[sh_n][i][j]<<",";
			}
			file_stock5<<endl;
		}
		file_stock5<<endl;
	}
	
	void read_debug(int sh_n){
		if(this->longstaff_method == "Boundary" || this->longstaff_method == "LS+Boundary"){
			string row;
			double toto;
			getline(file_stock3,row);
			char* retPtr = new char[row.length()+1];
			strcpy(retPtr, row.c_str());
			toto = -atof(strtok(retPtr, ","));
			for (int i=0; i<t0-1; i++){
				boundary_value[sh_n][i] = -atof(strtok(NULL, ","));
			}
			delete [] retPtr;
		}
		if(this->longstaff_method == "LS" || this->longstaff_method == "LS+Boundary"){
			string row;
			double toto;
			for(int k=0; k<p0; k++){
				getline(file_stock3,row);
				char* retPtr = new char[row.length()+1];
				strcpy(retPtr, row.c_str());
				toto = atof(strtok(retPtr, ","));
				for (int i=0; i<t0-1; i++){
					regression_parameters[sh_n][i][k] = atof(strtok(NULL, ",")); 
				}
				delete [] retPtr;
			}
			getline(file_stock3,row);
		}
	}

	void read_proba_LS(int sh_n) {
		string row;
		double toto;
		for (int j=0; j<n0; j++){
			getline(file_stock4,row);
			char* retPtr = new char[row.length()+1];
			strcpy(retPtr, row.c_str());
			toto = atof(strtok(retPtr, ","));
			for (int i=0; i<t0-1; i++){
				proba_exercise_traj_by_traj[sh_n][i][j] = atof(strtok(NULL, ","));
			}
			delete [] retPtr;
		}
		getline(file_stock4, row);
	}

	virtual void calculate_date_exercise_LS(int sh_n){
		LS Longstaff(t0, n0, p0, first_lapse_year, param_bar, length_smoothing, length_smoothing_av, paths[sh_n], explicative_variables[sh_n], this->get_longstaff_method(), longstaff_debug_info, longstaff_proba_info, regression_parameters[sh_n], boundary_value[sh_n], proba_exercise_traj_by_traj[sh_n]);		
		Longstaff.executeLS();
		proba_exercise_traj_by_traj[sh_n] = Longstaff.proba_exercise_traj_by_traj();

	}

	virtual void calculate_survival(int sh_n){
		
		if (t0!=0)
		{
			for(int j=0; j<n0; j++){
				proba_survival[sh_n][0][j] = 1 - proba_exercise_traj_by_traj[sh_n][0][j] ;
			}
		}
		for(int i=1; i<t0; i++){
			for(int j=0; j<n0; j++){
				proba_survival[sh_n][i][j] = proba_survival[sh_n][i-1][j] * (1 - proba_exercise_traj_by_traj[sh_n][i][j]);
			}
		}
	}

	virtual void calculate_proba_exercise_traj_by_traj(int sh_n){
		for(int i=0; i<t0; i++){
			proba_exercise[sh_n][i] = 0;
		}
		
		if (t0!=0)
		{
			for(int j=0; j<n0; j++){
				proba_exercise[sh_n][0] += proba_exercise_traj_by_traj[sh_n][0][j];
			}
			for(int j=0; j<n0; j++){
				for(int i=1; i<t0; i++){
					proba_exercise[sh_n][i] += proba_survival[sh_n][i-1][j] * proba_exercise_traj_by_traj[sh_n][i][j];
				}
			}
		}
		for(int i=0; i<t0; i++){
			proba_exercise[sh_n][i] = proba_exercise[sh_n][i] / n0;
		}
		double p=0.0;
		for(int i=0; i<t0; i++){
			p += proba_exercise[sh_n][i];
		}
		
		if (t0!=0)
			proba_exercise[sh_n][t0-1]=1-p;
	}

	virtual void fill_pv_LS(int sh_n){
		this->model_point->index_shock_pvs[sh_n][PV_GMAB] = 0;
		this->model_point->index_shock_pvs[sh_n][PV_GMDB] = 0;
		this->model_point->index_shock_pvs[sh_n][PV_GMWB] = 0;
		this->model_point->index_shock_pvs[sh_n][PV_GMIB] = 0;
		this->model_point->index_shock_pvs[sh_n][PV_GMSB] = 0;
		this->model_point->index_shock_pvs[sh_n][PV_GMXB_REAL_CHG] = 0;
		for(int scen_nb=0; scen_nb < n0 ; scen_nb++){					
			for (int i=1; i<t0; i++){
				this->model_point->index_shock_pvs[sh_n][PV_GMAB] += abclaims[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
				this->model_point->index_shock_pvs[sh_n][PV_GMDB] += dbclaims[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
				this->model_point->index_shock_pvs[sh_n][PV_GMWB] += wbclaims[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
				this->model_point->index_shock_pvs[sh_n][PV_GMIB] += ibclaims[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
				this->model_point->index_shock_pvs[sh_n][PV_GMXB_REAL_CHG] += gmxb_charges[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
				this->model_point->index_shock_pvs[sh_n][PV_GMSB] += sbclaims[sh_n][i][scen_nb] * proba_exercise_traj_by_traj[sh_n][i][scen_nb] * proba_survival[sh_n][i-1][scen_nb];
			}
		}
		if (model_point->gmxb_g_c ==0){
			this->model_point->index_shock_pvs[sh_n][PV_GMXB_EHC_CHG] = 0.;
		}
		else{
			this->model_point->index_shock_pvs[sh_n][PV_GMXB_EHC_CHG] = 
				(this->model_point->index_shock_pvs[sh_n][PV_GMXB_REAL_CHG] 
					* model_point->gmxb_ehc_charge / model_point->gmxb_g_c);
		}
	}

	virtual void delete_LS(){
		
		if(this->paths!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] paths[sh_n][i];
				}
				delete[] paths[sh_n];
			}
			delete[] paths;
		}

		if(this->explicative_variables!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					for(int j=0; j<p0; j++){
						delete[] explicative_variables[sh_n][i][j];
					}
					delete[] explicative_variables[sh_n][i];
				}
				delete[] explicative_variables[sh_n];
			}
			delete[] explicative_variables;
		}
		
		if(this->abclaims!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] abclaims[sh_n][i];
				}
				delete[] abclaims[sh_n];
			}
			delete[] abclaims;
		}
		
		if(this->dbclaims!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] dbclaims[sh_n][i];
				}
				delete[] dbclaims[sh_n];
			}
			delete[] dbclaims;
		}

		if(this->sbclaims!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] sbclaims[sh_n][i];
				}
				delete[] sbclaims[sh_n];
			}
			delete[] sbclaims;
		}

		if(this->ibclaims!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] ibclaims[sh_n][i];
				}
				delete[] ibclaims[sh_n];
			}
			delete[] ibclaims;
		}

		if(this->wbclaims!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] wbclaims[sh_n][i];
				}
				delete[] wbclaims[sh_n];
			}
			delete[] wbclaims;
		}

		if(this->gmxb_charges!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] gmxb_charges[sh_n][i];
				}
				delete[] gmxb_charges[sh_n];
			}
			delete[] gmxb_charges;
		}

		if(this->proba_exercise!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				delete[] proba_exercise[sh_n];
			}
			delete[] proba_exercise;
		}
		
		if(this->proba_exercise_traj_by_traj!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] proba_exercise_traj_by_traj[sh_n][i];
				}
				delete[] proba_exercise_traj_by_traj[sh_n];
			}
			delete[] proba_exercise_traj_by_traj;
		}

		if(this->proba_survival!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0; i++){
					delete[] proba_survival[sh_n][i];
				}
				delete[] proba_survival[sh_n];
			}
			delete[] proba_survival;
		}
		
		if(this->regression_parameters!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				for(int i=0; i<t0-1; i++){
					delete[] regression_parameters[sh_n][i];
				}
				delete[] regression_parameters[sh_n];
			}
			delete[] regression_parameters;
		}

		if(this->boundary_value!=0){
			for(int sh_n=0; sh_n<s0; sh_n++){
				delete[] boundary_value[sh_n];
			}
			delete[] boundary_value;
		}
		
	}
};

int Projection::compt_sen=0;
