build_dir := "./build"

build:
    cmake -S . -B {{build_dir}} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
    cmake --build {{build_dir}}

lint:
    iwyu-tool -j 4 -p {{build_dir}}/compile_commands.json src/*.cpp -- -Xiwyu --error | sd '^\(.* has correct #includes/fwd-decls\)$\n+' ''
    cppcheck --project={{build_dir}}/compile_commands.json -I/usr/include/gtest --std=c++20 --language=c++ --enable=all -i {{build_dir}}/_deps --suppress=missingIncludeSystem --error-exitcode=1 --quiet src

test: build
    ctest -j 4 --output-on-failure --test-dir {{build_dir}}

clean:
    cmake --build {{build_dir}} --target clean
