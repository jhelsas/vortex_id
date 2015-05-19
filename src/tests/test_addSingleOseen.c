#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "floodFill.h"
#include "lambdaInit.h"

int main(int argc,char **argv){
  const int Width = 100, Height = 100, Pop=10,nVortex=1;
  int i,j,err,ngbr,found;
  int nbList[8],label[Width*Height],eqList[Pop],**eqClass;
  float parVortex[4*nVortex],x0[2],dx[2],xf[2],*sField=NULL,*gField;
  float x,y,v0y0 = 0.0;

  eqClass=(int**)malloc(NumCls*sizeof(int*));
  if(eqClass==NULL)
    return 1;
  for(i=0;i<NumCls;i+=1){
    eqClass[i]=(int*)malloc(NumCls*sizeof(int));
    if(eqClass[i]==NULL)
      return(i+2);
  }
  
  x0[0]=-5.; xf[0]= 5.; dx[0] = (xf[0]-x0[0])/Height;
  x0[1]=-5.; xf[1]= 5.; dx[1] = (xf[1]-x0[1])/Width;

  parVortex[0]=1.; parVortex[1]=1.; parVortex[2]=0.; parVortex[3]=0.;
 
  gField = (float *)malloc(4*Height*Width*sizeof(float));
  if(gField==NULL){
    printf("memory not allocked\n");
    return 1;
  }
  for(i=0;i<4*Height*Width;i+=1)
    gField[i]=0.;

  err = addSingleOseen(nVortex,parVortex,x0,dx,Height,Width,&gField);
  if(err!=0)
    printf("Problems in addSingleOseen\n");

  err = gradUtoLamb(Height,Width,gField,&sField);
  if(err!=0)
    printf("Problems in gradUtoLamb\n");
  
  err = floodFill(sField,Width,Height,eqClass,label);
  if(err!=0)
    printf("Problems in floodFill\n");

  err = renameLabels(Height,Width,label);
  if(err>0)
    printf("%d connected component(s)\n",err);
  else
    printf("problems with renameLabels - %d\n",err);

  {
    FILE *dadosout;
    dadosout=fopen("data/initLambOseen2D-0.txt","w");
    for(i=0;i<Height;i+=1)
      for(j=0;j<Width;j+=1){
        x = x0[0] + i*dx[0];
        y = x0[1] + j*dx[1];
        
        fprintf(dadosout,"%f %f %f\n",x,y,sField[i*Width+j]);
      }

    fclose(dadosout);dadosout=NULL;

    dadosout=fopen("data/labelLambOseen2D-0.txt","w");
    for(i=0;i<Height;i+=1){
      for(j=0;j<Width;j+=1){
        x = x0[0] + i*dx[0];
        y = x0[1] + j*dx[1];
        
        fprintf(dadosout,"%f %f %2d \n",x,y,label[i*Width+j]+1);
      }
      fprintf(dadosout,"\n");
    }

    fclose(dadosout);
  }
  
  if(sField!=NULL)
    free(sField);

  for(i=0;i<NumCls;i+=1)
    free(eqClass[i]);
  free(eqClass);

  return 0;
}
