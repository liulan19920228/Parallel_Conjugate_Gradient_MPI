#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include "matvec_test.c"

double ddot(double* v, double* w, int n, int p);
double* saxpy(double a, double* v, double* w, int n, int p);
double* matvec(double *v, int n, int rank, int p);

double* cgsolve(int p, int n, int rank, double *b, int *niters, int maxiterations, double *norm)
{
    
double relres=1, alpha, beta,normb,rtrold,rtr;
double * Ad = malloc(n/p * sizeof(double)), * d = malloc(n/p * sizeof(double)), *x = malloc(n/p * sizeof(double)), *r = malloc(n/p * sizeof(double));
double dot_product, dAd;
int i;
for(i=0;i<=n/p-1;i++)
{
    x[i]=0;
    r[i]=b[i];
    d[i]=r[i];

}
dot_product = ddot(r,r,n,p);
MPI_Reduce(&dot_product, &rtr,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
//printf("%d dot_product %f\n", rank, dot_product);
    if(rank==0)
{
    normb=sqrt(rtr);
}
while(relres > 1e-8 && *niters < maxiterations)
{
   *niters = *niters+1;
    Ad = matvec(d, n, rank, p);
    dot_product = ddot(d,Ad,n,p);
    MPI_Reduce(&dot_product,&dAd,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
 //   printf("%d dot_product2 %f\n", rank,dot_product);
    if(rank==0)
    {
        alpha = rtr / dAd;
    }
    MPI_Bcast(&alpha,1, MPI_DOUBLE,0,MPI_COMM_WORLD);
    x =saxpy(alpha,x,d,n,p);
   /* for(i=0;i<n/p;i++)
        printf("iloop xi= %f", x[i]);*/
    r =saxpy(-1*alpha,r,Ad,n,p);
    if(rank==0)
    {
        rtrold = rtr;
    }
    dot_product=ddot(r,r,n,p);
    MPI_Reduce(&dot_product,&rtr,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    //printf("%d dot_product3 %f\n",rank, dot_product);
    if(rank==0)
    {
//	printf("%d rtr %f\n", rank,rtr);
        beta = rtr / rtrold;
    }
    MPI_Bcast(&beta,1, MPI_DOUBLE,0,MPI_COMM_WORLD);
    d = saxpy(beta,r,d,n,p);
 //   printf("saxpy done! %d %f a=%f b=%f\n", rank, relres, alpha, beta);
    if(rank==0)
    {
 //	printf("sqrt(rtr) %f\n", sqrt(rtr));
 //	printf("mormb %f\n", normb);
        relres = sqrt(rtr) / normb;
   //     printf("mormb %f\n", relres);
    }
    MPI_Bcast(&relres,1, MPI_DOUBLE,0,MPI_COMM_WORLD);
    
}
if(rank == 0){
    *norm=sqrt(rtr);
    }
/*for(i=0;i<n/p;i++)
	printf("xi= %f", x[i]);*/
return x;
    
}

double ddot(double* v, double* w, int n, int p)
{
    if(sizeof(v)!=sizeof(w))
        printf("the size of two vectors does not match each other!\n");
    double sum=0;
    int i;
    for(i=0;i<n/p;i++)
        sum+=v[i]*w[i];
    return sum;
}

double* saxpy(double a, double* v, double* w, int n, int p)
{
    double* answer;
    int i;
    answer = malloc(n/p * sizeof(double));
    for(i=0;i<n/p;i++){
        answer[i]=v[i]+a*w[i];
	}
    return answer;
}
/*double* matvec(double* v, int n)
{
    return v;
}*/

