PREFIX := /usr/local

CROSS :=
CC := $(CROSS)gcc
INSTALL := $(shell ginstall --help >/dev/null 2>&1 && echo g)install
CFLAGS := -O3 -D_GNU_SOURCE -ansi -pedantic -W -Wall -Werror

.PHONY: all check clean install

all: texcaller
texcaller: main.c ../c/texcaller.c ../c/texcaller.h
	$(CC) $(CFLAGS) -I../c -o texcaller main.c ../c/texcaller.c

check: all
	PATH=".:$$PATH" sh -eu ./example.sh
	[ -s hello.pdf ]

clean:
	rm -f texcaller
	rm -f hello.tex hello.pdf

install: all
	$(INSTALL) -d '$(PREFIX)'/bin
	$(INSTALL) -m755 texcaller '$(PREFIX)'/bin/
