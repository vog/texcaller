VERSION := $(shell git describe --always)

PARTS := doc c shell postgresql python
PREFIX := /usr/local
UPLOAD_DEST := www.profv.de:texcaller/

CROSS :=
AR := $(CROSS)ar
CC := $(CROSS)gcc
CXX := $(CROSS)g++
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
	$(CXX) $(CFLAGS) -Isrc -L.build/c -o .build/c/example_cxx src/example.cxx -ltexcaller
	.build/c/example_cxx

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

# postgresql
.build/postgresql.ok: .build/c.ok src/texcaller_postgresql.c src/texcaller.sql
	rm -fr   .build/postgresql
	mkdir -p .build/postgresql
	cp src/texcaller_postgresql.c .build/postgresql/
	( echo 'MODULE_big = texcaller'; \
	  echo 'OBJS := texcaller_postgresql.o'; \
	  echo 'DATA := ../../src/texcaller.sql'; \
	  echo 'PG_CPPFLAGS := -I../../src'; \
	  echo 'SHLIB_LINK := -L../c -ltexcaller'; \
	  echo 'PG_CONFIG := pg_config'; \
	  echo 'PGXS := $$(shell $$(PG_CONFIG) --pgxs)'; \
	  echo 'include $$(PGXS)'; \
	) > .build/postgresql/Makefile
	$(MAKE) -C .build/postgresql
	touch $@

check-postgresql: .build/postgresql.ok
	# ensure that texcaller.so is accessible by PostgreSQL
	$(MAKE) -C .build/postgresql install-lib
	# use the "template1" database because it is always there,
	# rolling back the transaction so the database won't be modified
	( echo 'start transaction;'; \
	  cat src/texcaller.sql; \
	  cat src/example.sql; \
	  echo 'rollback;'; \
	) | psql -v ON_ERROR_STOP=1 -f - template1

install-postgresql: .build/postgresql.ok
	$(MAKE) -C .build/postgresql install

# python
.build/python.ok: src/texcaller.i src/texcaller.h src/texcaller.c
	rm -fr   .build/python
	mkdir -p .build/python
	cd .build/python && swig -python -c++ -outcurrentdir ../../src/texcaller.i
	( echo 'from setuptools import setup, Extension'; \
	  echo ''; \
	  echo "_texcaller = Extension('_texcaller',"; \
	  echo "                       include_dirs = ['../../src'],"; \
	  echo "                       sources = ['texcaller_wrap.cxx', '../../src/texcaller.c'])"; \
	  echo ""; \
	  echo "setup(name = 'texcaller',"; \
	  echo "      version = '$(VERSION)',"; \
	  echo "      ext_modules = [_texcaller],"; \
	  echo "      py_modules = ['texcaller'])"; \
	) > .build/python/setup.py
	cd .build/python && python setup.py build_ext --inplace
	touch $@

check-python: .build/python.ok
	cp src/example.py .build/python/
	cd .build/python && python example.py

install-python: .build/python.ok
	cd .build/python && python setup.py install

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
