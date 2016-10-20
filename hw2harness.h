/*
 *  *  hw2harness.h
 *   *  
 *    * I WILL OVERWRITE YOUR COPY OF THIS FILE WITH MY OWN. ANY CHANGES YOU MAKE WILL NOT BE VISIBLE DURING GRADING.
 *     */
#ifndef HW2HARNESS_H
#define HW2HARNESS_H

/*
 * PROBLEM NOTES:
 * The problem that these functions generate/verify is a linear gradient with 0 on the left side and 1 on the right side. The returned b
 * follows this gradient.
 *
 * Note that this problem doesn't converge very quickly, and in fact you may need to change your upper iteration limit in your CG solver.
 * Instead of limiting to sqrt(n), try 5*sqrt(n).
 *
 * For larger problems you may also run into accuracy limits. If you want to verify correctly on large problems (k > 400 or so), then reduce
 * the norm of the residual requirement by an order of magnitude or two (add two '0' after the decimal point). 
 *
 * I will actually check your residual during grading, but this problem is nice because it has a nice closed form solution.
 * */

/*
 * Returns a single element of vector b. You may call this on any node you wish, in whatever order.
 * You should call this function n times.
 *
 * arguments:
 * index - the index of the element you want. Zero based (note that Matlab is 1 based). In other words, the first element has index 0, the second index 1, etc.
 * int_k - k
 *
 * returns:
 * b[index]
 * */
double cs240_getB(int index, int int_n);

/*
 * Verifies the correctness of the CG computed on the model problem with b as returned by cs240_getB().
 * cs240_verify() should be called once at the end of the program.
 *
 * arguments:
 * x - the entire vector x. If this doesn't exist on one processor because of your data layout, then you have to assemble it on one processor.
 * k - k
 * elapsedTime - The difference in time as given by MPI_Wtime() (on processor 0) between the start of CG calculation and the end.
 * */
int cs240_verify(double* x, int k, double elapsedTime);

#endif
