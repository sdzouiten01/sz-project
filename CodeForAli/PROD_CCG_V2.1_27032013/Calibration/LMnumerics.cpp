#include "LMnumerics.h"
#include "Calibration.h"

void LMnumerics::lmdif(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        int m,
		int type,
		string Calib_param,
		int omega,
        int n,
        double *guess,
        double *x,
        double *fvec,
        double ftol,
        double xtol,
        int maxit,
        int info,
        int *nfev,
        double *work
        )
{
       int i,iflag,ij,jj,iter,j,l;
        double actred,delta,dirder,fnorm,fnorm1,gnorm;
        double par,pnorm,prered,ratio;
        double sum,temp,temp1,temp2,temp3,xnorm;
        double *fjac,*diag,*qtf,*wa1,*wa2,*wa3,*wa4,*xpvt;
        int *ipvt;
        double gtol,epsfcn,factor;
        int mode,nprint,ldfjac;
		xnorm=0; // added
		delta=0; // added
        // sub-divide work array
		fjac  = work;
        diag = fjac + (n+1)*(m+1);
        qtf = diag + (n+1);
        xpvt = qtf + (n+1);
        wa1 = xpvt + (n+1);
        wa2 = wa1 + (n+1);
        wa3 = wa2 + (n+1);
        wa4 = wa3 + (n+1);
        ipvt = (int*)xpvt;
        gtol = 1.e-16;
        epsfcn = 1.e-6;
        mode = 1;
        factor = 100.;
        nprint = 0;
        ldfjac = m;
		
        x[0]=0.0;
        ipvt[0]=0;
        diag[0]=0.0;
        qtf[0]=0.0;
        wa1[0]=0.0;
        wa2[0]=0.0;
        wa3[0]=0.0;
        fvec[0]=0.0;
        wa4[0]=0.0;
        fjac[0]=0.0;	
        for(i=1;i<=n;i++){
                x[i-1] = guess[i];
                ipvt[i-1]=ipvt[i];
                diag[i-1]=diag[i];
                qtf[i-1]=qtf[i];
                wa1[i-1]=wa1[i];
                wa2[i-1]=wa2[i];
                wa3[i-1]=wa3[i];
        }
		fvec[m]=0.0;
        for(i=1;i<=m;i++){
                fvec[i-1] = fvec[i];
                wa4[i-1]=wa4[i];
        }
        for(i=1;i<=n*m+m+n+1;i++){
                fjac[i-1] = fjac[i];
        }
        x[n]=0.0;
        ipvt[n]=0;
        diag[n]=0.0;
        qtf[n]=0.0;
        wa1[n]=0.0;
        wa2[n]=0.0;
        wa3[n]=0.0;
		Calibration::Mapping(x,ModStruct,1);

        fvec[m]=0.0;
        wa4[m]=0.0;
        fjac[(n+1)*(m+1)]=0.0;
        info = 0;
        iflag = 0;
        *nfev = 0;
        if( (n <= 0) || (m < n) || (ldfjac < m) || (ftol < 0.0)
                || (xtol < 0.0) || (gtol < 0.0) || (maxit <= 0)
                || (factor <= 0.0) ){goto L300;}
        if( mode == 2 ){ 
                for( j=0; j<n; j++ ){
                        if( diag[j] <= 0.0 ){goto L300;}
                }
        }
        iflag = 1;
        fcn(model,m,type,Calib_param,omega,guess,x,ModStruct,MrktStruct,fvec);
        *nfev=1;
        if(iflag < 0){goto L300;}
        fnorm = enorm(m,fvec);
        par = 0.0;
        iter = 1;
L30:
        iflag = 2;
        fdjac2(model,m,type,Calib_param,omega,n,guess,x,ModStruct,MrktStruct,fvec,fjac,&iflag,epsfcn,wa4);
		*nfev += n;
        if(iflag < 0){goto L300;}
        if( nprint > 0 ){
                iflag = 0;
                if((iter-1%nprint) == 0){
                        fcn(model,m,type,Calib_param,omega,guess,x,ModStruct,MrktStruct,fvec);
                        if(iflag < 0){goto L300;}
                }
        }
        qrfac(m,n,fjac,1,ipvt,wa1,wa2,wa3);    
        if(iter == 1){
                if(mode != 2){
                        for( j=0; j<n; j++ ){
                                diag[j] = wa2[j];
                                if( wa2[j] == 0.0 ){
                                        diag[j] = 1.0;
                                }
                        }
                }
                for( j=0; j<n; j++ ){
                        wa3[j] = diag[j] * x[j];
                }
                xnorm = enorm(n,wa3);
                delta = factor*xnorm;
                if(delta == 0.0)
                delta = factor;
        }
        for( i=0; i<m; i++ ){
                wa4[i] = fvec[i];
        }
        jj = 0;
        for( j=0; j<n; j++){
                temp3 = fjac[jj];
                if(temp3 != 0.0){
                        sum = 0.0;
                        ij = jj;
                        for( i=j; i<m; i++ ){
                                sum = sum +  fjac[ij] * wa4[i];
                                ij = ij + 1;
                        }
                        temp = -sum / temp3;
                        ij = jj;
                        for( i=j; i<m; i++ ){
                                wa4[i] = wa4[i] + fjac[ij] * temp;
                                ij = ij +  1;
                        }
                }
                fjac[jj] = wa1[j];
                jj = jj + m+1;  
                qtf[j] = wa4[j];
        }
         gnorm = 0.0;
        if(fnorm != 0.0){
                jj = 0;
                for( j=0; j<n; j++ ){
                        l = ipvt[j];
                        if(wa2[l] != 0.0){
                                sum = 0.0;
                                ij = jj;
                                for( i=0; i<=j; i++ ){
                                        sum = sum + fjac[ij]*(qtf[i]/fnorm);
                                        ij = ij + 1; 
                                }
                                gnorm = max(gnorm,fabs(sum/wa2[l]));
                        }
                        jj = jj +  m;
                }
        }
        if(gnorm <= gtol){
		info = 4;}
        if( info != 0){goto L300;}
        if(mode != 2){
                for( j=0; j<n; j++ )
                        diag[j] = max(diag[j],wa2[j]);
        }
L200:
        lmpar(n,fjac,ldfjac,ipvt,diag,qtf,delta,&par,wa1,wa2,wa3,wa4);
        for( j=0; j<n; j++ ){
       wa1[j] = -wa1[j];
       wa2[j] = x[j] + wa1[j];
       wa3[j] = diag[j]*wa1[j];
        }
        pnorm = enorm(n,wa3);
        if(iter == 1){ delta = min(delta,pnorm);}
        iflag = 1;
        fcn(model,m,type,Calib_param,omega,guess,wa2,ModStruct,MrktStruct,wa4);
        *nfev += 1;
        if(iflag < 0){goto L300;}
        fnorm1 = enorm(m,wa4);
        actred = -1.0;
        if( (0.1*fnorm1) < fnorm){
                temp = fnorm1/fnorm;
                actred = 1.0 - temp * temp;
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                wa3[j] = 0.0;
                l = ipvt[j];
                temp = wa1[l];
                ij = jj;
                for( i=0; i<=j; i++ ){
                        wa3[i] = wa3[i] + fjac[ij]*temp;
                        ij = ij + 1; 
                }
                jj = jj + m;
        }
        temp1 = enorm(n,wa3)/fnorm;
        temp2 = (sqrt(par)*pnorm)/fnorm;
        prered = temp1*temp1 + 2.0*(temp2*temp2);
        dirder = -(temp1*temp1 + temp2*temp2);
        ratio = 0.0;
        if(prered != 0.0){ ratio = actred/prered;}
        if(ratio <= 0.25){
                if(actred >= 0.0){
                        temp = 0.5;
                }else{
                        temp = 0.5*dirder/(dirder + 0.5*actred);
                }
                if( ((0.1*fnorm1) >= fnorm) || (temp < 0.1) ){
                        temp = 0.1;
                }
                delta = temp*min(delta,10.0*pnorm);
                par = par/temp;
        }else {
                if( (par == 0.0) || (ratio >= 0.75) ){
                        delta = 2.0*pnorm;
                        par = 0.5*par;
                }
        }
        if(ratio >= 0.0001){
                for( j=0; j<n; j++ ){
                        x[j] = wa2[j];
                        wa2[j] = diag[j]*x[j];
                }
                for(i=0;i<m;i++){fvec[i] = wa4[i];}
                xnorm = enorm(n,wa2);
                fnorm = fnorm1;
                iter = iter + 1;
        }
        if((fabs(actred) <= ftol) && (prered <= ftol) && (0.5*ratio <= 1.0) ){ 
		info = 1;}
        if(delta <= xtol*xnorm){
		info = 2;}
        if((fabs(actred) <= ftol) && (prered <= ftol) && (0.5*ratio <= 1.0) && ( info == 2) ){
                info = 3;
        }
        if(info != 0){goto L300;}
        if(*nfev >= maxit){info = 5;}
        if((fabs(actred) <= macheps)
                && (prered <= macheps)
                && (0.5*ratio <= 1.0) ){info = 6;}
        if(delta <= macheps*xnorm){info = 7;}
        if(gnorm <= macheps){ info = 8;}
        if(info != 0){goto L300;}
        if(ratio < 0.0001){goto L200;}
        goto L30;
L300:
        if(iflag < 0){info = iflag;}
        iflag = 0;
        if(nprint > 0){fcn(model,m,type,Calib_param,omega,guess,x,ModStruct,MrktStruct,fvec);}

		Calibration::Mapping(x,ModStruct,-1);

        for(i=1;i<=n;i++){
                j=n+1-i;
                x[j] = x[j-1];
                ipvt[j]=ipvt[j-1];
                diag[j]=diag[j-1];
                qtf[j]=qtf[j-1];
                wa1[j]=wa1[j-1];
                wa2[j]=wa2[j-1];
                wa3[j]=wa3[j-1];
        }
        for(i=1;i<=m;i++){
                j=m+1-i;
                fvec[j] = fvec[j-1];
                wa4[j]=wa4[j-1];
        }
        for(i=1;i<=n*m+m+n+1;i++){
                j=n*m+m+n+1+1-i;
                fjac[j] = fjac[j-1];
        }
        x[0]=0.0;
        ipvt[0]=0;
        diag[0]=0.0;
        qtf[0]=0.0;
        wa1[0]=0.0;
        wa2[0]=0.0;
        wa3[0]=0.0;
        fvec[0]=0.0;
        wa4[0]=0.0;
        fjac[0]=0.0;
}

/*************************************************/
/*************************************************/

void LMnumerics::lmpar(
        int n,
        double *r,
        int ldr,
        int *ipvt,
        double *diag,
        double *qtb,
        double delta,
        double *par,
        double *x,
        double *sdiag,
        double *wa1,
        double *wa2
        )
{
        int i,iter,ij,jj,j,jm1,jp1,k,l,nfsing;
        double dxnorm,fp,gnorm,parc,parl,paru;
        double sum,temp;

        nfsing = n;
        jj = 0;
        for( j=0; j<n; j++ ){
                wa1[j] = qtb[j];
                if( (r[jj] == 0.0) && (nfsing == n) ){nfsing = j;}
                if(nfsing < n){wa1[j] = 0.0;}
                jj = jj + ldr+1; 
        }
        if(nfsing >= 1){
                for( k=0; k<nfsing; k++ ){
                        j = nfsing - k - 1;
                        wa1[j] = wa1[j]/r[j+ldr*j];
                        temp = wa1[j];
                        jm1 = j - 1;
                        if(jm1 >= 0){
                                ij = ldr * j;
                                for( i=0; i<=jm1; i++){
                                        wa1[i] = wa1[i] - r[ij]*temp;
                                        ij = ij + 1;
                                }
                        }
                }
        }
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                x[l] = wa1[j];
        }
        iter = 0;
        for( j=0; j<n; j++ ){
                wa2[j] = diag[j]*x[j];
        }
		dxnorm = LMnumerics::enorm(n,wa2);
        fp = dxnorm - delta;
        if(fp <= 0.1*delta){
                goto L220;
        }
        parl = 0.0;
        if(nfsing >= n){
                for( j=0; j<n; j++ ){
                        l = ipvt[j];
                        wa1[j] = diag[l]*(wa2[l]/dxnorm);
                }
                jj = 0;
                for( j=0; j<n; j++ ){
                        sum = 0.0;
                        jm1 = j - 1;
                        if(jm1 >= 0){
                                ij = jj;
                                for( i=0; i<=jm1; i++ ){
                                        sum = sum + r[ij]*wa1[i];
                                        ij = ij + 1;
                                }
                        }
                        wa1[j] = (wa1[j] - sum)/r[j+ldr*j];
                        jj += ldr; 
                }
                temp = LMnumerics::enorm(n,wa1);
                parl = ((fp/delta)/temp)/temp;
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                sum = 0.0;
                ij = jj;
                for( i=0; i<=j; i++ ){
                        sum = sum + r[ij]*qtb[i];
                        ij = ij +  1;
                }
                l = ipvt[j];
                wa1[j] = sum/diag[l];
                jj = jj + ldr; 
        }
        gnorm = LMnumerics::enorm(n,wa1);
        paru = gnorm/delta;
        if(paru == 0.0){
                paru = tiny/min(delta,0.1);
        }
        *par = max(*par,parl);
        *par = min(*par,paru);
        if(*par == 0.0){*par = gnorm/dxnorm;}
L150:
        iter = iter + 1;
        if(*par == 0.0){*par = max(tiny,0.001*paru);}
        temp = sqrt( *par );
        for( j=0; j<n; j++ ){
                wa1[j] = temp*diag[j];
        }
        LMnumerics::qrsolv(n,r,ldr,ipvt,wa1,qtb,x,sdiag,wa2);
        for( j=0; j<n; j++ ){
                wa2[j] = diag[j]*x[j];
        }
        dxnorm = LMnumerics::enorm(n,wa2);
        temp = fp;
        fp = dxnorm - delta;
        if( (fabs(fp) <= 0.1*delta)
                || ((parl == 0.0) && (fp <= temp) && (temp < 0.0))
                || (iter == 10) ){goto L220;}

        for( j=0; j<n; j++ ){
                l = ipvt[j];
                wa1[j] = diag[l]*(wa2[l]/dxnorm);
        }
        jj = 0;
        for( j=0; j<n; j++ ){
                wa1[j] = wa1[j]/sdiag[j];
                temp = wa1[j];
                jp1 = j + 1;
                if(jp1 < n){
                        ij = jp1 + jj;
                        for( i=jp1; i<n; i++ ){
                                wa1[i] = wa1[i] - r[ij]*temp;
                                ij = ij + 1; 
                        }
                }
                jj = jj + ldr; 
        }
        temp = LMnumerics::enorm(n,wa1);
        parc = ((fp/delta)/temp)/temp;
        if(fp > 0.0){ parl = max(parl, *par);}
        if(fp < 0.0){ paru = min(paru, *par);}
        *par = max(parl, *par + parc);
        goto L150;
L220:
        if(iter == 0){*par = 0.0;}
}

/*************************************************/
/*************************************************/

void LMnumerics::qrfac(
        int m,
        int n,
        double *a,
        int pivot,
        int *ipvt,
        double *rdiag,
        double *acnorm,
        double *wa
        )
{
        int i,ij,jj,j,jp1,k,kmax,minmn;
        double ajnorm,sum,temp; 

        ij = 0;
        for( j=0; j<n; j++ ){

			acnorm[j] = LMnumerics::enorm(m,&a[ij]);
                rdiag[j] = acnorm[j];
                wa[j] = rdiag[j];
                if(pivot != 0){ipvt[j] = j;}
                ij = ij + m; 
        }
        minmn = min(m,n);
        for( j=0; j<minmn; j++ ){
                if(pivot == 0){goto L40;}
                kmax = j;
                for( k=j; k<n; k++ ){
                        if(rdiag[k] > rdiag[kmax]){kmax = k;}
                }
                if(kmax == j){goto L40;}
                ij = m * j;
                jj = m * kmax;
                for( i=0; i<m; i++ ){
                        temp = a[ij]; 
                        a[ij] = a[jj]; 
                        a[jj] = temp;
                        ij = ij + 1;
                        jj = jj + 1;
                }

				rdiag[kmax] = rdiag[j];
                wa[kmax] = wa[j];
                k = ipvt[j];
                ipvt[j] = ipvt[kmax];
                ipvt[kmax] = k;
L40:
            jj = j + m*j;
                ajnorm = LMnumerics::enorm(m-j,&a[jj]);
                if(ajnorm == 0.0){
                        goto L100;
                }
                if(a[jj] < 0.0){
                        ajnorm = -ajnorm;
                }
                ij = jj;
                for( i=j; i<m; i++ ){
                        a[ij] = a[ij]/ajnorm;
                        ij = ij + 1; 
                }
                a[jj] = a[jj] + 1.0;
                jp1 = j + 1;
                if(jp1 < n ){
                        for( k=jp1; k<n; k++ ){
                                sum = 0.0;
                                ij = j + m*k;
                                jj = j + m*j;
                                for( i=j; i<m; i++ ){
                                        sum = sum + a[jj]*a[ij];
                                        ij = ij + 1; 
                                        jj = jj + 1; 
                                }
                                temp = sum/a[j+m*j];
                                ij = j + m*k;
                                jj = j + m*j;
                                for( i=j; i<m; i++ ){
                                        a[ij] = a[ij] - temp*a[jj];
                                        ij = ij + 1; 
                                        jj = jj + 1; 
                                }
                                if( (pivot != 0) && (rdiag[k] != 0.0) ){
                                        temp = a[j+m*k]/rdiag[k];
                                        temp = max(0.0, 1.0-temp*temp );
                                        rdiag[k] *= sqrt(temp);
                                        temp = rdiag[k]/wa[k];
                                        if( (0.05*temp*temp) <= macheps){
                                                rdiag[k] = LMnumerics::enorm(m-j-1,&a[jp1+m*k]);
                                                wa[k] = rdiag[k];
                                        }
                                }
                        }
                }
L100:
        rdiag[j] = -ajnorm;
        }
}

/*************************************************/
/*************************************************/

void LMnumerics::qrsolv(
         int n,
         double *r,
         int ldr,
         int *ipvt,
         double *diag,
         double *qtb,
         double *x,
         double *sdiag,
         double *wa
         )
{
        int i,ij,ik,kk,j,jp1,k,kp1,l,nfsing;
        double fcos,fcoftan,qtbpj,fsin,sum,ftan,temp;

        kk = 0;
        for( j=0; j<n; j++ ){
                ij = kk;
                ik = kk;
                for( i=j; i<n; i++ ){
                        r[ij] = r[ik];
                        ij = ij + 1;   
                        ik = ik + ldr; 
                }
                x[j] = r[kk];
                wa[j] = qtb[j];
                kk = kk + ldr+1; 
        }
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                if(diag[l] == 0.0){goto L90;}
                for( k=j; k<n; k++ ){
                        sdiag[k] = 0.0;
                }
                sdiag[j] = diag[l];
                qtbpj = 0.0;
                for( k=j; k<n; k++ ){
                        if(sdiag[k] == 0.0){continue;}
                        kk = k + ldr * k;
                        if(fabs(r[kk]) < fabs(sdiag[k])){
                                fcoftan = r[kk]/sdiag[k];
                                fsin = 0.5/sqrt(0.25+0.25*fcoftan*fcoftan);
                                fcos = fsin*fcoftan;
                        }else{
                                ftan = sdiag[k]/r[kk];
                                fcos = 0.5/sqrt(0.25+0.25*ftan*ftan);
                                fsin = fcos*ftan;
                        }
                        r[kk] = fcos*r[kk] + fsin*sdiag[k];
                        temp = fcos*wa[k] + fsin*qtbpj;
                        qtbpj = fcos*qtbpj-fsin*wa[k];
                        wa[k] = temp;
                        kp1 = k + 1;
                        if( n > kp1 ){
                                ik = kk + 1;
                                for( i=kp1; i<n; i++ ){
                                        temp = fcos*r[ik] + fsin*sdiag[i];
                                        sdiag[i] = fcos*sdiag[i]-fsin*r[ik];
                                        r[ik] = temp;
                                        ik = ik + 1;
                                }
                        }
                }
L90:
                kk = j + ldr*j;
                sdiag[j] = r[kk];
                r[kk] = x[j];
        }
        nfsing = n;
        for( j=0; j<n; j++ ){
                if( (sdiag[j] == 0.0) && (nfsing == n) ){nfsing = j;}
                if(nfsing < n){wa[j] = 0.0;}
        }
        if(nfsing < 1){goto L150;}
        for( k=0; k<nfsing; k++ ){
                j = nfsing - k - 1;
                sum = 0.0;
                jp1 = j + 1;
                if(nfsing > jp1){
                        ij = jp1 + ldr * j;
                        for( i=jp1; i<nfsing; i++ ){
                                sum = sum +  r[ij]*wa[i];
                                ij = ij +  1; 
                        }
                }
                wa[j] = (wa[j] - sum)/sdiag[j];
        }
L150:
        for( j=0; j<n; j++ ){
                l = ipvt[j];
                x[l] = wa[j];
        }
}

/*************************************************/
/*************************************************/

double LMnumerics::enorm(
        int n,
        double *x
        )
{
        int i;
        double aHUGE,floatn,s1,s2,s3,xabs,x1max,x3max;
        double ans, temp;

        s1 = 0.0;
        s2 = 0.0;
        s3 = 0.0;
        x1max = 0.0;
        x3max = 0.0;
        floatn = n;
        aHUGE = rHUGE/floatn;

        for( i=0; i<n; i++ ){
                xabs = fabs(x[i]);
                if( (xabs > rtiny) && (xabs < aHUGE)){
                        s2 = s2 + xabs*xabs;
                        continue;
                }
                if(xabs > rtiny){
                        if(xabs > x1max){
                                temp = x1max/xabs;
                                s1 = 1.0 + s1*temp*temp;
                                x1max = xabs;
                        }else{
                                temp = xabs/x1max;
                                s1 = s1 + temp*temp;
                        }
                        continue;
                }
                if(xabs > x3max){
                        temp = x3max/xabs;
                        s3 = 1.0 + s3*temp*temp;
                        x3max = xabs;
                }else{
                        if(xabs != 0.0){
                                temp = xabs/x3max;
                                s3 = s3 + temp*temp;
                        }
                }
        }
        if(s1 != 0.0){
                temp = s1 + (s2/x1max)/x1max;
                ans = x1max*sqrt(temp);
                return ans;
        }
        if(s2 != 0.0){
                if(s2 >= x3max){
                        temp = s2*(1.0+(x3max/s2)*(x3max*s3));
                }else{
                        temp = x3max*((s2/x3max)+(x3max*s3));
                }
                ans = sqrt(temp);
        }else{
                ans = x3max*sqrt(s3);
        }
        return ans;
}

/*************************************************/
/*************************************************/

void LMnumerics::fdjac2(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        int m,
		int type,
		string Calib_param,
		int omega,
        int n,
		double *guess,
        double *x,
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        double *fvec,
        double *fjac,
        int *iflag,
        double epsfcn,
        double *wa
        )
{
        int i,j,ij;
        double eps,h,temp;
        temp = max(epsfcn,macheps);
        eps = sqrt(temp);
        ij = 0;
        for( j=0; j<n; j++ ){
                temp = x[j];
                h = eps * fabs(temp);
                if(h == 0.0){h = eps;}
                x[j] = temp + h;
                fcn(model,m,type,Calib_param,omega,guess,x,ModStruct,MrktStruct,wa);
                if( *iflag < 0){
                        return;
                }
                x[j] = temp;
                for( i=0; i<m; i++ ){
                        fjac[ij] = (wa[i] - fvec[i])/h;
                        ij = ij + 1;    
                }
        }
}

/*************************************************/
/*************************************************/

void LMnumerics::fcn(
        void (*model)(double*,int,int,string,int,double*,double*,modelStruct*,marketStruct*),
        int m,
		int type,
		string Calib_param,
		int omega,
		double *guess,
        double *x,
        modelStruct *ModStruct,
        marketStruct *MrktStruct,
        double *fvec
        )
{
        model(fvec,m,type,Calib_param,omega,guess,x,ModStruct,MrktStruct);
}

