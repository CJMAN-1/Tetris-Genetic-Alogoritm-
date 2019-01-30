#include "Tetris.h"
#include <iostream>

#define ENTITY 30
#define GENE_LENGTH 7
#define GENE_INIT 5
#define PROBABILITY_CROSS 70

using namespace std;

int main()
{
	FILE* file=nullptr;
	Block block;
	mStack mothergeneration;
	mStack childgeneration;
	GeneNode* g_node = nullptr;
	GeneNode* top_node = nullptr;
	GeneNode alltop_node;
	int sedae = 1;
	int probability = 0;
	int entity_num;
	double sum_clearline = 0;
	double top_clearline = 0;
	int pre_topclear = 0;

	//초기화
	double initialize = 0;
	double geneinit[7] = { -3.000000, -0.230000, -11.870000, 14.890000, 9.500000, 8.790000, 10.980000 };

	//선택연산
	double	point = 0,
		sumoffit = 0,//한 세대에서 적합도들의 합
		cw = 0,//세대에서 가장 나쁜 해의 비용//좋은 해일수록 비용이 적다.
		ci = 0,//현재 해의 비용
		cb = 0,//세대에서 가장 좋은 해의 비용
		fi = 0,
		sum = 0;
	double k = 3;//선택압,이 값이 높아 지면 기본적으로 주어지는 적합도가 낮아진다->낮은해가 뽑힐 확률이 낮아진다
				 //적합도 계산할때 선택압 조절함 보통 3~4정도로 주어짐
	//교차연산
	GeneNode* cross1_node = nullptr;
	GeneNode* cross2_node = nullptr;
	//돌연변이
	int cubic, square;
	double probability_mutant = 0;
	int mutant_count = 0;
	double value_mutation = 0;

	while (1) {//한 세대 시작
		pre_topclear = top_clearline;
		sum_clearline = 0;
		cw = 10000;
		cb = 0;
		top_clearline = 0;
		top_node = nullptr;

		block.gotoxy(24, 2);
		printf("이전세대 탑값 : %d  ", pre_topclear);
		block.gotoxy(24, 1);
		printf("클리어한 줄의 갯수 : ");
		block.gotoxy(24, 3);
		printf("모든세대 탑값 : %d    ", alltop_node.clearlinecount);

		for (entity_num = 0; entity_num < ENTITY; entity_num++) {//한 객체 시작
			if ((sedae == 1) || (pre_topclear == 0)) {
				for (int index = 0; index < GENE_LENGTH; index++) {
					probability = block.getRandomNumber(0, 1);
					initialize = block.getRandomNumber(0, 800);
					initialize /= 100;

					switch (probability) {
					case 0:
						geneinit[index] = initialize;
						block.gene[index] = geneinit[index];
						break;
					
					case 1:
						geneinit[index] = -initialize;
						block.gene[index] = geneinit[index];
						break;
					}
				}
			}
		
			else {//2번째 세대부터의 초기화
				g_node = mothergeneration.g_top;
				for (int num = 0; num < entity_num; num++) {
					g_node = g_node->P_node;
				}

				for (int index = 0; index < 7; index++) {
					if (index < 3) {
						if (g_node->gene[index] > 0)
							g_node->gene[index] = -(g_node->gene[index]);
					}

					else {
						if (g_node->gene[index] < 0)
							g_node->gene[index] = -(g_node->gene[index]);
					}

					block.gene[index] = g_node->gene[index];
				}
			}
			
			/*
			for (int i = 0; i < GENE_LENGTH; i++) {
				block.gene[i] = geneinit[i];
			}*/

			block.gotoxy(24, 0);
			cout << sedae << "세대 " << entity_num + 1 << "번째 객체";

			block.initializeobj();
			block.PrintOutBoard();
			block.setCurrentBlockState(block.getRandomNumber(0, 6));//7개 모양의 블록중 하나 고름
																	
			while (true) {
				block.InitializeBlockState();
				block.setNextBlockState(block.getRandomNumber(0, 6));

				block.rotateblock();

				if (block.CheckBlockAround(block.getCurrentX(), block.getCurrentY(), block.getCurrentRotateState()))//게임 종료조건
					break;

				block.setCurrentBlockState(block.getNextBlockState());
			}
			sum_clearline += block.getclearlinecount();


			GeneNode* entity = new GeneNode;
			entity->clearlinecount = block.getclearlinecount();

			for (int i = 0; i<GENE_LENGTH; i++)
				entity->gene[i] = block.gene[i];


			if (entity->clearlinecount != 0) {//적합도 계산을 위한 준비
				if (entity->clearlinecount >= cb) {
					cb = entity->clearlinecount;
					top_clearline = entity->clearlinecount;
					top_node = entity;
				}

				if (entity->clearlinecount <= cw)
					cw = entity->clearlinecount;
			}


			if (sedae != 1)
				mothergeneration.insert_delete(entity_num, entity);

			else
				mothergeneration.insert_top(entity);
		}
		

		sumoffit = 0;

		if ((sum_clearline != 0) && (cw != cb)) {
			if (alltop_node.clearlinecount < top_node->clearlinecount) {
				for (int i = 0; i < GENE_LENGTH; i++) {
					alltop_node.gene[i] = top_node->gene[i];
					alltop_node.clearlinecount = top_node->clearlinecount;
				}
				file = fopen("good GENE.txt", "w");
				fprintf(file, "%lf %lf %lf %lf %lf %lf %lf %d", alltop_node.gene[0], alltop_node.gene[1], alltop_node.gene[2], alltop_node.gene[3]
					, alltop_node.gene[4], alltop_node.gene[5], alltop_node.gene[6], alltop_node.clearlinecount);
				fclose(file);
			}

			g_node = mothergeneration.gettopgene();
			while (g_node != nullptr) {
				if (g_node->clearlinecount != 0)
					ci = g_node->clearlinecount;

				else {
					g_node->fitness = 0;
					g_node = g_node->P_node;
					continue;
				}

				fi = ci - cw + (cb - cw) / (k - 1);

				sumoffit += fi;
				g_node->fitness = fi;
				g_node = g_node->P_node;
			}

			g_node = mothergeneration.gettopgene();
			while (g_node != nullptr) {//적합도를 퍼센트로 바꾸기
				g_node->fitness = g_node->fitness / sumoffit * 100;

				g_node = g_node->P_node;
			}

			
			for (int num_child = 0; num_child < ENTITY; num_child++) {//시작연산 시작
				cross1_node = nullptr;
				cross2_node = nullptr;

				for (int j = 0; j < 2; j++) {
					sum = 0;
					point = 0;
					g_node = mothergeneration.gettopgene();
					point = block.getRandomNumber(0, 99);

					for (int i = 0; i < ENTITY; i++) {
						sum += g_node->fitness;

						if (sum >= point) {
							if (j == 0)
								cross1_node = g_node;

							else if(j == 1)
								cross2_node = g_node;

							break;
						}
						g_node = g_node->P_node;
					}
				}
				
				point = block.getRandomNumber(0, 100);

				if (point <= PROBABILITY_CROSS) {//교차연산
					GeneNode* children = new GeneNode;

					for (int i = 0; i < GENE_LENGTH; i++) {
						children->gene[i] = (cross1_node->gene[i] + cross2_node->gene[i]) / 2;
					}

					childgeneration.insert_top(children);
				}

				else {
					GeneNode* children = new GeneNode;

					*children = *cross1_node;
					children->P_node = nullptr;

					childgeneration.insert_top(children);
				}		
			}


			cubic = top_clearline*top_clearline*top_clearline;
			square = top_clearline*top_clearline;
			probability_mutant = (double)85 / 500000 * cubic - (double)85 * 150 / 500000 * square + 100;
			if (top_clearline >= 100)
				probability_mutant = 15;

			g_node = childgeneration.gettopgene();
			for (int i = 0; i < ENTITY; i++) {
				point = block.getRandomNumber(0, 10000);
				point = point / 100;
				if (point <= probability_mutant) {
					mutant_count++;
					block.gotoxy(24, 4);
					cout << mutant_count << "번 돌연변이 발생!!";

					value_mutation = block.getRandomNumber(0, 800);
					value_mutation = value_mutation / 100;

					for (int index = 0; index < GENE_LENGTH; index++) {
						probability = block.getRandomNumber(0, 1);

						switch (probability) {
						case 0:
							g_node->gene[index] += value_mutation;
							break;

						case 1:
							g_node->gene[index] -= value_mutation;
							break;
						}
					}
				}
				g_node = g_node->P_node;
			}


			g_node = childgeneration.gettopgene();

			for (int i = 0; i < GENE_LENGTH; i++) {//가장 좋은해 유전
				g_node->gene[i] = top_node->gene[i];
			}
		}

		else {//하나도 클리어 못하거나 한 개의 객체만 클리어 한 경우
			for (int i = 0; i < ENTITY; i++) {
				GeneNode* children = new GeneNode;
				childgeneration.insert_top(children);
			}
		}




		g_node = mothergeneration.gettopgene();
		while (g_node != nullptr) {
			g_node = mothergeneration.pop_topgene();
			delete g_node;
			g_node = mothergeneration.gettopgene();
		}

		g_node = childgeneration.gettopgene();
		while (g_node != nullptr) {
			g_node = childgeneration.pop_topgene();
			mothergeneration.insert_top(g_node);
			delete g_node;
			g_node = childgeneration.gettopgene();
		}

		sedae++;
	}

	block.gotoxy(6, 5);
	cout << "You Are Failed!";

	system("pause>null");
	return 0;
}