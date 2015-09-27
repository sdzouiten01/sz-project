#pragma once
#ifndef DELTA_ASSET_H
#define DELTA_ASSET_H
#include "Math_Tools.h"

class Assumptions;
class Hedgeff;
class Rf_Futures;
/**
* Class for deltas of new futures
*/
class Delta_Asset
{
public:
	struct Objects_Needed
	{
		Assumptions* assumptions;
		Hedgeff* hedgeff;
		Rf_Futures* rf_futures;
	}objects_needed;
	struct Characteristics
	{
		//double shock_size;
	} characteristics;
	/*typedef enum {Asset_Dwn_1, Base, Asset_Up_1,Nb_Cols_In_Fund} Fund_Cols;
	typedef enum {S,D1,D2,N_MD1,N_MD2,MvUnit,Amount,Mv,Nb_Rows_In_Fund} Fund_Rows;*/
	struct Delta_Asset_Struct
	{
		int time;
		double k;
		double r;
		double q;
		double tte;
		double sigma;
		//double ** fund;
		double delta;
	} * tab_of_delta_asset_struct;
	void init_tab_of_delta_asset_struct();
	void init_time(int i);
	//void init_fund(int i);
	void init_k(int i);
	void init_r(int i);
	void init_q(int i);
	void init_tte(int i);
	void init_sigma(int i);
	void init_delta(int i);
	Delta_Asset(Assumptions* assumptions,Hedgeff* hedgeff,Rf_Futures* rf_futures);
	~Delta_Asset(void);
};
#endif