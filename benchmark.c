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
	puts("# " msg);\
	printf("# %-10s %s\n", "Size", "Time (sec)"); \
	for(n = 1; n <= max; n<<=step){ \
		do setup while(0); \
		s = clock(); \
		for(r = 0; r < repeat; ++r)\
			body \
		e = clock(); \
		do teardown while(0); \
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat; \
		printf("  %-10zu %f\n", n, t); \
	} \
	puts("");\
} while(0)

int
main (void)
{
	size_t n;
	size_t i;
	size_t r;
	size_t const max = 1000000000;
	size_t const repeat = 20;
	size_t const step = 1;
	clock_t s, e;
	double t;
	struct fyiter_ctx fctx;
	struct shufflep_ctx sctx;
	
	BENCH("fy init",
		{assert(fyiter_new(&fctx, n) == 0);},
		{fyiter_init(&fctx, n);},
		{fyiter_free(&fctx);}
	);
	
	BENCH("shufflep init",
		{},
		{shufflep_init(&sctx, n, rand());},
		{}
	);

	BENCH("fy iter all elements",
		{assert(fyiter_new(&fctx, n) == 0);},
		{
			for (i = 0; i < n; ++i){
				fyiter_next(&fctx);
			}
			fyiter_reset(&fctx);
		},
		{fyiter_free(&fctx);}
	);
		
	BENCH("shufflep iter all elements",
		{shufflep_init(&sctx, n, rand());},
		{
			for (i = 0; i < n; ++i){
				shufflep_index(&sctx, i);
			}
			shufflep_reseed(&sctx, rand());
		},
		{}
	);

	/*
	for(n = 1; n <= max; n<<=step){
		struct fyiter_ctx ctx;
		assert(fyiter_new(&fctx, n) == 0);

		s = clock();
		for(r = 0; r < repeat; ++r)
			fyiter_init(&fctx, n);

		e = clock();
		fyiter_free(&fctx);
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat;
		printf("Size: %10u Time: %f\n", n, t);
	}
	
	for(n = 1; n <= max; n<<=step){

		s = clock();
		for(r = 0; r < repeat; ++r)
			shufflep_init(&sctx, n, rand());

		e = clock();
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat;
		printf("Size: %10u Time: %f\n", n, t);
	}
	
	for(n = 1; n <= max; n<<=step){
		assert(fyiter_new(&fctx, n) == 0);

		s = clock();
		for (r = 0; r < repeat; ++r){
			for (i = 0; i < n; ++i){
				fyiter_next(&fctx);
			}
			fyiter_reset(&fctx);
		}

		e = clock();
		fyiter_free(&fctx);
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat;
		printf("Size: %10u Time: %f\n", n, t);
	}
	

	for(n = 1; n <= max; n<<=step){
		shufflep_init(&sctx, n, rand());

		s = clock();
		for(r = 0; r < repeat; ++r){
			for (i = 0; i < n; ++i){
				shufflep_index(&sctx, i);
			}
			shufflep_reseed(&sctx, rand());
		}

		e = clock();
		t = 1.0f * (e - s) / CLOCKS_PER_SEC / repeat;
		printf("Size: %10u Time: %f\n", n, t);
	}
	*/
}
