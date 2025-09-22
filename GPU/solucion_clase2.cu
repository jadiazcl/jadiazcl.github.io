#include <iostream>
#include <fstream> 
#include <sstream>
#include <string> 
#include <memory>

#include <cuda_runtime.h>

__global__ void kernel(int N, int* A, int* B, int* C){
	int tId = blockDim.x * blockIdx.x + threadIdx.x;
	if (tId < N) {
		C[tId] = A[i] + B[i];
	}
	
}

int main(int argc, char **argv){	
	
	cudaError_t err = cudaSuccess;	
	int N = 256;      
	int Nblocks = 1;
	int Nthreads = 256;
	int* d_A = NULL;
	int* d_B = NULL;
	int* d_C = NULL;
	int* A = new int[N];
	int* B = new int[N];
	int* C = new int[N];
	size_t size = N * sizeof(int);
	
	for(int i = 0; i < N ;i++){
			 A[i] = 1;
			 B[i] = 2;
	}		

	err = cudaMalloc((void **)&d_A, size);
	if (err != cudaSuccess)
	{
			fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
	}
	err = cudaMalloc((void **)&d_B, size);
	if (err != cudaSuccess)
	{
			fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
	}
	err = cudaMalloc((void **)&d_C, size);
	if (err != cudaSuccess)
	{
			fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
	}

	err = cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	if (err != cudaSuccess)
	{
			fprintf(stderr, "Failed to copy vector A from host to device (error code %s)!\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
	}
	err = cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	if (err != cudaSuccess)
	{
			fprintf(stderr, "Failed to copy vector B from host to device (error code %s)!\n", cudaGetErrorString(err));
			exit(EXIT_FAILURE);
	}
	
	kernel<<<Nblocks, Nthreads>>>(N, d_A, d_B,d_C);
	cudaDeviceSynchronize();
	err = cudaGetLastError();
if (err != cudaSuccess) {
    fprintf(stderr, "Failed to launch kernel (error code %s)!\n", cudaGetErrorString(err));
    exit(EXIT_FAILURE);
}

  err = cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
  if (err != cudaSuccess)
  {
      fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));
      exit(EXIT_FAILURE);
  }
	bool flag = true;
	for(int i = 0; i < N ;i++){		
		if( C[i] != A[i] + B[i]){
			flag = false;
		}				
	}
	if(flag){
		std::cout << "Vector Add Ok :D" << std::endl;
	}else{
		std::cout << "Oh no, Error detected D:"<< std::endl;
	}


	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	delete[] A;
	delete[] B;
	delete[] C;

	return 0;
}