//----------------------------------------------------------------------
// Steady State updating the W field
// Author : Amirhossein Taran - March 2024
//
// Email : amirhossein.taran@ucdconnect.ie
// 	Build with:	
// 		gcc -o steadyState steadyState.c -lm -fopenmp
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

    //- loop variables and thread numbers
    int i,j,k,u,nthreads;

    //- number of samples for parallel run
    int nSamples = 5;
 
   //- Grid size M*N
    int M = 100 ;
    int N = 100 ;

    //- iteration counter
    unsigned iter;

    //- convergance criteria
    double conv_tolerance = 1E-04;

    //- variables to meausre the time
    double startTime, endTime;
    double *times = (double *) malloc(nSamples*sizeof(double));
    double tmin=DBL_MAX;
    double tmax=0.0;

    //- Error for each iteration
    double err,max_err;

    //- declaring W Matrix and W_new Matrix
    struct matrix w_mat, w_mat_new;
    w_mat.nrow = M;
    w_mat.ncol = N;
    w_mat_new.nrow = M;
    w_mat_new.ncol = N;

    //- loop for changing thread number on each one
    for (nthreads=1 ; nthreads <=4; nthreads +=1){
    
        omp_set_num_threads(nthreads);
        for (u=1; u<=10; u++){
        /* Assign space */
        w_mat.matrix = (float *)malloc(w_mat.nrow * w_mat.ncol*sizeof(float));
        w_mat_new.matrix = (float *)malloc(w_mat_new.nrow * w_mat_new.ncol*sizeof(float));
        
        //- start of time measurements
        times[u-1] = omp_get_wtime();
        #pragma omp parallel default(shared) private(i,j,err)
        {
            /* Initializing W matrix */
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
    
            //- setting initial value to iteration counter and maximum error between W_new and W
            iter = 0;
            max_err = DBL_MAX;
            //- iteration (solution) loop; while not converged => iterate
            while(max_err > conv_tolerance)
            {
                ++iter;
                #pragma omp for schedule(static) collapse(2)
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
                //- Calculating difference between W_new and W
                max_err = 0.0;
                #pragma omp for schedule(static) collapse(2)
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
        }
        //- time measurement
        times[u-1] = omp_get_wtime()-times[u-1];    
        }
    //- Averaging time on samples, taken from practical 2
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
    printf("\nSummary:\n");
    printf("Number of Threads = %d \n",nthreads);
    printf("|  Avg. Time (s) | sigma Time(s)\n");
    printf("%16.8f %16.8f\n",timeAvg,sig);
    }
free(times);
free(w_mat.matrix);
free(w_mat_new.matrix);
}

//- function for fixing the index of a Matrix, taken from practical 2
int getindex(struct matrix m, int i, int j){

    return m.ncol*i + j;
}
