EXTENSION := texcaller
MODULE_big := texcaller
OBJS := texcaller_postgresql.o
DATA_built := texcaller--0.sql
PG_CONFIG := pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

texcaller--0.sql: texcaller.sql
	cp $< $@

check: install
	# Depending on "install" to ensure that texcaller.so is
	# accessible by PostgreSQL.
	# Using the "template1" database because it is always there,
	# rolling back the transaction so the database won't be modified.
	( echo 'start transaction;'; \
	  echo 'create extension texcaller;'; \
	  cat example.sql; \
	  echo 'rollback;'; \
	) | psql -v ON_ERROR_STOP=1 -f - template1
