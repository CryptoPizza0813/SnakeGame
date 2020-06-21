
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#ifndef SNAKEGAME_H
#define SNAKEGAME_H

//사용자 정의 데이터 구조체
struct CharPosition 
{
	int x, y;
	CharPosition(int col, int row);
	CharPosition();
};

class SnakeGame 
{
private:
	int score, del, delay, maxwidth, maxheight,map;
	char direction, headchar, partchar, edgechar, fruitchar, bad;
	int time1, fruitnum;
	int cursec, totalsec;
		
	bool get_poison;
	// partchar snake's body
	// headchaar snake's head
	// edgechar game wall
	// fruitchar fruit
	// del  delay
	bool bEatsFruit;
	bool EatsPoison;
	CharPosition fruit; // 구조체 변수 선언 
	CharPosition poison; // poison 구조체 변수 선언
	std::vector<CharPosition> snake; // 뱀 몸통담을 벡터
	std::vector<CharPosition> snake1;
	std::vector<CharPosition> snake2;
	std::vector<CharPosition> snake3;
	std::vector<CharPosition> snake4;

	void InitGameWindow();
	void DrawWindow();  
	void DrawSnake();
	void PrintScore();
	void PositionFruit();
	bool FatalCollision();
	void MoveSnake();
	bool GetsFruit();
	void nextStage();
	void reset();	
	void putPoison();
	bool getPoison();
public:
	
	SnakeGame();
	~SnakeGame(); 
	void PlayGame();
};

#endif
