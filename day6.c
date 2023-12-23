#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int BUF_SIZE = 74;

void remove_spaces(char* s) {
	char* r = s;
	while (*r++) {
		while (*r == ' ') r++;
		*s = *r;
		s++;
	}
}

// quadratic equation, a = -1
int roots(double b, double c, double* r1, double* r2) {
	double det = pow(b,2) - 4*c;
	if (det < 0) return 0; // no roots -- fail
	*r1 = (b - sqrt(det))/2.0;
	*r2 = (b + sqrt(det))/2.0;
	return 1; // success
}

int main(int argc, char** argv) {

	if (argc != 2) { printf("%s\n", "input filename only"); return 1; }

	int answer = 0;
	char buffer[BUF_SIZE];
	FILE* fp = fopen(argv[1], "r");
	fread(buffer, 1, BUF_SIZE, fp);
	fclose(fp);
	char* time;
	char* dist;
	char* s = strtok(buffer, ":\n");
	time = strtok(NULL, ":\n");
	s = strtok(NULL, ":\n");
	dist = strtok(NULL, ":\n");
	remove_spaces(time);
	remove_spaces(dist);
	long long time_num = atoll(time);
	long long dist_num = atoll(dist);
	double r1, r2;
	if (roots((double)time_num, (double)dist_num, &r1, &r2)) {
		int lower = (int)(r1 + 1.0);
		int upper = (int)r2;
		int range = upper - lower + 1;
		answer = range;
	}
	printf("%d\n", answer);
	return 0;
}
