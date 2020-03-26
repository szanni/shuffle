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


#ifndef FYITER_H
#define FYITER_H

#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>

struct fyiter_ctx {
	size_t size;
	size_t *data;
	size_t i;
};

void fyiter_init(struct fyiter_ctx *ctx, size_t size);
int fyiter_new(struct fyiter_ctx *ctx, size_t size);
size_t fyiter_next(struct fyiter_ctx *ctx);
void fyiter_reset(struct fyiter_ctx *ctx);
void fyiter_free(struct fyiter_ctx *ctx);

#endif

#ifdef FYITER_IMPLEMENTATION

void
fyiter_init(struct fyiter_ctx *ctx, size_t size)
{
	ctx->size = size;
	ctx->i = size - 1;

	size_t i;
	for(i = 0; i < size; ++i)
		ctx->data[i] = i;
}

int
fyiter_new(struct fyiter_ctx *ctx, size_t size)
{
	ctx->data = malloc(size * sizeof(*ctx->data));
	if (ctx->data == NULL)
		return -1;

	fyiter_init(ctx, size);

	return 0;
}

/* Do not call more than size times!
 * Call reset after size calls!
 */
size_t
fyiter_next(struct fyiter_ctx *ctx)
{

	size_t j = rand() % (ctx->i + 1);
	if (j != ctx->i) {
		size_t tmp = ctx->data[j];
		ctx->data[j] = ctx->data[ctx->i];
		ctx->data[ctx->i] = tmp;
	}

	return ctx->data[ctx->i--];
}

void
fyiter_reset(struct fyiter_ctx *ctx)
{
	ctx->i = ctx->size - 1;
}

void
fyiter_free(struct fyiter_ctx *ctx)
{
	free(ctx->data);
}

#endif

