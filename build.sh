mkdir out
cd out

cmake -GNinja -DCMAKE_CXX_COMPILER="/bin/g++-10" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
