mkdir out
cd out

cmake  -DCMAKE_CXX_COMPILER="/bin/clang++-10" -GNinja ..
cmake --build .
