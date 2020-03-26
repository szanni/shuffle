.PHONY: all plot install dist clean distclean

INSTALL ?= install
PREFIX ?= /usr/local
DESTDIR ?=
CC ?= cc
CFLAGS += -O2 -std=c99 -Wall -Wextra -pedantic

PACKAGE = shufflep
VERSION = 0.0.0

DIST = Makefile shufflep.h example.c benchmark.c fyiter.h LICENSE README.md plot.plg plot.svg

all:
	@echo 'As this is a header only library there is nothing to be done.'
	@echo 'See `make example`, `make benchmark`, `make stats`, `make plot`, and `make install`.'

benchmark: benchmark.c shufflep.h fyiter.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

example: example.c shufflep.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

stats: benchmark
	./$< > $@

plot.svg: plot.plg stats
	gnuplot -p plot.plg

plot: plot.svg

install: shufflep.h
	$(INSTALL) -D -m644 shufflep.h "$(DESTDIR)$(PREFIX)/include/shufflep.h"

dist: $(DIST)
	mkdir -p $(PACKAGE)-$(VERSION)
	cp $(DIST) $(PACKAGE)-$(VERSION)
	tar -cf $(PACKAGE)-$(VERSION).tar $(PACKAGE)-$(VERSION)
	gzip -fk $(PACKAGE)-$(VERSION).tar
	xz -f $(PACKAGE)-$(VERSION).tar

clean:
	rm -f example benchmark stats

distclean: clean
	rm -rf $(PACKAGE)-$(VERSION){,.tar.gz,.tar.xz}
