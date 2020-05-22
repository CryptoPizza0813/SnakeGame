#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#ifndef SNAKE_H
#define SNAKE_H

struct snakepart{
  int x,y;
  snakepart(int col, int row);
  snakepart();
};

class snakeclass{
  int points, del/*delay 의미*/, maxwidth, maxheight;
  char direction/*상하좌우*/, partchar/*뱀의 일부분?*/, oldalchar/*테두리? edge를 설정*/, foo;
  bool get;
  snakepart food;/*음식의 위치*/
  std::vector<snakepart> snake;/*뱀을 대표할 벡터*/ //std:: 쓰는 이유는 그대로 쓰면 나쁜 프로그래밍이 된다? 망가진다?

  void putfood();
  bool collision(); // 벽에 닿으면 ...?
  void movesnake();

public:
  snakeclass();
  ~snakeclass();
  void start();  // 뱀게임을 시작할 것이다.
};
#endif
