/******************************************************************
*	IN4026 Lab C: List Ranking (Pointer Jumping)
*	Author: Christopher McGirr
*	Student # 4415302
*
*	File: par_omp.c
*	Description: The OpenMP parallel version of the linked list 
*	distance calculation. Takes an input array S of size n
*	and attempts to find the distance of each node in S to 
*	the final node 0. The solution is then saved in the output
*	array R of size n.
*
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <math.h>
#include "fileIO.h"
#include <omp.h>

#define CHUNKSIZE 4

void nodeLength(int* S, int* R, int n);

int MAX_THREADS;


/****************************************************************
*
*	Function: main
*	Input:	int argc 	number of command line arguements
*		char **arg	pointer to those arguements
*
*	Output: int 0 for success and 1 for error
*
*	Description: Runs the simple merge algorithm multiple
*	times averaging the results and printing them to terminal. 
*
*****************************************************************/
int main(int argc, char **argv)
{
	double start, end;
	double cpu_time_used;
	
	char name[8] = "omp/";

	int status;
	int n;
	int* S;
	int* R;
	
	int RUNS;
	
	//Check if app was given enough input
	if(argc < 6){
		printf("Missing Arguement Parameters\n");
		printf("Format ./seq path_input input_size ans_Path RUNS MAX_THREADS\n");
		return 1;
	}
	
	//Save args to memory and allocate memory for arrays
	n = atoi(argv[2]);
	RUNS = atoi(argv[4]);
	MAX_THREADS = atoi(argv[5]);
	S = malloc(n*sizeof(int));
	R = malloc(n*sizeof(int));


	if(S==NULL){
		printf("Failed to Allocate Memory for Input Array S");	
	}
	if(R==NULL){
		printf("Failed to Allocate Memory for Input Array R");	
	}

	//Read the input array from file and save to memory
	status = read_input(S, n, argv[1]);

	if(status){
		#ifdef DEBUG	
		printf("Failed to Read Input S\n");
		#endif
		return 1;
	}
	
	
	//Start of testing of the algorithm
	int j;
	double average;
	for(j=0; j<RUNS; j++){
		memset(R, 0, n*sizeof(int));
		start = omp_get_wtime(); //start timer
		nodeLength(S, R, n);
		end = omp_get_wtime(); //end timer
		cpu_time_used = end - start;
		average += cpu_time_used;
		
	}
	average = average/RUNS; //Average the execution times

	//print results to terminal
	printf("%d 	%f	s \n",n,average);

	if(atoi(argv[3])!=1)
	{
		status = outputCheck(R, argv[3], n);
		if(status){
			printf("Incorrect Answer\n");
		}
		else{
			printf("Correct Answer\n");
		}
	}
	

	status = write_output(S, R, n, name);

	if(status){	
		printf("Failed to Write Output \n");
		return 1;
	}

	free(S);
	free(R);	
	
    	return 0;
}
/****************************************************************
*
*	Function: nodeLength
*	Input:	int *S	Pointer to Input Array S
*		int *R	Pointer to Output Array R
*		int n   Size of the Arrays S and R
*
*	Output: returns nothing
*
*	Description: Takes a linked list array S and finds the
*	distance of each node in S to the final node 0. The 
*	distance is saved in the output array R. 
*
*****************************************************************/
void nodeLength(int* S, int* R, int n){
	int *P = malloc(n*sizeof(int));	
	int i;
	int chunk = CHUNKSIZE;

	omp_set_dynamic(0); //Makes sures the number of threads available is fixed    
	omp_set_num_threads(MAX_THREADS); //Set thread number

	/*Copy Contents into working Array*/
	#pragma omp parrallel shared(S) private(i) for schedule(dynamic, chunk) nowait
	for(i=0; i<n; i++){
		P[i] = S[i];
		if(S[i] > 0){
			R[i] = 1;
		}
	}
	/*Process each node sequential*/
	#pragma omp parrallel shared(S, R, P) private(i) for schedule(dynamic, chunk)
	for(i=0; i<n; i++){
		while(P[i] > 0){	
			R[i] = R[i]+R[P[i]];
			P[i] = P[P[i]];
		}	
	}

	free(P);
}

