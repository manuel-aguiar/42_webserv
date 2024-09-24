#!/bin/bash

# Arrays for source files and corresponding program names
files=("mainaligned.cpp" "mainNoPool.cpp" "mainalloc.cpp" "maindealloc.cpp")
programs=("aligned" "nopool" "alloc" "dealloc")

# Number of times to run each program
times=10000

# Compilation loop
for i in "${!files[@]}"; do
  c++ -Wall -Wextra -Werror -std=c++98 -fsanitize=address "${files[i]}" -o "${programs[i]}"
done

# Clear the screen
clear

# Execution loop
for prog in "${programs[@]}"; do
    echo -n "$prog:"
    time ./$prog $times 
    echo ""
    rm -rf $prog
done