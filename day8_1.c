#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int BUF_SIZE = 13861;
const int NUM_NODES = 798;
const char* delim = " \n(),=";

union name_u {
	char* s;
	int i;
};

struct node {
	char* name;
	union name_u left;
	union name_u right;
};

int find_node(struct node* arr, char* target) {
	for (int i = 0; i < NUM_NODES; i++) {
		int cmp = strcmp(arr[i].name, target);
		if (!cmp) return i;
	}
	return -1;
}

int main(int argc, char** argv) {

	if (argc != 2) { printf("%s\n", "input filename only"); return 1; }

	char buffer[BUF_SIZE];
	FILE* fp = fopen(argv[1], "r");
	fread(buffer, 1, BUF_SIZE, fp);
	fclose(fp);

	char* directions = strtok(buffer, delim);
	int d_length = strlen(directions);
	struct node node_arr[NUM_NODES];

	for(int i = 0; i < NUM_NODES; i++) {
		struct node n;
		// name
		n.name = strtok(NULL, delim);
		// left
		n.left.s = strtok(NULL, delim);
		//right
		n.right.s = strtok(NULL, delim);
		node_arr[i] = n;
	}

	for(int i = 0; i < NUM_NODES; i++) {
		struct node* np = &(node_arr[i]);
		np->left.i = find_node(node_arr, np->left.s);
		np->right.i = find_node(node_arr, np->right.s);
	}

	int curr_node = find_node(node_arr, "AAA");
	int dest_node = find_node(node_arr, "ZZZ");
	int step_count = 0;
	while (curr_node != dest_node) {
		char step = directions[step_count % d_length];
		if (step == 'L') curr_node = node_arr[curr_node].left.i;
		else curr_node = node_arr[curr_node].right.i;
		step_count++;
	}

	printf("%d\n", step_count);
	return 0;
}
