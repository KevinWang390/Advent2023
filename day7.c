#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NUM_CARD(x) (x >= '2' && x <= '9')

const int BUF_SIZE = 9893;
const int NUM_LINES = 1000;

enum Hand {
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_KIND,
	FULL_HOUSE,
	FOUR_KIND,
	FIVE_KIND
};

char to_card(char c) {
	if (NUM_CARD(c)) return c - '0';
	switch (c) {
	case 'T':
		return 10;
	case 'J':
		return 1; // joker instead of jack
	case 'Q':
		return 12;
	case 'K':
		return 13;
	default:
		return c;
	}
}

int hand(char* s) {
	int mult[6];
	memset(mult, 0, 6*sizeof(int));
	int counter[128];
	memset(counter, 0, 128*sizeof(int));
	int jokers = 0;
	for (int i = 0; i < 5; i++) {
		char c = s[i];
		if (c == 'J') jokers++;
		else {
			mult[counter[c]]--;
			counter[c]++;
			mult[counter[c]]++;
		}
	}
	if (jokers == 5) mult[5] = 1;
	else for (int i = 5; i >= 1; i--) {
		if (mult[i]) {
			mult[i+jokers]++;
			mult[i]--;
			break;
		}
	}
	enum Hand h;
	if (mult[5]) h = FIVE_KIND;
	else if (mult[4]) h = FOUR_KIND;
	else if (mult[3] && mult[2]) h = FULL_HOUSE;
	else if (mult[3]) h = THREE_KIND;
	else if (mult[2] == 2) h = TWO_PAIR;
	else if (mult[2]) h = ONE_PAIR;
	else h = HIGH_CARD;
	return h;
}

int hand_cmp(char* h1, char* h2) {
	while(h1 && h2) {
		if (to_card(*h1) < to_card(*h2)) return -1;
		else if (to_card(*h1) > to_card(*h2)) return 1;
		h1++;
		h2++;
	}
	return 0;
}

int sort_hands(const void* a, const void* b) {
	char* cards1 = *(char**)a;
	char* cards2 = *(char**)b;
	int hand1 = hand(cards1);
	int hand2 = hand(cards2);
	if (hand1 == hand2) return hand_cmp(cards1, cards2);
	if (hand1 < hand2) return -1;
	return 1;
}

int main(int argc, char** argv) {

	if (argc != 2) { printf("%s\n", "input filename only"); return 1; }

	char buffer[BUF_SIZE];
	FILE* fp = fopen(argv[1], "r");
	fread(buffer, 1, BUF_SIZE, fp);
	fclose(fp);

	char* hands_and_bids[NUM_LINES];
	hands_and_bids[0] = strtok(buffer, "\n");
	for (int i = 1; i < NUM_LINES; i++) {
		hands_and_bids[i] = strtok(NULL, "\n");
	}
	qsort(hands_and_bids, NUM_LINES, sizeof(char*), sort_hands);

	int answer = 0;
	for (int i = 0; i < NUM_LINES; i++) {
		char* s = hands_and_bids[i]+6;
		int bid = atoi(s);
		answer += (i+1) * bid;
	}

	printf("%d\n", answer);
	return 0;
}
