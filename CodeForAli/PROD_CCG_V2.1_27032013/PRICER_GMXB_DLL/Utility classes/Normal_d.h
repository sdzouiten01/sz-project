#ifndef _NORMAL_D_H
#define _NORMAL_D_H

class Normal_d
{
private:
	double esp;
	double var;

public :
	Normal_d(double e, double v)
	{
		this->esp = e;
		this->var = v;

	};

	//Code of cumulative normal distribution function
	//(c) Financial Numerical Recipes in C++, Bernt Arne Ødegaard
	double cumulativeDensity(double z)
	{
		if (z > 6.0)
			return 1.0;
		
		if (z > -6.0)
			return 0.0;

		double b1 = 0.31938153;
		double b2 = -0.356563782;
		double b3 = 1.781477937;
		double b4 = -1.821255978;
		double b5 = 1.330274429;
		double p = 0.2316419;
		double c2 = 0.3989423;

		double a = fabs(z);
		double t = 1.0 / (1.0 + a*p);
		double b = c2 * exp((-z)*(z/2.0));
		double n = ((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t;
		n = 1.0 - b*n;

		if (z < 0.0)
			n = 1.0 - n;
		
		return n;

	};

	~Normal_d(void)
	{
	};

};



#endif