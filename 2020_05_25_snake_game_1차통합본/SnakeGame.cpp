#include "SnakeGame.h"
#include <unistd.h>

using namespace std;


CharPosition::CharPosition(int col, int row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}
/* 문제점들 지금 ,
 * fruit 생성시, 2,3,4라운드에서 새로운 벽들 위에 생겨나는 가능성이 포함되어있다. 그거 제거해야함
 */
SnakeGame::SnakeGame()
{
	// 변수 초기화
	headchar = (char)229;
	partchar = 'x'; 
	edgechar = (char)64; 
	fruitchar = 'x'; 
	fruit.x = 0;
	fruit.y = 0;
	score = 0;
	map=0;
	fruitnum=0;
	poison.x=0;
	poison.y=0;
	bad='!';
	get_poison=0;
	
	//del은 딜레이 1000000이 1초 50만은 0.5초
	del = 100000;
	delay = 5000000;
	bool bEatsFruit = 0;
	
	direction = 'l';
	srand(time(NULL));	
	InitGameWindow();
	//maxwidth- 15한 이유는 점수를 오른쪽에 출력해야하는데 그 자리만들라고
	maxwidth-=20;
	PositionFruit();
	putPoison();
	DrawWindow();
	DrawSnake();
	PrintScore();

	refresh();	
}

SnakeGame::~SnakeGame()
{
	nodelay(stdscr, false);
	getch();
	
	endwin();
}

//다음 스테이지 넘어갈 시 변수들의 초기화 실행부분
//score와 snake의 속도 뱀의 이동방향을 초기화시킴
void SnakeGame::reset(){
	score=0;
	del=100000;
	direction='l';
	
	
}
//목표조건 달성시 실행, 다음 스테이지(맵)으로 넘어가고 변수들 초기화
void SnakeGame::nextStage(){
	clear();
	refresh();
	reset();
	InitGameWindow();
	maxwidth-=20;
	PositionFruit();
	putPoison();
	DrawWindow();
	DrawSnake();

	snake.assign(snake2.begin(), snake2.end());	//스테이지 종료후 snake벡터를 초기화 길이를
	PrintScore();
	
	refresh();
}

// initialise the game window
void SnakeGame::InitGameWindow() 
{ 

	initscr(); // initialise the screen
	nodelay(stdscr,TRUE); //
	keypad(stdscr, true); // 방향키를 통해 뱀 이동을 조정할거니 keypad(*win,bool)의 bool true로
	noecho(); // 입출력모드 지정 반향기능을 정지  입력한 값을 화면에 안보이게
	curs_set(0); // 화면에 보이는 커서를 설정, 0은 안보이게, 1은 일반, 2는 큰 커서
	refresh();
	getch();	
		
	//현재 화면의 가로 세로의 크기를 구한다 getmaxyx(win,max_y,max_x) y값의크기와 x의크기를구해 각각 max_y,max_x에 넣음
	//win에 해당하는 stdscr은 표준화면을 의미함 
	getmaxyx(stdscr, maxheight, maxwidth); 
	return; 
}

// 맵 만들기map 변수 값에 따라서 다른 맵을 그리게끔, map 변수는 미션조건 달성시 1씩 늘어난다
void SnakeGame::DrawWindow()
{
	if(map==0){					//stage 1
		//뱀새기가 활동할 부분을 테두리로 표기, 이때 Immune Wall은 다른문자로 표기
		for (int i = 0; i < maxwidth; i++) // top
		{
			move(0, i);
			addch(edgechar);
	
		}
		for (int i = 0; i < maxwidth; i++) // bottom
		{	
			move(maxheight-1, i);
			addch(edgechar);
		}
		for (int i = 0; i < maxheight; i++) // left side
		{
			move(i, 0);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		for (int i = 0; i < maxheight; i++) // right side
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		return;
	}else if(map==1){					//stage 2
		for(int i=0; i< maxwidth; i++) // top
		{
			move(0, i);
			addch(edgechar);
		}
		move(0,maxwidth/2);
		addch((char)219);

		for(int i=0; i<maxwidth; i++) //bottom
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		move(maxheight-1, maxwidth/2);
		addch((char)219);
		
		//top center와 bottom center 잇는 선 중앙 일부는 뚫려서 지나가는길
		for(int i=1; i<maxheight-1; i++)
		{
			if(i>=maxheight/2-2 and i<=maxheight/2+2){
				continue;
			}
			move(i,maxwidth/2);
			addch(edgechar);
		}
		
		for(int i=0; i<maxheight; i++) // left side
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}

		for(int i=0; i<maxheight; i++) // right side
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		
		return;
	}else if(map==2){					//stage 3
		for(int i=0; i<maxwidth; i++) //top
		{
			move(0, i);
			addch(edgechar);
		}
		for(int i=0; i<maxwidth; i++) // bottom
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		for(int i=0; i<maxheight; i++) //left
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		for(int i=0; i<maxheight; i++) // right
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		//맵 중앙을 차지하는 네모 벽, 게이트가 생기는 경우, 네모의 내부에는 생기지않게 immune으로
		for(int i=maxwidth/4+5; i<(maxwidth*3)/4-5; i++)
		{
			for(int j=maxheight/4+2; j<(maxheight*3)/4-2; j++)
			{
				if(i==maxwidth/4+5 or i==(maxwidth*3)/4-6){
					move(j,i);
					addch(edgechar);
				}else if(j==maxheight/4+2 or j==(maxheight*3)/4-3){
					move(j,i);
					addch(edgechar);
				}
				else{
					move(j,i);
					addch((char)219);
				}
			}
			
		}

		return;
	}else if(map==3){				//stage 4
		for(int i=0; i<maxwidth; i++)
		{
			move(0,i);
			addch(edgechar);
		}
		for(int i=0; i<maxwidth; i++)
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		for(int i=0; i<maxheight; i++)
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		for(int i=0; i<maxheight; i++)
		{
			move(i,maxwidth-1);
			if(i==maxheight-1 or i==0){
				addch((char)219);
			}else{
				addch(edgechar);
			}
		}
		//회전 하듯이 걍 나눠 ㅜㅜ
		for(int i=1; i<(maxwidth*4)/5; i++)
		{
			move((maxheight*3)/4,i);
			addch(edgechar);
		}
		move((maxheight*3)/4,0);
		addch((char)219);
		for(int i=maxheight/2; i<=(maxheight*3)/4; i++){
			move(i, (maxwidth*4)/5);
			addch(edgechar);
		}
		move((maxheight*3)/4,(maxwidth*4)/5);
		addch((char)219);

		for(int i=maxwidth/5; i<maxwidth; i++)
		{
			move(maxheight/4, i);
			addch(edgechar);
		}
		move(maxheight/4,maxwidth-1);
		addch((char)219);

		for(int i=maxheight/4; i<=maxheight/2; i++)
		{
			move(i,maxwidth/5);
			addch(edgechar);
		}
		move(maxheight/4, maxwidth/5);
		addch((char)219);
		return;
	}

}



// draw snake's body
void SnakeGame::DrawSnake()
{
	if(map==0){
	for (int i = 0; i < 5; i++) 
	{
		snake.push_back(CharPosition(20+i, 5));
	}
	
	//길이가 몇이든 길이만큼 표시를해야함
	for (int i = 0; i < snake.size(); i++)
	{
		move(snake[i].y, snake[i].x);
		addch(partchar);
		
	}
	return;
	}else if(map==1){
		for(int i=0; i<5; i++){
			snake2.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake2.size(); i++){
			move(snake2[i].y, snake2[i].x);
			addch(partchar);
		}
		return;
	}else if(map==2){
		for(int i=0; i<5; i++){
			snake3.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake3.size(); i++){
			move(snake3[i].y, snake3[i].x);
			addch(partchar);
		}
		return;
	}else if(map==3){
		for(int i=0; i<5; i++){
			snake4.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake4.size(); i++){
			move(snake4[i].y, snake4[i].x);
			addch(partchar);
		}
		return;
	}
}

// 점수표시 우측상단에할거
void SnakeGame::PrintScore()
{
	move(0, maxwidth+1);
	printw("Score: %d", score);
	move(1, maxwidth+1);
	printw("Stage: %d", map+1);
	
	return;
}

// 화면에 먹이 출력  임의의 위치여야함
void SnakeGame::PositionFruit()
{
	
	while(1)
	{
		int tmpx = rand()%maxwidth+1; // 1늘린건 0을 피하기위해서  1~maxwidth까지
		int tmpy = rand()%maxheight+1;

		// 뱀 위에는 나타나지않는다
		for (int i = 0; i < snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				continue; // 뱀 위에 나타나는거였으면 밑에 무시하고 다시 루프돌림
			}
		}
		// 맵에 있으면 넘어간다 ** 
		if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
		{
			continue; 
		}
		//2,3,4스테이지의 맵까지 넘어가게 포함시킨다
		if(map==1){
			if((tmpx==maxwidth/2 and (tmpy>=0 and tmpy<maxheight/2-2)) or
			 (tmpx==maxwidth/2 and (tmpy<=maxheight and tmpy>maxheight/2+2))){
			continue;
			}	
		}else if(map==2){
			if((tmpx>=maxwidth/4+5 and tmpx<=(maxwidth*3)/4-6) and (tmpy>=maxheight/4+2 and 
						tmpy<=(maxheight*3)/4-3)) {
		continue;
			}		

		}else if(map==3){
			if(((tmpx==maxwidth/5) and (tmpy>=maxheight/4 and tmpy<=maxheight/2)) or
					((tmpy==maxheight/4) and (tmpx>maxwidth/5 and tmpx<maxwidth)) or
					((tmpy==(maxheight*3)/4) and (tmpx>0 and tmpx<=(maxwidth*4)/5)) or
				       ((tmpx==(maxwidth*4)/5) and (tmpy>=maxheight/2 and tmpy<=(maxheight*3)/4))){
			continue;
			}	
		}
		// 좌표설정되면 맵상에 표시
		fruit.x = tmpx;
		fruit.y = tmpy;
		break;	
		
	}
	move(fruit.y, fruit.x);
	addch(fruitchar);
	refresh();

}

void SnakeGame::putPoison()
{
	while(1)
	{
		int tmpx = rand()%maxwidth+1;
		int tmpy = rand()%maxheight+1;
		for(int i=0; i<snake.size(); i++){
			if((snake[i].x == tmpx && snake[i].y ==tmpy) || (fruit.x == tmpx && fruit.y==tmpy))
				continue;
		}
		if(tmpx >= maxwidth-2 || tmpy >= maxheight-3){
			continue;
		}

		if(map==1){
			if((tmpx==maxwidth/2 and (tmpy>=0 and tmpy<maxheight/2-2)) or
					(tmpx==maxwidth/2 and (tmpy<=maxheight and tmpy>maxheight/2+2))){
				continue;
			}
		}else if(map==2){
			if((tmpx>=maxwidth/4+5 and tmpx<=(maxwidth*3)/4-6) and (tmpy>=maxheight/4+2 and
						tmpy<=(maxheight*3)/4-3)){
				continue;
			}
		}else if(map==3){
			if(((tmpx==maxwidth/5) and (tmpy>=maxheight/4 and tmpy<=maxheight/2)) or
					((tmpy==maxheight/4) and (tmpx>maxwidth/5 and tmpx<maxwidth)) or
					((tmpy==(maxheight*3)/4) and (tmpx>0 and tmpx<=(maxwidth*4)/5)) or
					((tmpx==(maxwidth*4)/5) and (tmpy>=maxheight/2 and tmpy<=(maxheight*3)/4))){
				continue;
			}
		}
		
		poison.x = tmpx;
		poison.y = tmpy;
		break;
	}
	move(poison.y, poison.x);
	addch(bad);
	refresh();
}


// set game over situations
bool SnakeGame::FatalCollision()
{
	// 벽에 부딪칠경우
	if(map==0){		//stage 1 충돌조건 작동 잘됨
		if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2)
		{
			return true;
		}
	}else if(map==1){	//stage 2 충돌조건 작동 잘됨
		if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2
				|| (snake[0].x==maxwidth/2) and (snake[0].y>=0 and snake[0].y<maxheight/2-2)
				|| (snake[0].x==maxwidth/2) and (snake[0].y<=maxheight and snake[0].y>maxheight/2+2))
		{
			return true;
		}
	}else if(map==2){	//stage 3 충돌조건 작동 잘됨
		if(snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2
				|| (snake[0].x==maxwidth/4+5 or snake[0].x==(maxwidth*3)/4-6) and 
				(snake[0].y>=maxheight/4+2 and snake[0].y<(maxheight*3)/4-2)
				|| (snake[0].y==maxheight/4+2 or snake[0].y==(maxheight*3)/4-3) and
				(snake[0].x>=maxwidth/4+5 and snake[0].x<(maxwidth*3)/4-5))
		{
			return true;
		}
	}else if(map==3){	//stage 4 충돌조건 작동 잘됨
		if(snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-2
				|| (snake[0].x==maxwidth/5) and (snake[0].y>=maxheight/4 and snake[0].y<=maxheight/2)
				|| (snake[0].y==maxheight/4) and (snake[0].x>maxwidth/5 and snake[0].x<maxwidth)
				|| (snake[0].y==(maxheight*3)/4) and (snake[0].x>0 and snake[0].x<=(maxwidth*4)/5)
				|| (snake[0].x==(maxwidth*4)/5) and (snake[0].y>=maxheight/2 and 
					snake[0].y<=(maxheight*3)/4))
		{
			return true;
		}
	}

	
	//자기몸에 부딪친경우
	for (int i = 2; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			return true;
		}
	}

	return false;
}

//맵상의 fruit먹었을때
bool SnakeGame::GetsFruit()
{	
		
	
	//뱀새끼 대가리좌표 snake[0].x , snake[0].y가 fruit의 좌표와 동일하면 먹는상황임
	if (snake[0].x == fruit.x && snake[0].y == fruit.y)
	{	

		//PositionFruit() fruit을 먹었으니 랜덤한 위치에 재생성
		PositionFruit(); 
		score +=50; 
		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score%100) == 0)
		{
			del -= 10000;
		}
		return bEatsFruit = true;
	}
	else 
	{
		return bEatsFruit = false;
	}

	
	//}
	//else if((time(0)-time1) %5 == 0){
	//	move(fruit.y,fruit.x);
	//	printw(" ");
	//	fruitnum--;
	//	PositionFruit();
		
		//5초 뒤까지 안먹으면 사라지게했는데 저 시간간격이 말을안들어서 강제 딜레이줘야
	//	usleep(600000);
			
		
	//}
	
	return bEatsFruit;
	
}

bool SnakeGame::getPoison()
{
	if(snake[0].x==poison.x && snake[0].y == poison.y)
	{
		get_poison = true;
		putPoison();
		if(snake.size()==4){
			direction='q';
		}
		score-=50;
		if(score<0){		//score는 음수가 안나오게 한다
			score=0;
		}
		PrintScore();
	}else{
		return get_poison = false;
	}
	return get_poison;

}

void SnakeGame::MoveSnake()
{	
	//뱀새끼 움직일 방향 결정한다, 키 입력받아서 switch로 case를 나눠 실행, BackSpace시 게임 종료
	int KeyPressed = getch();
		
	switch(KeyPressed)
	{
		case KEY_LEFT:
			if (direction != 'r') 
			{ direction = 'l'; break;}	
			else if(direction == 'r')
			{ direction = 'q'; break;}
		case KEY_RIGHT:
			if (direction != 'l')
			{ direction = 'r'; break;}
			else if(direction == 'l')
			{ direction = 'q'; break;}
		case KEY_UP:
			if (direction != 'd')
			{ direction = 'u'; break;}
			else if(direction == 'd')
			{ direction = 'q'; break;}
		case KEY_DOWN:
			if (direction != 'u')
			{ direction = 'd'; break;}
			else if(direction == 'u')
			{ direction = 'q'; break;}
		case KEY_BACKSPACE:
			direction = 'q'; // key to quit the game
			break;
	}

	// Fruit과 Poison을 둘 다 안먹었을때 size 그대로, 앞으로 전진, 맨뒤제거, 새로운 머리, 예전머리 몸통으로표기
	if (!bEatsFruit)
	{	
		//snake 몸통 vector의 맨 마지막부분을 지우고 vector의 크기도 1줄임
		move(snake[snake.size()-1].y, snake[snake.size()-1].x); // 맨 뒤 요소로 이동
		printw(" "); 
		refresh();
		snake.pop_back();
	
	}
	
	if(get_poison)
	{
		move(snake[snake.size()-1].y, snake[snake.size()-1].x);
		printw(" ");
		move(snake[snake.size()-2].y, snake[snake.size()-2].x);
		printw(" ");
		refresh();
		snake.pop_back();

		
	}
	
	// 이동하는 방향에다가 문자를 넣어야
	if (direction == 'l')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x-1, snake[0].y)); } 
	else if (direction == 'r')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x+1, snake[0].y)); }
	else if (direction == 'u')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y-1)); }
	else if (direction == 'd')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y+1)); }

	move(snake[0].y, snake[0].x);	
	addch(headchar); // 새로운 머리를 지정
	move(snake[1].y, snake[1].x);
	addch(partchar); // 머리와 몸통의 문자가 다르니까 머리가 이동하고 그 이전 자리를 몸통의 문자로 바꿔줘야함	
	refresh();
	return;
	
	
}

// SnakeGame newGame >> newGame.PlayGame(); 으로 게임 시작
void SnakeGame::PlayGame()
{

    while(1)
    {	
	
	//머리 충돌시, 역방향입력시 게임 종료 가운데에 출력
        if (FatalCollision())
        {
            move((maxheight-2)/2,(maxwidth-5)/2-5);
            printw("GAME OVER");
            break;
        }
	
	//먹었냐안먹었냐 bool 판별한 뒤에 MoveSnake()실행
	GetsFruit();
	getPoison();
	MoveSnake();
	//조건 달성시 증감, 종료
	if(score==100){
		map+=1;
		if(map==4){
			direction='f';
		}else{
			direction='p';
		}
	}
	
	//4스테이지 까지 클리어 했을 시 실행 축하 메시지와 함께 게임 종료
	if(direction=='f'){
		move((maxheight-2)/2, (maxwidth-5)/2-10);
		printw("Congratualtion!!!, YOU ARE WINNER!!");
		refresh();
		usleep(2000000);
		break;
	}
        if (direction=='q') //exit
        {
		move((maxheight-2)/2,(maxwidth-5)/2+2);
		printw("GAME OVER");
		
		
        	break;
        }
	//스테이지 미션 클리어시 실행, nextStage()함수로 루프 내에서 반복
	if(direction=='p'){

		move((maxheight-2)/2,(maxwidth-5)/2-2);
		printw("Next Stage");
		refresh();
		//미션 충족시 next stage가 중앙에 뜨고 1.5초 뒤 자동으로 다음 스테이지
		usleep(1500000);
		clear();
		refresh();

		direction='l';
		nextStage();
		
		
	}
	
	/* 프로그램 속도 조절  usleep메소드는 usleep(int micro_seconds) 따라서 usleep(백만)= 1초 후 다음 작업 실행
	 * 즉, usleep(백만)은 1초의 딜레이를 준다는 것 우리는 0.5초의 틱을 준다면 50만으로 초기설정	 
	 */
	
        usleep(del); // delay, while문이 1번하는데 딜레이 >>GetsFruit()과 MoveSnake()가 딜레이마다 실행
    }
    
	 
    
}
