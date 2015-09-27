#pragma once
#include <map>
#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include "stdafx.h"
#include "GMXBException.h"
#include "fund_for_aging_policies.h"
#include <cmath>

class MP_AP_BOM
{

protected : 
	
	string row;
	string prod_key;
	
	bool lapsed_or_dead;

	map<string, fund_for_aging_policies> fund_description_A_BOM;
	map<string, fund_for_aging_policies> fund_description_T_BOM;
	int taille_A_BOM;
	int taille_T_BOM;

	double performance;
	double * current_weights;
	double * target_weights;

public:
	
	MP_AP_BOM(void)
	{
		this->current_weights = new double [12];
		this->target_weights = new double [12];
		performance = 0;
		int i;
		for(i=0;i<12;i++)
		{
			this->current_weights[i] = 0;
			this->target_weights[i] = 0;
		}
		lapsed_or_dead = true;
		prod_key = "";
	}
	
	MP_AP_BOM(const MP_AP_BOM& mp)
	{
		this->row = mp.row;
		this->lapsed_or_dead = mp.lapsed_or_dead;
		this->taille_A_BOM = mp.taille_A_BOM;
		this->taille_T_BOM = mp.taille_T_BOM;
		this->fund_description_A_BOM = mp.fund_description_A_BOM;
		this->fund_description_T_BOM = mp.fund_description_T_BOM;
		this->performance = mp.performance;
		int i;
		this->current_weights = new double [12];
		this->target_weights = new double [12];
		for (i = 0; i<12; i++)
		{
			this->current_weights[i] = mp.current_weights[i];
			this->target_weights[i] = mp.target_weights[i];
		}
		this->prod_key=mp.prod_key;
	}

	
	MP_AP_BOM(const string& row)
	{

		int inc;
		int nbCol=0;
		string dosc;
		string str1;
		string utl;
		string name;
		this->performance=0;
		this->current_weights = new double [12];
		this->target_weights = new double [12];
		for(inc = 0;inc<12;inc++)
		{
			this->current_weights[inc] = 0;
			this->target_weights[inc] = 0;
		}
		this->row = row;

		/* récuprération de la case fund_description*/
		istringstream line_stream(row.c_str(),istringstream::in);
		inc = 0;
		istringstream row_streamed;
		string element;
		row_streamed.str(row);
		while (getline(row_streamed,element,','))
			{
				nbCol++;
			}
		int nrd=nbCol;


		while(inc<nrd)
		{
			getline(line_stream,dosc,',');
			if(inc==4)
			{
				this->prod_key=dosc;
				//error handle : prod_key is empty
				if (dosc == "")
				{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the BOM inforce input file. Check the prod_key column");
					throw new GMXBException(message);	
				}
			}
			inc++;
		}
		//error handle : fund_desc is empty
		if (dosc == "")
		{
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "error from the BOM inforce input file. Check the fund_desc column");
			throw new GMXBException(message);	
		}

		istringstream desc_stream (dosc.c_str(),istringstream::in);
		istringstream  fund_desc (istringstream::in);
		fund_for_aging_policies fund;
		while (!desc_stream.eof())
		{
			getline(desc_stream,str1,'*');
			fund_desc.clear();
			//error handle : ...
			try
			{
				fund_desc.str(str1.c_str());

				getline(fund_desc,name,';');
				getline(fund_desc,utl,';');
				fund.set_number_of_shares(atof(utl.c_str()));
				getline(fund_desc,utl,';');
				fund.set_valorisation(atof(utl.c_str()));
				getline(fund_desc,utl,';');
				fund.set_global_valorisation(atof(utl.c_str()));
				
				getline(fund_desc,utl,';');
			}
			catch(const exception& e)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "error from the BOM inforce input file. Check the fund descritption column %s", e.what());
				throw new GMXBException(message);	

			}

			if (utl=="A")
			{
				this->fund_description_A_BOM.insert(pair<string,fund_for_aging_policies>(name,fund));
			}
			else 
			{
				if (utl=="T")
				{
					this->fund_description_T_BOM.insert(pair<string,fund_for_aging_policies>(name,fund));
				}
				else
				{
					this->fund_description_A_BOM.insert(pair<string,fund_for_aging_policies>(name,fund));
					this->fund_description_T_BOM.insert(pair<string,fund_for_aging_policies>(name,fund));
				}
			}

		}
		this->taille_A_BOM = this->fund_description_A_BOM.size();
		this->taille_T_BOM = this->fund_description_T_BOM.size();

	}

	
	~MP_AP_BOM(void)
	{
		delete [] this->current_weights;
		delete [] this->target_weights;
	}

	MP_AP_BOM& operator= (const MP_AP_BOM& mp)
	{
		this->row = mp.row;
		this->lapsed_or_dead = mp.lapsed_or_dead;
		this->taille_A_BOM = mp.taille_A_BOM;
		this->taille_T_BOM = mp.taille_T_BOM;
		this->fund_description_A_BOM = mp.fund_description_A_BOM;
		this->fund_description_T_BOM = mp.fund_description_T_BOM;
		this->performance = mp.performance;
		int i;

		for (i = 0; i<12; i++)
		{
			this->current_weights[i] = mp.current_weights[i];
			this->target_weights[i] = mp.target_weights[i];
		}
		this->prod_key=mp.prod_key;
		return *this;
	}


/*set private members functions*/
	
	void set_row (const string& str)
	{
		this->row = str;
	}

	void set_lapsed_or_dead (bool b)
	{
		this->lapsed_or_dead = b;
	}

	void set_performance (double num)
	{
		this->performance = num;
	}

	void set_prod_key (string str)
	{
		this->prod_key=str;
	}

	void set_params (const map<string,double>& valo_file,const map<string,vector<double>>& split_file)
	{
		
		double * alpha_0 = new double[this->fund_description_A_BOM.size()];
		double * alpha_1A = new double[this->fund_description_A_BOM.size()];
		double * alpha_1T = new double[this->fund_description_A_BOM.size()];

		int inc = 0;
		int onc = 0;

		double total_valo_0 = 0;
		double total_valo_1 = 0;

		string name;

		map<string, fund_for_aging_policies>::iterator it;
		fund_for_aging_policies * fund = new fund_for_aging_policies;
		double valo;
		for (it=this->fund_description_A_BOM.begin(); it!=this->fund_description_A_BOM.end(); it++)
		{
			*fund = it->second;
			//error handle : profile names don't match (valo file and bom inforce input)
			try
			{
				valo = valo_file.find(it->first)->second; 
			}
			catch(const exception& e)
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "at least one of the profile name between the bom inforce input file and the valorisation file doesn't match %s", e.what());
				throw new GMXBException(message);	


			}

			total_valo_0 += fund->get_global_valorisation();
			total_valo_1 += fund->get_global_valorisation() * (valo / (fund->get_valorisation()));
		};
		delete fund;
		
		for (it=this->fund_description_A_BOM.begin(); it!=this->fund_description_A_BOM.end(); it++)
		{
			alpha_0[inc] = it->second.get_global_valorisation() / total_valo_0;
			alpha_1A[inc] = it->second.get_global_valorisation() * (valo_file.find(it->first)->second / it->second.get_valorisation()) / total_valo_1;
			alpha_1T[inc] = it->second.get_global_valorisation() * (valo_file.find(it->first)->second / it->second.get_valorisation()) / total_valo_1;
			inc++;
		}
		
		this->performance = 0;
		for (onc = 0; onc<12; onc++)
		{
			target_weights[onc] = 0;
			current_weights[onc] = 0;
		}

		inc = 0;
		
		for (it=this->fund_description_A_BOM.begin(); it!=this->fund_description_A_BOM.end(); it++)
		{
			
			
			performance += alpha_0[inc] * (valo_file.find(it->first)->second / it->second.get_valorisation());
			for (onc = 0;onc<12;onc++)
			{
				name = it->first+'N';
				//error handle : can't find a profile in the split_file
				try
				{
					this->current_weights[onc] += alpha_1A[inc] * split_file.find(name)->second[onc];
				}
				catch(const exception& e)
				{

					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the BOM inforce input file or from the split file : can't find a profile in the split file %s", e.what());
					throw new GMXBException(message);
				}
				name = it->first+'Y';
				//error handle : ...
				try
				{
					this->target_weights[onc] += alpha_1T[inc] * split_file.find(name)->second[onc]*100;
				}
				catch(const exception& e)
				{

					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the BOM inforce input file or from the split file : can't find a profile in the split file %s", e.what());
					throw new GMXBException(message);
				}
			}
			inc++;
		}

		this->performance+= (-1);
		
		delete[]alpha_0;
		delete[]alpha_1A;
		delete[]alpha_1T;
	}
		

/* get private members functions */
	
	string get_row (void)
	{
		return this->row;
	}

	string get_prod_key (void)
	{
		return this->prod_key;
	}

	bool get_lapsed_or_dead (void)
	{
		return this->lapsed_or_dead;
	}
	
	double get_performance (void)
	{
		return this->performance;
	}

	double * get_current_weights (void)
	{
		return this->current_weights;
	}

	double * get_target_weights (void)
	{
		return this->target_weights;
	}

	int get_taille_A_BOM (void)
	{
		return this->taille_A_BOM;
	}

	int get_taille_T_BOM (void)
	{
		return this->taille_T_BOM;
	}

	map<string,fund_for_aging_policies> get_fund_description_A_BOM (void)
	{
		return this->fund_description_A_BOM;
	}

	map<string,fund_for_aging_policies> get_fund_description_T_BOM (void)
	{
		return this->fund_description_T_BOM;
	}

	/* set private members functions */

	void set_row (string& str)
	{
		this->row = str;
	}

	void set_fund_description_A_BOM (map<string,fund_for_aging_policies>& fund_desc)
	{
		this->fund_description_A_BOM = fund_desc;
	}

	void set_fund_description_T_BOM (map<string,fund_for_aging_policies>& fund_desc)
	{
		this->fund_description_T_BOM = fund_desc;
	}
	
	void set_taille_A_BOM (int n)
	{
		this->taille_A_BOM = n;
	}

	void set_taille_T_BOM (int n)
	{
		this->taille_T_BOM = n;
	}

	void set_CW(int i,double num)
	{
		this->current_weights[i] = num;
	}

	void set_TW(int i,double num)
	{
		this->target_weights[i] = num;
	}
};
