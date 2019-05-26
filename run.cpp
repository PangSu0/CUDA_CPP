#include "run.h"
#include <iostream>
#include <Windows.h>
#include <list>

void optimization(std::list<rotation>& formula)
{
	auto iterator = formula.begin();
	std::advance(iterator,2);	//세 번째 부터 시작 ( 첫 번째와 두 번째에서 아래의 공식이 실행될 수가 없음 )
	std::list<rotation>::iterator;
	while (iterator != formula.end())
	{
		auto targetIterator = iterator;
		//같은방향 3번일경우 반대방향 1번으로 바꾸는 동작
		if (*(--targetIterator) == *iterator && *(--targetIterator) == *iterator)//if 뒤의 두 개와 현재의 값이 동일한지 검사
		{
			iterator++;
			formula.emplace(iterator, OppositeRotation(*targetIterator));	//반대값 추가
			formula.erase(targetIterator, iterator);	//중복된 3개의 값 제거
			iterator--;//추가한 값부터 다시 검사하기 위해 이전위치로
			continue;	//마지막 줄의 반복자 증가연산을 받지않기위해
		}

		targetIterator = iterator;
		if (OppositeRotation (*(--targetIterator)) == *iterator)//if 뒤의 값과 현재 값이 반대방향인 경우
		{
			iterator++;
			formula.erase(targetIterator, iterator);	//이전값과 현재값을 제거
			continue;	//마지막 줄의 반복자 증가연산을 받지않기위해
		}

		iterator++;
	}
}

rotation OppositeRotation(rotation rot)
{
	switch (rot)
	{
	case U :	return U_;
	case D :	return D_;
	case L :	return L_;
	case R :	return R_;
	case F :	return F_;
	case B :	return B_;
	case U_:	return U ;
	case D_:	return D ;
	case L_:	return L ;
	case R_:	return R ;
	case F_:	return F ;
	case B_:	return B ;
	default:	throw "OppositeRotation ERROR";
	}
}

void SetTextColor(color col)
{
	Text tx;
	switch (col)
	{
	case None:	tx = BLACK; break;
	case cW:	tx = WHITE; break;
	case cB:	tx = BLUE;	break;
	case cO:	tx = ORANGE; break;
	case cG:	tx = GREEN; break;
	case cR:	tx = RED;	break;
	case cY:	tx = YELLOW; break;
	default: return;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tx);
}

piece::piece()
{
	col = None;
	conn[0] = nullptr;
	conn[1] = nullptr;
	conn[2] = nullptr;
	conn[3] = nullptr;
	conn[4] = nullptr;
	conn[0] = nullptr;
	for (int i = 0; i < 6; i++)
		conn[i] = nullptr;
}

void piece::Setcolor(color co)
{
	col = co;
}

void piece::Setconnect(conect cn, piece * pi)
{
	conn[cn] = pi;
}

piece* piece::Getconnect(conect cn) const
{
	return conn[cn];
}

color piece::Getcolor() const
{
	return col;
}

CUBE::CUBE(std::list<rotation>& formula)
	:cubeFormula(formula)
{
	sd_position[sd_top] = cW;;
	sd_position[sd_left] = cB;;
	sd_position[sd_front] = cO;
	sd_position[sd_right] = cG;
	sd_position[sd_back] = cR;
	sd_position[sd_bottom] = cY;
	//면의 요소들 연결
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 2; x++)
		{
			CrossConnect(pie[0][y][x], front, pie[0][y][x + 1], back);
			CrossConnect(pie[0][x][y], left, pie[0][x + 1][y], right);
			CrossConnect(pie[1][y][x], down, pie[1][y][x + 1], up);
			CrossConnect(pie[1][x][y], back, pie[1][x + 1][y], front);
			CrossConnect(pie[2][y][x], down, pie[2][y][x + 1], up);
			CrossConnect(pie[2][x][y], left, pie[2][x + 1][y], right);
			CrossConnect(pie[3][y][x], down, pie[3][y][x + 1], up);
			CrossConnect(pie[3][x][y], front, pie[3][x + 1][y], back);
			CrossConnect(pie[4][y][x], down, pie[4][y][x + 1], up);
			CrossConnect(pie[4][x][y], right, pie[4][x + 1][y], left);
			CrossConnect(pie[5][y][x], back, pie[5][y][x + 1], front);
			CrossConnect(pie[5][x][y], left, pie[5][x + 1][y], right);
		}
	//면&면 연결
	for (int i = 0; i < 3; i++)
	{
		CrossConnect(pie[1][i][2], down, pie[2][i][0], up);		//1 & 2
		CrossConnect(pie[2][i][2], down, pie[3][i][0], up);		//2 & 3
		CrossConnect(pie[3][i][2], down, pie[4][i][0], up);		//3 & 4
		CrossConnect(pie[4][i][2], down, pie[1][i][0], up);		//4 & 1

		CrossConnect(pie[0][i][0], back, pie[1][0][i], front);	//0 & 1
		CrossConnect(pie[1][2][i], back, pie[5][2 - i][0], front);//1 & 5
		CrossConnect(pie[5][i][2], back, pie[3][2][i], front);	//5 & 3
		CrossConnect(pie[3][0][i], back, pie[0][2 - i][2], front);//3 & 0

		CrossConnect(pie[0][2][i], left, pie[2][0][i], right);	//0 & 2
		CrossConnect(pie[2][2][i], left, pie[5][0][i], right);	//2 & 5
		CrossConnect(pie[5][2][i], left, pie[4][2][2 - i], right);//5 & 4
		CrossConnect(pie[4][0][2 - i], left, pie[0][0][i], right);	//0 & 4
	}
}

void CUBE::Rotate(rotation rot)
{
	color temcol[3][3];
	color rotemcol[12];
	const int fw[10][2] = { { 0,0 },{ 0,1 },{ 0,2 },{ 1,2 },{ 2,2 },{ 2,1 },{ 2,0 },{ 1,0 },{ 0,0 },{ 0,1 } };
	int face;
	bool tr;
	piece* pieptr = nullptr;
	conect conn;

	cubeFormula.push_back(rot);
	switch (rot)
	{
	case U:		face = 0;	pieptr = &pie[2][0][0];	conn = up;		tr = true;		break;
	case U_:	face = 0;	pieptr = &pie[2][0][0];	conn = down;	tr = false;		break;
	case L:		face = 1;	pieptr = &pie[2][0][0];	conn = left;	tr = true;		break;
	case L_:	face = 1;	pieptr = &pie[2][0][0];	conn = right;	tr = false;		break;
	case F:		face = 2;	pieptr = &pie[0][2][0];	conn = front;	tr = true;		break;
	case F_:	face = 2;	pieptr = &pie[0][2][0];	conn = back;	tr = false;		break;
	case R:		face = 3;	pieptr = &pie[0][2][2];	conn = right;	tr = true;		break;
	case R_:	face = 3;	pieptr = &pie[0][2][2];	conn = left;	tr = false;		break;
	case B:		face = 4;	pieptr = &pie[0][0][0];	conn = back;	tr = true;		break;
	case B_:	face = 4;	pieptr = &pie[0][0][0];	conn = front;	tr = false;		break;
	case D:		face = 5;	pieptr = &pie[2][2][0];	conn = down;	tr = true;		break;
	case D_:	face = 5;	pieptr = &pie[2][2][0];	conn = up;		tr = false;		break;
	default:	return;
	}
	//면의 회전
	temcol[1][1] = pie[face][1][1].Getcolor();
	if (tr)
		for (int i = 0; i < 8; i++)
			temcol[fw[i + 2][0]][fw[i + 2][1]] = pie[face][fw[i][0]][fw[i][1]].Getcolor();
	else
		for (int i = 0; i < 8; i++)
			temcol[fw[i][0]][fw[i][1]] = pie[face][fw[i + 2][0]][fw[i + 2][1]].Getcolor();

	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			pie[face][y][x].Setcolor(temcol[y][x]);

	//라인 회전
	for (int i = 0; i < 12; i++)
	{
		rotemcol[i] = pieptr->Getcolor();
		pieptr = pieptr->Getconnect(conn);
	}
	for (int i = 0; i < 3; i++)
		pieptr = pieptr->Getconnect(conn);
	for (int i = 0; i < 12; i++)
	{
		pieptr->Setcolor(rotemcol[i]);
		pieptr = pieptr->Getconnect(conn);
	}
}

void CUBE::Rotater(const char str[])
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i + 1] != '_')
			switch (str[i])
			{
			case 'U':	Rotate(U);	break;
			case 'D':	Rotate(D);	break;
			case 'L':	Rotate(L);	break;
			case 'R':	Rotate(R);	break;
			case 'F':	Rotate(F);	break;
			case 'B':	Rotate(B);	break;
			}
		else
			switch (str[i])
			{
			case 'U':	Rotate(U_);	break;
			case 'D':	Rotate(D_);	break;
			case 'L':	Rotate(L_);	break;
			case 'R':	Rotate(R_);	break;
			case 'F':	Rotate(F_);	break;
			case 'B':	Rotate(B_);	break;
			}
		i++;
	}
}

void CUBE::Setup()
{
	color colors[6] = { cW, cB, cO, cG, cR, cY };
	for (int f = 0; f < 6; f++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				pie[f][y][x].Setcolor(colors[f]);
}

void CUBE::Setpiece(piece cube[][3][3])
{
	for (int f = 0; f < 6; f++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++)
				pie[f][y][x].Setcolor(cube[f][y][x].Getcolor());
}

void CUBE::Chapter1()
{
	enum { GREEN, RED, BLUE, ORANGE };
	CP1_ChangeColor(GREEN);
	CP1_ChangeColor(RED);
	CP1_ChangeColor(BLUE);
	CP1_ChangeColor(ORANGE);
}

void CUBE::Chapter2()
{
	enum { RED, BLUE, ORANGE, GREEN };
	CP2_ChangeColor(RED);
	CP2_ChangeColor(BLUE);
	CP2_ChangeColor(ORANGE);
	CP2_ChangeColor(GREEN);
}

void CUBE::Chapter3()
{
	enum { RED, BLUE, ORANGE, GREEN };
	CP3_ChangeColor(RED);
	CP3_ChangeColor(BLUE);
	CP3_ChangeColor(ORANGE);
	CP3_ChangeColor(GREEN);
}

void CUBE::Chapter4()
{
	piece* pieptr = &pie[5][1][1];
	conect temcon[4] = { right ,front, left, back };
	bool conflag[4] = { false, };
	int count = 0;
	const char* formula = "BRDR_D_B_";
	for (int i = 0; i < 4; i++)
		if (pieptr->Getconnect(temcon[i])->Getcolor() == sd_position[sd_bottom])
		{
			count++;
			conflag[i] = true;
		}
	if (count == 3)
		count = 2;
	switch (count)
	{
	case 0:
		Rotater(formula);
		for (int i = 0; i < 4; i++)
			if (pieptr->Getconnect(temcon[i])->Getcolor() == sd_position[sd_bottom])
				conflag[i] = true;
	case 2:
		if (conflag[1] && conflag[3])
			;
		else if (conflag[0] && conflag[2])
			Rotater("D");
		else if (conflag[0] && conflag[1])
			Rotater(formula);
		else if (conflag[1] && conflag[2])
		{
			Rotater("D");
			Rotater(formula);
		}
		else if (conflag[2] && conflag[3])
		{
			Rotater("DD");
			Rotater(formula);
		}
		else if (conflag[3] && conflag[0])
		{
			Rotater("D_");
			Rotater(formula);
		}
		Rotater(formula);
	}
}

void CUBE::Chapter5()
{
	piece* pieptr = &pie[4][2][1];
	bool conflag[4] = { false, };
	color answer[4] = { sd_position[sd_back], sd_position[sd_left], sd_position[sd_front], sd_position[sd_right] };
	const char* formula = "LDL_DLDDL_D";
	int count;
	do {
		count = 0;
		for (int i = 0; i < 4; i++)
			conflag[i] = false;
		for (int i = 0; i < 4; i++)
		{
			if (pieptr->Getcolor() == answer[i])
			{
				count++;
				conflag[i] = true;
			}
			for (int j = 0; j < 3; j++)
				pieptr = pieptr->Getconnect(down);
		}
		switch (count)
		{
		case 0:
		case 1:
			Rotater("D");
			break;
		case 2:
			if (conflag[1] && conflag[2])
				Rotater("D_");
			else if (conflag[2] && conflag[3])
				Rotater("D_D_");
			else if (conflag[3] && conflag[0])
				Rotater("D");
			Rotater(formula);
		}
	} while (count != 4);
}

void CUBE::Chapter6()
{
	const char* formula = "DBD_F_DB_D_F";
	char temula[20] = { 0, };
	conect temcon[4] = { right ,back, left, front };
	color cetchcolor[4] = { sd_position[sd_back], sd_position[sd_left], sd_position[sd_front], sd_position[sd_right] };
	bool conflag[4] = { false, };
	piece* pieptr = &pie[4][2][2];
	int cetchcount;

	do {
		cetchcount = 0;
		for (int i = 0; i < 4; i++)
			conflag[i] = false;
		for (int index = 0; index < 4; index++)
		{
			int count = 0;
			piece* temptr = pieptr;
			color temcolor[3];
			temcolor[0] = sd_position[sd_bottom];
			temcolor[1] = cetchcolor[index];
			temcolor[2] = cetchcolor[(index + 1) % 4];

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					if (temptr->Getcolor() == temcolor[j])
						count++;
				if (i == 0)
					temptr = pieptr->Getconnect(temcon[index]);
				else if (i == 1)
					temptr = pieptr->Getconnect(down);
			}
			if (count == 3)
			{
				conflag[index] = true;
				cetchcount++;
			}
			for (int j = 0; j < 3; j++)
				pieptr = pieptr->Getconnect(down);
		}
		switch (cetchcount)
		{
		case 0:	Rotater(formula);	break;
		case 1:
			if (conflag[0])
				Conversion(0, formula, temula);
			else if (conflag[1])
				Conversion(90, formula, temula);
			else if (conflag[2])
				Conversion(180, formula, temula);
			else if (conflag[3])
				Conversion(270, formula, temula);
			Rotater(temula);
		}
	} while (cetchcount != 4);
}

void CUBE::Chapter7()
{
	const char* formula = "B_U_BU";
	piece* pieptr = &pie[4][2][2];
	color cetchcolor[4] = { sd_position[sd_back], sd_position[sd_left], sd_position[sd_front], sd_position[sd_right] };
	for (int index = 0; index < 4; index++)
	{
		int count = 0;
		piece* temptr = pieptr;
		color temcolor[3];
		temcolor[0] = cetchcolor[index];
		temcolor[1] = sd_position[sd_bottom];
		temcolor[2] = cetchcolor[(index + 1) % 4];

		for (int i = 0; i < 3; i++)
		{
			if (temptr->Getcolor() == temcolor[i])
				count++;
			if (i == 0)
				temptr = pieptr->Getconnect(right);
			else if (i == 1)
				temptr = pieptr->Getconnect(down);
		}
		while (count != 3)
		{
			temptr = pieptr;
			count = 0;
			Rotater(formula);
			for (int i = 0; i < 3; i++)
			{
				if (temptr->Getcolor() == temcolor[i])
					count++;
				if (i == 0)
					temptr = pieptr->Getconnect(right);
				else if (i == 1)
					temptr = pieptr->Getconnect(down);
			}
		}
		Rotater("D_");
	}
}

void CUBE::Run()
{
	cubeFormula.clear();
	Chapter1();
	Chapter2();
	Chapter3();
	Chapter4();
	Chapter5();
	Chapter6();
	Chapter7();
	cubeFormula.push_back(EndRotation);
}

conect CUBE::OppositeConnect(conect conn)
{
	switch (conn)
	{
	case up:	return down;
	case down:	return up;
	case left:	return right;
	case right:	return left;
	case front:	return back;
	case back:	return front;
	default:	return up;
	}
}

void CUBE::CrossConnect(piece & pi1, conect con1, piece & pi2, conect con2)
{
	pi1.Setconnect(con1, &pi2);
	pi2.Setconnect(con2, &pi1);
}

void CUBE::Conversion(int angle, const char str[], char sttr[]) const
{
	int i = 0;
	switch (angle)
	{
	case 0:
		while (str[i] != '\0')
		{
			sttr[i] = str[i];
			i++;
		}
		break;
	case 90:
		while (str[i] != '\0')
		{
			switch (str[i])
			{
			case 'U':	sttr[i] = 'U';	break;
			case 'D':	sttr[i] = 'D';	break;
			case 'L':	sttr[i] = 'F';	break;
			case 'R':	sttr[i] = 'B';	break;
			case 'F':	sttr[i] = 'R';	break;
			case 'B':	sttr[i] = 'L';	break;
			case '_':	sttr[i] = '_';	break;
			case ',':	sttr[i] = ',';	break;
			}
			i++;
		}
		break;
	case 180:
		while (str[i] != '\0')
		{
			switch (str[i])
			{
			case 'U':	sttr[i] = 'U';	break;
			case 'D':	sttr[i] = 'D';	break;
			case 'L':	sttr[i] = 'R';	break;
			case 'R':	sttr[i] = 'L';	break;
			case 'F':	sttr[i] = 'B';	break;
			case 'B':	sttr[i] = 'F';	break;
			case '_':	sttr[i] = '_';	break;
			case ',':	sttr[i] = ',';	break;
			}
			i++;
		}
		break;
	case 270:
		while (str[i] != '\0')
		{
			switch (str[i])
			{
			case 'U':	sttr[i] = 'U';	break;
			case 'D':	sttr[i] = 'D';	break;
			case 'L':	sttr[i] = 'B';	break;
			case 'R':	sttr[i] = 'F';	break;
			case 'F':	sttr[i] = 'L';	break;
			case 'B':	sttr[i] = 'R';	break;
			case '_':	sttr[i] = '_';	break;
			case ',':	sttr[i] = ',';	break;
			}
			i++;
		}
		break;
	}
	sttr[i] = '\0';

}

void CUBE::CP1_SearchColor(piece * pieptr, conect conn, color co, const char str[])
{
	int i = 0;
	int j = 0;
	int k = 0;
	char rotstr[8][16] = { { 0, } };
	while (str[i] != '\0')
	{
		if (str[i] == ',')
		{
			k++;
			i++;
			j = 0;
			continue;
		}
		rotstr[k][j++] = str[i++];
	}

	int index;
	piece* pptr = pieptr;
	int valuenum = -1;
	for (index = 0; index < 11; index++)
	{
		pptr = pptr->Getconnect(conn);
		if (index % 3 == 2)
			continue;
		valuenum++;
		if (pptr->Getcolor() == sd_position[sd_top])
		{
			if (valuenum % 2 == 0 && pptr->Getconnect(conn)->Getcolor() == co)
				break;
			else if (valuenum % 2 == 1 && pptr->Getconnect(OppositeConnect(conn))->Getcolor() == co)
				break;
		}
	}
	if (index < 11)
		Rotater(rotstr[valuenum]);
}

void CUBE::CP2_SearchColor(piece * pieptr, conect conn, color co1, color co2, const char str[], int num)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int index;
	char rotstr[7][16] = { { 0, } };
	conect sidedata[4][4] = { { down,back,up,front },{ down,left,up,right },{ down,front,up,back },{ down,right,up,left } };
	while (str[i] != '\0')
	{
		if (str[i] == ',')
		{
			k++;
			i++;
			j = 0;
			continue;
		}
		rotstr[k][j++] = str[i++];
	}
	piece* pptr = pieptr;
	int valuenum = -1;
	for (index = 0; index < 10; index++)
	{
		pptr = pptr->Getconnect(conn);
		if (index % 3 == 1)
			continue;
		valuenum++;
		conect sidecon = sidedata[num][valuenum / 2];

		if (pptr->Getcolor() == sd_position[sd_top])
		{
			if (valuenum % 2 == 0 && pptr->Getconnect(OppositeConnect(conn))->Getcolor() == co1 && pptr->Getconnect(sidecon)->Getcolor() == co2)
				break;
			if (valuenum % 2 == 1 && pptr->Getconnect(conn)->Getcolor() == co2 && pptr->Getconnect(sidecon)->Getcolor() == co1)
				break;
		}
	}
	if (index < 10)
		Rotater(rotstr[valuenum]);
}

void CUBE::CP1_ChangeColor(int _color)
{
	const char* SearColor[4][3] = { {"F_UFU_,FRF_,F_RF,DRR,D_RR,BR_B_,B_R_,BU_B_U",
		"LUUL_U_U_,LLDF_RF,DF_RF,DDRR,RR,D_F_RF,RRDBR_B_",
		"F_DRRF,UUL_U_U_,R,UF_U_,U_BU,R_,U_U_LUU,U_B_U"},
	{"R_URU_,RBR_,R_BR,DBB,D_BB,LB_L_,L_B_,LU_L_U",
		"FUUF_U_U_,FFDR_BR,DR_BR,DDBB,BB,D_R_BR,BBDLB_L_",
		"R_DBBR,UUF_U_U_,B,UR_U_,U_LU,B_,U_U_FUU,U_L_U" },
	{"B_UBU_,BLB_,B_LB,DLL,D_LL,FL_F_,F_L_,FU_F_U",
		"RUUR_U_U_,RRDB_LB,DB_LB,DDLL,LL,D_B_LB,LLDFL_F_",
		"B_DLLB,UUR_U_U_,L,UB_U_,U_FU,L_,U_U_RUU,U_F_U"},
	{"L_ULU_,LFL_,L_FL,DFF,D_FF,RF_R_,R_F_,RU_R_U",
		"BUUB_U_U_,BBDL_FL,DL_FL,DDFF,FF,D_L_FL,FFDRF_R_",
		"L_DFFL,UUB_U_U_,F,UL_U_,U_RU,F_,U_U_BUU,U_R_U"} };
	conect colorConect[4] = { back, left, front, right };
	color colorPosition[4] = { sd_position[sd_right], sd_position[sd_back], sd_position[sd_left], sd_position[sd_front] };
	piece* pieptr = &pie[0][1][1];

	CP1_SearchColor(pieptr, colorConect[(_color + 1) % 4], colorPosition[_color], SearColor[_color][0]);
	CP1_SearchColor(pieptr, colorConect[_color], colorPosition[_color], SearColor[_color][1]);
	pieptr = &pie[_color + 1][1][1];
	CP1_SearchColor(pieptr, down, colorPosition[_color], SearColor[_color][2]);
}

void CUBE::CP2_ChangeColor(int _color)
{
	const char* SearColor[4][4] = { { "FDF_RDR_,DRDR_,DRD_R_D_D_RDR_,RFDDF_R_,B_D_B,B_D_BDB_D_B",
		"RDR_D_RDR_,RDR_,B_L_DDLB,D_B_L_DDLB,RD_R_,L_RD_R_L,L_D_LRDR_",
		"BD_D_B_B_DB,D_RDR_,DLD_RDR_L_,LD_RDR_L_,RDDR_,F_RDDR_F,F_DDRDR_F",
		"LDDL_RDR_,DDRDR_,DDB_DBDDB_D_B,DB_DBRDDR_,DB_D_B,R_D_RDDB_D_B,R_DRDB_D_B"},
		{ "RDR_BDB_,DBDB_,DBD_B_D_D_BDB_,BRDDR_B_,L_D_L,L_D_LDL_D_L",
		"BDB_D_BDB_,BDB_,L_F_DDFL,D_L_F_DDFL,BD_B_,F_BD_B_F,F_D_FBDB_",
		"LD_D_L_L_DL,D_BDB_,DFD_BDB_F_,FD_BDB_F_,BDDB_,R_BDDB_R,R_DDBDB_R",
		"FDDF_BDB_,DDBDB_,DDL_DLDDL_D_L,DL_DLBDDB_,DL_D_L,B_D_BDDL_D_L,B_DBDL_D_L" },
		{"BDB_LDL_,DLDL_,DLD_L_D_D_LDL_,LBDDB_L_,F_D_F,F_D_FDF_D_F",
		"LDL_D_LDL_,LDL_,F_R_DDRF,D_F_R_DDRF,LD_L_,R_LD_L_R,R_D_RLDL_",
		"FD_D_F_F_DF,D_LDL_,DRD_LDL_R_,RD_LDL_R_,LDDL_,B_LDDL_B,B_DDLDL_B",
		"RDDR_LDL_,DDLDL_,DDF_DFDDF_D_F,DF_DFLDDL_,DF_D_F,L_D_LDDF_D_F,L_DLDF_D_F"},
		{"LDL_FDF_,DFDF_,DFD_F_D_D_FDF_,FLDDL_F_,R_D_R,R_D_RDR_D_R",
		"FDF_D_FDF_,FDF_,R_B_DDBR,D_R_B_DDBR,FD_F_,B_FD_F_B,B_D_BFDF_",
		"RD_D_R_R_DR,D_FDF_,DBD_FDF_B_,BD_FDF_B_,FDDF_,L_FDDF_L,L_DDFDF_L",
		"BDDB_FDF_,DDFDF_,DDR_DRDDR_D_R,DR_DRFDDF_,DR_D_R,F_D_FDDR_D_R,F_DFDR_D_R"} };
	piece* pieptr[4] = { &pie[0][2][2], &pie[0][0][2], &pie[0][0][0], &pie[0][2][0] };
	conect colorConect[4] = { left, front, right, back };
	color colorPosition[4] = { sd_position[sd_right], sd_position[sd_back], sd_position[sd_left], sd_position[sd_front] };

	CP2_SearchColor(pieptr[_color], colorConect[_color], colorPosition[(_color + 1) % 4], colorPosition[_color], SearColor[_color][0], _color);
	CP2_SearchColor(pieptr[(_color + 1) % 4], colorConect[(_color + 1) % 4], colorPosition[(_color + 1) % 4], colorPosition[_color], SearColor[_color][1], (_color + 1) % 4);
	CP2_SearchColor(pieptr[(_color + 2) % 4], colorConect[(_color + 2) % 4], colorPosition[(_color + 1) % 4], colorPosition[_color], SearColor[_color][2], (_color + 2) % 4);
	CP2_SearchColor(pieptr[(_color + 3) % 4], colorConect[(_color + 3) % 4], colorPosition[(_color + 1) % 4], colorPosition[_color], SearColor[_color][3], (_color + 3) % 4);
}

void CUBE::CP3_ChangeColor(int _color)
{
	char str[20] = "L_D_LDBDB_";
	char rightformula[20] = "DRDR_D_B_D_B";
	char leftformula[20] = "D_B_D_BDRDR_";
	conect sidecon[7] = { right,back,left,front,right,back,left };
	int index;

	const int angle[4] = { 0,90,180,270 };
	color co1[4] = { sd_position[sd_back], sd_position[sd_left], sd_position[sd_front], sd_position[sd_right] };
	color co2[4] = { sd_position[sd_right], sd_position[sd_back], sd_position[sd_left], sd_position[sd_front] };


	Conversion(angle[_color], str, str);
	Conversion(angle[_color], rightformula, rightformula);
	Conversion(angle[_color], leftformula, leftformula);
	piece* pptr = nullptr;
	//빼기
	pptr = &pie[4][1][1];
	for (int i = angle[_color]; i > 0; i -= 90)
		for (int j = 0; j < 3; j++)
			pptr = pptr->Getconnect(down);

	for (index = 0; index < 11; index++)
	{
		pptr = pptr->Getconnect(down);
		if (index % 3 == 2)
			continue;
		if (index % 3 == 0 && pptr->Getcolor() == co1[_color] && pptr->Getconnect(down)->Getcolor() == co2[_color])
			break;
		if (index % 3 == 1 && pptr->Getcolor() == co1[_color] && pptr->Getconnect(up)->Getcolor() == co2[_color])
			break;
	}
	if (index == 10)
		return;
	if (index < 11)
	{
		int temp = (index / 3) * 90;
		Conversion(temp, str, str);
		Rotater(str);
	}

	//맞추기
	pptr = &pie[4][2][1];
	for (int i = angle[_color]; i > 0; i -= 90)
		for (int j = 0; j < 3; j++)
			pptr = pptr->Getconnect(down);

	for (index = 0; index < 360; index += 90)
	{
		if (pptr->Getcolor() == co1[_color] && pptr->Getconnect(sidecon[(index + angle[_color]) / 90])->Getcolor() == co2[_color])
		{
			for (int i = 0; i < index / 90; i++)
				Rotate(D_);
			Rotater(rightformula);
			break;
		}
		if (pptr->Getcolor() == co2[_color] && pptr->Getconnect(sidecon[(index + angle[_color]) / 90])->Getcolor() == co1[_color])
		{
			for (int i = -1; i < index / 90; i++)
				Rotate(D_);
			Rotater(leftformula);
			break;
		}
		for (int j = 0; j < 3; j++)
			pptr = pptr->Getconnect(down);
	}
}

void CUBE::Shuffle()
{
	for (int i = 0; i < 16; i++)
		Rotate(rotation(rand() % 12));
}

void CUBE::Show()
{

	for (int y = 0; y < 3; y++)
	{
		std::cout << "       ";
		for (int x = 0; x < 3; x++)
		{
			SetTextColor(pie[0][y][x].Getcolor());
			std::cout << "  ";
		}
		SetTextColor(None);
		std::cout << std::endl;
	}
	for (int y = 0; y < 3; y++)
	{
		for (int f = 1; f < 5; f++)
		{
			for (int x = 0; x < 3; x++)
			{
				SetTextColor(pie[f][y][x].Getcolor());
				std::cout << "  ";
			}
			SetTextColor(None);
			std::cout << " ";
		}
		std::cout << std::endl;
	}
	for (int y = 0; y < 3; y++)
	{
		std::cout << "       ";
		for (int x = 0; x < 3; x++)
		{
			SetTextColor(pie[5][y][x].Getcolor());
			std::cout << "  ";
		}
		SetTextColor(None);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}