#include "header.h"

int main()
{
	const int arraySizeY = 10;
	const int arraySizeM = 15;
	const int arraySizeX = 20;
	Matrix<int> A(10, 15);
	Matrix<int> B(15, 20);
	Matrix<int> C(10, 20);
	 //행렬 a,b,c 를 만든다.
	int a[arraySizeY * arraySizeM] = { 0 };
	int b[arraySizeM * arraySizeX] = { 0 };
	int c[arraySizeY * arraySizeX] = { 0 };

	// 알맞은 값으로 초기화 한다.
	for (int i = 0; i < arraySizeY * arraySizeM; i++)
		a[i] = i;
	for (int i = 0; i < arraySizeM * arraySizeX; i++)
		b[i] = i;

	A.SetElements(a);
	B.SetElements(b);
	C.SetElements(c);

	// 작업할 함수를 콜한다.
	MatMul<int>(A, B ,C);

	// 결과를 출력한다.
	for (int i = 0; i < arraySizeY * arraySizeX; i++)
	{
		if (i % arraySizeY == 0)
			putchar('\n');
		printf("%d ", c[i]);
	}
	putchar('\n');
	cudaDeviceReset();

	return 0;
}
