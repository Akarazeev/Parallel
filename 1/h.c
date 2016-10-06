#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
	srand(time(NULL));
	FILE* f = fopen("mm.bin", "wb");	
	char* name = "MATRIX";
	fwrite(name, sizeof(char), strlen(name), f);
	// int n = 1000;
	// int m = 1000;
	int n = atoi(argv[1]);
	int m = n;
	int i;
	double tmp;
	fwrite(&n, sizeof(int), 1, f);
	fwrite(&m, sizeof(int), 1, f);
	for (i = 0; i < n * m; ++i) {
		tmp = ((double)rand()/(double)RAND_MAX) * 1000;
		if (i == 0) {
			// printf("tmp= %f\n", tmp);
		}
		fwrite(&tmp, sizeof(double), 1, f);
	}
	fclose(f);

	f = fopen("mm.bin", "rb");
	char* name_readed = malloc(6 * sizeof(char));
	fread(name_readed, sizeof(char), 6, f);
	// puts(name_readed);
	int x_readed;
	fread(&x_readed, sizeof(int), 1, f);
	// printf("%d\n", x_readed);
	fread(&x_readed, sizeof(int), 1, f);
	// printf("%d\n", x_readed);
	double y;
	fread(&y, sizeof(double), 1, f);
	// printf("%f\n", y);
	fclose(f);
	return 0;
}