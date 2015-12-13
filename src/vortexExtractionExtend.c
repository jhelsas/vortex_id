#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lambdaInit.h"
#include "floodFill.h"
#include "vortexExtraction.h"

int add_dgradU(int Height,int Width, int i,int j,int ik,int jk,double *gField,
               double dgradU[][2],double X[],double Y[])
{

  dgradU[0][1] = 9.*gField[4*(     i*Width     +  j )+1]+
                 3.*gField[4*((i+ik)*Width     +  j )+1]+
                 3.*gField[4*(     i*Width   +(j+jk))+1]+
                 1.*gField[4*((i+ik)*Width   +(j+jk))+1];

  dgradU[1][0] = 9.*gField[4*(     i*Width +   j  )+2]+
                 3.*gField[4*((i+ik)*Width +   j  )+2]+
                 3.*gField[4*(     i*Width +(j+jk))+2]+
                 1.*gField[4*((i+ik)*Width +(j+jk))+2];

  dgradU[0][1] *= fabs( (Y[i+ik]-Y[i])*(X[j+jk]-X[j]) )/64.0;
  dgradU[1][0] *= fabs( (Y[i+ik]-Y[i])*(X[j+jk]-X[j]) )/64.0;

  return 0;
}

int add_dxdy(int Height,int Width, int i,int j,int ik,int jk,double *gField,
             double *dx,double *dy,double X[],double Y[])
{

  *dy = 9.*gField[4*(     i*Width  + j  )+2]*Y[i]  +
        3.*gField[4*((i+ik)*Width  + j  )+2]*Y[i+1]+
        3.*gField[4*(     i*Width+(j+jk))+2]*Y[i]  +
        1.*gField[4*((i+ik)*Width+(j+jk))+2]*Y[i+1];

  *dy-= 9.*gField[4*(     i*Width  + j  )+1]*Y[i]  +
        3.*gField[4*((i+ik)*Width  + j  )+1]*Y[i+1]+
        3.*gField[4*(     i*Width+(j+jk))+1]*Y[i]  +
        1.*gField[4*((i+ik)*Width+(j+jk))+1]*Y[i+1];
        
  *dx = 9.*gField[4*(     i*Width  + j  )+2]*X[j]  +
        3.*gField[4*((i+ik)*Width  + j  )+2]*X[j]  +
        3.*gField[4*(     i*Width+(j+jk))+2]*X[j+1]+
        1.*gField[4*((i+ik)*Width+(j+jk))+2]*X[j+1];

  *dx-= 9.*gField[4*(     i*Width  + j  )+1]*X[j]  +
        3.*gField[4*((i+ik)*Width  + j  )+1]*X[j]  +
        3.*gField[4*(     i*Width+(j+jk))+1]*X[j+1]+
        1.*gField[4*((i+ik)*Width+(j+jk))+1]*X[j+1];
        
  *dy *= fabs( (Y[i+ik]-Y[i])*(X[j+jk]-X[j]) )/64.0;
  *dx *= fabs( (Y[i+ik]-Y[i])*(X[j+jk]-X[j]) )/64.0;

  return 0;
}

int add_dA(int Height,int Width, int i,int j,int k,int ik,int jk,
           double *gField,int *label,double *dA,double X[],double Y[])
{
  
  *dA = 9.*((label[     i*Width  +  j ]==k)?1.0:0.0)+
        3.*((label[(i+ik)*Width  +  j ]==k)?1.0:0.0)+
        3.*((label[     i*Width+(j+jk)]==k)?1.0:0.0)+
        1.*((label[(i+ik)*Width+(j+jk)]==k)?1.0:0.0);
       
  *dA *= (Y[i+1]-Y[i])*(X[j+1]-X[j])/64.0;

  return 0;
}

int vortexExtractionExtend(int Height,int Width, int nCnect,double *X,double *Y,
                           double *sField,double *gField,int *label,
                           double **vCatalogOut)
{
  int i,j,k,err;
  double G,a,b,rc,dx,dy,dA,dgradU[2][2]; // vorticity
  double w[nCnect],A[nCnect],a0[nCnect],b0[nCnect];
  double *vCatalog=NULL;

  if((Height<=0)||(Width<=0))
    return -1;
  if(vCatalogOut==NULL)
    return -2;
  
  vCatalog = *vCatalogOut;
  
  for(k=0;k<nCnect;k+=1){
    w[k]=0.;A[k]=0.;a0[k]=0.;b0[k]=0.;
  }

  for(i=1;i<Height-1;i+=1)
    for(j=1;j<Width-1;j+=1){
      k=label[i*Width+j];

      if((k>=0)&&(k<nCnect)){
        // ++ quadrant        
        err=add_dgradU(Height,Width,i,j,1,1,gField,dgradU,X,Y);
        w[k] += dgradU[1][0]-dgradU[0][1];
        
        err=add_dxdy(Height,Width,i,j,1,1,gField,&dx,&dy,X,Y);
        a0[k] += dx;
        b0[k] += dy;

        err=add_dA(Height,Width,i,j,k,1,1,gField,label,&dA,X,Y);
        A[k] += dA;
        
        /*************************************************/
        
        // -+ quadrant
        err=add_dgradU(Height,Width,i,j,-1,1,gField,dgradU,X,Y);
        w[k] += dgradU[1][0]-dgradU[0][1];
        
        err=add_dxdy(Height,Width,i,j,-1,1,gField,&dx,&dy,X,Y);
        a0[k] += dx;
        b0[k] += dy;

        err=add_dA(Height,Width,i,j,k,-1,1,gField,label,&dA,X,Y);
        A[k] += dA;
        
        /*************************************************/

        // +- quadrant
        err=add_dgradU(Height,Width,i,j,1,-1,gField,dgradU,X,Y);
        w[k] += dgradU[1][0]-dgradU[0][1];
        
        err=add_dxdy(Height,Width,i,j,1,-1,gField,&dx,&dy,X,Y);
        a0[k] += dx;
        b0[k] += dy;

        err=add_dA(Height,Width,i,j,k,1,-1,gField,label,&dA,X,Y);
        A[k] += dA;
        
        /*************************************************/
        
        // -- quadrant
        err=add_dgradU(Height,Width,i,j,-1,-1,gField,dgradU,X,Y);
        w[k] += dgradU[1][0]-dgradU[0][1];
        
        err=add_dxdy(Height,Width,i,j,-1,-1,gField,&dx,&dy,X,Y);
        a0[k] += dx;
        b0[k] += dy;

        err=add_dA(Height,Width,i,j,k,1,-1,gField,label,&dA,X,Y);
        A[k] += dA;
        
        /*************************************************/
      }
    }

  for(k=0;k<nCnect;k+=1){
    // 0.977816 corrects for gridsize
    rc= sqrt(A[k]/M_PI)*(sqrtf(2.));//0.977816); // Constant comming from lamb-oseen vortex;
    if(w[k]>0.){
      a=a0[k]/w[k]; 
      b=b0[k]/w[k];
    }
    else{
      a=0.;
      b=0.;
    }
    // 2.541494083 = 1/(1-1/sqrt(e)) ... should correct for finite grid size?
    //G = 2.55399076*rc*rc*w[k];
    G = 2.541494083*rc*rc*w[k];
    
    vCatalog[4*k+0] = G;
    vCatalog[4*k+1] = rc;
    vCatalog[4*k+2] = a;
    vCatalog[4*k+3] = b;
  }

  *vCatalogOut=vCatalog;

  return 0;
}