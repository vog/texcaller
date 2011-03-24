LANGUAGES := python ruby

WRAP_FILES := $(patsubst %,../%/texcaller_wrap.cxx,$(LANGUAGES))

.PHONY: all clean

all: $(WRAP_FILES)

clean:
	rm -f $(WRAP_FILES) ../python/texcaller.py

../%/texcaller_wrap.cxx: texcaller.i
	swig -$* -c++ -o $@ texcaller.i
