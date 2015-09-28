#include "GaussianGenModelParameters.h"
GaussianGenModelParameters::~GaussianGenModelParameters()
{
	for(int i=0;i<1+NB_equity;i++) delete[] ListSeed[i];
	delete[] ListSeed;
	
	for(int i=0;i<2+NB_equity;i++) delete[] Inverse_Cholesky_Correlation[i];
	delete[] Inverse_Cholesky_Correlation;
}
GaussianGenModelParameters::GaussianGenModelParameters(InterfaceESG0 * &interface_Esg,HW1FSwaptionPricer* &model )
{	

	Uniform_Method="Mersenne_Twister";
	Gaussian_Method="Polar";
	Martingale=interface_Esg->Test_martingale;
	Antithetic=interface_Esg->Antithetic;
	Nb_Step=interface_Esg->Number_Year*interface_Esg->Time_Step;
	Number_Scen=interface_Esg->Number_Scenario;
	Number_Scen_Antithetic=Number_Scen/Antithetic;
	NB_equity=interface_Esg->Nb_equity;
	Nb_rate=interface_Esg->Nb_rate;
	Nb_bond=interface_Esg->Nb_bond;
	myDebug_address_ESG=interface_Esg->Debug_ESG_address;
	myDebug_info_ScenGen=interface_Esg->Debug_Scenario_Generator_info;

	ListSeed=new unsigned long* [1+NB_equity];
	for(int i=0; i<NB_equity+1; i++){
		ListSeed[i]=new unsigned long [N+2];
		for(int j=0; j<N+2; j++)
			ListSeed[i][j]=0;
	}
	if(Martingale==2){
		ifstream file2((myDebug_address_ESG + "seed.csv").c_str(), ios::in);
		for(int i=0; i<NB_equity+1; i++){
			ListSeed[i][N+1]=read_seed(file2,ListSeed[i]);
		}
	}
	int corel_matrix_size_local=2+NB_equity;
	Inverse_Cholesky_Correlation=new double* [corel_matrix_size_local];
	for(int i=0; i<2+NB_equity; i++){
		Inverse_Cholesky_Correlation[i]=new double [corel_matrix_size_local];
		for(int j=0; j<corel_matrix_size_local; j++)
			Inverse_Cholesky_Correlation[i][j]=0.;//inverse_Cholesky_Correlation[i][j];
	}	


	
	double **Correlation_local=new double *[corel_matrix_size_local];
	for(int i=0;i<corel_matrix_size_local;i++) Correlation_local[i]=new double [corel_matrix_size_local];
		   
	Correlation_local[0][0]=interface_Esg->Correlation_matrix[0][0];
	Correlation_local[1][1]=1.0;
	for(int i=2;i<corel_matrix_size_local;i++){
		Correlation_local[0][i]=interface_Esg->Correlation_matrix[0][i-1];
		Correlation_local[i][0]=interface_Esg->Correlation_matrix[i-1][0];
	}
	for(int i=2;i<corel_matrix_size_local;i++){
		for(int j=2;j<corel_matrix_size_local;j++){
			Correlation_local[i][j]=interface_Esg->Correlation_matrix[i-1][j-1];
		}
	}
	double correl=(exp(model->getMRV()/interface_Esg->Time_Step)-1.0)/model->getMRV()*sqrt(2.0*model->getMRV()/(1.0/interface_Esg->Time_Step*(exp(2.0*model->getMRV()/interface_Esg->Time_Step)-1)));
	Correlation_local[0][1]=correl;
	Correlation_local[1][0]=correl;
	for(int i=2;i<corel_matrix_size_local;i++){
		Correlation_local[1][i]=Correlation_local[0][i]*correl;
		Correlation_local[i][1]=Correlation_local[0][i]*correl;
	}	   
	//cout << "Correlation_local[0][0]" <<setprecision(15) << Correlation_local[0][0] << endl;
	//cout << "Correlation_local[0][1]" <<setprecision(15)<<Correlation_local[0][1] << endl;
	//cout << "Correlation_local[0][2]" <<setprecision(15)<<Correlation_local[0][2] << endl;
	//cout << "Correlation_local[1][0]" <<setprecision(15)<<Correlation_local[1][0] << endl;
	//cout << "Correlation_local[1][1]" <<setprecision(15)<<Correlation_local[1][1] << endl;
	//cout << "Correlation_local[1][2]" <<setprecision(15)<<Correlation_local[1][2] << endl;
	//cout << "Correlation_local[2][0]" <<setprecision(15)<<Correlation_local[2][0] << endl;
	//cout << "Correlation_local[2][1]" <<setprecision(15)<<Correlation_local[2][1] << endl;
	//cout << "Correlation_local[2][2]" <<setprecision(15)<<Correlation_local[2][2] << endl;

	Cholesky(Correlation_local,corel_matrix_size_local);
	
	for(int i=0;i<corel_matrix_size_local;i++)
		for(int j=i+1;j<corel_matrix_size_local;j++)
			Correlation_local[i][j]=0.;
	
	//cout << "Correlation_local[0][0]" <<setprecision(15) << Correlation_local[0][0] << endl;
	//cout << "Correlation_local[0][1]" <<setprecision(15)<<Correlation_local[0][1] << endl;
	//cout << "Correlation_local[0][2]" <<setprecision(15)<<Correlation_local[0][2] << endl;
	//cout << "Correlation_local[1][0]" <<setprecision(15)<<Correlation_local[1][0] << endl;
	//cout << "Correlation_local[1][1]" <<setprecision(15)<<Correlation_local[1][1] << endl;
	//cout << "Correlation_local[1][2]" <<setprecision(15)<<Correlation_local[1][2] << endl;
	//cout << "Correlation_local[2][0]" <<setprecision(15)<<Correlation_local[2][0] << endl;
	//cout << "Correlation_local[2][1]" <<setprecision(15)<<Correlation_local[2][1] << endl;
	//cout << "Correlation_local[2][2]" <<setprecision(15)<<Correlation_local[2][2] << endl;


	Matrice Mat(Correlation_local,corel_matrix_size_local,corel_matrix_size_local);
	//Matrice* Mat=new Matrice(Correlation_local,corel_matrix_size_local,corel_matrix_size_local);
	Mat.Inverse2();
	//Mat->Inverse2();
	for(int i=0;i<corel_matrix_size_local;i++){
		for(int j=0;j<corel_matrix_size_local;j++){
			Correlation_local[i][j]=Mat._matrice[i][j];
		}
	}
	//cout << "Correlation_local[0][0]" <<setprecision(15) << Correlation_local[0][0] << endl;
	//cout << "Correlation_local[0][1]" <<setprecision(15)<<Correlation_local[0][1] << endl;
	//cout << "Correlation_local[0][2]" <<setprecision(15)<<Correlation_local[0][2] << endl;
	//cout << "Correlation_local[1][0]" <<setprecision(15)<<Correlation_local[1][0] << endl;
	//cout << "Correlation_local[1][1]" <<setprecision(15)<<Correlation_local[1][1] << endl;
	//cout << "Correlation_local[1][2]" <<setprecision(15)<<Correlation_local[1][2] << endl;
	//cout << "Correlation_local[2][0]" <<setprecision(15)<<Correlation_local[2][0] << endl;
	//cout << "Correlation_local[2][1]" <<setprecision(15)<<Correlation_local[2][1] << endl;
	//cout << "Correlation_local[2][2]" <<setprecision(15)<<Correlation_local[2][2] << endl;



	for(int i=0; i<2+NB_equity; i++){
		for(int j=0; j<2+NB_equity; j++)
			Inverse_Cholesky_Correlation[i][j]=Correlation_local[i][j];
	}

	for(int i=0; i<2+NB_equity; i++)
		delete [] Correlation_local[i];
	delete [] Correlation_local;
	//delete Mat;

}
int GaussianGenModelParameters::read_seed(ifstream &file2,unsigned long * seed){
	string row;
	getline(file2,row);
	char* retPtr = new char[row.length()+1];
	strcpy_s(retPtr, row.length()+1, row.c_str());
	char* context = NULL;
	int martingale = atoi(strtok_s(retPtr, ",",&context));
	if(context[0]==0){
		char message[MAX_LENGTH_MESSAGE_EXCEPTION];
		sprintf_s(message, "Error in Seed File");
		throw new GMXBException(message);
	}
	for (int i=0; i<N+1; i++){
		if(context[0]==0){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Error in Seed File");
			throw new GMXBException(message);
		}
		seed[i] = strtoul(strtok_s(NULL, ",",&context),NULL,10);
	}
	delete [] retPtr;
	return martingale;
}
// Cholesky decomposition :	takes a matrix A as input which must be definite positive
//							returns the unique matrix T triangular inferior with positive coefficients on the diagonal
//							such that A = TT*

// Can be dangerous to use with matrixes that are close to not being definite positive (ex : correl near 1)
void GaussianGenModelParameters::Cholesky(double** input, int dimension){
	double Epsilon = 1e-10;
	for(int i=0;i<dimension;i++){
		for(int k=0;k<=i-1;k++){
			input[i][i]-=input[i][k]*input[i][k];
		}
		if(input[i][i]<Epsilon){
			char message[MAX_LENGTH_MESSAGE_EXCEPTION];
			sprintf_s(message, "Correlation Matrix is not definite positive");
			throw new GMXBException(message);
		}
		input[i][i]=sqrt(input[i][i]);
		for(int j=i+1;j<dimension;j++){
			for(int k=0;k<=i-1;k++){
				input[j][i]-=input[i][k]*input[j][k];
			}
			input[j][i]=input[j][i]/input[i][i];
		}			
	}
}

