require 'ftools'
require 'mkmf'
File.copy('../c/texcaller.c', '.')
if !have_library('stdc++')
    puts 'Missing C++ runtime'
    exit 1
end
if !File.exists?('texcaller_wrap.cxx')
    puts 'Missing file texcaller_wrap.cxx'
    exit 1
end
if !find_header('texcaller.h', '../c')
    puts 'Missing file ../c/texcaller.h'
    exit 1
end
create_makefile('texcaller')
