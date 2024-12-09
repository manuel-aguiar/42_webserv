#!/usr/bin/bash
clear
c++ -Wall -Wextra -Werror -std=c++98 -g main.cpp ../../Nginx_MemoryPool.cpp ../../Nginx_MPool_Block.cpp -o a.out && valgrind ./a.out
rm -rf vgcore*
rm a.out