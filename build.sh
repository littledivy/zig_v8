export PATH="$PWD/depot_tools:$PATH"

# TODO: sanity checks
# TODO: Building from Source
# fetch v8
# cd v8

# gn gen gn_out/ --args='is_debug=false target_cpu="x64"'
# ninja -C gn_out/

g++ -Wall -W -Wno-unused-parameter -Iv8/include -fPIC -DV8_COMPRESS_POINTERS -g -c -o src/binding.o src/binding.cc
gcc -o libv8c.so src/binding.o -shared -Llibs -lstdc++ -lpthread -lv8 -ldl -lm

