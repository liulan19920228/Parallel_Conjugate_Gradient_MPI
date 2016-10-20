/* UCSB CS240A, Fall Quarter 2016
 *  * Main and supporting functions for the Conjugate Gradient Solver on a 5-point stencil
 *   *
 *    * NAMES: Lan Liu ; Pritha Doddahosahally Narayanappa;
 *     * PERMS: 7956394 ; 9727371
 *      * DATE: 10/17/2016
 *       */
#include"mpi.h"
//#include "hw2harness.c"
#include <stdio.h>
#include <stdlib.h>
#include "cgsolve.c"
double* load_vec( char* filename, int* k );
void save_vec( int k, double* x );



int main( int argc, char* argv[] ) {
    int writeOutX = 0;
    int n, k, p, rank, i;
    int maxiterations = 10000;
    int niters=0;
    double norm=1;
    double* b;
    double* x;
    double time;
    double t1, t2;
    double* x_initial;    

    MPI_Init( &argc, &argv );
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 

    
    if ( argc == 3 ) {
        k = atoi( argv[1] );
        n = k*k;
        b  = malloc(n/p * sizeof(double));
        for(i=rank*n/p;i < (rank+1)*n/p;i++){
            b[i-rank*n/p] = cs240_getB(i,n);
	}
    } else if  ( !strcmp( argv[1], "-i" ) && argc == 4 ) {
        b = load_vec( argv[2], &k );
    } else {
        printf( "\nCGSOLVE Usage: \n\t"
               "Model Problem:\tmpirun -np [number_procs] cgsolve [k] [output_1=y_0=n]\n\t"
               "Custom Input:\tmpirun -np [number_procs] cgsolve -i [input_filename] [output_1=y_0=n]\n\n");
        exit(0);
    }
    writeOutX = atoi( argv[argc-1] ); // Write X to file if true, do not write if unspecified.
   
    t1 = MPI_Wtime();
    x = cgsolve(p, n, rank, b, &niters, maxiterations, &norm); 
    x_initial=malloc(n* sizeof(double));
    MPI_Gather(x,n/p,MPI_DOUBLE, x_initial, n/p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    x=x_initial;
    t2 = MPI_Wtime();

   if(niters>0 && rank ==0){
        printf( "Norm of the residual after %d iterations: %lf\n",niters,norm);
    }
   if ( writeOutX ) {
        save_vec( k, x );
    }

    if(rank == 0){printf( "Problem size (k): %d\n",k);}
    if(niters>0 && rank ==0){
        printf( "Norm of the residual after %d iterations: %lf\n",niters,norm);
    }
    printf( "Elapsed time during CGSOLVE: %lf\n", t2-t1);
    if(rank ==0)
	    printf( "result %d \n",cs240_verify(x_initial, k, t2-t1));
    
    if(niters > 0){
        free(b);
    }
    if(niters > 0){
        free(x);
    }
    
    MPI_Finalize();
    
    return 0;
}


double* load_vec( char* filename, int* k ) {
    FILE* iFile = fopen(filename, "r");
    int nScan;
    int nTotal = 0;
    int n;
    
    if ( iFile == NULL ) {
        printf("Error reading file.\n");
        exit(0);
    }
    
    nScan = fscanf( iFile, "k=%d\n", k );
    if ( nScan != 1 ) {
        printf("Error reading dimensions.\n");
        exit(0);
    }
    
    n = (*k)*(*k);
    double* vec = (double *)malloc( n * sizeof(double) );
    
    do {
        nScan = fscanf( iFile, "%lf", &vec[nTotal++] );
    } while ( nScan >= 0 );
    
    if ( nTotal != n+1 ) {
        printf("Incorrect number of values scanned n=%d, nTotal=%d.\n",n,nTotal);
        exit(0);
    }
    
    return vec;
}

void save_vec( int k, double* x ) {
    FILE* oFile;
    int i;
    oFile = fopen("xApprox.txt","w");
    
    fprintf( oFile, "k=%d\n", k );
    
    for (i = 0; i < k*k; i++) { 
        fprintf( oFile, "%lf\n", x[i]);
    } 
    
    fclose( oFile );
}
