/*-
 * Copyright (c) 2020, Angelo Haller
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>

#define SHUFFLE_IMPLEMENTATION
#include "shuffle.h"

int
main (void)
{
	char* list[] = {
		"The",
		"quick,",
		"brown",
		"fox",
		"jumped",
		"over",
		"the",
		"fence",
		"and",
		"was",
		"never",
		"to",
		"be",
		"seen",
		"again.",
	};

	size_t size = sizeof(list)/sizeof(list[0]);

	struct shuffle_ctx ctx;
	shuffle_init(&ctx, size, 0xBAD5EEED);

	size_t i, j, k;
	for (i = 0; i < size; ++i) {
		j = shuffle_index(&ctx, i);
		k = shuffle_index_invert(&ctx, j);
		printf("%2zu %6s   %2zu %6s\n", j, list[j], k, list[k]);
	}

	return 0;
}

