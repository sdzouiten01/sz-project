#pragma once
#ifndef EXPORT_H
#define EXPORT_H
#include <direct.h> // for getcwd
#include <stdlib.h>// for MAX_PATH
#include <iostream> // for //cout and cin
#include <string>
#include <fstream>
#include "Launcher.h"
using namespace std;
/**
* Used to export some C++ functions to C functions. Thus these functions can be used by C Sharp Code.
*/
Launcher* launcher;
Launcher* launcher2;
//void GetCurrentPath(char* buffer)
//{
//	getcwd(buffer, _MAX_PATH);
//}
extern "C" __declspec(dllexport) void Grid_Trans_Costs_Gamma_Net_Gamma_Bounds(
	int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
	double moneyness,
	int reb_freq_delta,
	int reb_freq_gamma,
	double volatility,
	int length_of_option_contract,
	int sell_at_time_to_expiry,
	int length_of_futures_contract,
	double gamma_per_cont,
	double delta_per_cont,
	double from_net_gamma_lower_bound,
	double to_net_gamma_upper_bound,
	double nb_net_gamma_bound,
	double liab_gamma_lower_bound,
	double liab_gamma_upper_bound,
	double delta_transaction_cost,
	double delta_eq_vol_hist,
	double lower_option_transaction_cost,
	double upper_option_transaction_cost,
	double nb_option_transaction_cost,
	char*  hedge_gamma,
	bool rates_freq_ann,
	bool sheduled_futures,
	int next_time_step_sheduled,
	int nb_steps_per_year,
	char* name_of_output)
{
	////ofstream gg("gggggg.txt");
	////gg<<lower_option_transaction_cost<<endl;
	////gg<<upper_option_transaction_cost<<endl;
	////gg<<nb_option_transaction_cost<<endl;
	////gg.close();
	//ofstream output(name_of_output);
	//double opt_trans_cost;
	//double new_net_gamma_lower_bound;
	//double new_net_gamma_upper_bound;
	//for(int j=0;j<=nb_option_transaction_cost;++j)
	//{
	//	opt_trans_cost= lower_option_transaction_cost+(double)j*(upper_option_transaction_cost-lower_option_transaction_cost)/nb_option_transaction_cost;
	//	output<<","<<opt_trans_cost;
	//}
	//output<<endl;
	//for(int i=0;i<=nb_net_gamma_bound;++i)
	//{	
	//	output<<2*from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound;
	//	new_net_gamma_lower_bound=0-(from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound);
	//	new_net_gamma_upper_bound=0+(from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound);
	//	for(int j=0;j<=nb_option_transaction_cost;++j)
	//	{
	//		

	//		opt_trans_cost= lower_option_transaction_cost+(double) j*(upper_option_transaction_cost-lower_option_transaction_cost)/nb_option_transaction_cost;
	//		
	//		

	//		launcher =new Launcher(
	//			nb_of_scen,
	//			nb_index_to_cover,
	//			(string)hedgeff_path, 
	//			(string)rw_scn_path, 
	//			model_period,
	//			(string)outputs_path,
	//			moneyness,
	//			reb_freq_delta,
	//			reb_freq_gamma,
	//			volatility,
	//			length_of_option_contract,
	//			sell_at_time_to_expiry,
	//			length_of_futures_contract,
	//			gamma_per_cont,
	//			delta_per_cont,
	//			new_net_gamma_lower_bound,
	//			new_net_gamma_upper_bound,
	//			liab_gamma_lower_bound,
	//			liab_gamma_upper_bound,
	//			delta_transaction_cost,
	//			delta_eq_vol_hist,
	//			opt_trans_cost,
	//			hedge_gamma,
	//			rates_freq_ann,
	//			sheduled_futures,
	//			next_time_step_sheduled,
	//			nb_steps_per_year);
	//		output<<","<<launcher->return_total_pnl(0);
	//		delete launcher;
	//	}
	//	output<<endl;
	//}
}
extern "C" __declspec(dllexport) void Histo_Pnl_Explain_Time(int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
	double moneyness,
	int reb_freq_delta,
	int reb_freq_gamma,
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
	double delta_eq_vol_hist,
	double option_transaction_cost,
	char*  hedge_gamma,
	bool rates_freq_ann,
	bool sheduled_futures,
	int next_time_step_sheduled,
	int nb_steps_per_year,
	char* name_of_output
	)
{
	/*launcher2 =new Launcher(
			nb_of_scen,
			nb_index_to_cover,
			(string)hedgeff_path, 
			(string)rw_scn_path, 
			model_period,
			(string)outputs_path,
			moneyness,
			reb_freq_delta,
			reb_freq_gamma,
			volatility,
			length_of_option_contract,
			sell_at_time_to_expiry,
			length_of_futures_contract,
			gamma_per_cont,
			delta_per_cont,
			net_gamma_lower_bound,
			net_gamma_upper_bound,
			liab_gamma_lower_bound,
			liab_gamma_upper_bound,
			delta_transaction_cost,
			delta_eq_vol_hist,
			option_transaction_cost,
			"false",
			rates_freq_ann,
			sheduled_futures,
			next_time_step_sheduled,
			nb_steps_per_year
		);

	launcher =new Launcher(
		nb_of_scen,
		nb_index_to_cover,
		(string)hedgeff_path, 
		(string)rw_scn_path, 
		model_period,
		(string)outputs_path,
		moneyness,
		reb_freq_delta,
		reb_freq_gamma,
		volatility,
		length_of_option_contract,
		sell_at_time_to_expiry,
		length_of_futures_contract,
		gamma_per_cont,
		delta_per_cont,
		net_gamma_lower_bound,
		net_gamma_upper_bound,
		liab_gamma_lower_bound,
		liab_gamma_upper_bound,
		delta_transaction_cost,
		delta_eq_vol_hist,
		option_transaction_cost,
		hedge_gamma,
		rates_freq_ann,
		sheduled_futures,
		next_time_step_sheduled,
		nb_steps_per_year
		);
	ofstream output(name_of_output);
	output<<"Time";
	output<<",Total P&L (delta hedge)";
	output<<",Total P&L (delta gamma hedge)";
	output<<",Fut Trans Costs";
	output<<",Opt Trans Costs";
	output<<",Fut P&L";
	output<<",Opt P&L";
	output<<",Cash";
	output<<",Total Ov Change";
	output<<endl;
	for (int j=0;j<launcher->nb_of_scen;++j)
	{
		for(int k=0;k<launcher->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++k)
		{		output<<k;
			output<<","<<launcher2->extract_total_pnl(j)[k];
				output<<","<<launcher->extract_total_pnl(j)[k];
				output<<","<<-launcher->extract_fut_trans_cost_glob(j)[k];
				output<<","<<-launcher->extract_opt_trans_cost_glob(j)[k];
				output<<","<<launcher->extract_pnl_eq_fut_glob(j)[k];
				output<<","<<launcher->extract_pnl_opt_glob(j)[k];
				output<<","<<launcher->extract_cash_part(j)[k];
				output<<","<<-launcher->liab_ov_change(j)[k];		
				output<<endl;
		}
	}
	delete launcher;
	delete launcher2;*/
}

extern "C" __declspec(dllexport) void Histo_Pnl_Explain(
	int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
	double moneyness,
	int reb_freq_delta,
	int reb_freq_gamma,
	double volatility,
	int length_of_option_contract,
	int sell_at_time_to_expiry,
	int length_of_futures_contract,
	double gamma_per_cont,
	double delta_per_cont,
	double from_net_gamma_lower_bound,
	double to_net_gamma_upper_bound,
	double nb_net_gamma_bound,
	double liab_gamma_lower_bound,
	double liab_gamma_upper_bound,
	double delta_transaction_cost,
	double delta_eq_vol_hist,
	double option_transaction_cost,
	char*  hedge_gamma,
	bool rates_freq_ann,
	bool sheduled_futures,
	int next_time_step_sheduled,
	int nb_steps_per_year,
	char* name_of_output)
{
	/*double new_net_gamma_lower_bound;
	double new_net_gamma_upper_bound;
	ofstream output(name_of_output);
	output<<"Interval Length";
	output<<",Total P&L (delta hedge)";
	output<<",Total P&L (delta gamma hedge)";
	output<<",Fut Trans Costs";
	output<<",Opt Trans Costs";
	output<<",Fut P&L";
	output<<",Opt P&L";
	output<<",Cash";
	output<<",Total Ov Change";
	output<<endl;
	for(int i=0;i<=nb_net_gamma_bound;++i)
	{	
		output<<2*from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound;
			new_net_gamma_lower_bound=0-(from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound);
			new_net_gamma_upper_bound=0+(from_net_gamma_lower_bound+(double)i*(to_net_gamma_upper_bound-from_net_gamma_lower_bound)/nb_net_gamma_bound);
			launcher2 =new Launcher(
				nb_of_scen,
				nb_index_to_cover,
				(string)hedgeff_path, 
				(string)rw_scn_path, 
				model_period,
				(string)outputs_path,
				moneyness,
				reb_freq_delta,
				reb_freq_gamma,
				volatility,
				length_of_option_contract,
				sell_at_time_to_expiry,
				length_of_futures_contract,
				gamma_per_cont,
				delta_per_cont,
				new_net_gamma_lower_bound,
				new_net_gamma_upper_bound,
				liab_gamma_lower_bound,
				liab_gamma_upper_bound,
				delta_transaction_cost,
				delta_eq_vol_hist,
				option_transaction_cost,
				"false",
				rates_freq_ann,
				sheduled_futures,
				next_time_step_sheduled,
				nb_steps_per_year);

			launcher =new Launcher(
				nb_of_scen,
				nb_index_to_cover,
				(string)hedgeff_path, 
				(string)rw_scn_path, 
				model_period,
				(string)outputs_path,
				moneyness,
				reb_freq_delta,
				reb_freq_gamma,
				volatility,
				length_of_option_contract,
				sell_at_time_to_expiry,
				length_of_futures_contract,
				gamma_per_cont,
				delta_per_cont,
				new_net_gamma_lower_bound,
				new_net_gamma_upper_bound,
				liab_gamma_lower_bound,
				liab_gamma_upper_bound,
				delta_transaction_cost,
				delta_eq_vol_hist,
				option_transaction_cost,
				hedge_gamma,
				rates_freq_ann,
				sheduled_futures,
				next_time_step_sheduled,
				nb_steps_per_year);
				output<<","<<launcher2->return_total_pnl(0);
				output<<","<<launcher->return_total_pnl(0);
				output<<","<<-launcher->return_total_fut_trans_costs(0);
				output<<","<<-launcher->return_total_opt_trans_costs(0);
				output<<","<<+launcher->return_total_pnl_futs_glob(0);
				output<<","<<+launcher->return_total_pnl_opts_glob(0);
				output<<","<<+launcher->return_total_cash(0);
				output<<","<<-launcher->return_total_liab_ov_change(0);
			output<<endl;
			delete launcher;
			delete launcher2;
	}*/
}

extern "C" __declspec(dllexport) void Histo_Pnl_Explain_Opt_Trans_Costs(int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
	double moneyness,
	int reb_freq_delta,
	int reb_freq_gamma,
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
	double delta_eq_vol_hist,
	double lower_option_transaction_cost,
	double upper_option_transaction_cost,
	double nb_option_transaction_cost,
	char*  hedge_gamma,
	bool rates_freq_ann,
	bool sheduled_futures,
	int next_time_step_sheduled,
	int nb_steps_per_year,
	char* name_of_output
	)
{
	/*double opt_trans_costs;
	ofstream output(name_of_output);
	output<<"Opt Trans Costs";
	output<<",Total P&L (delta hedge)";
	output<<",Total P&L (delta gamma hedge)";
	output<<",Fut Trans Costs";
	output<<",Opt Trans Costs";
	output<<",Fut P&L";
	output<<",Opt P&L";
	output<<",Cash";
	output<<",Total Ov Change";
	output<<endl;
	launcher2 =new Launcher(
			nb_of_scen,
			nb_index_to_cover,
			(string)hedgeff_path, 
			(string)rw_scn_path,
			model_period,
			(string)outputs_path,
			moneyness,
			reb_freq_delta,
			reb_freq_gamma,
			volatility,
			length_of_option_contract,
			sell_at_time_to_expiry,
			length_of_futures_contract,
			gamma_per_cont,
			delta_per_cont,
			net_gamma_lower_bound,
			net_gamma_upper_bound,
			liab_gamma_lower_bound,
			liab_gamma_upper_bound,
			delta_transaction_cost,
			delta_eq_vol_hist,
			0,
			"false",
			rates_freq_ann,
			sheduled_futures,
			next_time_step_sheduled,
			nb_steps_per_year
		);
	for(int k=0;k<=nb_option_transaction_cost;++k)
	{
		opt_trans_costs=lower_option_transaction_cost+(double)k*(upper_option_transaction_cost-lower_option_transaction_cost)/nb_option_transaction_cost;
		output<<opt_trans_costs;
		launcher =new Launcher(
			nb_of_scen,
			nb_index_to_cover,
			(string)hedgeff_path, 
			(string)rw_scn_path,
			model_period,
			(string)outputs_path,
			moneyness,
			reb_freq_delta,
			reb_freq_gamma,
			volatility,
			length_of_option_contract,
			sell_at_time_to_expiry,
			length_of_futures_contract,
			gamma_per_cont,
			delta_per_cont,
			net_gamma_lower_bound,
			net_gamma_upper_bound,
			liab_gamma_lower_bound,
			liab_gamma_upper_bound,
			delta_transaction_cost,
			delta_eq_vol_hist,
			opt_trans_costs,
			hedge_gamma,
			rates_freq_ann,
			sheduled_futures,
			next_time_step_sheduled,
			nb_steps_per_year
		);
		
		output<<","<<launcher2->return_total_pnl(0);
		output<<","<<launcher->return_total_pnl(0);
		output<<","<<-launcher->return_total_fut_trans_costs(0);
		output<<","<<-launcher->return_total_opt_trans_costs(0);
		output<<","<<+launcher->return_total_pnl_futs_glob(0);
		output<<","<<+launcher->return_total_pnl_opts_glob(0);
		output<<","<<+launcher->return_total_cash(0);
		output<<","<<-launcher->return_total_liab_ov_change(0);
		output<<endl;
	delete launcher;
	
	}
	delete launcher2;*/
}
extern "C" __declspec(dllexport) void Histo_Pnl_Explain_Gamma_Reb_Freq(int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
	double moneyness,
	int reb_freq_delta,
	int lower_reb_freq_gamma,
	int upper_reb_freq_gamma,
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
	double delta_eq_vol_hist,
	double option_transaction_cost,
	char*  hedge_gamma,
	bool rates_freq_ann,
	bool sheduled_futures,
	int next_time_step_sheduled,
	int nb_steps_per_year,
	char* name_of_output
	)
{
	/*ofstream output(name_of_output);
	output<<"Reb Freq Gamma";
	output<<",Total P&L (delta hedge)";
	output<<",Total P&L (delta gamma hedge)";
	output<<",Fut Trans Costs";
	output<<",Opt Trans Costs";
	output<<",Fut P&L";
	output<<",Opt P&L";
	output<<",Cash";
	output<<",Total Ov Change";
	output<<endl;
	launcher2 =new Launcher(
			nb_of_scen,
			nb_index_to_cover,
			(string)hedgeff_path, 
			(string)rw_scn_path, 
			model_period,
			(string)outputs_path,
			moneyness,
			reb_freq_delta,
			1,
			volatility,
			length_of_option_contract,
			sell_at_time_to_expiry,
			length_of_futures_contract,
			gamma_per_cont,
			delta_per_cont,
			net_gamma_lower_bound,
			net_gamma_upper_bound,
			liab_gamma_lower_bound,
			liab_gamma_upper_bound,
			delta_transaction_cost,
			delta_eq_vol_hist,
			option_transaction_cost,
			"false",
			rates_freq_ann,
			sheduled_futures,
			next_time_step_sheduled,
			nb_steps_per_year
		);
	for(int reb_freq_gamma=lower_reb_freq_gamma;reb_freq_gamma<=upper_reb_freq_gamma;++reb_freq_gamma)
	{
		output<<reb_freq_gamma;
		
		launcher =new Launcher(
			nb_of_scen,
			nb_index_to_cover,
			(string)hedgeff_path, 
			(string)rw_scn_path, 
			model_period,
			(string)outputs_path,
			moneyness,
			reb_freq_delta,
			reb_freq_gamma,
			volatility,
			length_of_option_contract,
			sell_at_time_to_expiry,
			length_of_futures_contract,
			gamma_per_cont,
			delta_per_cont,
			net_gamma_lower_bound,
			net_gamma_upper_bound,
			liab_gamma_lower_bound,
			liab_gamma_upper_bound,
			delta_transaction_cost,
			delta_eq_vol_hist,
			option_transaction_cost,
			hedge_gamma,
			rates_freq_ann,
			sheduled_futures,
			next_time_step_sheduled,
			nb_steps_per_year
		);
		
		output<<","<<launcher2->return_total_pnl(0);
		output<<","<<launcher->return_total_pnl(0);
		output<<","<<-launcher->return_total_fut_trans_costs(0);
		output<<","<<-launcher->return_total_opt_trans_costs(0);
		output<<","<<+launcher->return_total_pnl_futs_glob(0);
		output<<","<<+launcher->return_total_pnl_opts_glob(0);
		output<<","<<+launcher->return_total_cash(0);
		output<<","<<-launcher->return_total_liab_ov_change(0);
		output<<endl;
	delete launcher;
	
	}
	delete launcher2;*/
}
/** Create a Launcher object and launch the hedging calculus for every scenario and every assets*/
extern "C" __declspec(dllexport) void Compute(
	int nb_of_scen, 
	int nb_index_to_cover,
	char* hedgeff_path,
	char* rw_scn_path,
	int model_period,
	char* outputs_path,
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
	double* index_bases,
	double* index_to_hedge,
	double* scen_to_hedge,
	
	//chth221209
	//int my_chth
	//chth221209-2
	double* start_mty,
	double* end_mty
	)
{

	//ofstream output("ALEX.txt");	
	//output<<"aaaa: "<<rates_freq_ann<<endl;
	//output<<double_back_slash((string)CurrentPath);
	//output<<inputs_path<<endl;
	//output<<outputs_path<<endl;
	//output.close();
	launcher =new Launcher(0,0,
		nb_of_scen,
		nb_index_to_cover,
		(string)hedgeff_path, 
		(string)rw_scn_path, 
		model_period,
		(string)outputs_path,
		moneyness,
		reb_freq_delta,
		reb_freq_gamma,
		reb_freq_rho,
		volatility,
		length_of_option_contract,
		sell_at_time_to_expiry,
		length_of_futures_contract,
		gamma_per_cont,
		delta_per_cont,
		net_gamma_lower_bound,
		net_gamma_upper_bound,
		liab_gamma_lower_bound,
		liab_gamma_upper_bound,
		delta_transaction_cost,
		option_transaction_cost,
		rho_transaction_cost,
		hedge_delta,
		hedge_gamma,
		hedge_rho,
		rates_freq_ann,
		sheduled_futures,
		next_time_step_sheduled,
		nb_steps_per_year,
		nb_maturities_to_hedge,
	    maturities_to_hedge,
	    shocks_to_apply,
		compounding_frequencies,
		index_bases,
		index_to_hedge,
		scen_to_hedge,

		//chth221209
		//my_chth
		//chth221209-2
		start_mty,
		end_mty
		);

}
/** Used to display a Report2 like output (see VBA code)*/
extern "C" __declspec(dllexport) int get_report_field(int scen_nbr,int asset_nbr,int swap_no,double* result,int model_period,char* field)
{
	// Comments by Yann & Yuan - used for interfacing
	/*double* field_to_extract=0;
	if (strcmp(field,"Period")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->hedgeff->period;
	if (strcmp(field,"Delta Liab")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->liability_delta.delta_liab;
	if (strcmp(field,"Asset Delta Before Futures")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->asset_delta.asset_delta_before_futures;
	if (strcmp(field,"Net Delta Before Futures")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->net_delta_before_futures;
	if (strcmp(field,"Asset Delta After Futures")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->asset_delta.asset_delta_after_futures;
	if (strcmp(field,"Net Delta After Futures")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->net_delta_after_futures;
	if (strcmp(field,"Eq Fut Ct")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->nb_of_futures.eq_fut_1_ct;
	if (strcmp(field,"Future Pursh")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->future_pursh;
	if (strcmp(field,"Future Sales")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->future_sales;
	if (strcmp(field,"Market Value (Fut)")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->delta->mkt_value;
	if (strcmp(field,"Gamma Liab")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->hedgeff->gamma_liab;
	if (strcmp(field,"Asset Gamma Before Options")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->asset_gamma_before_options;
	if (strcmp(field,"Net Gamma Before Options")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->net_gamma_before_options;
	if (strcmp(field,"Asset Gamma After Options")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->asset_gamma_after_options;
	if (strcmp(field,"Net Gamma After Options")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->net_gamma_after_options;
	if (strcmp(field,"Opt Not")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->hand_calculate.opt1_not;
	if (strcmp(field,"Eq Put Ct")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->hand_calculate.eq_put_1_ct;
	if (strcmp(field,"Option Pursh")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->hand_calculate.option_pursh;
	if (strcmp(field,"Option Sales")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->hand_calculate.option_sales;
	if (strcmp(field,"Market Value (Opt)")==0) field_to_extract=launcher->
		tab_of_delta_gamma_hedge[scen_nbr-1][asset_nbr-1]->gamma->hand_calculate.mkt_value;

	if (strcmp(field,"Rho Liab")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->liability_rho.rho_liab;
	if (strcmp(field,"Asset Rho Before Swaps")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->asset_rho.asset_rho_before_swaps;
	if (strcmp(field,"Net Rho Before Swaps")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->net_rho_before_swaps;
	if (strcmp(field,"Asset Rho After Swaps")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->asset_rho.asset_rho_after_swaps;
	if (strcmp(field,"Net Rho After Swaps")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->net_rho_after_swaps;
	if (strcmp(field,"Rt Swp Ct")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->nb_of_swaps.rt_swp_1_ct;
	if (strcmp(field,"Swap Pursh")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->swap_pursh;
	if (strcmp(field,"Swap Sales")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->swap_sales;
	if (strcmp(field,"Market Value (Swp)")==0) field_to_extract=launcher->
		tab_of_rho_hedge[scen_nbr-1][swap_no]->rho->mkt_value;

	if (strcmp(field,"Pnl Eq Futures")==0) field_to_extract=launcher->extract_pnl_eq_fut_glob(scen_nbr-1);
	if (strcmp(field,"Fut Trans Costs")==0) field_to_extract=launcher->extract_fut_trans_cost_glob(scen_nbr-1);
	if (strcmp(field,"Pnl Opt" )==0) field_to_extract=launcher->extract_pnl_opt_glob(scen_nbr-1);
	if (strcmp(field,"Opt Trans Costs")==0) field_to_extract=launcher->extract_opt_trans_cost_glob(scen_nbr-1);
	if (strcmp(field,"Pnl Rt Swaps")==0) field_to_extract=launcher->extract_pnl_rt_swp_glob(scen_nbr-1);
	if (strcmp(field,"Swp Trans Costs")==0) field_to_extract=launcher->extract_swp_trans_cost_glob(scen_nbr-1);
	if (strcmp(field,"Trans Costs Opt + Fut + Swp")==0) field_to_extract=launcher->extract_fut_opt_swp_trans_cost_glob(scen_nbr-1);
	if (strcmp(field,"Pnl Opt + Fut + Swp")==0) field_to_extract=launcher->extract_pnl_opt_fut_swp_glob(scen_nbr-1);
	if (strcmp(field,"Cash")==0) field_to_extract=launcher->extract_cash_part(scen_nbr-1);
	if (strcmp(field,"Option value Change")==0) field_to_extract=launcher->liab_ov_change(scen_nbr-1);
	if (strcmp(field,"Porftolio Pnl")==0) field_to_extract=launcher->extract_portfolio_pnl(scen_nbr-1);
	if (strcmp(field,"TOTAL PNL")==0) field_to_extract=launcher->extract_total_pnl(scen_nbr-1);

	for(int i=0;i<model_period;++i)
	{
		result[i]=field_to_extract[i];
	}*/
	return 0;
}
/** Used to display a report1 type output (see VBA code)*/
// Comments by Yuan 
//extern "C" __declspec(dllexport) int display_report1_bis(char* out_path)
//{
//	launcher->display_report1_bis((string)out_path);
//	return 0;
//}

/** Used to display a 2 dimensional tab <BR>
* Usefull for intermediary reasults.
//////*/
//////extern "C" __declspec(dllexport) int display_two_dim_tab(char* name,char* out_path,int scen_no,int asset_no)
//////{
//////	double ** two_dim_tab;
//////	int row_nb,col_nb;
//////	row_nb=launcher->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;
//////	col_nb=launcher->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;
//////	if (strcmp(name,"Annual Swap Rates")==0 || strcmp(name,"Zero Coupon Rates")==0)
//////	{
//////		col_nb=launcher->tab_of_delta_gamma_hedge[0][0]->annual_swap_rates->dim_y;
//////	}
//////	if (strcmp(name,"Rw Scn")==0 )
//////	{
//////		col_nb=launcher->tab_of_delta_gamma_hedge[0][0]->rw_scn->nb_of_mty;
//////	}
//////	row_nb=launcher->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;
//////	two_dim_tab=new double*[row_nb];
//////	for(int i=0;i<launcher->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//////	{
//////		two_dim_tab[i]=new double[col_nb];
//////	}
//////	//ofstream output("ALEXa.txt");	
//////	//output<<name<<endl;
//////	//	output<<(name=="Nbr Puts")<<endl;
//////	//	output.close();
//////	if (strcmp(name,"Nbr Puts")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->nbr_puts->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Asset Delta")==0) 
//////		launcher->display_one_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_in_portfolio->asset_delta, 
//////		(string)name, (string)out_path, scen_no, asset_no);
//////	else if (strcmp(name,"Trading Options")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->trading->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Trading Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->trading_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Option Notional")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->option_notional->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"New Option Notional")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->new_option_notional->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Existing Gamma")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->existing_gamma->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Market Value Options")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->market_value->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Market Value Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->market_value_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Gamma Puts In Portfolio")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->gamma_puts_in_portfolio->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Gamma Puts In Portfolio (2)")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->gamma_puts_in_portfolio_2->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Gamma Puts")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->gamma_puts->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Delta In Portfolio")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_in_portfolio->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Delta Puts")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_puts->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Tte")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->tte->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Annual Swap Rates")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->annual_swap_rates->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Zero Coupon Rates")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->zero_coupon_rates->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->rf->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Annual_Swap_Rates")==0) ;
//////	/*launcher->display_two_dim_tab(
//////	launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->->tab, 
//////	(string)name, (string)out_path, scen_no, asset_no);*/
//////	else if (strcmp(name,"Underlying")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->underlying->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Und_Base")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->und_base->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Ov_Base")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->ov_base->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Fv_Base")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->fv_base->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Gamma Asset")==0) 
//////		launcher->display_gamma_asset(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->gamma_asset, 
//////		(string)name, (string)out_path, scen_no, asset_no);
//////	else if (strcmp(name,"Delta Asset")==0) 
//////		launcher->display_delta_asset(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_asset, 
//////		(string)name, (string)out_path, scen_no, asset_no);
//////	else if (strcmp(name,"Asset Purchase")==0) 
//////		launcher->display_asset_purchase(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->option_trade, 
//////		(string)name, (string)out_path, scen_no, asset_no);
//////	else if (strcmp(name,"Futures Purchase")==0) 
//////		launcher->display_futures_purchase(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->future_trade, 
//////		(string)name, (string)out_path, scen_no, asset_no);
//////	else if (strcmp(name,"Tte Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->tte_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Delta Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->rf_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Delta Futures In Portfolio")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_futures_in_portfolio->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Delta Futures In Portfolio (2)")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->delta_futures_in_portfolio_2->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Nbr Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->nbr_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Existing Delta")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->existing_delta->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Existing Delta Futures")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->existing_delta_futures->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rw Scn")==0) 
//////		launcher->display_two_dim_tab(
//////		launcher->tab_of_delta_gamma_hedge[scen_no-1][asset_no-1]->rw_scn->tab, 
//////		(string)name, (string)out_path, scen_no, asset_no,row_nb,col_nb);
//////
//////
//////
//////
//////	return 0;
//////}
//////
//////extern "C" __declspec(dllexport) int display_two_dim_tab_in_three_dim_tab(char* name,char* out_path,int scen_no,int swap_no,int asset_no)
//////{
//////	//double ** two_dim_tab;
//////	int row_nb,col_nb;
//////	row_nb=launcher->tab_of_rho_hedge[0][0]->assumptions->model_period;
//////	col_nb=launcher->tab_of_rho_hedge[0][0]->assumptions->model_period;
//////	if (strcmp(name,"Annual Swap Rates Shock Up")==0 || 
//////		strcmp(name,"Annual Swap Rates Shock Down")==0 || 
//////		strcmp(name,"Zero Coupon Rates Shock Up")==0|| 
//////		strcmp(name,"Zero Coupon Rates Shock Down")==0)
//////	{
//////		col_nb=launcher->tab_of_rho_hedge[0][0]->annual_swap_rates_shock_down->dim_y;
//////	}
//////	if (strcmp(name,"Rw Scn Shock Up")==0 || 
//////		strcmp(name,"Rw Scn Shock Down")==0 )
//////	{
//////		col_nb=launcher->tab_of_rho_hedge[0][0]->annual_swap_rates_shock_up->dim_y;
//////	}
//////	
//////	//ofstream output("alex.txt");
//////	//output<<name<<endl;
//////	//output.close();
//////	if (strcmp(name,"Annual Swap Rates Shock Up")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->annual_swap_rates_shock_up->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Annual Swap Rates Shock Down")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->annual_swap_rates_shock_down->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	if (strcmp(name,"Zero Coupon Rates Shock Up")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->zero_coupon_rates_shock_up->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Zero Coupon Rates Shock Down")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->zero_coupon_rates_shock_down->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf Options Shock Up")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rf_shock_up->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf Options Shock Down")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rf_shock_down->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf Futures Shock Up")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rf_futures_shock_up->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rf Futures Shock Down")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rf_futures_shock_down->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho Options")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_puts_glob[asset_no-1]->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho Futures")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_futures_glob[asset_no-1]->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Existing Rho")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->existing_rho->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Existing Rho Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->existing_rho_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Nbr Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->nbr_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho Swaps In Portfolio")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_swaps_in_portfolio->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho Swaps In Portfolio (2)")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_swaps_in_portfolio_2->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Tte Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->tte_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Sv Base")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->sv_base->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Rho In Porftolio")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rho_in_portfolio->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Market Value Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->market_value_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"Trading Swaps")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->trading_swaps->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
//////	else if (strcmp(name,"New Swaps Notional")==0) 
//////	launcher->display_two_dim_tab(
//////		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->new_swaps_notional->tab, 
//////		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
	/*else if (strcmp(name,"Rw Scn Shock Up")==0) 
	launcher->display_two_dim_tab(
		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rw_scn_up->tab, 
		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);
	else if (strcmp(name,"Rw Scn Shock Down")==0) 
	launcher->display_two_dim_tab(
		launcher->tab_of_rho_hedge[scen_no-1][swap_no]->rw_scn_down->tab, 
		(string)name, (string)out_path, scen_no, swap_no,row_nb,col_nb);*/
////////
////////	return 0;
////////}

extern "C" __declspec(dllexport) void delete_launcher()
{

	delete launcher;
}

// Comments by Yuan and Yann
//extern "C" __declspec(dllexport) double return_total_pnl(int scen)
//{
//	return launcher->return_total_pnl(scen-1);
//}
#endif