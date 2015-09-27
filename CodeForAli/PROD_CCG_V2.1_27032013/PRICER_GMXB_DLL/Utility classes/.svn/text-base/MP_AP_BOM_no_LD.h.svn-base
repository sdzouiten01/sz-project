#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "stdafx.h"
#include "fund_for_aging_policies.h"
#include "MP_AP_BOM.h"
#include "MP_AP_EOM_no_NB.h"


class MP_AP_BOM_no_LD : public MP_AP_BOM
{
private : 
	
	bool switched;
	bool number_of_shares_changed;
	
	map<string, fund_for_aging_policies> fund_description_A_EOM;
	map<string, fund_for_aging_policies> fund_description_T_EOM;
	int taille_A_EOM;
	int taille_T_EOM;

public:
	
	MP_AP_BOM_no_LD(void)
	{
	}
	
	//MP_AP_BOM_no_LD(const MP_AP_BOM& mp) : MP_AP_BOM (mp)
	//{
	//	this->set_lapsed_or_dead(false);
	//	this->switched = false;
	//	this->number_of_shares_changed = false;
	//}

	MP_AP_BOM_no_LD (const string & str) : MP_AP_BOM (str)
	{
		this->taille_A_EOM = 0;
		this->taille_T_EOM = 0;
		this->switched = false;
		this->number_of_shares_changed = false;
		/*this->fund_description_A_EOM = map<string,fund_for_aging_policies>;
		this->fund_description_T_EOM = map<string,fund_for_aging_policies>;*/
	}
	
	MP_AP_BOM_no_LD (const MP_AP_BOM_no_LD& mp) : MP_AP_BOM (mp)
	{
		this->fund_description_A_EOM = mp.fund_description_A_EOM;
		this->fund_description_T_EOM = mp.fund_description_T_EOM;
		this->taille_A_EOM = mp.taille_A_EOM;
		this->taille_T_EOM = mp.taille_T_EOM;
		this->switched = mp.switched;
		this->number_of_shares_changed = mp.number_of_shares_changed;
	}


	~MP_AP_BOM_no_LD(void)
	{
	}

	MP_AP_BOM_no_LD& operator= (const MP_AP_BOM_no_LD& mp)
	{
		this->row = mp.row;
		this->lapsed_or_dead = mp.lapsed_or_dead;
		this->taille_A_BOM = mp.taille_A_BOM;
		this->taille_T_BOM = mp.taille_T_BOM;
		this->fund_description_A_BOM = mp.fund_description_A_BOM;
		this->fund_description_T_BOM = mp.fund_description_T_BOM;
		this->performance = mp.performance;
		int i;
		/*this->current_weights = new double [12];
		this->target_weights = new double [12];*/
		for (i = 0; i<12; i++)
		{
			this->current_weights[i] = mp.current_weights[i];
			this->target_weights[i] = mp.target_weights[i];
		}

		this->fund_description_A_EOM = mp.fund_description_A_EOM;
		this->fund_description_T_EOM = mp.fund_description_T_EOM;
		this->taille_A_EOM = mp.taille_A_EOM;
		this->taille_T_EOM = mp.taille_T_EOM;
		this->switched = mp.switched;
		this->number_of_shares_changed = mp.number_of_shares_changed;
		this->prod_key= mp.prod_key;

		return *this;
	}

	//MP_AP_BOM_no_LD& operator= (MP_AP_BOM& mp)
	//{
	//	this->row = mp.get_row();
	//	this->lapsed_or_dead = mp.get_lapsed_or_dead();
	//	this->taille_A_BOM = mp.get_taille_A_BOM();
	//	this->taille_T_BOM = mp.get_taille_T_BOM();
	//	this->fund_description_A_BOM = mp.get_fund_description_A_BOM();
	//	this->fund_description_T_BOM = mp.get_fund_description_T_BOM();
	//	this->performance = mp.get_performance();
	//	int i;
	//	/*this->current_weights = new double [12];
	//	this->target_weights = new double [12];*/
	//	for (i = 0; i<12; i++)
	//	{
	//		this->current_weights[i] = mp.get_current_weights()[i];
	//		this->target_weights[i] = mp.get_target_weights()[i];
	//	}

	//	this->fund_description_A_EOM.clear();
	//	this->fund_description_T_EOM.clear();
	//	this->taille_A_EOM = 0;
	//	this->taille_T_EOM = 0;
	//	this->switched = false;
	//	this->number_of_shares_changed = false;
	//	this->prod_key=mp.get_prod_key();

	//	return *this;
	//}

	void set_fund_description_EOM (MP_AP_EOM_no_NB mp) /*!!!!! CONST & !!!!!!!*/
	{
		fund_description_A_EOM = mp.get_fund_description_A();
		fund_description_T_EOM = mp.get_fund_description_T();
		taille_A_EOM = mp.get_taille_A();
		taille_T_EOM = mp.get_taille_T();
	}

	void switch_detection (void)
	{
		map<string,fund_for_aging_policies> fund_desc;
		map<string,fund_for_aging_policies>::iterator it;

		if (this->fund_description_A_EOM.empty())
		{
			/* raise ERROR */
		}
		else
		{
			if (this->fund_description_A_EOM.size() != this->get_fund_description_A_BOM().size())
			{
				this->switched = true;
			}
			else
			{
				for (it = this->fund_description_A_EOM.begin();it!=this->fund_description_A_EOM.end();it++)
				{
					fund_desc = this->get_fund_description_A_BOM();
					if (fund_desc.find(it->first) == fund_desc.end())
					{
						this->switched = true;
						break;
					}
				}
			}
		}
	}

	void share_changed_detection (void)
	{
		map<string,fund_for_aging_policies>::iterator it;
		map<string,fund_for_aging_policies> fund_desc;
		if (this->fund_description_A_EOM.empty())
		{
			/* raise ERROR */
		}
		else
		{
			fund_desc = get_fund_description_A_BOM();
			for (it = this->fund_description_A_EOM.begin();it!=this->fund_description_A_EOM.end();it++)
			{
				if (fund_desc.find(it->first)->second.get_number_of_shares() != it->second.get_number_of_shares())
				{
					this->number_of_shares_changed = true;
					break;
				}
			}
		}
	}

	void set_params (const map<string,double>& valo_file,const map<string,vector<double>>& split_file)
	{
		if ((this->switched == false)&&(this->lapsed_or_dead == false))
		{
			int inc = 0;
			int onc = 0;

			double total_valo_0 = 0;
			double total_valo_1 = 0;
			double total_valo_T = 0;

			double perf = 0;
			double CW;
			double TW;

			string name;

			map<string,double>::const_iterator ite;
			map<string,vector<double>>::const_iterator ito;

			/* get total_valo*/
			map<string, fund_for_aging_policies>::iterator it;
			map<string,fund_for_aging_policies> A_BOM;
			map<string,fund_for_aging_policies> A_EOM;
			map<string,fund_for_aging_policies> T_EOM;
			A_BOM = this->get_fund_description_A_BOM();
			A_EOM = this->fund_description_A_EOM;
			T_EOM = this->fund_description_T_EOM;

			double * alpha_0  = new double[A_BOM.size()];
			double * alpha_1A = new double[A_EOM.size()];
			double * alpha_1T = new double[T_EOM.size()];


			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				total_valo_0 += it->second.get_global_valorisation();
			}
			for (it=A_EOM.begin(); it!=A_EOM.end(); it++)
			{
				total_valo_1 += it->second.get_global_valorisation();
			}
			for (it=T_EOM.begin(); it!=T_EOM.end(); it++)
			{
				total_valo_T += it->second.get_number_of_shares(); 
			}
			
			/* get alpha_0, _1A, _1T */
			inc = 0;
			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				alpha_0[inc] = it->second.get_global_valorisation() / total_valo_0;
				inc++;
			}
			inc = 0;
			for (it=A_EOM.begin(); it!=A_EOM.end(); it++)
			{
				alpha_1A[inc] = it->second.get_global_valorisation() / total_valo_1;
				inc++;
			}
			inc = 0;
			for (it=T_EOM.begin(); it!=T_EOM.end(); it++)
			{
				alpha_1T[inc] = it->second.get_number_of_shares()/total_valo_T; //il s'agit bien de la proportion target//
				inc++;
			}
			
			/* initialisation des paramètres */
			
			/*for (onc = 0;onc<12;onc++)
			{
				get_target_weights()[onc] = 0;
				get_current_weights()[onc] = 0;
			}*/

			/* calcul de la performance */
			inc = 0;
			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				//error handle : can't find a profile in the valo_file
				ite = valo_file.find(it->first);
				if (ite==valo_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the valorisation file : can't find a profile in the valorisation file");
					throw new GMXBException(message);
				}

				perf += alpha_0[inc] * (ite->second / it->second.get_valorisation());
				inc++;
			}
			this->set_performance(perf-1);
			
			/* calcul des current weights */
			inc = 0;
			for (it=A_EOM.begin(); it!=A_EOM.end(); it++)
			{
				name = it->first + 'N';
				//error handle : ...
				ito = split_file.find(name);
				if (ito==split_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the split file : can't find a profile in the split file");
					throw new GMXBException(message);
				}
				for (onc = 0;onc<12;onc++)
				{
					CW = get_current_weights()[onc] + alpha_1A[inc] * ito->second[onc];
					set_CW(onc,CW);
				}
				inc++;
			}
			
			/* calcul des target_weights */
			inc = 0;
			for (it=T_EOM.begin();it!=T_EOM.end();it++)
			{
				name = it->first + 'Y';
				//error handle : ...
				ito = split_file.find(name);
				if (ito==split_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the split file : can't find a profile in the split file");
					throw new GMXBException(message);
				}

				for (onc = 0;onc<12;onc++)
				{
					TW = get_target_weights()[onc]+ alpha_1T[inc] * ito->second[onc]*100;
					set_TW(onc,TW);
				}
				inc++;
			}
			delete[]alpha_0;
			delete[]alpha_1A;
			delete[]alpha_1T;
			
		}
		else
		{
			int inc = 0;
			int onc = 0;

			double total_valo_0 = 0;
			double total_valo_1 = 0;

			double perf = 0;
			double CW;
			double TW;

			string name;

			map<string,double>::const_iterator ite;
			map<string,vector<double>>::const_iterator ito;
			map<string,vector<double>>::const_iterator itu;
			
			map<string, fund_for_aging_policies>::iterator it;
			map<string,fund_for_aging_policies> A_BOM = this->get_fund_description_A_BOM();

			double * alpha_0 = new double[A_BOM.size()];
			double * alpha_1A = new double[A_BOM.size()];
			double * alpha_1T = new double[A_BOM.size()];

			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				//error handle : ...

				ite = valo_file.find(it->first);
				if (ite==valo_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the valorisation file : can't find a profile in the valorisation file");
					throw new GMXBException(message);
				}
				total_valo_0 +=it->second.get_global_valorisation();
				total_valo_1 +=it->second.get_global_valorisation() * (ite->second / it->second.get_valorisation());
			}
			
			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				ite = valo_file.find(it->first);
				alpha_0[inc] = it->second.get_global_valorisation() / total_valo_0;
				alpha_1A[inc] = it->second.get_global_valorisation() * (ite->second / it->second.get_valorisation()) / total_valo_1;
				alpha_1T[inc] = it->second.get_global_valorisation() * (ite->second / it->second.get_valorisation()) / total_valo_1;
				inc++;
			}
			
			/*for (onc = 0;onc<12;onc++)
			{
				get_target_weights()[onc] = 0;
				get_current_weights()[onc] = 0;
			}*/
			
			inc = 0;
			for (it=A_BOM.begin(); it!=A_BOM.end(); it++)
			{
				//error handle : ...
				ite = valo_file.find(it->first);
				if (ite==valo_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the valorisation file : can't find a profile in the valorisation file");
					throw new GMXBException(message);
				}
				name = it->first + 'N';
				//error handle : ...

				ito = split_file.find(name);
				if (ito==split_file.end())
				{

					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the split file : can't find a profile in the split file");
					throw new GMXBException(message);
				}
				name = it->first + 'Y';
				//error handle : ...
				itu = split_file.find(name);
				if (itu==split_file.end())
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the input bom inforce file or from the split file : can't find a profile in the split file");
					throw new GMXBException(message);
				}
				perf += alpha_0[inc] * (ite->second / it->second.get_valorisation());
				for (onc = 0;onc<12;onc++)
				{
					CW = get_current_weights()[onc]+ alpha_1A[inc] * ito->second[onc]; 
					TW = get_target_weights()[onc]+ alpha_1T[inc] * itu->second[onc]*100;
					set_CW(onc,CW);
					set_TW(onc,TW);
				}
				inc++;
			}

			this->set_performance(perf-1);
			
			delete[]alpha_0;
			delete[]alpha_1A;
			delete[]alpha_1T;
		};
	}

	bool get_switch (void)
	{
		return this->switched;
	}

	bool get_change_of_shares (void)
	{
		return this->number_of_shares_changed;
	}

};

