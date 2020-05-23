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


#ifndef SHUFFLEP_H
#define SHUFFLEP_H

#include <stdint.h>
#include <limits.h>

struct shufflep_ctx {
	size_t key;
	size_t nitems;
	/* Half block size in bits */
	size_t hbsz;
};

void shufflep_init(struct shufflep_ctx *ctx, size_t nitems, size_t seed);

size_t shufflep_index(struct shufflep_ctx *ctx, size_t index);

size_t shufflep_index_invert(struct shufflep_ctx *ctx, size_t index);

void shufflep_reseed(struct shufflep_ctx *ctx, size_t seed);

#endif


#ifdef SHUFFLEP_IMPLEMENTATION

/* Power of 2! Otherwise change round function sliding window */
#define _SHUFFLEP_ROUNDS 4

enum _shufflep_op {
	_shufflep_op_encrypt,
	_shufflep_op_decrypt
};

/* Calculate needed block cipher size in bits.
 * Block size needs to be even for symmetric feistel cipher and we want a
 * minimum of 4 bits to not degenerate.
 */
static size_t
_shufflep_block_size (size_t n)
{
	/* Number of bits needed to represent n; equal to sizeof(size_t) * CHAR_BITS - clz(n) */
	size_t bsz = 1;
	while ((n >>= 1))
		++bsz;

	if (bsz%2)
		++bsz;

	return (bsz < 4) ? 4 : bsz;
}

void
shufflep_init (struct shufflep_ctx *ctx, size_t nitems, size_t seed)
{
	ctx->key = seed;
	ctx->nitems = nitems;
	ctx->hbsz = _shufflep_block_size(nitems) / 2;
}

/* taken from hash-prospector */
static uint32_t
_shufflep_hash32 (uint32_t x)
{
	x ^= x >> 16;
	x *= UINT32_C(0x7feb352d);
	x ^= x >> 15;
	x *= UINT32_C(0x846ca68b);
	x ^= x >> 16;
	return x;
}

/* splittable64 */
static uint64_t
_shufflep_hash64 (uint64_t x)
{
	x ^= x >> 30;
	x *= UINT64_C(0xbf58476d1ce4e5b9);
	x ^= x >> 27;
	x *= UINT64_C(0x94d049bb133111eb);
	x ^= x >> 31;
	return x;
}

static size_t
_shufflep_hash (size_t x)
{
	switch (sizeof(size_t)) {
		case 4:
			return _shufflep_hash32(x);
		case 8:
			return _shufflep_hash64(x);
	}
}

static size_t
_shufflep_round (struct shufflep_ctx *ctx, size_t r, size_t i)
{
	size_t k_i = ctx->key >> (i*sizeof(ctx->key)*CHAR_BIT/_SHUFFLEP_ROUNDS);
	return _shufflep_hash(r^k_i)^k_i;
}

/* Feistel network to encrypt or decrypt a value. Cycle walk until we have a
 * desired value in target range, see format preserving encryption.
 */
static size_t
_shufflep_cipher (struct shufflep_ctx *ctx, size_t val, enum _shufflep_op op)
{
	size_t mask = (1 << ctx->hbsz) - 1;

	do {

		size_t l, l1, r, r1;
		l = val >> ctx->hbsz;
		r = val & mask;

		size_t i, r_i;
		for (i = 0; i < _SHUFFLEP_ROUNDS; ++i) {
			r_i = (op == _shufflep_op_encrypt) ? i : _SHUFFLEP_ROUNDS-1-i;

			l1 = r;
			r1 = l ^ _shufflep_round(ctx, r, r_i);
			l = l1;
			r = r1 & mask;
		}

		val = (r << ctx->hbsz) | l;

	}
	while (val >= ctx->nitems);

	return val;
}

size_t
shufflep_index (struct shufflep_ctx *ctx, size_t index)
{
	return _shufflep_cipher(ctx, index, _shufflep_op_encrypt);
}

size_t
shufflep_index_invert (struct shufflep_ctx *ctx, size_t index)
{
	return _shufflep_cipher(ctx, index, _shufflep_op_decrypt);
}

void
shufflep_reseed (struct shufflep_ctx *ctx, size_t seed)
{
	ctx->key = seed;
}

#endif

