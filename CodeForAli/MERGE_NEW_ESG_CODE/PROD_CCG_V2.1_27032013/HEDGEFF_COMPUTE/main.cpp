#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include "Portfolio.h"
using namespace std;

// 9964
int main()
{
	/*
	Portfolio portfolio;
	portfolio._isHedgeGamma = true;
	portfolio._isHedgeDelta = true;
	portfolio._isHedgeRho = true;
	portfolio._isHedgeRhoConv = false;

	//portfolio.loadConfig("./config/Liab_Output_Config.txt", "./input/Agreg_mpout_dJ_baseline.csv", "./input/Scenarios_D_BC_4.csv");
	//portfolio.hedging("./input/Agreg_mpout_dJ_baseline.csv", "./input/Scenarios_D_BC_4.csv", 1, 1, "./rlt/hedge_rlt_Agreg_mpout_dJ_baseline.csv");
	portfolio.loadConfig("./config/Liab_Output_Config.txt", "./input/MP_Agreg_Baseline.csv", "./input/Scenarios-France_sans_RiskB.csv");
	portfolio.hedging("./input/MP_Agreg_Baseline.csv", "./input/Scenarios-France_sans_RiskB.csv", 1, 1, "./hedge_MP_Agreg_Baseline-08-26_gamma_2.csv");
	return 0;
	*/
	int N;
	bool isHedgeGamma, isHedgeDelta, isHedgeRho;
	isHedgeGamma = false;
	isHedgeDelta = false;
	isHedgeRho = false;
	ifstream fin("./AXA_HedgeEffective.txt");
	fin >> isHedgeGamma;
	fin >> isHedgeDelta;
	fin >> isHedgeRho;
	fin >> N;
	cout << "Hedge for " << N << " files " << endl;

	cout << "Hedge ";
	if ( isHedgeGamma )
	{
		cout << " Gamma ";
	}
	if ( isHedgeDelta )
	{
		cout << " Delta ";
	}
	if ( isHedgeRho )
	{
		cout << " Rho ";
	}
	cout << endl;
	string liab_file_name, scenario_name, config_name, rlt_name;
	int start_scen, end_scen;
	for (int i = 0; i < N; ++i)
	{
		fin >> liab_file_name;
		fin >> scenario_name;
		fin >> config_name;
		fin >> rlt_name;
		fin >> start_scen >> end_scen;

		Portfolio portfolio;
		portfolio._isHedgeGamma = isHedgeGamma;
		portfolio._isHedgeDelta = isHedgeDelta;
		portfolio._isHedgeRho = isHedgeRho;
		portfolio._isHedgeRhoConv = false;

		portfolio.loadConfig(config_name, liab_file_name, scenario_name);
		//portfolio.hedging(liab_file_name, scenario_name, start_scen, end_scen, rlt_name);
	}

	return 0;
}