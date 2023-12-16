#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define DIGIT(x) (x >= 48 && x <= 57)
#define STAR(x) (x == '*')

const int ESIZE = 141;

void process_num(int* row) {
	int left_idx = 0;
	int right_idx = 0;
	int d = row[0];
	while (right_idx < ESIZE) {
		int buffer = 0;
		while (d >= 0) {
			buffer = buffer * 10 + d;
			right_idx++;
			d = row[right_idx];
		}
		while (left_idx < right_idx) {
			row[left_idx] = buffer;
			left_idx++;
		}
		left_idx++;
		right_idx++;
		d = row[right_idx];
	}
}

int process_gear(int* engine, int row) {
	int row_sum = 0;
	for (int i = 0; i < ESIZE; i++) {
		const int offset = ESIZE * row + i;
		int prod = 1;
		int touching = 0;
		if (engine[offset] == INT_MIN) {
			if (i && engine[offset-1] > 0) { prod *= engine[offset-1]; touching++; }
			if (i < ESIZE-1 && engine[offset+1] > 0) { prod *= engine[offset+1]; touching++; }
			if (row) {
				if (engine[offset-ESIZE] > 0) {prod *= engine[offset-ESIZE]; touching++; }
				else {
					if (i && engine[offset-ESIZE-1] > 0) { prod *= engine[offset-ESIZE-1]; touching++; }
					if (i < ESIZE-1 && engine[offset-ESIZE+1] > 0) { prod *= engine[offset-ESIZE+1]; touching++; }
				}
			}
			if (row < 139) {
				if (engine[offset+ESIZE] > 0) {prod *= engine[offset+ESIZE]; touching++; }
				else {
					if (i && engine[offset+ESIZE-1] > 0) { prod *= engine[offset+ESIZE-1]; touching++; }
					if (i < ESIZE-1 && engine[offset+ESIZE+1] > 0) { prod *= engine[offset+ESIZE+1]; touching++; }
				}
			}
		}
		if (touching == 2) row_sum += prod;
	}
	return row_sum;
}

int main(int argc, char** argv) {

	if (argc != 2) return 1;

	int answer = 0;
	int engine[19740];
	FILE* fp = fopen(argv[1], "r");
	char c;
	int idx = 0;
	do {
		c = fgetc(fp);
		if (DIGIT(c)) engine[idx] = c - 48;
		else if (STAR(c)) engine[idx] = INT_MIN;
		else engine[idx] = -1;
		idx++;
	} while (c != EOF);
	fclose(fp);
	for (int row = 0; row < 140; row++) {
		process_num(&engine[row*ESIZE]);
	}
	for (int row = 0; row < 140; row++) {
		answer += process_gear(engine, row);
	}

	printf("%d\n", answer);
	return 0;
}
