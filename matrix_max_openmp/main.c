#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })

#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

int array_size;
int batch_count;
int leng;
double ans_max;
double ans_min;
int max_cur_pos = 0;
int min_cur_pos = 0;
int* ans_max_positions;
int* ans_min_positions;
double* array;

void find_extr(int ind) {
	int batch_i = leng * ind;

	int batch_end = batch_i + leng;

	if (ind == batch_count - 1) {
		batch_end = array_size;
	}

	assert(batch_i < array_size);
	assert(batch_end >= batch_i);

	double loc_max = array[batch_i];
	double loc_min = array[batch_i];
	int i;
	for (i = batch_i; i < batch_end; ++i) {
		if (array[i] > loc_max) {
			loc_max = array[i];
		}
		if (array[i] < loc_min) {
			loc_min = array[i];
		}
	}
	
	#pragma omp critical
	{
		if (loc_max > ans_max) {
			ans_max = loc_max;
		}
		if (loc_min < ans_min) {
			ans_min = loc_min;
		}
	}
}

void find_pos(int ind) {
	int batch_i = leng * ind;

	int batch_end = batch_i + leng;

	if (ind == batch_count - 1) {
		batch_end = array_size;
	}

	assert(batch_i < array_size);
	assert(batch_end >= batch_i);

	int i;
	for (i = batch_i; i < batch_end; ++i) {
		if (array[i] == ans_max) {
			#pragma omp critical
			{	
				ans_max_positions[max_cur_pos] = i;
				max_cur_pos += 1;
			}
		}
		if (array[i] == ans_min) {
			#pragma omp critical
			{
				ans_min_positions[min_cur_pos] = i;
				min_cur_pos += 1;
			}
		}
	}
}

int comp (const void * elem1, const void * elem2) {
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) {
    	return 1;
    }
    if (f < s) {
    	return -1;
    }
    return 0;
}

int main(int argc, char **argv) {
	struct timeval t1, t2;

	gettimeofday(&t1, NULL);

    FILE* f = fopen("matrix.bin", "rb");
    char* type_readed = malloc(6 * sizeof(char));
    fread(type_readed, sizeof(char), 6, f);
    if (strcmp(type_readed, "MATRIX") != 0) {
    	puts("type_readed != MATRIX.");
    	puts("Exit.");
    	return 1;
    }

    if (argc < 2) {
    	perror("Usage: ./a.out <num_threads>");
    	exit(1);
    }

	batch_count = atoi(argv[1]);

	int m;
	int n;
	fread(&m, sizeof(int), 1, f);
	fread(&n, sizeof(int), 1, f);

	array_size = m * n;
	leng = array_size / batch_count;

	pthread_t* threads = malloc(batch_count * sizeof(pthread_t));
	ans_max_positions = malloc(array_size * sizeof(int));
	ans_min_positions = malloc(array_size * sizeof(int));
	array = malloc(array_size * sizeof(double));

	int i;
	double tmp;
	for (i = 0; i < array_size; ++i) {
		fread(&tmp, sizeof(double), 1, f);
		array[i] = tmp;
	}
	ans_max = array[0];
	ans_min = array[0];

	gettimeofday(&t2, NULL);
	unsigned long int tv1 = t1.tv_sec*1e6 + t1.tv_usec;
	unsigned long int tv2 = t2.tv_sec*1e6 + t2.tv_usec;
	// printf("%lu\n", tv2 - tv1);

	gettimeofday(&t1, NULL);
	/**
	* Main Part
	*/
	#pragma omp parallel for num_threads(batch_count)
	for (i = 0; i < batch_count; ++i) {
		// printf("num_threads: %d\n", omp_get_num_threads());
		find_extr(i);
	}
	#pragma omp parallel for num_threads(batch_count)
	for (i = 0; i < batch_count; ++i) {
		find_pos(i);
	}
	/**
	* End of Main Part
	*/
	gettimeofday(&t2, NULL);

	tv1 = t1.tv_sec*1e6 + t1.tv_usec;
	tv2 = t2.tv_sec*1e6 + t2.tv_usec;
	printf("Time: %lu\n", tv2 - tv1);

	qsort(ans_max_positions, max_cur_pos, sizeof(int), comp);
	qsort(ans_min_positions, min_cur_pos, sizeof(int), comp);

	printf("%.0lf\n", ans_max);
	for (i = 0; i < max_cur_pos; ++i) {
		if (i == 0) {
			printf("%d", ans_max_positions[i]);
		} else {
			printf(" %d", ans_max_positions[i]);
		}
	}
	printf("\n");
	printf("%.0lf\n", ans_min);
	for (i = 0; i < min_cur_pos; ++i) {
		if (i == 0) {
			printf("%d", ans_min_positions[i]);
		} else {
			printf(" %d", ans_min_positions[i]);
		}
	}
	printf("\n");

	free(array);
	free(threads);
	free(ans_max_positions);
	free(ans_min_positions);
	return 0;
}
