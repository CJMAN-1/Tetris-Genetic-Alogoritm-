#include <iostream>
#define DATA char
#define CHAR 0
#define INT 1
using namespace std;
class GeneNode {
public:
	double gene[7] = { 0, };
	int clearlinecount = 0;
	double fitness = 0;

	GeneNode* P_node = nullptr;
};

class StackNode {
public:
	struct Criteria {
		int blockheight=0;
		int obstruct=0;
		int space=0;
		int fulllinecount=0;
		int contactfloor=0;
		int contactside=0;
		int contactblock=0;
		double gene[7] = { 0, };
	};

	Criteria criteria;
	int _currentX=0;
	int _currentrotatestate=0;
	double goodsum =-100;
	int clearlinecount = 0;//적합도 변수

	StackNode* P_node = nullptr;

	void goodnesssum();
};

class mStack {

	StackNode* s_top = nullptr;
	StackNode* T_node;
	
	GeneNode* gene_node;
	int node_count;

public:
	GeneNode* g_top = nullptr;
	mStack();
	void insert_delete(int num, GeneNode* insertnode);
	void insert_top(StackNode* criteria);
	void insert_top(GeneNode* gene);

	StackNode* pop_top();
	GeneNode* pop_topgene();

	GeneNode* gettopgene();

	bool gisEmpty();
	bool isEmpty();
	int mStack::length();
	int getnodecount();
	StackNode* peek();
};