#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "run.h"
#include <conio.h>
#include <list>

using namespace std;
#define BLOCK_SIZE 10

typedef struct
{
	int height;
	int width;
	double* elements;
}Matrix;

__global__ void MatMulKernel(const Matrix, const Matrix, Matrix);

void MatMul(const Matrix A, const Matrix B, Matrix C)
{
	// Load A and B to device memory
	Matrix d_A;
	d_A.width = A.width; d_A.height = A.height;
	size_t size = A.width * A.height * sizeof(double);
	cudaMalloc(&d_A.elements, size);
	cudaMemcpy(d_A.elements, A.elements, size,
		cudaMemcpyHostToDevice);
	Matrix d_B;
	d_B.width = B.width; d_B.height = B.height;
	size = B.width * B.height * sizeof(double);
	cudaMalloc(&d_B.elements, size);
	cudaMemcpy(d_B.elements, B.elements, size,
		cudaMemcpyHostToDevice);

	// Allocate C in device memory
	Matrix d_C;
	d_C.width = C.width; d_C.height = C.height;
	size = C.width * C.height * sizeof(double);
	cudaMalloc(&d_C.elements, size);

	// Invoke kernel
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(B.width / dimBlock.x, A.height / dimBlock.y);
	MatMulKernel << <dimGrid, dimBlock >> > (d_A, d_B, d_C);

	// Read C from device memory
	cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);

	// Free device memory
	cudaFree(d_A.elements);
	cudaFree(d_B.elements);
	cudaFree(d_C.elements);
}

__global__ void MatMulKernel(Matrix A, Matrix B, Matrix C)
{
	// Each thread computes one element of C
	// by accumulating results into Cvalue
	float Cvalue = 0;
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	for (int e = 0; e < A.width; ++e)
		Cvalue += A.elements[row * A.width + e]
		* B.elements[e * B.width + col];
	C.elements[row * C.width + col] = Cvalue;
}

double ReturnFromZeroToOneExclusives() // 0~1 미포함 값
{
	return (double)(rand()+1) / (RAND_MAX + 2);
}

int main()
{
	std::list<rotation> cubeFormula;
	CUBE cube(cubeFormula);
	cube.Setup();
	
	//srand((unsigned)time(NULL));
	/*srand(1);
	const int arraySizeY = 10;
	const int arraySizeM = 15;
	const int arraySizeX = 20;
	Matrix A;
	Matrix B;
	Matrix C;
	A.height = arraySizeY;
	A.width = arraySizeM;
	B.height = arraySizeM;
	B.width = arraySizeX;
	C.height = arraySizeY;
	C.width = arraySizeX;

	double a[arraySizeY * arraySizeM] = { 0 };
	double b[arraySizeM * arraySizeX] = { 0 };
	double c[arraySizeY * arraySizeX] = { 0 };

	for (int i = 0; i < arraySizeY * arraySizeM; i++)
		a[i] = ReturnFromZeroToOneExclusives();
	for (int i = 0; i < arraySizeM * arraySizeX; i++)
		b[i] = ReturnFromZeroToOneExclusives();

	A.elements = a;
	B.elements = b;
	C.elements = c;

	MatMul(A, B ,C);

	for (int i = 0; i < arraySizeY * arraySizeX; i++)
	{
		if (i % arraySizeY == 0)
			putchar('\n');
		printf("%6.10f\t", C.elements[i]);
	}
	putchar('\n');
	cudaDeviceReset();*/

	return 0;
}
