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
    int nSamples = 5;
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
    double *times = (double *) malloc(nSamples*sizeof(double));
    double tmin=DBL_MAX;
    double tmax=0.0;
        for (u=1; u<=10; u++){
        /* Assign space */
        w_mat.matrix = (float *)malloc(w_mat.nrow * w_mat.ncol*sizeof(float));
        w_mat_new.matrix = (float *)malloc(w_mat_new.nrow * w_mat_new.ncol*sizeof(float));
        times[u-1] = omp_get_wtime();
            /* Initialization */
            for (i=0 ; i<M ; i++)
                for (j=0 ; j<N ; j++)
                    {
                        w_mat.matrix[getindex(w_mat,i,j)]=75.0;
                        w_mat_new.matrix[getindex(w_mat_new,i,j)]=75.0;
                    }
            //- First row
            for (j=1; j<N-1; j++)
            {
                w_mat.matrix[getindex(w_mat,0,j)] = 0.0;
                w_mat_new.matrix[getindex(w_mat_new,0,j)] = 0.0;
            }
            //- Last row
            for (j=0 ; j<N ; j++)
            {
                w_mat.matrix[getindex(w_mat,N-1,j)]=100.0;
                w_mat_new.matrix[getindex(w_mat_new,N-1,j)]=100.0;
            }
            //- First column
            for (i=0 ; i<M ; i++)
            {
                w_mat.matrix[getindex(w_mat,i,0)]=100.0;
                w_mat_new.matrix[getindex(w_mat_new,i,0)]=100.0;
            }
            //- Last column
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
                for (i=1 ; i<M-1 ; i++){
                    for (j=1 ; j<N-1 ; j++){
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
                for (i=1 ; i<M-1 ; i++){
                    for (j=1 ; j<N-1 ; j++){
                        err =fabs(
                            w_mat_new.matrix[getindex(w_mat_new,i,j)] -
                            w_mat.matrix[getindex(w_mat,i,j)]
                        );
                    if (err > max_err){
                        max_err = err;
                    }
                    w_mat.matrix[getindex(w_mat,i,j)] =
                    w_mat_new.matrix[getindex(w_mat_new,i,j)] ;
                    }
                }
            }
        times[u-1] = omp_get_wtime()-times[u-1];    
        }
    double timeAvg = 0.0;
    for(i=0; i<nSamples; ++i) {
        timeAvg+=times[i];
    }
    timeAvg/=nSamples;
    double sig=0.0;
    for(i=0; i<nSamples; ++i) {
    sig+=(times[i]-timeAvg)*(times[i]-timeAvg);
    }
    sig=sqrt(sig/nSamples);
    printf("\nSummary (Serial version):\n");
    printf("|  Avg. Time (s) | sigma Time(s)\n");
    printf("%16.8f %16.8f\n",timeAvg,sig);
}


int getindex(struct matrix m, int i, int j){

    return m.ncol*i + j;
}
