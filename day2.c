#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFSIZE = 256;

const int REDMAX = 12;
const int GREENMAX = 13;
const int BLUEMAX = 14;
const char* delim = " ,;:\n";

int max(int a, int b) {
	if (a > b) return a;
	return b;
}

int game_power(char* str) {

	int redmin = 0;
	int greenmin = 0;
	int bluemin = 0;
	char * t = strtok(str, delim);
	t = strtok(NULL, delim); // discard first token
	t = strtok(NULL, delim); // discard second token
	while(t != NULL) {
		int n = atoi(t); // get number
		t = strtok(NULL, delim); // get type
		if (strcmp(t,"red") == 0) redmin = max(redmin, n);
		else if (strcmp(t,"green") == 0) greenmin = max(greenmin, n);
		else if (strcmp(t,"blue") == 0) bluemin = max(bluemin, n);
		t = strtok(NULL, delim);
	}

	return redmin * greenmin * bluemin;
}

int main(int argc, char** argv) {

	if (argc != 2) return 1;

	FILE* fp = fopen("input.txt", "r");
	char buf[BUFSIZE];
	int answer = 0;
	while(fgets(buf, BUFSIZE, fp)) {
		answer += game_power(buf);
	}

	printf("%d\n", answer);
	fclose(fp);
	return 0;
}
