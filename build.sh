rm -rf ./out/build/debug
mkdir -p out/build/debug/
cmake -S . -B ./out/build/debug -DCMAKE_CXX_COMPILER="/bin/clang++" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_BUILD_PARALLEL_LEVEL=3
