#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGIT(x) (x >= 48 && x <= 57)
#define SYMBOL(x) (!DIGIT(x) && x != '.' && x != '\n')

// line size is 141 with newline
const int ESIZE = 141;

int eval_line(char* back, char* current, char* front) {

	int sum = 0;
	int buffer = 0;
	int prev_adjacent = 0;
	int is_valid = 0;

	for (int i = 0; i < 141; i++) {
		char c = current[i];
		int curr_adjacent = SYMBOL(back[i]) || SYMBOL(c) || SYMBOL(front[i]);
		if (curr_adjacent || prev_adjacent) is_valid = 1;
		if (DIGIT(c)) {
			buffer = buffer * 10 + c - 48;
		}
		else {
			if (is_valid) {
				sum += buffer;
				is_valid = 0;
			}
			buffer = 0;
		}
		prev_adjacent = curr_adjacent;
	}
	return sum;
}

int main(int argc, char** argv) {

	if (argc != 2) return 1;

	FILE* fp = fopen(argv[1], "r");
	char* back = (char*)malloc(ESIZE);
	char* current = (char*)malloc(ESIZE);
	char* front = (char*)malloc(ESIZE);
	memset(back, '.', ESIZE);
	fgets(current, ESIZE + 1, fp);
	int answer = 0;
	while (fgets(front, ESIZE + 1, fp)) {
		answer += eval_line(back, current, front);
		char* tmp = back;
		back = current;
		current = front;
		front = tmp;
	}
	memset(front, '.', ESIZE);
	answer += eval_line(back, current, front);

	printf("%d\n", answer);
	fclose(fp);
	return 0;
}
