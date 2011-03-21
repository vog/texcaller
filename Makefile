VERSION := $(shell git describe --always)
UPLOAD_DEST := www.profv.de:texcaller/

.PHONY: default indep all check clean dist

default: indep
	@echo
	@echo '-----------------------------------------------------------------'
	@echo 'Please change to the directory of the part you are interested in,'
	@echo 'or alternatively run "make all" to build all at once.'
	@echo '-----------------------------------------------------------------'
	@echo

indep:
	$(MAKE) -C doc-mk
	$(MAKE) -C swig

all: indep
	$(MAKE) -C c
	$(MAKE) -C shell
	$(MAKE) -C postgresql
	cd python && python setup.py build_ext --inplace

check: all
	$(MAKE) -C c check
	$(MAKE) -C shell check
	$(MAKE) -C postgresql check
	cd python && python example.py

clean:
	$(MAKE) -C doc-mk clean
	$(MAKE) -C swig clean
	$(MAKE) -C c clean
	$(MAKE) -C shell clean
	$(MAKE) -C postgresql clean
	cd python && rm -fr build dist texcaller.egg-info _texcaller.so texcaller.pyc

dist: all
	@echo 'not yet implemented'
	@exit 1
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
