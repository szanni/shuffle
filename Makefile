.PHONY: all plot install dist clean distclean bias

INSTALL ?= install
PREFIX ?= /usr/local
DESTDIR ?=
CC ?= cc
CFLAGS += -O2 -std=c99 -Wall -Wextra -pedantic

PACKAGE = shuffle
VERSION = 0.0.0

DIST = Makefile shuffle.h example.c benchmark.c fyiter.h LICENSE README.md plot.plg plot.svg
DIST += chisqr-bias.c chisqr-bias.R

all:
	@echo 'As this is a header only library there is nothing to be done.'
	@echo 'See `make example`, `make stats`, `make plot`, `make bias`, and `make install`.'

benchmark: benchmark.c shuffle.h fyiter.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

example: example.c shuffle.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

stats: benchmark
	./$< > $@

plot.svg: plot.plg stats
	gnuplot -p plot.plg

chisqr-bias: chisqr-bias.c shuffle.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

bias: chisqr-bias chisqr-bias.R
	./chisqr-bias | Rscript chisqr-bias.R

plot: plot.svg

install: shuffle.h
	$(INSTALL) -D -m644 shuffle.h "$(DESTDIR)$(PREFIX)/include/shuffle.h"

dist: $(DIST)
	mkdir -p $(PACKAGE)-$(VERSION)
	cp $(DIST) $(PACKAGE)-$(VERSION)
	tar -cf $(PACKAGE)-$(VERSION).tar $(PACKAGE)-$(VERSION)
	gzip -fk $(PACKAGE)-$(VERSION).tar
	xz -f $(PACKAGE)-$(VERSION).tar

clean:
	rm -f example benchmark stats chisqr-bias

distclean: clean
	rm -rf $(PACKAGE)-$(VERSION){,.tar.gz,.tar.xz}
