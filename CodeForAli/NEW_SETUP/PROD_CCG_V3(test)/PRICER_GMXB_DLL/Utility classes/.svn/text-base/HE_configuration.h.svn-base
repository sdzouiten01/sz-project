#pragma once
#include "stdafx.h"
#include <string>


using namespace std;

class HE_configuration
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

public :
	int step_scenarios_write[PERIOD];
	int result[14];

	~HE_configuration()
	{
		if (this->output!=0)
		{
			for( int i =0; i< this->size; i++)
				delete[] output[i];
			delete[] output;
		}
	}

	HE_configuration()
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

};