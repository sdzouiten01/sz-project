#pragma once
#include "stdafx.h"
#include <string>
#include "Utility classes/Product_configuration.h"

using namespace std;

class Summary_configuration
{

private :
	vector<Product_configuration> v_product_configuration;
	/*
	* Hedge effectiveness
	* v_product_configuration_in : contains the inner products configurations
	* v_product_configuration_out : contains the outer products configurations
	*/
	vector<Product_configuration> v_product_configuration_in;
	vector<Product_configuration> v_product_configuration_out;
	
	// Indicates which type of
	// output should be created
	bool write_ehc_format;
	bool write_policy_by_policy_format;
	bool write_total_format;
	bool write_scen_by_scen_format;
	bool write_mso_format;

	// Path of the outputs
	string ehc_file_directory;
	string policy_by_policy_file_directory;
	string total_file_directory;
	string scen_by_scen_file_directory;
	string mso_file_directory;

	// Output File name 
	string ehc_file_name;
	string policy_by_policy_file_name;
	string total_file_name;
	string ehc_sensi_file_directory;
	string ehc_sensi_file_name;
	string scen_by_scen_file_name;
	string mso_file_name;

	// Scenario id 
	// for ehc calculation
	int ehc_scen_pass_number;

	string he_file_path;
	string he_file_name;
	bool recalculate_ehc;

public :

	~Summary_configuration()
	{
	}

	/*
	* Constructor for the hedge effectiveness
	*/
	Summary_configuration(const string& hedge_eff_file_path, const string& hedge_eff_file_name)
	{
		this->he_file_path = hedge_eff_file_path;
		this->he_file_name = hedge_eff_file_name;
	}
	Summary_configuration(bool ehc_format, const string& ehc_directory, const string & ehc_name,
						  bool policy_by_policy_format, const string& policy_by_policy_directory, const string& policy_by_policy_name,
						  bool scen_by_scen_format, const string& scen_by_scen_directory, const string& scen_by_scen_name,
						  bool total_format, const string& total_directory, const string& total_name,
						  int ehc_scn_pass_nb, const string& ehc_sen_file_d, const string& ehc_sen_file_n,
						  bool recalc_ehc,bool mso_format, const string& mso_directory, const string& mso_file)
	{
		this->write_ehc_format = ehc_format;
		this->write_policy_by_policy_format = policy_by_policy_format;
		this->write_total_format = total_format;
		this->write_scen_by_scen_format = scen_by_scen_format;
		this->write_mso_format=mso_format;

		this->ehc_file_directory = ehc_directory;
		this->policy_by_policy_file_directory = policy_by_policy_directory;
		this->total_file_directory = total_directory;
		this->scen_by_scen_file_directory = scen_by_scen_directory;
		this->mso_file_directory = mso_directory;

		this->ehc_file_name =  ehc_name;
		this->policy_by_policy_file_name = policy_by_policy_name;
		this->total_file_name = total_name;
		this->ehc_scen_pass_number = ehc_scn_pass_nb;
		this->ehc_sensi_file_directory = ehc_sen_file_d;
		this->ehc_sensi_file_name = ehc_sen_file_n;
		this->recalculate_ehc = recalc_ehc;
		this->scen_by_scen_file_name = scen_by_scen_name;
		this->mso_file_name=mso_file;
	}

	void add_product_configuration(Product_configuration& conf)
	{
		this->v_product_configuration.push_back(conf);
	}

	// HE part 
	void add_product_configuration_in(Product_configuration& conf)
	{
		this->v_product_configuration_in.push_back(conf);
	}

	void add_product_configuration_out(Product_configuration& conf)
	{
		this->v_product_configuration_out.push_back(conf);
	}

	vector<Product_configuration>& get_v_product_out()
	{
		return this->v_product_configuration_out;
	}

	vector<Product_configuration>& get_v_product_in()
	{
		return this->v_product_configuration_in;
	}
	vector<Product_configuration> get_product_configurations()
	{
		return this->v_product_configuration;
	}

	bool get_ehc_format_indicator()
	{
		return this->write_ehc_format;
	}

	bool get_policy_by_policy_indicator()
	{
		return this->write_policy_by_policy_format;
	}

	bool get_scen_by_scen_indicator()
	{
		return this->write_scen_by_scen_format;
	}
	bool get_total_file_indicator()
	{
		return this->write_total_format;
	}

	string get_ehc_file_directory()
	{
		return this->ehc_file_directory;
	}

	string get_policy_by_policy_file_directory()
	{
		return this->policy_by_policy_file_directory;
	}

	string get_scen_by_scen_file_directory()
	{
		return this->scen_by_scen_file_directory;
	}
	string get_total_file_directory()
	{
		return this->total_file_directory;
	}

	string get_ehc_file_name()
	{
		return this->ehc_file_name;
	}

	string get_policy_by_policy_file_name()
	{
		return this->policy_by_policy_file_name;
	}

	string get_scen_by_scen_file_name()
	{
		return this->scen_by_scen_file_name;
	}

	string get_total_file_name()
	{
		return this->total_file_name;
	}

	int get_ehc_scen_pass_num()
	{
		return this->ehc_scen_pass_number;	
	}

	string get_ehc_sensi_file_directory()
	{
		return this->ehc_sensi_file_directory;
	}

	string get_ehc_sensi_file_name()
	{
		return this->ehc_sensi_file_name;
	
	}

	string get_he_file_path()
	{
		return this->he_file_path;
	}

	string get_he_file_name()
	{		
	return this->he_file_name;
	}
	
	
	bool get_recalculate_param_indicator()
	{
		return this->recalculate_ehc;
	}

	void RA_reset()
	{
	  for (int i=0;i<this->v_product_configuration.size();i++)
	  {
		  v_product_configuration[i].RA_set_run_name();
	  }
	  this->ehc_file_name ="RA_"+this->ehc_file_name;
	  this->policy_by_policy_file_name ="RA_"+ this->policy_by_policy_file_name;
	  this->total_file_name = "RA_"+this->total_file_name;
	  this->ehc_sensi_file_name = "RA_"+this->ehc_sensi_file_name;
	  this->mso_file_name="RA_"+this->mso_file_name;

	}

	bool get_mso_file_indicator()
	{
        return this->write_mso_format;	
	}

	string get_mso_file_directory()
	{
		return this->mso_file_directory;
	}

	string get_mso_file_name()
	{
		return this->mso_file_name;
	}
};
	