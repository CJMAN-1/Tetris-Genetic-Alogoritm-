#pragma once
#ifndef _Tetris
#define _Tetris

#include <random>
//#include <conio.h>
#include <Windows.h>
#include "stack.h"

class Block
{
private:
	int _currentX, _currentY;
	int _shadowY = 20;
	int _currentBlockState;//현재 블록의 모양
	int _nextBlockState;
	int _currentRotateState;//현재 블록의 회전상태
	int _precurrentX;
	int _clearlinecount =0;
	int left, right;
	int __blockheight;
	int __obstruct;
	int __space;
	int __fulllinecount;
	int __contactfloor;
	int __contactside;
	int __contactblock;

	//
	StackNode emptynode;//그냥 빈노드
	StackNode topnode;//하나의 블럭에서 가장높은 적합도를 가진 노드
	StackNode* resultnode = nullptr;//하나의 블럭에서 적합도를 계산한 결과를 저장한 노드

	mStack buffer;//하나의 블럭을 놓을때 계산결과를 저장하는 스택

	int rotatecount[7] = {2,1,2,2,4,4,4 };
	struct Point { int _blockX, _blockY; };
	Point _blockShape[7][4][4] =
	{
		{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },//일자긴거
		{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },//네모난거
		{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },//지렁
		{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },//지렁
		{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },//기역
		{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },//니은
		{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },//뻐큐
	};
	

public:
	double gene[7] = { 0, };
	StackNode tempnode;//얘가 계산을 수행할때 가장 많이 쓰임
	//내가 만든소스
	//객체 시작할때 설정
	void initializeobj();
	int getclearlinecount();
	//블럭이 다른블럭과 닿는 면의 갯수
	void calcontactblock();
	//블럭이 옆벽과 닿는 면의 갯수
	void calcontactside();
	//블럭이 바닥과 닿는 면의 갯수
	void calcontactfloor();
	//블럭을 놓았을때 완성되는 줄의 갯수를 구함
	void calfullline(int y);
	//블럭 사이의 빈공간의 개수를 구함
	void calobstruct(int i, int j);
	//블럭 높이의 합을 구함
	void calblockheight();
	//임시 스택노드 초기화
	void cleargood();
	//적합도 임시 스택노드에 저장
	void makegood();
	//적합도 출력
	void showgoodness();
	//블럭을 가장왼쪽으로 옮김
	void gotoleft();
	//블럭의 가장 왼쪽 반환
	int getleft();
	//블럭의 가장 오른쪽 반환
	int getright();
	//오른쪽으로 한칸씩 움직임
	void moveright();
	//블럭을 하나씩 돌림!
	void rotateblock();
	//
	void InitializeBlockState();
	int getRandomNumber(int min, int max);
	void gotoxy(int x, int y);
	void PrintOutBoard();
	void PrintInBoard();
	void PrintBlock(bool isBlock);

	int CheckBlockAround(int cx, int cy, int rot);

	int getCurrentX();
	int getCurrentY();
	int getCurrentRotateState();
	int getCurrentBlockState();
	int getNextBlockState();

	void setCurrentBlockState(int c);
	void setNextBlockState(int n);


	bool isMoveDown();
//	bool isInputKey();
	void FullLineDelete();

	void PrintNextBlockBox();
	void PrintNextBlock(bool isBlock);
	void EmptyNextBlockBox();

	void PrintShadowBlock(bool isBlock);

};

#endif