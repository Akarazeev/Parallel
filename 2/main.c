int** a,b,c;
int n;

enum actions {DO_WORK, DONE, EXIT, CALCULATE};

struct worker_info {
	int x1, y1, x2, y2;
	pthread_cond_t cond_var;
	pthread_mutex_t mx;
	int action;
}

void* f(void* arg) {
	struct worker_info* info;
	info = (struct worker_info*) arg;
	while (1) {
		pthread_mutex_lock(&info_mtx);
		pthread_cond_wait(&info->mx, &cond_var);
		if (info->action == EXIT) {
			pthread_mutex_unlock(&info->mx);
			pthread_cond_signal(&cond_var);
			return NULL;
		} else if (info->action == CALCULATE) {
			pthread_mutex_unlock(&info->mx);
			info->action = DO_WORK;
			for (int i = x1; i <= x2; ++i) {
				for (int j = y1; j <= y2; ++j) {
					int sum = 0;
					for (int k = 0; k < n; ++k) {
						sum += a[i][k] * b[k][j];
					}
					matrix[i][j] = sum;
				}
			}
			info->action = DONE;
			pthread_cond_signal(master_cond);
		}
	}
}
