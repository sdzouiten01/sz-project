#include "Launcher.h"
#include "Gamma.h"
#include "Gamma_Asset.h"
#include "Option_Trade.h"
#include "Debug.h"
//#include "tbb/parallel_for.h"


//CHTH Fonction qui devra etre appeler par le VBA 



Launcher::Launcher(
		int start_scn,
		int end_scn,
		int nb_of_scen,
	    int nb_index_to_cover,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	    string hedgeff_path,
		string rw_scn_path,
	    int model_period,
	    string outputs_path,
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



	//{

	// this->nb_index_to_cover=nb_index_to_cover;
	// this->nb_of_scen=nb_of_scen;


	// Creer l output
	
	// Initialiser l output


//	for (int j=0;j<nb_of_scen;++j)
//	{

		// Creer un Delta gamma Hedge

		// Creer un Rho Hedge

		// Ecriture (Delta Gamma) + Rho

		// Destruction Delta gamma

		// Destruction Rho hedge


//	}


	// Fermeture fichier output

//}
	//chth221209
	//this->my_chth = my_chth;
	
	this->nb_index_to_cover=nb_index_to_cover;
	this->nb_of_scen=nb_of_scen;
	
	for (int it_scen=start_scn-1;it_scen<end_scn;++it_scen)
	{
		cout << "Scenario running : " << it_scen + 1 << endl;
		Delta_Gamma_Hedge** tab_of_delta_gamma_hedge = new Delta_Gamma_Hedge*[nb_index_to_cover];
		
		for (int i=0;i<nb_index_to_cover;++i)
		{
			tab_of_delta_gamma_hedge[i] = new Delta_Gamma_Hedge(
					hedgeff_path,
					rw_scn_path,
					outputs_path,
					(int)index_to_hedge[i],
					(int)scen_to_hedge[it_scen],
					model_period,
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
					index_bases[i],
					//chth221209-2
					start_mty,
					end_mty
					);
					if ((it_scen) == start_scn-1 && (i ==0))
					{
						this->nb_maturities_to_hedge=tab_of_delta_gamma_hedge[0]->assumptions->nb_maturities_to_hedge;
						//comment by chth
						//display_header("C:\\HEDGEEFF_ASSET\\NEEDED\\Outputs\\mynewtest.csv", tab_of_delta_gamma_hedge);
						display_header(outputs_path , tab_of_delta_gamma_hedge, start_scn, end_scn);

					}
		}
        //! Maybe to be deleted later ... 
		this->nb_maturities_to_hedge=tab_of_delta_gamma_hedge[0]->assumptions->nb_maturities_to_hedge;

		Rho_Hedge ** table_of_rho_hedge = new Rho_Hedge*[this->nb_maturities_to_hedge];

		for (int j=0; j < this->nb_maturities_to_hedge ;++j)
		{
			table_of_rho_hedge[j] = new Rho_Hedge(j, tab_of_delta_gamma_hedge, this->nb_index_to_cover, it_scen);
		}

		//// Ecriture - comment by chth
		//display_report1_bis("C:\\HEDGEEFF_ASSET\\NEEDED\\Outputs\\mynewtest.csv", tab_of_delta_gamma_hedge, table_of_rho_hedge, it_scen);
		display_report1_bis(outputs_path , tab_of_delta_gamma_hedge, table_of_rho_hedge, it_scen, start_scn, end_scn);

		// Delete the delta gamma Hedge
		for (int i=0;i<nb_index_to_cover;++i)
		{
			tab_of_delta_gamma_hedge[i]->erase_objects();	
		}


		for (int i=0;i<nb_maturities_to_hedge;++i)
		{
			delete table_of_rho_hedge[i]; 
		}
		delete[] table_of_rho_hedge;
		



	
	}

	

//	debug_file.open("Debug_File.txt");
//	ofstream output("C:\\Documents and Settings\\l-cohen\\My Documents\\aaaaaaaaaaaaaa.txt");
/*	
	this->nb_index_to_cover=nb_index_to_cover;
	this->nb_of_scen=nb_of_scen;
	tab_of_delta_gamma_hedge=new Delta_Gamma_Hedge** [nb_of_scen];
	for (int i=0;i<nb_of_scen;++i)
	{
		tab_of_delta_gamma_hedge[i]=new Delta_Gamma_Hedge*[nb_index_to_cover];
	}
	for (int j=0;j<nb_of_scen;++j)
	{
		for (int i=0;i<nb_index_to_cover;++i)
		{
		
			tab_of_delta_gamma_hedge[j][i] =
				new Delta_Gamma_Hedge(
					hedgeff_path,
					rw_scn_path,
					outputs_path,
					(int)index_to_hedge[i],
					(int)scen_to_hedge[j],
					model_period,
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
					index_bases[i]
					);
					
			
			
		}
		//cout<<endl;
	}
	this->nb_maturities_to_hedge=this->tab_of_delta_gamma_hedge[0][0]->assumptions->nb_maturities_to_hedge;

	tab_of_rho_hedge=new Rho_Hedge** [nb_of_scen];
	for (int i=0;i<nb_of_scen;++i)
	{
		tab_of_rho_hedge[i]=new Rho_Hedge*[this->tab_of_delta_gamma_hedge[0][0]->assumptions->nb_maturities_to_hedge];
	}

	for (int j=1;j<=nb_of_scen;++j)
	{
		for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->nb_maturities_to_hedge;++i)
		{
			this->tab_of_rho_hedge[j-1][i]=new Rho_Hedge(i,tab_of_delta_gamma_hedge,this->nb_index_to_cover,j);
		}
	}
	

//fichier a comparer
display_report1_bis("mynewtest.csv");

//CHTH destructeur
	

	for (int j=0;j<nb_of_scen;++j)
	{
		for (int i=0;i<nb_index_to_cover;++i)
		{
			this->tab_of_delta_gamma_hedge[j][i]->erase_objects();	
		}
		
		for (int i=0;i< nb_maturities_to_hedge ;++i)
		{
			delete  this->tab_of_rho_hedge[j][i];
		}
	}
	

	//for (int j=1;j<=nb_of_scen;++j)
	//{
	//	for (int i=0;i< nb_maturities_to_hedge ;++i)
	//	{
	//		delete  this->tab_of_rho_hedge[j-1][i];
	//	}
	//}
	*/



}

void Launcher::display_gamma_asset(Gamma_Asset* gamma_asset,
								   string name,
								   string out_path,
								   int scen_no,
								   int asset_no,
								   Delta_Gamma_Hedge** tab_of_delta_gamma_hedge)
{
	string output_name;
	
	output_name=out_path+"\\"+name;
	char* tst=new char[3];
	output_name+="_s";
	_itoa_s(scen_no,tst,3,10);
	output_name+=(string)tst;
	output_name+="_a";
	_itoa_s(asset_no,tst,3,10);
	output_name+=(string)tst;
	output_name=output_name+".csv";
	ofstream test("Alexandre.txt");
	test<<output_name<<endl;
	test.close();
	ofstream output;
	const char* cc=output_name.c_str();

	output.open(cc);
	

	for (int i=0;i<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++i)
	{
		output<<"Time,K,r,q,tte,Sigma"<<endl;
		output<<gamma_asset->tab_of_gamma_asset_struct[i].time<<","
			<<gamma_asset->tab_of_gamma_asset_struct[i].k<<","
			<<gamma_asset->tab_of_gamma_asset_struct[i].r<<","
			<<gamma_asset->tab_of_gamma_asset_struct[i].q<<","
			<<gamma_asset->tab_of_gamma_asset_struct[i].tte<<","
			<<gamma_asset->tab_of_gamma_asset_struct[i].sigma<<endl;
		output<<"gamma: ,"<<gamma_asset->tab_of_gamma_asset_struct[i].gamma<<endl<<endl
			<<"-----------------------------------------------------"<<endl<<endl;

	}

	output.close();
}
void Launcher::display_delta_asset(Delta_Asset* delta_asset,
								   string name,
								   string out_path,
								   int scen_no,
								   int asset_no,
								   Delta_Gamma_Hedge** tab_of_delta_gamma_hedge)
{
	string output_name;
	
	output_name=out_path+"\\"+name;
	char* tst=new char[3];
	output_name+="_s";
	_itoa_s(scen_no,tst,3,10);
	output_name+=(string)tst;
	output_name+="_a";
	_itoa_s(asset_no,tst,3,10);
	output_name+=(string)tst;
	output_name=output_name+".csv";
	//ofstream test("Alexandre.txt");
	//test<<output_name<<endl;
	//test.close();
	ofstream output;
	const char* cc=output_name.c_str();

	output.open(cc);
	

	for (int i=0;i<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++i)
	{
		output<<"K,"<<delta_asset->tab_of_delta_asset_struct[i].k<<endl;
		output<<"q,"<<delta_asset->tab_of_delta_asset_struct[i].q<<endl;
		output<<"r,"<<delta_asset->tab_of_delta_asset_struct[i].r<<endl;
		output<<"sigma,"<<delta_asset->tab_of_delta_asset_struct[i].sigma<<endl;
		output<<"tte,"<<delta_asset->tab_of_delta_asset_struct[i].tte<<endl;
		output<<"delta,"<<delta_asset->tab_of_delta_asset_struct[i].delta<<endl;
		output<<"-----------------------------------------------------------------------"<<endl<<endl;
	}

}
void Launcher::display_futures_purchase(Future_Trade* future_trade,
										string name,
										string out_path,
										int scen_no,
										int asset_no,
										Delta_Gamma_Hedge** tab_of_delta_gamma_hedge)
{
	string output_name;
	output_name=out_path+"\\"+name;
	char* tst=new char[3];
	output_name+="_s";
	_itoa_s(scen_no,tst,3,10);
	output_name+=(string)tst;
	output_name+="_a";
	_itoa_s(asset_no,tst,3,10);
	output_name+=(string)tst;
	output_name=output_name+".csv";
	ofstream test("Alexandre.txt");
	test<<output_name<<endl;
	test.close();
	ofstream output;
	const char* cc=output_name.c_str();

	output.open(cc);

	

	for (int i=0;i<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++i)
	{
		output<<"Time: ,"<<future_trade->tab_futures_purchase_struct[i].time<<endl;
		output<<"Hedging: ,"<<future_trade->tab_futures_purchase_struct[i].hedging<<endl<<endl;
		
		output<<",Ex_Delta_Fut>=0,Ex_Delta_Fut<=0"<<endl;
		output<<"Net delta>=0,"<<future_trade->tab_futures_purchase_struct[i].tab[0][0]
								<<","<<future_trade->tab_futures_purchase_struct[i].tab[0][1]
								
								<<endl;
		output<<"Net delta<=0,"<<future_trade->tab_futures_purchase_struct[i].tab[1][0]
								<<","<<future_trade->tab_futures_purchase_struct[i].tab[1][1]
								
								<<endl;

		

		output<<"Existing,Existing,Delta,Delta,Net,Futures sales,Contracts"<<endl;
		output<<"Delta,Delta Fut,Asset,Liability,Delta,Indicator"<<endl;
		output<<future_trade->tab_futures_purchase_struct[i].ex_delta
			  <<","<<future_trade->tab_futures_purchase_struct[i].ex_delta_futures
			  <<","<<future_trade->tab_futures_purchase_struct[i].delta_asset
			  <<","<<future_trade->tab_futures_purchase_struct[i].delta_liability
			  <<","<<future_trade->tab_futures_purchase_struct[i].net_delta
			  <<","<<future_trade->tab_futures_purchase_struct[i].short_existing_positions
			  <<","<<future_trade->tab_futures_purchase_struct[i].contracts<<endl;
		output<<","<<" "
			  <<","<<" "
			  <<",New Net Delta"
			  <<","<<future_trade->tab_futures_purchase_struct[i].new_net_delta<<endl<<endl;

		output<<"--------------------------------------------"
			  <<"--------------------------------------------"
			  <<"--------------------------------------------"
			  <<"--------------------------------------------"<<endl<<endl;
	}
}

void Launcher::display_asset_purchase(Option_Trade* option_trade,string name,string out_path,int scen_no,int asset_no)
{
//	string output_name;
//	output_name=out_path+"\\"+name;
//	char* tst=new char[3];
//	output_name+="_s";
//	output_name+=(string)itoa(scen_no,tst,10);
//	output_name+="_a";
//	output_name+=(string)itoa(asset_no,tst,10);
//	output_name=output_name+".csv";
//	ofstream test("Alexandre.txt");
//	test<<output_name<<endl;
//	test.close();
//	ofstream output;
//	const char* cc=output_name.c_str();
//
//	output.open(cc);
//
//	
//
//	/*for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		output<<"Time: ,"<<option_trade->tab_option_trade_struct[i].time<<endl;
//		output<<"Hedging: ,"<<option_trade->tab_option_trade_struct[i].hedging<<endl<<endl;
//		
//		output<<",,,                  Liability Tolerance                 "<<endl;
//		output<<",,,Outside,               Inside             "<<endl;
//		output<<",,,,existing_gamma<0,existing_gamma>0"<<endl;
//		output<<",>Upper Bound,"<<option_trade->tab_option_trade_struct[i].tab[0][0]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[0][1]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[0][2]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[0][3]
//								<<endl;
//		output<<"Net greek value,Inside Tolerance Area,"<<option_trade->tab_option_trade_struct[i].tab[1][0]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[1][1]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[1][2]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[1][3]
//								<<endl;
//
//		output<<",<Lower Bound,"<<option_trade->tab_option_trade_struct[i].tab[2][0]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[2][1]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[2][2]
//								<<","<<option_trade->tab_option_trade_struct[i].tab[2][3]
//								<<endl<<endl;
//
//		output<<"Existing,Gamma,Gamma,Net,Option sales,Contracts"<<endl;
//		output<<"Gamma,Put,Liability,Gamma,Indicator"<<endl;
//		output<<option_trade->tab_option_trade_struct[i].ex_gamma
//			  <<","<<option_trade->tab_option_trade_struct[i].gamma_put
//			  <<","<<option_trade->tab_option_trade_struct[i].gamma_liability
//			  <<","<<option_trade->tab_option_trade_struct[i].net_gamma
//			  <<","<<option_trade->tab_option_trade_struct[i].short_existing_positions
//			  <<","<<option_trade->tab_option_trade_struct[i].contracts<<endl;
//		output<<","<<" "
//			  <<","<<" "
//			  <<",New Net Gamma"
//			  <<","<<option_trade->tab_option_trade_struct[i].new_net_gamma<<endl<<endl;
//
//		output<<"--------------------------------------------"
//			  <<"--------------------------------------------"
//			  <<"--------------------------------------------"
//			  <<"--------------------------------------------"<<endl<<endl;
//	}*/
}



void Launcher::display_two_dim_tab(double** two_dim_tab,string name,string out_path,int scen_no,int asset_no,int row_nb,int col_nb)
{

	string output_name;
	/*ofstream gg("hhhhhhhhhhhhh.txt");
	gg<<out_path<<endl;
	gg.close();*/
	output_name=out_path;
	/*char* tst=new char[3];
	output_name+="_s";
	_itoa_s(scen_no,tst,3,10);
	output_name+=(string)tst;
	output_name+="_a";
	_itoa_s(asset_no,tst,3,10);
	output_name+=(string)tst;
	output_name=output_name+".csv";*/
	/*ofstream test("Alexandre.txt");
	test<<output_name<<endl;
	test.close();*/

	ofstream output;
	const char* cc=output_name.c_str();

	output.open(cc);
	for(int i=0;i<row_nb;++i)
	{
		for(int j=0;j<col_nb;++j)
		{
			output<<two_dim_tab[i][j]<<", ";
		}
		output<<endl;
	}
	output.close();
}
void Launcher::display_one_dim_tab(double* one_dim_tab,string name,string out_path,int scen_no,int asset_no)
{
//
//	string output_name;
//	
//	output_name=out_path+"\\"+name;
//	char* tst=new char[3];
//	output_name+="_s";
//	output_name+=(string)itoa(scen_no,tst,10);
//	output_name+="_a";
//	output_name+=(string)itoa(asset_no,tst,10);
//	output_name=output_name+".csv";
//	/*ofstream test("Alexandre.txt");
//	test<<output_name<<endl;
//	test.close();*/
//	ofstream output;
//	const char* cc=output_name.c_str();
//
//	output.open(cc);
//	for(int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		
//		output<<one_dim_tab[i]<<endl;
//		
//	}
//	output.close();
}



// 1 - ADDING tables as input 
double* Launcher::extract_pnl_eq_fut_glob(Delta_Gamma_Hedge **tab_of_delta_gamma_hedge)
{
	
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->delta->pnl_eq_fut[k];
		}
	}
	return res;
}

// 2 - ADDING tables as input 
double* Launcher::extract_fut_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		
		res[k]=0;
		
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->delta->trans_cost[k];
		}
	}
	return res;
}

// 3 - ADDING tables as input 
double* Launcher::extract_pnl_opt_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge)
{
	
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.pnl_opts[k];
		}
	}
	return res;
}

// 4 - ADDING tables as input
double* Launcher::extract_opt_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.trans_costs[k];
		}
	}
	return res;
}

// 5 - - ADDING tables as input 
double* Launcher::extract_pnl_rt_swp_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge)
{
	
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		for (int i=0;i<this->nb_maturities_to_hedge;++i)
		{
			res[k]+=tab_of_rho_hedge[i]->rho->pnl_rt_swp[k];
		}
	}
	return res;
}

// 6 - ADDING tables as input 
double* Launcher::extract_swp_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		
		res[k]=0;
		
		for (int i=0;i<this->nb_maturities_to_hedge;++i)
		{
			res[k]+=tab_of_rho_hedge[i]->rho->trans_cost[k];
		}
	}
	return res;
}

// 7 - ADDING tables as input 
double* Launcher::extract_fut_opt_swp_trans_cost_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		
		res[k]=0;
		
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.trans_costs[k];
			res[k]+=tab_of_delta_gamma_hedge[i]->delta->trans_cost[k];
		}
		for (int i=0;i<this->nb_maturities_to_hedge;++i)
		{
			res[k]+=tab_of_rho_hedge[i]->rho->trans_cost[k];
		}
	}
	return res;
}

// 8 - ADDING tables as input 
double* Launcher::extract_pnl_opt_fut_swp_glob(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge **tab_of_rho_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	for (int k=0;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.pnl_opts[k];
			res[k]+=tab_of_delta_gamma_hedge[i]->delta->pnl_eq_fut[k];
		}
		for (int i=0;i<this->nb_maturities_to_hedge;++i)
		{
			res[k]+=tab_of_rho_hedge[i]->rho->pnl_rt_swp[k];
		}
	}
	return res;
}

// 9 - ADDING tables as input
double* Launcher::extract_cash_part(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge ** tab_of_rho_hedge)
{
	double act;
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	res[0]=0;
	for (int k=1;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		res[k]=0;
		for (int i=0;i<this->nb_index_to_cover;++i)
		{
			res[k]+=tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.mkt_value[k-1]+tab_of_delta_gamma_hedge[i]->delta->mkt_value[k-1];
		}
		for(int i=0;i<this->nb_maturities_to_hedge;++i)
			res[k]+=tab_of_rho_hedge[i]->rho->mkt_value[k-1];
		/*if (k==32)
			double x=2;*/
		act= 1./tab_of_delta_gamma_hedge[0]->assumptions->nb_steps_per_year*
			tab_of_delta_gamma_hedge[0]->zero_coupon_rates->tab[k][1]+
			(1-1./tab_of_delta_gamma_hedge[0]->assumptions->nb_steps_per_year)*
			tab_of_delta_gamma_hedge[0]->zero_coupon_rates->tab[k][0];

		res[k]=(tab_of_delta_gamma_hedge[0]->hedgeff->liab_ov[k-1]-res[k])*
			act/
			tab_of_delta_gamma_hedge[0]->assumptions->nb_steps_per_year;
	}
	return res;
}



// 10 - ADDING tables as input 
double* Launcher::liab_ov_change(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	res[0]=tab_of_delta_gamma_hedge[0]->hedgeff->liab_ov[0];
	res[0]=0;
	for (int k=1;k<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
	{
		
			res[k]=tab_of_delta_gamma_hedge[0]->hedgeff->liab_ov[k]-
				tab_of_delta_gamma_hedge[0]->hedgeff->liab_ov[k-1];
		
	}
	return res;
}

// 11 - ADDING tables as input 
double* Launcher::extract_portfolio_pnl(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,Rho_Hedge **tab_of_rho_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	double* tmp1=this->extract_pnl_opt_fut_swp_glob(tab_of_delta_gamma_hedge, tab_of_rho_hedge);
	double* tmp2=this->extract_cash_part(tab_of_delta_gamma_hedge,tab_of_rho_hedge);
	double* tmp3=this->extract_fut_opt_swp_trans_cost_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge);
	for (int i=0;i<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++i)
	{

		res[i]=tmp1[i]
				+tmp2[i]
				-tmp3[i];
				

	}
	delete tmp1;
	delete tmp2;
	delete tmp3;
	return res;
}
// 12 - ADDING tables as input 
double* Launcher::extract_total_pnl(Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, Rho_Hedge ** tab_of_rho_hedge)
{
	double* res=new double [tab_of_delta_gamma_hedge[0]->assumptions->model_period];
	double* tmp1=this->extract_pnl_opt_fut_swp_glob(tab_of_delta_gamma_hedge, tab_of_rho_hedge);
	double* tmp2=this->extract_cash_part(tab_of_delta_gamma_hedge,tab_of_rho_hedge);
	double* tmp3=this->extract_fut_opt_swp_trans_cost_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge);
	double* tmp4=this->liab_ov_change(tab_of_delta_gamma_hedge);

	for (int i=0;i<tab_of_delta_gamma_hedge[0]->assumptions->model_period;++i)
	{
		res[i]=tmp1[i]
				+tmp2[i]
				-tmp3[i]
				-tmp4[i]
				+tab_of_delta_gamma_hedge[0]->hedgeff->Total_charges[i]
				-+tab_of_delta_gamma_hedge[0]->hedgeff->claims[i];
	}
	delete tmp1;
	delete tmp2;
	delete tmp3;
	delete tmp4;
	return res;
}

// Comments by Yuan and Yann
//double* Launcher::extract_int_cont_glob(int scen)
//{
//	
//	double* res=new double [this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period];
//	for (int k=0;k<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++k)
//	{
//		res[k]=0;
//		for (int i=0;i<this->nb_index_to_cover;++i)
//		{
//			res[k]+=this->tab_of_delta_gamma_hedge[scen][i]->delta->int_cont[k];
//		}
//	}
//	return res;
//}
// Comments by Yuan and Yann
//double* Launcher::extract_settle_cont_glob(int scen)
//{
//	
//	double* res=new double [this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period];
//	for (int k=0;k<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++k)
//	{
//		res[k]=0;
//		for (int i=0;i<this->nb_index_to_cover;++i)
//		{
//			res[k]+=this->tab_of_delta_gamma_hedge[scen][i]->delta->settle_cont[k];
//		}
//	}
//	return res;
//}


// Comments by Yuan and Yann
//double Launcher::return_total_pnl(int scen)
//{
//	double res=0;
//	double* tmp=this->extract_total_pnl(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}	
//	delete tmp;
//	return res;
//}


// Comments Yuan Yann
//double Launcher::return_total_fut_trans_costs(int scen)
//{
//	double res=0;
//	double* tmp=extract_fut_trans_cost_glob(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}

// Comments Yuan Yann
//double Launcher::return_total_pnl_opts_glob(int scen)
//{
//	double res=0;
//	double* tmp=extract_pnl_opt_glob(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}

// Comments Y & Y
//double Launcher::return_total_pnl_futs_glob(int scen)
//{
//	double res=0;
//	double* tmp=extract_pnl_eq_fut_glob(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}
// Comments by Yuan and Yann
//double Launcher::return_total_cash(int scen)
//{
//	double res=0;
//	double* tmp=extract_cash_part(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}

// Comments by Yuan and Yann
//double Launcher::return_total_liab_ov_change(int scen)
//{
//	double res=0;
//	double* tmp=liab_ov_change(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}

// Comments by Yuan and Yann
//double Launcher::return_total_opt_trans_costs(int scen)
//{
//	double res=0;
//	double* tmp=extract_opt_trans_cost_glob(scen);
//	for (int i=0;i<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++i)
//	{
//		res+=tmp[i];
//	}
//	delete tmp;
//	return res;
//}
void Launcher::display_report1(string outputs_path)
{
	/*string output_name=outputs_path+"\\gamma.csv";
	ofstream output(output_name.c_str());
	output<<"Scenario,Period";
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Gamma Liab "<<i;
	}
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Gamma Asset "<<i;
	}
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Opt Not "<<i;
	}
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Option Pursh "<<i;
	}
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Option Sales "<<i;
	}
	output<<endl;
	for (int j=0;j<this->nb_of_scen;++j)
	{
		
		for(int k=0;k<this->tab_of_delta_gamma_hedge[0][0]->assumptions->model_period;++k)
		{
			output<<j<<","<<this->tab_of_delta_gamma_hedge[0][0]->hedgeff->period[k];
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<<this->tab_of_delta_gamma_hedge[j][i]->hedgeff->gamma_liab[k];
			}
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<<this->tab_of_delta_gamma_hedge[j][i]->gamma->hand_calculate.gamma_new_asset[k];
			}
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<<this->tab_of_delta_gamma_hedge[j][i]->gamma->hand_calculate.opt1_not[k];
			}
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<<this->tab_of_delta_gamma_hedge[j][i]->gamma->hand_calculate.option_pursh[k];
			}
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<<this->tab_of_delta_gamma_hedge[j][i]->gamma->hand_calculate.option_sales[k];
			}
			output<<","<<this->extract_pnl_opt_glob(j)[k];
			output<<endl;
		}
	}*/

}

void Launcher::display_header(string outputs_path,Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge, int start_scn, int end_scn)
{

	ostringstream str_scn;
	ostringstream ed_scn;

	str_scn << start_scn;
	ed_scn << end_scn;


	string output_name=outputs_path + "_" + str_scn.str() + "_"  + ed_scn.str();

	cout << " output_name " << output_name  << endl;

	ofstream output(output_name.c_str());
	output<<"Scenario,Period";
	output<<",Option Value";
	for (int i=1;i<=this->nb_index_to_cover;++i)
	{
		output<<",Delta Liab "<<i;
		output<<",Asset Delta Before Futures"<<i;
		output<<",Net Delta Before Futures"<<i;
		output<<",Asset Delta After Futures"<<i;
		output<<",Net Delta After Futures"<<i;
		output<<",Eq Fut Ct "<<i;
		output<<",Future Pursh "<<i;
		output<<",Future Sales "<<i;
		output<<",Market Value (Fut) "<<i;
	//	output<<",Total Notional "<<i;
		output<<",Gamma Liab "<<i;	
		output<<",Asset Gamma Before Options"<<i;
		output<<",Net Gamma Before Options"<<i;
		output<<",Asset Gamma After Options"<<i;
		output<<",Net Gamma After Options"<<i;;	
		output<<",Opt Not "<<i;
		output<<",Eq Put Ct "<<i;
		output<<",Option Pursh "<<i;
		output<<",Option Sales "<<i;
		output<<",Market Value (Opt)"<<i;
	}

	for (int i=0;i<this->nb_maturities_to_hedge;++i)
	{
		output<<",Rho Liab "<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Asset Rho Before Swaps"<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Net Rho Before Swaps"<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Asset Rho After Swaps"<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Net Rho After Swaps"<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Rt Swp Ct "<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Swap Pursh "<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Swap Sales "<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
		output<<",Market Value (Swp) "<<tab_of_delta_gamma_hedge[0]->assumptions->maturities_to_hedge[i];
	}
	//output<<",Int Cont";
	//output<<",Settle Cont";
	output<<",Pnl Eq Fututres";
	output<<",Fut Trans Costs";
	output<<",Pnl Opt";
	output<<",Opt Trans Costs";
	output<<",Pnl Rt Swaps";
	output<<",Swp Trans Costs";
	output<<",Trans Costs Opt + Fut + Swp";	
	output<<",Pnl Opt + Fut + Swp";	
	output<<",Cash";
	output<<",Option value Change";
	output<<",Total_charges";
	output<<",claims";
	output<<",Porftolio Pnl";
	output<<",TOTAL PNL";

//chth221209
	//output<<this->my_chth;

	//output<<",Trans Costs";

	//output<<",Int Cont";
	//output<<",Settle Cont";

	//output<<",Trans Costs";
	output<<endl;

};




void Launcher::display_report1_bis(string outputs_path, 
								   Delta_Gamma_Hedge ** tab_of_delta_gamma_hedge,
								   Rho_Hedge ** tab_of_rho_hedge,
								   int nb_of_scenarios, int start_scn, int end_scn)
{

	ostringstream str_scn;
	ostringstream ed_scn;

	str_scn << start_scn;
	ed_scn << end_scn;


	string output_name=outputs_path + "_" + str_scn.str() + "_"  + ed_scn.str();
	ofstream output(output_name.c_str(), ios::app);
		for(int k=0;k< tab_of_delta_gamma_hedge[0]->assumptions->model_period;++k)
		{
			output<< nb_of_scenarios + 1<<","<<k;
			output<<","<< tab_of_delta_gamma_hedge[0]->hedgeff->liab_ov[k];
			
			for (int i=0;i<this->nb_index_to_cover;++i)
			{
				output<<","<< tab_of_delta_gamma_hedge[i]->delta->liability_delta.delta_liab[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->asset_delta.asset_delta_before_futures[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->net_delta_before_futures[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->asset_delta.asset_delta_after_futures[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->net_delta_after_futures[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->nb_of_futures.eq_fut_1_ct[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->future_pursh[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->future_sales[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->delta->mkt_value[k];				
				
			//	output<<","<<tab_of_delta_gamma_hedge[j][i]->delta->total_notional[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->hedgeff->gamma_liab[k];

				//output<<","<<tab_of_delta_gamma_hedge[j][i]->option_trade->tab_option_trade_struct[k].new_net_gamma+tab_of_delta_gamma_hedge[j][i]->option_trade->tab_option_trade_struct[k].gamma_liability;
			
				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->asset_gamma_before_options[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->net_gamma_before_options[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->asset_gamma_after_options[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->net_gamma_after_options[k];
			
				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.opt1_not[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.eq_put_1_ct[k];
			
				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.option_pursh[k];
			
				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.option_sales[k];

				output<<","<<tab_of_delta_gamma_hedge[i]->gamma->hand_calculate.mkt_value[k];



			}
			for (int i=0;i<this->nb_maturities_to_hedge;++i)
			{
				output<<","<< tab_of_rho_hedge[i]->rho->liability_rho.rho_liab[k];
				output<<","<< tab_of_rho_hedge[i]->rho->asset_rho.asset_rho_before_swaps[k];
				output<<","<< tab_of_rho_hedge[i]->rho->net_rho_before_swaps[k];
				output<<","<< tab_of_rho_hedge[i]->rho->asset_rho.asset_rho_after_swaps[k];
				output<<","<< tab_of_rho_hedge[i]->rho->net_rho_after_swaps[k];
				output<<","<< tab_of_rho_hedge[i]->rho->nb_of_swaps.rt_swp_1_ct[k];
				output<<","<< tab_of_rho_hedge[i]->rho->swap_pursh[k];
				output<<","<< tab_of_rho_hedge[i]->rho->swap_sales[k];
				output<<","<< tab_of_rho_hedge[i]->rho->mkt_value[k];
			}
			
			output<<","<<this->extract_pnl_eq_fut_glob(tab_of_delta_gamma_hedge)[k];
			output<<","<<this->extract_fut_trans_cost_glob(tab_of_delta_gamma_hedge)[k];
			output<<","<<this->extract_pnl_opt_glob(tab_of_delta_gamma_hedge)[k];
			output<<","<<this->extract_opt_trans_cost_glob(tab_of_delta_gamma_hedge)[k];
			output<<","<<this->extract_pnl_rt_swp_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];
			output<<","<<this->extract_swp_trans_cost_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];
			output<<","<<this->extract_fut_opt_swp_trans_cost_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];
			output<<","<<this->extract_pnl_opt_fut_swp_glob(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];	
			output<<","<<this->extract_cash_part(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];
			output<<","<<this->liab_ov_change(tab_of_delta_gamma_hedge)[k];
			output<<","<< tab_of_delta_gamma_hedge[0]->hedgeff->Total_charges[k];
			output<<","<< tab_of_delta_gamma_hedge[0]->hedgeff->claims[k];
			output<<","<<this->extract_portfolio_pnl(tab_of_delta_gamma_hedge, tab_of_rho_hedge)[k];
			output<<","<<this->extract_total_pnl(tab_of_delta_gamma_hedge,tab_of_rho_hedge)[k];
			output<<endl;
		}
}

Launcher::~Launcher(void)
{// Comments by Yuan and Yann
	//for (int j=0;j<nb_of_scen;++j)
	//{
	//	for (int i=0;i<nb_index_to_cover;++i)
	//	{
	//		this->tab_of_delta_gamma_hedge[j][i]->erase_objects();
	//	}
	//}
}