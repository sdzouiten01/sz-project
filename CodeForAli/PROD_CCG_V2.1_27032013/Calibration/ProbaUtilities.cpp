#include "ProbaUtilities.h"
#include <math.h>

double CumNorm(double xraw)
{
	double a[4] = {3.1611237438705e+00, 1.1386415415105e+02,
		           3.7748523768530e+02, 3.2093775891384e+03};
	double b[4] = {2.3601290952344e+01, 2.4402463793444e+02,
		           1.2826165260773e+03, 2.8442368334391e+03};
	double c[8] = {5.6418849698867e-01, 8.8831497943883e+00,
		           6.6119190637141e+01, 2.9863513819740e+02,
			       8.8195222124176e+02, 1.7120476126340e+03,
				   2.0510783778260e+03, 1.2303393547979e+03};
	double d[8] = {1.5744926110709e+01, 1.1769395089131e+02,
		           5.3718110186200e+02, 1.6213895745666e+03,
			       3.2907992357334e+03, 4.3626190901432e+03,
				   3.4393676741437e+03, 1.2303393548037e+03};
	double p[5] = {3.0532663496123e-01, 3.6034489994980e-01,
		           1.2578172611122e-01, 1.6083785148742e-02,
			       6.5874916152983e-04};
	double q[5] = {2.5685201922898e+00, 1.8729528499234e+00,
		           5.2790510295142e-01, 6.0518341312441e-02,
			       2.3352049762686e-03};

	double anorm = 1.8577770618460e-01;
	double cnorm = 2.1531153547440e-08;
	double pnorm = 1.6315387137302e-02;
	double rrtpi = 5.6418958354776e-01;
	double xbreak,result,y,z,xden,xnum,del,pi,ax,x;
	int i;

	xbreak = 0.46875;
	pi = 3.14159265358979;
    x = xraw*0.70710678118655;
    ax = fabs(x);


	if (ax <= xbreak) {
		y = ax;
		z = y*y;
		xnum = anorm*z;
		xden = z;
		for (i=0; i<=2; i++) {
			xnum = (xnum + a[i])*z;
			xden = (xden + b[i])*z;
		}
		result = x*(xnum + a[3]) / (xden + b[3]);
	}

	
	else if ((ax > xbreak) & (ax <= 4.)) {
		y = ax;
		xnum = cnorm*y;
		xden = y;
		for (i=0; i<=6; i++) {
			xnum = (xnum + c[i])*y;
			xden = (xden + d[i])*y;
		}
		result = (xnum + c[7]) / (xden + d[7]);
		z = floor(y*16.)/16.;
		del = (y-z)*(y+z);
		result = result*exp(-z*z-del);
	}


	else {
		y = ax;
		z = 1. / (y*y);
		xnum = pnorm*z;
		xden = z;
		for (i=0; i<=3; i++) {
			xnum = (xnum + p[i])*z;
			xden = (xden + q[i])*z;
		}
		result = z*(xnum + p[4]) / (xden + q[4]);
		result = (rrtpi -  result) / y;
		z = floor(y*16.)/16.;
		del = (y-z)*(y+z);
		result = result*exp(-z*z-del);
	}

	if (ax>xbreak) {
		result = 1. - result;
	}
	if (x<=-xbreak) {
		result = -result;
	}

	return 0.5*(1. + result);
}




