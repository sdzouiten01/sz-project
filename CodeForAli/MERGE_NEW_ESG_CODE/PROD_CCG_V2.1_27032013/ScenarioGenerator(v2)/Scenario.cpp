#include "Scenario.h"
#include "GMXBException.h"

#include <math.h>

//! Yuan modifié par yuan, ajouter les tailles de table* dans les parametres
Scenario::Scenario(int nb_scen,bool ant,int nb_eq,int nb_bond,int nb_rate,
	    int nb_step,int nb_initial,int nb_vol,double a,double sigma,int delta_t,
		double *error,   int error_size,
		double *add_error,  int add_error_size,
		int * max_try,  int max_try_size,
		double **vol,    //int vol_size,
	    double *m_bond,  int m_bond_size,
		double *bond_coupon, int bond_coupon_size,       
		double *bond_freq, int bond_freq_size,
		double *m_rate,  int m_rate_size,
		int *rate_type, int rate_type_size,
		double *rate_period, int rate_period_size, 
		double *rate,    int rate_size,
		double **cor,    int cor_size/*,
		string* equity,   int equity_size*/)
{
	int antithetic;
	if(ant==true){antithetic=2;}
	else{antithetic=1;}

	BI.copy(nb_scen,antithetic,nb_eq,nb_bond,nb_rate,
			nb_step,nb_initial,nb_vol,a,sigma,delta_t,
			error,   error_size,
			add_error,  add_error_size,
			max_try,  max_try_size,
			vol,     //vol_size,
			m_bond,  m_bond_size,
			bond_coupon, bond_coupon_size,
			bond_freq, bond_freq_size,
			m_rate,  m_rate_size,
			rate_type, rate_type_size,
			rate_period, rate_period_size,
			rate,    rate_size,
			cor,     cor_size/*,
			equity,  equity_size*/);

	int n=4+BI.nb_eq()+BI.nb_bond()+BI.nb_rate();
	int m=2+BI.nb_eq();
	scenario=new double **[n];
	random=new double **[m];
	for(int i=0; i<n; i++){
		scenario[i]= new double * [BI.nb_step()+1];
		for(int j=0;j<BI.nb_step()+1;j++){
			scenario[i][j]=new double [BI.nb_scens()];
		}
	}
	for(int i=0;i<m;i++){
		random[i]=new double *[BI.nb_step()];
		for(int j=0;j<BI.nb_step();j++){
			random[i][j]=new double [BI.nb_scens()/BI.antithetic()];
		}
	}
}

Scenario::Scenario(Scenario &a){
	BI.copy(a.BI);
	scenario=new double **[4+BI.nb_eq()+BI.nb_bond()+BI.nb_rate()];
	random=new double **[2+BI.nb_eq()];
	for(int i=0; i<4+BI.nb_eq()+BI.nb_bond()+BI.nb_rate(); i++){
		scenario[i]= new double * [BI.nb_step()+1];
		for(int j=0;j<BI.nb_step()+1;j++){
			scenario[i][j]=new double [BI.nb_scens()];
		}
	}
	for(int i=0;i<=2+BI.nb_eq();i++){
		random[i]=new double *[BI.nb_step()];
		for(int j=0;j<BI.nb_step();j++){
			random[i][j]=new double [BI.nb_scens()/BI.antithetic()];
		}
	}
}

//! --Yuan : supprimer tous les explicite destructor of SmartPointer
//! --Yuan : le table n'est pas contruit dans le constructeur mais detruit dans le destructeur, Dangereux !!!
Scenario::~Scenario(){
		int n=4+BI.nb_eq()+BI.nb_bond()+BI.nb_rate();
		int m=2+BI.nb_eq();
		for(int i=0; i<n; i++){
			for(int j=0;j<BI.nb_step()+1;j++){
				delete[] scenario[i][j];
			}
			delete[] scenario[i];
		}
		delete[] scenario;
		for(int i=0;i<m;i++){
			for(int j=0;j<BI.nb_step();j++){
				delete[] random[i][j];
			}
			delete[] random[i];
		}
		delete[] random;
}



void Scenario::affiche(ofstream &scen, int precision, string* equity){
	cout<<endl<<"Writing the scenario file : ";
	scen<<"iteration"<<","<<"period"<<","<<"short_rate"<<",";
	for(int i=0;i<BI.nb_rate();i++){
		scen<<"nom_rate_"<<BI.m_rate(i)<<"yr"<<",";
	}
	scen<<"beta"<<",";
	for(int i=0;i<BI.nb_eq();i++){
		//scen<<BI.name_eq(i)<<"_return"<<",";
		scen<<equity[i]<<"_return"<<",";
	}
	for(int i=0;i<BI.nb_bond();i++){
		scen<<"nom_returnT"<<BI.m_bond(i)<<"C"<<BI.bond_coupon(i)<<",";
	}
	scen<<endl;
	double temp=0.;
	for(int i=0;i<BI.nb_scens();i++){
		for(int j=0;j<BI.nb_step()+1;j++){
			for(int k=0;k<BI.nb_eq()+4+BI.nb_rate()+BI.nb_bond();k++){
				if( k==(BI.nb_eq()+4+BI.nb_rate()+BI.nb_bond()-1) )
					temp=(exp(log(1. + scenario[k][j][i]/100. ) / 12.)-1.)*100.;
				else 
					temp=scenario[k][j][i];

				scen<<setprecision(precision)<< temp  <<"," ;
			}
			scen<<endl;
		}
	}
	cout<<"finished"<<endl<<endl;
}



void Scenario::initial(Basic_Information BI){
	for(int j=0;j<BI.nb_scens();j++){
		for(int i=0;i<BI.nb_step()+1;i++){
			this->scenario[0][i][j]=j+1;
			this->scenario[1][i][j]=i;
		}
		
		this->scenario[2][0][j]=0;
		this->scenario[3+BI.nb_rate()][0][j]=1;
		for(int i=0;i<BI.nb_eq();i++){
			this->scenario[4+BI.nb_rate()+i][0][j]=1;
		}
	}
}

void Scenario::convert_vol(int index, double a, double sigma, double correl, bool HW, bool vol_loc, bool debug_info, ofstream &filevol){
	int nb_vol = BI.nb_vol();
	double * vol = BI.vol_eq(index);
	SmartPointer<int> time = BI.vol_t();
	if(debug_info==true){
		filevol<<"Maturity"<<",";
		for(int i=0;i<nb_vol;i++){
			filevol<<time[i]<<",";
		}
		filevol<<endl;
		filevol<<"Vol_Imp_BS"<<",";
		for(int i=0;i<nb_vol;i++){
			filevol<<setprecision(4)<<vol[i]<<",";
		}
		filevol<<endl;
	}

	if(HW==true){
		if(abs(a)<1e-8){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Impossible vol conversion : a too small");
			throw new GMXBException(message);
		}
		for(int i=0;i<nb_vol;i++){
			double mat = time[i] / 12.;
			if(abs(mat)<1e-8){
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Impossible vol conversion : maturity equals 0");
				throw new GMXBException(message);
			}
			double v1 = ((sigma * sigma) / (a * a)) * (mat + (2 / a) * exp(-a * mat) - (1 / (2 * a)) * exp(-2 * a * mat) - (3 / (2 * a)));
			double const1 = 2 * correl * (sigma / a) * (mat - (1 / a) * (1 - exp(-a * mat)));
			if(const1 * const1 - 4 * mat * (v1 - mat * vol[i] * vol[i])<0){
				char message[MAX_LENGTH_MESSAGE_EXCEPTION];
				sprintf_s(message, "Impossible vol conversion : negative implied vol value");
				throw new GMXBException(message);
			}
			vol[i] = (- const1 + sqrt(const1 * const1 - 4 * mat * (v1 - mat * vol[i] * vol[i]))) / (2 * mat);
		}
	}
	if(debug_info==true){
		filevol<<"Vol_Imp_BS_HW"<<",";
		for(int i=0;i<nb_vol;i++){
			filevol<<setprecision(4)<<vol[i]<<",";
		}
		filevol<<endl;
	}

	if(vol_loc==true){
		double * vol_mem = new double[nb_vol];
		for(int i=0;i<nb_vol;i++){
			if(i==0){vol_mem[i] = vol[i];}
			else{
				if(time[i] - time[i-1]<=0){
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Impossible vol conversion : negative time step");
					throw new GMXBException(message);
				}
				if(vol[i] * vol[i] + (time[i-1] / (double) (time[i] - time[i-1])) * (vol[i] * vol[i] - vol[i-1] * vol[i-1])<0){
					char message[MAX_LENGTH_MESSAGE_EXCEPTION];
					sprintf_s(message, "Impossible vol conversion : negative local vol value");
					throw new GMXBException(message);
				}
				else{
					vol_mem[i] = sqrt(vol[i] * vol[i] + (time[i-1] / (double) (time[i] - time[i-1])) * (vol[i] * vol[i] - vol[i-1] * vol[i-1]) );
				}
			}
		}
		for(int i=0;i<nb_vol;i++){
			vol[i] = vol_mem[i];
		}
		delete [] vol_mem;
	}
	if(debug_info==true){
		filevol<<"Vol_Loc"<<",";
		for(int i=0;i<nb_vol;i++){
			filevol<<setprecision(4)<<vol[i]<<",";
		}
		filevol<<endl;
	}

}

