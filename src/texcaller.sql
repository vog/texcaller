create function
texcaller_convert(src text, src_format text, dest_format text, max_runs integer) returns bytea stable strict
language c as '$libdir/texcaller', 'texcaller_postgresql_convert';

create function
texcaller_escape_latex(s text) returns text immutable strict
language c as '$libdir/texcaller', 'texcaller_postgresql_escape_latex';
