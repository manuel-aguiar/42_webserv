#!/usr/bin/bash
clear
c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp -o a.out && valgrind ./a.out
rm -rf vgcore*
rm a.out