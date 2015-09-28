#ifndef EQUITY_MODEL_H
#define EQUITY_MODEL_H

#include "tools.h"

class Equity_Model{

public:
	   //modele black scholes, le prix calcule sont le prix actualise
	   static void Black_Sholes(double* output, double vol,Basic_Information &BI,double delta_t);
};

#endif