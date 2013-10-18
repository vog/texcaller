VERSION = $(shell git describe --always)
UPLOAD_DEST := ../texcaller-website/

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
	cd ruby && ruby extconf.rb
	$(MAKE) -C ruby
	[ -e php/Makefile ] || { cd php && phpize && ./configure ; }
	$(MAKE) -C php

check: all
	$(MAKE) -C c check
	$(MAKE) -C shell check
	$(MAKE) -C postgresql check
	cd python && python example.py
	cd ruby && ruby example.rb
	$(MAKE) -C php test NO_INTERACTION=1 PHP_TEST_SHARED_EXTENSIONS='-n -d extension=texcaller.so'

clean:
	$(MAKE) -C doc-mk clean
	$(MAKE) -C swig clean
	$(MAKE) -C c clean
	$(MAKE) -C shell clean
	$(MAKE) -C postgresql clean
	cd python && rm -fr build dist texcaller.egg-info _texcaller.so texcaller.pyc
	[ ! -e ruby/Makefile ] || $(MAKE) -C ruby clean
	cd ruby && rm -fr Makefile texcaller.c
	cd php && phpize --clean
	rm -fr release

dist:
	rm -fr release
	mkdir -p release/texcaller-$(VERSION)
	git checkout-index -a --prefix=release/texcaller-$(VERSION)/
	$(MAKE) -C release/texcaller-$(VERSION) indep
	cd release && tar -cf - texcaller-$(VERSION)/ | gzip -9 > texcaller-$(VERSION).tar.gz
	@echo
	@echo '---------------------------------------------------------------'
	@echo 'Upload will start in 5 seconds. Last chance to cancel! (Ctrl+C)'
	@echo '---------------------------------------------------------------'
	@echo
	@sleep 5
	rsync -rtvz --delete -f 'protect *.tar*' -f 'protect .git' --chmod=u=rwX,go= \
	    release/texcaller-$(VERSION).tar.gz \
	    release/texcaller-$(VERSION)/doc/ \
	    $(UPLOAD_DEST)
	x-www-browser \
	    'http://vog.github.io/texcaller/download.html' \
	    'http://freshmeat.net/projects/texcaller/releases/new'

