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


#include <assert.h>
#include <stdio.h>
#include <time.h>

#define SHUFFLEP_IMPLEMENTATION
#include "shufflep.h"

#define FYITER_IMPLEMENTATION
#include "fyiter.h"

#define BENCH(msg, setup, body, teardown) do {\
		do setup while(0); \
		s = clock(); \
		for(r = 0; r < repeat; ++r)\
			body \
		e = clock(); \
		do teardown while(0); \
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat; \
		printf(" %-15f", t); \
} while(0)

int
main (void)
{
	size_t n;
	size_t i;
	size_t r;
	size_t const max = 100000000;
	size_t const repeat = 20;
	clock_t s, e;
	double t;
	struct fyiter_ctx fctx;
	struct shufflep_ctx sctx;
	
	printf("%-15s %-15s %-15s %-15s %-15s\n", "N", "FY-init", "shufflep-init", "FY-iter-all", "shufflep-iter-all");

	for(n = 10; n <= max; n*=1.1){
		printf("%-15zu", n);

		BENCH("FY-init",
				{assert(fyiter_new(&fctx, n) == 0);},
				{fyiter_init(&fctx, n);},
				{fyiter_free(&fctx);}
		     );

		BENCH("shufflep-init",
				{},
				{shufflep_init(&sctx, n, rand());},
				{}
		     );

		BENCH("FY-iter-all",
				{assert(fyiter_new(&fctx, n) == 0);},
				{
				for (i = 0; i < n; ++i){
				fyiter_next(&fctx);
				}
				fyiter_reset(&fctx);
				},
				{fyiter_free(&fctx);}
		     );

		BENCH("shufflep-iter-all",
				{shufflep_init(&sctx, n, rand());},
				{
				for (i = 0; i < n; ++i){
				shufflep_index(&sctx, i);
				}
				shufflep_reseed(&sctx, rand());
				},
				{}
		     );

		puts("");
		fflush(stdout);
	}

	return 0;
}
