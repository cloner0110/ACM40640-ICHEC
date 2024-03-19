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
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

struct matrix {
    int nrow;
    int ncol;
    float *matrix;
};

int getindex(struct matrix m, int i, int j);

int main(void) {
    int i,j,k,u,nthreads;
    int M = 100 ;
    int N = 100 ;
    unsigned iter;
    struct matrix w_mat, w_mat_new;
    double conv_tolerance = 1E-04;
    double startTime, endTime;
    double err,max_err;
    w_mat.nrow = M;
    w_mat.ncol = N;
    w_mat_new.nrow = M;
    w_mat_new.ncol = N;
    for (u=0; u<5; u++)
    {
        for (nthreads=1 ; nthreads <=4; nthreads +=1)
            {
                if (nthreads ==10)
                    {
                        nthreads = 16;
                    }
        omp_set_num_threads(nthreads);
        
/* Assign space */
    w_mat.matrix = (float *)malloc(w_mat.nrow * w_mat.ncol*sizeof(float));
    w_mat_new.matrix = (float *)malloc(w_mat_new.nrow * w_mat_new.ncol*sizeof(float));

    startTime = omp_get_wtime();
#pragma omp parallel default(shared) private(i,j,err)
{
/* Initialization */
    #pragma omp for schedule (static) collapse(2)
        for (i=0 ; i<M ; i++)
            for (j=0 ; j<N ; j++)
                {
                    w_mat.matrix[getindex(w_mat,i,j)]=75.0;
                    w_mat_new.matrix[getindex(w_mat_new,i,j)]=75.0;
                }
        //- First row
        #pragma omp for nowait schedule(static)
        for (j=1; j<N-1; j++)
        {
            w_mat.matrix[getindex(w_mat,0,j)] = 0.0;
            w_mat_new.matrix[getindex(w_mat_new,0,j)] = 0.0;
        }
        //- Last row
        #pragma omp for nowait schedule(static)
        for (j=0 ; j<N ; j++)
        {
            w_mat.matrix[getindex(w_mat,N-1,j)]=100.0;
            w_mat_new.matrix[getindex(w_mat_new,N-1,j)]=100.0;
        }
        //- First column
        #pragma omp for nowait schedule(static)
        for (i=0 ; i<M ; i++)
        {
            w_mat.matrix[getindex(w_mat,i,0)]=100.0;
            w_mat_new.matrix[getindex(w_mat_new,i,0)]=100.0;
        }
        //- Last column
        #pragma omp for nowait schedule(static)
        for (i=0 ; i<M ; i++)
        {
            w_mat.matrix[getindex(w_mat,i,N-1)]=100.0;
            w_mat_new.matrix[getindex(w_mat_new,i,N-1)]=100.0;
        }
        //- Internal Values

iter = 0;
max_err = DBL_MAX;
//- iteration (solution) loop
while(max_err > conv_tolerance)
{
        ++iter;
    #pragma omp for schedule(static) collapse(2)
    for (i=1 ; i<M-1 ; i++)
		{
			for (j=1 ; j<N-1 ; j++)
			{
				w_mat_new.matrix[getindex(w_mat_new,i,j)] =
                        0.25*
                        (
                        w_mat.matrix[getindex(w_mat,i+1,j)] +
                        w_mat.matrix[getindex(w_mat,i-1,j)] +
                        w_mat.matrix[getindex(w_mat,i,j+1)] +
                        w_mat.matrix[getindex(w_mat,i,j-1)]
                        );
			}
		}
        max_err = 0.0;
    #pragma omp for schedule(static) collapse(2)
    for (i=1 ; i<M-1 ; i++)
		{
			for (j=1 ; j<N-1 ; j++)
			{
                err =fabs(
                     w_mat_new.matrix[getindex(w_mat_new,i,j)] -
                    w_mat.matrix[getindex(w_mat,i,j)]
                );
                if (err > max_err)
                {
                    max_err = err;
                }
                w_mat.matrix[getindex(w_mat,i,j)] =
                w_mat_new.matrix[getindex(w_mat_new,i,j)] ;
            }
        }
}
}
double endTime = omp_get_wtime() - startTime;

printf("Run Time (s) = %4.3lf for %d threads\n", endTime,nthreads);
}
		printf("\n");

}


/* /\* Write to a file *\/ */
//const char *filename2 = "result.dat"; 
//FILE *file2 = fopen(filename2, "w"); 
//
//     if (file2 == NULL) {
//         printf("Error opening file %s\n", filename2); 
//         return 0; 
//     } 
//
//     for (i = 0; i < M; i++) { 
//         for (j = 0; j < N; j++) { 
//             fprintf(file2, "%9.6f ", w_mat_new.matrix[getindex(w_mat,i,j)]); 
//         } 
//         fprintf(file2, "\n"); 
//     } 
 } 


int getindex(struct matrix m, int i, int j){

    return m.ncol*i + j;
}
