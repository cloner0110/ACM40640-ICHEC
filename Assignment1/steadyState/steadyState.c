//----------------------------------------------------------------------
// Steady State updating the W field
// Author : Amirhossein Taran - March 2024
//
// Email : amirhossein.taran@ucdconnect.ie
// 	Build with:	
// 		gcc -o steadyState steadyState.c -lm
// 	Run with:
// 		./steadyState
//----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <stdbool.h>

int main (){
	float w_mat[100][100], w_mat_old[100][100];
	int i,j;
	for (i=0 ; i<100 ; i++){
		for(j=0 ; j<100 ; j++){
			//- First row
			if (i==0 && j>0 && j<99){
				w_mat[i][j]=0.0;
			}
			//- Last row
			else if (i==99){
				w_mat[i][j]=100.0;
			}
			//- First column
			else if (j==0){
				w_mat[i][j]=100.0;
			}
			//- Last column
			else if (j==99){
				w_mat[i][j]=100.0;
			}
			//- Internal Values
			else{
				w_mat[i][j]=75.0;
			}
		}	
	}
	int iter=0;
	int maxErr = -1;
	bool cond = true;
	while (cond) {
		iter+=1;
		for (i=0 ; i<100 ; i++){
			for (j=0 ; j<100 ; j++){
				w_mat_old[i][j] = w_mat[i][j];
			}
		}
		for (i=1 ; i<99 ; i++){
			for (j=1 ; j<99 ; j++){
				w_mat[i][j] = (w_mat_old[i+1][j] + w_mat_old[i-1][j] + w_mat_old[i][j+1] + w_mat_old[i][j-1])/4.0;
			}	
		}
	for (i=0 ; i<100 ; i++){
		for (j=0 ; j<100 ; j++){
			maxErr = fmax(maxErr , (w_mat[i][j]-w_mat_old[i][j]));
		}
	}
	printf("iteration = %d ,Error = %f \n", iter, maxErr);
	if (maxErr < 0.0001){
		cond = false;
	}
	}
return 0;
}
