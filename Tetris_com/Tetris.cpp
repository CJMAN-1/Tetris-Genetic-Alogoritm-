#include "Tetris.h"
//8.22 블럭높이,빈공간 막고있는블럭갯수, 테트리스 오류 몇가지 고침
//8.23 빈공간 막고있는 블럭갯수 판별 수정, 빈공간의 갯수 ,완성되는 줄의 갯수는 제대로 계산하는지 확인을 못해봄!
//	   블럭이 바닥과 닿는면의 갯수, 블럭이 좌우 벽과 닿는면의 갯수, 옆의 블럭과 닿는 면의갯수 평가 기준 다 짬!
//8.24 스택구조 손보기,적합도 가장 높은곳에 블럭 놓기까지 했는데 놓는곳이 1번째 자리일때 게임이 끝나버림
enum : int
{
	LEFT = 75,
	RIGHT = 77,
	UP = 72,
	DOWN = 80,
	SPACE = 32
};

enum : int { EMPTY, BLOCK, WALL, SHADOW ,CENTER};

namespace
{
	const int _boardWidth = 10;
	const int _boardHeight = 20;
	int board[_boardWidth + 2][_boardHeight + 2];
	const char* _arrayTile[] = { ".", "■","□","▩","Ⅱ" };
}

//내가만든 소스

void Block::initializeobj() {
	for (int i = 0; i < 7; i++) {
		tempnode.criteria.gene[i] = gene[i];
	}
	_clearlinecount = 0;
	
}

int Block::getclearlinecount() {
	return _clearlinecount;
}
void Block::calcontactblock()
{
	__contactblock = 0;

	for (int i = 0; i < 4; i++) {
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX + 1]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY] == BLOCK)
			__contactblock++;
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX - 1]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY] == BLOCK)
			__contactblock++;
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY + 1] == BLOCK)
			__contactblock++;
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY - 1] == BLOCK)
			__contactblock++;
	}
}
void Block::calcontactside()
{
	__contactside = 0;


	for (int i = 0; i < 4; i++) {
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX + 1]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY] == WALL)
			__contactside++;

		else if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX - 1]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY] == WALL)
			__contactside++;
	}
}
void Block::calcontactfloor()
{
	__contactfloor = 0;

	for (int i = 0; i < 4; i++) {
		if (board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _shadowY + 1] == WALL)
			__contactfloor++;
	}
	
}

//완성되는 줄의 갯수는 제대로 계산하는지 확인을 못해봄!
void Block::calfullline(int y)
{
	int x = 0;

	for (y = 2; y < _boardHeight + 1; y++) {
		for (x = 1; x < _boardWidth + 1; x++)
			if (board[x][y]==EMPTY || board[x][y]==WALL)
				break;
		if(x==_boardWidth+1)//게임판 끝까지 갔을때
			__fulllinecount++;
	}
}

void Block::cleargood()
{
	tempnode.criteria.blockheight = 0;
	tempnode.criteria.obstruct = 0;
	tempnode.criteria.space = 0;
	tempnode.criteria.fulllinecount = 0;
	tempnode.criteria.contactfloor = 0;
	tempnode.criteria.contactside = 0;
	tempnode.criteria.contactblock = 0;
	tempnode._currentX = 0;
	tempnode._currentrotatestate = 0;
	tempnode.clearlinecount = 0;
}
void Block::makegood()
{
	tempnode.criteria.blockheight = __blockheight;
	tempnode.criteria.obstruct = __obstruct;
	tempnode.criteria.space = __space;
	tempnode.criteria.fulllinecount = __fulllinecount;
	tempnode.criteria.contactfloor = __contactfloor;
	tempnode.criteria.contactside = __contactside;
	tempnode.criteria.contactblock = __contactblock;
	tempnode._currentX = _currentX;
	tempnode._currentrotatestate = _currentRotateState;
	tempnode.goodnesssum();
	tempnode.clearlinecount = _clearlinecount;
}
void Block::showgoodness()
{
/*	gotoxy(24, 0);
	cout << "쌓이는 블럭의 높이 :" << tempnode.criteria.blockheight << "  ";
	gotoxy(24, 1);
	cout << "빈공간을 막고있는 블럭의 갯수 :" << tempnode.criteria.obstruct << "  ";
	gotoxy(24, 2);
	cout << "블럭사이의 빈공간의 갯수 :" << tempnode.criteria.space << "  ";
	gotoxy(24, 3);
	cout << "완성되는 줄의 갯수 :" << tempnode.criteria.fulllinecount << "  ";
	gotoxy(24, 4);
	cout << "바닥과 블럭이 접촉하는 면의 갯수 :" << tempnode.criteria.contactfloor << "  ";
	gotoxy(24, 5);
	cout << "좌우 벽면과 블럭이 접촉하는 면의 갯수 :" << tempnode.criteria.contactside << "  ";
	gotoxy(24, 6);
	cout << "기존블럭들과 현재 블럭이 접촉하는 면의 갯수 :" << tempnode.criteria.contactblock << "  ";
	gotoxy(24, 7);
	cout << "현재블럭위치의 점수 합 :" << tempnode.goodsum << "  ";
	gotoxy(24, 8);
	cout << "스택에 쌓인 노드의 갯수 :" << buffer.getnodecount() << "  ";
*/	gotoxy(24, 0);
	cout << "클리어한 라인" << tempnode.clearlinecount;

}

void Block::calobstruct(int i, int j)
{
	int initial = j;
	int a = 0;//i가 바뀌는지를 판별하는 변수
	for (j; j < 21; j++) {
		if (board[i][j] == EMPTY) {
			if (a == 0) {
				__obstruct += j - initial;
				a++;
			}				
			__space++;
		}
	}
}

void Block::calblockheight()//높이, 빈공간, 빈공간 막는 블럭 계산 한꺼번에 함
{
	int width = 0;
	int start = 0;
	int j;

	width = getright() - getleft() + 1;
	start = getleft() + _currentX - 1;

	__blockheight = 0;
	__obstruct = 0;
	__space = 0;
	__fulllinecount = 0;

	for (int i = 0; i < width; i++) {
		start ++;
		for (j = 1; j < 21; j++) {
			if (board[start][j] == SHADOW) {
				calobstruct(start, j);

				if (i == 0) {
					calfullline(j);
				}

				break;
			}
		}
		__blockheight = __blockheight >= 20 - (j - 1) ? __blockheight : 20 - (j - 1);
	}
	return;
}

void Block::rotateblock() {

	for (int i = 0; i < rotatecount[_currentBlockState]; i++) {

		

		_currentX = 5;//블럭을 가운데로 옮김
		moveright();
		if (!strcmp(flag, "finish")) {
			break;
		}
		_currentX--;

		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//종료조건
			break;

		_currentRotateState>3?0:_currentRotateState++;
	}
	_currentRotateState--;

	//적합도 저장한 스택을 꺼내서 가장높은 적합도를 가진 자리 찾는 부분
	topnode.goodsum = -100;//아무노드나 가리키게 지정해놓은것
	while (!buffer.isEmpty()) {
		resultnode = buffer.pop_top();
		if (topnode.goodsum < resultnode->goodsum) {
			topnode = *resultnode;
		}
		delete resultnode;
	}
	//여기부터 가장 높은 적합도를 가진 자리로 이동해서 블럭을 놓음
	_currentX = topnode._currentX;
	_currentRotateState = topnode._currentrotatestate;
	//블럭 내리는 구간
	PrintBlock(true);
	PrintShadowBlock(true);
	Sleep(50);
	PrintBlock(false);
	PrintShadowBlock(false);
	while (isMoveDown() == false);//이부분에서 줄이 먼저 지워지고 그다음에 블럭이 내려감 그래서 줄이 지워지고난후 블럭 그려짐
	PrintBlock(true);
	FullLineDelete();
	_currentY = 3;
	_shadowY = 3;
	//여기까지가 블럭 내림
	return;
}
void Block::moveright() {
	gotoleft();

	while (_currentX + getright()<11) {//맨 왼쪽에서 오른쪽으로 하나씩 옮겨감
		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//옆에 갈곳이 없으면 브레이크
			break;
		
		PrintBlock(true);
		PrintShadowBlock(true);
		
		file = fopen("flag.txt", "r");
		fscanf(file, "%s", flag);
		fclose(file);

		if (!strcmp(flag, "finish")) {
			gotoxy(5, 10);
			cout << "I  A M  L O S E R ...  :(";
			break;
		}

		cleargood();//
		//적합도 계산 구간
		calblockheight();
		calcontactfloor();
		calcontactside();
		calcontactblock();
		//여기까지 적합도 계산구간
		makegood();

		buffer.insert_top(&tempnode);

		showgoodness();
		Sleep(10);
		PrintBlock(false);
		PrintShadowBlock(false);
		

		_currentX++;
		
	}

	return;
}


void Block::gotoleft() {

	while (_currentX + getleft() > 1) {
		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//옆에 갈곳이 없으면 브레이크
			break;

		_currentX--;
	}
		
	return;
}

int Block::getleft() {
	left = 5;
	for (int i = 0; i < 4; i++) {
		left = _blockShape[_currentBlockState][_currentRotateState][i]._blockX < left ?
			_blockShape[_currentBlockState][_currentRotateState][i]._blockX : left;
	}

	return left;
}

int Block::getright() {
	right = -5;

	for (int i = 0; i < 4; i++) {
		right = _blockShape[_currentBlockState][_currentRotateState][i]._blockX > right ?
			_blockShape[_currentBlockState][_currentRotateState][i]._blockX : right;
	}

	return right;
}
//여기까지가 


void Block::gotoxy(int x, int y)
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//블록을 처음 위치에 가져다 놓고 회전하지 않은 상태로둠 
void Block::InitializeBlockState()
{
	_currentX = _boardWidth / 2;
	_currentY = 3;
	_currentRotateState = 0;
}

int Block::getRandomNumber(int min, int max)
{
	if (min >= max) {
		max = min + 1;//max랑 min이랑 같으면 에러가 남
	}
	std::random_device randomNumber;
	std::mt19937_64 randomNumberDevice(randomNumber());
	std::uniform_int_distribution<int> range(min, max);

	return range(randomNumberDevice);
}

void Block::PrintOutBoard()
{
	for (int i = 0; i < _boardWidth + 2; i++)
		for (int j = 0; j < _boardHeight + 2; j++) {
			if (i == 0 || i == _boardWidth + 1 || j == 0 || j == _boardHeight + 1)
				board[i][j] = WALL;

			else
				board[i][j] = EMPTY;
		}

	for (int i = 0; i < _boardWidth + 2; i++)
		for (int j = 0; j < _boardHeight + 2; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[board[i][j]]);
		}
}

void Block::PrintInBoard()
{
	for (int i = 1; i < _boardWidth + 1; i++)
		for (int j = 1; j < _boardHeight + 1; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[board[i][j]]);
		}
}

void Block::PrintBlock(bool isBlock)
{
	for (int i = 0; i < 4; i++) {
		gotoxy((_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX) * 2,
			_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY);
//		if (i==0) {
	//		puts(_arrayTile[isBlock ? CENTER:EMPTY]);
	//	}
	//	else
		puts(_arrayTile[isBlock ? BLOCK : EMPTY]);
	}
}

int Block::CheckBlockAround(int cx, int cy, int rot)
{
	int k = EMPTY;
	for (int i = 0; i < 4; i++)
		k = max(k,
			board[_blockShape[_currentBlockState][rot][i]._blockX + cx]
			[_blockShape[_currentBlockState][rot][i]._blockY + cy]);

	return k;
}

int Block::getCurrentX() { return _currentX; }
int Block::getCurrentY() { return _currentY; }
int Block::getCurrentRotateState() { return _currentRotateState; }
int Block::getCurrentBlockState() { return _currentBlockState; }
int Block::getNextBlockState() { return _nextBlockState; }
void Block::setNextBlockState(int n) { _nextBlockState = n; }
void Block::setCurrentBlockState(int c) { _currentBlockState = c; }

bool Block::isMoveDown()
{
	if (CheckBlockAround(_currentX, _currentY + 1, _currentRotateState) != EMPTY) {
//		FullLineDelete();
		return true;
	}

//	PrintBlock(false);
	_currentY++;
//	PrintBlock(true);

	return false;
}
/*
bool Block::isInputKey()
{
	int ch, trot;

	if (kbhit())	{
		ch = getch();

		if (ch == 224 || ch == 0)		{
			ch = getch();
			switch (ch)			{
		case LEFT:
		if (CheckBlockAround(_currentX - 1, _currentY, _currentRotateState) == EMPTY)				{
		PrintBlock(false);
		_currentX--;
		PrintBlock(true);
		}
		break;

		case RIGHT:
		if (CheckBlockAround(_currentX + 1, _currentY, _currentRotateState) == EMPTY)				{
		PrintBlock(false);
		_currentX++;
		PrintBlock(true);
		}
		break;

		case UP:
		trot = (_currentRotateState == 3) ? 0 : _currentRotateState + 1;
		if (CheckBlockAround(_currentX, _currentY, trot) == EMPTY)				{
		PrintBlock(false);
		_currentRotateState = trot;
		PrintBlock(true);
		}

		else if (CheckBlockAround(_currentX + 1, _currentY, trot) != EMPTY)		{
		PrintBlock(false);
		_currentX--;

		if (getCurrentBlockState() == 0)
		_currentX--;

		_currentRotateState = trot;
		PrintBlock(true);
		}

		else if (CheckBlockAround(_currentX - 1, _currentY, trot) != EMPTY)				{
		PrintBlock(false);
		_currentX++;
		_currentRotateState = trot;
		PrintBlock(true);
		}
		break;

		case DOWN:
		if (isMoveDown())
		return true;

		break;
		}
		}

		else
		switch (ch)			{
		case SPACE:
		while (isMoveDown() == false);
		return true;
		}
	}

	return false;
}
*/


void Block::FullLineDelete()
{
	for (int i = 0; i < 4; i++)
		board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
		[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY] = BLOCK;

	int x, y, ty;
	for (y = 1; y < _boardHeight + 1; y++) {
		for (x = 1; x < _boardWidth + 1; x++)
			if (board[x][y] != BLOCK)
				break;

		if (x == _boardWidth + 1) {
			for (ty = y; ty > 1; ty--)
				for (x = 1; x < _boardWidth + 1; x++)
					board[x][ty] = board[x][ty - 1];
//			Sleep(300);
			_clearlinecount++;
			PrintInBoard();

		}
	}
}

void Block::PrintNextBlockBox() {
	for (int i = 18; i < 27; i++)
		for (int j = 12; j < 18; j++) {
			gotoxy(i * 2, j);

			if (i == 18 || i == 26 || j == 12 || j == 17)
				puts(_arrayTile[WALL]);

			else
				puts(_arrayTile[EMPTY]);
		}
}

void Block::PrintNextBlock(bool isBlock) {
	for (int i = 0; i < 4; i++) {
		gotoxy((22 + _blockShape[_nextBlockState][_currentRotateState][i]._blockX) * 2,
			15 + _blockShape[_nextBlockState][_currentRotateState][i]._blockY);
		puts(_arrayTile[isBlock ? BLOCK : EMPTY]);
	}
}

//다음 블록나오는 박스를 지운다
void Block::EmptyNextBlockBox()
{
	for (int i = 19; i < 26; i++)
		for (int j = 13; j < 17; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[EMPTY]);
		}
}


void Block::PrintShadowBlock(bool isBlock) {
	
	if (_currentX != _precurrentX) {
		_shadowY = 3;
	}
	_precurrentX = _currentX;

	while (CheckBlockAround(_currentX, _shadowY + 1, _currentRotateState) == EMPTY) {
		_shadowY++;
	}

//	if (CheckBlockAround(_currentX, _shadowY, _currentRotateState) != EMPTY) {
//		_shadowY--;
//		return;
//	}

	for (int i = 0; i < 4; i++) {
		gotoxy((_currentX + _blockShape[_currentBlockState][_currentRotateState][i]._blockX) * 2,
			_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY);

/*		if (_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY//같으면 블럭으로 표시
			== _currentY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY)
			puts(_arrayTile[BLOCK]);
		*/
	
		if (isBlock == true) {
			board[_currentX + _blockShape[_currentBlockState][_currentRotateState][i]._blockX][_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY] =SHADOW;
			puts(_arrayTile[SHADOW]);
		}
			

		else {
			board[_currentX + _blockShape[_currentBlockState][_currentRotateState][i]._blockX][_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY] = EMPTY;
			puts(_arrayTile[EMPTY]);
		}
		
			
	}
}