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
#include <omp.h>

int main ()
{
	float w_mat[100][100], w_mat_new[100][100];
	int i,j;
	//#pragma omp parallel for 
	for (i=0 ; i<100 ; i++){
		for(j=0 ; j<100 ; j++){
			//- First row
			if (i==0 && j>0 && j<99)
			{
				w_mat[i][j]=0.0;
			}
			//- Last row
			else if (i==99)
			{
				w_mat[i][j]=100.0;
			}
			//- First column
			else if (j==0)
			{
				w_mat[i][j]=100.0;
			}
			//- Last column
			else if (j==99)
			{
				w_mat[i][j]=100.0;
			}
			//- Internal Values
			else
			{
				w_mat[i][j]=75.0;
			}
		}	
	}
	for (i=0 ; i<100 ; i++)
	{
			for (j=0 ; j<100 ; j++)
			{
				w_mat_new[i][j] = w_mat[i][j];
			}
	}

	const char *filename1 = "before.dat";
	FILE *file1 = fopen(filename1, "w");
    
    if (file1 == NULL) {
        printf("Error opening file %s\n", filename1);
        return;
    }

    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            fprintf(file1, "%9.6f ", w_mat_new[i][j]);
        }
        fprintf(file1, "\n");
    }

	int iter = 0;
	float conv_tolerance = 0.0001;
	while (1) 
	{
		iter+=1;
		for (i=1 ; i<99 ; i++)
		{
			for (j=1 ; j<99 ; j++)
			{
				w_mat_new[i][j] = (w_mat[i+1][j] + w_mat[i-1][j] + w_mat[i][j+1] + w_mat[i][j-1])/4.0;
			}	
		}
		
	float maxErr = -1.0;
	for (i=0 ; i<100 ; i++)
	{
		for (j=0 ; j<100 ; j++)
		{
			float temp = fabs(w_mat_new[i][j]-w_mat[i][j]);
			maxErr = fmax(maxErr ,temp);
		}
	}
	printf("iteration = %d ,Error = %f \n", iter, maxErr);
	if (maxErr < conv_tolerance)
	{
		break;
	}
	for (i=0 ; i<100 ; i++)
	{
			for (j=0 ; j<100 ; j++)
			{
				w_mat[i][j] = w_mat_new[i][j];
			}
	}
	}
	const char *filename2 = "after.dat";
	FILE *file2 = fopen(filename2, "w");
    
    if (file2 == NULL) {
        printf("Error opening file %s\n", filename2);
        return;
    }

    for (i = 0; i < 100; i++) {
        for (j = 0; j < 100; j++) {
            fprintf(file2, "%9.6f ", w_mat_new[i][j]);
        }
        fprintf(file2, "\n");
    }
	return 0;
}
