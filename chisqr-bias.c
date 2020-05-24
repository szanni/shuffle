#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SHUFFLE_IMPLEMENTATION
#include "shuffle.h"

int
main(void)
{
	const unsigned n = 1000;
	const unsigned k = n*100;
	uint16_t O[n][n];

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

