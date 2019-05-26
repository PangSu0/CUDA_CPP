// CUBE.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
#include <cstdio>
#include <iostream>
#include "run.h"
#include <conio.h>
#include <list>
void Test();

int main()
{
	/*std::list<rotation> cubeFormulda(300);
	CUBE cube(cubeFormulda);*/
	Test();

	return 0;
}

void Test()
{
	std::list<rotation> cubeFormula;
	//rotation cubeFormulda[300];

	CUBE cube(cubeFormula);
	std::cout << "`: 초기화" << std::endl;
	std::cout << "0:  출력" << std::endl;
	std::cout << "S:  실행" << std::endl;
	std::cout << "A:  랜덤" << std::endl;
	std::cout << "Z:  설정" << std::endl;
	std::cout << "X:  종료" << std::endl;
	char ch;
	cube.Setup();

	do {
		std::cin >> ch;
		switch (ch)
		{
		case 'S':	cube.Run();			break;
		case '`':	cube.Setup();		break;
		case '0':	cube.Show();		break;
		case '1':	cube.Chapter1();	break;
		case '2':	cube.Chapter2();	break;
		case '3':	cube.Chapter3();	break;
		case '4':	cube.Chapter4();	break;
		case '5':	cube.Chapter5();	break;
		case '6':	cube.Chapter6();	break;
		case '7':	cube.Chapter7();	break;
		case 'A':	cube.Shuffle();		break;

		case 'R':	cube.Rotate(R);		break;
		case 'L':	cube.Rotate(L);		break;
		case 'F':	cube.Rotate(F);		break;
		case 'B':	cube.Rotate(B);		break;
		case 'U':	cube.Rotate(U);		break;
		case 'D':	cube.Rotate(D);		break;

		case 'r':	cube.Rotate(R_);	break;
		case 'l':	cube.Rotate(L_);	break;
		case 'f':	cube.Rotate(F_);	break;
		case 'b':	cube.Rotate(B_);	break;
		case 'u':	cube.Rotate(U_);	break;
		case 'd':	cube.Rotate(D_);	break;

		case 'x':	std::cout << cubeFormula.size() << std::endl; break;
		case 'Q':
			for (int i = 0; i < 100; i++)
			{
				cube.Shuffle();
				cube.Run();
				cube.Show();
			}
		case 'q':
			for (int i = 0; i < 100000; i++)
			{
				cube.Shuffle();
				cube.Run();
				std::cout << i << std::endl;
				/*std::cout << "전" << std::endl;
				for (auto i : cubeFormula)
					std::cout << i;
				std::cout << std::endl;

				optimization(cubeFormula);
				std::cout << "후" << std::endl;
				for (auto i : cubeFormula)
					std::cout << i;
				std::cout << std::endl;*/
			}
		default:	std::cout << "Error" << std::endl;	break;
		}
	} while (ch != 'X');
	std::cout << " END " << std::endl;
}