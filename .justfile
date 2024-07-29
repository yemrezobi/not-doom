build_dir := "./build"
gtest_dir := "/usr/include/gtest"
build_config := "Debug"

build:
    cmake -S . -B {{build_dir}} -G "Ninja Multi-Config" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE
    cmake --build {{build_dir}} --config {{build_config}} --parallel $(lscpu -p=CPU | wc -l)

lint:
    iwyu-tool -j 4 -p {{build_dir}}/compile_commands.json -- -Xiwyu --error -Xiwyu --cxx17ns | sd '^\(.* has correct #includes/fwd-decls\)$\n+' ''
    cppcheck --project={{build_dir}}/compile_commands.json -I{{gtest_dir}} --std=c++20 --language=c++ --enable=all -i {{build_dir}}/_deps --suppress=missingIncludeSystem --error-exitcode=1 --quiet --inline-suppr src

test: build
    #ctest -j 4 --output-on-failure --test-dir {{build_dir}}
    # ctest does not provide a way to pass arguments to test executable
    {{build_dir}}/test/not-doom_test --gtest_color=yes

clean:
    cmake --build {{build_dir}} --target clean
