#pragma once
#include "Utility classes/Stat_Inforce_Configuration.h"
#include "Model point classes/Model_Point.h"
#include <string>
#include <algorithm>
#define NB_HEADER_FIELDS 48


using namespace std;

class Stat_Inforce_Manager
{

protected :
		vector<Model_Point*> v_of_new_business_policies;
		vector<Model_Point*> v_of_old_policies;
		vector<Model_Point*> v_of_recalc_policies;


	void concatenate_vector_of_mps(vector<Model_Point*>& initial_v_mps, 
									vector<Model_Point*>& v_mps_to_add)
	{
		for(unsigned int i = 0; i < v_mps_to_add.size(); i++)
		{

			Model_Point* mp = v_mps_to_add[i];
			initial_v_mps.push_back(mp);
		}
	}

	void get_v_of_policies(vector<Model_Point*>& initial_v_model_points)
	{

		for (unsigned int i = 0 ; i < initial_v_model_points.size(); i++)
		{
			Model_Point *mp = initial_v_model_points[i];
			if (mp->group.substr(mp->group.length() - 2, mp->group.length() - 1) == "_1")
			{
				// Try to find the old policies 
				// with _0
				bool old_policy_found = false;
				Model_Point *old_mp;
				string group_id_0 = mp->group.substr(0, mp->group.length() - 2) + "_0";
				for (int j = 0; j < initial_v_model_points.size(); j++)
				{
					old_mp = initial_v_model_points[j];
					if (old_mp->group == group_id_0)
					{
						old_policy_found = true;
						break;
					}
				}

				if (old_policy_found)
				{
					this->v_of_old_policies.push_back(old_mp);
					this->v_of_recalc_policies.push_back(mp);
				}
				else
					this->v_of_new_business_policies.push_back(mp);
			}
		}
	}

	void build_fund_stat_vector(vector<Model_Point::model_point_fund_stat>&  inforce_fund_list, Model_Point::model_point_fund_stat& fund_element, vector<string>& fund_ind_list)
	{
		/*
		* Check if the type of funds (A or T) is  found in the vector of types
		* Check if the fund is already in the vector inforce_fund_list
		* If so, statistics are made directly for fund in the vector result
		* If the fund is not fund in the vector, pushback in the vector result
		*/

		vector<string>::iterator fund_ind_it = find(fund_ind_list.begin(), fund_ind_list.end(), fund_element.fund_indicator);
	
		if(fund_ind_it != fund_ind_list.end())
		{
		    vector<Model_Point::model_point_fund_stat>::iterator fund_it = find(inforce_fund_list.begin(), inforce_fund_list.end(), fund_element); 
			
			if(fund_it == inforce_fund_list.end())
			{
			    inforce_fund_list.push_back(fund_element);
			}
			else
			{
			   fund_it->stat_fund_in_vector(fund_element);
			}
		}
	}

	void calculate_total_stat_funds(const vector<Model_Point*>& v_Model_Points, Stat_Inforce_Configuration& stat_inforce_config)
	{
		string fund_name;
		double total_nb_shares = 0.;
		double fund_value = 0.;
		double total_fund_value = 0;

		int number_of_model_points = v_Model_Points.size();

		vector<Model_Point::model_point_fund_stat> inforce_fund_list_A, inforce_fund_list_T;
		vector<string> fund_ind_list_A, fund_ind_list_T;

		fund_ind_list_A.push_back("A");
		fund_ind_list_A.push_back("");
		fund_ind_list_T.push_back("T");

		for(int mp_nb = 0; mp_nb < number_of_model_points; mp_nb++)
		{
			for (unsigned int fund_nb = 0; fund_nb < v_Model_Points[mp_nb]->stat_list_of_funds.size(); fund_nb++)
			{
				build_fund_stat_vector(inforce_fund_list_A, v_Model_Points[mp_nb]->stat_list_of_funds[fund_nb], fund_ind_list_A);
				build_fund_stat_vector(inforce_fund_list_T, v_Model_Points[mp_nb]->stat_list_of_funds[fund_nb], fund_ind_list_T);
			}
		}
		
		char file_name_A[MAX_LENGTH_FILE_NAME];
		char file_name_T[MAX_LENGTH_FILE_NAME];
		
		sprintf(file_name_A, "%s%s%s", stat_inforce_config.get_output_directory().c_str(),
				 stat_inforce_config.get_stat_fund_A_file_name().c_str(),".csv");
		sprintf(file_name_T, "%s%s%s", stat_inforce_config.get_output_directory().c_str(),
				 stat_inforce_config.get_stat_fund_T_file_name().c_str(),".csv");

		ofstream stat_inforce_file_fund_A(file_name_A);
		ofstream stat_inforce_file_fund_T(file_name_T);

		for (unsigned int i = 0; i < inforce_fund_list_A.size(); i++)
		{
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].fund_name << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].nb_share << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].fund_value << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].total_value << "," ;
			stat_inforce_file_fund_A << endl;
		}

		for (unsigned int i = 0; i < inforce_fund_list_T.size(); i++)
		{
			stat_inforce_file_fund_T << setprecision(15) << inforce_fund_list_T[i].fund_name << "," ;
			stat_inforce_file_fund_T << setprecision(15) << inforce_fund_list_T[i].nb_share << "," ;
			stat_inforce_file_fund_T << endl;
		}

		stat_inforce_file_fund_A.close();
		stat_inforce_file_fund_T.close();
	}

	void calculate_total_stat_funds_NB(const vector<Model_Point*>& v_Model_Points, 
				Stat_Inforce_Configuration& stat_inforce_config, string& fund_stat_file_name_A,
				string& fund_stat_file_name_T)
	{
		string fund_name;
		double total_nb_shares = 0.;
		double fund_value = 0.;
		double total_fund_value = 0;

		int number_of_model_points = v_Model_Points.size();

		vector<Model_Point::model_point_fund_stat> inforce_fund_list_A, inforce_fund_list_T;
		vector<string> fund_ind_list_A, fund_ind_list_T;

		fund_ind_list_A.push_back("A");
		fund_ind_list_A.push_back("");
		fund_ind_list_T.push_back("T");

		for(int mp_nb = 0; mp_nb < number_of_model_points; mp_nb++)
		{
			for (unsigned int fund_nb = 0; fund_nb < v_Model_Points[mp_nb]->stat_list_of_funds.size(); fund_nb++)
			{
				build_fund_stat_vector(inforce_fund_list_A, v_Model_Points[mp_nb]->stat_list_of_funds[fund_nb], fund_ind_list_A);
				build_fund_stat_vector(inforce_fund_list_T, v_Model_Points[mp_nb]->stat_list_of_funds[fund_nb], fund_ind_list_T);
			}
		}
		
		char file_name_A[MAX_LENGTH_FILE_NAME];
		char file_name_T[MAX_LENGTH_FILE_NAME];
		
		sprintf(file_name_A, "%s", fund_stat_file_name_A.c_str());
		sprintf(file_name_T, "%s", fund_stat_file_name_T.c_str());

		ofstream stat_inforce_file_fund_A(file_name_A);
		ofstream stat_inforce_file_fund_T(file_name_T);

		for (unsigned int i = 0; i < inforce_fund_list_A.size(); i++)
		{
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].fund_name << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].nb_share << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].fund_value << "," ;
			stat_inforce_file_fund_A << setprecision(15) << inforce_fund_list_A[i].total_value << "," ;
			stat_inforce_file_fund_A << endl;
		}

		for (unsigned int i = 0; i < inforce_fund_list_T.size(); i++)
		{
			stat_inforce_file_fund_T << setprecision(15) << inforce_fund_list_T[i].fund_name << "," ;
			stat_inforce_file_fund_T << setprecision(15) << inforce_fund_list_T[i].nb_share << "," ;
			stat_inforce_file_fund_T << endl;
		}

		stat_inforce_file_fund_A.close();
		stat_inforce_file_fund_T.close();
	}
	
	virtual void calculate_total_stat(const vector<Model_Point*>& v_Model_Points, Stat_Inforce_Configuration& stat_inforce_config)
	{
		double total_paid_premium = 0.;
		double total_free_partial_withdrawal = 0.;
		double total_current_premium = 0.;
		double total_gmab_bb_for_rollup = 0.;
		double total_gmab_bb_for_ratchet = 0.;
		double total_gmdb_bb_for_rollup = 0.;
		double total_gmdb_bb_for_ratchet = 0.;
		double total_gmib_bb_for_rollup = 0.;
		double total_gmib_bb_for_ratchet = 0.;
		double total_gmwb_bb_for_rollup = 0.;
		double total_gmwb_bb_for_ratchet = 0.;

		double total_account_value_by_index[MAXIMUM_NUMBER_OF_INDEXES] = {0.0};

		double total_account_value = 0.;
		int total_number_of_policies = 0; 
		double total_benefit_payment = 0.;
		double total_year_payment = 0.;
		double total_realized_rrc = 0.;
		double total_realized_ehc = 0.;
		double male_percentage = 0.;
		double female_percentage = 0.;
		double male_n = 0.;
		double female_n = 0.;
		double account_value_weighted_duration = 0.;
		double account_value_weighted_age = 0.;
		double account_value_weighted_fund_mgmt_fees = 0.;
		double account_value_weighted_administration_fees = 0.;
		double weighted_rrc = 0.; 
		double weighted_ehc = 0.;
		double total_rrc = 0.;
		double total_ehc = 0.;
		double account_value_per_mp = 0.;
		double prem_paid_per_mp = 0.;
		double prem_curr_per_mp = 0.;
		double diff_av = 0.;
		double diff_premium = 0.;
		double total_av_old = 0.;
		double total_premium_old = 0.;
		double av_prem_curr = 0.;
		double total_marge=0.;
		double min_marge = 0.;
		double marge= 0.;
		double max_marge=0.;
		double realized_rrc = 0.;
		double realized_ehc = 0.;
		double total_lapse_amount=0.;
		double total_reappeared_amount=0.;
		double guaranted_payment_total=0.;
		/*new columns */
		double realized_rrc_total = 0.;
		double realized_ehc_total = 0.;
		double admin_realized = 0.;
		double upfront_g_c_value = 0.;
		double fees_and_others = 0.;

		//Create the output file
		char file_name[MAX_LENGTH_FILE_NAME];
		sprintf(file_name, "%s%s%s", stat_inforce_config.get_output_directory().c_str(), stat_inforce_config.get_stat_inforce_file_name().c_str(),".csv");
		ofstream stat_inforce_file(file_name);

		stat_inforce_file << "Total Paid premiums,Total Free Partial Withdrawal,Total Current premium," ;
		stat_inforce_file << " AB Ratchet,AB rollup,DB Ratchet,DB rollup,IB Ratchet,IB rollup,WB Ratchet,WB rollup,";
		stat_inforce_file << " AV Total ,RRC_average,EHC_average,Administrative fees,Management fees,";
		stat_inforce_file << " Av Sub0,Av Sub1,Av Sub2,Av Sub3,Av Sub4,Av Sub5,Av Sub6,Av Sub7,Av Sub8,Av Sub9,Av Sub10,Av Sub11,";
		stat_inforce_file << " Weighted Age,Weighted Duration,Nb policies,male,female,Benefit payment,";
		stat_inforce_file << " Benefit year payment,AV_prem_curr,Moyenne_marge,Max_marge,Min_marge,Montant_lapse,Reappeared_policy,Guaranteed_payement,";
		stat_inforce_file << " Realized RRC,Realized EHC,Admin_realized,Upfront_g_c,Fees_and_others,";
		stat_inforce_file << " Diff_AV , Diff_Premium"; 
		stat_inforce_file << endl;

		/*
		* check if nb MP = 0. If is 0 then create an empty file, display a message and continue
		*/
		int number_of_model_points = v_Model_Points.size();
		
		if (number_of_model_points == 0)
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "The number of model points is zero for one (or many) product(s) of the stat inforce %s", stat_inforce_config.get_stat_inforce_name().c_str());
			for (int empty_file =0; empty_file < NB_HEADER_FIELDS; empty_file ++)
				stat_inforce_file << 0 << "," ;
			stat_inforce_file << 0; 
			stat_inforce_file.close();
			return;

		}

		if (stat_inforce_config.get_do_compare_files())	
			compare_inforce_files(stat_inforce_config,total_lapse_amount,total_reappeared_amount,guaranted_payment_total,fees_and_others);

		int current_mp_nb = 0;
		for(int mp_nb = 0; mp_nb < number_of_model_points; mp_nb++)
		{
			current_mp_nb=current_mp_nb+1; 
			account_value_per_mp = 0.0;
			prem_paid_per_mp = 0.0;
			prem_curr_per_mp = 0.;
			marge=0.0;

			Model_Point * mp = v_Model_Points[mp_nb];

			total_paid_premium += mp->prem_paid; 
			total_current_premium += mp->prem_curr;
			total_free_partial_withdrawal += mp->fpw_accum_total;
			prem_paid_per_mp = mp->prem_paid; 
			prem_curr_per_mp = mp->prem_curr; 
			total_gmab_bb_for_rollup += mp->gmab_rollup_valn;
			total_gmab_bb_for_ratchet += mp->gmab_ratchet_valn;
			total_gmdb_bb_for_rollup += mp->gmdb_rollup_valn;
			total_gmdb_bb_for_ratchet += mp->gmdb_ratchet_valn;
			total_gmib_bb_for_rollup += mp->gmib_rollup_valn;
			total_gmib_bb_for_ratchet += mp->gmib_ratchet_valn;
			total_gmwb_bb_for_rollup += mp->gmwb_rollup_valn;
			total_gmwb_bb_for_ratchet += mp->gmwb_ratchet_valn;
			total_benefit_payment += mp->gmwb_wdl_cum_dyn_lapse;
			total_year_payment += mp->gmwb_payt_yr;

			for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
			{
				total_account_value_by_index[i] += mp->av_if_array[i];
				total_account_value += mp->av_if_array[i];
				account_value_per_mp += mp->av_if_array[i];
			}

			if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_CURR")
			{
				total_rrc += (mp->gmxb_g_c * prem_curr_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * prem_curr_per_mp);
			}
			else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_PAID")
			{
				total_rrc += (mp->gmxb_g_c * prem_paid_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * prem_paid_per_mp);
			}
			else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "AV")
			{
				total_rrc += (mp->gmxb_g_c * account_value_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * account_value_per_mp);
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Allowed values for 'realized_rrc_ehc_prem' parameter are : PREM_PAID, PREM_CURR, AV ");
				throw new GMXBException(message);	
			}

			av_prem_curr += mp->prem_curr;	
			realized_rrc_total += mp->realized_RRC; 
			/*Divison by zero test*/
			if (mp->gmxb_g_c == 0) 
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The RRC value is zero in the inforce file for the run name %s and the policy id %s", stat_inforce_config.get_stat_inforce_name().c_str(), mp->policy_id.c_str());
				throw new GMXBException(message);	
			}
			else
			{
				realized_ehc_total += (mp->realized_RRC * mp->gmxb_ehc_charge/mp->gmxb_g_c); 
			}

			admin_realized += mp->admin_charges;
			upfront_g_c_value += (mp->up_front_g_c * mp->prem_paid);

			total_realized_rrc += mp->gmxb_g_c;
			total_realized_ehc += mp->gmxb_ehc_charge;
			//Calculer le max et le min----------------------
			realized_rrc = mp->gmxb_g_c;
			realized_ehc = mp->gmxb_ehc_charge;
			if (current_mp_nb == 1) 
			{
				min_marge=realized_rrc - realized_ehc;
				max_marge=realized_rrc - realized_ehc;
			}
			marge=realized_rrc - realized_ehc;
			if (marge < min_marge) 
				min_marge=marge;
			else if (marge > max_marge)  
				max_marge= marge;
			//-----------------------------------------------

			if (mp->sex == "M")
				male_n++;
			else
				female_n++;

			account_value_weighted_duration += (mp->duration * account_value_per_mp);
			account_value_weighted_age += (mp->age_valn * account_value_per_mp);
			account_value_weighted_fund_mgmt_fees += (mp->gmxb_m_c * account_value_per_mp) ;
			account_value_weighted_administration_fees += (mp->gmxb_a_c * account_value_per_mp);
		}

		total_marge = (total_realized_rrc - total_realized_ehc)/number_of_model_points ;
		
		if (total_account_value != 0.)
		{
			account_value_weighted_administration_fees = account_value_weighted_administration_fees / total_account_value;
			account_value_weighted_fund_mgmt_fees = account_value_weighted_fund_mgmt_fees / total_account_value;
			account_value_weighted_duration = account_value_weighted_duration / total_account_value;
			account_value_weighted_age = account_value_weighted_age / total_account_value ;
		}

		if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_CURR")
		{
			if (total_current_premium != 0.)
			{
				total_rrc = total_rrc/total_current_premium;
				total_ehc = total_ehc/total_current_premium;
			}
		}
		else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_PAID")
		{
			if (total_paid_premium != 0.)
			{
				total_rrc = total_rrc/total_paid_premium;
				total_ehc = total_ehc/total_paid_premium;
			}
		}
		else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "AV")
		{
			if (total_account_value != 0.)
			{
				total_rrc = total_rrc/total_account_value;
				total_ehc = total_ehc/total_account_value;
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Allowed values for 'realized_rrc_ehc_prem' parameter are : PREM_PAID, PREM_CURR, AV ");
			throw new GMXBException(message);	
		}

		male_percentage = male_n /number_of_model_points ;
		female_percentage = female_n / number_of_model_points;
		total_number_of_policies = number_of_model_points;
		av_prem_curr = av_prem_curr /  number_of_model_points;

		// Write the output result
		stat_inforce_file << setprecision(15) << total_paid_premium << ","; 
		stat_inforce_file << setprecision(15) << total_free_partial_withdrawal << ",";
		stat_inforce_file << setprecision(15) << total_current_premium << ",";
		stat_inforce_file << setprecision(15) << total_gmab_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmab_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmdb_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmdb_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmib_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmib_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmwb_bb_for_ratchet << ",";		
		stat_inforce_file << setprecision(15) << total_gmwb_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_account_value << ",";
		stat_inforce_file << setprecision(15) << total_rrc << ",";
		stat_inforce_file << setprecision(15) << total_ehc << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_administration_fees << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_fund_mgmt_fees << ",";

		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
			stat_inforce_file << setprecision(15) << total_account_value_by_index[i] << ",";

		stat_inforce_file << setprecision(15) << account_value_weighted_age << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_duration << ",";
		stat_inforce_file << setprecision(15) << total_number_of_policies << ",";
		stat_inforce_file << setprecision(15) << male_percentage << ",";
		stat_inforce_file << setprecision(15) << female_percentage << ",";
		stat_inforce_file << setprecision(15) << total_benefit_payment << ",";
		stat_inforce_file << setprecision(15) << total_year_payment << ",";
		stat_inforce_file << setprecision(15) << av_prem_curr << ",";
		stat_inforce_file << setprecision(15) << total_marge << ",";
		stat_inforce_file << setprecision(15) << max_marge << ",";
		stat_inforce_file << setprecision(15) << min_marge << ",";
		stat_inforce_file << setprecision(15) << total_lapse_amount << ",";
		stat_inforce_file << setprecision(15) << total_reappeared_amount << ",";
		stat_inforce_file << setprecision(15) << guaranted_payment_total << ",";
		/*added by Andrei -> new columns*/
		stat_inforce_file << setprecision(15) << realized_rrc_total << ",";
		stat_inforce_file << setprecision(15) << realized_ehc_total << ",";
		stat_inforce_file << setprecision(15) << admin_realized << ",";
		stat_inforce_file << setprecision(15) << upfront_g_c_value << ",";
		stat_inforce_file << setprecision(15) << fees_and_others << ",";
		stat_inforce_file << setprecision(15) << diff_av << ","; /*on va afficher 0 */
		stat_inforce_file << setprecision(15) << diff_premium; /*on va afficher 0. this columns will contains values only for recalc_ehc*/
		stat_inforce_file << endl;
		stat_inforce_file.close();

	}

	virtual void calculate_total_stat_NB(const vector<Model_Point*>& v_Model_Points, Stat_Inforce_Configuration& stat_inforce_config, string& name)
	{
		double total_paid_premium = 0.;
		double total_free_partial_withdrawal = 0.;
		double total_current_premium = 0.;
		double total_gmab_bb_for_rollup = 0.;
		double total_gmab_bb_for_ratchet = 0.;
		double total_gmdb_bb_for_rollup = 0.;
		double total_gmdb_bb_for_ratchet = 0.;
		double total_gmib_bb_for_rollup = 0.;
		double total_gmib_bb_for_ratchet = 0.;
		double total_gmwb_bb_for_rollup = 0.;
		double total_gmwb_bb_for_ratchet = 0.;

		double total_account_value_by_index[MAXIMUM_NUMBER_OF_INDEXES] = {0.0};

		double total_account_value = 0.;
		int total_number_of_policies = 0; 
		double total_benefit_payment = 0.;
		double total_year_payment = 0.;
		double total_realized_rrc = 0.;
		double total_realized_ehc = 0.;
		double realized_rrc = 0.;
		double realized_ehc = 0.;
		double total_marge = 0.;
		double min_marge = 0.;
		double marge= 0.;
		double max_marge=0.;
		double male_percentage = 0.;
		double female_percentage = 0.;
		double male_n = 0.;
		double female_n = 0.;
		double account_value_weighted_duration = 0.;
		double account_value_weighted_age = 0.;
		double account_value_weighted_fund_mgmt_fees = 0.;
		double account_value_weighted_administration_fees = 0.;
		double total_rrc = 0.;
		double total_ehc = 0.;
		double account_value_per_mp = 0.;
		double prem_paid_per_mp = 0.;
		double prem_curr_per_mp = 0.;
		double diff_av = 0.;
		double diff_premium = 0.;
		double total_av_recalc = 0.;
		double total_premium_recalc = 0.;
		double total_av_old = 0.;
		double total_premium_old = 0.;
		double av_prem_curr = 0.0;

		double total_lapse_amount=0.;
		double total_reappeared_amount=0.;
		double guaranted_payment_total=0.;
		/*new columns added*/
		double realized_rrc_total = 0.;
		double realized_ehc_total = 0.;
		double admin_realized = 0.;
		double upfront_g_c_value = 0.;
		double fees_and_others = 0.;

		//Create the output file
		char file_name[MAX_LENGTH_FILE_NAME];
		sprintf(file_name, "%s%s%s", stat_inforce_config.get_output_directory().c_str(), stat_inforce_config.get_stat_inforce_file_name().c_str(), name.c_str());
		ofstream stat_inforce_file(file_name);

		stat_inforce_file << "Total Paid premiums,Total Free Partial Withdrawal,Total Current premium," ;
		stat_inforce_file << " AB Ratchet,AB rollup,DB Ratchet,DB rollup,IB Ratchet,IB rollup,WB Ratchet,WB rollup,";
		stat_inforce_file << " AV Total ,RRC_average,EHC_average,Administrative fees,Management fees,";
		stat_inforce_file << " Av Sub0,Av Sub1,Av Sub2,Av Sub3,Av Sub4,Av Sub5,Av Sub6,Av Sub7,Av Sub8,Av Sub9,Av Sub10,Av Sub11,";
		stat_inforce_file << " Weighted Age,Weighted Duration,Nb policies,male,female,Benefit payment,";
		stat_inforce_file << " Benefit year payment,AV_prem_curr,Moyenne_marge,Max_marge,Min_marge,Montant_lapse,Reappeared_policy,Guaranteed_payement,";
		stat_inforce_file << " Realized RRC,Realized EHC,Admin_realized,Upfront_g_c,Fees_and_others,";
		stat_inforce_file << " Diff_AV , Diff_Premium"; 
		stat_inforce_file << endl;

		/*
		*check if nb MP = 0. If is 0 then create an empty file, display a message and continue
		*/
		int number_of_model_points = v_Model_Points.size();
		if (number_of_model_points == 0)
		{ 
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "The number of model points is zero for one (or many) product(s) of the stat inforce %s", stat_inforce_config.get_stat_inforce_name().c_str());
			for (int empty_file =0; empty_file < NB_HEADER_FIELDS; empty_file ++)
				stat_inforce_file << 0 << "," ;
			stat_inforce_file << 0; 
			stat_inforce_file.close();
			return;
		}

		for (unsigned int i = 0; i < this->v_of_recalc_policies.size(); i ++)
		{
			Model_Point* mp = v_of_recalc_policies[i];
			for (int j = 0; j <MAXIMUM_NUMBER_OF_INDEXES; j++)
					total_av_recalc += mp->av_if_array[j];
			
			total_premium_recalc += mp->prem_paid;
		}

		for (unsigned int i = 0; i < this->v_of_old_policies.size(); i ++)
		{
			Model_Point* mp = v_of_old_policies[i];
			for (int j = 0; j <MAXIMUM_NUMBER_OF_INDEXES; j++)
				total_av_old += mp->av_if_array[j];
			
			total_premium_old += mp->prem_paid;
		}

		diff_av = total_av_recalc - total_av_old;
		diff_premium = total_premium_recalc - total_premium_old;
		
		int current_mp_nb = 0;
		for(int mp_nb = 0; mp_nb < number_of_model_points; mp_nb++)
		{
			current_mp_nb=current_mp_nb+1;
			account_value_per_mp = 0.0;
			prem_paid_per_mp = 0.0;
			prem_curr_per_mp = 0.;
			marge=0.0;

			Model_Point * mp = v_Model_Points[mp_nb];

			total_paid_premium += mp->prem_paid; 
			total_current_premium += mp->prem_curr;
			prem_paid_per_mp = mp->prem_paid; 
			prem_curr_per_mp = mp->prem_curr; 
			total_free_partial_withdrawal += mp->fpw_accum_total;
			total_gmab_bb_for_rollup += mp->gmab_rollup_valn;
			total_gmab_bb_for_ratchet += mp->gmab_ratchet_valn;
			total_gmdb_bb_for_rollup += mp->gmdb_rollup_valn;
			total_gmdb_bb_for_ratchet += mp->gmdb_ratchet_valn;
			total_gmib_bb_for_rollup += mp->gmib_rollup_valn;
			total_gmib_bb_for_ratchet += mp->gmib_ratchet_valn;
			total_gmwb_bb_for_rollup += mp->gmwb_rollup_valn;
			total_gmwb_bb_for_ratchet += mp->gmwb_ratchet_valn;
			
			for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
			{
				total_account_value_by_index[i] += mp->av_if_array[i];
				total_account_value += mp->av_if_array[i];
				account_value_per_mp += mp->av_if_array[i];
			}
			total_benefit_payment += mp->gmwb_wdl_cum_dyn_lapse;
			total_year_payment += mp->gmwb_payt_yr;
			
			if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_CURR")
			{
				total_rrc += (mp->gmxb_g_c * prem_curr_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * prem_curr_per_mp);
			}
			else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_PAID")
			{
				total_rrc += (mp->gmxb_g_c * prem_paid_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * prem_paid_per_mp);
			}
			else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "AV")
			{
				total_rrc += (mp->gmxb_g_c * account_value_per_mp);
				total_ehc += (mp->gmxb_ehc_charge * account_value_per_mp);
			}
			else
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Allowed values for 'realized_rrc_ehc_prem' parameter are : PREM_PAID, PREM_CURR, AV ");
				throw new GMXBException(message);	
			}

			av_prem_curr += mp->prem_curr;
			realized_rrc_total += mp->realized_RRC;
			/*Divison by zero test*/
			if (mp->gmxb_g_c == 0)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "The RRC value is zero in the inforce file for the run name %s and the policy id %s",stat_inforce_config.get_stat_inforce_name().c_str(),mp->policy_id.c_str() );
				throw new GMXBException(message);	
			}
			else
			{
				realized_ehc_total += (mp->realized_RRC * mp->gmxb_ehc_charge/mp->gmxb_g_c);
			}

			admin_realized += mp->admin_charges;
			upfront_g_c_value += (mp->up_front_g_c * mp->prem_paid);

			if (mp->sex == "M")
				male_n++;
			else
				female_n++;

			account_value_weighted_duration += (mp->duration * account_value_per_mp);
			account_value_weighted_age += (mp->age_valn * account_value_per_mp);
			account_value_weighted_fund_mgmt_fees += (mp->gmxb_m_c * account_value_per_mp) ;
			account_value_weighted_administration_fees += (mp->gmxb_a_c * account_value_per_mp);

			//Calculer le max et le min----------------------
			realized_rrc = mp->gmxb_g_c;
			total_realized_rrc += mp->gmxb_g_c; 
			realized_ehc = mp->gmxb_ehc_charge;
			total_realized_ehc += mp->gmxb_ehc_charge;
			if (current_mp_nb == 1) 
			{
				min_marge=realized_rrc - realized_ehc;
				max_marge=realized_rrc - realized_ehc;
			}
			marge=realized_rrc - realized_ehc;
			if (marge < min_marge) 
				min_marge=marge;
			else if (marge > max_marge)  
				max_marge= marge;
			//-----------------------------------------------
		}
		
		total_marge = (total_realized_rrc - total_realized_ehc)/number_of_model_points ;
		
		if (total_account_value != 0.)
		{
			account_value_weighted_administration_fees = account_value_weighted_administration_fees / total_account_value;
			account_value_weighted_fund_mgmt_fees = account_value_weighted_fund_mgmt_fees / total_account_value;
			account_value_weighted_duration = account_value_weighted_duration / total_account_value;
			account_value_weighted_age = account_value_weighted_age / total_account_value ;
		}

		if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_CURR")
		{
			if (total_current_premium != 0.)
			{
				total_rrc = total_rrc / total_current_premium;
				total_ehc = total_ehc / total_current_premium;
			}
		}
		else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "PREM_PAID")
		{
			if (total_paid_premium != 0.)
			{
				total_rrc = total_rrc / total_paid_premium;
				total_ehc = total_ehc / total_paid_premium;
			}
		}
		else if (stat_inforce_config.get_ehc_rrc_weighted_criteria() == "AV")
		{
			if (total_account_value != 0.)
			{
				total_rrc = total_rrc / total_account_value;
				total_ehc = total_ehc / total_account_value;
			}
		}
		else
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf(message, "Allowed values for 'realized_rrc_ehc_prem' parameter are : PREM_PAID, PREM_CURR, AV ");
			throw new GMXBException(message);	
		}

		male_percentage = male_n /number_of_model_points ;
		female_percentage = female_n / number_of_model_points;
		total_number_of_policies = number_of_model_points;
		av_prem_curr = av_prem_curr /  number_of_model_points;
		
		// Write the output result
		stat_inforce_file << setprecision(15) << total_paid_premium << ","; 
		stat_inforce_file << setprecision(15) << total_free_partial_withdrawal << ",";
		stat_inforce_file << setprecision(15) << total_current_premium << ",";
		stat_inforce_file << setprecision(15) << total_gmab_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmab_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmdb_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmdb_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmib_bb_for_ratchet << ",";
		stat_inforce_file << setprecision(15) << total_gmib_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_gmwb_bb_for_ratchet << ",";		
		stat_inforce_file << setprecision(15) << total_gmwb_bb_for_rollup << ",";
		stat_inforce_file << setprecision(15) << total_account_value << ",";
		stat_inforce_file << setprecision(15) << total_rrc << ",";
		stat_inforce_file << setprecision(15) << total_ehc << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_administration_fees << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_fund_mgmt_fees << ",";

		for (int i = 0; i < MAXIMUM_NUMBER_OF_INDEXES; i++)
			stat_inforce_file << setprecision(15) << total_account_value_by_index[i] << ",";

		stat_inforce_file << setprecision(15) << account_value_weighted_age << ",";
		stat_inforce_file << setprecision(15) << account_value_weighted_duration << ",";
		stat_inforce_file << setprecision(15) << total_number_of_policies << ",";
		stat_inforce_file << setprecision(15) << male_percentage << ",";
		stat_inforce_file << setprecision(15) << female_percentage << ",";
		stat_inforce_file << setprecision(15) << total_benefit_payment << ",";
		stat_inforce_file << setprecision(15) << total_year_payment << ",";

		//Realized EHC,Realized RRC,Total Premium
		stat_inforce_file << setprecision(15) << av_prem_curr << ",";
		stat_inforce_file << setprecision(15) << total_marge << ","; 
		stat_inforce_file << setprecision(15) << max_marge << ",";
		stat_inforce_file << setprecision(15) << min_marge << ",";
		stat_inforce_file << setprecision(15) << total_lapse_amount << ","; //0 no compare files
		stat_inforce_file << setprecision(15) << total_reappeared_amount << ","; // 0 no compare files
		stat_inforce_file << setprecision(15) << guaranted_payment_total << ","; //0 no compare files
		/*new columns*/
		stat_inforce_file << setprecision(15) << realized_rrc_total << ",";
		stat_inforce_file << setprecision(15) << realized_ehc_total << ",";
		stat_inforce_file << setprecision(15) << admin_realized << ",";
		stat_inforce_file << setprecision(15) << upfront_g_c_value << ",";
		stat_inforce_file << setprecision(15) << fees_and_others << ","; // 0 no compare files
		stat_inforce_file << setprecision(15) << diff_av << ","; 
		stat_inforce_file << setprecision(15) << diff_premium; 
		stat_inforce_file << endl;
		stat_inforce_file.close();
	}


	virtual void compare_inforce_files(Stat_Inforce_Configuration& stat_inforce_config,double &total_lapse_amount, double &total_reappeared_amount,double &guaranted_payment_total, double &fees_and_others)
	{
		double mp_lapse_amount = 0.;
		double mp_reappeared_amount=0.;


		Inforce_Manager inforce_mgr;

		vector<string> v_prodKey = stat_inforce_config.get_v_product_codes_key();

		for (unsigned int prod=0; prod<v_prodKey.size();prod++) 
		{
			double guaranted_payment_new=0.;
			double guaranted_payment_old=0.;
			double guaranted_payment_prod=0.;
			double fees_and_others_mp;
			double av_new_inforce;
			double av_ref_inforce;

			string product_code = v_prodKey[prod];

			vector<Model_Point*> v_of_old_policies_c;
			vector<Model_Point*> v_of_new_policies_c;

			Product_configuration product_config(product_code, stat_inforce_config.get_inforce_file_name());
			Product_configuration product_config_old(product_code, stat_inforce_config.get_reference_file());
			Scenario_configuration scenario_config;

			inforce_mgr.get_list_of_model_points_per_product(product_code, v_of_new_policies_c, product_config, scenario_config);
			inforce_mgr.get_list_of_model_points_per_product(product_code, v_of_old_policies_c, product_config_old, scenario_config);			
			
			//Montant Lapse
			for (int j=0; j< v_of_old_policies_c.size(); j++)
			{
				bool check = false;
				for( int i=0; i < v_of_new_policies_c.size() ; i++ )
				{
					if (v_of_old_policies_c[j]->policy_id == v_of_new_policies_c[i]->policy_id)
					{
						check = true;
						break;
					}
				}
				if (check == false) 					 
				{
					mp_lapse_amount = 0;
					for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES; k++)
					{
						mp_lapse_amount += v_of_old_policies_c[j]->av_if_array[k]; //sum=sum(col N to col W) 
					}
					total_lapse_amount+=mp_lapse_amount;
				}
			}

			//Reappeared Policy
			for( int i=0; i < v_of_new_policies_c.size() ; i++ )
			{
				bool check = false;
				for (int j=0; j< v_of_old_policies_c.size(); j++)
				{
					if (v_of_new_policies_c[i]->policy_id == v_of_old_policies_c[j]->policy_id)
					{
						check = true;
						break;
					}
				}	

				if (check == false) 					 
				{
					mp_reappeared_amount = 0;
					for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES; k++)
					{
						mp_reappeared_amount += v_of_new_policies_c[i]->av_if_array[k];
					}
					total_reappeared_amount+=mp_reappeared_amount;
				}
			}
			//Fees and others
			for( int i=0; i < v_of_new_policies_c.size() ; i++ )
			{
				for (int j=0; j< v_of_old_policies_c.size(); j++)
				{
					if (v_of_new_policies_c[i]->policy_id == v_of_old_policies_c[j]->policy_id)
					{
						av_new_inforce = 0.; 
						av_ref_inforce = 0.;
						fees_and_others_mp = 0.;

						for (int k = 0; k < MAXIMUM_NUMBER_OF_INDEXES; k++)
						{
							av_new_inforce += v_of_new_policies_c[i]->av_if_array[k];
							av_ref_inforce += v_of_old_policies_c[j]->av_if_array[k];
						}
						fees_and_others_mp = av_new_inforce - av_ref_inforce;
						fees_and_others += fees_and_others_mp;
					}
				}
			}

			////Guaranted Payement 
			for( int i=0; i < v_of_new_policies_c.size() ; i++ )
			{
				guaranted_payment_new += v_of_new_policies_c[i]->gmwb_wdl_cum_dyn_lapse;
			}
			for (int j=0; j< v_of_old_policies_c.size(); j++)
			{
				guaranted_payment_old += v_of_old_policies_c[j]->gmwb_wdl_cum_dyn_lapse;
			}

			guaranted_payment_prod = guaranted_payment_new-guaranted_payment_old;
			guaranted_payment_total+=guaranted_payment_prod;

			for (int j=0; j < v_of_old_policies_c.size(); j++)
				delete v_of_old_policies_c[j];

			for (int i=0;i< v_of_new_policies_c.size() ; i++ )
				delete v_of_new_policies_c[i];
		}

	}

public:

	Stat_Inforce_Manager()
	{
	}

	virtual void perform_statistics(Stat_Inforce_Configuration& stat_inforce_config)
	{
		if (stat_inforce_config.get_recalc_ehc_indicator())
		{
			perform_statistics_NB(stat_inforce_config);
		}
		else
			perform_statistics_not_NB(stat_inforce_config);		
	}

	void perform_statistics_not_NB(Stat_Inforce_Configuration& stat_inforce_config)
	{
		vector<Model_Point*> v_mps_initial;
		Inforce_Manager inforce_mgr;
		vector<string> v_pCK = stat_inforce_config.get_v_product_codes_key();
		vector<string> v_gmxb_type = stat_inforce_config.get_v_gmxb_type();

		if ((stat_inforce_config.get_criteria() == "ALL"))
		{
			string product_code = v_pCK[0];
			Product_configuration product_config(product_code, stat_inforce_config.get_inforce_file_name());
			Scenario_configuration scenario_config;
			
			inforce_mgr.get_list_of_all_model_points(product_code, v_mps_initial, product_config, scenario_config);
			this->calculate_total_stat(v_mps_initial, stat_inforce_config);
			this->calculate_total_stat_funds(v_mps_initial, stat_inforce_config);
		}
		else
		{
			for (unsigned int j =0; j < v_pCK.size(); j++)
			{
				vector<Model_Point*> v_mps;
				string product_code = v_pCK[j];

				Product_configuration product_config(product_code, stat_inforce_config.get_inforce_file_name());
				Scenario_configuration scenario_config;

				if ((stat_inforce_config.get_criteria() == "PRODUCT"))
				{
					inforce_mgr.get_list_of_model_points_per_product(product_code, v_mps, product_config, scenario_config);
					this->concatenate_vector_of_mps(v_mps_initial, v_mps);

					// The calculation is made only when all the mp are concatened
					if (j == v_pCK.size()-1)
					{	
						this->calculate_total_stat(v_mps_initial, stat_inforce_config);
						this->calculate_total_stat_funds(v_mps_initial, stat_inforce_config);
					}
				}
				else if ((stat_inforce_config.get_criteria() == "BOTH"))
				{
					for (unsigned int i = 0; i < v_gmxb_type.size();i++)
					{
						string gmxb_type = v_gmxb_type[i];
						inforce_mgr.get_list_of_model_points_per_product_and_gmxb_type(product_code, gmxb_type, v_mps, product_config, scenario_config);
						this->concatenate_vector_of_mps(v_mps_initial, v_mps);
						v_mps.clear();
					}
					
					if (j == v_pCK.size()-1)
					{
						this->calculate_total_stat(v_mps_initial,stat_inforce_config);
						this->calculate_total_stat_funds(v_mps_initial,stat_inforce_config);
					}
				}
				else
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "The stat inforce criteria %s is unknown. It must take only 3 values : PRODUCT, BOTH or ALL.", stat_inforce_config.get_criteria().c_str());
					throw new GMXBException(message);
				}
			}
		}

		for (int mp=0; mp< v_mps_initial.size(); mp++)
			delete v_mps_initial[mp];

	}

	void perform_statistics_NB(Stat_Inforce_Configuration& stat_inforce_config)
	{
		vector<Model_Point*> v_mps_initial;
		Inforce_Manager inforce_mgr;

		vector<string> v_pCK = stat_inforce_config.get_v_product_codes_key();

		for (unsigned int j =0; j < v_pCK.size(); j++)
		{
			vector<Model_Point*> v_mps;
			string product_code = v_pCK[j];
		
			Product_configuration product_config(product_code, stat_inforce_config.get_inforce_file_name());
			Scenario_configuration scenario_config;

			inforce_mgr.get_list_of_model_points_per_product(product_code, v_mps, product_config, scenario_config);
			this->concatenate_vector_of_mps(v_mps_initial, v_mps);
		}

		get_v_of_policies(v_mps_initial);

		string old("_recalc_old.csv");
		string nb(".csv");
		string recalc("_recalc_new.csv");

		this->calculate_total_stat_NB(this->v_of_new_business_policies,stat_inforce_config, nb);
		this->calculate_total_stat_NB(this->v_of_old_policies, stat_inforce_config, old);
		this->calculate_total_stat_NB(this->v_of_recalc_policies,stat_inforce_config, recalc);

		string recalc_file_name_A = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_A_file_name() + "_recalc_new.csv" ;
		string recalc_file_name_T = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_T_file_name()+ "_recalc_new.csv";

		string old_file_name_A = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_A_file_name()+ "_recalc_old.csv";
		string old_file_name_T = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_T_file_name()+ "_recalc_old.csv";

		string nb_file_name_A = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_A_file_name() + ".csv" ;
		string nb_file_name_T = stat_inforce_config.get_output_directory() + stat_inforce_config.get_stat_fund_T_file_name() + ".csv" ;

		this->calculate_total_stat_funds_NB(this->v_of_recalc_policies, stat_inforce_config, recalc_file_name_A, recalc_file_name_T);
		this->calculate_total_stat_funds_NB(this->v_of_old_policies, stat_inforce_config, old_file_name_A, old_file_name_T);
		this->calculate_total_stat_funds_NB(this->v_of_new_business_policies, stat_inforce_config, nb_file_name_A, nb_file_name_T);

		//  delete the vector of model points
		for (int i =0; i < v_mps_initial.size(); i++)
		{
			Model_Point *mp = v_mps_initial[i];
			delete mp;
		}	
	}

};