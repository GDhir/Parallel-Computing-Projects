#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string>
#include <ctime>

using namespace std;

bool debug;

void printMatrix( double *A, int rows, int cols){
	for(int i = 0 ; i < rows; i++){
		for(int j = 0; j < cols; j++){
			printf("%.2f /", A[i * cols + j]);
		}
		printf("\n");
	}
}

double get2ndMin( double vals[] ){
	double MIN = std::min(vals[0], std::min(vals[1], std::min(vals[2], vals[3])));
	double temp[3];
	int index = 0;
	for(int i = 0; i< 4; i++){
		if(vals[i] != MIN){
			temp[index] = vals[i]; 
			index++;
		}
	}
	double result;
	if( index != 3){
		result = MIN;
	}
	else{
		result = std::min(temp[0], std::min(temp[1], temp[2]));
	}
	return result;
}


void initialize(double *A, int n) 
{	
	for(int i = 0; i<n; i++){
		for(int j = 0; j<n; j++){
			A[ i*n + j ] =  pow( (1.0 + cos( (double) 2*i ) + sin( (double) j )), 2.0);
		}
	}
}


void iterate(double *A, double *A0, int n,  int t) 
{	
	std::clock_t start, end;
    start = std::clock();

	for(int step = 0 ; step < t; step++){
		for(int i = 0 ;i < n; i++){
			for(int j = 0; j < n; j++){
				if( (i == 0) || (i == n - 1) || (j == 0) || (j == n - 1) ){
					A[i * n + j] = A0[ i * n + j];
				}
				else{
					double br, bl, tr, tl;
					br = A0[(i+1)*n + (j+1)];
					bl = A0[(i+1)*n + (j-1)];
					tr = A0[(i-1)*n + (j+1)];
					tl = A0[(i-1)*n + (j-1)];
				
					double vals[] = {br, bl, tr, tl};
					double SecondMin = get2ndMin(vals);
					A[i * n + j] = A0[i * n + j] + SecondMin;
					
					//Debugging Code
					/*
					if(i == 31 && j == 25){		
						printf("Iter #: %d\n", step);
						printf("i:%d, j:%d\n", i, j);
						printf("2nd Min: %f\n", SecondMin);
						//printf("Candidates: %f %f %f %f\n", br, bl, tr, tl);
						printf("A0[i][j]: %f\n", A0[i*n+j]);
						printf("A[i][j]: %f\n\n", A[i*n+j]);
					}
					*/
				}
		
			}
		}
		if(step < t-1){
			double *temp  = A;
			A = A0;
			A0 = temp;
		}
	}

	//Debugging Code
	if(debug){
		printMatrix(A,n,n);
	}

	double v1,v2,v3;
	v1 = 0.0;
	for(int i = 0 ; i < n*n ; i++){
		v1 += A[i];
	}
	v2 = A[ (int) ( n * floor(n/2.0) + floor(n/2.0) )  ];
	v3 = A[n * 37 + 47];

	A[0] = v1;
	A[1] = v2;
	A[2] = v3;

	end = std::clock();
	double ms = (end - start) / (double) (CLOCKS_PER_SEC / 1000);    

	//"Verification Value #1 - Sum of all entries: "
	printf("Verification Value #1 :%f\n", A[0]);
	//"Verification Value #2 - A[floor(n/2)][floor(n/2)]:  "
	printf("Verification Value #2 :%f\n", A[1]);
	//"Verification Value #3 - A[37][47]:  "
	printf("Verification Value #3 :%f\n", A[2]);
	printf("Finished in %f seconds\n",ms/1000.0);
	printf("Finished in %f milliseconds\n\n", ms);


	return;
}
 

int main(int argc, char** argv){
	//Do this for n = 500, 1000, 2000 and t = 10.
	//t iterated stencil calculation on an n x n matrix
	int n, t;
	
	if(argc != 4){
		n = 500;
		t = 10;
		debug = false;
	}
	else{
	    n = std::stoi(argv[1]);
	    t = std::stoi(argv[2]);
	    if(std::stoi(argv[3])){
		    debug = true;
	    }
	    else{
		    debug = false;
	    }
	}
	printf("Serial using n = %d, t = %d\n", n, t);

	const int size = n*n*sizeof(double);	
	
	double *A, *A0;
	
	A = (double *)malloc( size );
	A0 = (double *)malloc( size );

	//initialize the matrix
	initialize(A, n);
	initialize(A0, n);

	
	iterate(A, A0, n, t);

	return 0;
	
}
