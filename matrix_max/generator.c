//
//  generator.c
//  matrix_max
//
//  Created by <anton.karazeev@gmail.com> on 01/10/16. MIPT.
//  Copyright © 2016 Anton Karazeev. All rights reserved.
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv) {
	srand(time(NULL));

	int n = atoi(argv[1]);
	int m = n;
	int i;
	double tmp;

	FILE* f = fopen("matrix.bin", "wb");

	char* name = "MATRIX";
	fwrite(name, sizeof(char), strlen(name), f);
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

	/* Just test */
	if (1) {
		f = fopen("matrix.bin", "rb");
		char* name_readed = malloc(6 * sizeof(char));
		int x_readed;
		double y;

		fread(name_readed, sizeof(char), 6, f);
		puts(name_readed);

		fread(&x_readed, sizeof(int), 1, f);
		printf("%d\n", x_readed);

		fread(&x_readed, sizeof(int), 1, f);
		printf("%d\n", x_readed);

		fread(&y, sizeof(double), 1, f);
		printf("%f\n", y);
		fclose(f);
	}

	return 0;
}
