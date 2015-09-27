#pragma once
#include "Regression.h"						
const double BIG = 10000000000;								

class LS
{
	private:
		int nb_time_steps;
		int nb_scenarios;					
		int nb_informations;								
		int first_lapse_year;
		double** Trajectories;							
		double*** Explicative_var;			//explicative variables used for the regression in Longstaff				
		double** Proba_Exercice_traj_by_traj;
		double length_smoothing_assump;						
		double length_smoothing;
		double param_bar;
		double* value;						// continuation value estimated by regression if LS; explicative variable if Boundary
		double* true_continuation_value;					
		string longstaff_method;

		string debug_info;
		string proba_info; // m
		string file_name;
		double** Regression_parameters;
		double* Boundary_value;
		double* sum_value_table;							
		double* bar_coord;									
	
	public:
		LS(int t0,int n0, int p0, int t, double param_bar_assump, double length_smoothing_0, double length_smoothing_LS, double** Traj, double*** Var_exp, string method, string debug, string probability, double** Reg_parameters, double* Boundary, double** proba_traj_by_traj)
		{
			nb_time_steps = t0;
			nb_scenarios = n0;
			nb_informations = p0;
			first_lapse_year = t;
			Trajectories = Traj;
			Explicative_var = Var_exp;
			longstaff_method= method;
			

			value = new double [nb_scenarios];
			true_continuation_value = new double [nb_scenarios];
			length_smoothing_assump = length_smoothing_0;
			length_smoothing = length_smoothing_LS;
			param_bar = param_bar_assump;

			bar_coord = new double [nb_scenarios];						
			sum_value_table = new double [nb_scenarios];				
			
			debug_info = debug;
			proba_info = probability; // mihaela
			Regression_parameters = Reg_parameters;
			Boundary_value = Boundary;
			Proba_Exercice_traj_by_traj = proba_traj_by_traj; // m

		}							
		~LS(){
			delete[] value;
			delete[] true_continuation_value;
			delete[] bar_coord;
			delete[] sum_value_table;
						
		}

		void executeLS(){
			
			double proba_out;
			for (int i=nb_time_steps-2; i>=first_lapse_year-1; i--){

				double* Traj_reg = new double [nb_scenarios];
				for (int j=0; j<nb_scenarios; j++){
					Traj_reg[j] = Trajectories[i+1][j] - Trajectories[i][j];		
				}

				double* coeff ;
				
				// we first obtain the regression coefficients for LS
				if (this->longstaff_method=="LS" || this->longstaff_method=="LS+Boundary"){
					Regression reg = Regression(nb_scenarios, nb_informations, Traj_reg, Explicative_var[i]);
					bool not_linearly_linked = reg.Calculate();						
					coeff = new double [nb_informations];
					if(not_linearly_linked){
						reg.result(coeff);											
					}
					else{															
						for(int k=0; k<nb_informations; k++){
							coeff[k] = 0;
						}
						for(int j=0; j<nb_scenarios; j++){
							coeff[0] += Traj_reg[j];
						}
						coeff[0] /= nb_scenarios;
					}																
																					
					if(debug_info == "Y" || debug_info == "N"){
						for(int k=0; k<nb_informations; k++){
							Regression_parameters[i][k] = coeff[k];
						}															
																					
					}
					if(debug_info == "USE"){
						for(int k=0; k<nb_informations; k++){
							coeff[k] = Regression_parameters[i][k];
						}
					}																
																					
				}
				else{
					 coeff=NULL;
				}																	

				delete [] Traj_reg;

				double frontier = 0;
				
				// obtains the frontier value
				if(this->longstaff_method=="LS+Boundary" || this->longstaff_method=="Boundary"){
					frontier = this->sortLS(i, coeff);
					if(debug_info == "Y" || debug_info == "N"){
						Boundary_value[i] = frontier;
					}
					if(debug_info == "USE"){
						frontier = Boundary_value[i];
					}
				}

				// then we obtain the estimated continuation value for LS
				// for boundary, we need the explicative variable value
				for (int j=0; j<nb_scenarios; j++){
					double value = 0;
					
					if(this->longstaff_method=="LS" || this->longstaff_method=="LS+Boundary"){ 
						for (int k=0; k<nb_informations; k++){
							value += coeff[k] * Explicative_var[i][k][j];
						}
					}
					else{
						value=- Explicative_var[i][1][j];
						length_smoothing = length_smoothing_assump * Explicative_var[i][1][j]; 
					}

					// first type of smoothing
					// linear lapse for a certain interval around the frontier
					if(abs( - value + frontier) >= length_smoothing){
						proba_out = 1;
						if( - value + frontier < 0){
							proba_out = 0;
						}
					}
					else{
						proba_out = ( - value + frontier)/(2*length_smoothing) + 0.5;
					}
					Trajectories[i][j] = proba_out * Trajectories[i][j] + (1 - proba_out) * Trajectories[i+1][j];
					if (proba_info == "Y" || proba_info == "N")// m
						Proba_Exercice_traj_by_traj[i][j] = proba_out;
					if (proba_info == "USE") // m
						Proba_Exercice_traj_by_traj = this->Proba_Exercice_traj_by_traj; // m
						
				}
				if (coeff!= NULL){
					delete [] coeff;
				}

			}
			
		}

		//returns the frontier for the boundary method
		double sortLS(int i, double* coeff){
			for (int j=0; j<nb_scenarios; j++){
				value[j] = 0;
				if (coeff==NULL){
					value[j] = - Explicative_var[i][1][j];
				}																			
				else{
					for (int k=0; k<nb_informations; k++){
						value[j] += coeff[k] * Explicative_var[i][k][j];
					}																		
				}
			}
			for (int j=0; j<nb_scenarios; j++){
				true_continuation_value[j] = Trajectories[i+1][j] - Trajectories[i][j];
			}

			

			this->sort(0, nb_scenarios-1);													
			sum_value_table[nb_scenarios-1] = true_continuation_value[nb_scenarios-1];		
			for (int j=nb_scenarios-2; j>=0; j--){
				sum_value_table[j] = sum_value_table[j+1] + true_continuation_value[j];
				}																
			//double frontier = this->frontier();
			//double frontier = this -> bar_frontier();						
			double frontier = this -> frontier_new();					
			return frontier;
		}
		// sorts increasingly the estimated continuation value vector
		// corresponding order in the true continuation value vector
		void sort(int start, int end){
			if (start == end){}
			else{
				double pivot = value[start];
				double pivot_true_continuation_value = true_continuation_value[start];
				int pos_pivot = start;
				for (int j=start+1; j<=end; j++){
					if (value[j] < pivot){
						value[pos_pivot] = value[j];
						value[j] = value[pos_pivot+1];
						value[pos_pivot+1] = pivot;
						true_continuation_value[pos_pivot] = true_continuation_value[j];
						true_continuation_value[j] = true_continuation_value[pos_pivot+1];
						true_continuation_value[pos_pivot+1] = pivot_true_continuation_value;
						pos_pivot++;					
					}
				}
				sort(start, max(pos_pivot - 1,start));
				sort(min(pos_pivot+1, end), end);
			}
		}																					
		
		
		// computes the frontier value
		// if the second smoothing coefficient is zero, 
		// it just keeps the scenario which gives the max payoff and returns 
		// as frontier the value of the exp var for this scenario
		// else it builds a barycenter of values of the explicative variable

		double frontier_new() {

			int ind_max = 0;
			double max = -BIG;
			for (int j=nb_scenarios-1; j>=0; j--){
				if(j==0){
					if (sum_value_table[j] > max){
						ind_max = j;
						max = sum_value_table[j];
					}
				}
				else if (abs (value[j] - value[j-1]) > epsilon){
					if (sum_value_table[j] > max){
						ind_max = j;
						max = sum_value_table[j];
					}
				}
			}

			int ind_min = 0;
			double min = BIG;

			for (int j=nb_scenarios-1; j>=0; j--){
				if(j==0){
					if (sum_value_table[j] < min){
						ind_min = j;
						min = sum_value_table[j];
					}
				}
				else if (abs (value[j] - value[j-1]) > epsilon){
					if (sum_value_table[j] < min){
						ind_min = j;
						min = sum_value_table[j];
					}
				}
			}
			
			if (ind_max==0){
				return -BIG;
			}
			else if (ind_max==nb_scenarios){
				return BIG;
			}

			else if (param_bar <= epsilon)
				return ((value[ind_max] + value[ind_max - 1])/2);

			else {
				double ecart = sum_value_table[ind_max]-sum_value_table[ind_min];
			
			
				this->bar_coord[ind_max] = 1;
				for (int j = 0; j< nb_scenarios; j++){
					if(j!=ind_max){	
						double k = (sum_value_table[ind_max] - sum_value_table[j])/(0.001 * ecart);						
						bar_coord[j] = max(1 - k * 0.001 * (1/param_bar), 0);
					}
				}
				double normalisation_coefficient = 0;
				for (int j = 0; j< nb_scenarios; j++){
					normalisation_coefficient += bar_coord[j];
					}
				double baryc_frontier = 2 * bar_coord[0] * value[0];

				for (int j = 1; j< nb_scenarios; j++)
					baryc_frontier += bar_coord[j] * (value[j] + value[j-1]);

				baryc_frontier /= 2 * normalisation_coefficient;

				return baryc_frontier;
			}
		}

		double** proba_exercise_traj_by_traj(){
			return Proba_Exercice_traj_by_traj;
		}
};
