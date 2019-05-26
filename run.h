#ifndef RUN_H0
#define RUN_H
#include <list>
enum color { cR, cG, cB, cO, cY, cW, None };
enum position { sd_top, sd_bottom, sd_left, sd_right, sd_front, sd_back };
enum conect { up, down, left, right , front, back};
enum rotation { EndRotation, U, D, L, R, F, B, U_, D_, L_, R_, F_, B_ };
enum Text { BLACK = 15, BLUE = 9 * 16, GREEN = 10 * 16, RED = 12 * 16, ORANGE = 13 * 16, YELLOW = 14 * 16, WHITE = 15 * 16 };

class piece
{
public:
	piece();
	void Setcolor(color);
	void Setconnect(conect, piece*);
	piece* Getconnect(conect) const;
	color Getcolor() const;
private:
	color col;
	piece* conn[6];
};

class CUBE
{
public:
	CUBE(std::list<rotation>&);
	void Rotate(rotation);
	void Rotater(const char[]);
	void Setup();
	void Setpiece(piece [][3][3]);
	void Chapter1();
	void Chapter2();
	void Chapter3();
	void Chapter4();
	void Chapter5();
	void Chapter6();
	void Chapter7();
	void Run();
	void Shuffle();
	void Show();
private:
	conect OppositeConnect(conect);
	void CrossConnect(piece&, conect, piece&, conect);
	void Conversion(int, const char[], char[]) const;
	void CP1_SearchColor(piece*, conect, color, const char[]);
	void CP2_SearchColor(piece*, conect, color, color, const  char[], int);
	void CP1_ChangeColor(int);
	void CP2_ChangeColor(int);
	void CP3_ChangeColor(int);
private:
	//WCube* pcb;
	color sd_position[6];
	piece pie[6][3][3];
	std::list<rotation>& cubeFormula;
};
void SetTextColor(color);
void optimization(std::list<rotation>&);
rotation OppositeRotation(rotation);
#endif //RUN_H