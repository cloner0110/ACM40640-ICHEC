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
#include "ran2.c"

int main (){
	float randNumberx, randNumbery,final_pi;
	long seed;
	int nd = 0;
	//- number of random generations
	double randGenLim=100000;
	int i;
	for (i=0; i<randGenLim; i++){
		randNumberx = ran2(&seed);
		randNumbery = ran2(&seed);
		if ((pow((randNumberx-0.5),2)+pow((randNumbery-0.5),2)) < 0.25){
			nd+=1;
		}
	}
	final_pi=4*(nd/randGenLim);
	printf("Pi = %f \n",final_pi);	
	
return 0;
}

