#pragma once
#ifndef _Tetris
#define _Tetris

#include <random>
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <Windows.h>


class Block
{
public:
	int _currentX, _currentY;
	int _precurrentX;
	int _shadowY = 1;
	int _currentBlockState;
	int _nextBlockState;
	int _currentRotateState;
	int clearline=0;


	struct Point { int _blockX, _blockY; };

	Point _blockShape[7][4][4] =//블록 모양 결정 y축은 위쪽이 마이너스
	{

		{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },//일자 긴거

		{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },//네모난거

		{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },//└┐

		{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },//┌┘

		{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },//└─

		{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },//─┘

		{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },//ㅜ

	};


public:

	void InitializeBlockState();//내려올 블럭 처음 위치 설정

	int getRandomNumber(int min, int max);//랜덤 함수 

	void gotoxy(int x, int y);//지정한 위치로 커서 이동

	void PrintOutBoard();//기본판 출력

	void PrintInBoard();//안쪽 게임판 재출력

	void PrintBlock(bool isBlock);//블럭 찍어내기

	int CheckBlockAround(int cx, int cy, int rot);//요긴 소스보면서 봐야할듯



	int getCurrentX();

	int getCurrentY();

	int getCurrentRotateState();

	int getCurrentBlockState();

	int getNextBlockState();

	int getCurrentShadowY();




	void setCurrentBlockState(int c);

	void setNextBlockState(int n);



	bool isMoveDown();//못내려가면 true,내려갈수있으면 false

	bool isInputKey();

	void FullLineDelete();//줄이 다 차면 지워줌



	void PrintNextBlockBox();

	void PrintNextBlock(bool isBlock);

	void EmptyNextBlockBox();



	void PrintShadowBlock(bool isBlock);
};

#endif