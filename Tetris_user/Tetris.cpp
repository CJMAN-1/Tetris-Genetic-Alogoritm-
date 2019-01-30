#include "Tetris.h"

using namespace std;
//블럭 높이의 합 구하기 완료
enum : int

{

	LEFT = 75,

	RIGHT = 77,

	UP = 72,

	DOWN = 80,

	SPACE = 32

};

enum : int { EMPTY, BLOCK, WALL, SHADOW };

namespace//경기장
{
	const int _boardWidth = 10;
	const int _boardHeight = 20;
	int board[_boardWidth + 2][_boardHeight + 2];
	int board2[_boardWidth + 2][_boardHeight + 2];
	const char* _arrayTile[] = { ".", "■","□","▩" };

}

void Block::gotoxy(int x, int y)//지정한 위치로 커서 이동
{
	COORD pos = { x,y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Block::InitializeBlockState()//내려올 블럭 처음 위치 설정
{
	_currentX = _boardWidth / 2;//가운데 놓고
	_currentY = 3;//중심축의 높이가 3
	_currentRotateState = 0;//회전 상태는 0번째
}

int Block::getRandomNumber(int min, int max)//랜덤 함수
{
	std::random_device randomNumber;
	std::mt19937_64 randomNumberDevice(randomNumber());
	std::uniform_int_distribution<int> range(min, max);

	return range(randomNumberDevice);
}

void Block::PrintOutBoard()//기본판 출력
{
	for (int i = 0; i < _boardWidth + 2; i++)//벽과 빈공간 구분할 이름 부여
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

	for (int i = 0; i < _boardWidth + 2; i++)//벽과 빈공간(.) 출력
		for (int j = 0; j < _boardHeight + 2; j++)
		{
			gotoxy(i * 2, j);//특수문자는 2칸, 점은 1칸 차지하므로!
			puts(_arrayTile[board[i][j]]);
		}
}

void Block::PrintInBoard()//안쪽 게임판 입력된 상태에 따라 재출력
{
	for (int i = 1; i < _boardWidth + 1; i++)
		for (int j = 1; j < _boardHeight + 1; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[board[i][j]]);
		}

}

void Block::PrintBlock(bool isBlock)//블럭모양에 블럭을 놓을지 빈공간을 넣을지 결정!
{
	for (int i = 0; i < 4; i++)
	{
		gotoxy((_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX) * 2,
			_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY);
		puts(_arrayTile[isBlock ? BLOCK : EMPTY]);
	}
}

int Block::CheckBlockAround(int cx, int cy, int rot)//한칸 밑에 블럭,벽이 있는지 빈칸인지 체크
{
	int k = EMPTY;

	for (int i = 0; i < 4; i++)
		k = max(k,//한칸밑에 블럭이 있으면 k=1,밑에 벽이있으면 k=2;
			board[_blockShape[_currentBlockState][rot][i]._blockX + cx]
			[_blockShape[_currentBlockState][rot][i]._blockY + cy]);

	return k;
}//요거 다시 봐야 할듯


int Block::getCurrentX() { return _currentX; }
int Block::getCurrentY() { return _currentY; }
int Block::getCurrentRotateState() { return _currentRotateState; }
int Block::getCurrentBlockState() { return _currentBlockState; }
int Block::getNextBlockState() { return _nextBlockState; }
int Block::getCurrentShadowY() { return _shadowY; }

void Block::setNextBlockState(int n) { _nextBlockState = n; }
void Block::setCurrentBlockState(int c) { _currentBlockState = c; }


bool Block::isMoveDown()//못내려가면 true,내려갈수있으면 false//내려가는것도 실행
{
	if (CheckBlockAround(_currentX, _currentY + 1, _currentRotateState) != EMPTY) {//
		FullLineDelete();

		return true;
	}

	PrintBlock(false);//찍혀있던 블럭 지우고
	_currentY++;//한칸 내리고
	PrintBlock(true);//다시 찍어내고

	return false;
}

bool Block::isInputKey()//true: false: 로테이트부분 이상하지만 스페이스만 써서 하면 별 문제 없을것 같음
{
	int ch, trot;

	if (kbhit()) {//kbhit:키보드 입력이 들어오면 1 아니면 0
		ch = getch();//getch:키보드 입력을 받음 입력 들어올때 까지 기다림
	

		if (ch == 224 || ch == 0) {//방향키는 특수키로 인식이 되어 누르면 224와 75 같이 2개 입력됨
			ch = getch();
			switch (ch) {
			case LEFT:
				if (CheckBlockAround(_currentX - 1, _currentY, _currentRotateState) == EMPTY) {
					PrintBlock(false);//지우고
					PrintShadowBlock(false);
					_currentX--;//옮기고
					PrintBlock(true);//찍고
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

			case UP://로테이트 부분 수정해야함! 돌릴수 없는부분에서 돌아감
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
				if (isMoveDown())//내려갈곳이없으면
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

void Block::FullLineDelete()//줄이 다 차면 지워줌
{
	for (int i = 0; i < 4; i++) {//게임판의 블럭이 내린 위치를 블럭으로 칠함
		board[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY] = BLOCK;

		board2[_blockShape[_currentBlockState][_currentRotateState][i]._blockX + _currentX]
			[_blockShape[_currentBlockState][_currentRotateState][i]._blockY + _currentY] = BLOCK;

	}
	int x, y, ty;

	for (y = 1; y < _boardHeight + 1; y++) {
		for (x = 1; x < _boardWidth + 1; x++)//한줄을 검사해서 빈공간 있으면 다음줄로 넘어감
			if (board[x][y] != BLOCK)
				break;

		if (x == _boardWidth + 1) {//마지막 칸까지 빈공간이없다! 
			for (ty = y; ty > 1; ty--)
				for (x = 1; x < _boardWidth + 1; x++) {
					board[x][ty] = board[x][ty - 1];
					board2[x][ty] = board[x][ty - 1];
				}


			PrintInBoard();
			clearline++;
			Sleep(200);//줄이없어지고 0.2초 대기
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

void Block::EmptyNextBlockBox()//다음 블럭 박스 초기화
{
	for (int i = 19; i < 26; i++)
		for (int j = 13; j < 17; j++) {
			gotoxy(i * 2, j);
			puts(_arrayTile[EMPTY]);
		}
}

void Block::PrintShadowBlock(bool isBlock)//else 쪽 잘 이해 안됌
{// 그림자 놓는 위치판별이 잘못됌
	_shadowY = 3;

	while (CheckBlockAround(_currentX, _shadowY + 1, _currentRotateState) == EMPTY) {
		_shadowY++;
	}
	/*
	if (CheckBlockAround(_currentX, _shadowY + 1, _currentRotateState) == EMPTY) {//한칸 아래가 비었으면 하나더 내린다
		_shadowY++;

		return;
	}

	if (CheckBlockAround(_currentX, _shadowY, _currentRotateState) != EMPTY) {//지금위치가 비어있지않으면 올린다
		_shadowY--;

		return;
	}
*/
	//여기부터 다 
	for (int i = 0; i < 4; i++) {
		gotoxy((_currentX + _blockShape[_currentBlockState][_currentRotateState][i]._blockX) * 2,
			_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY);

		if (_shadowY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY//같을경우  블럭으로처리
			== _currentY + _blockShape[_currentBlockState][_currentRotateState][i]._blockY)
			puts(_arrayTile[BLOCK]);

		else
			puts(_arrayTile[isBlock ? SHADOW : EMPTY]);
	}
}