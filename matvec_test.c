#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
double* matvec(double *v, int n, int rank, int p);

double* matvec(double *v, int n, int rank, int p)
{
    int k=sqrt(n);
    double *lower=malloc(k*sizeof(double)), *upper = malloc(k*sizeof(double)), *newv=malloc(n/p*sizeof(double));
    int i,r,s;
    MPI_Status status;
    
    if(rank != p-1)
    {
    for(i = n/p-k; i < n/p; i++)
    {
        lower[i - (n/p-k)] = v[i];
    }
    MPI_Send(lower, k, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD);
    }

   if(rank != 0){
        MPI_Recv(lower, k, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD, &status);//(k-1)'s lower, and located upper of k
        /*for (i = 0; i < k; i++)
           printf("loweri =%f",lower[i]);*/
    }

    
    if(rank !=0)
    {
    for(i = 0; i < k; i++)
    {
        upper[i] = v[i];
    }
    MPI_Send(upper, k, MPI_DOUBLE, rank-1, 0, MPI_COMM_WORLD);
    }

    if(rank != p-1)
    MPI_Recv(upper, k, MPI_DOUBLE, rank+1, 0, MPI_COMM_WORLD, &status);

    for(r=0;r<k/p;r++)
    {
        for(s=0;s<k;s++)
        {
            i=r*k+s;
            newv[i]=4*v[i];
            
            if(s != 0)
                newv[i] -= v[i-1];
            if(s != k-1)
                newv[i] -= v[i+1];
            
            if(r != 0)
                newv[i] -= v[i-k];
            else if (rank !=0 && r == 0)
                newv[i] -= lower[i];
            
            if(r != k/p-1)
                newv[i] -= v[i+k];
            else if (rank !=p-1 && r == k/p-1)
                newv[i] -= upper[i-(n/p-k)];
        

        }
    }
    return newv;
}

