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

	Point _blockShape[7][4][4] =//��� ��� ���� y���� ������ ���̳ʽ�
	{

		{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },//���� ���

		{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },//�׸𳭰�

		{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },//����

		{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },//����

		{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },//����

		{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },//����

		{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },//��

	};


public:

	void InitializeBlockState();//������ �� ó�� ��ġ ����

	int getRandomNumber(int min, int max);//���� �Լ� 

	void gotoxy(int x, int y);//������ ��ġ�� Ŀ�� �̵�

	void PrintOutBoard();//�⺻�� ���

	void PrintInBoard();//���� ������ �����

	void PrintBlock(bool isBlock);//�� ����

	int CheckBlockAround(int cx, int cy, int rot);//��� �ҽ����鼭 �����ҵ�



	int getCurrentX();

	int getCurrentY();

	int getCurrentRotateState();

	int getCurrentBlockState();

	int getNextBlockState();

	int getCurrentShadowY();




	void setCurrentBlockState(int c);

	void setNextBlockState(int n);



	bool isMoveDown();//���������� true,�������������� false

	bool isInputKey();

	void FullLineDelete();//���� �� ���� ������



	void PrintNextBlockBox();

	void PrintNextBlock(bool isBlock);

	void EmptyNextBlockBox();



	void PrintShadowBlock(bool isBlock);
};

#endif