#include"stack.h"
/*//위에 넣는 함수, 위에 빼는 함수, 길이 재는 함수, 꼭대기 확인만 하는 함수 비어있는 지 확인
*/
void StackNode::goodnesssum() {

	goodsum = criteria.blockheight * criteria.gene[0];
	goodsum += criteria.obstruct * criteria.gene[1];
	goodsum += criteria.space * criteria.gene[2];
	goodsum += criteria.fulllinecount * criteria.gene[3];
	goodsum += criteria.contactfloor * criteria.gene[4];
	goodsum += criteria.contactside * criteria.gene[5];
	goodsum += criteria.contactblock * criteria.gene[6];
}

mStack::mStack() {
	s_top = nullptr;
	T_node = nullptr;
	g_top = nullptr;
	gene_node = nullptr;

	node_count = 0;
}


int mStack::getnodecount() {return node_count;}
GeneNode* mStack::gettopgene() { return g_top; }

void mStack::insert_delete(int num,GeneNode* insertnode) {
	GeneNode* R_node = g_top;
	gene_node = g_top;
	for (int i = 0; i < num; i++) {
		R_node = gene_node;
		gene_node = gene_node->P_node;
	}
	insertnode->P_node = gene_node->P_node;

	if(num)
		R_node->P_node = insertnode;

	delete gene_node;
	if (!num)
		g_top = insertnode;

	return;
}

StackNode* mStack::pop_top() {
	if (!isEmpty()) {
		T_node = s_top;
		s_top = T_node->P_node;
		T_node->P_node = nullptr;
		
		node_count--;
		return T_node;
	}
}

GeneNode* mStack::pop_topgene() {
	if (!gisEmpty()) {
		gene_node = g_top;
		g_top = gene_node->P_node;
		gene_node->P_node = nullptr;

		node_count--;
		return gene_node;
	}
}

void mStack::insert_top(StackNode* tempnode) {
	StackNode* Pnew = new StackNode();
	*Pnew= *tempnode;

	Pnew->P_node = s_top;
	s_top = Pnew;
	node_count++;
	
	return;
}

void mStack::insert_top(GeneNode* gene) {
	GeneNode* Pnew = new GeneNode();
	*Pnew = *gene;

	Pnew->P_node = g_top;
	g_top = Pnew;
	node_count++;

	return;
}
/*
DATA mStack::pop_top() {
	if (!isEmpty()) {
		T_node = s_top;
		s_top = s_top->P_node;
		T_node->P_node = nullptr;

		node_count--;

		return T_node->data;
	}
	else {
		cout << "There isn't data" << endl;
		return 0;
	}
}*/
/*
int mStack::pop_topint() {
	if (!isEmpty()) {
		T_node = s_top;
		s_top = s_top->P_node;
		T_node->P_node = nullptr;

		node_count--;

		return T_node->data_int;
	}
	else {
		cout << "There isn't data" << endl;
		return 0;
	}
}
*/

bool mStack::isEmpty() {
	if (s_top == nullptr)
		return true;

	else
		return false;
}

bool mStack::gisEmpty() {
	if (g_top == nullptr)
		return true;

	else
		return false;
}

int mStack::length() {
	int length = 0;

	if (!isEmpty()) {
		T_node = s_top;

		while (T_node->P_node != nullptr) {
			T_node = T_node->P_node;
			length++;
		}

		return length;
	}

	return length;
}

StackNode* mStack::peek() {
	return s_top;
}