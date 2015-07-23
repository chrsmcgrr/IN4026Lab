#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <math.h>

//#define DEBUG

int read_input(int* A, int n, char* file);
int write_output(int* P, int* S, int n);
int minArray(int *A, int n, int i);
int outputCheck(int *P, int *S, char* pfile, char* sfile, int n);
void psMin(int *A, int *P, int *S, int n);
void randArray(int *A, int n);
int write_Array(int* A, int n);
void generateArrays(void);

int main(int argc, char **argv)
{
	clock_t start, end;
	double cpu_time_used;
	
	int status;
	int n;
	int* A;
	int* P;
	int* S;

	if(argc < 5){
		printf("Missing Arguement Parameters\n");
		printf("Format ./seq file_path array_size P_ans_path S_ans_Path\n");
		return 1;
	}

	n = atoi(argv[2]);
	A = malloc(n*sizeof(int));
	P = malloc(n*sizeof(int));
	S = malloc(n*sizeof(int));

	if(A==NULL){
		printf("Failed to Allocate Memory for Input Array");	
	}

	status = read_input(A, n, argv[1]);

	if(status){
		#ifdef DEBUG	
		printf("Failed to Read Input \n");
		#endif
		return 1;
	}
	//Start of the Algorithm
/*
	int j;
	double average;
	for(j=0; j<50000; j++){
		start = clock();
		//psMin(A, P, S, n);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		if(j==0) average = cpu_time_used;
		else	 average = (average+cpu_time_used)/2;
	}	
	//End of Algorithm
*/
	int temp = minArray(A, 24, 0);
	printf("The Min of A is %d \n", temp);

//	printf("Average Computation Time %fs for an input size of %d \n",average, n);
	if((atoi(argv[3])!=1) && (atoi(argv[4])!=1))
	{
		status = outputCheck(P, S, argv[3], argv[4], n);
		if(status){
			printf("Incorrect Answer\n");
		}
		else{
			printf("Correct Answer\n");
		}
		status = write_output(P, S, n);

		if(status){
			#ifdef DEBUG	
			printf("Failed to Write Output \n");
			#endif
			return 1;
		}
	}



	free(P);
	free(S);
	free(A);
	
	//Used to generate random data sets and save to disk as text files
//	generateArrays();


    	return 0;
}
void generateArrays(){
	int i;	
	for(i=16; i<=48; i++){
		int *A;
		A = malloc(i*sizeof(int));
		randArray(A, i);
		write_Array(A,i);
		free(A);
	}
}
int write_Array(int* A, int n){

	FILE *output;
	char name[16];
	snprintf(name, sizeof(name), "Input_Data/input_%d.txt", n);
	output = fopen(name, "w");
	if(output==NULL){
		#ifdef DEBUG	
		printf("Failed to create the Output File \n");
		#endif
		return 1;
	}
	fprintf(output, "P = {\n");
	
	int i;
	for(i=0; i<n; i++){
		fprintf(output, "%d,\n", A[i]);
	}
	fprintf(output, "};");
	fclose(output);

	return 0;
}
void randArray(int *A, int n){
	if(A==NULL){
		printf("Failed to allocate RandArray\n");	
	}
	int i;	
	for(i=0; i<n; i++){
		A[i] = rand() % 256;
	}
}
void psMin(int *A, int *P, int *S, int n){
	int i;
	for(i=0; i<n; i++){		
		P[i] = minArray(A, i+1, 0);
		S[i] = minArray(A, n, i);
	}
}

int minArray(int *A, int n, int i){
	int j, l;
	int k;
	k = n-i;
	int *B = malloc(k*sizeof(int));
	int min, m, p;
	min = INT_MAX;

	m = ceil(log2(k));
	
	if(k==1){
		return A[i];
	}
	if((k%2) == 1){
		min = minArray(A,n-1,i);
	}
	for(j=1;j<=m;j++){
		printf("Main Loop J = %d  of %d\n", j,m);
		if(j==1){
			for(l=0; l<(k/2); l++){
				p = 2*l;
				if(A[p+i]>A[p+1+i]) B[l] = A[p+1+i];
				else B[l] = A[p+i];
				printf("	Inner Loop l = %d of %d\n", l,(k/2));
				printf("	Value of A[%d] = %d and Value of A[%d] = %d \n",p+i,A[p+i],p+i+1,A[p+1+i]);
				printf("	Value of B[%d] = %d \n\n", l, B[l]);
			}
			if(((k/2)%2) == 1){
				if(min > B[k/2]){
					min = B[k/2];
				}
			}
		}
		else{
			for(l=0; l<(k>>j);l++){
				p = 2*l;
				printf("	Inner Loop l = %d of %d\n", l,(k>>j));
				printf("	Value of B[%d] = %d and Value of B[%d] = %d \n",p,B[p],p+1,B[p+1]);
				if(B[p]>B[p+1]) B[l] = B[p+1];
				else B[l] = B[p];
				printf("	Value of B[%d] = %d \n\n", l, B[l]);
				if(l==(k>>j-1)){
					if(((k>>j)%2) == 1){
						if(min > B[k>>j]){
							min = B[k>>j];
						}
					}
				}
			}
		}
	}
	if( min > B[0]) min = B[0];
	free(B);
	return min;
}
int outputCheck(int *P, int *S, char* pfile, char* sfile, int n){

	int *P_ans = malloc(n*sizeof(int));
	int *S_ans = malloc(n*sizeof(int));

	int correct = 0;

	read_input(P_ans, n, pfile);
	read_input(S_ans, n, sfile);
	
	int i;
	for(i=0; i<n; i++){
		if(P[i] != P_ans[i]){
			correct=1;		
		}	
	}
	for(i=0; i<n; i++){
		if(S[i] != S_ans[i]){
			correct=1;		
		}	
	}

	free(P_ans);
	free(S_ans);
	
	return correct;
}
int read_input(int* A, int n, char* file) {

	FILE *input;
	char buf[20];

	input =fopen(file,"r");

	if (!input){
		#ifdef DEBUG	
		printf("Error File Does not Exist. \n");
		#endif
		return 1;
	}

	if(fgets(buf,20, input)!=NULL){
		#ifdef DEBUG	
		printf("%s",buf);
		#endif	
	}
	else{
		#ifdef DEBUG	
		printf("First Line could not be read \n");
		#endif
		return 1;	
	}
	int i;
	for(i = 0; i<n; i++){
		if(fgets(buf,20, input)!=NULL){
			A[i] = atoi(buf);
			A[i] = A[i] - atoi(",");
			#ifdef DEBUG	
			printf("%d \n",A[i]);
			#endif	
		}
	}

	fclose(input);
	return 0;
}
int write_output(int* P, int* S, int n){

	FILE *output;
	output = fopen("results.txt", "w");
	if(output==NULL){
		#ifdef DEBUG	
		printf("Failed to create the Output File \n");
		#endif
		return 1;
	}
	fprintf(output, "P = {");
	
	int i;
	for(i=0; i<n; i++){
		if(i==n-1){
			fprintf(output, "%d", P[i]);		
		}
		else{
			fprintf(output, "%d, ", P[i]);
		}
		if((i%16 == 0) && (i!=0)){
			fprintf(output, "\n");
		}
		
	}
	fprintf(output, "} \n");

	fprintf(output, "S = {");
	for(i=0; i<n; i++){
		if(i==n-1){
			fprintf(output, "%d", S[i]);		
		}
		else{
			fprintf(output, "%d, ", S[i]);
		}
		if((i%16 == 0) && (i!=0)){
			fprintf(output, "\n");
		}
		
	}
	fprintf(output, "} \n");
	fclose(output);

	return 0;
}
