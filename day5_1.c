#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

const int BUF_SIZE = 7190; // size of input file
const int NUM_SEEDS = 20;
#define MAP_SIZE 128

struct Map_statement {
	long long dst_start;
	long long src_start;
	long long range_len;
};

struct Map {
	int size;
	struct Map_statement statements[MAP_SIZE];
};

void read_map(struct Map* m) {
	char* s = "dummy";
	int line_count = 0;
	while (!isdigit(s[0])) s = strtok(NULL, " \n");
	while (s && isdigit(s[0])) {
		struct Map_statement m_stmt;
		m_stmt.dst_start = atoll(s); // dst range start
		s = strtok(NULL, " \n");
		m_stmt.src_start = atoll(s); // src range start
		s = strtok(NULL, " \n");
		m_stmt.range_len = atoll(s); // range len
		s = strtok(NULL, " \n");
		m->statements[line_count] = m_stmt;
		line_count++;
	}
	m->size = line_count;
}

// stands for Linear Map Search
long long lms(struct Map* m, long long src_target) {
	struct Map_statement stmt = {0};
	long long lowest_src = 0;
	for (int i = 0; i < m->size; i++) {
		long long src_current = m->statements[i].src_start;
		if (src_current >= lowest_src && src_current <= src_target) {
			lowest_src = src_current;
			stmt = m->statements[i];
		}
	}
	long long map_offset = src_target - stmt.src_start;
	if (map_offset < stmt.range_len) return stmt.dst_start + map_offset;
	else return src_target;
}

int main(int argc, char** argv) {

	if (argc != 2) { printf("%s\n", "input filename only"); return 1; }

	long long seeds[NUM_SEEDS];
	struct Map seed_to_soil = {0};
	struct Map soil_to_fert = {0};
	struct Map fert_to_watr = {0};
	struct Map watr_to_lite = {0};
	struct Map lite_to_temp = {0};
	struct Map temp_to_humi = {0};
	struct Map humi_to_loca = {0};
	long long location = LONG_MAX;
	char buffer[BUF_SIZE];
	FILE* fp = fopen(argv[1], "r");
	fread(buffer, 1, BUF_SIZE, fp);
	fclose(fp);

	char* s = strtok(buffer, " \n");
	/////////////////// ingest seeds /////////////////////////////////
	s = strtok(NULL, " \n"); // discard first
	for (int i = 0; i < NUM_SEEDS; i++) {
		seeds[i] = atoll(s);
		s = strtok(NULL, " \n");
	}
	/////////////////// ingest maps //////////////////////////////////
	read_map(&seed_to_soil);
	read_map(&soil_to_fert);
	read_map(&fert_to_watr);
	read_map(&watr_to_lite);
	read_map(&lite_to_temp);
	read_map(&temp_to_humi);
	read_map(&humi_to_loca);
	/////////////////// traverse maps ////////////////////////////////
	for (int i = 0; i < NUM_SEEDS; i++) {
		long long res = seeds[i];
		res = lms(&seed_to_soil, res);
		res = lms(&soil_to_fert, res);
		res = lms(&fert_to_watr, res);
		res = lms(&watr_to_lite, res);
		res = lms(&lite_to_temp, res);
		res = lms(&temp_to_humi, res);
		res = lms(&humi_to_loca, res);
//		printf("%lld\n", res);
		if (res < location) location = res;
	}

	printf("%lld\n", location);
	return 0;
}
