#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int BUFSIZE = 512;
const int TABLESIZE = 256;

int main(int argc, char** argv) {

	if (argc != 2) return 1;

	char buffer[BUFSIZE];
	char table[TABLESIZE];
	int cards[208]; // cheating a little bit, because I know how many cards I start with...
	memset(cards, 0, 208*sizeof(int));
	int card_id = 0;
	int answer = 0;
	FILE* fp = fopen(argv[1], "r");

	while(fgets(buffer, BUFSIZE, fp)) {
		int win_count = 0;
		memset(table, 0, TABLESIZE);
		char* t = strtok(buffer, " ");
		t = strtok(NULL, " "); // discard first token
		t = strtok(NULL, " "); // discard second token
		while (strcmp(t, "|")) {
			table[atoi(t)] = 1;
			t = strtok(NULL, " ");
		}
		t = strtok(NULL, " "); // discard "|"
		while (t != NULL) {
			if (table[atoi(t)]) win_count++;
			t = strtok(NULL, " ");
		}
		cards[card_id]++;
		for (int i = card_id+1; i <= card_id+win_count; i++) cards[i] += cards[card_id];
		answer += cards[card_id];
		card_id++;
	}
	fclose(fp);
	printf("%d\n", answer);
	return 0;
}
