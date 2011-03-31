PHP_ARG_ENABLE(texcaller, for texcaller support,
[  --enable-texcaller          Include texcaller support])

if test "$PHP_TEXCALLER" != no; then
  PHP_REQUIRE_CXX
  PHP_ADD_LIBRARY(stdc++, [], TEXCALLER_SHARED_LIBADD)
  PHP_NEW_EXTENSION(texcaller, texcaller_wrap.cxx, $ext_shared)
  PHP_SUBST(TEXCALLER_SHARED_LIBADD)
fi
