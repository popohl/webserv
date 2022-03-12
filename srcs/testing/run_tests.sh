#!/bin/bash

# before running this script, you need to make
# compilation is made using the objs directory and the files contained in testing

gtest_dir='gtest_incs/'
gtest_lib='libgtest.a'

objs='../../objs/parsing/*.o'
tests='goo_test_main.cpp'
incs='../../incs'

clang++ $objs -I$gtest_dir -I$incs -pthread $tests $gtest_dir$gtest_lib
./a.out
rm -f a.out
