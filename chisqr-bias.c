#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SHUFFLE_IMPLEMENTATION
#include "shuffle.h"

int
table(unsigned n)
{
	uint16_t O[n][n];
	const unsigned k = n*1000;

	struct shuffle_ctx ctx;
	shuffle_init(&ctx, n, 0);

	unsigned i, j;
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			O[i][j] = 0;

	for (i = 0; i < k; ++i) {
		shuffle_reseed(&ctx, rand());
		for (j = 0; j < n; ++j)
			O[j][shuffle_index(&ctx, j)] += 1;
	}

	for (i = 0; i < n; ++i) {
		for (j = 0; j < n; ++j) {
			printf("%d ", O[i][j]);
		}
		puts("");
	}

	return 0;
}

int
main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s SIZE\n", argv[0]);
	}

	unsigned n;
	sscanf(argv[1], "%u", &n);

	return table(n);
}

