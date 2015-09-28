#pragma once
#include <sstream>
#include <string>
#include "stdafx.h"
#include "fund_for_aging_policies.h"


class MP_AP_EOM
{

protected : 

	string row;
	bool new_business;
	string prod_key;

public:

	MP_AP_EOM(void)
	{
	}

	MP_AP_EOM(const string& str)
	{
		int inc;
		inc = 0;
		string chain;
		
		this->row = str;
		this->new_business = false;

		string dosc;
		istringstream line_stream(row.c_str(),istringstream::in);
		inc = 0;
		while(inc<5)
		{
			getline(line_stream,dosc,',');
			inc++;
		}

		this->prod_key = dosc;
	}

	MP_AP_EOM (const MP_AP_EOM& mp)
	{
		this->row = mp.row;
		this->new_business = mp.new_business;
		this->prod_key = mp.prod_key;
	}
	
	~MP_AP_EOM(void)
	{
	}

	MP_AP_EOM& operator= (const MP_AP_EOM& f)
	 {
		 this->row = f.row;
		 this->new_business = f.new_business;
		 this->prod_key = f.prod_key;
		 return *this;
	}


/*set private members functions*/
	
	void set_row (const string& str)
	{
		this->row = str;
	}

	void set_new_business(bool b)
	{
		this-> new_business = b;
	}

	void set_prod_key(const string& str)
	{
		this->prod_key = str;
	}

	
/* get private members functions */
	
	string get_row (void)
	{
		return this->row;
	}

	bool get_new_business (void)
	{
		return this->new_business;
	}

	string get_prod_key (void)
	{
		return this->prod_key;
	}
};

