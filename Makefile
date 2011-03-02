VERSION := $(shell git describe --always)

PARTS := doc c shell
PREFIX := /usr/local
UPLOAD_DEST := www.profv.de:texcaller/

CROSS :=
AR := $(CROSS)ar
CC := $(CROSS)gcc
INSTALL := $(shell ginstall --help >/dev/null 2>&1 && echo g)install
CFLAGS := -O3 -ansi -pedantic -W -Wall -Werror

.PHONY: build install clean upload-release

.PHONY: $(addprefix build-,$(PARTS))
build:  $(addprefix build-,$(PARTS))
$(foreach PART,$(PARTS),$(eval build-$(PART): .build/$(PART).ok))

.PHONY: $(addprefix check-,$(PARTS))
check:  $(addprefix check-,$(PARTS))

.PHONY:  $(addprefix install-,$(PARTS))
install: $(addprefix install-,$(PARTS))

# doc
.build/doc.ok: $(wildcard src/*)
	rm -fr   .build/doc
	mkdir -p .build/doc
	( cat src/Doxyfile; \
	  echo 'PROJECT_NUMBER = $(VERSION)'; \
	  echo 'INPUT = src'; \
	  echo 'EXAMPLE_PATH = src'; \
	  echo 'OUTPUT_DIRECTORY = .build/doc'; \
	) | doxygen -
	touch $@

check-doc: .build/doc.ok

install-doc: .build/doc.ok
	$(INSTALL) -d '$(PREFIX)'/share/doc/texcaller
	$(INSTALL) -m644 .build/doc/* '$(PREFIX)'/share/doc/texcaller/

# c
.build/c.ok: src/texcaller.c src/texcaller.h
	rm -fr   .build/c
	mkdir -p .build/c
	$(CC) $(CFLAGS) -c -o .build/c/texcaller.o src/texcaller.c
	$(AR) crs .build/c/libtexcaller.a .build/c/texcaller.o
	touch $@

check-c: .build/c.ok
	$(CC) $(CFLAGS) -Isrc -L.build/c -o .build/c/example src/example.c -ltexcaller
	.build/c/example

install-c: .build/c.ok
	( echo 'Name: texcaller'; \
	  echo 'Description: texcaller'; \
	  echo 'Version: $(VERSION)'; \
	  echo 'Libs: -L$(PREFIX)/lib -ltexcaller'; \
	  echo 'Cflags: -I$(PREFIX)/include'; \
	) > .build/c/texcaller.pc
	$(INSTALL) -d '$(PREFIX)'/include
	$(INSTALL) -m644 src/texcaller.h '$(PREFIX)'/include/
	$(INSTALL) -d '$(PREFIX)'/lib/pkgconfig
	$(INSTALL) -m644 .build/c/libtexcaller.a '$(PREFIX)'/lib/
	$(INSTALL) -m644 .build/c/texcaller.pc   '$(PREFIX)'/lib/pkgconfig/

# shell
.build/shell.ok: .build/c.ok src/main.c
	rm -fr   .build/shell
	mkdir -p .build/shell
	$(CC) $(CFLAGS) -L.build/c -o .build/shell/texcaller src/main.c -ltexcaller
	touch $@

check-shell: .build/shell.ok
	cd .build/shell && PATH=".:$$PATH" sh -eu ../../src/example.sh
	[ -s .build/shell/hello.pdf ]

install-shell: .build/shell.ok
	$(INSTALL) -d '$(PREFIX)'/bin
	$(INSTALL) -m755 .build/shell/texcaller '$(PREFIX)'/bin/

# others
clean:
	rm -fr .build

upload-release: .build/doc.ok
	rm -fr .build/release
	$(INSTALL) -d .build/release/texcaller-$(VERSION)/src
	( echo 'VERSION := $(VERSION)'; \
	  tail -n +2 Makefile; \
	) > .build/release/Makefile
	$(INSTALL) -m644 .build/release/Makefile .build/release/texcaller-$(VERSION)/
	$(INSTALL) -m644 src/* .build/release/texcaller-$(VERSION)/src/
	cd .build/release && tar -cf - texcaller-$(VERSION)/ | gzip -9 > texcaller-$(VERSION).tar.gz
	rsync -rtvz --delete -f 'protect *.tar*' --chmod=u=rwX,go= \
	    .build/release/texcaller-$(VERSION).tar.gz \
	    .build/doc/ \
	    $(UPLOAD_DEST)
