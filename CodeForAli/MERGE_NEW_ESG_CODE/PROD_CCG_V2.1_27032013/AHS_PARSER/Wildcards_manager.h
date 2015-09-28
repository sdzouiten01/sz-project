#pragma once
#include "stdafx.h"
#include <string>

using namespace std;


class Wildcards_manager
{

private :
	map<string, string> wild_card_list;


	vector<string> get_list_of_wild_card;

public:

	~Wildcards_manager()
	{
	}

	Wildcards_manager()
	{
	}

	map<string, string> get_wild_cards_list()
	{
		return this->wild_card_list;
	
	}

	void get_full_path(const string& wilcard)
	{
	
	}
		

};