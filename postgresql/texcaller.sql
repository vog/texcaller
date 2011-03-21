create function
texcaller_convert(source text, source_format text, result_format text, max_runs integer) returns bytea stable strict
language c as '$libdir/texcaller', 'postgresql_texcaller_convert';

create function
texcaller_escape_latex(s text) returns text immutable strict
language c as '$libdir/texcaller', 'postgresql_texcaller_escape_latex';
