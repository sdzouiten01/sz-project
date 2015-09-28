#pragma once
#include <string>
#include <istream>
#include <sstream>
#include "stdafx.h"
#include "stdlib.h"

using namespace std;



class fund_for_aging_policies
{
private : 

	double valorisation;
	double number_of_shares;
	double global_valorisation;

public:
	
	fund_for_aging_policies(void)
	{
		valorisation = 0.0;
		global_valorisation = 0.0;
		number_of_shares = 0.0;
	}
	
	// inutile ???
	//fund_for_aging_policies(const string& str)
	//{

	//	char * chain;
	//	char * desc = (char *)str.c_str();

	//	//error handle : ...
	//	try
	//	{
	//		chain = strtok(desc,";");
	//		this->number_of_shares = atof(chain);

	//		chain = strtok(NULL,";");
	//		this->valorisation = atof(chain);

	//		chain = strtok(NULL,";");
	//		this->global_valorisation = atof(chain);
	//	}
	//	catch (const exception& e)
	//	{
	//		throw new ap_exception ();
	//	}

	//}

	fund_for_aging_policies(const fund_for_aging_policies& fund)
	{
		this->number_of_shares = fund.number_of_shares;
		this->valorisation = fund.valorisation;
		this->global_valorisation = fund.global_valorisation;	
	}

	 fund_for_aging_policies& operator= (const fund_for_aging_policies& f)
	 {
		 this->valorisation = f.valorisation;
		 this->global_valorisation = f.global_valorisation;
		 this->number_of_shares = f.number_of_shares;
		 return *this;

	 }

	
	~fund_for_aging_policies(void)
	{
	}

	/*set private members functions*/

	void set_valorisation (double num) {this->valorisation = num;}

	void set_number_of_shares (double num) {this->number_of_shares = num;}

	void set_global_valorisation (double num) {this->global_valorisation = num;}

	/*get private members functions*/

	double get_valorisation (void)
	{
		return this->valorisation;
	}

	double get_number_of_shares (void)
	{
		return this->number_of_shares;
	}

	double get_global_valorisation (void)
	{
		return this->global_valorisation;
	}
};
