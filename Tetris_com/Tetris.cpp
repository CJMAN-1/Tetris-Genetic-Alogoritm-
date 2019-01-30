#include "Tetris.h"
//8.22 ������,����� �����ִº�����, ��Ʈ���� ���� ��� ��ħ
//8.23 ����� �����ִ� ������ �Ǻ� ����, ������� ���� ,�ϼ��Ǵ� ���� ������ ����� ����ϴ��� Ȯ���� ���غ�!
//	   ���� �ٴڰ� ��¸��� ����, ���� �¿� ���� ��¸��� ����, ���� ���� ��� ���ǰ��� �� ���� �� «!
//8.24 ���ñ��� �պ���,���յ� ���� �������� �� ������� �ߴµ� ���°��� 1��° �ڸ��϶� ������ ��������
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
	const char* _arrayTile[] = { ".", "��","��","��","��" };
}

//�������� �ҽ�

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

//�ϼ��Ǵ� ���� ������ ����� ����ϴ��� Ȯ���� ���غ�!
void Block::calfullline(int y)
{
	int x = 0;

	for (y = 2; y < _boardHeight + 1; y++) {
		for (x = 1; x < _boardWidth + 1; x++)
			if (board[x][y]==EMPTY || board[x][y]==WALL)
				break;
		if(x==_boardWidth+1)//������ ������ ������
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
	cout << "���̴� ���� ���� :" << tempnode.criteria.blockheight << "  ";
	gotoxy(24, 1);
	cout << "������� �����ִ� ���� ���� :" << tempnode.criteria.obstruct << "  ";
	gotoxy(24, 2);
	cout << "�������� ������� ���� :" << tempnode.criteria.space << "  ";
	gotoxy(24, 3);
	cout << "�ϼ��Ǵ� ���� ���� :" << tempnode.criteria.fulllinecount << "  ";
	gotoxy(24, 4);
	cout << "�ٴڰ� ���� �����ϴ� ���� ���� :" << tempnode.criteria.contactfloor << "  ";
	gotoxy(24, 5);
	cout << "�¿� ����� ���� �����ϴ� ���� ���� :" << tempnode.criteria.contactside << "  ";
	gotoxy(24, 6);
	cout << "��������� ���� ���� �����ϴ� ���� ���� :" << tempnode.criteria.contactblock << "  ";
	gotoxy(24, 7);
	cout << "�������ġ�� ���� �� :" << tempnode.goodsum << "  ";
	gotoxy(24, 8);
	cout << "���ÿ� ���� ����� ���� :" << buffer.getnodecount() << "  ";
*/	gotoxy(24, 0);
	cout << "Ŭ������ ����" << tempnode.clearlinecount;

}

void Block::calobstruct(int i, int j)
{
	int initial = j;
	int a = 0;//i�� �ٲ������ �Ǻ��ϴ� ����
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

void Block::calblockheight()//����, �����, ����� ���� �� ��� �Ѳ����� ��
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

		

		_currentX = 5;//���� ����� �ű�
		moveright();
		if (!strcmp(flag, "finish")) {
			break;
		}
		_currentX--;

		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//��������
			break;

		_currentRotateState>3?0:_currentRotateState++;
	}
	_currentRotateState--;

	//���յ� ������ ������ ������ ������� ���յ��� ���� �ڸ� ã�� �κ�
	topnode.goodsum = -100;//�ƹ���峪 ����Ű�� �����س�����
	while (!buffer.isEmpty()) {
		resultnode = buffer.pop_top();
		if (topnode.goodsum < resultnode->goodsum) {
			topnode = *resultnode;
		}
		delete resultnode;
	}
	//������� ���� ���� ���յ��� ���� �ڸ��� �̵��ؼ� ���� ����
	_currentX = topnode._currentX;
	_currentRotateState = topnode._currentrotatestate;
	//�� ������ ����
	PrintBlock(true);
	PrintShadowBlock(true);
	Sleep(50);
	PrintBlock(false);
	PrintShadowBlock(false);
	while (isMoveDown() == false);//�̺κп��� ���� ���� �������� �״����� ���� ������ �׷��� ���� ���������� �� �׷���
	PrintBlock(true);
	FullLineDelete();
	_currentY = 3;
	_shadowY = 3;
	//��������� �� ����
	return;
}
void Block::moveright() {
	gotoleft();

	while (_currentX + getright()<11) {//�� ���ʿ��� ���������� �ϳ��� �Űܰ�
		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//���� ������ ������ �극��ũ
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
		//���յ� ��� ����
		calblockheight();
		calcontactfloor();
		calcontactside();
		calcontactblock();
		//������� ���յ� ��걸��
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
		if (CheckBlockAround(_currentX, _currentY, _currentRotateState))//���� ������ ������ �극��ũ
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
//��������� 


void Block::gotoxy(int x, int y)
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//����� ó�� ��ġ�� ������ ���� ȸ������ ���� ���·ε� 
void Block::InitializeBlockState()
{
	_currentX = _boardWidth / 2;
	_currentY = 3;
	_currentRotateState = 0;
}

int Block::getRandomNumber(int min, int max)
{
	if (min >= max) {
		max = min + 1;//max�� min�̶� ������ ������ ��
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

//���� ��ϳ����� �ڽ��� �����
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

/*		if (_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY//������ ������ ǥ��
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