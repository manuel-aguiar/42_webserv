#!/usr/bin/bash
clear
rm -rf vgcore*
c++ -Wall -Wextra -Werror -std=c++98 -g test.cpp ../BlockFinder.cpp -o a.out && valgrind ./a.out
rm -rf a.out