#ifndef _INTERPOLATION_H
#define _INTERPOLATION_H
#include "Utility classes/Normal.h"
#include "GMXB_Assumption_Table.h"

class Interpolation
{

	private:
		/* Table as an input for the interpolation */
		GMXB_Assumption_Table * table;
		
		/* column for two or multidimension tables */
		int column;
	
	public:
		Interpolation(GMXB_Assumption_Table * table, int column)
		{
			this->table = table;
			this->column = column;

		};
		
		double linear_interpolation(double x)
		{

			double x1 = floor(x);
			double x2 = ceil(x);
		
			if (x1 != x2)
				return table->search((int)x1, this->column) + ((x - x1)/(x2 - x1))*(table->search((int)x2, this->column) - table->search((int)x1, this->column));
			else
				return table->search((int)x, this->column);
		};

};

#endif


