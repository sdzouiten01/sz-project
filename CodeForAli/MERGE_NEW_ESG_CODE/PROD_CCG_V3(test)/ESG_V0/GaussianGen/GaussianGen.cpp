#include "GaussianGen.h"


double GaussianGen::Polar(){
	double a,b,r;
		do{
			a=2*Uniform_Generator->Genrand()-1;
			b=2*Uniform_Generator->Genrand()-1;
			r=a*a+b*b;
		}while(r>=1);
	return a*sqrt(-2*log(r)/r);	
}
void GaussianGen::GenAll(VarianceReduction* &t)
{
	if(Martingale==0) GenGaussian_mod0();
	else if(Martingale==1) GenGaussian_mod1(t);
	else if(Martingale==2) GenGaussian_mod2();
	else if(Martingale==3) GenGaussian_mod3(t);
}
void GaussianGen::GenGaussian_mod0()
{
	Uniform_Generator->Outputseed(ListSeed[0]);
	GenGaussian_IR();
	combination(1);
	for(int index_position=2;index_position<2+NB_equity;index_position++)
	{
		Uniform_Generator->Outputseed(ListSeed[index_position-1]);
		GenGaussian_Equity(index_position);
		combination(index_position);
	}
}
void GaussianGen::GenGaussian_mod2()
{
	Uniform_Generator->Inputseed(ListSeed[0]);
	GenGaussian_IR();
	combination(1);
	for(int index_position=2;index_position<2+NB_equity;index_position++)
	{
		Uniform_Generator->Inputseed(ListSeed[index_position-1]);
		GenGaussian_Equity(index_position);
		combination(index_position);
	}
}
void GaussianGen::GenGaussian_mod1(VarianceReduction* &t)
{
	bool control=0;
	int count=0;
	double ** temp;
	temp=new double * [Nb_Step];
	for (int k=0;k<Nb_Step;k++)
		temp[k]=new double  [Number_Scen_Antithetic];
	for(int k=0;k<Nb_Step;k++)
		for(int i=0;i<Number_Scen_Antithetic;i++)
			temp[k][i]=0.;	

	while(control==0 || count==MAXGENTRY)
	{
		Uniform_Generator->Outputseed(ListSeed[0]);
		GenGaussian_IR();
		combination(1);
		control=t->Test_Rate(Gaussian_vector[0],Gaussian_vector[1]);
		count++;
	}
	for(int index_position=2;index_position<2+NB_equity;index_position++)
	{
		control=0;
		count=0;
		for(int k=0;k<Nb_Step;k++)
			for(int i=0;i<Number_Scen_Antithetic;i++)
				temp[k][i]=0.;	

		while(control==0 || count==MAXGENTRY)
		{
			Uniform_Generator->Outputseed(ListSeed[index_position-1]);
			GenGaussian_Equity(index_position);
			if(count==0) Init_intermediate_tab(index_position,temp);
			combination(index_position,temp);
			control=t->Test_Equity(Gaussian_vector[index_position],index_position-2);
			count++;
		}
	}

	for(int k=0;k<Nb_Step;k++)	delete[] temp[k];
	delete[] temp;
}
void GaussianGen::GenGaussian_mod3(VarianceReduction* &t)
{
	bool control=0;
	int count=0;
	int count2=0;
	double ** temp;
	temp=new double * [Nb_Step];
	for (int k=0;k<Nb_Step;k++)
		temp[k]=new double  [Number_Scen_Antithetic];
	for(int k=0;k<Nb_Step;k++)
		for(int i=0;i<Number_Scen_Antithetic;i++)
			temp[k][i]=0.;	

	unsigned long * tmp = new unsigned long[N+1];
	while(control==0 || count==MAXGENTRY)
	{
		if(count2==t->Martingale_level_try_tab[0][1]+1){
			t->Adjust_Error_table();
			Uniform_Generator->Initialisation_seed0();
			count2=0;
		}
		Uniform_Generator->Outputseed(ListSeed[0]);
		GenGaussian_IR();
		combination(1);
		control=t->Test_Rate(Gaussian_vector[1],Gaussian_vector[2]);
		count++;
		count2++;
		
	}
	for(int index_position=2;index_position<2+NB_equity;index_position++)
	{
		Uniform_Generator->Outputseed(tmp);
		control=0;
		count=0;
		count2=0;
		for(int k=0;k<Nb_Step;k++)
			for(int i=0;i<Number_Scen_Antithetic;i++)
				temp[k][i]=0.;	

		while(control==0 || count==MAXGENTRY)
		{
			if(count2==t->Martingale_level_try_tab[index_position-1][1]+1){
				t->Adjust_Error_table();
				Uniform_Generator->Inputseed(tmp);
				count2=0;
			}
			Uniform_Generator->Outputseed(ListSeed[index_position-1]);
			GenGaussian_Equity(index_position);
			if(count==0) Init_intermediate_tab(index_position,temp);
			combination(index_position,temp);
			control=t->Test_Equity(Gaussian_vector[index_position],index_position-2);
			count++;
			count2++;
		}
	}
	delete[] tmp;
	for(int k=0;k<Nb_Step;k++)	delete[] temp[k];
	delete[] temp;

}	
void GaussianGen::GenGaussian_Equity(int index_position)
{
	for(int i=0;i<Number_Scen_Antithetic;i++)
		for(int j=0;j<Nb_Step;j++)
			Gaussian_vector[index_position][j][i]=Polar();
}
void GaussianGen::GenGaussian_IR()
{
	for(int i=0;i<Number_Scen_Antithetic;i++)
		for(int j=0;j<Nb_Step;j++)
				for(int k=0;k<2;k++)
					Gaussian_vector[k][j][i]=Polar();
}
void  GaussianGen::combination(int index_position)
{
	double const0=1./Inverse_Cholesky_Correlation[index_position][index_position];
	
	for(int j=0;j<Nb_Step;j++)
		for(int i=0;i<Number_Scen_Antithetic;i++){
			for(int k=0;k<index_position;k++){
				//cout << "index_position=" << index_position<< endl;
				//cout << "k=" << k<< endl;
				//cout << "Inverse_Cholesky_Correlation[index_position][k]=" << Inverse_Cholesky_Correlation[index_position][k]<< endl;
				Gaussian_vector[index_position][j][i] -=Inverse_Cholesky_Correlation[index_position][k]*Gaussian_vector[k][j][i];
			}
			Gaussian_vector[index_position][j][i] *=const0;
			//cout << "Inverse_Cholesky_Correlation[0][0]=" << Inverse_Cholesky_Correlation[0][0]<< endl;
			//cout << "Inverse_Cholesky_Correlation[0][1]=" << Inverse_Cholesky_Correlation[0][1]<< endl;
			//cout << "Inverse_Cholesky_Correlation[0][2]=" << Inverse_Cholesky_Correlation[0][2]<< endl;
			//cout << "Inverse_Cholesky_Correlation[1][0]=" << Inverse_Cholesky_Correlation[1][0]<< endl;
			//cout << "Inverse_Cholesky_Correlation[1][1]=" << Inverse_Cholesky_Correlation[1][1]<< endl;
			//cout << "Inverse_Cholesky_Correlation[1][2]=" << Inverse_Cholesky_Correlation[1][2]<< endl;
			//cout << "Inverse_Cholesky_Correlation[2][0]=" << Inverse_Cholesky_Correlation[2][0]<< endl;
			//cout << "Inverse_Cholesky_Correlation[2][1]=" << Inverse_Cholesky_Correlation[2][1]<< endl;
			//cout << "Inverse_Cholesky_Correlation[2][2]=" << Inverse_Cholesky_Correlation[2][2]<< endl;
			//cout << "Gaussian_vector[1][" << j << "][" << i << "]=" << Gaussian_vector[index_position][j][i] << endl;

		}
}
void GaussianGen::Init_intermediate_tab(int index_position,double ** tmp)
{
	for(int j=0;j<Nb_Step;j++)
		for(int i=0;i<Number_Scen_Antithetic;i++){
			for(int k=0;k<index_position;k++)
				tmp[j][i] -=Inverse_Cholesky_Correlation[index_position][k]*Gaussian_vector[k][j][i];
		}


}
void GaussianGen::combination(int index_position, double ** tmp)
{
	double const0=1./Inverse_Cholesky_Correlation[index_position][index_position];
	for(int j=0;j<Nb_Step;j++)
		for(int i=0;i<Number_Scen_Antithetic;i++){
			Gaussian_vector[index_position][j][i] +=tmp[j][i];
			Gaussian_vector[index_position][j][i] *=const0;
		}
}	
GaussianGen::GaussianGen(GaussianGenModelParameters * &Input_Gen_Model_Parameters)
{	
	
	Uniform_Method=Input_Gen_Model_Parameters->Uniform_Method;
	if(Uniform_Method == "Mersenne_Twister" ) Uniform_Generator=new MT();
	else Uniform_Generator=new UniformGen();	

	Gaussian_Method=Input_Gen_Model_Parameters->Gaussian_Method;
	Martingale=Input_Gen_Model_Parameters->Martingale;
	Antithetic=Input_Gen_Model_Parameters->Antithetic;
	Nb_Step=Input_Gen_Model_Parameters->Nb_Step;
	Number_Scen=Input_Gen_Model_Parameters->Number_Scen;

	Number_Scen_Antithetic=Input_Gen_Model_Parameters->Number_Scen_Antithetic;
	NB_equity=Input_Gen_Model_Parameters->NB_equity;

	Gaussian_vector=new double** [2+NB_equity];
	for(int i=0; i<2+NB_equity; i++){
		Gaussian_vector[i]=new double* [Nb_Step];
		for(int j=0; j<Nb_Step; j++){
			Gaussian_vector[i][j]=new double [Number_Scen];
			for(int k=0; k<Number_Scen; k++)
				Gaussian_vector[i][j][k]=0.;
		}
	}

	ListSeed=new unsigned long* [1+NB_equity];
	for(int i=0; i<NB_equity+1; i++){
		ListSeed[i]=new unsigned long [N+2];
		for(int j=0; j<N+2; j++)
			ListSeed[i][j]=Input_Gen_Model_Parameters->ListSeed[i][j];
	}

	Inverse_Cholesky_Correlation=new double* [2+NB_equity];
	for(int i=0; i<2+NB_equity; i++){
		Inverse_Cholesky_Correlation[i]=new double [2+NB_equity];
		for(int j=0; j<2+NB_equity; j++)
			Inverse_Cholesky_Correlation[i][j]=Input_Gen_Model_Parameters->Inverse_Cholesky_Correlation[i][j];
	}

}
GaussianGen::~GaussianGen()
{
	for(int i=0;i<2+NB_equity;i++)
	{
		for(int j=0;j<Nb_Step;j++) delete[] Gaussian_vector[i][j];
		delete[] Gaussian_vector[i];
	}
	delete[] Gaussian_vector;
	
	for(int i=0;i<1+NB_equity;i++) delete[] ListSeed[i];
	delete[] ListSeed;
	
	for(int i=0;i<2+NB_equity;i++) delete[] Inverse_Cholesky_Correlation[i];
	delete[] Inverse_Cholesky_Correlation;

	delete Uniform_Generator;
}
void GaussianGen::Write_Debug_File(string DbgFilePath, int CurrentScen)
{
	ofstream output;
	string ss;
	ss = DbgFilePath + "Gaussian" + to_string(CurrentScen) + "_0"+ ".csv";
	output.open(ss.c_str());
	
	output<<"iteration"<<","<<"period"<<","<<"IR_Gaussian_1"<<","<<"IR_Gaussian_2"<<",";
	for(int i=0;i<NB_equity;i++) output<<"Gaussian_Equity_"<< i <<",";
	output<<endl;
	for(int i=0;i<Number_Scen_Antithetic;i++){
		for(int j=0;j<Nb_Step ;j++){
			output<<i<<","<<j<<",";
			for(int k=0;k<NB_equity+2;k++){
				output<<setprecision(15)<<Gaussian_vector[k][j][i]<<",";
			}
			output<<endl;
		}
	}
	output.close();

}	