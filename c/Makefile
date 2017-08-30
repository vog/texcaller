PREFIX := /usr/local

CROSS :=
AR := $(CROSS)ar
CC := $(CROSS)gcc
CXX := $(CROSS)g++
INSTALL := $(shell ginstall --help >/dev/null 2>&1 && echo g)install
CFLAGS := -O3 -D_DEFAULT_SOURCE -ansi -pedantic -W -Wall -Werror

.PHONY: all check clean install

all: libtexcaller.a
libtexcaller.a: texcaller.c texcaller.h
	$(CC) $(CFLAGS) -c -o texcaller.o texcaller.c
	rm -f libtexcaller.a
	$(AR) crs libtexcaller.a texcaller.o

check: all
	$(CC) $(CFLAGS) -I. -L. -o example example.c -ltexcaller
	./example
	$(CXX) $(CFLAGS) -I. -L. -o example_cxx example.cxx -ltexcaller
	./example_cxx

clean:
	rm -f texcaller.o libtexcaller.a
	rm -f example example_cxx
	rm -f texcaller.pc

install: all
	( echo 'Name: texcaller'; \
	  echo 'Description: texcaller'; \
	  echo 'Version: 0'; \
	  echo 'Libs: -L$(PREFIX)/lib -ltexcaller'; \
	  echo 'Cflags: -I$(PREFIX)/include'; \
	) > texcaller.pc
	$(INSTALL) -d '$(PREFIX)'/include
	$(INSTALL) -m644 texcaller.h '$(PREFIX)'/include/
	$(INSTALL) -d '$(PREFIX)'/lib/pkgconfig
	$(INSTALL) -m644 libtexcaller.a '$(PREFIX)'/lib/
	$(INSTALL) -m644 texcaller.pc   '$(PREFIX)'/lib/pkgconfig/
