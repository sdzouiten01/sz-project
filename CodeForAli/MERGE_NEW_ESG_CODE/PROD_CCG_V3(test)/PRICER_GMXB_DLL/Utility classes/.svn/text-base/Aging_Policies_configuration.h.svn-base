#pragma once
#include "stdafx.h"
#include <string>


using namespace std;

class Aging_Policies_configuration
{

private :

	string ** output; 
	int size; 
	string output_path_file;
	string inforce_in_path_file; 
	string inforce_out_path_file; 
	string calibration_file_path;
	string inforce_in_file_name; 
	string inforce_out_file_name; 
	string calibration_file_name;
	int write_output_file;
	int write_inforce_in_file;
	int write_inforce_out_file;
	int nb_inforce_inner;
	string inter_result_file_path;
	string result_file_path;
	string result_file_name;
	string result_inter_file_name;
	int he_mode;
	int shock_size;
	int use_scenario_file_inner;
	int output_calibration_parameters;
	int output_inner_pvs;
	string inner_pvs_file_path;
	string inner_pvs_file_name;

	//AP modification

	string inforce_bom_prod;
	string inforce_eom_prod;
	string fund_split_eom;
	string fund_valo_eom;

	string eom_date;
	string bom_date;
	double lapse_multiplier;
	double death_rate_multiplier;
	double charges_multiplier;
	double duration_age_step;
	int mode;

	string inforce_eom_prod_no_nb;
	string inforce_eom_model;
	string inforce_new_business;
	string temporary_directory;
	vector<string> products;
	vector<string> bom_s;
	vector<string> cw_s;
	vector<string> perfs_s;
	vector<string> tw_s;
	vector<string> eom_no_nb_s;
	vector<string> new_business_s;
	vector<string> eom_model_s;




public :
	
	int result[14];

	~Aging_Policies_configuration()
	{
		if (this->output!=0)
		{
			for( int i =0; i< this->size; i++)
				delete[] output[i];
			delete[] output;
		}
	}

	Aging_Policies_configuration()
	{
		this->output=0;
	}

	int get_size()
	{
		return this->size;
	}

	int get_write_output_file()
	{
		return this->write_output_file;
	}

	int get_write_inforce_out_file()
	{
		return this->write_inforce_out_file;
	}

	int get_write_inforce_in_file()
	{
		return this->write_inforce_in_file;
	}

	int get_nb_inforce_inner()
	{
		return this->nb_inforce_inner;
	}

	string get_output(int line, int col)
	{
		return this->output[line][col];
	}

	string get_inforce_out_path_file(){
		return this->inforce_out_path_file;
	}
	
	string get_inforce_in_path_file(){
		return this->inforce_in_path_file;
	}

	string get_calibration_file_path(){
		return this->calibration_file_path;
	}
		
	string get_inforce_out_file_name(){
		return this->inforce_out_file_name;
	}
	
	string get_inforce_in_file_name(){
		return this->inforce_in_file_name;
	}

	string get_calibration_file_name(){
		return this->calibration_file_name;
	}

	string get_output_path_file(){
		return this->output_path_file;
	}
		
	string get_result_file_path(){
		return this->result_file_path;
	}

	string get_inter_result_file_path(){
		return this->inter_result_file_path;
	}

	string get_result_file_name(){
		return this->result_file_name;
	}

	string get_result_inter_file_name(){
		return this->result_inter_file_name;
	}

	int get_shock_size()
	{
		return this->shock_size;
	}

	int get_he_mode()
	{
		return this->he_mode;
	}

	int get_output_inner_pvs(){
		return this->output_inner_pvs;
	}

	string get_inner_pvs_file_path(){
		return this->inner_pvs_file_path;
	}

	string get_inner_pvs_file_name(){
		return this->inner_pvs_file_name;
	}


	void set_result_file_path(string s){
		this->result_file_path=s;
	}
	
	void set_inter_result_file_path(string s){
		this->inter_result_file_path=s;
	}


	void set_result_file_name(string s){
		this->result_file_name=s;
	}

	void set_result_inter_file_name(string s){
		this->result_inter_file_name=s;
	}

	void set_output_inner_pvs(int i){
		this->output_inner_pvs = i;
	}

	void set_inner_pvs_file_path(string s){
		this->inner_pvs_file_path=s;
	}

	void set_inner_pvs_file_name(string s){
		this->inner_pvs_file_name=s;
	}


	void set_size(int i)
	{
		this->size=i;
	}

	void set_nb_inforce_inner(int i)
	{
		this->nb_inforce_inner=i;
	}

	void set_write_output_file(int i)
	{
		this->write_output_file=i;
	}

	void set_write_inforce_out_file(int i)
	{
		this->write_inforce_out_file=i;
	}

	void set_write_inforce_in_file(int i)
	{
		this->write_inforce_in_file=i;
	}

	void set_calibration_file_path(string path)
	{
		this->calibration_file_path = path;
	}

	void set_calibration_file_name(string file_name)
	{
		this->calibration_file_name = file_name;
	}

	void set_output(int line, int col, string val)
	{
		this->output[line][col]=val;
	}

	void init_output(int nb_line, int nb_col)
	{
		this->output=new string*[nb_line];
		for (int i=0; i<nb_line;i++)
			this->output[i]=new string[nb_col];
	}

	void set_output_path_file(string s){
		this->output_path_file=s;
	}

	void set_inforce_out_path_file(string s){
		this->inforce_out_path_file=s;
	}

	void set_inforce_in_path_file(string s){
		this->inforce_in_path_file=s;
	}
		
	void set_inforce_out_file_name(string s){
		this->inforce_out_file_name=s;
	}
	
	void set_inforce_in_file_name(string s){
		this->inforce_in_file_name=s;
	}

	void set_shock_size(int i)
	{
		this->shock_size=i;
	}

	void set_he_mode(int i){
		this->he_mode=i;
	}

	int get_use_scenario_file_inner()
	{
		return this->use_scenario_file_inner;
	}


	void set_use_scenario_file_inner(int i){
		this->use_scenario_file_inner=i;
	}

	int get_output_calibration_parameters()
	{
		return this->output_calibration_parameters;
	}

	void set_output_calibration_parameters(int indicator)
	{
		
		this->output_calibration_parameters = indicator;
	}

	// AP - modification for dates
	void set_bom_date(string date)
	{
		this->bom_date = date;
	}

	void set_eom_date(string date)
	{
		this->eom_date = date;
	}

	// AP - multipliers
	void set_lapse_multiplier(string lapse_multiplier)
	{
		this->lapse_multiplier = atof(lapse_multiplier.c_str());
	}

	void set_death_rate_multiplier(string death_rate_multiplier)
	{
		this->death_rate_multiplier = atof(death_rate_multiplier.c_str());
	}

	void set_charges_multiplier(string charges_multiplier)
	{
		this->charges_multiplier = atof(charges_multiplier.c_str());
	}

	double get_lapse_multiplier()
	{
		return this->lapse_multiplier;
	}

	double get_death_rate_multiplier()
	{
		return this->death_rate_multiplier;
	}

	double get_charges_multiplier()
	{
		return this->charges_multiplier;
	}

	double get_duration_age_step()
	{
		return this->duration_age_step;
	}

	//AP modif;

	string get_bom_date (void)
	{
		return this->bom_date;
	}

	string get_eom_date (void)
	{
		return this->eom_date;
	}

	string get_bom_prod (void)
	{
		return this->inforce_bom_prod;
	}
	string get_eom_prod (void)
	{
		return this->inforce_eom_prod;
	}
	string get_fund_split_eom (void)
	{
		return this->fund_split_eom;
	}
	string get_fund_valo_eom (void)
	{
		return this->fund_valo_eom;
	}
	int get_mode(void)
	{
		return this->mode;
	}
	string get_eom_prod_no_nb(void)
	{
		return this->inforce_eom_prod_no_nb;
	}
	string get_eom_model(void)
	{
		return this->inforce_eom_model;
	}

	string get_new_business (void)
	{
		return this->inforce_new_business;
	}

	string get_temporary_directory (void)
	{
		return this->temporary_directory;
	}

	vector<string> get_products (void)
	{
		return this->products;
	}
	
	void set_bom_prod (string str)
	{
		this->inforce_bom_prod = str;
	}
	void set_eom_prod (string str)
	{
		this->inforce_eom_prod = str;
	}
	void set_fund_split_eom (string str)
	{
		this->fund_split_eom = str;
	}
	void set_fund_valo_eom (string str)
	{
		this->fund_valo_eom = str;
	}
	void set_mode(int m)
	{
		this->mode = m;
	}
	void set_eom_prod_no_nb(string str)
	{
		this->inforce_eom_prod_no_nb = str;
	}
	void set_eom_model(string str)
	{
		this->inforce_eom_model = str;
	}
	void set_new_business (string str)
	{
		this->inforce_new_business = str;
	}
	void set_temporary_directory (string str)
	{
		this->temporary_directory = str;
	}

	void set_products (string str)
	{
		string nm;
		istringstream line_stream (istringstream::in);
		line_stream.str(str.c_str());
		while(!line_stream.eof())
		{
			getline(line_stream,nm,',');
			this->products.push_back(nm);
		}
	}
	void add_temp_pack(const string& bom, 
		const string& perfs, const string& cw, const string& tw,
		const string& eom_no_nb, const string& new_business, const string& eom_model)
	{
		this->bom_s.push_back(bom);
		this->cw_s.push_back(cw);
		this->perfs_s.push_back(perfs);
		this->tw_s.push_back(tw);
		this->eom_no_nb_s.push_back(eom_no_nb);
		this->new_business_s.push_back(new_business);
		this->eom_model_s.push_back(eom_model);
	}

	vector<string> get_bom_s(void)
	{
		return this->bom_s;
	}
	vector<string> get_cw_s(void)
	{
		return this->cw_s;
	}
	vector<string> get_perfs_s(void)
	{
		return this->perfs_s;
	}
	vector<string> get_tw_s(void)
	{
		return this->tw_s;
	}
	vector<string> get_eom_no_nb_s(void)
	{
		return this->eom_no_nb_s;
	}
	vector<string> get_new_business_s(void)
	{
		return this->new_business_s;
	}
	vector<string> get_eom_model_s(void)
	{
		return this->eom_model_s;
	}

	void set_duration_age_step(void)
	{
		istringstream line_stream (istringstream::in);
		string str;

		line_stream.str(this->get_eom_date().c_str());
		getline(line_stream,str,'/');
		int d_eom = atoi(str.c_str());
		getline(line_stream,str,'/');
		int m_eom = atoi(str.c_str());
		getline(line_stream,str,'/');
		int y_eom = atoi(str.c_str());
		line_stream.clear();

		line_stream.str(this->get_bom_date().c_str());
		getline(line_stream,str,'/');
		int d_bom = atoi(str.c_str());
		getline(line_stream,str,'/');
		int m_bom = atoi(str.c_str());
		getline(line_stream,str,'/');
		int y_bom = atoi(str.c_str());
		line_stream.clear();

		double * year = new double [12];
		int i;
		year[0] = 31;
		year[1] = 28;
		year[2] = 31;
		year[3] = 30;
		year[4] = 31;
		year[5] = 30;
		year[6] = 31;
		year[7] = 31;
		year[8] = 30;
		year[9] = 31;
		year[10] = 30;
		year[11] = 31;
		double e_m = 0;
		double b_m = 0;
		for (i=0;i<m_eom-1;i++)
		{
			e_m += year[i];
		}
		for (i=0;i<m_bom-1;i++)
		{
			b_m += year[i];
		}
		delete [] year;
		this->duration_age_step = (d_eom + e_m + y_eom*365 - d_bom - b_m - y_bom*365)/365;
	}
	
};