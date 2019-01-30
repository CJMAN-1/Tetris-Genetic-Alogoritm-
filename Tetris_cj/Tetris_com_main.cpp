#include "Tetris.h"
#include <iostream>

#define GENE_LENGTH 7
#define CLEAR 100
using namespace std;

int main()
{
	FILE* file = nullptr;

	Block block;
	double geneinit[7] = { -3.000000, -0.230000, -11.870000, 14.890000, 9.500000, 8.790000, 10.980000 };
	char flag[10];
	
	file = fopen("C:\\Users\\kimcj\\Documents\\Visual Studio 2015\\Projects\\Tetris_cj\\Tetris_cj\\good GENE.txt", "r");
	fscanf(file,"%lf %lf %lf %lf %lf %lf %lf" ,&geneinit[0], &geneinit[1], &geneinit[2], &geneinit[3], &geneinit[4], &geneinit[5], &geneinit[6]);
	fclose(file);

	for (int i = 0; i < GENE_LENGTH; i++) {
		block.gene[i] = geneinit[i];
	}
	system("mode con cols=70 lines=27");
	block.initializeobj();
	block.PrintOutBoard();
	block.PrintNextBlockBox();
	block.setCurrentBlockState(block.getRandomNumber(0, 6));//7개 모양의 블록중 하나 고름

	file = fopen("flag.txt", "w");
	fprintf(file, "%s", "no");
	fclose(file);

	while (true) {
		file = fopen("flag.txt", "r");
		fscanf(file, "%s", flag);
		fclose(file);

		if (!strcmp(flag, "Started")) {
			break;
		}
	}

	while (true) {
		block.InitializeBlockState();
		block.setNextBlockState(block.getRandomNumber(0, 6));
		block.PrintNextBlock(true);

		block.rotateblock();
		if (!strcmp(block.flag, "finish")) {
			break;
		}
		if (block.CheckBlockAround(block.getCurrentX(), block.getCurrentY(), block.getCurrentRotateState()))//게임 종료조건
			break;

		block.EmptyNextBlockBox();
		block.setCurrentBlockState(block.getNextBlockState());

		

		if (block.getclearlinecount() >= CLEAR) {
			file = fopen("flag.txt", "w");
			fprintf(file, "%s", "finish");
			fclose(file);

			block.gotoxy(5, 10);
			cout << "I   A M   W I N N E R ! !  :)";

			break;
		}
	}

	Sleep(10000);
	return 0;
}