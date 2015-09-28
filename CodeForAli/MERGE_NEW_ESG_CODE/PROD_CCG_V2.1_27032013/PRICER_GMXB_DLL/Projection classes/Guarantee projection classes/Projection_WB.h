#pragma once
#include "Projection classes/Guarantee projection classes/Projection.h"


#define NO_RATCHET 0
#define DEF_RATCHET_YR 1
#define PAYOUT_RATCHET_YR 2

class Projection_WB : virtual public Projection {
	
public :

	GMXBParameters * assump_WB;
	string gmwb_ind;
	string gmwb_ratchet_area;
	string gmwb_rollup_type;
	string gmwb_ratchet_type;
	string gmwb_timing;
	double gmwb_max_bonus_term;
	double gmwb_max_annual_wdl_pct;
	double gmwb_vested_base;
	double gmwb_withdrawal_period;
	string gmwb_ratchet_timing;
	bool apply_ratchet; 
	int ratchet_period_indicator;
	int withdrawal_prorata;
	int policy_wdlw_year;

public:
	Projection_WB(const vector<vector <int>>& i_s_a,const vector<vector <int>>& f_s_a,
				  const char* tables, bool generate_dbg_files, const string& dbg_file_path,
				  const int t_high_shorter, bool l_longevity_tables, int time_step):
		Projection (i_s_a, f_s_a, tables, generate_dbg_files, dbg_file_path,t_high_shorter, l_longevity_tables, time_step)
	{
		assump_WB = new GMXBParameters(tables, "assump_WB");
		gmwb_ind = this->assump_WB->search("gmwb_ind");
		gmwb_ratchet_area = this->assump_WB->search("gmwb_ratchet_area");	
		gmwb_rollup_type = this->assump_WB->search("gmwb_rollup_type");
		gmwb_ratchet_type = this->assump_WB->search("gmwb_ratchet_type");
		gmwb_timing = this->assump_WB->search("gmwb_timing");
		gmwb_max_bonus_term = atof(this->assump_WB->search("gmwb_max_bonus_term").c_str());
		gmwb_max_annual_wdl_pct = atof(this->assump_WB->search("gmwb_max_annual_wdl_pct").c_str());
		gmwb_withdrawal_period = atof(this->assump_WB->search("gmwb_withdrawal_period").c_str());
		gmwb_ratchet_timing = this->assump_WB->search("gmwb_ratchet_timing").c_str();
		policy_wdlw_year = atoi(this->assump_WB->search("policy_wdlw_year").c_str());
		withdrawal_prorata = atoi(this->assump_WB->search("withdrawal_prorata").c_str());
	}

public:

	virtual ~Projection_WB( void )
	{
		delete this->assump_WB;
	}

   	int product_charging_duration_s()
	{
		if (this->time_step == ANNUAL)
			return int(model_point->gmxb_deferral_period) + int(this->gmwb_withdrawal_period);	
		else if (this->time_step == MONTHLY)
			return T_Max_Monthly;	
		else
			return (int(model_point->gmxb_deferral_period) + int(this->gmwb_withdrawal_period))*this->time_step;	
	}

	void avBBefBis(int t)
	{
		main_proj_array[t][AV_B_BEF_BIS] = 0.0;
		main_proj_array[t][GMWB_PAYMENT_FROM_AV] = 0.0;
		if (this->gmwb_timing == "BEG")
		{
			double current_gmwb_payment;

			if (this->withdrawal_prorata == 1)
				current_gmwb_payment = this->timing_items[t][PRORATA] * main_proj_array[t][GMWB_PAYMENT];
			else
				current_gmwb_payment = main_proj_array[t][GMWB_PAYMENT];
			
			// Option to be deleted for the release V13.0
			if (this->is_he == true)
				if (t == 1)
				{
					current_gmwb_payment = main_proj_array[t][GMWB_PAYMENT];
				}

			main_proj_array[t][AV_B_BEF_BIS] = 
											 max(0.0, main_proj_array[t][AV_B_BEF]- current_gmwb_payment);
			main_proj_array[t][GMWB_PAYMENT_FROM_AV]= 
											 min(current_gmwb_payment, main_proj_array[t][AV_B_BEF]);
		}
		else
		{
			Projection::avBBefBis(t);
		}		
	}


	void avE (int t)
	{
		if ((this->gmwb_timing == "END") && 
					(main_proj_array[t][AV_E_BEF_CLAIMS] >= SMALL_DOUBLE))
		{
			double current_gmwb_payment;

			if (this->withdrawal_prorata == 1)
			{
				current_gmwb_payment = this->timing_items[t][PRORATA] * main_proj_array[t][GMWB_PAYMENT]; 
			}
			else
			{
				current_gmwb_payment = main_proj_array[t][GMWB_PAYMENT]; 
			}

			// Option to be deleted for the release V13.0
			if (this->is_he == true)
				if (t == 1)
				{
					current_gmwb_payment = main_proj_array[t][GMWB_PAYMENT];
				}

			main_proj_array[t][AV_E]= max(0.0, main_proj_array[t][AV_E_BEF_CLAIMS]- current_gmwb_payment);
		}
		else
				Projection::avE(t);
	}

	virtual void project_gmwb_payments_claims_beg(int t)
	{
		double wdl_rate = this->gmwb_max_annual_wdl_pct;
		if (this->payment_fees_frequency == EACH_BEG_YEAR)
			wdl_rate = wdl_rate / 100.0;
		else
			wdl_rate = adjusted_rate(wdl_rate/ 100.0,time_step,0);
	
		if ((this->gmwb_timing == "BEG")&& ((this->payment_fees_frequency == EACH_BEG_YEAR) && 
			((int)this->timing_items[t][POL_MONTH] % 12 ==0) || this->payment_fees_frequency==EACH_T))
		{
			if (t == 1) 
			{
				main_proj_array[t][GMWB_ROLLUP_PREV] = model_point->gmwb_rollup_valn;     
				main_proj_array[t][GMWB_RATCHET_PREV] = model_point->gmwb_ratchet_valn;
				main_proj_array[t][GMWB_IF] = max(model_point->gmwb_rollup_valn, model_point->gmwb_ratchet_valn);
			}
			else
			{      
				main_proj_array[t][GMWB_RATCHET_PREV] = main_proj_array[t-1][GMWB_AV_IF];

				if (this->gmwb_ratchet_type == "RATCHET")
					main_proj_array[t][GMWB_ROLLUP_PREV] = main_proj_array[t-1][GMWB_PREM_IF];
				else
				{
					main_proj_array[t][GMWB_ROLLUP_PREV]  = main_proj_array[t-1][GMWB_IF] ; 	
				}
			}	

			if ((this->timing_items[t][POL_YEAR]> model_point->gmxb_deferral_period)
				&&(this->timing_items[t][POL_YEAR] <= model_point->gmxb_deferral_period + this->gmwb_withdrawal_period))
			{
				if (t == 1)
				{
					main_proj_array[t][GMWB_PAYMENT] = wdl_rate * main_proj_array[t][GMWB_IF];
					
					// Option to be deleted for the release V13.0
					if (this->is_he == true)
					{
						if (this->withdrawal_prorata == 1)
							main_proj_array[t][GMWB_PAYMENT] = wdl_rate * main_proj_array[t][GMWB_IF] * timing_items[t][PRORATA];
						else
							main_proj_array[t][GMWB_PAYMENT]= max(wdl_rate * main_proj_array[t][GMWB_IF] - model_point->gmwb_payt_yr, 0.0);
					}
				}
				else
					main_proj_array[t][GMWB_PAYMENT] = wdl_rate * main_proj_array[t-1][GMWB_IF];
				
				main_proj_array[t][GMWB_CLAIMS] = 
							max(0.0, main_proj_array[t][GMWB_PAYMENT]  - main_proj_array[t][AV_B_BEF]);
			}
			else
			{
				main_proj_array[t][GMWB_PAYMENT] =  0.0;
				main_proj_array[t][GMWB_CLAIMS] = 0.0;
			}
		}
	}

	virtual void gmwb_specific_end_claims_calculation(int t)
	{

		this->main_proj_array[t][GMWB_PAYMENT] = this->gmwb_max_annual_wdl_pct/ 100. 
																* this->main_proj_array[t][GMWB_IF];
		this->main_proj_array[t][GMWB_CLAIMS] = max(0.0, this->main_proj_array[t][GMWB_PAYMENT] - 
																	this->main_proj_array[t][AV_E_BEF_CLAIMS]);		
	}

	void project_gmwb_payments_claims_end(int t)
	{
		if ((this->timing_items[t][POL_YEAR] > int(this->model_point->gmxb_deferral_period))
			&&(this->timing_items[t][POL_YEAR] <= int(this->model_point->gmxb_deferral_period) + this->gmwb_withdrawal_period))
			{
				gmwb_specific_end_claims_calculation(t);
			}
		else
   			{
   				this->main_proj_array[t][GMWB_PAYMENT] =  0.0;
				this->main_proj_array[t][GMWB_CLAIMS] = 0.0;
			}
	}

	void set_current_model_point(Model_Point *mp)
	{
		Projection::set_current_model_point(mp);
		
		if ((this->gmwb_ratchet_type == "RATCHET") && (this->model_point->gmwb_ratchet_period != 0))
			this->apply_ratchet = true;
		else
			this->apply_ratchet = false;

		this->ratchet_period_indicator = NO_RATCHET;
	}

	void set_current_model_point (Projection* proj_out, int t, string* inforce, 
					int index_shock_size_in, int fund_shock_size_in)
	{
		Projection::set_current_model_point (proj_out, t, inforce, index_shock_size_in, fund_shock_size_in);
		if ((this->gmwb_ratchet_type == "RATCHET") && (this->model_point->gmwb_ratchet_period != 0))
			this->apply_ratchet = true;
		else
			this->apply_ratchet = false;

		this->ratchet_period_indicator = NO_RATCHET;
	}
	//indicates if we are in the right period
	//First, this function checks if the policy year is the year when we apply the ratchet
	//returns DEF_RATCHET_YR if the policy year is lower than the deferral period
	//returns PAYOUT_RATCHET_YR if the policy year is greater than the deferral period
	//returns NO_RATCHET if the conditions above are disrespected

	virtual int get_ratchet_indicator(int t)
	{
		int ratchet_indicator = NO_RATCHET;

		if (time_step == ANNUAL){

			if (this->model_point->gmwb_ratchet_period != 0.)
			{
				if(((this->gmwb_ratchet_area =="Contract_duration")
						||( (this->gmwb_ratchet_area =="Deferral_period") 
						&& (this->timing_items[t][POL_YEAR] <= int(model_point->gmxb_deferral_period))))
						&& ((int)this->timing_items[t][POL_YEAR] % (int)model_point->gmwb_ratchet_period == 0))	
						
						ratchet_indicator = DEF_RATCHET_YR;

				else if((this->gmwb_ratchet_area =="Payout_period") 
						&& (this->timing_items[t][POL_YEAR] >= int(model_point->gmxb_deferral_period))
						&& ((int(this->timing_items[t][POL_YEAR]) - int(model_point->gmxb_deferral_period)) % (int)model_point->gmwb_ratchet_period) == 0)
						
						ratchet_indicator = PAYOUT_RATCHET_YR;
			}
		}

		else{

			if (this->model_point->gmwb_ratchet_period != 0.)
			{
				if(((this->gmwb_ratchet_area =="Contract_duration")
						||( (this->gmwb_ratchet_area =="Deferral_period") 
						&& ((int)this->timing_items[t][POL_MONTH] <= int(model_point->gmxb_deferral_period)*12)))
						&& ((int)this->timing_items[t][POL_MONTH] % (((int)model_point->gmwb_ratchet_period)*12) == 0))	
						
						ratchet_indicator = DEF_RATCHET_YR;

				else if((this->gmwb_ratchet_area =="Payout_period") 
						&& ((int)this->timing_items[t][POL_MONTH] >= int(model_point->gmxb_deferral_period)*12)
						&& (((int)this->timing_items[t][POL_MONTH] - int(model_point->gmxb_deferral_period)*12) % (int)model_point->gmwb_ratchet_period*12) == 0)
						
						ratchet_indicator = PAYOUT_RATCHET_YR;
			}
		}

		return ratchet_indicator;

	}

	virtual void init_bases (int t)
	{
		if (t == 1) 
		{
			main_proj_array[t][GMWB_ROLLUP_PREV] = this->model_point->gmwb_rollup_valn;     
			main_proj_array[t][GMWB_RATCHET_PREV] = this->model_point->gmwb_ratchet_valn;
		}
		else
		{      
			main_proj_array[t][GMWB_RATCHET_PREV] = this->main_proj_array[t-1][GMWB_AV_IF];
			main_proj_array[t][GMWB_ROLLUP_PREV] = this->main_proj_array[t-1][GMWB_PREM_IF];	
		}

	}

	void calculate_rollup_base(int t)
	{ 
		double gmwb_vested_base = 0.0;  

		double gmwb_rollup_rate = 0.;

		if (this->time_step == ANNUAL)
		{
			gmwb_rollup_rate = model_point->gmwb_rollup_rate / 100;
		}
		else if (this->time_step == MONTHLY)
		{
			// Actuarial tool
			if (this->project_aging_policies)
			{
				if (this->timing_items[t][POL_YEAR] == int(this->model_point->duration) + 1)
				{
					int time_to_anniversary = time_step - this->model_point->elapsed_mths_in_valn_yr;
					gmwb_rollup_rate = adjusted_rate(model_point->gmwb_rollup_rate / 100.0, time_to_anniversary, 1);	
				}
				else
					gmwb_rollup_rate = adjusted_rate(model_point->gmwb_rollup_rate / 100.0, time_step, 1);

				if (this->rollup_frequency==EACH_BEG_YEAR && t==1 && (ceil(this->duration_age_step + this->model_point->duration) == ceil(this->model_point->duration) + 1) )
					gmwb_rollup_rate = model_point->gmwb_rollup_rate / 100.0;
				else if (this->rollup_frequency==EACH_BEG_YEAR)
					gmwb_rollup_rate = 0;
			
				if (this->rollup_frequency==EACH_END_YEAR && t==1 && (int(this->duration_age_step + this->model_point->duration) == int(this->model_point->duration) + 1) )
					gmwb_rollup_rate = model_point->gmwb_rollup_rate / 100.0;
				else if (this->rollup_frequency==EACH_END_YEAR)
					gmwb_rollup_rate = 0;
			}
			else
			{
			if (this->timing_items[t][POL_YEAR] == int(this->model_point->duration) + 1)
			{
				int time_to_anniversary = time_step - this->model_point->elapsed_mths_in_valn_yr;
				gmwb_rollup_rate = adjusted_rate(model_point->gmwb_rollup_rate / 100.0, time_to_anniversary, 1);	
			}
			else
				gmwb_rollup_rate = adjusted_rate(model_point->gmwb_rollup_rate / 100.0, time_step, 1);
		
			if (this->rollup_frequency==EACH_BEG_YEAR && ((int)this->timing_items[t][POL_MONTH] % 12 == 1) )
				gmwb_rollup_rate = model_point->gmwb_rollup_rate / 100.0;
			else if (this->rollup_frequency==EACH_BEG_YEAR)
				gmwb_rollup_rate = 0;
		
			if (this->rollup_frequency==EACH_END_YEAR && ((int)this->timing_items[t][POL_MONTH] % 12 == 0) )
				gmwb_rollup_rate = model_point->gmwb_rollup_rate / 100.0;
			else if (this->rollup_frequency==EACH_END_YEAR)
				gmwb_rollup_rate = 0;
			}
		
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
			throw new GMXBException(message);
		}
	
		if (this->gmwb_rollup_type == "LINEAR") 
			gmwb_vested_base = (model_point->prem_paid + main_proj_array[t][PREMIUM]) * surv_cumul[t-1];
		else if (this->gmwb_rollup_type == "COMPOUND")
			gmwb_vested_base = main_proj_array[t][GMWB_ROLLUP_PREV] + main_proj_array[t][PREMIUM] * surv_cumul[t-1];

		if (this->timing_items[t][POL_YEAR] <= min(model_point->gmxb_deferral_period, this->gmwb_max_bonus_term)) 
		{
			main_proj_array[t][GMWB_PREM_IF_BEF] = main_proj_array[t][GMWB_ROLLUP_PREV] + main_proj_array[t][PREMIUM] + gmwb_vested_base * gmwb_rollup_rate;
			/*if (t == 8 || t == 9 || t == 10) 
			{
				cout << "t " << t << " premium " << main_proj_array[t][PREMIUM] << " vested base " << gmwb_vested_base << " rollup rate " << gmwb_rollup_rate << endl;
				getchar();
			}*/
		}
		else
			main_proj_array[t][GMWB_PREM_IF_BEF] = main_proj_array[t][GMWB_ROLLUP_PREV] + main_proj_array[t][PREMIUM];

	}

	virtual void calculate_ratchet_base_begining_period(int t)
	{
		double surv_month = this->surv_mth_calc(t);

		if (ratchet_period_indicator == DEF_RATCHET_YR || ratchet_period_indicator == PAYOUT_RATCHET_YR)
		{
			if (this->time_step == ANNUAL)
			{
				main_proj_array[t][GMWB_AV_IF_BEF] = max(main_proj_array[t][AV_E_BEF], main_proj_array[t][GMWB_RATCHET_PREV] + main_proj_array[t][PREMIUM]);
			}
			else if (this->time_step == MONTHLY)
			{
				if ((int)this->timing_items[t][POL_MONTH] % 12 == 0)
				{
					main_proj_array[t][GMWB_AV_IF_BEF] = max(main_proj_array[t][AV_E_BEF], main_proj_array[t][GMWB_RATCHET_PREV] + main_proj_array[t][PREMIUM]);
				}
				else 
					main_proj_array[t][GMWB_AV_IF_BEF] = main_proj_array[t][GMWB_RATCHET_PREV] + main_proj_array[t][PREMIUM];
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Allowed values for the projection time step should either ANNUAL or MONTHLY");
				throw new GMXBException(message);
			}
		}
		else 
				main_proj_array[t][GMWB_AV_IF_BEF] = main_proj_array[t][GMWB_RATCHET_PREV] + main_proj_array[t][PREMIUM];
	}

	virtual void calculate_ratchet_base_end_period(int t)
	{
		double surv_month = this->surv_mth_calc(t);

		if (ratchet_period_indicator == DEF_RATCHET_YR)	   
			main_proj_array[t][GMWB_AV_IF] = max(main_proj_array[t][AV_E_BEF_CLAIMS], main_proj_array[t][GMWB_AV_IF]);		   
		else if (ratchet_period_indicator == PAYOUT_RATCHET_YR)
		{
			if (this->withdrawal_prorata == 1)
			{
				main_proj_array[t][GMWB_AV_IF] = max(main_proj_array[t][AV_E_BEF_CLAIMS] - this->timing_items[t][PRORATA]
					* this->main_proj_array[t][GMWB_PAYMENT],main_proj_array[t][GMWB_AV_IF]);
			}
			else
			{
				main_proj_array[t][GMWB_AV_IF] = max(main_proj_array[t][AV_E_BEF_CLAIMS] -  this->main_proj_array[t][GMWB_PAYMENT],
								main_proj_array[t][GMWB_AV_IF]);
			}
		}

		this->main_proj_array[t][GMWB_IF] = max(main_proj_array[t][GMWB_AV_IF], main_proj_array[t][GMWB_PREM_IF]);

	}

	void calculate_ratchet_base_and_claims(int t)
	{	

			if (this->gmwb_ratchet_timing == "BEG")
			{
				this->calculate_ratchet_base_begining_period(t);
				this->calculate_gmwb_payments(t);
				if (this->gmwb_timing == "END")
					project_gmwb_payments_claims_end(t);

			}
			else if (this->gmwb_ratchet_timing == "END")
			{				
				main_proj_array[t][GMWB_AV_IF_BEF] = main_proj_array[t][GMWB_RATCHET_PREV];
				this->calculate_gmwb_payments(t);
				if (this->gmwb_timing == "END")
					project_gmwb_payments_claims_end(t);
				this->calculate_ratchet_base_end_period(t);
			}
			else 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Projection_WB : gmwb_ratchet_timing should be either BEG or END ");
				throw new GMXBException(message);	
			}
	}

	void calculate_gmwb_payments(int t)
	{
		double surv_month = this->surv_mth_calc(t); 
		main_proj_array[t][GMWB_IF_BEF] = max(main_proj_array[t][GMWB_AV_IF_BEF], main_proj_array[t][GMWB_PREM_IF_BEF]);
		main_proj_array[t][GMWB_AV_IF] = main_proj_array[t][GMWB_AV_IF_BEF] * surv_month; 
		main_proj_array[t][GMWB_PREM_IF] = main_proj_array[t][GMWB_PREM_IF_BEF] * surv_month;	
		main_proj_array[t][GMWB_IF]  = main_proj_array[t][GMWB_IF_BEF] * surv_month; 
	}

	virtual void project_gmwb_bases_payments_claims(int t)
	{		
		this->ratchet_period_indicator = this->get_ratchet_indicator(t);
		//Initialization of rollup/ratchet bases
		this->init_bases(t);

		if (this->timing_items[t][POL_YEAR] > model_point->gmxb_deferral_period + this->gmwb_withdrawal_period)
			main_proj_array[t][GMWB_IF] = 0.0;
		else 
		{
			this->calculate_rollup_base(t);
			if (this->apply_ratchet)
			{
				this->calculate_ratchet_base_and_claims(t);
			}
			else
			{
				main_proj_array[t][GMWB_AV_IF_BEF] = main_proj_array[t][GMWB_RATCHET_PREV];
				this->calculate_gmwb_payments(t);
				if (this->gmwb_timing == "END")
					project_gmwb_payments_claims_end(t);
			}
		}
	}
};
