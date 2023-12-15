#include <stdio.h>
#include <string.h>

int main() {

	FILE* fp = fopen("input.txt", "r");
	char buf[256];
	int answer = 0;

	char digit_table[9][6] = {
		"one\0", "two\0", "three\0",
		"four\0", "five\0", "six\0",
		"seven\0", "eight\0", "nine\0"
	};

	int dig_str_idx[9];

	while(fgets(buf, sizeof(buf), fp)) {
		int first = 0;
		int second = 0;
		memset(dig_str_idx, 0, 9*sizeof(int));
		for (int i = 0; i < 256; i++) {
			int d = 0;
			char c = buf[i];
			if (c == '\n') break;
			if (c >= 48 && c <= 57) d = c - 48;
			else {
				for (int i = 0; i < 9; i++) {
					if (c == digit_table[i][dig_str_idx[i]]) dig_str_idx[i]++;
					else {
						if (i == 8 && c == 'i' && dig_str_idx[i] == 3) dig_str_idx[i] = 2;
						else if (c == digit_table[i][0]) dig_str_idx[i] = 1;
						else dig_str_idx[i] = 0;
					}
					if (dig_str_idx[i] == strlen(digit_table[i])) {
						d = i+1;
						dig_str_idx[i] = 0;
					}
				}
			}
			if (d && !first) first = second = d;
			else if (d) second = d;
		}
		answer += 10*first + second;
	}

	printf("%d\n", answer);
	return 0;

}
