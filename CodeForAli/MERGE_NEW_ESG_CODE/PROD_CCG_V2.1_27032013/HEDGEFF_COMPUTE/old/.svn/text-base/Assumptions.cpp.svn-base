#include "Assumptions.h"

Assumptions::Assumptions(
				int model_period,
				double moneyness,
				int reb_freq_delta,
				int reb_freq_gamma,
				int reb_freq_rho,
				double volatility,
				int length_of_option_contract,
				int sell_at_time_to_expiry,
				int length_of_futures_contract,
				double gamma_per_cont,
				double delta_per_cont,
				double net_gamma_lower_bound,
				double net_gamma_upper_bound,
				double liab_gamma_lower_bound,
				double liab_gamma_upper_bound,
				double delta_transaction_cost,
				double option_transaction_cost,
				double rho_transaction_cost,
				bool  hedge_delta,
				bool  hedge_gamma,
				bool  hedge_rho,
				bool rates_freq_ann,
				bool sheduled_futures,
				int next_time_step_sheduled,
				int nb_steps_per_year,
				int nb_maturities_to_hedge,
				double* maturities_to_hedge,
				double* shocks_to_apply,
				double* compounding_frequencies,
				double index_base,
				//chth221209-2
				double* start_mty,
				double* end_mty
				)
{
	//this->init_fields(file_input_name);	

	//int* p=NULL;
//	int a=(*p);
	this->model_period=model_period;
	this->moneyness=moneyness;
	this->reb_freq_delta=reb_freq_delta;
	this->reb_freq_gamma=reb_freq_gamma;
	this->reb_freq_rho=reb_freq_rho;
	this->volatility=volatility;
	this->length_of_option_contract=length_of_option_contract;
	this->sell_at_time_to_expiry=sell_at_time_to_expiry;
	this->length_of_futures_contract=length_of_futures_contract;
	this->gamma_per_cont=gamma_per_cont;
	this->delta_per_cont=delta_per_cont;
	this->net_gamma_lower_bound=net_gamma_lower_bound;
	this->net_gamma_upper_bound=net_gamma_upper_bound;
	this->liab_gamma_lower_bound=liab_gamma_lower_bound;
	this->liab_gamma_upper_bound=liab_gamma_upper_bound;
	this->delta_transaction_cost=delta_transaction_cost;
	this->rho_transaction_cost=rho_transaction_cost;
	this->option_transaction_cost=option_transaction_cost;
	this->rates_freq_ann=rates_freq_ann;
	this->nb_steps_per_year=nb_steps_per_year;
	this->hedge_gamma=hedge_gamma;
	this->hedge_delta=hedge_delta;
	this->hedge_rho=hedge_rho;
	this->next_time_step_sheduled=next_time_step_sheduled;
	this->sheduled_futures=sheduled_futures;

	this->maturities_to_hedge=maturities_to_hedge,

	this->nb_maturities_to_hedge=nb_maturities_to_hedge;

	this->shocks_to_apply=shocks_to_apply;
	this->compounding_frequencies=compounding_frequencies;
	this->index_base=index_base;

	//chth221209-2
	this->start_mty=start_mty;
	this->end_mty= end_mty;


	//this->shock_size=300;
//	ofstream aspt("Asspt.txt");
	//aspt<<this->moneyness<<endl;
	//aspt<<this->reb_freq_gamma<<endl;
	////aspt<<this->shock_size<<endl;
	//aspt<<this->volatility<<endl;
	//aspt<<this->length_of_option_contract<<endl;
	//aspt<<this->sell_at_time_to_expiry<<endl;
	//aspt<<this->gamma_per_cont<<endl;
	//aspt<<this->delta_per_cont<<endl;
	//aspt<<this->model_period<<endl;
	//aspt<<this->net_gamma_lower_bound<<endl;
	//aspt<<this->net_gamma_upper_bound<<endl;
	//aspt<<this->liab_gamma_lower_bound<<endl;
	//aspt<<this->liab_gamma_upper_bound<<endl;
	//aspt<<this->delta_transaction_cost<<endl;
	//aspt<<this->delta_eq_vol_hist<<endl;
	//aspt<<this->option_transaction_cost<<endl;
	//aspt<<this->hedge_gamma<<endl;

	//aspt.close();
	//this->init_fields(file_input_name);
}

Assumptions::~Assumptions(void)
{
}

//void Assumptions::init_fields(string file_input_name)
//{
//	//cout<<"Opening\n"<<file_input_name<<endl;
//	fstream file_input_stream(file_input_name.c_str());
//	//cout<<"\nOpen!"<<endl<<endl;
//	//cout<<"Reading\n"<<file_input_name<<endl<<endl;
//	string row;
//
//	getline(file_input_stream,row);
//	char * pch;
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->reb_freq_gamma=atof(pch);
//	//cout<<this->reb_freq_gamma<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->shock_size=atof(pch);
//	//cout<<this->shock_size<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->volatility=atof(pch);
//	//cout<<this->volatility<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->length_of_option_contract=atof(pch);
//	//cout<<this->length_of_option_contract<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->sell_at_time_to_expiry=atof(pch);
//	//cout<<this->sell_at_time_to_expiry<<endl;
//
//	getline(file_input_stream,row);
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->gamma_per_cont=atof(pch);
//	//cout<<this->gamma_per_cont<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->delta_per_cont=atof(pch);
//	//cout<<this->delta_per_cont<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
////	this->model_period=atof(pch)+1;
//	//cout<<this->model_period<<endl;
//
//	getline(file_input_stream,row);
//	getline(file_input_stream,row);
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->net_gamma_lower_bound=atof(pch);
//	//cout<<this->net_gamma_lower_bound<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->net_gamma_upper_bound=atof(pch);
//	//cout<<this->net_gamma_upper_bound<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->liab_gamma_lower_bound=atof(pch);
//	//cout<<this->liab_gamma_lower_bound<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->liab_gamma_upper_bound=atof(pch);
//	//cout<<this->liab_gamma_upper_bound<<endl;
//
//	getline(file_input_stream,row);
//	getline(file_input_stream,row);
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->delta_transaction_cost=atof(pch);
//	//cout<<this->delta_transaction_cost<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->delta_eq_vol_hist=atof(pch);
//	//cout<<this->delta_eq_vol_hist<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	this->option_transaction_cost=atof(pch);
//	//cout<<this->option_transaction_cost<<endl;
//
//	getline(file_input_stream,row);
//	pch = strtok ((char*)row.c_str(),",");
//	//cout<<pch<<": ";
//	pch = strtok (NULL, ",");
//	if(strcmp(pch," VRAI ")==0) this->hedge_gamma=true;
//	else this->hedge_gamma=false;
//	//cout<<this->hedge_gamma<<endl;
//
//	//cout<<"\nRead!"<<endl;
//	file_input_stream.close();
//}