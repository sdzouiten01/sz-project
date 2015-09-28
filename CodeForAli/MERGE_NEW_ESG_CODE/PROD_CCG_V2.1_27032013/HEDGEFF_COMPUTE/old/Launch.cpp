#include "Launcher.h"
//#include "Export.h"
using namespace std;
// 
//// function to return the current working directory
//// this is generally the application path
//void GetCurrentPath(char* buffer)
//{
//	getcwd(buffer, _MAX_PATH);
//}
//
//

void myFunction()
{

	Launcher* launcher;
	//int k;

	int nb_of_scen=10;
	int nb_index_to_cover=1;
	char* hedgeff_path="C:\\HEDGEEFF_ASSET\\NEEDED\\Inputs\\mp1_p06.csv";
	char* rw_scn_path="C:\\HEDGEEFF_ASSET\\NEEDED\\Inputs\\RW.csv";
	int model_period=142;
	char* outputs_path="C:\\HEDGEEFF_ASSET\\NEEDED\\NEEDED\\Outputs";
	double moneyness=1;
	int reb_freq_delta=1;
	int reb_freq_gamma=1;
	int reb_freq_rho=1;
	double volatility=0.2;
	int length_of_option_contract=24;
	int sell_at_time_to_expiry=12;
	int	length_of_futures_contract=1;
	double gamma_per_cont=1;
	double delta_per_cont=1;
	double net_gamma_lower_bound=0;
	double net_gamma_upper_bound=0;
	double liab_gamma_lower_bound=0;
	double liab_gamma_upper_bound=0;
	double delta_transaction_cost=1;
	double option_transaction_cost=10;
	double rho_transaction_cost=10;
    bool hedge_delta=true;
	bool hedge_gamma=false;
	bool hedge_rho=true;
	bool rates_freq_ann=true;
    bool sheduled_futures=false;
    int next_time_step_sheduled=1;
	int	nb_steps_per_year=12;
	int nb_maturities_to_hedge=10;
	double* maturities_to_hedge=new double[nb_maturities_to_hedge];
	double* shocks_to_apply=new double[nb_maturities_to_hedge];
	double* type_of_swaps=new double[nb_maturities_to_hedge];
	double* compounding_frequencies=new double[nb_maturities_to_hedge];
	double* swap_rates=new double[nb_maturities_to_hedge];

	//cout << "enter " << endl;
	//getchar();

	maturities_to_hedge[0]=1;
	maturities_to_hedge[1]=3;
		maturities_to_hedge[2]=5;
		maturities_to_hedge[3]=7;
		maturities_to_hedge[4]=10;
		maturities_to_hedge[5]=15;
		maturities_to_hedge[6]=20;
		maturities_to_hedge[7]=30;
		maturities_to_hedge[8]=40;
		maturities_to_hedge[9]=50;
	//maturities_to_hedge[1]=10;
	//maturities_to_hedge[2]=20;
	for (int i=0; i<10;i++)
	{	shocks_to_apply[i]=0.1;
	compounding_frequencies[i]=6;
}
	//compounding_frequencies[1]=6;
	//compounding_frequencies[2]=6;
	double* index_bases=new double[nb_index_to_cover];
	double* index_to_hedge=new double[nb_index_to_cover];
	double* scen_to_hedge=new double[nb_of_scen];
	for (int i=0;i<nb_of_scen;++i)
	{
		scen_to_hedge[i]=i+1;
	}
	for (int i=0;i<nb_index_to_cover;++i)
	{
		index_bases[i]=1000;
		index_to_hedge[i]=i+1;
	}
	//chth221209
	//int my_chth;
		//chth221209-2
	double* start_mty=new double[nb_maturities_to_hedge];
		for (int i=0;i<nb_maturities_to_hedge;++i)
	{
		start_mty[i]=i;
	}
	double* end_mty=new double[nb_maturities_to_hedge];
		for (int i=0;i<nb_maturities_to_hedge;++i)
	{
		end_mty[i]=i;
	}

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
		rho_transaction_cost,
		option_transaction_cost,
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

	/*
int main()
{
		cout << "Beginning program..." << endl;
		myFunction();
		cout<< "End of program. Press any key to quit."<<endl;
		getchar();
	return 0;
}
*/

