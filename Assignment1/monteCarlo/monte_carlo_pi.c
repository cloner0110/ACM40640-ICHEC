//----------------------------------------------------------------------
// Estimating pi using Monte-Carlo method
//
// Author : Amirhossein Taran - March 2024
//
// Email : amirhossein.taran@ucdconnect.ie
// 	Build with:	
// 		gcc -o monte_carlo_pi_serial monte_carlo_pi.c -lm
// 	Run with:
// 		./monte_carlo_pi_serial
//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <omp.h>
#include "ran2.c"
int main (){
    float randNumberx, randNumbery, final_pi;
    //- number of random generations
    int i,tid;
    int randGenLim = 100000;
    int nd = 0;
	double time;
    long seed;
	time = omp_get_wtime();
    omp_set_num_threads(4);
	#pragma omp parallel for private (i,randNumberx, randNumbery, seed) reduction(+:nd)
	for (i=0; i<randGenLim; i++){
        seed = i;
		randNumberx = ran2(&seed);
		randNumbery = ran2(&seed);
	 	if ((pow((randNumberx-0.5),2)+pow((randNumbery-0.5),2)) < 0.25){
	 		nd++;
	 }
	}
	final_pi=4.0*( (double)nd/(double)randGenLim);
	printf("Pi = %20.16f \n",final_pi);	
	printf("Time: %f \n",omp_get_wtime()-time);
return 0;
	}

