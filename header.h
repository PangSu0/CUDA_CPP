#pragma once
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cstdio>
#include <cstdlib>
#define BLOCK_SIZE 16

template <typename T>
class Matrix
{
public:
	Matrix(int height, int width)
	{
		this->height = height;
		this->width = width;
		elements = nullptr;
	}
	int GetHeight()
	{
		return height;
	}
	int GetWidth()
	{
		return width;
	}
	T*& GetElements()
	{
		return elements;
	}

	T* GetElements() const
	{
		return elements;
	}

	size_t GetSize()const
	{
		return height * width * sizeof(T);
	}
	T& operator[] (int index)
	{
		return elements[index];
	}
	T operator[] (int index) const
	{
		return elements[index];
	}
	void SetElements(T* inputElements)
	{
		elements = inputElements;
	}
private:
	int height;
	int width;
	T* elements;
};

template <typename T>
__global__ void MatMulKernel( Matrix<T> A,  Matrix<T> B, Matrix<T> C)
{
	int row = blockIdx.y * blockDim.y + threadIdx.y;
	int col = blockIdx.x * blockDim.x + threadIdx.x;
	for (int e = 0; e < A.GetWidth(); ++e)
		C[row * C.GetWidth() + col] +=
		A[row * A.GetWidth() + e] * B[e * B.GetWidth() + col];
}

template <typename T>
void MatMul( Matrix<T> A,  Matrix<T> B, Matrix<T> C)
{
	// Load A and B to device memory
	Matrix<T> d_A(A.GetHeight(), A.GetWidth());
	cudaMalloc(&d_A.GetElements(), d_A.GetSize());
	cudaMemcpy(d_A.GetElements(), A.GetElements(), d_A.GetSize(), cudaMemcpyHostToDevice);

	Matrix<T> d_B(B.GetHeight(), B.GetWidth());
	cudaMalloc(&d_B.GetElements(), d_B.GetSize());
	cudaMemcpy(d_B.GetElements(), B.GetElements(), d_B.GetSize(), cudaMemcpyHostToDevice);

	// Allocate C in device memory
	Matrix<T> d_C(C.GetWidth(), C.GetWidth());
	cudaMalloc(&d_C.GetElements(), C.GetSize());



	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
	dim3 dimGrid(d_B.GetWidth() / dimBlock.x, d_A.GetHeight() / dimBlock.y);
	MatMulKernel<T> << < dimBlock, dimGrid >> > (d_A, d_B, d_C);

	cudaMemcpy(C.GetElements(), d_C.GetElements(), C.GetSize(), cudaMemcpyDeviceToHost);

	// Free device memory
	cudaFree(d_A.GetElements());
	cudaFree(d_B.GetElements());
	cudaFree(d_C.GetElements());
}
