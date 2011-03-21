PREFIX := /usr/local

INSTALL := $(shell ginstall --help >/dev/null 2>&1 && echo g)install

.PHONY: all clean install

all: ../doc/index.html
../doc/index.html: Doxyfile ../c/texcaller.h ../swig/texcaller.i $(wildcard ../*/*.c)
	rm -fr ../doc
	( cat Doxyfile; \
	  echo 'INPUT = $^'; \
	  echo 'OUTPUT_DIRECTORY = ../doc'; \
	) | doxygen -

clean:
	rm -fr ../doc

install: all
	$(INSTALL) -d '$(PREFIX)'/share/doc/texcaller
	$(INSTALL) -m644 ../doc/* '$(PREFIX)'/share/doc/texcaller/
