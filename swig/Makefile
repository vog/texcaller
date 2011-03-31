LANGUAGES := python ruby php

WRAP_FILES := $(patsubst %,../%/texcaller_wrap.cxx,$(LANGUAGES))

.PHONY: all clean

all: $(WRAP_FILES)

clean:
	rm -f $(WRAP_FILES) ../python/texcaller.py ../php/php_texcaller.h ../php/texcaller.php

../%/texcaller_wrap.cxx: texcaller.i
	swig -$* -c++ -o $@ texcaller.i
