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
	//- TODO: add definition for err, by subtraction and abs of the difference	
	while (err > 0.0001) {
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
	}
return 0;
}
