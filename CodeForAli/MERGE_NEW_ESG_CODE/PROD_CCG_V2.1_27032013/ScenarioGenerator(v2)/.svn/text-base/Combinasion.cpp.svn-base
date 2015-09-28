#include "Combinasion.h"
#include <math.h>

// Creates correlated Gaussian variables (but which should still be of variance 1)
// Independent gaussian variables in random for the index position_index are transformed into variables
// correctly correlated with the previous gaussian variables generated for smaller indexes (and stocked in scenario)
// This corresponds to a multiplication of the matrix BI.cor obtained by Cholesky with the vector of independent gaussian variables.
void combination(int position_index,int position_step,Basic_Information BI,Scenario *s,int flag){
	int n=BI.nb_scens()/BI.antithetic();
	// In this case the second rate variable is generated correlated with the first one and stocked
	// in scenario with position_index = 0 (this remained empty if flag == 0).
	// This means that the matrix BI.cor has one additional column so there is a shift in position_index with this matrix.
	for (int j = 0; j<n; j++) {
		s->scenario[3+position_index+BI.nb_rate()][position_step+1][j] = 0;
		for ( int k = 0 ; k <= position_index+1 ; k++ ){
			s->scenario[3+position_index+BI.nb_rate()][position_step+1][j] += BI.cor(position_index+1,k)*s->random[k][position_step][j];
		}
	}
}