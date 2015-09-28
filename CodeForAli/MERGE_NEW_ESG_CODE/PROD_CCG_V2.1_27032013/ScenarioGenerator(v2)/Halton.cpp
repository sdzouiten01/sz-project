#include "Halton.h"
#include <math.h>

void allocate_seed(){
	Seed1[0]=3;Seed2[0]=7;
	Seed1[1]=5;Seed2[1]=7;
	Seed1[2]=11;Seed2[2]=13;
	Seed1[3]=17;Seed2[3]=19;
	Seed1[4]=23;Seed2[4]=29;
	Seed1[5]=31;Seed2[5]=37;
	Seed1[6]=41;Seed2[6]=43;
	Seed1[7]=47;Seed2[7]=53;
	Seed1[8]=59;Seed2[8]=61;
	Seed1[9]=67;Seed2[9]=71;
}

void VanDerCorput(int NSeed){
	div_t a1,a2;
	sum[0]=0;
	sum[1]=0;
	double p1=1.0,p2=1.0;
	if(period==0&&Ns==1){
		allocate_seed();
		base1=Seed1[0];
		base2=Seed2[0];
	}
	if(NSeed>Period){
		Period++;
		base1=Seed1[period];
		base2=Seed2[period];
		Ns=1;
	}
	int n=Ns;
	while(n>0){
		a1=div(n,base1);
		p1/=base1;
		sum[0]+=a1.rem*p1;
		n=a1.quot;
	}
	n=Ns;
	while(n>0){
		a2=div(n,base2);
		p2/=base2;
		sum[1]+=a2.rem*p2;
		n=a2.quot;
	}
	Ns++;
}

void Halton(int ns,
			  int nt,
			  int TYPE_Gaussian,
			  int antithetic,
			  double **U,
			  int NSeed)
{
	int row=ns/antithetic;
	
	if(TYPE_Gaussian!=2){
		for(int j=0;j<row;j++){
			VanDerCorput(NSeed);
			U[2*nt][j]=sum[0];
			U[2*nt+1][j]=sum[1];
		}
	}
	if(TYPE_Gaussian==2){
		double a,b;
		for(int j=0;j<row;j++){
			double r=1;
			while(r>=1){
				VanDerCorput(NSeed);
				a=2*sum[0]-1;
				b=2*sum[1]-1;
				r=a*a+b*b;
			}
			U[2*nt][j]=a;
			U[2*nt+1][j]=b;
		}
	}
}