#include "Tetris.h"


#define GENE_LENGTH 7
#define CLEAR 10

using namespace std;

int main()
{
	Block block;
	FILE* file;
	system("mode con cols=70 lines=27");
	block.PrintOutBoard();
	block.PrintNextBlockBox();
	char flag[10] = {0,};

	int frame, interval;

	frame = 20;
	while (true) {
		if (kbhit()) {
			break;
		}
	}
	

	block.setCurrentBlockState(block.getRandomNumber(0, 6));
	file = fopen("C:\\Users\\kimcj\\Documents\\Visual Studio 2015\\Projects\\Tetris_com\\Tetris_com\\flag.txt", "w");
	fprintf(file, "Started");
	fclose(file);

	while (true)
	{
		block.InitializeBlockState();
		block.setNextBlockState(block.getRandomNumber(0, 6));
		block.PrintBlock(true);
		block.PrintNextBlock(true);
		block.gotoxy(24, 0);
		cout << "클리어한 라인" << block.clearline;

		interval = frame;
		block.PrintShadowBlock(true);
		while (true)
		{
			if (--interval == 0) {
				interval = frame;

				if (block.isMoveDown())
					break;

			}

			if (block.isInputKey())
				break;

			file = fopen("C:\\Users\\kimcj\\Documents\\Visual Studio 2015\\Projects\\Tetris_com\\Tetris_com\\flag.txt", "r");
			fscanf(file, "%s", flag);
			fclose(file);

			if (!strcmp(flag, "finish")) {
				block.gotoxy(5, 10);
				cout << "Y O U   A R E   L O S E R ! !";
				break;
			}

			Sleep(50);
		}
		if (!strcmp(flag, "finish")) {
			break;
		}

		block.EmptyNextBlockBox();

		block.setCurrentBlockState(block.getNextBlockState());


		if (block.clearline >= CLEAR) {
			file = fopen("C:\\Users\\kimcj\\Documents\\Visual Studio 2015\\Projects\\Tetris_com\\Tetris_com\\flag.txt", "w");
			fprintf(file, "finish");
			fclose(file);

			block.gotoxy(5, 10);
			cout << "Y O U   A R E   W I N N E R ! !";

			break;
		}
	}
	Sleep(10000);
	return 0;

}