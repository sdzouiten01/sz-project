#pragma once
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "stdafx.h"
#include "fund_for_aging_policies.h"
#include "MP_AP_EOM.h"


class MP_AP_EOM_no_NB : public MP_AP_EOM
{

private : 
	
	map<string, fund_for_aging_policies> fund_description_A;
	map<string, fund_for_aging_policies> fund_description_T;
	int taille_A;
	int taille_T;

public:
	
	MP_AP_EOM_no_NB(void)
	{
	}

	MP_AP_EOM_no_NB (const MP_AP_EOM_no_NB& mp) : MP_AP_EOM(mp)
	{
		this->fund_description_A = mp.fund_description_A;
		this->fund_description_T = mp.fund_description_T;
		this->taille_A = mp.taille_A;
		this->taille_T = mp.taille_T;
	}

	//MP_AP_EOM_no_NB (MP_AP_EOM& mp) : MP_AP_EOM(mp)
	//{
	//	this->set_new_business(false);
	//}
	
	~MP_AP_EOM_no_NB(void)
	{
	}

	MP_AP_EOM_no_NB (string & str) : MP_AP_EOM(str)
	{
		taille_A = 0;
		taille_T = 0;
	}
	
	MP_AP_EOM_no_NB& operator= (const MP_AP_EOM_no_NB& mp)
	{
		this->row = mp.row;
		this->new_business = mp.new_business;
		this->prod_key = mp.prod_key;
		this->fund_description_A = mp.fund_description_A;
		this->fund_description_T = mp.fund_description_T;
		this->taille_A = mp.taille_A;
		this->taille_T = mp.taille_T;
		return *this;
	}

	//MP_AP_EOM_no_NB& operator= (MP_AP_EOM& mp)
	//{
	//	this->row = mp.get_row();
	//	this->new_business = mp.get_new_business();
	//	this->fund_description_A.clear();
	//	this->fund_description_T.clear();
	//	this->taille_A = 0;
	//	this->taille_T = 0;
	//	return *this;
	//}

	void set_fund_description (const string & row)
	{
		int inc;
		int nbCol=0;
		string dosc;
		string str1;
		string utl;
		string name;

		this->set_row(row);

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
			inc++;
		}
		//error handle : fund_desc is empty
		if (dosc == "")
		{

			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "error from the EOM inforce input file. Check the fund_desc column");
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
			catch (const exception& e)
			{
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "error from the EOM inforce input file. Check the fund description column %s", e.what());
					throw new GMXBException(message);
			}

			if (utl=="A")
			{
				this->fund_description_A.insert(pair<string,fund_for_aging_policies>(name,fund));
			}
			else 
			{
				if (utl=="T")
				{
					this->fund_description_T.insert(pair<string,fund_for_aging_policies>(name,fund));
				}
				else
				{
					this->fund_description_A.insert(pair<string,fund_for_aging_policies>(name,fund));
					this->fund_description_T.insert(pair<string,fund_for_aging_policies>(name,fund));
				}
			}
		}
		this->taille_A = this->fund_description_A.size();
		this->taille_T = this->fund_description_T.size();
	}
/* get members*/

	int get_taille_A(void)
	{
		return this->taille_A;
	}

	int get_taille_T(void)
	{
		return this->taille_T;
	}

	map<string,fund_for_aging_policies> get_fund_description_A (void)
	{
		return this->fund_description_A;
	}

	map<string,fund_for_aging_policies> get_fund_description_T (void)
	{
		return this->fund_description_T;
	}


};