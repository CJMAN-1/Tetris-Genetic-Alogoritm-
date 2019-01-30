#include "Tetris.h"

using namespace std;
//�� ������ �� ���ϱ� �Ϸ�
enum : int

{

	LEFT = 75,

	RIGHT = 77,

	UP = 72,

	DOWN = 80,

	SPACE = 32

};

enum : int { EMPTY, BLOCK, WALL, SHADOW };

namespace//�����
{
	const int _boardWidth = 10;
	const int _boardHeight = 20;
	int board[_boardWidth + 2][_boardHeight + 2];
	int board2[_boardWidth + 2][_boardHeight + 2];
	const char* _arrayTile[] = { ".", "��","��","��" };

}

void Block::gotoxy(int x, int y)//������ ��ġ�� Ŀ�� �̵�
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Block::InitializeBlockState()//������ �� ó�� ��ġ ����
{
	_currentX = _boardWidth / 2;//��� ����
	_currentY = 3;//�߽����� ���̰� 3
	_currentRotateState = 0;//ȸ�� ���´� 0��°
}

int Block::getRandomNumber(int min, int max)//���� �Լ�
{
	std::random_device randomNumber;
	std::mt19937_64 randomNumberDevice(randomNumber());
	std::uniform_int_distribution<int> range(min, max);

	return range(randomNumberDevice);
}

void Block::PrintOutBoard()//�⺻�� ���
{
	for (int i = 0; i < _boardWidth + 2; i++)//���� ����� ������ �̸� �ο�
		for (int j = 0; j < _boardHeight + 2; j++)
		{
			if (i == 0 || i == _boardWidth + 1 || j == 0 || j == _boardHeight + 1) {
				board[i][j] = WALL;
				board2[i][j] = board[i][j];
			}


			else {
				board[i][j] = EMPTY;
				board2[i][j] = board[i][j];
			}

		}

	for (int i = 0; i < _boardWidth + 2; i++)//���� �����(.) ���
		for (int j = 0; j < _boardHeight + 2; j++)
		{
			gotoxy(i * 2, j);//Ư�����ڴ� 2ĭ, ���� 1ĭ �����ϹǷ�!
			puts(_arrayTile[board[i][j]]);
		}
}

void Block::PrintInBoard()//���� ������ �Էµ� ���¿� ���� �����
{
	for (int i = 1; i < _boardWidth + 1; i++)
		for (int j = 1; j < _boardHeight + 1; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[board[i][j]]);
		}

}

void Block::PrintBlock(bool isBlock)//����翡 ���� ������ ������� ������ ����!
{
	for (int i = 0; i < 4; i++)
	{
		gotoxy((_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX) * 2,
			_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY);
		puts(_arrayTile[isBlock ? BLOCK : EMPTY]);
	}
}

int Block::CheckBlockAround(int cx, int cy, int rot)//��ĭ �ؿ� ��,���� �ִ��� ��ĭ���� üũ
{
	int k = EMPTY;

	for (int i = 0; i < 4; i++)
		k = max(k,//��ĭ�ؿ� ���� ������ k=1,�ؿ� ���������� k=2;
			board[_blockShape[_currentBlockState][rot][i]._blockX + cx]
			[_blockShape[_currentBlockState][rot][i]._blockY + cy]);

	return k;
}//��� �ٽ� ���� �ҵ�


int Block::getCurrentX() { return _currentX; }
int Block::getCurrentY() { return _currentY; }
int Block::getCurrentRotateState() { return _currentRotateState; }
int Block::getCurrentBlockState() { return _currentBlockState; }
int Block::getNextBlockState() { return _nextBlockState; }
int Block::getCurrentShadowY() { return _shadowY; }

void Block::setNextBlockState(int n) { _nextBlockState = n; }
void Block::setCurrentBlockState(int c) { _currentBlockState = c; }


bool Block::isMoveDown()//���������� true,�������������� false//�������°͵� ����
{
	if (CheckBlockAround(_currentX, _currentY + 1, _currentRotateState) != EMPTY) {//
		FullLineDelete();

		return true;
	}

	PrintBlock(false);//�����ִ� �� �����
	_currentY++;//��ĭ ������
	PrintBlock(true);//�ٽ� ����

	return false;
}

bool Block::isInputKey()//true: false: ������Ʈ�κ� �̻������� �����̽��� �Ἥ �ϸ� �� ���� ������ ����
{
	int ch, trot;

	if (kbhit()) {//kbhit:Ű���� �Է��� ������ 1 �ƴϸ� 0
		ch = getch();//getch:Ű���� �Է��� ���� �Է� ���ö� ���� ��ٸ�
	

		if (ch == 224 || ch == 0) {//����Ű�� Ư��Ű�� �ν��� �Ǿ� ������ 224�� 75 ���� 2�� �Էµ�
			ch = getch();
			switch (ch) {
			case LEFT:
				if (CheckBlockAround(_currentX - 1, _currentY, _currentRotateState) == EMPTY) {
					PrintBlock(false);//�����
					PrintShadowBlock(false);
					_currentX--;//�ű��
					PrintBlock(true);//���
					PrintShadowBlock(true);
				}
				break;

			case RIGHT:

				if (CheckBlockAround(_currentX + 1, _currentY, _currentRotateState) == EMPTY) {
					PrintBlock(false);
					PrintShadowBlock(false);
					_currentX++;
					PrintBlock(true);
					PrintShadowBlock(true);
				}
				break;

			case UP://������Ʈ �κ� �����ؾ���! ������ ���ºκп��� ���ư�
				trot = (_currentRotateState == 3) ? 0 : _currentRotateState + 1;
				if (CheckBlockAround(_currentX, _currentY, trot) == EMPTY) {
					PrintBlock(false);
					PrintShadowBlock(false);
					_currentRotateState = trot;
					PrintBlock(true);
					PrintShadowBlock(true);
				}

				else if (CheckBlockAround(_currentX + 1, _currentY, trot) != EMPTY) {
					PrintBlock(false);
					PrintShadowBlock(false);
					_currentX--;

					if (getCurrentBlockState() == 0)
						_currentX--;

					_currentRotateState = trot;
					PrintBlock(true);
					PrintShadowBlock(true);
				}

				else if (CheckBlockAround(_currentX - 1, _currentY, trot) != EMPTY) {
					PrintBlock(false);
					PrintShadowBlock(false);
					_currentX++;
					_currentRotateState = trot;
					PrintBlock(true);
					PrintShadowBlock(true);
				}
				break;

			case DOWN:
				if (isMoveDown())//���������̾�����
					return true;
				break;

			}

		}

		else
			switch (ch) {
			case SPACE:
				while (isMoveDown() == false);
				return true;
			}
	}

	return false;

}

void Block::FullLineDelete()//���� �� ���� ������
{
	for (int i = 0; i < 4; i++) {//�������� ���� ���� ��ġ�� ������ ĥ��
		board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY] = BLOCK;

		board2[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY] = BLOCK;

	}
	int x, y, ty;

	for (y = 1; y < _boardHeight + 1; y++) {
		for (x = 1; x < _boardWidth + 1; x++)//������ �˻��ؼ� ����� ������ �����ٷ� �Ѿ
			if (board[x][y] != BLOCK)
				break;

		if (x == _boardWidth + 1) {//������ ĭ���� ������̾���! 
			for (ty = y; ty > 1; ty--)
				for (x = 1; x < _boardWidth + 1; x++) {
					board[x][ty] = board[x][ty - 1];
					board2[x][ty] = board[x][ty - 1];
				}


			PrintInBoard();
			clearline++;
			Sleep(200);//���̾������� 0.2�� ���
		}
	}
}

void Block::PrintNextBlockBox()
{
	for (int i = 18; i < 27; i++)
		for (int j = 12; j < 18; j++) {
			gotoxy(i * 2, j);
			if (i == 18 || i == 26 || j == 12 || j == 17)
				puts(_arrayTile[WALL]);

			else
				puts(_arrayTile[EMPTY]);
		}
}

void Block::PrintNextBlock(bool isBlock)
{
	for (int i = 0; i < 4; i++) {
		gotoxy((22 + _blockShape[_nextBlockState][_currentRotateState][i]._blockX) * 2,
			15 + _blockShape[_nextBlockState][_currentRotateState][i]._blockY);
		puts(_arrayTile[isBlock ? BLOCK : EMPTY]);
	}
}

void Block::EmptyNextBlockBox()//���� �� �ڽ� �ʱ�ȭ
{
	for (int i = 19; i < 26; i++)
		for (int j = 13; j < 17; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[EMPTY]);
		}
}

void Block::PrintShadowBlock(bool isBlock)//else �� �� ���� �ȉ�
{// �׸��� ���� ��ġ�Ǻ��� �߸���
	_shadowY = 3;

	while (CheckBlockAround(_currentX, _shadowY + 1, _currentRotateState) == EMPTY) {
		_shadowY++;
	}
	/*
	if (CheckBlockAround(_currentX, _shadowY + 1, _currentRotateState) == EMPTY) {//��ĭ �Ʒ��� ������� �ϳ��� ������
		_shadowY++;

		return;
	}

	if (CheckBlockAround(_currentX, _shadowY, _currentRotateState) != EMPTY) {//������ġ�� ������������� �ø���
		_shadowY--;

		return;
	}
*/
	//������� �� 
	for (int i = 0; i < 4; i++) {
		gotoxy((_currentX + _blockShape[_currentBlockState][_currentRotateState][i]._blockX) * 2,
			_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY);

		if (_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY//�������  ������ó��
			== _currentY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY)
			puts(_arrayTile[BLOCK]);

		else
			puts(_arrayTile[isBlock ? SHADOW : EMPTY]);
	}
}