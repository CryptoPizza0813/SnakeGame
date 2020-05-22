#include "snake.h"
using namespace std;
/*
TODO list
-make a menu
-toplist
-colors
*/


// 헤더의 line8
snakepart::snakepart(int col, int row)
{
  x = col;
  y = row;
}

snakepart::snakepart()
{
  x = 0;
  y = 0;
}

snakeclass::snakeclass()
{
  initscr(); // 가장 먼저 시작해야한다. ncurses TUI모드 사용 시작
  nodelay(stdscr, true);  // 프로그램이 유저가 키를 누르는것을 기다리지 않게 설정
                          // the getch not wait until the user press a
  keypad(stdscr, true);   // 키보드의 특수 키 입력을 가능하게 함
  noecho();               // 입력한게 바로스크린에뜨지   않는다.
  curs_set(0);            // 화면에 보이는 커서 설정 0 --> 커서 사라짐
  getmaxyx(stdscr, maxheight, maxwidth); // get maximun width and height

  // init a few veriables --> 헤더 파일에서 선언한 함수들을 이제 설정한다.
  partchar = 'x';           // this is for what char do we want for snake
  oldalchar = (char)219;    // 터미널 켰을 때 커서랑 똑같은 모양의 직사각형인듯?
  foo = '*';
  food.x = 0;
  food.y = 0;               // 이건 그냥 x y가 0이 되길 확신하기 위해서? 스크린 다른데서 안뜨게 ㅎ
  for(int i = 0; i < 5; i++)
    snake.push_back(snakepart(40 + i, 10));
  points = 0;               // points 0으로 초기화
  del = 110000;             // delay를 초기화 한다.  밀리언 밀리언 거리는데 이해 못했음. part3 00:30 부근
  get = 0;              // 첨에는 food를 가지지 않으니
  direction = 'l';          // left를 의미. 기본 설정
  srand(time(NULL));            // 헤더에서 cstdlib를 가져왔던걸 기억하는가?
  putfood();                // 음식 넣기

  // 이제 edge of the game을 만들어보자!
  // up-verticle
  for(int i = 0; i < maxwidth -1; i++) // 0에서 시작하니까 maxwidth - 1
  {
    move(0, i);  // 순서대로 verticle? 수직? 그리고 y는 horizontal, 수평  -2 하는 이유는.... 제대로 설명안함 part3 4:40
    addch(oldalchar);
  }
  // left-horizontal
  for(int i = 0; i < maxheight - 1; i++)
  {
    move(i, 0);
    addch(oldalchar);
  }
  // down-verticle
  for(int i = 0; i < maxwidth-1; i++)
  {
    move(maxheight - 2, i);
    addch(oldalchar);
  }
  // right-horizontal
  for(int i = 0; i < maxheight -1; i++)
  {
    move(i, maxwidth -2);
    addch(oldalchar);
  }

  // draw the snake
  for(int i = 0; i < snake.size(); i++)   // 5 elements를 줄걸 안다?
  {
    move(snake[i].y, snake[i].x);         // y가 첫번째, x가 두번째 파라미터인 것을 잊지 말라!
    addch(partchar);
  }
  move(maxheight - 1, 0);
  printw("%d", points);
  move(food.y, food.x); // food를 스크린에 띄어야 함
                        // 위에 putfood를 썻고, it will set food which is (entering snake.h) here(line 18)에서
                        // snakepart object called food, 위에 integer x and y
  addch(foo);
  refresh();            // 스크린 전체를 초기화
}

snakeclass::~snakeclass()
{
  nodelay(stdscr, false);
  getch();
  endwin();
}

void snakeclass::putfood()
{
  while(1)
  {
    int tmpx = rand() % maxwidth+1;
    int tmpy = rand() % maxheight+1;
    for(int i = 0; i < snake.size(); i++) // food가 뱀 안에 있는지를 확인
      if(snake[i].x == tmpx && snake[i].y == tmpy)
        continue;
    if(tmpx >= maxwidth -2 || tmpy >= maxheight - 3)    // -3하는 이유는 empty line이 맨 아래에 있으니
      continue;
    food.x = tmpx;
    food.y = tmpy;
    break;
  }
  move(food.y, food.x);
  addch(foo);
  refresh();   // refresh를 통해 음식이 화면에 나타나게 한다.
}

bool snakeclass::collision()
{
  if(snake[0].x == 0 || snake[0].x == maxwidth - 1 || snake[0].y == 0 || snake[0].y == maxheight -2) // 왼쪽 벽, 오른쪽 벽, 위 아래
    return true; // collision이 있다.

  for(int i = 2; i < snake.size(); i++ ) // head of snake는 할필요가 없다?  ㅇㅎ~  머리가 몸에 닿는지 조사.. 닿으면 collision true
    if(snake[0].x == snake[i].x && snake[0].y == snake[i].y)
      return true;

  // collision with the food
  if(snake[0].x == food.x && snake[0].y == food.y)  // 뱀의 머리가 음식과 부딛히면
  {
    get = true;
    putfood();
    points += 10;
    move(maxheight - 1, 0);
    printw("%d", points);
    if((points % 100) == 0)
      del -=10000;
  }else
    get = false;  // 음식과의collision이 없으면 false지 ㅋㅋ

  return false;
}

// 이제 뱀을 움직이는 move snake 함수를 만들 것이다.
void snakeclass::movesnake()
{
  int tmp=getch();
  switch(tmp)
  {
    case KEY_LEFT: // ncurses 헤더 파일에 있다
      if(direction != 'r')  // 방향이 r이 아니라면
        direction = 'l';    // l, 왼쪽으로 설정
      break;
    case KEY_UP:
      if (direction != 'd')
        direction = 'u';
      break;
    case KEY_DOWN:
      if(direction != 'u')
        direction = 'd';
      break;
    case KEY_RIGHT:
      if(direction != 'l')
        direction = 'r';
      break;
    case KEY_BACKSPACE: // 그만둔다는 뜻
      direction = 'q';
      break;
  }

  // 음식을 먹지 않으면 벡터의 멘 뒤를 제거, 음식을 먹었으면 제거 ㄴㄴ
  if(!get)
  {
    move(snake[snake.size() - 1].y, snake[snake.size() -1].x);
    printw(" ");
    refresh();
    snake.pop_back();
  }
  if(direction == 'l')    // 진행하는 방향으로 점을 한개 찍는것이다!
    snake.insert(snake.begin(), snakepart(snake[0].x-1, snake[0].y));  // 헤더의 처음에서 snakepart(int row, int col)임을 기억하는가?
  else if(direction == 'r')
    snake.insert(snake.begin(), snakepart(snake[0].x+1, snake[0].y));
  else if(direction == 'u')
    snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y-1));
  else if(direction == 'd')
    snake.insert(snake.begin(), snakepart(snake[0].x, snake[0].y+1));

  move(snake[0].y, snake[0].x);
  addch(partchar);
  refresh();
}


// 이제 snake.h에 있는 것 중, 이것을 모두 실행시키는 void start()의 구현만이 남았다.
void snakeclass::start()
{
  while(1)
  {
    if(collision())   // collision이 있는지 확인.. 있으면 종료!  collision()함수는 충돌이 있을 경우 true를 리턴함을 기억!
    {
      move(12, 36);  // game over 텍스트 때문에? -4? part6 1:50
      printw("GAME OVER");
      break;
    }

    movesnake();


    // 실행이 안된다 ㅠ
    if(direction == 'q')
      break;
    usleep(del);    // this will get whole module delay. 이것은 모든 모듈이 딜레이되게 한다.

  }
}
