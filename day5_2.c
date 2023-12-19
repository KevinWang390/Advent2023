#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

const int BUF_SIZE = 7190; // size of input file
const int NUM_SEEDS = 10;
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

int sort_by_src(const void* a, const void* b) {
	struct Map_statement* Ms1 = (struct Map_statement*)a;
	struct Map_statement* Ms2 = (struct Map_statement*)b;
	if (Ms1->src_start > Ms2->src_start) return 1;
	if (Ms1->src_start < Ms2->src_start) return -1;
	return 0;
}

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
	qsort(m->statements, m->size, sizeof(struct Map_statement), sort_by_src);
}

// stands for Linear Map Search
long long lms(struct Map* m, long long src_target, long long* next_offset) {
	struct Map_statement stmt = {0};
	long long src = 0;
	long long range = 0;
	for (int i = 0; i < m->size; i++) {
		src = m->statements[i].src_start;
		range = m->statements[i].range_len;
		if (src <= src_target && src + range > src_target) {
			long long map_offset = src_target - src;
			long long remainder = range - map_offset;
			if (*next_offset > remainder) *next_offset = remainder;
			return m->statements[i].dst_start + map_offset;
		}
		else if (src > src_target) {
			long long remainder = src - src_target;
			if (*next_offset > remainder) *next_offset = remainder;
			break;
		}
	}
	return src_target;
}

int main(int argc, char** argv) {

	if (argc != 2) { printf("%s\n", "input filename only"); return 1; }

	long long seed_start[NUM_SEEDS];
	long long seed_range[NUM_SEEDS];
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
		seed_start[i] = atoll(s);
		s = strtok(NULL, " \n");
		seed_range[i] = atoll(s);
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
		while (seed_range[i]) {
			long long res = seed_start[i];
			long long next_offset = seed_range[i];
			res = lms(&seed_to_soil, res, &next_offset);
			res = lms(&soil_to_fert, res, &next_offset);
			res = lms(&fert_to_watr, res, &next_offset);
			res = lms(&watr_to_lite, res, &next_offset);
			res = lms(&lite_to_temp, res, &next_offset);
			res = lms(&temp_to_humi, res, &next_offset);
			res = lms(&humi_to_loca, res, &next_offset);
			if (res < location) location = res;
			seed_range[i] -= next_offset;
			seed_start[i] += next_offset;
		}
	}

	printf("%lld\n", location);
	return 0;
}
