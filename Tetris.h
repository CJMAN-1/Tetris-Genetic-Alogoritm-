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
	int _currentBlockState;//���� ����� ���
	int _nextBlockState;
	int _currentRotateState;//���� ����� ȸ������
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
	StackNode emptynode;//�׳� ����
	StackNode topnode;//�ϳ��� ������ ������� ���յ��� ���� ���
	StackNode* resultnode = nullptr;//�ϳ��� ������ ���յ��� ����� ����� ������ ���

	mStack buffer;//�ϳ��� ���� ������ ������� �����ϴ� ����

	int rotatecount[7] = {2,1,2,2,4,4,4 };
	struct Point { int _blockX, _blockY; };
	Point _blockShape[7][4][4] =
	{
		{ { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },//���ڱ��
		{ { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },//�׸𳭰�
		{ { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },//����
		{ { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },//����
		{ { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },//�⿪
		{ { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },//����
		{ { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },//��ť
	};
	

public:
	double gene[7] = { 0, };
	StackNode tempnode;//�갡 ����� �����Ҷ� ���� ���� ����
	//���� ����ҽ�
	//��ü �����Ҷ� ����
	void initializeobj();
	int getclearlinecount();
	//���� �ٸ����� ��� ���� ����
	void calcontactblock();
	//���� ������ ��� ���� ����
	void calcontactside();
	//���� �ٴڰ� ��� ���� ����
	void calcontactfloor();
	//���� �������� �ϼ��Ǵ� ���� ������ ����
	void calfullline(int y);
	//�� ������ ������� ������ ����
	void calobstruct(int i, int j);
	//�� ������ ���� ����
	void calblockheight();
	//�ӽ� ���ó�� �ʱ�ȭ
	void cleargood();
	//���յ� �ӽ� ���ó�忡 ����
	void makegood();
	//���յ� ���
	void showgoodness();
	//���� ����������� �ű�
	void gotoleft();
	//���� ���� ���� ��ȯ
	int getleft();
	//���� ���� ������ ��ȯ
	int getright();
	//���������� ��ĭ�� ������
	void moveright();
	//���� �ϳ��� ����!
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