PRGM=main

CC=gcc
IDIR=include/.
CFLAGS=-Wall -Wextra -Wpedantic -Werror -fsanitize=address,leak,undefined -std=c2x -O2

.PHONY: build clean cleanbin cleanobj expand

build: $(PRGM) cleanobj

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -I$(IDIR)

$(PRGM): $(PRGM).o
	$(CC) -o $@ $< $(CFLAGS)


clean: cleanbin cleanobj

cleanbin:
	rm -f $(PRGM)

cleanobj:
	rm -f $(PRGM).o

expand:
	$(CC) -E $(PRGM).c $< $(CFLAGS) -I$(IDIR) -undef -P -C | sed -n '/<main::start>/,/<main::end>/p'
