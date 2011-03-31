require 'ftools'
require 'mkmf'
if !have_library('stdc++')
    puts 'Missing C++ runtime'
    exit 1
end
if !File.exists?('texcaller_wrap.cxx')
    puts 'Missing file texcaller_wrap.cxx'
    exit 1
end
create_makefile('texcaller')
