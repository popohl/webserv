#!/bin/bash

gtest_dir='gtest_incs/'
gtest_lib='libgtest.a'

objs='../../objs/parsing/*.o'
tests='goo_test_main.cpp'
incs='../../incs'

clang++ $objs -I$gtest_dir -I$incs -pthread $tests $gtest_dir$gtest_lib
./a.out
rm -f a.out
