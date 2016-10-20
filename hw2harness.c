/*
 *  *  hw2harness.c
 *   *  
 *    * I WILL OVERWRITE YOUR COPY OF THIS FILE WITH MY OWN. ANY CHANGES YOU MAKE WILL NOT BE VISIBLE DURING GRADING.
 *     */

#include <math.h>
#include <stdlib.h>
#include "hw2harness.h"
#include <math.h>

double cs240_getB(int index, int int_n) {
	int n = int_n;
	double k = floor(sqrt((double)int_n));
	int int_k = (int)k;
	double i = (double)index;
	double ret = 0;
	int rowi;
	
	if (index < int_k) {
		ret += (i+1)/(k+1);
	} else if (index > n - int_k - 1) {
		rowi = index % int_k;
		ret += (rowi + 1)/(k+1);
	}
	
	if ((index+1) % int_k == 0 && index != 0) {
		ret += 1;
	}
	
	
	return ret;
}

int cs240_verify(double* x, int k, double elapsedTime) {
	int i;
	int rowi;
	int correct = 1;
	double expected;
	double diff;
	int n = k*k;
	
	for (i = 0; i < n; i++) {
		if (isnan(x[i])) {
			correct = 0;
			break;
		}
		
		rowi = i % k;
		expected = ( (double)( rowi + 1 ) ) / ( (double)(k+1) );
		diff = x[i] - expected;
		if (diff < 0)
			diff = -diff;
			
		if (diff > 0.01) {
			correct = 0;
			break;
		}
	}
	
	
	
	return correct;
}
