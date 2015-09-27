#pragma once

using namespace std;

class Stat_Inforce_Configuration
{
private:

		string stat_inforce_name;
		string inforce_file_name;
		vector<string> v_product_codes_key;
		vector<string> v_gmxb_type;
		string criteria; // Possible values are : All, Both, Product
		string ehc_rrc_weighted_criteria; 
		string output_directory;
		string stat_inforce_file_name;
		string stat_fund_A_file_name;
		string stat_fund_T_file_name;
		bool compare_files;
		string reference_file;
		bool recalculate_ehc;


		void check_stat_inforce_parameters()
		{
			File_Manager f_mgr;
			// Check that the inforce file exist
			f_mgr.is_file_path_valid(this->inforce_file_name);

			// Check the gmxb_type_2 parameter (value should BASE, AV or PREMIUM)
			/*if (this->gmxb_type_2 != "AV" && this->gmxb_type_2 != "BASE" && this->gmxb_type_2 != "PREMIUM")
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Allowed values for 'gmxb_type_2' parameter are : AV, BASE, PREMIUM ");
				throw new GMXBException(message);			
			}*/

			// Check the ehc_rrc_weighted_criteria
			if (this->ehc_rrc_weighted_criteria != "PREM_PAID" && 
					this->ehc_rrc_weighted_criteria != "PREM_CURR" && 
					this->ehc_rrc_weighted_criteria != "AV")
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Allowed values for 'ehc_rrc_weighted_criteria' parameter are : PREM_PAID, PREM_CURR, AV ");
				throw new GMXBException(message);			
			}

			// Check the criteria parameter (value should be ALL, BOTH, PRODUCT)
			if (this->criteria != "ALL" && this->criteria != "BOTH" && this->criteria != "PRODUCT")
			{
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf(message, "Allowed values for 'criteria' parameter are : ALL, BOTH, PRODUCT ");
				throw new GMXBException(message);			
			}

			// Check the output directory
			f_mgr.is_directory_path_valid(this->output_directory);

			// Check the reference file if compare_files option is activated
			if (this->compare_files)
				f_mgr.is_file_path_valid(this->reference_file);

		
		}

public:

	Stat_Inforce_Configuration()
	{
	
	}

	Stat_Inforce_Configuration(const string& stat_inforce_n,
		const string& inforce_file_n,
		const string& gmxb_type,
		const string& crit, 
		const string& ehc_rrc_weighted_crit, 
		const string& output_dir,
		const string& stat_inforce_file_n,
		const string& stat_fund_A_file_n,
		const string& stat_fund_T_file_n,
		bool do_compare_files,
		const string& reference_f, bool recalc_ehc)
	{

		this->stat_inforce_name = stat_inforce_n;
		this->inforce_file_name = inforce_file_n;
		/*this->gmxb_type_2 = gmxb_type;*/
		this->criteria = crit; 
		this->ehc_rrc_weighted_criteria = ehc_rrc_weighted_crit; 
		this->output_directory = output_dir;
		this->stat_inforce_file_name = stat_inforce_file_n;
		this->stat_fund_A_file_name = stat_fund_A_file_n;
		this->stat_fund_T_file_name = stat_fund_T_file_n;
		this->compare_files = do_compare_files;
		this->reference_file = reference_f;
		this->recalculate_ehc = recalc_ehc;

		check_stat_inforce_parameters();

	}

	void add_prod_code_key(const string& pCK)
	{
		this->v_product_codes_key.push_back(pCK);
	}

	void add_gmxb_type(const string& gmxb_type)
	{
		this->v_gmxb_type.push_back(gmxb_type);
	}

	string get_stat_inforce_name()
	{
		return this->stat_inforce_name;
	}

	string get_inforce_file_name()
	{
		return this->inforce_file_name;
	}

	vector<string> get_v_product_codes_key()
	{
		return this->v_product_codes_key;
	
	}

	vector<string> get_v_gmxb_type()
	{
		return this->v_gmxb_type;
	
	}

	/*string get_gmxb_type_2()
	{
		return this->gmxb_type_2;
	}*/

	string get_criteria()
	{
		return this->criteria;
	
	}

	string get_ehc_rrc_weighted_criteria()
	{
		return this->ehc_rrc_weighted_criteria;
	}

	string get_output_directory()
	{
		return this->output_directory;
	
	}

	string get_stat_inforce_file_name()
	{
		return this->stat_inforce_file_name;
	
	}

	string get_stat_fund_A_file_name()
	{
		return this->stat_fund_A_file_name;
	
	}

	string get_stat_fund_T_file_name()
	{
		return this->stat_fund_T_file_name;
	
	}

	bool get_do_compare_files()
	{
		return this->compare_files;
	}

	string get_reference_file()
	{
		return this->reference_file;
	}

	bool get_recalc_ehc_indicator()
	{
		return this->recalculate_ehc;
	}
};