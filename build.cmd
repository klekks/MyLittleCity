
mkdir .\builds\distr

cmake -B builds -G "MinGW Makefiles"
cmake --build builds
