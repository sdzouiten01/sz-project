#include "Generator.h"
#include "Mersenne_Twister.h"
#include <time.h>
#include <math.h>
#include <iostream>

using namespace std;

void Generator::Gaussian(Basic_Information &BI,double*** output,unsigned long * seed, int start_index, int number_indexes, int martingale){
	if((martingale==1)||(martingale==3)){Outputseed(seed);}
	else if((martingale==2)){Inputseed(seed);}
	for(int i=0;i<BI.nb_scens()/BI.antithetic();i++){
		for(int j=0;j<BI.nb_step();j++){
			for(int k=start_index;k<start_index+number_indexes;k++){
				output[k][j][i]=Generator::Polar();
			}
		}
	}
}

void Generator::Gaussian_reset(Basic_Information &BI,double*** output, int number_indexes,int new_index){
	Mosesrand();
	if(new_index!=0){new_index++;}
	for(int i=0;i<BI.nb_scens()/BI.antithetic();i++){
		for(int j=0;j<BI.nb_step();j++){
			for(int k=0;k<number_indexes;k++){
				double a = Generator::Polar();
				if(new_index==0 && k<2){output[k][j][i]=a;}
				else if(k==new_index){output[k][j][i]=a;}
			}
		}
	}
}

/*double Generator::Polar(unsigned long Seed){
	static double memory=0;
	static bool pre_calculated=false;
	if(pre_calculated==false){
		double a, b, r, loc;
		do{
			a=2*genrand(Seed)-1;
			b=2*genrand(Seed)-1;
			r=a*a+b*b;
		}while(r>=1);
		pre_calculated=true;
		loc = sqrt(-2*log(r)/r);
		memory = b*loc;
		return a*loc;
	}
	else{
		pre_calculated=false;
		return memory;
	}
}*/

double Generator::Polar(){
	double a,b,r;
		do{
			a=2*genrand()-1;
			b=2*genrand()-1;
			r=a*a+b*b;
		}while(r>=1);
	return a*sqrt(-2*log(r)/r);	
}

// Generation of a rate or index, for all the time steps of the simulation, 
// with the correct correlation with previous indexes and which satisfies the martingality tests
int Generator::Indice_Generator(int position_index, Scenario *s,Variance_Reduction t,Basic_Information BI, unsigned long * seed){
	if(position_index == 0)
	{
		cout << "          GENERATION OF INTEREST RATES : " << endl << endl;
	}
	else
	{
		cout << "          GENERATION OF INDEX " << position_index << " : " << endl << endl;
	}
	double *tmp=new double [BI.nb_scens()];
	double error=BI.error(position_index);
	bool control=0; // control=1 if the martingality tests have succedded
	int max1=0; int max2=0; int max3=0;
	int max_tot1=0; int max_tot2=0; int max_tot3=0;
	int count = 1;
	while(control==0){
		for(int j=0;j<BI.nb_scens();j++){
			tmp[j]=1;
		}
		for(int i=0;i<BI.nb_step();i++){
			if(position_index==0) { // Rate diffusion : two highly correlated Gaussian variables
				combination(position_index,i,BI,s);
				control=t.test_rate(s,BI,position_index,i,tmp,error);	
			}
			if(position_index>=1){ // Equity diffusion
				double vol=interpole(BI.nb_vol(),i,BI.vol_t(),BI.vol_eq(position_index-1));
				// vol represents the volatility for time step i
				// 1/BI.delta_t() represents the time period in years (BI.delta_t() = 12 if monthly)
				combination(position_index,i,BI,s);
				control=t.test_equity(s,BI,vol,position_index,i,tmp,error);
			}
			if (control==0){
				if(i+1>max1){max3=max2;max2=max1;max1=i+1;}
				else if(i+1>max2){max3=max2;max2=i+1;}
				else if(i+1>max3){max3=i+1;}
				if(i+1>max_tot1){max_tot3=max_tot2;max_tot2=max_tot1;max_tot1=i+1;}
				else if(i+1>max_tot2){max_tot3=max_tot2;max_tot2=i+1;}
				else if(i+1>max_tot3){max_tot3=i+1;}
				cout << i+1 << " ";
				if(count%100==0){
					if(position_index == 0){cout << endl << endl << "interest rates : " << "failure number " << count << endl;}
					else{cout << endl << endl <<"index " << position_index << " : failure number " << count << endl;}
					cout << "max : " << max1 << "  " << max2 << "  " << max3 << endl;
					cout << "max tot : " << max_tot1 << "  " << max_tot2 << "  " << max_tot3 << endl << endl;
					max1=0;max2=0;max3=0;
				}
				//cout << "failure number " << count << " at time step " << i+1 << endl;
				count++;
				if(t.mart()==3 && count>BI.max_try(position_index)){
					delete[] tmp;
					return 0;
				}
				i=BI.nb_step();
				if (position_index==0){Gaussian(BI,s->random,seed,0,2,t.mart());}
				if (position_index>=1){Gaussian(BI,s->random,seed,position_index+1,1,t.mart());}
			}
		}
	}
	if(count>1){cout << endl << endl;}
	cout << "succeed at try number " << count << endl << endl;
	delete[] tmp;
	return count;
}

// Creates correlated Gaussian variables (but which should still be of variance 1)
// Independent gaussian variables in random for the index position_index are transformed into variables
// correctly correlated with the previous gaussian variables generated for smaller indexes (and stocked in scenario)
// This corresponds to a multiplication of the matrix BI.cor obtained by Cholesky with the vector of independent gaussian variables.
void Generator::combination(int position_index,int position_step,Basic_Information &BI,Scenario *s){
	int n=BI.nb_scens()/BI.antithetic();
	// In this case the second rate variable is generated correlated with the first one and stocked
	// in scenario with position_index = 0 (this remained empty if flag == 0).
	// This means that the matrix BI.cor has one additional column so there is a shift in position_index with this matrix.
	int loc_var_1 = 3+position_index+BI.nb_rate();
	int loc_var_2 = position_step+1;
	
	for (int j=0; j<n; j++){
		s->scenario[loc_var_1][loc_var_2][j] = 0;
	}
	for (int k=0 ; k <= position_index+1 ; k++ ){
		double loc_var_3 = BI.cor(position_index+1,k);
		for (int j=0; j<n; j++) {
			s->scenario[loc_var_1][loc_var_2][j] += loc_var_3*s->random[k][position_step][j];
		}
	}
}

/*
void Generator::calcul_scenario(Scenario *s,Basic_Information BI,bool fast){
	cout<<"Calculating scenarios : ";

	if(fast==0){
		for(int j=0;j<BI.nb_scens();j++){
			s->scenario[2][0][j]+=BI.forward_rate(0)*100;
			for(int k=0;k<BI.nb_eq();k++){
				s->scenario[4+BI.nb_rate()+k][0][j]=0;
			}
			for(int k=0;k<BI.nb_bond();k++){
				s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][0][j]=0;
			}
		}

		double ** forward_rates = new double* [BI.nb_step()+1];
		double ** zero_coupon = new double* [BI.nb_step()+1];
		double longest_maturity = 0;
		for(int k=0;k<BI.nb_rate();k++){
			longest_maturity = max(longest_maturity,BI.m_rate(k));
		}
		for(int k=0;k<BI.nb_bond();k++){
			longest_maturity = max(longest_maturity,BI.m_bond(k));
		}
		longest_maturity *= BI.delta_t()/6;
		longest_maturity++;
		for(int i=0;i<BI.nb_step()+1;i++){
			forward_rates[i]= new double [(int) longest_maturity];
			zero_coupon[i] = new double [(int) longest_maturity];
			for(int j=0;j<longest_maturity;j++){
				forward_rates[i][j]=0;
				zero_coupon[i][j]=0;
			}
		}

		double ** forward_rates_bis = new double* [BI.nb_step()+1];
		double ** zero_coupon_bis = new double* [BI.nb_step()+1];
		for(int i=0;i<BI.nb_step()+1;i++){
			forward_rates_bis[i]= new double [(int) longest_maturity];
			zero_coupon_bis[i] = new double [(int) longest_maturity];
			for(int j=0;j<longest_maturity;j++){
				forward_rates_bis[i][j]=0;
				zero_coupon_bis[i][j]=0;
			}
		}

		double f=-BI.sigma()*BI.sigma()/BI.a()/BI.a()/BI.a();
		for(int j=0;j<BI.nb_scens();j++){
			for(int c=0;c<longest_maturity;c++){
				forward_rates[0][c] = BI.forward_rate(6*c)+BI.forward_rate(6*c+1)+BI.forward_rate(6*c+2)+BI.forward_rate(6*c+3)+BI.forward_rate(6*c+4)+BI.forward_rate(6*c+5);
			}
			for(int r=1;r<BI.nb_step()+1;r++){
				int c=0;
				for(c=0;c<longest_maturity;c++){
					double ed= exp(BI.a()*(r)/BI.delta_t());
					double e1=exp(-BI.a()*(r+6*c)/BI.delta_t());
					double e2=exp(-BI.a()*(r+6*c+6)/BI.delta_t());
					forward_rates[r][c] = BI.forward_rate(r+6*c)+BI.forward_rate(r+6*c+1)+BI.forward_rate(r+6*c+2)+BI.forward_rate(r+6*c+3)+BI.forward_rate(r+6*c+4)+BI.forward_rate(r+6*c+5);
					forward_rates[r][c] += f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*BI.delta_t();
					forward_rates[r][c] += s->scenario[2][r][j] * (BI.sigma()/BI.a()) * (e1-e2) * BI.delta_t() ;	
				}
			}
			for(int r=0;r<BI.nb_step()+1;r++){
				for(int c=1;c<longest_maturity;c++){
					zero_coupon[r][c] = zero_coupon[r][c-1]+forward_rates[r][c-1]/BI.delta_t();
				}
			}
		
			forward_rates_bis[0][0] = BI.forward_rate(0)+BI.forward_rate(1)+BI.forward_rate(2)+BI.forward_rate(3)+BI.forward_rate(4);
			for(int c=1;c<longest_maturity;c++){
				forward_rates_bis[0][c] = BI.forward_rate(6*c-1)+BI.forward_rate(6*c)+BI.forward_rate(6*c+1)+BI.forward_rate(6*c+2)+BI.forward_rate(6*c+3)+BI.forward_rate(6*c+4);
			}
			for(int r=1;r<BI.nb_step()+1;r++){
				double fd=exp(BI.a()*(r)/BI.delta_t());
				double f1=exp(-BI.a()*(r)/BI.delta_t());
				double f2=exp(-BI.a()*(r+5)/BI.delta_t());
				forward_rates_bis[r][0] = BI.forward_rate(r)+BI.forward_rate(r+1)+BI.forward_rate(r+2)+BI.forward_rate(r+3)+BI.forward_rate(r+4);
				forward_rates_bis[r][0] += f * ((f2-f1)*(fd-1.0)-0.25*(f2*f2-f1*f1)*(fd*fd-1.0))*BI.delta_t();
				forward_rates_bis[r][0] += s->scenario[2][r][j] * (BI.sigma()/BI.a()) * (f1-f2) * BI.delta_t();	
				for(int c=1;c<longest_maturity;c++){
					double ed=exp(BI.a()*(r)/BI.delta_t());
					double e1=exp(-BI.a()*(r+6*c-1)/BI.delta_t());
					double e2=exp(-BI.a()*(r+6*c+6-1)/BI.delta_t());
					forward_rates_bis[r][c] = BI.forward_rate(r+6*c-1)+BI.forward_rate(r+6*c)+BI.forward_rate(r+6*c+1)+BI.forward_rate(r+6*c+2)+BI.forward_rate(r+6*c+3)+BI.forward_rate(r+6*c+4);
					forward_rates_bis[r][c] += f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*BI.delta_t();
					forward_rates_bis[r][c] += s->scenario[2][r][j] * (BI.sigma()/BI.a()) * (e1-e2) * BI.delta_t();	
				}
			}
			for(int r=0;r<BI.nb_step()+1;r++){
				for(int c=1;c<longest_maturity;c++){
					zero_coupon_bis[r][c] = zero_coupon_bis[r][c-1]+forward_rates_bis[r][c-1]/BI.delta_t();
				}
			}
		
		
			for(int k=0;k<BI.nb_rate();k++){
				if(BI.rate_type(k)==1) s->scenario[3+k][0][j]=Rate_Model::interest_rate(BI.m_rate(k),-1,BI.delta_t(),zero_coupon);
				if(BI.rate_type(k)==2) s->scenario[3+k][0][j]=Rate_Model::swap_rate(BI.m_rate(k),-1,BI.delta_t(),BI.coupon_period(),zero_coupon);
			}
			for(int i=0;i<BI.nb_step();i++){
				double e1=exp(-BI.a()*(i+1)/BI.delta_t());
				double e2=exp(-BI.a()*(i+2)/BI.delta_t());
				double ed=exp(BI.a()*(i+1)/BI.delta_t());
				double rate=BI.forward_rate(i+1)+ f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*BI.delta_t();
				s->scenario[2][i+1][j]*=BI.sigma()*exp(-BI.a()*(i+1)/BI.delta_t())*(1.0-exp(-BI.a()/BI.delta_t()))/BI.a()*BI.delta_t()*100;
				s->scenario[2][i+1][j]+=rate*100;
				for(int k=0;k<BI.nb_rate();k++){
					if(BI.rate_type(k)==1) s->scenario[3+k][i+1][j]=Rate_Model::interest_rate(BI.m_rate(k),i,BI.delta_t(),zero_coupon);
					if(BI.rate_type(k)==2) s->scenario[3+k][i+1][j]=Rate_Model::swap_rate(BI.m_rate(k),i,BI.delta_t(),BI.coupon_period(),zero_coupon);
				}
				s->scenario[3+BI.nb_rate()][i+1][j]*=exp(BI.forward_rate1(i+1))/exp(BI.forward_rate1(i))*s->scenario[3+BI.nb_rate()][i][j];
				for(int k=0;k<BI.nb_eq();k++){
					s->scenario[4+BI.nb_rate()+k][i+1][j]*=s->scenario[3+BI.nb_rate()][i+1][j]/s->scenario[3+BI.nb_rate()][i][j];
					s->scenario[4+BI.nb_rate()+k][i+1][j]=(pow(s->scenario[4+BI.nb_rate()+k][i+1][j],12.0)-1.0)*100;
				}
				for(int k=0;k<BI.nb_bond();k++){
					if(BI.bond_coupon(k)<0.000001){
						s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][i+1][j]=Rate_Model::bond_return(BI.m_bond(k),i,BI.delta_t(),zero_coupon_bis,zero_coupon);
					}
					else{
						s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][i+1][j]=Rate_Model::bond_return_coupon(BI.m_bond(k),i,BI.delta_t(),zero_coupon_bis,zero_coupon,BI.bond_coupon(k),BI.bond_freq(k));
					}
				}
			}
		}

		for(int i=0;i<BI.nb_step()+1;i++){
			delete[] forward_rates[i];
			delete[] zero_coupon[i];
		}
		delete[] forward_rates;
		delete[] zero_coupon;

		for(int i=0;i<BI.nb_step()+1;i++){
			delete[] forward_rates_bis[i];
			delete[] zero_coupon_bis[i];
		}
		delete[] forward_rates_bis;
		delete[] zero_coupon_bis;
	}
	

	else{
		for(int j=0;j<BI.nb_scens();j++){
			s->scenario[2][0][j]+=BI.forward_rate(0)*100;
			for(int k=0;k<BI.nb_rate();k++){
				if(BI.rate_type(k)==1) s->scenario[3+k][0][j]=Rate_Model::interest_rate(BI.a(),BI.sigma(),BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][0][j]/100,-1,BI.delta_t());
				if(BI.rate_type(k)==2) s->scenario[3+k][0][j]=Rate_Model::swap_rate(BI.a(),BI.sigma(),BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][0][j]/100,-1,BI.delta_t(),BI.coupon_period());
			}
			for(int k=0;k<BI.nb_eq();k++){
				s->scenario[4+BI.nb_rate()+k][0][j]=0;
			}
			for(int k=0;k<BI.nb_bond();k++){
				s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][0][j]=0;
			}
		}
	
		for(int i=0;i<BI.nb_step();i++){
			double e1=exp(-BI.a()*(i+1)/BI.delta_t());
			double e2=exp(-BI.a()*(i+2)/BI.delta_t());
			double ed=exp(BI.a()*(i+1)/BI.delta_t());
			double f=-BI.sigma()*BI.sigma()/BI.a()/BI.a()/BI.a();
			double rate=BI.forward_rate(i+1)+ f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*BI.delta_t();
			for(int j=0;j<BI.nb_scens();j++){
				s->scenario[2][i+1][j]*=BI.sigma()*exp(-BI.a()*(i+1)/BI.delta_t())*(1.0-exp(-BI.a()/BI.delta_t()))/BI.a()*BI.delta_t()*100;
				s->scenario[2][i+1][j]+=rate*100;
				for(int k=0;k<BI.nb_rate();k++){
					if(BI.rate_type(k)==1) s->scenario[3+k][i+1][j]=Rate_Model::interest_rate(BI.a(),BI.sigma(),BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100,i,BI.delta_t());
					if(BI.rate_type(k)==2) s->scenario[3+k][i+1][j]=Rate_Model::swap_rate(BI.a(),BI.sigma(),BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100,i,BI.delta_t(),BI.coupon_period());
				}
				s->scenario[3+BI.nb_rate()][i+1][j]*=exp(BI.forward_rate1(i+1))/exp(BI.forward_rate1(i))*s->scenario[3+BI.nb_rate()][i][j];
				for(int k=0;k<BI.nb_eq();k++){
					s->scenario[4+BI.nb_rate()+k][i+1][j]*=s->scenario[3+BI.nb_rate()][i+1][j]/s->scenario[3+BI.nb_rate()][i][j];
					s->scenario[4+BI.nb_rate()+k][i+1][j]=(pow(s->scenario[4+BI.nb_rate()+k][i+1][j],12.0)-1.0)*100;
				}
				for(int k=0;k<BI.nb_bond();k++){
					if(BI.bond_coupon(k)<0.000001){
						s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][i+1][j]=Rate_Model::bond_return(BI.a(),BI.sigma(),BI.m_bond(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100.0,s->scenario[2][i][j]/100.0,i,BI.delta_t());
					}
					else{
						s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][i+1][j]=Rate_Model::bond_return_coupon(BI.a(),BI.sigma(),BI.m_bond(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100.0,s->scenario[2][i][j]/100.0,i,BI.delta_t(),BI.bond_coupon(k),BI.bond_freq(k));
					}
				}
			}
		}
	}

	cout<<"finished"<<endl<<endl;
}
*/

void Generator::calcul_scenario(Scenario *s,Basic_Information BI, bool closed_formula){
	//clock_t start_gen, end_gen;
	//cout<<"Calculating scenarios : ";
	//start_gen = clock();
	//clock_t t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15;
	//t1 = clock();
	
	if(closed_formula==0){
		bool swap_rate=false;
		for(int k=0;k<BI.nb_rate();k++){
			if(BI.rate_type(k)==2){
				swap_rate=true;
			}
		}


		for(int j=0;j<BI.nb_scens();j++){
			s->scenario[2][0][j]+=BI.forward_rate(0)*100;
			for(int k=0;k<BI.nb_eq();k++){
				s->scenario[4+BI.nb_rate()+k][0][j]=0;
			}
			for(int k=0;k<BI.nb_bond();k++){
				s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][0][j]=0;
			}
		}

		double ** forward_rates = new double* [BI.nb_step()+1];
		double ** forward_rates_save = new double* [BI.nb_step()+1];
		double ** zero_coupon = new double* [BI.nb_step()+1];
		double longest_maturity = 0;
		for(int k=0;k<BI.nb_rate();k++){
			longest_maturity = max(longest_maturity,BI.m_rate(k));
		}
		for(int k=0;k<BI.nb_bond();k++){
			longest_maturity = max(longest_maturity,BI.m_bond(k));
		}
		longest_maturity *= BI.delta_t()/6;
		longest_maturity++;
		for(int i=0;i<BI.nb_step()+1;i++){
			forward_rates[i]= new double [(int) longest_maturity];
			forward_rates_save[i]= new double [(int) longest_maturity];
			zero_coupon[i] = new double [(int) longest_maturity];
			for(int j=0;j<longest_maturity;j++){
				forward_rates[i][j]=0;
				forward_rates_save[i][j]=0;
				zero_coupon[i][j]=0;
			}
		}

		double ** forward_rates_bis = new double* [BI.nb_step()+1];
		double ** forward_rates_bis_save = new double* [BI.nb_step()+1];
		double ** zero_coupon_bis = new double* [BI.nb_step()+1];
		for(int i=0;i<BI.nb_step()+1;i++){
			forward_rates_bis[i]= new double [(int) longest_maturity];
			forward_rates_bis_save[i]= new double [(int) longest_maturity];
			zero_coupon_bis[i] = new double [(int) longest_maturity];
			for(int j=0;j<longest_maturity;j++){
				forward_rates_bis[i][j]=0;
				forward_rates_bis_save[i][j]=0;
				zero_coupon_bis[i][j]=0;
			}
		}
	
		double ** zero_coupon_ter = new double* [BI.nb_step()+1];
		for(int i=0;i<BI.nb_step()+1;i++){
			zero_coupon_ter[i] = new double [(int) longest_maturity];
			for(int j=0;j<longest_maturity;j++){
				zero_coupon_ter[i][j]=0;
			}
		}
	

		double f=-BI.sigma()*BI.sigma()/BI.a()/BI.a()/BI.a();

		//! Added by yuan !!!
		double delta = BI.delta_t(); //! YY
		double a = BI.a();           //! YY
		double sigma = BI.sigma();   //! YY
		double inv_delta = 1./delta;
	
		vector<double> e_r_negativ_a(BI.nb_step()+6);   //pow(e^r,-a/delta)
		e_r_negativ_a[0] = 0;  //not used
		for(unsigned int r=1; r<e_r_negativ_a.size(); r++)
		{
		   e_r_negativ_a[r] = exp(-a*r/delta);
		}

		vector<double> e_r_a(BI.nb_step()+6);   //pow(e^r,a/delta)
		e_r_a[0] = 0;  //not used
		for(unsigned int r=1; r<e_r_a.size(); r++)
		{
		    //e_r_a[r] = exp(a*r/delta);
			e_r_a[r] = 1.0/e_r_negativ_a[r];  // ?? precision ???
		}

		vector<double> e_c_negative_6_a((int)longest_maturity);   //pow(c^r,-6*a/delta)  
		for(unsigned int c=0; c<e_c_negative_6_a.size(); c++)
		{
		    e_c_negative_6_a[c] = exp(-a*6*c/delta);
		}

		double const1 = exp(-6*a/delta);   
		double const2 = exp(a/delta);
		double const3 = (1- const1) * sigma*delta/a;
		double const4 = sigma*delta/a;
		double const5 = sigma*(1.0-1/const2)/a*delta*100;



		for(int c=0;c<longest_maturity;c++){
			forward_rates[0][c] = BI.forward_rate(6*c)+BI.forward_rate(6*c+1)+BI.forward_rate(6*c+2)+BI.forward_rate(6*c+3)+BI.forward_rate(6*c+4)+BI.forward_rate(6*c+5);
		}

		forward_rates_bis[0][0] = BI.forward_rate(0)+BI.forward_rate(1)+BI.forward_rate(2)+BI.forward_rate(3)+BI.forward_rate(4);
		for(int c=1;c<longest_maturity;c++){
			forward_rates_bis[0][c] = BI.forward_rate(6*c-1)+BI.forward_rate(6*c)+BI.forward_rate(6*c+1)+BI.forward_rate(6*c+2)+BI.forward_rate(6*c+3)+BI.forward_rate(6*c+4);
		}


		for(int r=1;r<BI.nb_step()+1;r++){
			for(int c=0;c<longest_maturity;c++){
				double ed = e_r_a[r];
				double e1 = e_r_negativ_a[r]*e_c_negative_6_a[c];
				double e2 = e1*const1;
				forward_rates_save[r][c] = BI.forward_rate(r+6*c)+BI.forward_rate(r+6*c+1)+BI.forward_rate(r+6*c+2)+BI.forward_rate(r+6*c+3)+BI.forward_rate(r+6*c+4)+BI.forward_rate(r+6*c+5);
				forward_rates_save[r][c] += f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*delta;
			}
		}

		for(int r=1;r<BI.nb_step()+1;r++){
			double fd=exp(a*(r)/delta);
			double f1=exp(-a*(r)/delta);
			double f2=exp(-a*(r+5)/delta);
			forward_rates_bis_save[r][0] = BI.forward_rate(r)+BI.forward_rate(r+1)+BI.forward_rate(r+2)+BI.forward_rate(r+3)+BI.forward_rate(r+4);
			forward_rates_bis_save[r][0] += f * ((f2-f1)*(fd-1.0)-0.25*(f2*f2-f1*f1)*(fd*fd-1.0))*delta;
		}

		for(int r=1;r<BI.nb_step()+1;r++){
			for(int c=1;c<longest_maturity;c++){
				//! YY
				//double ed=exp(a*(r)/delta);      
				//double e1=exp(-a*(r+6*c-1)/delta);   // e^(-a*r/delta)*e^(-6*a*c/delta)*e^(a/delta)
				//double e2=exp(-a*(r+6*c+6-1)/delta); // e^(-a*r/delta)*e^(-6ac/delta)*e^(-5*a/delta) = e1*e(-6*a/delta)
				double ed = e_r_a[r];
				double e1 = e_r_negativ_a[r]*e_c_negative_6_a[c]*const2;
				double e2 = e1*const1;
				forward_rates_bis_save[r][c] = BI.forward_rate(r+6*c-1)+BI.forward_rate(r+6*c)+BI.forward_rate(r+6*c+1)+BI.forward_rate(r+6*c+2)+BI.forward_rate(r+6*c+3)+BI.forward_rate(r+6*c+4);
				forward_rates_bis_save[r][c] += f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*delta;
			}
		}

		//t2 = clock();

		double* forward = new double[BI.nb_step()];
		for(int i=0;i<BI.nb_step();i++){
			forward[i]=exp(BI.forward_rate1(i+1)-BI.forward_rate1(i));
		}

		int nb_rate = BI.nb_rate();
		int nb_equity = BI.nb_eq();

		for(int j=0;j<BI.nb_scens();j++){
		
			for(int r=1;r<BI.nb_step()+1;r++){
				double scen_2_r_j = s->scenario[2][r][j]*const3* e_r_negativ_a[r];
				for(int c=0;c<longest_maturity;c++){
					forward_rates[r][c] = forward_rates_save[r][c] + scen_2_r_j * e_c_negative_6_a[c];
				}
			}
		
			for(int r=0;r<BI.nb_step()+1;r++){
				for(int c=1;c<longest_maturity;c++){
					zero_coupon[r][c] = zero_coupon[r][c-1]+forward_rates[r][c-1]*inv_delta;
					if(swap_rate){zero_coupon_ter[r][c] = exp(-zero_coupon[r][c]);}
				}
			}
		
			for(int r=1;r<BI.nb_step()+1;r++){
				forward_rates_bis[r][0] = forward_rates_bis_save[r][0] + s->scenario[2][r][j] * const4 * (e_r_negativ_a[r]-e_r_negativ_a[r+5]);	
				double scen_2_r_j = s->scenario[2][r][j] * const3 * const2 * e_r_negativ_a[r];
				for(int c=1;c<longest_maturity;c++){
					forward_rates_bis[r][c] = forward_rates_bis_save[r][c] + scen_2_r_j * e_c_negative_6_a[c];	
				}
			}
		
			for(int r=0;r<BI.nb_step()+1;r++){
				for(int c=1;c<longest_maturity;c++){
					zero_coupon_bis[r][c] = zero_coupon_bis[r][c-1]+forward_rates_bis[r][c-1]*inv_delta;
				}
			}
		
			for(int k=0;k<BI.nb_rate();k++){
				if(BI.rate_type(k)==1) s->scenario[3+k][0][j]=Rate_Model::interest_rate(BI.m_rate(k),-1,delta,zero_coupon);
				if(BI.rate_type(k)==2) s->scenario[3+k][0][j]=Rate_Model::swap_rate(BI.m_rate(k),-1,delta,BI.rate_period(k),zero_coupon_ter);
			}
		
			for(int i=0;i<BI.nb_step();i++){
			
				double e1=e_r_negativ_a[i+1];
				double e2=e_r_negativ_a[i+2];
				double ed=e_r_a[i+1];
				double rate=BI.forward_rate(i+1)+ f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*delta;
				s->scenario[2][i+1][j] = s->scenario[2][i+1][j]*e1*const5 + rate*100;
			
				for(int k=0;k<nb_rate;k++){
					if(BI.rate_type(k)==1) s->scenario[3+k][i+1][j]=Rate_Model::interest_rate(BI.m_rate(k),i,delta,zero_coupon);
					if(BI.rate_type(k)==2) s->scenario[3+k][i+1][j]=Rate_Model::swap_rate(BI.m_rate(k),i,delta,BI.rate_period(k),zero_coupon_ter);
				}
			
				//s->scenario[3+nb_rate][i+1][j]*=exp(BI.forward_rate1(i+1)-BI.forward_rate1(i))*s->scenario[3+nb_rate][i][j];
				s->scenario[3+nb_rate][i+1][j]*=(forward[i])*s->scenario[3+nb_rate][i][j];

				for(int k=0;k<BI.nb_eq();k++){
					s->scenario[4+nb_rate+k][i+1][j]*=s->scenario[3+nb_rate][i+1][j]/s->scenario[3+nb_rate][i][j];
					double loc = s->scenario[4+nb_rate+k][i+1][j];
					double loc2 = loc*loc*loc;
					double loc3 = loc2*loc2;
					s->scenario[4+nb_rate+k][i+1][j]=(loc3*loc3-1.0)*100;
					//s->scenario[4+BI.nb_rate()+k][i+1][j]=(pow(s->scenario[4+BI.nb_rate()+k][i+1][j],12.0)-1.0)*100;
				}
			
				for(int k=0;k<BI.nb_bond();k++){
					if(BI.bond_coupon(k)<0.000001){
						s->scenario[4+nb_rate+nb_equity+k][i+1][j]=Rate_Model::bond_return(BI.m_bond(k),i,delta,zero_coupon_bis,zero_coupon);
					}
					else{
						s->scenario[4+nb_rate+nb_equity+k][i+1][j]=Rate_Model::bond_return_coupon(BI.m_bond(k),i,delta,zero_coupon_bis,zero_coupon,BI.bond_coupon(k),BI.bond_freq(k));
					}
				}
			
			}
		
		}
	
		//t10 = clock();

		for(int i=0;i<BI.nb_step()+1;i++){
			delete[] forward_rates[i];
			delete[] forward_rates_save[i];
			delete[] zero_coupon[i];
			delete[] zero_coupon_ter[i];
			delete[] forward_rates_bis[i];
			delete[] forward_rates_bis_save[i];
			delete[] zero_coupon_bis[i];
		}
		delete[] forward_rates;
		delete[] forward_rates_save;
		delete[] zero_coupon;
		delete[] zero_coupon_ter;
		delete[] forward_rates_bis;
		delete[] forward_rates_bis_save;
		delete[] zero_coupon_bis;
		delete[] forward;
	}
	//t11 = clock();

	else{

		int nb_rate = BI.nb_rate();
		int nb_equity = BI.nb_eq();
		double delta = BI.delta_t();
		double a = BI.a();
		double sigma = BI.sigma();
		double const1 = sigma*(1.0-exp(-a/delta))/a*delta*100;

		double longest_maturity = 0;
		for(int k=0;k<BI.nb_rate();k++){
			longest_maturity = max(longest_maturity,BI.m_rate(k));
		}
		for(int k=0;k<BI.nb_bond();k++){
			longest_maturity = max(longest_maturity,BI.m_bond(k));
		}
		int mat_max = int (longest_maturity*delta)+1;
		double* B1 = new double[mat_max];
		double* C1 = new double[BI.nb_step()+1];

		for(int i=0;i<mat_max;i++){
			B1[i]=(1-exp(-a*i/delta))/a;
		}

		for(int i=0;i<BI.nb_step()+1;i++){
			C1[i]=(sigma*sigma/4.0/a)*(1-exp(-2*a*i/delta));
		}

		double* forward = new double[BI.nb_step()];
		for(int i=0;i<BI.nb_step();i++){
			forward[i]=exp(BI.forward_rate1(i+1)-BI.forward_rate1(i));
		}

		for(int j=0;j<BI.nb_scens();j++){
			s->scenario[2][0][j]+=BI.forward_rate(0)*100;
			for(int k=0;k<BI.nb_rate();k++){
				if(BI.rate_type(k)==1) s->scenario[3+k][0][j]=Rate_Model::interest_rate(BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][0][j]/100,-1,delta,B1,C1);
				if(BI.rate_type(k)==2) s->scenario[3+k][0][j]=Rate_Model::swap_rate(BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][0][j]/100,-1,delta,BI.rate_period(k),B1,C1);
			}
			for(int k=0;k<BI.nb_eq();k++){
				s->scenario[4+BI.nb_rate()+k][0][j]=0;
			}
			for(int k=0;k<BI.nb_bond();k++){
				s->scenario[4+BI.nb_rate()+BI.nb_eq()+k][0][j]=0;
			}
		}
	
		for(int i=0;i<BI.nb_step();i++){
			double e1=exp(-a*(i+1)/delta);
			double e2=exp(-a*(i+2)/delta);
			double ed=exp(a*(i+1)/delta);
			double f=-sigma*sigma/a/a/a;
			double rate=BI.forward_rate(i+1)+ f * ((e2-e1)*(ed-1.0)-0.25*(e2*e2-e1*e1)*(ed*ed-1.0))*delta;
			for(int j=0;j<BI.nb_scens();j++){
				s->scenario[2][i+1][j]*=const1*e1;
				s->scenario[2][i+1][j]+=rate*100;
				
				for(int k=0;k<nb_rate;k++){
					if(BI.rate_type(k)==1) s->scenario[3+k][i+1][j]=Rate_Model::interest_rate(BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100,i,delta,B1,C1);
					if(BI.rate_type(k)==2) s->scenario[3+k][i+1][j]=Rate_Model::swap_rate(BI.m_rate(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100,i,delta,BI.rate_period(k),B1,C1);
				}
				//s->scenario[3+nb_rate][i+1][j]*=exp(BI.forward_rate1(i+1)-BI.forward_rate1(i))*s->scenario[3+nb_rate][i][j];
				s->scenario[3+nb_rate][i+1][j]*=(forward[i])*s->scenario[3+nb_rate][i][j];
				if(delta==12){
				for(int k=0;k<nb_equity;k++){
					s->scenario[4+nb_rate+k][i+1][j]*=s->scenario[3+nb_rate][i+1][j]/s->scenario[3+nb_rate][i][j];
					double loc = s->scenario[4+nb_rate+k][i+1][j];
					double loc2 = loc*loc*loc;
					double loc3 = loc2*loc2;
					s->scenario[4+nb_rate+k][i+1][j]=(loc3*loc3-1.0)*100;
					//s->scenario[4+BI.nb_rate()+k][i+1][j]=(pow(s->scenario[4+BI.nb_rate()+k][i+1][j],12.0)-1.0)*100;
					}
				}
				else{
					for(int k=0;k<nb_equity;k++){
						s->scenario[4+nb_rate+k][i+1][j]*=s->scenario[3+nb_rate][i+1][j]/s->scenario[3+nb_rate][i][j];
						s->scenario[4+nb_rate+k][i+1][j]=(pow(s->scenario[4+nb_rate+k][i+1][j],delta)-1.0)*100;
					}
				}
				
				//for(int k=0;k<BI.nb_eq();k++){
				//	s->scenario[4+BI.nb_rate()+k][i+1][j]*=s->scenario[3+BI.nb_rate()][i+1][j]/s->scenario[3+BI.nb_rate()][i][j];
				//	s->scenario[4+BI.nb_rate()+k][i+1][j]=(pow(s->scenario[4+BI.nb_rate()+k][i+1][j],12.0)-1.0)*100;
				//}
				
				for(int k=0;k<BI.nb_bond();k++){
					if(BI.bond_coupon(k)<0.000001){
						s->scenario[4+nb_rate+nb_equity+k][i+1][j]=Rate_Model::bond_return(BI.m_bond(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100.0,s->scenario[2][i][j]/100.0,i,delta,B1,C1);
					}
					else{
						s->scenario[4+nb_rate+nb_equity+k][i+1][j]=Rate_Model::bond_return_coupon(BI.m_bond(k),BI.forward_rate(),BI.forward_rate1(),s->scenario[2][i+1][j]/100.0,s->scenario[2][i][j]/100.0,i,delta,BI.bond_coupon(k),BI.bond_freq(k),B1,C1);
					}
				}
				
			}
		}
		delete[] B1;
		delete[] C1;
		delete[] forward;
	}
	
	//end_gen = clock();
	//cout<<"finished"<<endl;
	//cout << "time of calculating scenario = " << (end_gen - start_gen) << " msecond(s) " << endl;
}
