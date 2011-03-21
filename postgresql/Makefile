MODULE_big = texcaller
OBJS := texcaller_postgresql.o
DATA := texcaller.sql
PG_CPPFLAGS := -I../c
SHLIB_LINK := ../c/texcaller.c
PG_CONFIG := pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

check: install-lib
	# Depending on "install-lib" to ensure that texcaller.so is
	# accessible by PostgreSQL.
	# Using the "template1" database because it is always there,
	# rolling back the transaction so the database won't be modified.
	( echo 'start transaction;'; \
	  cat texcaller.sql; \
	  cat example.sql; \
	  echo 'rollback;'; \
	) | psql -v ON_ERROR_STOP=1 -f - template1
