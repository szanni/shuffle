shufflep
========

A pseudo random permutation generator for array indices — in plain terms, an algorithm to iterate
over a fixed sized array in pseudo random fashion visiting each element exactly once.

Map a set of n integers to another set of n integers in a pseudo random fashion: `[0..n-1]` → `[0..n-1]`


Usage
-----

This is a single-file library so just include the header where needed and specify one file that the implementation resides in:
```c
#define SHUFFLEP_IMPLEMENTATION
#include "shufflep.h"
```

Then just create and initialize a shufflep context:
```c
struct shufflep_ctx ctx;
shufflep_init(&ctx, size, 0xBAD5EEED);
```

And iterate over your data using the random index:
```c
shufflep_index(&ctx, i);
```

Reseed:
```c
shufflep_reseed(&ctx, 0xBAAD5EED);
```

You can also get the original index by running:
```c
shufflep_index_invert(&ctx, ri);
```

Example
-------

Full example, see included [example.c](example.c).


```c
#include <stdio.h>

#define SHUFFLEP_IMPLEMENTATION
#include "shufflep.h"

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

	struct shufflep_ctx ctx;
	shufflep_init(&ctx, size, 0xBAD5EEED);

	size_t i, j, k;
	for (i = 0; i < size; ++i) {
		j = shufflep_index(&ctx, i);
		k = shufflep_index_invert(&ctx, j);
		printf("%2zu %6s   %2zu %6s\n", j, list[j], k, list[k]);
	}

	return 0;
}

```

Which will output:
```
 3    fox    0    The
14 again.    1 quick,
13   seen    2  brown
 9    was    3    fox
10  never    4 jumped
 7  fence    5   over
 8    and    6    the
12     be    7  fence
 5   over    8    and
 4 jumped    9    was
11     to   10  never
 6    the   11     to
 0    The   12     be
 1 quick,   13   seen
 2  brown   14 again.
```

Rationale
---------

Why not use the classic Fisher-Yates (FY) algorithm to create a pseudo random permutation?

Fisher-Yates comes with several constraints: Your data has to be either mutable or you need a ton of extra space.

A full comparison of constraints and complexities can be found in the following table:

|                                 | Classic FY | Inside-out FY | shufflep |
|---------------------------------|------------|---------------|----------|
| Needs mutable data              | Y          | N             | N        |
| Memory                          | O(1)       | O(n)          | O(1)     |
| Runtime                         | O(n)       | O(n)          | O(n)     |
| Runtime to access first element | O(1)       | O(n)          | O(1)     |


Implementation Details
----------------------

**DISCLAIMER: THIS ALGORITHM IS NOT CRYPTOGRAPHICALLY SECURE!**

This algorithm uses format preserving encryption to encrypt (or decrypt) indices.

It does so by using a variable bit block cipher, implemented through a symmetric Feistel network with cyclic walking to ensure the output is within the specified domain.

The average cycle walking distance is 4.

The round function is a very simple 2 pass xorshift-multiply-xorshift hash function.

Key derivation is done by using a sliding window to extract a few bits each round from the provided seed.

Performance
-----------

![performance graph](plot.svg)

Interestingly shufflep seems to outperform the classic Fisher-Yates algorithm with bigger
inputs.

My guess is, that we are essentially profiling the system's `rand()` implementation more
than anything else.

```
Intel(R) Core(TM) i7-8650U CPU @ 1.90GHz
32GiB Memory

Linux 5.4.24-1-lts x86_64
glibc 2.31-2

```

License
-------

ISC license, see [LICENSE](LICENSE).
