#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define max(a,b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
		_a > _b ? _a : _b; })

#define min(a,b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

/*
int pthread_create() {
	pthread_t *t;
	pthread_t *attr;
	void *(*thread_body(void*));
}
*/

/*=========================================

There are two options to cancel the thread:
1. pthread_exit()
2. pthread_cancel()
2'. pthread_join()

==========================================*/

/*==========================================
	MUTEXES

Recommended to declare globally.
 
pthread_mutex
pthread_mutex_init
pthread_mutex_trylock
pthread_mutex_timedlock - not recommended to use (!)


	CONDITION VARIABLES

pthread_cond_wait
pthread_cond_signal

============================================*/

pthread_mutex_t mutex;
pthread_mutex_t mutex_position;
pthread_cond_t cv;
int array_size;
int thread_count;
int leng;
double ans_max;
double ans_min;
int max_cur_pos = 0;
int min_cur_pos = 0;
int* ans_max_positions;
int* ans_min_positions;
int* indxs;
double* array;
int done_threads = 0;

void* func(void* arg) {
	int ind = *(int*) arg;

	int batch_i = leng * ind;
	// printf("> Batch_i: %d\n", ind);
	// int batch_end = batch_i + leng + 1;
	int batch_end = batch_i + leng;
	if (ind == thread_count - 1) {
		batch_end = array_size;
	}

	assert(batch_i < array_size);
	assert(batch_end >= batch_i);

	double loc_max = array[batch_i];
	double loc_min = array[batch_i];
	int i;
	for (i = batch_i; i < batch_end; ++i) {
		// printf("+%d\n", i);
		if (array[i] > loc_max) {
			loc_max = array[i];
		}
		if (array[i] < loc_min) {
			loc_min = array[i];
		}
	}
	pthread_mutex_lock(&mutex);
	if (loc_max > ans_max) {
		ans_max = loc_max;
	}
	if (loc_min < ans_min) {
		ans_min = loc_min;
	}
	done_threads += 1;
	pthread_cond_broadcast(&cv);
	pthread_mutex_unlock(&mutex);

	pthread_mutex_lock(&mutex_position);
	while (done_threads < thread_count) {
		pthread_cond_wait(&cv, &mutex_position);
	}

	pthread_mutex_lock(&mutex);
	for (i = batch_i; i < batch_end; ++i) {
		// printf("))%d\n", i);
		if (array[i] == ans_max) {
			ans_max_positions[max_cur_pos] = i;
			max_cur_pos += 1;
		}
		if (array[i] == ans_min) {
			ans_min_positions[min_cur_pos] = i;
			// printf("))%d\n", ans_min_positions[min_cur_pos]);
			min_cur_pos += 1;
			// printf("----%d\n", min_cur_pos);
		}
	}
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&mutex_position);

	return NULL;
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
	if (pthread_mutex_init(&mutex, NULL) != 0 && 
		pthread_mutex_init(&mutex_position, NULL) != 0 && 
		pthread_cond_init(&cv, NULL) != 0) {
        printf("\n mutex or cv init failed\n");
        return 1;
    }

	scanf("%d", &thread_count);
	int m;
	int n;
	scanf("%d %d", &m, &n);
	array_size = m * n;
	if (thread_count >= array_size) {
		thread_count = max(1, array_size / 2);
	}
	leng = array_size / thread_count;
	// printf("> Leng: %d\n", leng);

	pthread_t* threads = malloc(thread_count * sizeof(pthread_t));
	ans_max_positions = malloc(array_size * sizeof(int));
	ans_min_positions = malloc(array_size * sizeof(int));
	array = malloc(array_size * sizeof(double));
	indxs = malloc(thread_count * sizeof(int));

	int i;
	// FILE* f = fopen("data.txt", "r");
	for (i = 0; i < array_size; ++i) {
		double tmp;
		scanf("%lf", &tmp);
		// fscanf(f, "%d", &tmp);
		array[i] = tmp;
	}
	ans_max = array[0];
	ans_min = array[0];

	for (i = 0; i < thread_count; ++i) {
		indxs[i] = i;
		pthread_create(&threads[i], NULL, func, &indxs[i]);
	}
	for (i = 0; i < thread_count; ++i) {
		pthread_join(threads[i], NULL);
	}

	// puts("----<<--->>------");

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

	// pthread_mutex_destroy(&mutex);
	// pthread_mutex_destroy(&mutex_position);
	// pthread_cond_destroy(&cv);
	// free(array);
	// free(threads);
	// free(indxs);
	// free(ans_max_positions);
	// free(ans_min_positions);
	return 0;
}
