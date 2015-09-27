#ifndef SCENARIO_H
#define SCENARIO_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "tools.h"

using namespace std;



class Scenario{

private:
	Basic_Information BI;
		
public:	
		//double *** ScenarioFang;
	double ***scenario;
		//SmartPointer<double**> scenario;

		//double *** Random;
		//SmartPointer<double**> random;
	double *** random;
		

public:
	    //! -- Yuan Ajouter les tailles dans les parametres 
		//Fang ajoute deux paramètres: coupon_period(pour calculer le swap rate
		//et flag(pour décider si on utilise ou pas la variable d'état
      	Scenario(int nb_scen,bool ant,int nb_eq,int nb_bond,int nb_rate,
				int nb_step,int nb_initial,int nb_vol,double a,double sigma,int delta_t,
				double *error,   int error_size,
				double *add_error,  int add_error_size,
				int *max_try,  int max_try_size,
				double **vol,    //int vol_size,
				double *m_bond,  int m_bond_size,
				double *bond_coupon, int bond_coupon_size,
				double *bond_freq, int bond_freq_size,
				double *m_rate,  int m_rate_size,
				int *rate_type, int rate_type_size,
				double *rate_period, int rate_period_size,
				double *rate,    int rate_size,
				double **cor,    int cor_size/*,
				string* equity,  int equity_size*/);
		

	Scenario(Scenario &a);

	~Scenario();
	
	Basic_Information & bi(){return BI;}

	void modif_martingality(int i){BI.increase_martingality(i);}

	void affiche(ofstream &scen, int precision, string* equity);

	void initial(Basic_Information BI);

	double*** get_scenario(){return scenario;}

	void convert_vol(int index, double a, double sigma, double correl, bool HW, bool vol_loc, bool debug_info, ofstream &filevol);

};



#endif