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
    //- x,y random numbers, estimated pi
    float randNumberx, randNumbery, final_pi; 
    
    //- loop counter
    int i;
    
    //- Total sample size
    int sampleSize = 100000;
    
    //- samples hit the circle
    int nd = 0;
    
    //- access to time for measurements
    double time;
    time = omp_get_wtime();
    
    //- seed for ran2()
    long seed;

    //-setting number of threads
    omp_set_num_threads(4);
    
    //- starting parallel region
    #pragma omp parallel for private (i,randNumberx, randNumbery, seed) reduction(+:nd)
    for (i=0; i<sampleSize; i++){
    //- setting seed based on loop counter
    seed = i;
    randNumberx = ran2(&seed);
    randNumbery = ran2(&seed);

    //- if condition for counting hits => nd
    if ((pow((randNumberx-0.5),2)+pow((randNumbery-0.5),2)) < 0.25){
        nd++;
    }
    }   
final_pi=4.0*((double)nd/(double)sampleSize);
printf("Pi = %1.16f \n",final_pi);	    
printf("Time: %1.6f \n",omp_get_wtime()-time);
return 0;
}

