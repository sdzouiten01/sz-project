#include "test.h"

void correlation(Scenario *s,Basic_Information BI){
	for(int i=0;i<BI.nb_eq();i++){
		for(int j=0;j<BI.nb_step();j++){
			for(int k=0;k<BI.nb_scens()/BI.antithetic();k++){
				s->random[i+1][j][k]=s->scenario[4+BI.nb_rate()+i][j+1][k];
			}
		}
	}
	double cof;
	ofstream out("correlation.csv");
	for(int i=0;i<BI.nb_step();i++){
		for(int j=0;j<BI.nb_eq()+1;j++){
			for(int k=0;k<=j;k++){
				cof=0;
				for(int l=0;l<BI.nb_scens()/BI.antithetic();l++){
					cof+=s->random[j][i][l]*s->random[k][i][l]/(BI.nb_scens()/BI.antithetic());
				}
				out<<cof<<",";
			}
			out<<endl;
		}
		out<<endl;
	}
}

