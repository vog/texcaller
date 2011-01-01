VERSION := 0.1

PREFIX := /usr/local
UPLOAD_DEST := www.profv.de:texcaller/

CROSS :=
AR := $(CROSS)ar
CC := $(CROSS)gcc
CFLAGS := -O3 -ansi -pedantic -W -Wall -Werror
TEXCALLER_CFLAGS := -I.build/result/include -L.build/result/lib -ltexcaller

.PHONY: all
all: .build/.done
.build/.done: $(wildcard src/*)
	rm -fr .build
	mkdir -p .build/intermediate .build/result/{bin,include,lib,share/doc/texcaller}
	cp -Rp src/texcaller.h .build/result/include/
	$(CC) $(CFLAGS) -c -o .build/intermediate/texcaller.o src/texcaller.c
	$(AR) crs .build/result/lib/libtexcaller.a .build/intermediate/texcaller.o
	$(CC) $(CFLAGS) -o .build/intermediate/example src/example.c $(TEXCALLER_CFLAGS)
	$(CC) $(CFLAGS) -o .build/result/bin/texcaller src/main.c $(TEXCALLER_CFLAGS)
	mkdir -p .build/result/share/doc/texcaller
	( cat src/Doxyfile; \
	  echo 'PROJECT_NUMBER = $(VERSION)'; \
	  echo 'INPUT = src'; \
	  echo 'EXAMPLE_PATH = src'; \
	  echo 'OUTPUT_DIRECTORY = .build/result/share/doc/texcaller'; \
	) | doxygen -
	touch $@

.PHONY: clean
clean:
	rm -fr .build

.PHONY: install
install: all
	mkdir -p               '$(PREFIX)'
	cp -Rp .build/result/* '$(PREFIX)'/
	mkdir -p               '$(PREFIX)'/lib/pkgconfig
	( echo 'Name: texcaller'; \
	  echo 'Description: texcaller'; \
	  echo 'Version: $(VERSION)'; \
	  echo 'Libs: -L$(PREFIX)/lib -ltexcaller'; \
	  echo 'Cflags: -I$(PREFIX)/include'; \
	) > '$(PREFIX)'/lib/pkgconfig/texcaller.pc

.PHONY: upload-release
upload-release: all
	rm -rf              .build/release
	mkdir -p            .build/release/texcaller-$(VERSION)/
	cp -Rp Makefile src .build/release/texcaller-$(VERSION)/
	cd .build/release && tar -cf - */ | gzip -9 > texcaller-$(VERSION).tar.gz
	rsync -rtvz --delete -f 'protect *.tar*' --chmod=u=rwX,go= \
	    .build/release/*.tar* \
	    .build/result/share/doc/texcaller/ \
	    $(UPLOAD_DEST)
